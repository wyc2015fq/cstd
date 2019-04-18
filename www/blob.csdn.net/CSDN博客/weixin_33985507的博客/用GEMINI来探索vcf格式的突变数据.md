# 用GEMINI来探索vcf格式的突变数据 - weixin_33985507的博客 - CSDN博客
2018年01月17日 20:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：22
第一次听说这个软件，是一个香港朋友推荐的：
> 
[http://davetang.org/muse/2016/01/13/getting-started-with-gemini/](https://link.jianshu.com?t=http%3A%2F%2Fdavetang.org%2Fmuse%2F2016%2F01%2F13%2Fgetting-started-with-gemini%2F)
他写的很棒，但是我当初以为是一个类似于SQLite的数据库浏览模式，所以没在意。实际上，我现在仍然觉得这个软件没什么用！
软件[官网](https://link.jianshu.com?t=https%3A%2F%2Fgemini.readthedocs.io%2Fen%2Flatest%2F)有详细的介绍。
而且提供丰富的 **教程**：
We recommend that you follow these tutorials in order, as they introduce concepts that build upon one another.
- Introduction to GEMINI, basic variant querying and data exploration. [html](https://link.jianshu.com?t=https%3A%2F%2Fspeakerdeck.com%2Farq5x%2Fan-introduction-and-tutorial-for-variant-exploration-with-gemini)[pdf](https://link.jianshu.com?t=https%3A%2F%2Fs3.amazonaws.com%2Fgemini-tutorials%2FIntro-To-Gemini.pdf)
- Identifying de novo mutations underlying Mendelian disease [html](https://link.jianshu.com?t=https%3A%2F%2Fspeakerdeck.com%2Farq5x%2Fidentifying-de-novo-mutations-with-gemini)[pdf](https://link.jianshu.com?t=https%3A%2F%2Fs3.amazonaws.com%2Fgemini-tutorials%2FGemini-DeNovo-Tutorial.pdf)
- Identifying autosomal recessive variants underlying Mendelian disease [html](https://link.jianshu.com?t=https%3A%2F%2Fspeakerdeck.com%2Farq5x%2Fidentifying-recessive-candidates-with-gemini)[pdf](https://link.jianshu.com?t=https%3A%2F%2Fs3.amazonaws.com%2Fgemini-tutorials%2FGemini-Recessive-Tutorial.pdf)
- Identifying autosomal dominant variants underlying Mendelian disease [html](https://link.jianshu.com?t=https%3A%2F%2Fspeakerdeck.com%2Farq5x%2Fidentifying-dominant-candidates-with-gemini)[pdf](https://link.jianshu.com?t=https%3A%2F%2Fs3.amazonaws.com%2Fgemini-tutorials%2FGemini-Dominant-Tutorial.pdf)
- Other GEMINI tools [html](https://link.jianshu.com?t=https%3A%2F%2Fspeakerdeck.com%2Farq5x%2Fother-gemini-tools)[pdf](https://link.jianshu.com?t=https%3A%2F%2Fs3.amazonaws.com%2Fgemini-tutorials%2FGEMINI-Other-Tools.pdf)
软件本身并不提供注释，虽然它的功能的确包括注释，号称可以利用 **(ENCODE tracks, UCSC tracks, OMIM, dbSNP, KEGG, and HPRD.)**对你的突变位点注释，**比如你输入 1861389.CT,它告诉你这个突变发生在哪个基因，对蛋白改变如何？是否会产生某些疾病？**
虽然它本身没有注释功能，但是它会调用`snpEFF`或者`VEP`进行注释，你需要自己先学习它们。
![7027828-a1b361d21053f06a.png](https://upload-images.jianshu.io/upload_images/7027828-a1b361d21053f06a.png)
1
### 软件安装：
**GEMINI**是用python写的，有一个小脚本可以自动完成安装过程：
```bash
7.3K May  4 14:44 gemini_install.py
```
下载这个脚本，然后安装即可
```bash
wget https://github.com/arq5x/gemini/raw/master/gemini/scripts/gemini_install.py
python gemini_install.py $tools $data
PATH=$tools/bin:$data/anaconda/bin:$PATH
where *$tools* and *$data* are paths writable on your system.
```
我把`$tools`用的就是当前文件夹，`$data`也是当前文件夹下面的gemini文件夹。
这样就会在当前文件夹下面生成两个文件夹，`bin`是存储程序，`gemini`是存储数据用的，而且注意要把`bin`目录的全路径添加到 **环境变量！**
### 输入数据：
我们可以直接下载软件作者提供的测试数据
首先是22号染色体的所有突变位点经过WEP注释的文件
然后是一个三口之家的突变ped格式数据
数据存放在亚马逊云，所有的教程pdf也在
> 
[http://s3.amazonaws.com/gemini-tutorials](https://link.jianshu.com?t=http%3A%2F%2Fs3.amazonaws.com%2Fgemini-tutorials)
如果是你自己的vcf文件，需要自己用VEP注释一下
![7027828-96dde13c1df57095.png](https://upload-images.jianshu.io/upload_images/7027828-96dde13c1df57095.png)
1
### 运行命令：
![7027828-137fdfacdf5245c9.png](https://upload-images.jianshu.io/upload_images/7027828-137fdfacdf5245c9.png)
2
### 结果解读：
产生是chr22.db就是一个数据库格式的文件，但是需要用 gemini 来进行查询，个人认为，并没有多大意思！
你只要熟悉mySQL等SQL语言，完全可以自己来！
