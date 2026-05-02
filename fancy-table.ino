#define I2S_BCK  16
#define I2S_DOUT 21
#define I2S_WS   17

uint32_t expanderState = 0;

// Bit indices (example)
#define BIT_ENABLE 0
#define BIT_X_STEP 1
#define BIT_X_DIR  2
#define BIT_Z_STEP 3
#define BIT_Z_DIR  4
#define BIT_Y_STEP 5
#define BIT_Y_DIR  6

void i2sSend32(uint32_t word)
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

void setBit(int bit, bool v)
{
	if (v) expanderState |= (1u << bit);
	else expanderState &= ~(1u << bit);
	i2sSend32(expanderState);
}

void pulseStep(int bit, unsigned int pulseUs = 5)
{
	setBit(bit, true);
	delayMicroseconds(pulseUs);
	setBit(bit, false);
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Hello World from MKS DLC32!\n");

	pinMode(I2S_BCK, OUTPUT);
	pinMode(I2S_DOUT, OUTPUT);
	pinMode(I2S_WS, OUTPUT);
	digitalWrite(I2S_BCK, LOW);
	digitalWrite(I2S_WS, LOW);
	i2sSend32(expanderState);
}

// Y Motor : Rotation - 10:1 - 1600 Steps
// X Motor : Distance - 4.7 cm / 1600 steps

void loop()
{

	/*setBit(BIT_X_DIR, true);
	for (int i = 0; i < 1600; ++i) {
		pulseStep(BIT_X_STEP, 800);
		delayMicroseconds(800);
	}
	delay(500);*/

	static char buf[16];
	static int i = 0;

	if(Serial.available())
	{
		int c = Serial.read();
		if(c == '\n')
		{
			buf[i] = '\0';
			i = 0;
			if(!strcmp(buf, "y+"))
			{
				setBit(BIT_Y_DIR, true);
				for (int i = 0; i < 1600; ++i) {
					pulseStep(BIT_Y_STEP, 800);
					delayMicroseconds(800);
				}
			}
			if(!strcmp(buf, "y-"))
			{
				setBit(BIT_Y_DIR, false);
				for (int i = 0; i < 1600; ++i) {
					pulseStep(BIT_Y_STEP, 800);
					delayMicroseconds(800);
				}
			}
			if(!strcmp(buf, "x+"))
			{
				setBit(BIT_X_DIR, true);
				for (int i = 0; i < 1600; ++i) {
					pulseStep(BIT_X_STEP, 800);
					delayMicroseconds(800);
				}
			}
			if(!strcmp(buf, "x-"))
			{
				setBit(BIT_X_DIR, false);
				for (int i = 0; i < 1600; ++i) {
					pulseStep(BIT_X_STEP, 800);
					delayMicroseconds(800);
				}
			}
		}
		else
		{
			buf[i++] = c;
		}
	}
}
