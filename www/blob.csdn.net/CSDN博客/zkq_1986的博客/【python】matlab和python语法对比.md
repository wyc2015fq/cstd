# 【python】matlab和python语法对比 - zkq_1986的博客 - CSDN博客





2016年11月22日 20:07:53[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：3139









# matlab和python语法对比

     python+NumPy+Scipy+matplotlib = matlab

python免费，matlab收费，所以python用的人越来越多。



下面简单比较下2个语言的区别：


||matlab|python|
|----|----|----|
|逻辑行和物理行|用分号“;”disp(‘hello’);disp(‘yes’)|用分号“;”print “hello”;disp(‘yes’)|
|缩进|无要求，但是要程序有逻辑，清晰最好有缩进，尤其是同一层次用相同的缩进数目。|用缩进表示程序逻辑，要求同一层次的代码必须有相同的缩进空白数目，否则报错。def myfun1(a,b):c=a+breturn cdef myfun2(a,b):   c=a+b   return c|
|注释|%|#|
|续行|…|\|
|条件语句|if conditionelseif  conditionelseend|if  condition :elif  condition:else:|
|循环语句|for number  = 1:nendwhile  conditionendbreak 跳出整个循环continue  跳出该次循环|for  number in range(1,n) :while :break 跳出整个循环continue  跳出该次循环|
|函数定义|function  [output]=name(parameters)end|def name(parameters): return output|
|运算符|+，-，*，/,^|+,-,*,/,**|
|逻辑运算|>,<,>=,<=,==，~=|>,<,>=,<=,==,!=|
|布尔运算|&，&&|，||~|andornot|
|开始索引|1|0|
|整除问题|/直接取得准确结果如，5/3结果是1.6667|/两边是整数时候为整除5/3结果是1|
|数据结构|矩阵结构体structpatient.name  = 'John Doe';patient.billing  = 127.00;patient.test  = [79, 75, 73; 180, 178, 177.5; 220, 210, 205];元胞数组cell|列表listmylist=[‘apple’,’mango’,’carrot’,’banana’]字典dicab = { 'Swaroop' :'swaroopch@byteofpython.info','Larry' : 'larry@wall.org','Matsumoto' : 'matz@ruby-lang.org','Spammer' : 'spammer@hotmail.com'}元组zoo =  ('wolf', 'elephant', 'penguin')|







转载自：http://blog.sciencenet.cn/blog-437026-812967.html



