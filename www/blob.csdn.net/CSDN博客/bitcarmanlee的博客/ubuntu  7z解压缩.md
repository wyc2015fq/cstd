
# ubuntu .7z解压缩 - bitcarmanlee的博客 - CSDN博客


2018年11月20日 14:57:40[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：65标签：[7z																](https://so.csdn.net/so/search/s.do?q=7z&t=blog)[解压缩																](https://so.csdn.net/so/search/s.do?q=解压缩&t=blog)[ubuntu																](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=解压缩&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=7z&t=blog)个人分类：[linux shell																](https://blog.csdn.net/bitcarmanlee/article/category/6148977)
[
																								](https://so.csdn.net/so/search/s.do?q=7z&t=blog)


kaggle上很多数据集都是.7z的格式。下载过来以后第一步是需要解压。
首先需要安装p7zip
`sudo apt-get install p7zip-full`然后解压文件:
`7z x xxx.7z -r -o localdir`其中，x标识解压缩文件，并按原始目录解压
-r 表示递归所有的子文件夹
当然最简单的方式就是
`7z x xxx.7z`这样直接将xxx.7z文件解压到当前路径

