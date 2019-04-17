# Rhythmbox中文乱码 - xiahouzuoxin - CSDN博客





2013年07月03日 15:05:31[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1015
个人分类：[Linux](https://blog.csdn.net/xiahouzuoxin/article/category/1286747)








建立一个Rhythmbox播放器的图标到Panel或者桌面或者任何可以修改图标内容的地方



解决：


图标上，右键 -> 属性 -> 命令 改为 env GST_ID3_TAG_ENCODING=GBK rhythmbox %U

(原为rhythmbox %U)


然后以新的图标启动rhythmbox,清空原有的媒体库,重新导入文件/目录即可.


