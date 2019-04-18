# 在ubuntu下如何搜索文件? - gauss的专栏 - CSDN博客
2015年04月23日 21:19:34[gauss](https://me.csdn.net/mathlmx)阅读数：907
　　1.whereis [文件](/Soft/Tools/File/index.html)名
　　特点:快速,但是是模糊查找,例如 找 #whereis my[sql](/Dev/DataBase/MsSql/index.html) 它会把my[sql](/Dev/DataBase/MsSql/index.html),my[sql](/Dev/DataBase/MsSql/index.html).ini,my[sql](/Dev/DataBase/MsSql/index.html).*所在的目录都找出来.我一般的查找都用这条命令.
　　2.find / -name [文件](/Soft/Tools/File/index.html)名
　　特点:准确,但速度慢,消耗资源大,例如我想找到php.ini的准确位置,就需要用
　　#find / -name php.ini
　　3.locate [文件](/Soft/Tools/File/index.html)名
　　强力推荐的方法,最快,最好的方法.
