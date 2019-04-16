# FFmpeg(12)-使用NDK通过GLSurfaceView完成视频的播放 - 夜行过客 - 博客园







# [FFmpeg(12)-使用NDK通过GLSurfaceView完成视频的播放](https://www.cnblogs.com/yongdaimi/p/9834843.html)





**一.包含头文件和库文件**

这里采用的是NDK中的ANativeWindow来完成视频的播放，因为需要添加相关的库和头文件。

CMakeLists

```
target_link_libraries( # Specifies the target library.
                       native-lib
                       avcodec
                       avformat
                       avutil
                       swscale
                       swresample
                       android

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
```

添加头文件 

```
#include <android/native_window.h>
#include <android/native_window_jni.h>
```

**二.示例代码**

```
// 显示窗口初始化
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (!window) {
        LOGE("ANativeWindow_fromSurface create failed");
        return;
    }
    ANativeWindow_setBuffersGeometry(window, outWidth, outHeight, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer wbuf;
```

```
if (h > 0) {
                        ANativeWindow_lock(window, &wbuf, 0);
                        uint8_t *dst = (uint8_t *)wbuf.bits; // 这个dst是用来交换的内存
                        memcpy(dst, rgb, outWidth*outHeight*4);
                        ANativeWindow_unlockAndPost(window);
                    }
```














