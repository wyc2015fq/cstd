# 使用replace pioneer批量修改文件名 - wishchinYang的专栏 - CSDN博客
2015年04月13日 18:07:15[wishchin](https://me.csdn.net/wishchin)阅读数：1238
    shell的正则表达式还是很难记忆的，也没有沉静的心情看文档，于是使用了replace pioneer。
1.  启动replace pioneer，Tools—>batch runner  ，在 set out file name 填入#{198701,16}.jpg；
         结果为  文件以198701开始，以16起跳，以此加16命名文件。
2.在 set out file name 填入${FILENAME}{replace,".jpgface",""}；
        结果为 把 所有字符串 “.jpgface” 替换为空格，即去掉！
