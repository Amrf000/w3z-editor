#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		printf("This tiny program sets the size of the given file to 0.\nSyntaxe: ZeroFile <filename>\n");
	}
	else
	{
		fopen(argv[1], "w");
	}
	return 0;
}

