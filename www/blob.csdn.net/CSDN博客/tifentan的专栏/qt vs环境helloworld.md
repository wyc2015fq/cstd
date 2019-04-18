# qt vs环境helloworld - tifentan的专栏 - CSDN博客

2018年02月24日 10:07:26[露蛇](https://me.csdn.net/tifentan)阅读数：111标签：[qt																[vs																[2015																[helloworld](https://so.csdn.net/so/search/s.do?q=helloworld&t=blog)](https://so.csdn.net/so/search/s.do?q=2015&t=blog)](https://so.csdn.net/so/search/s.do?q=vs&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[qt](https://blog.csdn.net/tifentan/article/category/7464054)


## 原理

实际上下载一个[qt](https://www.qt.io/download)，安装上后在vs2015里面配置好头文件路径跟库文件路径，再添加用到的库接口lib就可以了，并不需要装什么vs插件什么的。

## 做法
- 创建一个新解决方案，在解决方案下创建一个新的空项目 
![这里写图片描述](https://img-blog.csdn.net/20180224095817730?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意一下别使用预编译头
- 头文件目录 

将你安装好的qt头文件目录添加到项目的头文件目录中 
![这里写图片描述](https://img-blog.csdn.net/20180224100151108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 库文件目录 
![这里写图片描述](https://img-blog.csdn.net/20180224100241712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 添加用到的库文件 
![这里写图片描述](https://img-blog.csdn.net/20180224100329250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- mian文件内容

```
#include "QApplication.h"
#include "QLabel"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel label("Hello, world");
    label.show();

    return app.exec();

}
```

运行就可以 了，简单吧！


