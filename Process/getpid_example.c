#include<stdio.h>
#include<unistd.h>
int main(int argc, char *argv[])
{
	printf("the current program's pid is %d\n", getpid());
	printf("the current program's ppid is %d\n", getppid());
	printf("the current program's pgid is %d\n", getpgid(0));
	return 0;
}
