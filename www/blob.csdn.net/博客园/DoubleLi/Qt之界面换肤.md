# Qt之界面换肤 - DoubleLi - 博客园






# 简述

常用的软件基本都有换肤功能，例如：QQ、360、迅雷等。换肤其实很简单，并没有想象中那么难，利用前面分享过的QSS系列文章，沃我们完全可以实现各种样式的定制！




- [简述](http://blog.csdn.net/liang19890820/article/details/52384042#简述)
- [实现原理](http://blog.csdn.net/liang19890820/article/details/52384042#实现原理)
- [效果](http://blog.csdn.net/liang19890820/article/details/52384042#效果)
- [新建QSS文件](http://blog.csdn.net/liang19890820/article/details/52384042#新建qss文件)
- [编写QSS代码](http://blog.csdn.net/liang19890820/article/details/52384042#编写qss代码)
- [加载QSS](http://blog.csdn.net/liang19890820/article/details/52384042#加载qss)
- [更多参考](http://blog.csdn.net/liang19890820/article/details/52384042#更多参考)





# 实现原理
- 新建多个QSS文件
- 为各个QSS文件编写对应的样式代码
- 换肤时，进行全局切换

# 效果

![这里写图片描述](http://img.blog.csdn.net/20160831130431216)

# 新建QSS文件

首先，新建两个（按需添加）后缀名为qss的文件，例如：black.qss、white.qss，将它们加入资源文件（qrc）中。

**提示：**也可以使用绝对路径或相对路径。

# 编写QSS代码

分别在“black.qss”、“white.qss”文件中编写自己的样式代码，例如：

black.qss：

```
/**********提示**********/
QToolTip {
        border: 1px solid rgb(45, 45, 45);
        background: white;
        color: black;
}
```

white.qss：

```
/**********提示**********/
QToolTip {
        border: 1px solid rgb(111, 156, 207);
        background: white;
        color: rgb(51, 51, 51);
}
```

QSS代码请参考：[Qt之QSS（黑色炫酷）](http://blog.csdn.net/liang19890820/article/details/51992070)、[Qt之QSS（白色靓丽）](http://blog.csdn.net/liang19890820/article/details/51992137)，可以直接使用！

# 加载QSS

为了方便以后调用，可以写一个静态加载样式的函数：

```
#include <QFile>
#include <QApplication>

class CommonHelper
{
public:
    static void setStyle(const QString &style) {
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
};
```

在需要换肤的时候进行全局切换，例如，切换为黑色皮肤：

```bash
CommonHelper::setStyle("black.qss");
```

如果要切换为白色皮肤，只需要将“black.qss”替换为“white.qss”即可。

这样，我们就完成了一个换肤功能，就这么简单！

# 更多参考
- [Qt之加载QSS文件](http://blog.csdn.net/liang19890820/article/details/51993435)
- [Qt之QSS（语法高亮）](http://blog.csdn.net/liang19890820/article/details/51699473)
- [Qt之QSS（样式表语法）](http://blog.csdn.net/liang19890820/article/details/51691212)
- [Qt之QSS（动态属性）](http://blog.csdn.net/liang19890820/article/details/51693956)
- [Qt之QSS（Q_PROPERTY-原始属性）](http://blog.csdn.net/liang19890820/article/details/51698536)
- [Qt之QSS（Q_PROPERTY-自定义属性）](http://blog.csdn.net/liang19890820/article/details/51699229)
- [Qt之QSS（QDarkStyleSheet）](http://blog.csdn.net/liang19890820/article/details/51899025)
- [Qt之QSS（黑色炫酷）](http://blog.csdn.net/liang19890820/article/details/51992070)
- [Qt之QSS（白色靓丽）](http://blog.csdn.net/liang19890820/article/details/51992137)

from:http://blog.csdn.net/liang19890820/article/details/52384042









