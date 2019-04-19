# qt findChildren() - xqhrs232的专栏 - CSDN博客
2016年10月14日 23:55:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：280
原文地址::[http://blog.csdn.net/mmz_xiaokong/article/details/8501206](http://blog.csdn.net/mmz_xiaokong/article/details/8501206)
相关文章 
1、findChildren()----[http://blog.sina.com.cn/s/blog_510737a301012t7w.html](http://blog.sina.com.cn/s/blog_510737a301012t7w.html)
2、
## 你还在用QPushButton吗?
 ----[http://blog.sina.com.cn/s/blog_510737a301012vv7.html](http://blog.sina.com.cn/s/blog_510737a301012vv7.html)
转自：http://blog.sina.com.cn/s/blog_510737a301012t7w.html
这篇文章标题比较特殊，呵呵，昨天在读别人源代码得时候发现了一个函数 ：findChildren()，当时就吸引我了，这是什么意思，翻了下文当终于明白是干什么用得了。在实际得Qt编程过程中，通常会有多个空间嵌套，就比如说：主窗体是QMainWdow，中央挂件是QTabWidget，而每一个QTabWidget子窗口中又有QTextEdit，这样得组合就是经典得多标签页文本编辑器样式，当然这个暑假我也在做这种编辑器。通常我们可以很容易得获得QMainWindow上QTabWidget控件得对象，但是想要直接操作QTabWidget中嵌套得QTextEdit就不能直接引用，现在我们有了这个函数就可以轻松搞定了，函数原型如下：
QList<T> QObject::findChildren(const QString &*name*=QString()) const
当然关于“找孩子”得函数不只这一个，具体得大家可以再查文档，这里给出一个例子：
QList<QLineEdit *> lineEdit = tabWidget.findChildren<QLineEdit *>();
这句话的意思就是在tabWidget挂件种寻找所有得QLineEdit控件，然后返回给QList容器（QList容器中每一个元素都为 QLineEdit *类型），下面给出一个实例：
描述：主窗体为QMainWindow，中央挂件为QTabWidget，每个子页面包括若干个QLineEdit（注意：Tab1页面有两个，Tab2页面有1个），当单击“setText”按钮时候，自动给每一个QLineEdit填充文字。
关键代码如下：
﻿
void MainWindow::on_pushButton_clicked()
{
    QList<QLineEdit *> lineEdit=ui->tabWidget->findChildren<QLineEdit *>();
    for (int i=0;i<lineEdit.count();i++)
    lineEdit.at(i)->setText("www.hicjiajia.cn");
}
`
这样对我们操作多嵌套式得控件确实很方便，但是又有一个问题了，这种方法会将所有QLineEdit都给影响到了，但是我现在想只让某个特定得QLineEdit来响应怎摸办，还好，findChildren()函数还是可以带参数得：
QList<QLineEdit *> lineEdit = tabWidget.findChildren<QLineEdit *>("lineEdit");
注意：括号里得 lineEdit 参数为控件得 objectName ，因此这里我们可以指定任何控件，很方便….改成上述代码效果如图：
**注意： findChild()与findChildren()不支持MSVC6，因此Qt专门提供了另外两个兼容VC6得“找孩子函数”分别为：qFindChild()与qFindChildren()**
