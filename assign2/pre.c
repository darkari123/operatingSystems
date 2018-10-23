#include <stdio.h>

int main()
{
	struct data
	{
		char state[4];
		int pop;
	};
	struct data dataEntries[10];
	struct data dataInput;
	int used=0;
	while (scanf("%s %d",dataInput.state, &dataInput.pop)!= EOF
		&&used!=10)
	{ if (dataInput.pop >= 10)
		{
		  dataEntries[used]= dataInput;
		  used++;
		}

	}
	int integer;
	for (integer=0; integer<used;++integer)
	{
		printf("%s\n",dataEntries[integer].state);
	}

}
