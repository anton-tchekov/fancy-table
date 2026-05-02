static int cur_pos_x = 0;
static int cur_pos_y = 0;
static int y_steps_since_extra = 0;
static bool cur_x_dir = false;
static bool cur_y_dir = false;
static int step_delay_us = 200;

#define MAX_Y_STEPS 16000

// X and Y in mm (-300 to +300)
// A (angle) for Y motor pos
// R (radius) for X motor pos
void cartesian_to_polar_steps(double x, double y, double *out_a, double *out_r)
{
	*out_r = sqrt(x * x + y * y);
	*out_a = atan2(y, x);
}

double motor_y_steps(double a)
{
	return a / (2.0 * M_PI) * 16000.0;
}

double motor_x_steps(double r)
{
	return r / 47.0 * 1600.0;
}

void step_x_untracked(bool dir)
{
	if(cur_x_dir != dir)
	{
		stepper_set_dir(BIT_X_DIR, dir);
		cur_x_dir = dir;
	}

	stepper_step(BIT_X_STEP, step_delay_us);
	delayMicroseconds(step_delay_us);
}

void step_x(bool dir)
{
	step_x_untracked(dir);
	if(dir)
	{
		cur_pos_x++;
	}
	else
	{
		cur_pos_x--;
	}
}

void step_y(bool dir)
{
	if(cur_y_dir != dir)
	{
		stepper_set_dir(BIT_Y_DIR, dir);
		cur_y_dir = dir;
	}

	stepper_step(BIT_Y_STEP, step_delay_us);
	delayMicroseconds(step_delay_us);
	if(dir)
	{
		cur_pos_y++;
		if(cur_pos_y >= MAX_Y_STEPS)
		{
			cur_pos_y = 0;
		}

		y_steps_since_extra++;
	}
	else
	{
		cur_pos_y--;
		if(cur_pos_y < 0)
		{
			cur_pos_y = MAX_Y_STEPS - 1;
		}

		y_steps_since_extra--;
	}

	if(y_steps_since_extra >= 10)
	{
		step_x_untracked(true);
		y_steps_since_extra = 0;
	}
	else if(y_steps_since_extra <= -10)
	{
		step_x_untracked(false);
		y_steps_since_extra = 0;
	}
}

void set_zero(void)
{
	cur_pos_x = 0;
	cur_pos_y = 0;
}

double safe_div(int a, int b)
{
	if(b == 0)
	{
		return 0.0;
	}

	return a / (double)b;
}

void move_to(double angle, double radius)
{
	angle = fmod(angle, 2.0 * M_PI);
	if(angle < 0.0)
	{
		angle += 2.0 * M_PI;
	}

	int target_x = motor_x_steps(radius);
	int target_y = motor_y_steps(angle);

	int diff_x = target_x - cur_pos_x;
	int diff_y = target_y - cur_pos_y;

	if(diff_y > MAX_Y_STEPS / 2)
	{
		diff_y = diff_y - MAX_Y_STEPS;
	}
	else if(diff_y < -MAX_Y_STEPS / 2)
	{
		diff_y = MAX_Y_STEPS + diff_y;
	}

	int steps_x = abs(diff_x);
	int steps_y = abs(diff_y);

	int max_steps = max(steps_x, steps_y);
	bool x_main = (steps_x == max_steps);
	double ratio = x_main ? safe_div(steps_y, steps_x) : safe_div(steps_x, steps_y);

	int other_count = 0;
	for(int i = 0; i < max_steps; ++i)
	{
		if(x_main)
		{
			step_x(diff_x > 0);
		}
		else
		{
			step_y(diff_y > 0);
		}

		double error = ratio * i - other_count;
		if(error >= 1.0)
		{
			if(x_main)
			{
				step_y(diff_y > 0);
			}
			else
			{
				step_x(diff_x > 0);
			}

			other_count ++;
		}
	}
}

void center(void)
{
	for(int i = 0; i < 7 * 1600; ++i)
	{
		step_x(false);
	}

	set_zero();
}
