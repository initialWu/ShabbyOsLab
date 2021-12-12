# include "stdio.h"
# include "type.h"

#define FREE_FAILURE    0
#define PAGE_SIZE       4096
#define NULL            (void*)0

void flush(char* buf, int len)
{
    for(int i = 0; i < len; ++i)
    {
        buf[i] = '\0';
    }
    return;
}

unsigned int atoi16(char* str)
{
    char* p = str;
    unsigned int res = 0;

    if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))p += 2;
    while(*p && p < str + 70)
    {
        int t = 0;
        res *= 16;
        
        if((*p) >= '0' && (*p) <= '9')     t = (*p) - '0';
        else if((*p) >= 'a' && (*p) <= 'f')t = (*p) - 'a' + 10;
        else if((*p) >= 'A' && (*p) <= 'F')t = (*p) - 'A' + 10;
        else return 0;
        res += t;
        p++;
    }

    return res;
}

int atoi(char* str)
{
    char* p = str;
    int res = 0;
    while(*p && p < str + 70)
    {
        if(*p < '0' || *p > '9')return 1;
        res *= 10;
        res += (*p) - '0';
        p++;
    }
    return res;
}

int main(int argc, char * argv[])
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("OP INPUT:\n");
    printf("ALLOC: a\n");
    printf("FREE: f\n");
    printf("QUIT: q\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    void* addr = NULL;
	int fd_stdin = open("/dev_tty1",O_RDWR);
    char buf[70];
    read(fd_stdin, buf, 70);
    
    while(1)
    {
        switch(buf[0])
        {
        case 'a':
            printf("Input page count(1 ~ 1024):");
            read(fd_stdin, buf, 70);
            int cnt = atoi(buf);

            addr = alloc(cnt * PAGE_SIZE);
            if(addr == NULL)
            {
                printf("Alloc failed: size too large!\n");
                break;
            }
            printf("Alloc success at: 0x%x, size: %d\n", addr, cnt * PAGE_SIZE);
            break;
        case 'f':
            printf("Input address:");
            read(fd_stdin, buf, 70);

            unsigned int faddr = atoi16(buf);
            printf("faddr: 0x%x\n", faddr);

            if(free((void*)faddr) == FREE_FAILURE)
            {
                printf("Invalid address!\n");
                break;
            }

            printf("Free success!\n");
            break;
        case 'q':
            printf("Bye!\n");
            return 0;
        default:
            printf("Illegal op!\n");
            break;
        }

        flush(buf, 70);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("OP INPUT:\n");
        printf("ALLOC: a\n");
        printf("FREE: f\n");
        printf("QUIT: q\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");        
        read(fd_stdin, buf, 70);
    }

    return 0;
}

