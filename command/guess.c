# include "stdio.h"
# include "type.h"
# include "string.h"

int list[10]={3,6,4,9,5,7,1,0,8,2};


int numinit(int num) {
	
	return list[num-1];
}

void welcome() {
	//clear();
	printf("                         welcome\n");
	printf("***************************************************************\n");
	printf("GUESS\n");
	printf("direction:\n");
	printf("    system will generate 1 numbers between 0 and 9 ,\n");
	printf("    try to find out what and where the number are!\n");
	printf("    we will give you some hints £º\n");
	printf("    press 'q' to quit the game \n");
	printf("***************************************************************\n");
	printf("\n");

}

int main(int argc, char * argv[])
{
    int flag=0;
    int default_num;
	welcome();
	
	int fd_stdin=open("/dev_tty1",O_RDWR);
    int guess;
    char buf[70];
    printf("Please input a integer between 0 and 9:");
    read(fd_stdin,buf,70);
    default_num = buf[0]-'0';
    default_num = numinit(default_num);

	while (1) {		
        
        if(buf[0] == 'q') {
        	break;
        }
        if(buf[0] < '0' || buf[0] >'9'){
            printf("Your number is out of range!");
            break;
        }
		guess = buf[0]-'0';

		if(guess == default_num) {
			printf("Congratulation!\n");
			break;
		}else if(guess > default_num) {
			printf("Your number is too big.Please try again:");
			read(fd_stdin,buf,70);
		}else if(guess < default_num){
            printf("Your number is too small.Please try again:");
            read(fd_stdin,buf,70);
		}
	}

	return 0;
}
