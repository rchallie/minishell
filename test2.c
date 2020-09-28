#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	int i;
	int gen_fork_;
	char *str;
	int gen_status_ = 0;

	i = 0;
	if ((gen_fork_ = fork()) < 0)
	{
		exit(1258);
	}
	int fpipe[2];
	if (gen_fork_ == 0)
	{

		int nb = 5;
		int fd_in = 0;
		
		while (i < nb)
		{
		
			if (pipe(fpipe) == -1)
			{
				printf("ERROR\n");
				return (1);
			}
		
			int fork_;
			int gen_ = 0;
			char buffer[4096];
		
			fork_ = 0;
			if ((fork_ = fork()) == 0)
			{
				dup2(fd_in, 0);
		
				if (i != nb - 1)
					dup2(fpipe[1], 1);
		
				close(fpipe[0]);

				if (i == 0)
					printf("SALUT\n");

				else
				{
					read(0, &buffer, 4096);
					printf("%s", buffer);
				}

				close(fpipe[1]);
				exit(0);
			}

			else
			{
				waitpid(fork_, &gen_, 0);
				close(fpipe[1]);
				fd_in = fpipe[0];
			}
			i++;
		}
	}
	else
		waitpid(gen_fork_, &gen_status_, 0);
}
