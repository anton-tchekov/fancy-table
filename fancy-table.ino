#include "stepper.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("Hello World from MKS DLC32!\n");
}

void loop()
{
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
				for (int i = 0; i < 1600; ++i) {
					stepper_step(BIT_Y_STEP, 800);
					delayMicroseconds(800);
				}
			}
			if(!strcmp(buf, "y-"))
			{
				stepper_set_dir(BIT_Y_DIR, false);
				for (int i = 0; i < 1600; ++i) {
					stepper_step(BIT_Y_STEP, 800);
					delayMicroseconds(800);
				}
			}
			if(!strcmp(buf, "x+"))
			{
				stepper_set_dir(BIT_X_DIR, true);
				for (int i = 0; i < 1600; ++i) {
					stepper_step(BIT_X_STEP, 800);
					delayMicroseconds(800);
				}
			}
			if(!strcmp(buf, "x-"))
			{
				stepper_set_dir(BIT_X_DIR, false);
				for (int i = 0; i < 1600; ++i) {
					stepper_step(BIT_X_STEP, 800);
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
