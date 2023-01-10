#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc== 4)
	{
		if(strcmp(argv[3], "Mandelbrot") == 0)
		{
			printf("manny!\n");
		}
		else
		{
			if(strcmp(argv[3], "Julia") == 0)
			{
				printf("juju!\n");
			}
			else
			{
				printf("Les deux noms de fractales valides sont 'Mandelbrot' et 'Julia'\n");
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
