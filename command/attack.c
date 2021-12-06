# include "stdio.h"
# include "type.h"
# include "string.h"

// void trap(){
//     printf("You are hacked!");
// }

// int overflow(){
//     __asm__("xchg %%bx,%%bx"::);
//     int access;
//     char password[5];
//     access = 0;
    
//     read(0,password,70);
//     memcpy(password+29,&trapaddr,4);
//     memcpy(password+41,&trapaddr,4);
//     if (strcmp(password,"12345") == 0) {
//      access = 1;
//     }
//     if(access != 0) {
//         printf("Success!");
//     } else {
//         printf("Failed!");
//     }
    
//     access=1;

// }

// int main(int argc, char * argv[]){
//     overflow();
//     return 0;
// }

int main(int argc, char * argv[]) {
    overflow();
}

void payload(){
    printf("You are hacked!\n");
}
unsigned char maldata[24];

void overflow(){
    unsigned char testbuf[8]={1};
    unsigned long dwPayLoad=(unsigned long)payload;
    memcpy(maldata+20,&dwPayLoad,4);
    memcpy(testbuf,maldata,24);

}