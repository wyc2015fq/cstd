# 编译动态库和静态库的Makefile模板 - V__KING__的专栏 - CSDN博客





2017年08月30日 19:48:01[v__king__](https://me.csdn.net/V__KING__)阅读数：391








参考：[linux编译动态库和静态库的makefile示例](http://blog.csdn.net/shaoxiaohu1/article/details/46943417)

```
# 1、准备工作，编译方式、目标文件名、依赖库路径的定义。
CC = gcc
CFLAGS  := -w -g -Wall

LIB = libltkc.so 

HDRS = \
    version.inc     \
    ltkc.h          \
    ltkc_base.h     \
    ltkc_connection.h   \
    ltkc_frame.h        \
    ltkc_platform.h     \
    ltkc_xmltext.h      \


OBJS = \
    ltkc_array.o        \
    ltkc_connection.o   \
    ltkc_element.o      \
    ltkc_encdec.o       \
    ltkc_error.o        \
    ltkc_framedecode.o  \
    ltkc_frameencode.o  \
    ltkc_frameextract.o \
    ltkc_hdrfd.o        \
    ltkc_xmltextencode.o    \
    ltkc_xmltextdecode.o    \
    ltkc_typeregistry.o \



INC= -I /usr/include/libxml2

# 依赖的lib名称
# DEPEND_ON_LIB = -lxmlllrp -lxml2
DEPEND_ON_LIB = -lxml2

all : $(LIB)

# 2. 生成.o文件 
%.o:%.c
    $(CC) $(CFLAGS) -c $< -o $@ $(INC) $(DEPEND_ON_LIB)
    @echo "-----"

# 3. 生成动态库文件
$(LIB):$(OBJS)
    $(CC) $(CFLAGS) -shared -o $@ $(OBJS)


# 4. 删除中间过程生成的文件 
clean:
    rm -f $(OBJS) $(TARGET) $(LIB)
```

## 测试动态库

1、在测试过程中，经常会报错：找不到.so文件。一种简单的解决方法如下： 

> 
cannot open shared object file: No such file or directory 


在linux终端输入如下命令：

> 
export LD_LIBRARY_PATH=/home/shaoxiaohu/lib:LD_LIBRARY_PATH:


更多解决方法可参考：Linux下gcc编译生成动态链接库*.so文件并调用它的第4部分。

参考： 

1]:[3种方法加载到系统动态库](http://www.cnblogs.com/smartvessel/archive/2011/01/21/1940868.html)

2]: [Linux下gcc编译生成动态链接库*.so文件并调用它的第4部分](http://blog.sina.com.cn/s/blog_54f82cc20101153x.html)



