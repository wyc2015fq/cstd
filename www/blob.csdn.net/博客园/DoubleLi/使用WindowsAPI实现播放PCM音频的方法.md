# 使用WindowsAPI实现播放PCM音频的方法 - DoubleLi - 博客园







这篇文章主要介绍了使用WindowsAPI实现播放PCM音频的方法,很实用的一个功能,需要的朋友可以参考下







本文介绍了使用WindowsAPI实现播放PCM音频的方法，同前面一篇[使用WindowsAPI获取录音音频的方法](http://www.jb51.net/article/53858.htm)原理具有相似之处，这里就不再详细介绍具体的函数与结构体的参数，相同的部分加以省略，只介绍主要的功能部分代码。如下所示：

1. waveOutGetNumDevs

2. waveOutGetDevCaps

3. waveOutOpen

**回调函数：**




[?](http://www.jb51.net/article/53860.htm#)

```
```cpp
void
```

```cpp
CALLBACK PlayCallback(HWAVEOUT hwaveout,
```

```cpp
UINT
```

```cpp
uMsg,
```

```cpp
DWORD
```

```cpp
dwInstance,
```

```cpp
DWORD
```

```cpp
dwParam1,
```

```cpp
DWORD
```

```cpp
dwParam2);
```
```




4. waveOutPrepareHeader

5. waveOutWrite：执行后立即开始播放，当前缓冲区播放完成会调用回调函数

**这里需要注意：**

为了能够实现连续播放效果，在第4部需要准备两个或两个以上的播放数据。
感兴趣的朋友可以对比前面的文章手动调试一下本文所述实例。










