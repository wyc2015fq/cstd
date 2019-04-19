# AD封装库转成pads封装库 - xqhrs232的专栏 - CSDN博客
2015年03月18日 23:36:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5941
原文地址::[http://yuandi6.blog.163.com/blog/static/2072651852012101384222263/](http://yuandi6.blog.163.com/blog/static/2072651852012101384222263/)
相关文章
1、AD、Pads、Allegro、Mentor间的转换、复用及设计----[http://wenku.baidu.com/link?url=GraoiNTlFNk64D1xC8f6sdy--BQKxb63rM9J2knSFJcrWI1YL--l4w9mUN6It-sJKv0uB4tSFuDrKzl7khfupcs2zudjfo-mgeBaLRZ0lv3](http://wenku.baidu.com/link?url=GraoiNTlFNk64D1xC8f6sdy--BQKxb63rM9J2knSFJcrWI1YL--l4w9mUN6It-sJKv0uB4tSFuDrKzl7khfupcs2zudjfo-mgeBaLRZ0lv3)

1、使用AD6打开AD封装库，CTRL+A复制所有的元件。file-new一个新的PCB文件，粘贴所有的元件到PCB中。
当元件比较多时候，可以shift+选中一部分元件粘贴到PCB文件中。
注意：保存的时候，把这个PCB文件保存为4.0即protel 99se格式。
![AD封装库转成pads封装库 - 红樱桃 - 红樱桃之家](http://img8.ph.126.net/_-Xdva4dyJ06m9jkZjHKZQ==/6597806439447115338.jpg)
 2、使用pads layout  translator进行转换，把99se格式pcb加载。
3、转好后，打开pads layout文件，select component，ctrl+A所有的元件，右键save to library，选中保存在哪个库中即可。
想放在一个空的库中，可create  library一个新的库加载即可。
