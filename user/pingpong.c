#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
	int p[2], c[2];
        char buf[5];
	const int MSG_SIZE = 5;
	int pid;

	pipe(p); /* parent */
	pipe(c); /* child */

	if(pipe(p) == -1){
		printf("Parent pipe failed");
		exit(0);
	}

	if(pipe(c) == -1){
		printf("Child pipe failed");
		exit(0);
	}

	if((pid = fork()) == -1){
		printf("Fork failed");
		exit(0);
	}
	
	/* Child code */
	if (pid  == 0){ 
		close(c[1]); /* close unused child write  */
	        close(p[0]); /* close unused parent read  */

		read(c[0], buf, MSG_SIZE);

		printf("%d: received %s\n", getpid(), buf);

		write(p[1], "pong", 5);
		close(c[0]); /* close child read  */
		close(p[1]); /* close parent write  */

		exit(0);
	}
	/* Parent code */
	else{
		close(p[1]); /* close unused parent write  */
		close(c[0]); /* close unused child read  */

		write(c[1], "ping", 5);
		read(p[0], buf, 5);

		printf("%d: received %s\n", getpid(), buf);

		close(p[0]); /* close parent read  */
		close(c[1]); /* close child write  */

		exit(0);
	}
}
