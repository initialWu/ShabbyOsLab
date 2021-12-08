#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

#define MaxFileNameLen 20
#define MaxFileBuf 50000
#define FileNum 4

typedef struct checkResult{
	char filename[MaxFileNameLen];
	unsigned char result_value;
}check_result;

unsigned char parityCheck(unsigned char* buf,int size);
unsigned char open_check(const char* filename);


PUBLIC int proc_parity_check(char *filename){
	int i;
	unsigned char store_check;
	unsigned char new_check;
    check_result result[FileNum];
	char OutFile[20]="CheckFile";
	printf("======parity check=======\n");
	int fd=open(OutFile,O_RDWR);
	if(fd==-1){
		printf("File:Check File open failed!\n");
		return -1;
	}
	int readlength=read(fd,(char *)result,sizeof(check_result)*FileNum);
    
    for(int i=0;i<FileNum;i++){
        if(strcmp(result[i].filename,filename)==0){
            store_check = result[i].result_value;
            printf("Record:\n File:%s,check value:%d\n",result[i].filename,result[i].result_value);
        }

    }
	// printf("Stroed result is %s\n",store_check);
	// printf("readlength:%d\n",readlength);
	new_check = open_check(filename);
	if(new_check==store_check){
        printf("Check Passed!\n");
		return 1;
	} else{
		printf("Check Failed!\n");
		return 0;
	}

}


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

unsigned char open_check(const char *filename){
	//memcpy((char*)result.filename,(char*)filename,MaxFileNameLen);
    unsigned result;
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

    result = parity_check(filebuf,statbuf.st_size);
    printf("file:%s,checkvalue=%d\n",filename,result);
    close(fd);
    printf("The result when open is %d\n",result);

    return result;
}