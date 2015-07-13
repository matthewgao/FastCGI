#ifndef FCGIRECORD
#define FCGIRECORD

#include "protocol.h"
#include <string.h>

class FCGIRecord{
  public:
    FCGIRecord();
    FCGIRecord(size_t size);
    ~FCGIRecord();

    unsigned char* getBodyStartPtr();
    FCGI_Header* getHeaderPtr();
    unsigned char* getOffsetBody();
    bool isEndOfBody();
    bool isEndOfHeader();
    bool isEndOfPadding();
    bool isFinished();
    int readBytes(unsigned char* ptr, size_t size);
  
  private:
    unsigned char* body;
    FCGI_Header header;
    unsigned int headOffset;
    unsigned int bodyOffset;
    unsigned int bodyLength;
    unsigned int paddingOffset;
    bool isCompleted;
};
#endif
