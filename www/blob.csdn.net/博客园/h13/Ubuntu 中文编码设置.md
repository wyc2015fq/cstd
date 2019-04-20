# Ubuntu 中文编码设置 - h13 - 博客园
Ubuntu 默认采用UTF8编码，可以方便global。但对中文支持，还不细致，即便默认采用中文安装，也并不会自动添加GB＊等支持，致使在Ubuntu下访问部分Win文本文件时，出现乱码。
I. 配置系统环境
执行 sudo vi /var/lib/locales/supported.d/zh
加入以下配置参数
zh_CN.GB18030 GB18030 (最新汉字编码字符集，向下兼容GBK,GB2312)
zh_CN.GBK GBK (汉字扩展编码,向下兼容GB2312, 并包含BIG5全部汉字)
zh_CN.GB2312 GB2312 (简化汉字编码字符集, 最近有客户要我们改进GB2312,太看得起我们了，我只能说:"NO!")
zh_CN.GB18031 GB18031 (数字键盘汉字编码输入,面向手持设备，我的Nokia3120从来就是发短信，接听电话，无法和PC通讯，就不用这个了。maybe用Google Android SDK的大侠们需要这个)
zh_HK.BIG5 BIG5 (繁体)
zh_TW.BIG5 BIG5 (繁体)
然后执行 sudo locale-gen 
提示以下信息，成功了
zh_CN.GB18030... done
zh_CN.GBK... done
......
II. 系统环境支持GB*内码了，但用vi, gedit等工具访问文件还会继续乱码，需要针对不同的工具分别配置，使之自己检测支持范围内的编码
e.g. vi
执行 sudo vi /etc/vim/vimrc 
加入以下配置参数
let &termencoding=&encoding
set fileencodings=utf-8,gb18030,gbk,gb2312,big5
e.g. gedit
执行 sudo gconf-editor
选择 apps/gedit-2/preferences/encodings
找到 auto_detected 编辑，在Values中分别加入 GB18030,GBK,GB2312,BIG5
OK~
感谢lec的支援。 
enca工具很方便，用于检测文件内码（前提是系统环境支持，如，环境不支持BIG5，是无法检测的，这个晕了我许久）
安装 sudo apt-get install enca
用法 enca filename 
这个工具是在google搜索到的(http://blog.oolec.com/?p=79)，绕了地球一圈才找到，猛发现文章作者就坐在我旁边，以后再找东西先问问，别瞎跑。
-----------------------------
其他参考：
首先说一下, 这篇文章主要是针对 Java 开发者的, 如果是做 Windows 开发, 那么基本上没有办法把工作环境移植到 Ubuntu 上的(当然也不能说死, 也许以后可以使用 WINE 试试, 呵呵).
1.系统默认编码
我们知道在中文 Windows 下一般来说系统的默认编码是 GBK, 从 Java 开发的角度来说, 就是在进行诸如 Java 源代码编译, 文件读写等操作时, 默认使用的编码是 GBK; 而我们参照一般的 Ubuntu 中文安装文档, 可能会建议将默认的语言设置为 zh_CN.UTF-8, 这样会带来一些问题, 比如我在 Windows 使用 Eclipse 时的编辑器的默认编码是 GBK, 但是在 Ubuntu 下面就是 UTF-8, 同样是使用 Eclipse, 在 Windows 下的中文文档到了 Ubuntu 中会产生乱码, 必须手工逐个文档设置属性中的编码为 GBK 才可以正常; 一些测试代码也是以默认编码读取文件的, 这些代码读取出来的文件内容也会包含乱码.
所以, 最好是将系统的默认编码设置为 GBK, 这样和 Windows 下面的开发环境最接近. 设置的方法应该不止一种, 我采用的是编辑 /etc/environment, 改成这个样子:
  LANGUAGE="zh_CN:zh:en_US:en"
  GST_ID3_TAG_ENCODING=GBK
  LANG=zh_CN.GBK
  LC_CTYLE=zh_CN.GBK
2.中文字体
Ubuntu 5.10 默认安装后的中文字体不是点阵字体, 因此在小字显示的时候模模糊糊比较难看, 而且, 在浏览网页时, 无法显示中文粗体. 关于字体美化的文章网上太多了, 有些是采用 Windows 字体来进行的; 最近 "文泉驿"([http://wqy.sourceforge.net/](http://wqy.sourceforge.net/) ) 的开源点阵字体受到很多好评, 而且点阵字体正好合适用于小字的显示, 我用的是最新的 0.7 beta 版(0.6.9 pcf update 3), 按照 [文泉驿点阵宋体安装指南](http://pastora.blog.sohu.com/74094009.html) 上的步骤安装, 修改了 xorg.conf, 也执行了 fc-cache -f -v , 安装过程还算顺利, 就是有一点需要注意, 那就是字体一定要放在 /usr/share/fonts/ 的子目录下面, 不然执行 fc-cache 不会起作用的. 安装完毕后需要配置一下系统的默认字体, Firefox 也需要配置一下字体, 效果还是不错的(见下面截图).
3.Java(Swing)程序的字体
这个我主要参考了 [最简单的对JDK1.5的中文乱码处理方法](http://pastora.blog.sohu.com/74094009.html) , 按照其中更简单的那个方法, 在 $JAVA_HOME/jre/lib/fonts/ 目录下创建 fallback 目录, 并把一个中文的 TTF 字体链接或者安装到 fallback 目录下就可以了.
和参考文章说的稍有差别, 我是下载了文泉驿 0.6.9 ttf update 3 的 TTF 字体, 直接解压后复制到 fallback 目录下, 接着执行 mkfontdir 和 mkfontscale, 果然问题得以解决; 现在 NetBeans 或者 JMeter 的 "打开" 文件对话框中的按钮和标签就正确显示出中文了.
安装在 Ubuntu 上面的 NetBeans 5.0: 
存在中文问题的"打开项目"对话框: 
经过处理后可以正确显示中文的对话框: 
