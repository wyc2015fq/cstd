# Yoga710笔记本Win10和Ubuntu系统共存 - wishchinYang的专栏 - CSDN博客
2017年05月08日 15:48:41[wishchin](https://me.csdn.net/wishchin)阅读数：2242
       联想yoga710默认安装了win10系统，且使用EFI分区格式，安装Ubuntu不是一般的困难，经公司小哥的帮助下，几次终于完成了Ubuntu和Win10 共存。
       经过多次安装测试，暂时能运行成功的系统版本有Ubuntu14.04.02和UbuntuKylin17.04。
**安装步骤：**
1.默认使用GPT分区表，默认安装Win10系统
      如果不是GPT分区，则把所有的逻辑磁盘转化为主分区，可以[无损一键转化为GPT分区](http://blog.sina.com.cn/s/blog_53f6c6cd0102xc7b.html)。
      记住，一键转换，不损害任何数据！！！
      Fn+F2进入BIOS，关掉safeBoot选项；
      关掉双显卡，使用UMA显卡模式，使用EFI-BIOS支持，关掉Legacy支持；
2.进行硬盘管理，保留一个空闲分区，什么都不做；
3.插入Ubuntu系统U盘，重启电脑，Fn+F12，选择USB启动；
4.进去使用Ubuntu选项或者直接安装；
     到安装选项时候，选择其他安装系统，出现多个磁盘的选项。选择其他选项
![](https://img-blog.csdn.net/20170509113733006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    在空白磁盘前面开启一个ESP分区-EFI启动分区
![](https://img-blog.csdn.net/20170509113735951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    之后把空白盘格式化为ext4分区，挂载为根目录，安装Ubuntu系统到此盘
![](https://img-blog.csdn.net/20170509113739327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5.安装完成后重启。
    会发现Ubuntu启动页面第三行出现Windowsbootmanage选项，此后可以进入window系统。
