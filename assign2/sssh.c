#include <fcntl.h>  // open
#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <string.h> // strcmp
#include <unistd.h> // fork, pipe, dup
#include <sys/wait.h>  // wait


// Write = 1 , read = 0

int main()
{
    const int MaxCommandLength = 20;
    const int MaxBufferLength = 80;

    char *path,                     
         *argv[MaxCommandLength],   // The separated components of the command
         buf[MaxBufferLength],      // Command buffer
         n,                         // The current character being read
         *p;                        // Pointer for the buffer
    
    int m,          // Index for building argv
        status,     // Used for wait()
        inword,     // Whether in the middle of a word, for building argv
        proceed;    // Whether to continue reading chars to build argv

    // Main loop, terminates when 'exit' is entered
    while (1)
    {
        inword = 0;
        p = buf;
        m = 0;
        proceed = 0;

        printf ("\nshhh>");

        // Build argv (commands, options, and sentinels)
        while ((n = getchar()) != '\n' || proceed)
        {
            if (n == ' ')
            {
                if (inword)
                {
                    inword = 0;
                    *(p++) = 0;
                }
            }
            else if (n == '\n') { proceed = 0; }
            else if (n == '\\' && !inword) { proceed = 1; }
            else
            {
                if (!inword)
                {
                    inword = 1;
                    argv[m++] = p;
                    *(p++) = n;
                }
                else { *(p++) = n; }
            }
        }
        *(p++) = 0;
        argv[m] = 0;

        // Handle special case: exit
        if (strcmp(argv[0], "exit") == 0) { exit(0); }

        // Find, take note of, and remove any '|', '<', or '>'
        int pipeCount = 0,  // The number of pipes in argv
            pipes[10],      // The locations of pipes in argv
            inloc = -1,     // The location of the input file if any
            outloc = -1,    // The location of the output file if any
            cmdCount = 0,   // The number of commands in argv
            cmds[10];       // The starting locations of commands in argv

        // The first command is at the beginning of argv
        cmds[cmdCount++] = 0;
        
        // Check subsequent indices of argv
	int i;
        for (i = 1; i < m; ++i)
        {
            // Pipe, = 0 means they are equal
            if (strcmp(argv[i], "|") == 0) 
            {
                pipes[pipeCount++] = i;
                argv[i] = 0;
                // Command must follow pipe
                cmds[cmdCount++] = i + 1;
            }
            // Input redirection
            else if (strcmp(argv[i], "<") == 0) 
            {
                inloc = i + 1;
                argv[i] = 0;
            }
            // Output redirection
            else if (strcmp(argv[i], ">") == 0) 
            {
                outloc = i + 1;
                argv[i] = 0;
            }
        }

        // Execution: Execute commands in child processes
        // Note: Assuming only one input (on first command)
        //       and one output (on last command)
        int lfd[2],     // Left pipe (sent from previous command)
            rfd[2],     // Right pipe (sent to next command)
            pid,        // Process ID
            inf = -1,   // Handle to input file if there is one
            outf = -1;  // Handle to output file if there is one

	int cmd;
        for (cmd = 0; cmd <= pipeCount; ++cmd)
        {
            // Create a pipe to the next command
            if (cmd < pipeCount) { pipe(rfd); }

            // Create a child process
            pid = fork();

            // Error
            if (pid < 0) { exit(-1); }
            // Child
            else if (pid == 0)
            {
                // Input redirection
                if (cmd == 0 && inloc >= 0)
                {
                    // Open file to read
                    inf = open(argv[inloc], O_RDONLY);
                    if (inf < 0) { exit(-1); }

                    // Modify input
                    close(0);
                    dup(inf);
                    close(inf);
                }

                if (pipeCount > 0) {
		    // First command
                    if (cmd == 0)
                    {
                        // Only write to pipe
                        close(1);
                        dup(rfd[1]);
                        close(rfd[1]);
                        close(rfd[0]);
                        close(lfd[0]);
                        close(lfd[1]);
                    }
                    // Intermediate command
                    else if (cmd < pipeCount)
                    {
                        // Read from & write to pipe
                        close(0);
                        dup(lfd[0]);
                        close(lfd[0]);
                        close(lfd[1]);
                        //close(1);
                        //dup(rfd[1]);
                        //close(rfd[1]);
                        //close(rfd[0]);
                    }
                    // Last command
                    else
                    {
                        // Only read from pipe
                        close(0);
                        dup(lfd[0]);
                        close(lfd[0]);
                        close(lfd[1]);
                        close(rfd[0]);
                        close(rfd[1]);
                    }
                }

                // Output redirection
                if (cmd == pipeCount && outloc >= 0)
                {
                    // Open file to write
                    outf = open(argv[outloc], O_CREAT | O_WRONLY, 0754);
                    if (outf < 0) { exit(-1); }

                    // Modify output
                    close(1);
                    dup(outf);
                    close(outf);
                }

                execvp(argv[cmds[cmd]], &argv[cmds[cmd]]);
                printf(" didn't exec \n ");
            }
            // Parent
            else
            {
                // Close left pipe if opened from a previous command
                if (cmd > 0)
                {
                    close(lfd[0]);
                    close(lfd[1]);
                }

                // Move right pipe to left pipe
                lfd[0] = rfd[0];
                lfd[1] = rfd[1];

                wait(&status);
            }
        }
    }
}
