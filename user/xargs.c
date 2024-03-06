#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
	int index = 0;
	int _argc = 1;
	char *_argv[MAXARG]; // its set to 32 in param.h
	
	// Check if the -n argument is used
	if(strcmp(argv[1], "-n") == 0)
	{
		index = 3;
	}
	else
	{
		index = 1;
	}

	// Copy the command to be run into _argv[0]
	_argv[0] = malloc(strlen(argv[index]) + 1);
	strcpy(_argv[0], argv[index]);
	
	// Copy the remaining command line arguments into _argv
	for(int i = index + 1; i < argc; ++i)
	{
		//printf("--%s--\n",argv[i]);
		_argv[_argc] = malloc(strlen(argv[i]) + 1);
		strcpy(_argv[_argc++], argv[i]);
	}
	
	// Allocate memory for the command line buffer
	_argv[_argc] = malloc(128);
	
	// Read input from stdin one character at a time
	char buf;
	int i = 0;
	while(read(0, &buf, 1))
	{
		// If a newline character is found, run the command
		if(buf == '\n')
		{
			_argv[_argc][i++] = '\0';
			
			if(fork() == 0)
			{
				exec(argv[index], _argv);
			}
			else
			{
				i = 0;
				wait(0);
			}
		}
		// Otherwise, store the character in the command line buffer
		else
		{
			_argv[_argc][i++] = buf;
		}
	}
	// Exit the process
	exit(0);
}


