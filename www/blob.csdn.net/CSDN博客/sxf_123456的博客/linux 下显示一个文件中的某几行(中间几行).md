# linux 下显示一个文件中的某几行(中间几行) - sxf_123456的博客 - CSDN博客
2017年09月26日 20:42:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：172
                
linux 下显示一个文件中的某几行(中间几行)
1、从100行开始，显示200行；即显示100-299行
cat filename | tail -n +100 | head -n 200
2、显示100行到300行
cat filename | head -n 300 | tail -n 100
3、显示最后100行
tail -n 100
4、显示从1000行开始显示，显示1000行以后的
tail -n +1000
5、显示前面1000行
head -n 1000 
6、用sed命令
查看文件的第5行到第10行内容
sed -n '5,10p' filename
            
