#include "./incs/minishell.h"

int main()
{
	char	*str;

	str = malloc(sizeof(char) * 3);

	str[0] = 'a';
	str[1] = 'b';
	str[2] = '\0';
	printf("%s %s\n", str, str);
	return (0);
}
