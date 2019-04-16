# FFmpeg编译： undefined reference to 'av_frame_alloc()' - 夜行过客 - 博客园







# [FFmpeg编译： undefined reference to 'av_frame_alloc()'](https://www.cnblogs.com/yongdaimi/p/10173434.html)





今天使用CMake编译FFmpeg的时候，死活编不过，提示什么“undefined reference to 'av_frame_alloc()”

后来仔细查找，发现是**头文件包含错误**。

错误的代码：

```
#include <libavutil/frame.h>
#include "IDecoder.h"

struct AVCodecContext;
class FFDecoder : public IDecoder{

public:
    virtual bool Open(XParameters params);
    /** 发送数据到解码队列 */
    virtual bool SendPacket(XData pkt);
    /** 从解码队列中获取一帧数据 */
    virtual XData RecvFrame();

protected:
    AVCodecContext *avctx = 0;
    AVFrame        *frame = 0;

};
```

解决办法

因为使用的是C++,所以在包含头文件的时候要特别注意，如果要包含的是C语言的头文件，必须用extern "C" 来包裹。比如：

```
extern "C" {
#include <libavcodec/avcodec.h>
}
```

我的问题就是直接在头文件中引入了FFmpeg的头文件 #include <libavutil/frame.h> ，但没有用extern "C" 包裹才出错的。正确的做法是使用extern "C" 包裹该头文件。

或者是直接在顶部声明用到的结构体即可。如：

```
#include "IDecoder.h"

struct AVCodecContext;
struct AVFrame;
class FFDecoder : public IDecoder{

public:
    virtual bool Open(XParameters params);
    /** 发送数据到解码队列 */
    virtual bool SendPacket(XData pkt);
    /** 从解码队列中获取一帧数据 */
    virtual XData RecvFrame();

protected:
    AVCodecContext *avctx = 0;
    AVFrame        *frame = 0;

};
```














