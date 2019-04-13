
# error MSB6006: “LC.exe”已退出 - 追求卓越,做到专业 - CSDN博客


2015年10月22日 11:01:01[Waldenz](https://me.csdn.net/enter89)阅读数：4774


最近一个c\#工程，之前编译正常。后重装系统，安装DevExpress后，编译的过程中，什么错误也没提示，只提示"4>C:\Windows\Microsoft.NET\Framework\v4.0.30319\Microsoft.Common.targets(2401,5): error MSB6006: “LC.exe”已退出，代码为 -1。",真好的诡异，后来想到把工程下licenses.licx文件内容清空，就解决问题了！

