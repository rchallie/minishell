#include "stdio.h"

void plop(int *val1, int *val2)
{
	(*val1 == *val2) ? *val1 = 10 : *val2= 20;
}

int main(int argc, char **argv)
{
	int i = 0;
	int test1, test2;
	test1 = test2 = 0;
	plop(:
	printf("Test 1 = %d\n", test1);
	while (argv[i])
	{
		printf("ARGV[%d] = |%s|\n", i, argv[i]);
		i++;
	}
	printf("ARGC = %d\n", argc);
	return (0);
}
