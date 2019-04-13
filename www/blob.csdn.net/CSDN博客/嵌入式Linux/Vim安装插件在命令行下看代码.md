
# Vim安装插件在命令行下看代码 - 嵌入式Linux - CSDN博客

2018年10月23日 17:37:28[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：87


这几天我又想抛弃source insight 了，主要是太慢了
## 安装如下
[VIM万能插件](https://github.com/weiqifa0/vim)
![图标](https://pic1.zhimg.com/v2-8a51f1bfb23723090f340902ec8d4a0c_ipico.jpg)

## 或者使用
sudo apt-get install**exuberant-ctags 安装**
## 我主要是使用函数跳转功能，需要记住几个指令
**1、生成tags用来实现函数跳转**
这样会生成一个tags文件，作为跳转的依据
![](https://pic4.zhimg.com/80/v2-082e3a71bc0d17ce1dd818cb9a3e263b_hd.jpg)
**2、**”**CTRL + ] “ 实现函数跳转**
![](https://pic1.zhimg.com/80/v2-c7c5b0ce2785b3094992818a412e2f28_hd.jpg)
**3、**“**CTRL + T**”**原路返回之前的函数**
![](https://pic2.zhimg.com/80/v2-d46359d080ec094c0e991a7e67bb20e1_hd.jpg)
**4、显示函数列表 ":Tlist" 窗口切换"Ctrl +ww"**
![](https://pic3.zhimg.com/80/v2-e3a12ab19f54fc7b460c8b47f3e05b56_hd.jpg)
**5、高亮显示搜索出来的字符“set hlsearch”**
![](https://pic1.zhimg.com/80/v2-4e0a4615e6573d03b0b936b004c49a18_hd.jpg)
**6、搜索并打开**
**:grep -wrn root ./**
**:copen**
**然后"Ctrl +ww" 跳转到搜索结果的窗口，到指定文件 按下回车**
**:q! 退出搜索结果窗**
![](https://pic2.zhimg.com/80/v2-22315a53da269e33393c0ca2e626c229_hd.jpg)
如果觉得有帮助，请帮忙关注微信公众号-**嵌入式Linux**

