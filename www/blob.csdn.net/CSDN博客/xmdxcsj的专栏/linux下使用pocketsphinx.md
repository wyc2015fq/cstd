# linux下使用pocketsphinx - xmdxcsj的专栏 - CSDN博客





2015年01月20日 22:39:34[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1196








1.下载路径

[http://sourceforge.net/projects/cmusphinx/files/](http://sourceforge.net/projects/cmusphinx/files/)




Pocketsphinx —用C语言编写的轻量级识别库，主要是进行识别的。




Sphinxbase — Pocketsphinx所需要的支持库，主要完成的是语音信号的特征提取；




Sphinx3 —为语音识别研究用C语言编写的解码器




Sphinx4 —为语音识别研究用JAVA语言编写的解码器




CMUclmtk —语言模型训练工具




Sphinxtrain —声学模型训练工具




2.安装步骤




#./configure




#make




# make install




3.使用

需要在脚本添加以下说明：

export LD_LIBRARY_PATH=/usr/local/lib  #库文件的搜索路径

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig  #库文件的编译链接参数设置文件



