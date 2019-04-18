# Skywind Inside » 转换 Intel汇编格式到 AT&T汇编风格
## 转换 Intel汇编格式到 AT&T汇编风格
April 10th, 2015[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
常用 MSVC写内嵌汇编需要兼容 GCC是一件头疼的事情，不是说你不会写 GCC的 AT&T风格汇编，而是说同一份代码写两遍，还要调试两遍，是一件头疼的事情，特别是汇编写了上百行的时候。于是五年前写过一个小工具，可以方便的进行转换，能把 MSVC/MASM的汇编转成纯 AT&T风格汇编，或者 GCC Inline风格汇编，自动识别寄存器和变量，还有跳转地址，并且自动导出。今天把他放上来，或许有用到的人吧。
![](https://raw.githubusercontent.com/skywind3000/Intel2GAS/master/images/intel2gas_1.png)
项目下载：[https://github.com/skywind3000/Intel2GAS](https://github.com/skywind3000/Intel2GAS)。
支持GUI转换和命令行转换，GUI转换使用 intel2gui.py, 命令行使用 intel2gui.py，当年用 Python匆匆完成，代码写的比较烂，但是还是在我后面的项目中帮了比较大的忙，MSVC里面调试干净了，一次性转换到GCC，再也不用象以前一样人肉转换汇编，写两份代码了。
