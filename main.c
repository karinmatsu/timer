#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// s
int main(int , char **argv)
{
	int time = 60 * atoi(argv[1]);

	for (int i = 0; i < time; i++)
	{
		printf("%d\n", i);	
		sleep(1);
	}
	printf("%d\n", time);	
			
	system("notify-send 'boa!' 'sessão finalizada'");
	//log_to_file((float)time/60);
	return 0;
}
