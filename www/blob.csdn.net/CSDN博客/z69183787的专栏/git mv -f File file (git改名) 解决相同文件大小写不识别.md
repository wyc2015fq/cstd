# git mv -f File file (git改名) 解决相同文件大小写不识别 - z69183787的专栏 - CSDN博客
2016年11月30日 17:33:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：825
git mv -f oldFile newFile
本地
git config core.ignorecase false 
代实验：
git config core.filemode false 
