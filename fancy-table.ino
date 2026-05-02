#include "stepper.h"
#include "stack.h"
#include "command.h"
#include <string.h>

void setup()
{
	stepper_init();

	Serial.begin(115200);
	Serial.println("Hello World from MKS DLC32!\n");
}

void loop()
{
	static char buf[64];
	static int i = 0;

	if(Serial.available())
	{
		int c = Serial.read();
		if(c == '\n' || c == ' ')
		{
			buf[i] = '\0';
			i = 0;
			process_token(buf);
		}
		else
		{
			buf[i++] = c;
		}
	}
}
