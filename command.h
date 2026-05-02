static int step_delay_us = 800;

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

	if(!strcmp(buf, "delay"))
	{
		long val = 0;
		if(!stack_pop(&val))
		{
			err_underflow();
			return;
		}

		step_delay_us = val;
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

		stepper_set_dir(BIT_Y_DIR, true);
		for(int i = 0; i < num_steps; ++i)
		{
			stepper_step(BIT_Y_STEP, step_delay_us);
			delayMicroseconds(step_delay_us);
		}
	}

	if(!strcmp(buf, "y-"))
	{
		long num_steps = 0;
		if(!stack_pop(&num_steps))
		{
			err_underflow();
			return;
		}

		stepper_set_dir(BIT_Y_DIR, false);
		for(int i = 0; i < num_steps; ++i)
		{
			stepper_step(BIT_Y_STEP, step_delay_us);
			delayMicroseconds(step_delay_us);
		}
	}

	if(!strcmp(buf, "x+"))
	{
		long num_steps = 0;
		if(!stack_pop(&num_steps))
		{
			err_underflow();
			return;
		}

		stepper_set_dir(BIT_X_DIR, true);
		for(int i = 0; i < num_steps; ++i)
		{
			stepper_step(BIT_X_STEP, step_delay_us);
			delayMicroseconds(step_delay_us);
		}
	}

	if(!strcmp(buf, "x-"))
	{
		long num_steps = 0;
		if(!stack_pop(&num_steps))
		{
			err_underflow();
			return;
		}

		stepper_set_dir(BIT_X_DIR, false);
		for(int i = 0; i < num_steps; ++i)
		{
			stepper_step(BIT_X_STEP, step_delay_us);
			delayMicroseconds(step_delay_us);
		}
	}
}
