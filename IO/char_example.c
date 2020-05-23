#include<stdio.h>
int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	char ch;
	if(argc <= 1)
	{
		printf("check usage of %s \n", argv[0]);
		return -1;
	}

	if((fp = fopen(argv[1], "r")) == NULL)
	{
		perror(argv[1]);
		return -1;
	}
	while((ch=fgetc(fp)) != EOF)
	{
		fputc(ch, stdout);
	}
	fclose(fp);
	return 0;
}
