#include <stdio.h>

int main(int argc, char **argv)
{
	if(argc== 4)
	{
		if(argv[3] == "Mandelbrot")
		{
			printf("manny!\n");
		}
		else
		{
			if(argv[3] == "Julia")
			{
				printf("juju!\n");
			}
			else
			{
				printf("non\n");
				return 1;
			}
		}
	}
	else
	{
		printf("non\n");
		return 1;
	}
	
	return 0;
}
