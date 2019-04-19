# Linux shell文件读写的几种方法 - fanyun的博客 - CSDN博客
2017年04月04日 19:35:32[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3775
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
Linux shell文件操作还是非常高效的，下面介绍几种常用方法：
方法一：文件描述符法（也是最常用的方法）
 特点：最常用
```python
Function while_read_line_fd(){
Exec 3<&0
Exec 0<$FILENAME
While read LINE
Do 
 Echo $LINE
 Exec 0<&<3
}
```
        注释：这种方法分2步骤，第一，通过将所有内容重定向到文件描述符3来关闭文件描述符0.为此我们用了语法Exec 3<&0 。第二部将输入文件放送到文件描述符0，即标准输入。
方法二：while循环法
特点：效率最高
```python
function while_read_LINE_bottm(){
 While read LINE
 do
echo $LINE
done  < $FILENAME
}
```
        注释：我习惯把这种方式叫做read釜底抽薪，因为这种方式在结束的时候需要执行文件，就好像是执行完的时候再把文件读进去一样。
方法三 ：重定向法；
特点：管道法: cat $FILENAME | while read LINE，使用简单，效率较低
```python
Function While_read_LINE(){
 cat $FILENAME | while read LINE
 do 
echo $LINE
done
}
```
          注释：我只所有把这种方式叫做管道法，相比大家应该可以看出来了吧。当遇见管道的时候管道左边的命令的输出会作为管道右边命令的输入然后被输入出来。
方法四： for  循环。
特点：高效易用
```python
function  for_in_file(){
For  i  in  `cat $FILENAME`
do
echo $i
done
}
```
