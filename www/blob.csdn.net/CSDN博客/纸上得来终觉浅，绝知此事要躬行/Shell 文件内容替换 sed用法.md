# Shell 文件内容替换 sed用法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月08日 23:07:52[boonya](https://me.csdn.net/boonya)阅读数：835








sed是一个很好的文件处理工具，本身是一个管道命令，主要是以行为单位进行处理，可以将数据行进行替换、删除、新增、选取等特定工作，下面先了解一下sed的用法。

调用sed命令有两种形式：

```bash
sed [options] 'command' file(s)

sed [options] -f scriptfile file(s)
```

 常用选项：



       -n∶使用安静(silent)模式。在一般sed 的用法中，所有来自 STDIN的资料一般都会被列出到萤幕上。但如果加上 -n 参数后，则只有经过sed特殊处理的那一行(或者动作)才会被列出来。

       -e∶直接在指令列模式上进行 sed 的动作编辑；

       -f∶直接将 sed 的动作写在一个档案内， -f filename 则可以执行 filename 内的sed动作；

       -r∶sed 的动作支援的是延伸型正规表示法的语法。(预设是基础正规表示法语法)

       -i∶直接修改读取的档案内容，而不是由萤幕输出。       



常用命令：



        a∶新增， a 的后面可以接字串，而这些字串会在新的一行出现(目前的下一行)～

        c∶取代， c 的后面可以接字串，这些字串可以取代n1,n2 之间的行！

        d∶删除，因为是删除啊，所以 d后面通常不接任何咚咚；

        i∶插入， i的后面可以接字串，而这些字串会在新的一行出现(目前的上一行)；

        p∶列印，亦即将某个选择的资料印出。通常 p 会与参数 sed-n 一起运作～

        s∶取代，可以直接进行取代的工作哩！通常这个 s的动作可以搭配正规表示法！例如 1,20s/old/new/g 就是啦！

举例：（假设我们有一文件名为abc.file）

```bash
删除某行
 
     [root@localhost ruby] # sed '1d' abc.file  
 
      #删除第一行
 
     [root@localhost ruby] # sed '$d' abc.file
 
      #删除最后一行
 
     [root@localhost ruby] # sed '1,2d' abc.file
 
     #删除第一行到第二行
 
     [root@localhost ruby] # sed '2,$d' abc.file
 
     #删除第二行到最后一行
 
显示某行
 
.    [root@localhost ruby] # sed -n '1p' abc.file
 
      #显示第一行
 
     [root@localhost ruby] # sed -n '$p' abc.file           
 
      #显示最后一行
 
     [root@localhost ruby] # sed -n '1,2p' abc.file       
 
     #显示第一行到第二行
 
     [root@localhost ruby] # sed -n '2,$p' abc.file
 
      #显示第二行到最后一行
 
使用模式进行查询
 
     [root@localhost ruby] # sed -n '/ruby/p' abc.file
 
    #查询包括关键字ruby所在所有行
 
     [root@localhost ruby] # sed -n '/\$/p' abc.file
 
    #查询包括关键字$所在所有行，使用反斜线\屏蔽特殊含义
 
增加一行或多行字符串
 
     [root@localhost ruby]# cat abc.file
     Hello!
     ruby is me,welcome to my blog.
     end
     [root@localhost ruby] # sed '1a drink tea' abc.file  
 
    #第一行后增加字符串"drink tea"
 
     Hello!
     drink tea
     ruby is me,welcome to my blog.
     end
     [root@localhost ruby] # sed '1,3a drink tea' abc.file
 
    #第一行到第三行后增加字符串"drink tea"
     Hello!
     drink tea
     ruby is me,welcome to my blog.
     drink tea
     end
     drink tea
     [root@localhost ruby] # sed '1a drink tea\nor coffee' abc.file
 
   #第一行后增加多行，使用换行符\n
     Hello!
     drink tea
     or coffee
     ruby is me,welcome to my blog.
     end
 
代替一行或多行
 
     [root@localhost ruby] # sed '1c Hi' abc.file                
 
    #第一行代替为Hi
 
     Hi
     ruby is me,welcome to my blog.
     end
     [root@localhost ruby] # sed '1,2c Hi' abc.file
 
     #第一行到第二行代替为Hi
 
     Hi
     end
 
替换一行中的某部分格式：sed 's/要替换的字符串/新的字符串/g'   （要替换的字符串可以用正则表达式）

   #替换ruby为bird
 
  [root@localhost ruby] # sed -n '/ruby/p' abc.file | sed 's/ruby/bird/g'
  #删除ruby    
 
  [root@localhost ruby] # sed -n '/ruby/p' abc.file | sed 's/ruby//g'  
 
   
 
插入
 
     [root@localhost ruby] # sed -i '$a bye' abc.file
 
     #在文件abc.file中最后一行直接输入"bye"
 
     [root@localhost ruby]# cat abc.file
     Hello!
     ruby is me,welcome to my blog.
     end
     bye
```








