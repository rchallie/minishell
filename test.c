#include "stdio.h"

int main(int argc, char **argv)
{
	int i = 0;
	while (argv[i])
	{
		printf("ARGV[%d] = |%s|\n", i, argv[i]);
		i++;
	}
	printf("ARGC = %d\n", argc);
	return (0);
}
