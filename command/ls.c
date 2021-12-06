#include "stdio.h"
#include "string.h"
int main(int argc, char * argv[])
{
	int num = 0;
	int dir;
	int n;
	char dir_buf[40];
	int i = 20;
	dir = open(".",O_RDWR);
	if(argc == 1 || strcmp(argv[1],"-a") != 0)
    for(int j=0;j<6;j++)
        n = read(dir,dir_buf,16);
	n = read(dir,dir_buf,16);
	printf("%s ",&dir_buf[40]);
	while(i){
		if(dir_buf[4] != 0)
		{
		    num++;
            printf("%s ",&dir_buf[4]);
		    if(num % 9 == 0) printf("\n");
		}
		n = read(dir,dir_buf,16);
		i--;
	}
	printf("\n");
	close(dir);
	return 0;

}
