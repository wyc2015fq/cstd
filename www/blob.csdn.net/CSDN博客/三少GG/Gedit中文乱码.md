# Gedit中文乱码 - 三少GG - CSDN博客
2011年10月19日 21:32:32[三少GG](https://me.csdn.net/scut1135)阅读数：1928标签：[ubuntu																[文本编辑																[图形																[终端																[string																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=文本编辑&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
# [Gedit中文乱码](http://wiki.ubuntu.org.cn/Gedit%E4%B8%AD%E6%96%87%E4%B9%B1%E7%A0%81)
### **修改配置**
![](https://www.deleak.com/blog/wp-content/uploads//2011/05/images.jpeg)gnome2
 的 “注册表编辑器” gconf-editor 正在被  dconf-editor 所取代，更多的隐藏设置也可以通过 dconf-editor 进行修改。不过目前您修改 gconf-editor 同样生效。
> 
使用 dconf 是GNOME 3的若干改进之一（倒退？！），由于dconf 的配置文件是二进制的，只能通过dconf-editor来编辑。
**Ubuntu 11.10版本**
step 1.安装dconf-editor
           //不可以 sudo apt-get install dconf-editor
替代方法
安装dconf-tools
sudo apt-get install dconf-tools
step 2.运行dconf-editor
          sudo dconf-editor
step 3.加入GB18030或者GB2312编码
       ->org->gnome->gedit->preferences->encodings
       ->auto-detected的value项中加入’GB18030′或者GB2312，写在第一位；
       ->show-in-menu的value项中加入’GB18030′或者GB2312，写在第一位
**本人验证上面到方法到最后一步时无法写入gb2312等**
成功方法：
**gedit 中文字**乱码****
**ubuntu11.10** 开始默认 GSettings （基于命令行）作为系统的配置工具，要解决 gedit 的中文字**乱码**，只需在终端执行：
**gsettings set org.gnome.gedit.preferences.encodings auto-detected "['UTF-8', 'GB18030', 'GB2312', 'GBK', 'BIG5', 'CURRENT', 'UTF-16']"**
**Ubuntu 10.10以下版本：**
### 出自Ubuntu中文
缺省配置下，用 Ubuntu 的文本编辑器（gedit）打开 GB18030/GBK/GB2312 等类型的中文编码文本文件时，将会出现乱码。
出现这种情况的原因是，gedit 使用一个编码匹配列表，只有在这个列表中的编码才会进行匹配，不在这个列表中的编码将显示为乱码。您要做的就是将 GB18030 加入这个匹配列表。
- 命令行方式，适用于所有 Ubuntu 用户。
复制以下命令到终端中，然后回车即可：
gconftool-2 --set --type=list --list-type=string /apps/gedit-2/preferences/encodings/auto_detected "[UTF-8,CURRENT,GB18030,BIG5-HKSCS,UTF-16]"
- 图形化方式，适用于 Ubuntu 用户，而不适用于 KUbuntu/XUbuntu 用户。
您可以遵循以下步骤，使您的 gedit 正确显示中文编码文件。
- 按下 Alt-F2，打开“运行应用程序”对话框。
- 在文本框中键入“gconf-editor”，并按下回车键，打开“配置编辑器”。
- 展开左边的树节点，找到 /apps/gedit-2/preferences/encodings 节点并单击它。
- 双击右边的 auto_detected 键，打开“编辑键”对话框。
- 单击列表右边的“添加”按钮，输入“GB18030”，单击确定按钮。
- 列表的最底部新增加了一个“GB18030”。单击选中它，并单击右边的 “向上” 按钮直到 “GB18030” 位于列表的顶部为止。
- 单击确定按钮，关闭配置编辑器。
本人采用图形化方式，成功可显示中文！ 2011.10.19
