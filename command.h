
bool parse_int(char *str, long *out)
{
	char *end;
	long val = strtol(str, &end, 10);
	if((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
		(errno != 0 && val == 0))
	{
		return false;
	}

	if(end == str)
	{
		return false;
	}

	*out = val;
	return true;
}

void err_overflow(void)
{
	Serial.println("Error: Stack underflow");
}

void err_underflow(void)
{
	Serial.println("Error: Stack underflow");
}

void process_token(char *buf)
{
	long val;
	if(parse_int(buf, &val))
	{
		if(!stack_push(val))
		{
			err_overflow();
			return;
		}
	}

	if(!strcmp(buf, "zero"))
	{
		set_zero();
		Serial.println("Set current position as zero");
		return;
	}

	if(!strcmp(buf, "move"))
	{
		long radius = 0;
		if(!stack_pop(&radius))
		{
			err_underflow();
			return;
		}

		long angle_deg = 0;
		if(!stack_pop(&angle_deg))
		{
			err_underflow();
			return;
		}

		Serial.printf("Moving to %d deg, %d mm\n", angle_deg, radius);

		move_to(angle_deg / 180.0 * M_PI, radius);
		return;
	}

	if(!strcmp(buf, "delay"))
	{
		long val = 0;
		if(!stack_pop(&val))
		{
			err_underflow();
			return;
		}

		step_delay_us = val;
		Serial.printf("Set Step Delay to %d microseconds\n", step_delay_us);
		return;
	}

	if(!strcmp(buf, "y+"))
	{
		long num_steps = 0;
		if(!stack_pop(&num_steps))
		{
			err_underflow();
			return;
		}

		Serial.printf("Moving Y Motor in Positive Direction for %d Steps\n", num_steps);

		for(int i = 0; i < num_steps; ++i)
		{
			step_y(true);
		}

		return;
	}

	if(!strcmp(buf, "y-"))
	{
		long num_steps = 0;
		if(!stack_pop(&num_steps))
		{
			err_underflow();
			return;
		}

		Serial.printf("Moving Y Motor in Negative Direction for %d Steps\n", num_steps);

		for(int i = 0; i < num_steps; ++i)
		{
			step_y(false);
		}

		return;
	}

	if(!strcmp(buf, "x+"))
	{
		long num_steps = 0;
		if(!stack_pop(&num_steps))
		{
			err_underflow();
			return;
		}

		Serial.printf("Moving X Motor in Positive Direction for %d Steps\n", num_steps);

		for(int i = 0; i < num_steps; ++i)
		{
			step_x(true);
		}

		return;
	}

	if(!strcmp(buf, "x-"))
	{
		long num_steps = 0;
		if(!stack_pop(&num_steps))
		{
			err_underflow();
			return;
		}

		Serial.printf("Moving X Motor in Negative Direction for %d Steps\n", num_steps);

		for(int i = 0; i < num_steps; ++i)
		{
			step_x(false);
		}

		return;
	}
}
