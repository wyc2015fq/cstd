# FFMPEG学习【libavformat】:I/O Read/Write - 一世豁然的专栏 - CSDN博客





2017年06月14日 14:42:58[一世豁然](https://me.csdn.net/Explorer_day)阅读数：348








一、头文件


|[avio.h](http://ffmpeg.org/doxygen/trunk/avio_8h.html)|
|----|



二、目录列表

目录列表API使得可以在远程服务器上列出文件。





一些可能的用例：


1、一个“打开文件”对话框可以从远程位置选择文件，

2、递归媒体查找器，为播放器提供播放所有文件从给定目录的能力。




一）、打开一个目录

首先，需要通过调用与URL一起提供的avio_open_dir（）和可选的包含协议特定参数的AVDictionary来打开目录。 该函数返回零或正整数，并在成功时分配AVIODirContext。




```cpp
AVIODirContext *ctx = NULL;
if (avio_open_dir(&ctx, "smb://example.com/some_dir", NULL) < 0) {
    fprintf(stderr, "Cannot open directory.\n");
    abort();
}
```


此代码尝试使用smb协议打开示例目录，而不需要任何其他参数。




二）、阅读目录

每个目录的条目（即文件，另一个目录，AVIODirEntryType中的其他目录）由AVIODirEntry表示。 从打开的AVIODirContext读取连续条目是通过重复调用avio_read_dir（）来完成的。 如果成功，每个调用返回零或正整数。 读取可以在NULL条目读取之后立即停止 - 这意味着没有任何条目被读取。 以下代码从与ctx关联的目录中读取所有条目，并将其名称打印到标准输出。




```cpp
AVIODirEntry *entry = NULL;
for (;;) {
    if (avio_read_dir(ctx, &entry) < 0) {
        fprintf(stderr, "Cannot list directory.\n");
        abort();
    }
    if (!entry)
        break;
    printf("%s\n", entry->name);
    avio_free_directory_entry(&entry);
}
```











