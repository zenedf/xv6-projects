#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int primestart = 2;
int main(int argc, char *argv[])
{
   int _pipe[2][2];
   pipe(_pipe[0]);

   int index = 0;
   int num;

   for(int i = primestart; i < 35; ++i){
	write(_pipe[index][1],&i,4);
   }
   close(_pipe[0][1]);

   while(fork() == 0){
	if(read(_pipe[index][0],&primestart,4) != 0){
	   printf("prime %d\n",primestart);
	}
       	else{
	     exit(0);
	}
	pipe(_pipe[index^1]);
	while(read(_pipe[index][0], &num, 4) != 0){
		if(num%primestart){
			write(_pipe[index^1][1], &num, 4);
		}
	}
	close(_pipe[index^1][1]);
	index = index^1;
    }
    close(_pipe[index][0]);
    wait(0);
    exit(0); 
}
