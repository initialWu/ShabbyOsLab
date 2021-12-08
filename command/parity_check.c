#include "type.h"
#include "stdio.h"
#include "string.h" 

#define MaxFileNameLen 20
#define MaxFileBuf 50000
#define FileNum 4

typedef struct checkResult{
	char filename[MaxFileNameLen];
	unsigned char result_value;
}check_result;

char filename[FileNum][MaxFileNameLen]={"echo","pwd","ls","bufof"};
check_result result[FileNum];

unsigned char parity_check(unsigned char * buf,int size) {
    int i;
    unsigned char ch;
    ch=buf[0];
    for(i=1;i<size;i++){
    	ch=buf[i]^ch;
    }
    ch=(ch>>4)^(ch&0x0F);
	ch=(ch>>2)^(ch&0x03);
	ch=(ch>>1)^(ch&0x01);
	unsigned char value=(unsigned char)(ch);
	return value;
}

void file_check(const char *filename,int n){
	memcpy((char*)result[n].filename,(char*)filename,MaxFileNameLen);
    
    //define file status
    struct stat statbuf;
    if(stat(filename,&statbuf)!=0){
        printf("Error!\n");
        exit(-1);
    }
    printf("read file Success!\n");
    printf("%d",statbuf.st_size);
    
    unsigned char filebuf[MaxFileBuf];
    int fd = open(filename,O_RDWR);
    if(fd==-1) {
    	printf("file:%s open failed.\n",filename);
    	exit(-1);
    }

    int readlength;
    readlength = read(fd,filebuf,statbuf.st_size);
    printf("readlength:%d,FileSize:%d\n",readlength,statbuf.st_size);

    result[n].result_value = parity_check(filebuf,statbuf.st_size);
    printf("file:%s,checkvalue=%d\n",filename,result[n].result_value);
    close(fd);

    return ;
}

int main(int argc,char *argv[]){
	printf("Enter parity_check...\n");
	char OutFile[20]="CheckFile";
	//int fd = open(OutFile,O_CREAT|O_RDWR);
	int fd = open(OutFile,O_RDWR|O_CREAT);
    if(fd==-1) {
    	printf("file:%s open failed.\n",OutFile);
    	exit(-1);
    }

	int writelength;
	for(int i=0;i<FileNum;i++){
		file_check(filename[i],i);
		printf("result %d is %d",i,result[i].result_value);
	}

    writelength = write(fd,(char *)result,sizeof(check_result)*FileNum);
    if(writelength!=sizeof(check_result)*FileNum){
        printf("Check File:%s write failed!\n",OutFile);
        return -1;
    }
    close(fd);
    return 0;

	
}
