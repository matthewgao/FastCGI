#include <sys/socket.h>
//#include "protocol.h"
#include <errno.h>
#include "fcgirecord.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#define READ_SZIE 500
int main(){
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    unsigned char buff[1024];
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9002);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr));
    
    listen(listenfd, 256);

    while(true){
        printf("start listen\n");
        connfd = accept(listenfd, NULL, NULL);
        if(connfd == -1){
            printf("accept fail\n");
            continue;
        }
        FCGIRecord* record = new FCGIRecord();
        while(true){
            int result = read(connfd, buff, READ_SZIE);
            printf("got size: %d\n",result);
            if(result == 0){
                printf("close connection\n");
                //write(connfd,"yyy",3);
                close(connfd);
                break;
            }else if(result < 0){
                printf("Error %d", errno);
                close(connfd);
                break;
            }
            printf("Input: ");
            for(int i = 0; i<READ_SZIE; i++){
                printf("%x ", buff[i]);
            }
            printf("\n");
            int allCount = 0; 
            while(allCount < result){
                printf("result size:%d \n", result); 
                int count = record->readBytes(buff+allCount, result-allCount);
                allCount = allCount + count;
                if(record-> isEndOfHeader() && record->isEndOfBody() && record->isEndOfPadding()){
                    printf("ver:%d, type:%d, requestid:%d, contentLen:%d, paddingLen:%d\n",
                        record->getHeaderPtr()->version,
                        record->getHeaderPtr()->type, 
                        (record->getHeaderPtr()->requestIdB1<<8) + record->getHeaderPtr()->requestIdB0,
                        (record->getHeaderPtr()->contentLengthB1<<8) + record->getHeaderPtr()->contentLengthB0,
                        record->getHeaderPtr()->paddingLength);    
                    char temp[5] = "1";
                    write(connfd, temp, 5);
                    delete record;
                    record = new FCGIRecord();
                }
                printf("loop allcount:%d, count:%d\n", allCount,count);
            }
        }
//        close(connfd);

    }
    close(listenfd);
}
