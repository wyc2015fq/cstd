# shell中for循环用法 - ljx0305的专栏 - CSDN博客
2009年06月15日 17:15:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1798
shell语法好麻烦的，一个循环都弄了一会 ，找了几个不同的方法来实现输出1－100间可以被3整除的数
1.用(())
#!/bin/bash
clear
for((i=1;i<100;i++))
for
        do
        if((i%3==0))
        then
        echo $i
        continue
        fi
        done
2.使用`seq 100`
#!/bin/bash
clear
for i in `seq 100`
        do
        if((i%3==0))
        then
        echo $i
        continue
        fi
        done
3.使用while
#!/bin/bash
clear
i=1
while(($i<100))
do
        if(($i%3==0))
        then
        echo $i
        fi
        i=$(($i+1))
done
引用:http://qq164587043.blog.51cto.com/261469/48873
