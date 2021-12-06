# include "stdio.h"
# include "type.h"
# include "string.h"

int i;
int* addr;
int *codeaddr;
void code(){
    printf("You are hacked!");
}

void main(int argc, char * argv[]){
    char buff[72] = {0};
    codeaddr = (unsigned long)code;
    for(i=0;i<72;i++) {
        if(argv[1][i]==0) {
            break;
        }
        buff[i] = argv[1][i];
    }
    addr = &buff[72];
    for(i=0;i<10;i++) {
        addr[i] = codeaddr;
    }

}