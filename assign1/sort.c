#include <stdio.h>

int main()
{
	int i,j;
	char temp[2];
	struct data
	{
		char state[4];
	};
	struct data dataEntries[10];
	struct data dataInput;
	int used=0;
	while (scanf("%s",dataInput.state)!= EOF
		&&used<10)
	{ 
	 strcpy(dataEntries[used].state,dataInput.state);
	 used++; 

	}
	for (i = 0; i < used -1 ;++i)
	{
		for (j= i + 1; j < used;++j)
		{
			if (strcmp(dataEntries[i].state,dataEntries[j].state)> 0)
			{
				strcpy(temp, dataEntries[i].state);
				strcpy(dataEntries[i].state, dataEntries[j].state);
				strcpy(dataEntries[j].state, temp);
			}
		}
	} 
	int integer;
	for (integer=0; integer<used;++integer)
	{
		printf("%s\n",dataEntries[integer].state);
	}

}
