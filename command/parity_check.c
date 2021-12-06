#include "type.h"
#include "stdio.h"
#include "string.h" 

#define MaxFileNameLen 20
#define MaxFileBuf 50000
typedef struct checkResult{
	char filename[MaxFileNameLen];
	unsigned char result_value;
}check_result;

check_result result;

unsigned char parity_check(unsigned char * buf,int size) {
    return 5;
}

check_result file_check(const char *filename){
    
    
	memcpy((char*)result.filename,(char*)filename,MaxFileNameLen);
    
    //define file status
    struct stat statbuf;
    if(stat(filename,&statbuf)!=0){
        printf("Error!\n");
        exit(-1);
    }
    printf("read file Success!\n");
    printf("%d",statbuf.st_size);
    //unsigned char* filebuf=(unsigned char *)malloc(statbuf.st_size * sizeof(unsigned char));
    unsigned char filebuf[MaxFileBuf];
    int fd = open(filename,O_RDWR);
    if(fd==-1) {
    	printf("file:%s open failed.\n",filename);
    	exit(-1);
    }

    int readlength;
    readlength = read(fd,filebuf,statbuf.st_size);
    printf("readlength:%d,FileSize:%d\n",readlength,statbuf.st_size);

    result.result_value = parity_check(filebuf,statbuf.st_size);
    printf("file:%s,checkvalue=%d\n",filename,result.result_value);
    close(fd);

    return result;
}

int main(int argc,char *argv[]){
	char filename[3]="ls";
	
	result = file_check(filename);
		
	
}