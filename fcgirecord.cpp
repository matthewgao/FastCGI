#include "fcgirecord.h"
#include <stdio.h>
#include <stdlib.h>

FCGIRecord::FCGIRecord():
    		body(NULL),
    		headOffset(0),
    		bodyOffset(0),
    		bodyLength(0),
    		isCompleted(false),
    		paddingOffset(0)
{
    memset(&header,0,sizeof(FCGI_Header));
};

FCGIRecord::FCGIRecord(size_t size){
    FCGIRecord();
    bodyLength = size;
    body =(unsigned char*) malloc(size);
};

FCGIRecord::~FCGIRecord(){
    if(body != NULL){
        free(body);
    } 
};

unsigned char* 
FCGIRecord::getBodyStartPtr(){
    if(body == NULL){
        return NULL;
    }
    return body;
};

FCGI_Header* 
FCGIRecord::getHeaderPtr(){
    return &header;
};
unsigned char* 
FCGIRecord::getOffsetBody(){
    return body+bodyOffset;
};
    
bool 
FCGIRecord::isEndOfBody(){
    if(bodyOffset == bodyLength){
        return true;
    }else{
        return false;
    }
};

bool 
FCGIRecord::isEndOfHeader(){
    if(headOffset == 8){
        return true;
    }else{
        return false;
    }
};
bool 
FCGIRecord::isEndOfPadding(){
    if(paddingOffset == header.paddingLength){
        return true;
    }else{
        return false;
    }
};
bool 
FCGIRecord::isFinished(){
    return isCompleted;
};

int 
FCGIRecord::readBytes(unsigned char* ptr, size_t size){
    int remainSize = size;
    if(!isEndOfHeader()){
        int pendingSize = 8-headOffset;
        if(size < pendingSize){
            pendingSize = size;
        }
        /*
         * need to explict convert &header to unsigned char*
         * otherwise the compiler don't know the offset for each step
         */
        memcpy((unsigned char*)(&header)+headOffset, ptr+(size - remainSize), pendingSize);
        headOffset = headOffset + pendingSize;
        remainSize = remainSize - pendingSize;
    }
    if(!isEndOfHeader()){
        printf("Not End of Header : %ld, %d\n", size - remainSize, headOffset);
        isCompleted = false;
        return size - remainSize;
    }else{
        if(body == NULL){
            bodyLength =(unsigned int)(((header.contentLengthB1)<<8) + header.contentLengthB0);
            printf("body size: %d,%x,%x,%x,%x\n", 
                   bodyLength, header.contentLengthB1, 
                   header.contentLengthB0,header.version,header.type);
            body =(unsigned char*) malloc(bodyLength);
        }
    }
    if(remainSize != 0 && !isEndOfBody()){
        int pendingBodySize = bodyLength - bodyOffset;
        if(remainSize < pendingBodySize){
            pendingBodySize = remainSize;
        }
        memcpy(body+bodyOffset, ptr+(size - remainSize), pendingBodySize);
        bodyOffset = bodyOffset + pendingBodySize;
        remainSize = remainSize - pendingBodySize;
    }

    if(remainSize == 0 && !isEndOfBody()){
        printf("Not EOF body\n");
        return size - remainSize;
    }

    if(remainSize != 0 && !isEndOfPadding()){
        int pendingPaddingSize = header.paddingLength - paddingOffset;
        if(remainSize < pendingPaddingSize){
            pendingPaddingSize = remainSize;
        }
        paddingOffset = paddingOffset + pendingPaddingSize;
        remainSize = remainSize - pendingPaddingSize;
    }
    
        printf("return : %ld\n", size - remainSize);
    return size - remainSize;
};
