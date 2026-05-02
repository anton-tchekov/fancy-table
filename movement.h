static int cur_pos_x = 0;
static int cur_pos_y = 0;
static int y_steps_since_extra = 0;
static bool cur_x_dir = false;
static bool cur_y_dir = false;
static int step_delay_us = 800;

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
		y_steps_since_extra++;
	}
	else
	{
		cur_pos_y--;
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
