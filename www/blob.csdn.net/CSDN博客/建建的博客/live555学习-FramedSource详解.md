# live555学习-FramedSource详解 - 建建的博客 - CSDN博客
2017年02月24日 10:34:35[纪建](https://me.csdn.net/u013898698)阅读数：423
FramedSource讲解
FramedSource是一个抽象类，继承自mediaSource继承自medium，里面有纯虚函数virtual void doGetNextFrame() = 0;
主要作用就是从文件中获得数据，只是获得数据，交给分析H264or5VideoStreamParser分析获得NALU
关于有关牵涉到文件操作的基本都是直接或者间接继承自该类：例如H26VideoFileSourceMediaSubsession
主要函数流程：获得下一包数据getNextFrame
**[cpp]**[view
 plain](http://blog.csdn.net/zhangjikuan/article/details/38564199#)[copy](http://blog.csdn.net/zhangjikuan/article/details/38564199#)
- void FramedSource::getNextFrame(unsigned char* to, unsigned maxSize,  
-                 afterGettingFunc* afterGettingFunc,  
- void* afterGettingClientData,  
-                 onCloseFunc* onCloseFunc,  
- void* onCloseClientData) {  
- // Make sure we're not already being read:
- if (fIsCurrentlyAwaitingData) {  
-     envir() << "FramedSource[" << this << "]::getNextFrame(): attempting to read more than once at the same time!\n";  
-     envir().internalError();  
-   }  
- 
-   fTo = to;//存放读取到数据的内存指针
-   fMaxSize = maxSize;//允许的最大数据量，即fTo指针指向的内存区间的大小
-   fNumTruncatedBytes = 0; // by default; could be changed by doGetNextFrame()   表示实际读取数据大于<span style="font-family: Arial, Helvetica, sans-serif; font-size: 12px;">fMaxSize的时候，多了多少字节</span>
-   fDurationInMicroseconds = 0; // by default; could be changed by doGetNextFrame()
-   fAfterGettingFunc = afterGettingFunc;  
-   fAfterGettingClientData = afterGettingClientData;  
-   fOnCloseFunc = onCloseFunc;  
-   fOnCloseClientData = onCloseClientData;  
-   fIsCurrentlyAwaitingData = True;  
- 
-   doGetNextFrame();  
- }  
由此可见，此函数初始化了几个必要参数，并调用了纯虚函数doGetNextFrame();此函数留给派生类实现，例如ByteStreamFileSource类实现如下
**[cpp]**[view
 plain](http://blog.csdn.net/zhangjikuan/article/details/38564199#)[copy](http://blog.csdn.net/zhangjikuan/article/details/38564199#)
- void ByteStreamFileSource::doGetNextFrame() {  
- if (feof(fFid) || ferror(fFid) || (fLimitNumBytesToStream && fNumBytesToStream == 0)) {  
-     handleClosure();  
- return;  
-   }  
- 
- #ifdef READ_FROM_FILES_SYNCHRONOUSLY
-   doReadFromFile();  
- #else
- if (!fHaveStartedReading) {  
- // Await readable data from the file:
-     envir().taskScheduler().turnOnBackgroundReadHandling(fileno(fFid),  
-            (TaskScheduler::BackgroundHandlerProc*)&fileReadableHandler, this);  
-     fHaveStartedReading = True;  
-   }  
- #endif
- }  
**[cpp]**[view
 plain](http://blog.csdn.net/zhangjikuan/article/details/38564199#)[copy](http://blog.csdn.net/zhangjikuan/article/details/38564199#)
- void ByteStreamFileSource::doReadFromFile() {  
- // Try to read as many bytes as will fit in the buffer provided (or "fPreferredFrameSize" if less)
- if (fLimitNumBytesToStream && fNumBytesToStream < (u_int64_t)fMaxSize) {  
-     fMaxSize = (unsigned)fNumBytesToStream;  
-   }  
- if (fPreferredFrameSize > 0 && fPreferredFrameSize < fMaxSize) {  
-     fMaxSize = fPreferredFrameSize;  
-   }  
- #ifdef READ_FROM_FILES_SYNCHRONOUSLY
-   fFrameSize = fread(fTo, 1, fMaxSize, fFid);  
- #else
- if (fFidIsSeekable) {  
-     fFrameSize = fread(fTo, 1, fMaxSize, fFid);  
-   } else {  
- // For non-seekable files (e.g., pipes), call "read()" rather than "fread()", to ensure that the read doesn't block:
-     fFrameSize = read(fileno(fFid), fTo, fMaxSize);  
-   }  
- #endif
- if (fFrameSize == 0) {  
-     handleClosure();  
- return;  
-   }  
-   fNumBytesToStream -= fFrameSize;  
- 
- // Set the 'presentation time':
- if (fPlayTimePerFrame > 0 && fPreferredFrameSize > 0) {  
- if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0) {  
- // This is the first frame, so use the current time:
-       gettimeofday(&fPresentationTime, NULL);  
-     } else {  
- // Increment by the play time of the previous data:
-       unsigned uSeconds = fPresentationTime.tv_usec + fLastPlayTime;  
-       fPresentationTime.tv_sec += uSeconds/1000000;  
-       fPresentationTime.tv_usec = uSeconds%1000000;  
-     }  
- 
- // Remember the play time of this data:
-     fLastPlayTime = (fPlayTimePerFrame*fFrameSize)/fPreferredFrameSize;  
-     fDurationInMicroseconds = fLastPlayTime;  
-   } else {  
- // We don't know a specific play time duration for this data,
- // so just record the current time as being the 'presentation time':
-     gettimeofday(&fPresentationTime, NULL);  
-   }  
- 
- // Inform the reader that he has data:
- #ifdef READ_FROM_FILES_SYNCHRONOUSLY
- // To avoid possible infinite recursion, we need to return to the event loop to do this:
-   nextTask() = envir().taskScheduler().scheduleDelayedTask(0,  
-                 (TaskFunc*)FramedSource::afterGetting, this);  
- #else
- // Because the file read was done from the event loop, we can call the
- // 'after getting' function directly, without risk of infinite recursion:
-   FramedSource::afterGetting(this);  
- #endif
- }  
在以上函数中关于几个重要参数主要是通过这里传送的nextTask() = envir().taskScheduler().scheduleDelayedTask(0,
(TaskFunc*)FramedSource::afterGetting,
 this);
FramedSource::afterGetting 再回到FramedSource中，发现afterGetting是个静态函数，所以在这里可以直接的调用，FramedSource中定义如下
**[cpp]**[view
 plain](http://blog.csdn.net/zhangjikuan/article/details/38564199#)[copy](http://blog.csdn.net/zhangjikuan/article/details/38564199#)
- void FramedSource::afterGetting(FramedSource* source) {  
-   source->fIsCurrentlyAwaitingData = False;  
- // indicates that we can be read again
- // Note that this needs to be done here, in case the "fAfterFunc"
- // called below tries to read another frame (which it usually will)
- 
- if (source->fAfterGettingFunc != NULL) {  
-     (*(source->fAfterGettingFunc))(source->fAfterGettingClientData,  
-                    source->fFrameSize, source->fNumTruncatedBytes,  
-                    source->fPresentationTime,  
-                    source->fDurationInMicroseconds);  
-   }  
- }  
由上可见，afterGetting实际上就是把getNextFrame和doGetNextFrame();中初始化或者改变的参数包装了一下，用nextTask() = envir().taskScheduler().scheduleDelayedTask(0,
(TaskFunc*)FramedSource::afterGetting, this);传递下去
关于参数还有一个叫fFrameSize的，此参数代表实际读取的字节数，肯定这个数值要小于fMaxSize
[转载地址](http://blog.csdn.net/zhangjikuan/article/details/38564199)
