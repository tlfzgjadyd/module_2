#include "getRequest.h"	

int main(int cSock, int *conFlag, char **type, char **path, char **ip){
    int fd=open("./request.txt", O_RDONLY, 0444);
    if(fd==-1){
        perror("open");
        return -1;
    }
    char buf[BUFSIZ];
    while(1){
        int nRead=read(fd, buf, sizeof(buf));
        if(nRead<0){
            perror("read");
            return -1;
        }
        else if(nRead==0){
            break;
        }
    }
    char *savePtr;
    char *saveStr;
    char *ptr=strtok_r(buf,"\n", &savePtr);

    *type=strtok_r(ptr,":", &saveStr);
    *type=strtok_r(NULL,":", &saveStr);
    if(strcmp(*type, "ls")!=0&&strcmp(*type, "download")!=0)
    {
        perror("type");
        return -1;
    }

    ptr=strtok_r(NULL, "\n", &savePtr);

    *path=strtok_r(ptr, ":", &saveStr);
    *path=strtok_r(NULL, ":", &saveStr);

    ptr=strtok_r(NULL, "\n", &savePtr);

    *ip=strtok_r(ptr, ":", &saveStr);
    *ip=strtok_r(NULL, ":", &saveStr);

    return 0;
}