#include<stdio.h>
#include<stdlib.h>
int main(int argc,char **argv)
{
	int i=100;
	if(argc>1)
	i = atoi(argv[1]);
	printf("sum(100) = %d",sum(i));
}
