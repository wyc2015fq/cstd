# 循环缓存区之C++实现 - fanyun的博客 - CSDN博客
2017年09月06日 22:12:07[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：404
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
      最近，在优化服务器端底层代码时，遇到了一个比较影响系统性能的东西——缓存区，最已有的方案里，其采用了单缓冲区，每次需要空间时，都会检查缓冲区空闲空间是否满足，如果满足还得进行移动内存的操作（想必搞过服务器开发的同事，这点深有体会吧），当数据交换量猛增时，这个方案效果立马显现，于是自己私下写了一个简单版的循环缓存，原理很简单，在这之上自己将代码进行了规整和优化，并替换了原来的单缓存区方案，效果显著，好了，在这里，只放测试用的原始代码，至于优化代码鉴于商业原因，还是不放了，代码如下：
```cpp
#ifndef __CIRBUFFER__H
#define __CIRBUFFER__H
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <boost/assert.hpp>
using namespace std;
using namespace boost;
#define DEFAULT_SIZE 20
class CirBuffer
{
    public:
        CirBuffer(int maxSize):bufferSize(maxSize)
        {
            readPtr = writePtr = 0;
            isReadFast = isWriteFast = false;
            buffer = new char[bufferSize];
            assert(buffer);
            bzero(buffer,bufferSize);
        }
        ~CirBuffer()
        {
            assert(buffer);
            delete[] buffer;
        }
        int getBufferSize()
        {
            return bufferSize;
        }
        void display()
        {
            cout<<"writePtr:"<<writePtr<<"  readPtr:"<<readPtr<<" buffer:"<<buffer<<endl;
        }
        int usedMemorySize()
        {
            int len = 0;
            if(writePtr > readPtr)
                len = writePtr - readPtr;
            else if(readPtr > writePtr)
                len = bufferSize - readPtr + writePtr;
            return len;
        }
        void addMemory(int size,int len)
        {
            assert(buffer);
            int freelen = bufferSize - len;
            while(freelen < size)
            {
                bufferSize = bufferSize<<1;
                freelen = bufferSize - len;
            }
            char* newBuffer = new char[bufferSize];
            assert(newBuffer);
            bzero(newBuffer,bufferSize);
            if(writePtr > readPtr)
            {
                memcpy(newBuffer,&buffer[readPtr],writePtr-readPtr);
                delete[] buffer;
                buffer = newBuffer;
                readPtr = 0;
                writePtr = writePtr-readPtr;
            }
            else
            {
                int end = bufferSize - readPtr;
                memcpy(newBuffer,&buffer[readPtr],end);
                memcpy(&newBuffer[end],buffer,writePtr);
                delete[] buffer;
                buffer = newBuffer;
                readPtr = 0;
                writePtr = end + writePtr;
            }
        }
        void reserveBuffer(int size)
        {
            int usedLen = usedMemorySize();
            if(bufferSize < usedLen + size)
                addMemory(size,usedLen);
        }
        void readMv(int len)
        {
            if(len > bufferSize) return;
            readPtr = (readPtr + len) %(bufferSize);
            if(readPtr == writePtr)
                isReadFast = true;
            else
                isReadFast = false;
            display();
        }
        void writeMv(int len)
        {
            if(len > bufferSize) return;
            writePtr = (writePtr + len) %(bufferSize);
            if(writePtr == readPtr)
                isWriteFast = true;
            else
                isWriteFast = false;
            display();
        }
        void write(const char* Buffer,int size)
        {
            assert(Buffer);
            reserveBuffer(size);
            if(writePtr > readPtr)
            {
                int end = bufferSize - writePtr;
                if(end >= size)
                {
                    memcpy(&buffer[writePtr],Buffer,size);
                    writeMv(size);
                }
                else
                {
                    memcpy(&buffer[writePtr],Buffer,end);
                    memcpy(buffer,&Buffer[end],size-end);
                    writeMv(size);
                }
            }
            else if(writePtr < readPtr)
            {
                int len = readPtr - writePtr;
                if(len >= size)
                {
                    memcpy(&buffer[writePtr],Buffer,size);
                    writeMv(size);
                }
            }
            else
            {
                if(isReadFast)
                {
                    int end = bufferSize - writePtr;
                    if(end >= size)
                    {
                        memcpy(&buffer[writePtr],Buffer,size);
                        writeMv(size);
                    }
                    else
                    {
                        memcpy(&buffer[writePtr],Buffer,end);
                        memcpy(buffer,&Buffer[end],size-end);
                        writeMv(size);
                    }
                }
                else if(!isWriteFast && !isReadFast)
                {
                    memcpy(&buffer[writePtr],Buffer,size);
                    writeMv(size);
                }
            }
        }
        void read(char* Buffer,int size)
        {
            assert(Buffer);
            if(writePtr > readPtr)
            {
                int len = writePtr - readPtr;
                int readlen = (len > size)?size:len;
                memcpy(Buffer,&buffer[readPtr],readlen);
                readMv(readlen);
            }
            else if(writePtr < readPtr)
            {
                int end = bufferSize - readPtr;
                if(end >= size)
                {
                    memcpy(Buffer,&buffer[readPtr],size);
                    readMv(size);
                }
                else
                {
                    int len = (size > end + readPtr)?(readPtr):(size-end);
                    memcpy(Buffer,&buffer[readPtr],end);
                    memcpy(&Buffer[end],buffer,len);
                    readMv(len+end);
                }
            }
            else
            {
                if(isWriteFast)
                {
                    int end = bufferSize - readPtr;
                    if(end >= size)
                    {
                        memcpy(Buffer,&buffer[readPtr],size);
                        readMv(size);
                    }
                    else
                    {
                        memcpy(Buffer,&buffer[readPtr],end);
                        memcpy(&Buffer[end],buffer,size-end);
                        readMv(size);
                    }
                }
            }
        }
    private:
        char* buffer;
        int readPtr;
        int writePtr;
        int bufferSize;
        bool isReadFast;
        bool isWriteFast;
};
#endif
```
Main.cpp
```cpp
#include "CirBuffer.h"
int main()
{
    CirBuffer cirBuffer(DEFAULT_SIZE);
    const char* str = "Hello,everyone";
    char buffer[14];
    for(int i=0;i<4;i++)
    {
        cirBuffer.write(str,strlen(str));
        bzero(buffer,sizeof(buffer));
        cirBuffer.read(buffer,sizeof(buffer));
    }
    cout<<cirBuffer.getBufferSize()<<endl;
    return 0;
}
```
测试结果：
writePtr:14  readPtr:0 buffer:Hello,everyone
writePtr:14  readPtr:14 buffer:Hello,everyone
writePtr:8  readPtr:14 buffer:everyoneeryoneHello,
writePtr:8  readPtr:8 buffer:everyoneeryoneHello,
writePtr:2  readPtr:8 buffer:neeryoneHello,everyo
writePtr:2  readPtr:2 buffer:neeryoneHello,everyo
writePtr:16  readPtr:2 buffer:neHello,everyoneeryo
writePtr:16  readPtr:16 buffer:neHello,everyoneeryo
20
总结
       本篇博文主要是实现了一个循环缓冲区，设计思路比较简单，实用性不是很好，但是其中的一些思想可以借鉴，循环缓存区在实际应用中，使用的还是很多的，目前针对缓冲区设计一般集中在循环缓存、双循环缓存以及循环缓存链等，如果有时间的话，会实现一个双循环缓存和循环缓存链
