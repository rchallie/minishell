#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
	int i;
	int gen_fork_;
	int fpipe[2];
	int spipe[2];
	char *str;
	int gen_status_ = 0;
	i = 0;
	if ((gen_fork_ = fork()) < 0)
	{
		exit(1258);
	}
	if (gen_fork_ == 0)
	{
		if (pipe(fpipe) == -1)
		{
			printf("ERROR\n");
			return (1);
		}
		if (pipe(spipe) == -1)
		{
			printf("ERROR\n");
			return (1);
		}
		while (i < 10)
		{
			int fork_;
			int gen_ = 0;

			fork_ = 0;
			if (fork_ == 0)
			{
				if (i == 0)
				{
					dup2(fpipe[1], 1);
					printf("SALUT\n");
					close(fpipe[0]);
				}
				else
				{
					char buffer[4096];

					dup2(fpipe[0], 0);
					close(fpipe[1]);
					read(0, &buffer, 4096);
					if (i != 9)
					{

						dup2(spipe[1], 1);
						close(spipe[0]);
					}				
					printf("%s", buffer);
				}
				exit(1);
			}
			else
				waitpid(fork_, &gen_, 0);
			i++;
		}
	}
	else
		waitpid(gen_fork_, &gen_status_, 0);
}
