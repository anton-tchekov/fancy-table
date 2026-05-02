#define I2S_BCK  16
#define I2S_DOUT 21
#define I2S_WS   17

#define BIT_ENABLE 0
#define BIT_X_STEP 1
#define BIT_X_DIR  2
#define BIT_Z_STEP 3
#define BIT_Z_DIR  4
#define BIT_Y_STEP 5
#define BIT_Y_DIR  6

static uint32_t expander_state = 0;

static void i2s_send32(uint32_t word)
{
	for (int i = 31; i >= 0; --i)
	{
		digitalWrite(I2S_BCK, LOW);
		digitalWrite(I2S_DOUT, (word >> i) & 1);
		digitalWrite(I2S_BCK, HIGH);
	}

	digitalWrite(I2S_WS, HIGH);
	delayMicroseconds(1);
	digitalWrite(I2S_WS, LOW);
}

static void i2s_set_bit(int bit, bool v)
{
	if(v)
	{
		expander_state |= (1U << bit);
	}
	else
	{
		expander_state &= ~(1U << bit);
	}

	i2s_send32(expander_state);
}

static void pulse_step(int bit, unsigned int pulse_us)
{
	i2s_set_bit(bit, true);
	delayMicroseconds(pulse_us);
	i2s_set_bit(bit, false);
}

void stepper_init(void)
{
	pinMode(I2S_BCK, OUTPUT);
	pinMode(I2S_DOUT, OUTPUT);
	pinMode(I2S_WS, OUTPUT);
	digitalWrite(I2S_BCK, LOW);
	digitalWrite(I2S_WS, LOW);
	i2s_send32(expander_state);
}

void stepper_set_dir(int stepper, bool dir)
{
	i2s_set_bit(stepper, dir);
}

void stepper_step(int stepper, unsigned int delay)
{
	pulse_step(stepper, 800);
}