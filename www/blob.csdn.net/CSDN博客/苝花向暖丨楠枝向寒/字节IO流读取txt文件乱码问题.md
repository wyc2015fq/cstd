# 字节IO流读取txt文件乱码问题 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年02月01日 17:58:06[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：460


原因一：将txt文件的内容转为IO流 与 byte[] 转 String 所用的编码不一致（byte[]：用于存读取的数据）。

原因二 : 字节流是以字节单位读取的，假设用一个 byte bytes[] = new byte[11]；去接收文本里面有4个汉字的txt文档的IO流。

此时假设编码格式为UTF-8，UTF-8存储一个汉字需要3个字节。也就是说bytes.length>=3*4 才能一次性接收，但我现在bytes数组开辟的空间是11，也就是说需要接收2次。这时候问题就产生了，一个汉字是3个字节，最后一个字的3个字节被拆成了两部分，当你打印的时候，结果显然会出问题。

原因一 解决方式：打开txt文档 点另存为，对话框最下面 可以看到该txt的编码格式![](https://img-blog.csdn.net/20180201173923084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如我的这个是ANSI（GBK），这个编码格式决定了 txt文本中的内容 转 IO 流的 编码格式。 这个格式必须跟你 .java 文件设置的编码格式一样，否则就会产生乱码。

原因一  代码演示

下面程序 演示  ， 我的a.txt 是ANSI(GBK)编码，b.txt是UTF-8编码。两个txt文本内容"测试数据"  该 .java 文件是 GBK编码。   

![](https://img-blog.csdn.net/20180201174600735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180201174720588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原因二 解决方式 ： 就是尽量别用 字节io流 去 传 带中文的 txt 文本。用字符流传。

原因二 导致的乱码  演示   

![](https://img-blog.csdn.net/20180201175413316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20180201175440343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


