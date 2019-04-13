
# git上传大于100M的文件 - 嵌入式Linux - CSDN博客

2019年01月15日 19:21:58[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：125



## 错误日志
Git lfs - “this exceeds GitHub’s file size limit of 100.00 MB”
## 解决办法
看这个网址
[https://git-lfs.github.com/](https://git-lfs.github.com/)
下载上面的后，然后使用下面的命令使用
git lfs install 上面下载而已，并没有安装
git lfs track “*.rar”     这个是要指定的大文件
git add .gitattributes  关联这个文件
git commit -m “aaa”

