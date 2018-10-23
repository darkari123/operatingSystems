#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[])
{
	int integer;	
	for (integer=0; integer < argc -1; ++integer)
		strcpy(argv[integer], argv[integer+1]);
	argv[argc -1] = (char*)0;
	
	int pid;
	
	pid = fork();

	if (pid < 0)
		exit(-1);
	else if (pid ==0)
	{
		printf("Child PID: %d\nParent PID : %d\n", getpid(), getppid());
		execvp(argv[0], argv);
	}
	else
		wait(0);

	return 0;
}
