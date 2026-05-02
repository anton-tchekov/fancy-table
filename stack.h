#define STACK_SIZE 16

static int top = 0;
static long stack[STACK_SIZE];

bool stack_push(long val)
{
	if(top >= STACK_SIZE)
	{
		return false;
	}

	stack[top++] = val;
	return true;
}

bool stack_pop(long *val)
{
	if(top == 0)
	{
		return false;
	}

	*val = stack[--top];
	return true;
}
