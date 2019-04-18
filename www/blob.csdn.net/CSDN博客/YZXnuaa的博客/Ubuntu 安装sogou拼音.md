# Ubuntu 安装sogou拼音 - YZXnuaa的博客 - CSDN博客
2018年04月23日 14:58:10[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：199
首先，下载搜狗拼音安装包。 
[搜狗拼音for Linux官方下载链接](https://pinyin.sogou.com/linux/?r=pinyin)
官方安装教程是直接下载，点击安装deb包，然后就可以直接重启使用。 
但是根据教程我没有成功。
```bash
cd Downloads  #切换到下载目录
sudo apt-get install -f
sudo dpkg -i sogoupinyin_2.1.0.0086_amd64.deb# 如果没有出错就说明安装成功了
```
- 1
- 2
- 3
安装成功以后，需要进行显示。 
在左上角搜索‘fcitx configuration’ 
![搜索fcitx configuration](https://img-blog.csdn.net/20170829214142786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWlhb2ppNjA3Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点开之后，先选择左下角的＋号 
![这里写图片描述](https://img-blog.csdn.net/20170829214448335?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWlhb2ppNjA3Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后，在add input method 页面中，将only show current language（只显示当前系统的语言），因为是装的英文系统，这里需要搜索中文输入法，就需要将勾去掉。
然后，在下面搜索框内，输入‘Sogou’就可以看到，然后选中。
最后，可以使用shift切换出搜狗拼音了。
