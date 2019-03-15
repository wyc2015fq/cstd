# MySQL基础 -- 关系代数

2018年07月14日 11:40:47 [ls秦](https://me.csdn.net/qq_38328378) 阅读数：344



 版权声明：	https://blog.csdn.net/qq_38328378/article/details/81036115

### MySQL基础 -- 关系代数

​       关系代数是一种**抽象的查询语言**，它用对关系的运算来表达查询。
​       任何一种运算都是将一定的运算符作用于一定的运算对象上，得到预期的结果。所以运算对象、运算符、运算结果是运算的三大要素。
​       按运算符的不同分为传统的集合运算和专门的关系运算两类：
​              传统的集合运算包括：并（∪）、差（−）、交（∩）、笛卡尔积（×）。 
​              专门的关系运算包括：选择（σ）、投影（π）、连接（⋈）、除运算（÷）。

![img](https://img-blog.csdn.net/20180713191434585?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

### MySQL基础 -- 传统的集合运算

​       传统的集合运算是二目运算，并（∪）、差（−）、交（∩）、笛卡尔积（×）四种运算。
​       设关系 R 和关系 S 具有相同的目 n（即两个关系都有 n 个属性），且相应的的属性取自同一个域，t 是元组变量，t∈R 表示 t 是 R 的一个元组。
​       下图分别是具有三个属性列的关系 R、S ：

​                                                   **![img](https://img-blog.csdn.net/20180713210523625?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)                     ![img](https://img-blog.csdn.net/20180713210535844?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

​       可以定义并、差、交、笛卡尔积运算如下：

​       **1、并（union）**

​       关系 R 与关系 S 的并由属于 R 且属于 S 的[元组](https://baike.baidu.com/item/%E5%85%83%E7%BB%84)组成。其结果关系仍为 n 目关系。记作：

 

![img](https://img-blog.csdn.net/20180714005437406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       下图为关系 R 与关系 S 的并：

![img](https://img-blog.csdn.net/20180713210741308?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **2、差（except）**

​       关系R与关系S的差由属于R而不属于S的所有元组组成。其结果关系仍为n目关系。记作：

 

![img](https://img-blog.csdn.net/20180714005814664?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       下图为关系R与关系S的差：

![img](https://img-blog.csdn.net/20180713210805880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **3、交（intersection）**

​       关系R与关系S的交由既属于R又属于S的元组组成。其结果关系仍为n目关系。记作：

 

![img](https://img-blog.csdn.net/20180714005943264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       下图为关系R与关系S的交：

![img](https://img-blog.csdn.net/20180713210943464?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **4、笛卡尔积（cartesian product）**

​       这里的笛卡尔积严格地讲是广义笛卡尔积(Extended Cartesian Product)。在不会出现混淆的情况下广义笛卡尔积也称为笛卡尔积。
​       两个分别为n目和m目的关系R和S的广义笛卡尔积是一个(n+m)列的[元组](https://baike.baidu.com/item/%E5%85%83%E7%BB%84)的集合。元组的前n列是关系R的一个元组，后m列是关系S的一个元组。若R有k1个元组，S有k2个元组，则关系R和关系S的广义笛卡尔积有k1×k2个元组。
​       记作：

![img](https://img-blog.csdn.net/20180714011216494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       下图为关系R与关系S的笛卡尔积：

![img](https://img-blog.csdn.net/20180713211015130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### MySQL基础 -- 专门的关系运算

​       专门的关系运算(Specific relation operations)包括选择、投影、连接、除等。

​       为了叙述上的方便，我们先引入几个记号：

​       1、设关系模式为![R\left (A_{1},A_{2},...,A_{n} \right )](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20R%5Cleft%20%28A_%7B1%7D%2CA_%7B2%7D%2C...%2CA_%7Bn%7D%20%5Cright%20%29)。它的一个关系设为R。t∈R表示t是R的一个[元组](https://baike.baidu.com/item/%E5%85%83%E7%BB%84)。![t[A_{i}]](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20t%5BA_%7Bi%7D%5D)则表示元组t中相应于属性![A_{i}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20A_%7Bi%7D)的一个分量 。
​       2、若![A=\left \{ A_{i1},A_{i2},...,A_{ik} \right \}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20A%3D%5Cleft%20%5C%7B%20A_%7Bi1%7D%2CA_%7Bi2%7D%2C...%2CA_%7Bik%7D%20%5Cright%20%5C%7D)，其中![A_{i1},A_{i2},...,A_{ik}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20A_%7Bi1%7D%2CA_%7Bi2%7D%2C...%2CA_%7Bik%7D)是![A_{1},A_{2},...,A_{n}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20A_%7B1%7D%2CA_%7B2%7D%2C...%2CA_%7Bn%7D)中的一部分，则A称为属性列或域列。![\overline{A}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Coverline%7BA%7D)则表示![\left \{ A_{1},A_{2},...,A_{n} \right \}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Cleft%20%5C%7B%20A_%7B1%7D%2CA_%7B2%7D%2C...%2CA_%7Bn%7D%20%5Cright%20%5C%7D)中去掉![\left \{ A_{i1},A_{i2},...,A_{ik} \right \}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Cleft%20%5C%7B%20A_%7Bi1%7D%2CA_%7Bi2%7D%2C...%2CA_%7Bik%7D%20%5Cright%20%5C%7D)后剩余的属性组。![t\left [ A \right ] = \left ( t\left [ A_{i1} \right ],t\left [ A_{i2} \right ],...t\left [ A_{ik} \right ] \right )](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20t%5Cleft%20%5B%20A%20%5Cright%20%5D%20%3D%20%5Cleft%20%28%20t%5Cleft%20%5B%20A_%7Bi1%7D%20%5Cright%20%5D%2Ct%5Cleft%20%5B%20A_%7Bi2%7D%20%5Cright%20%5D%2C...t%5Cleft%20%5B%20A_%7Bik%7D%20%5Cright%20%5D%20%5Cright%20%29)表示元组t在属性列A上诸分量的集合。
​       3、R为n目关系，S为m目关系。设![t_{r}\in R](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Cbg_white%20t_%7Br%7D%5Cin%20R)，![\bg_white t_{s}\in S](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Cbg_white%20t_%7Bs%7D%5Cin%20S)，则![\widehat{t_{r}t_{s}}](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Cwidehat%7Bt_%7Br%7Dt_%7Bs%7D%7D) 称为元组的连接（Concatenation）。它是一个(n+m)列的元组，前n个分量为R中的一个n元组，后m个分量为S中的一个m元组。
​       4、给定一个关系R(X,Z)，X和Z为属性组。我们定义，当t[X]=x时，x在R中的[象集](https://baike.baidu.com/item/%E8%B1%A1%E9%9B%86)（Images Set）为：

![img](https://img-blog.csdn.net/20180714005301169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       x在R中的像集为R中Z属性对应分量的集合，而这些分量所对应的元组中的属性组X上的值为x。

​       例如： x1在R中的像集Z(x1)={Z1，Z2，Z3}， 
​                  x2在R中的像集Z(x2)={Z2，Z3}，
​                  x3在R中的像集Z(x3)={Z1，Z3}。

![img](https://img-blog.csdn.net/20180713214319343?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **1、选择（selection）**

​       选择又称为限制（Restriction）。它是在关系R中选择满足给定条件的诸元组，记作：

![img](https://img-blog.csdn.net/20180714004427700?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       其中F表示选择条件，它是一个[逻辑表达式](https://baike.baidu.com/item/%E9%80%BB%E8%BE%91%E8%A1%A8%E8%BE%BE%E5%BC%8F)，取逻辑值‘真’或‘假’。
​       逻辑表达式F的基本形式为：

 

![img](https://img-blog.csdn.net/20180714004909748?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       θ表示比较运算符，它可以是>、≥、<、≤、=或<>。X1、Y1等是属性名或[常量](https://baike.baidu.com/item/%E5%B8%B8%E9%87%8F)或简单函数。属性名也可以用它的序号来代替。φ表示[逻辑运算符](https://baike.baidu.com/item/%E9%80%BB%E8%BE%91%E8%BF%90%E7%AE%97%E7%AC%A6)，它可以是非（┓）、与（∧）、或（∨）。[ ]表示任选项，即[ ]中的部分可以要也可以不要，...表示上述格式可以重复下去。    
​       选择运算实际上是从关系R中选取使得逻辑表达是F为真的组。这是从行的角度进行的运算。
​       条件表达式中的运算符如表所示：

![img](https://img-blog.csdn.net/20180713220552527?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       【例】设有一个学生-课程数据库，包括学生关系Student、课程关系Course和选修关系SC。如图所示：

![img](https://img-blog.csdn.net/20180713224741662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

​       【例】查询[信息系](https://www.baidu.com/s?wd=%E4%BF%A1%E6%81%AF%E7%B3%BB&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)（IS系）全体学生：

![img](https://img-blog.csdn.net/20180714004019213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```sql
select * form Student where Sdept = 'IS'
```

​       查询结果：

![img](https://img-blog.csdn.net/20180713224357820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

 

​       【例】查询年龄小于20岁的学生：

![img](https://img-blog.csdn.net/20180714004106588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```sql
select  * form Student where Sage < 20
```

​       查询结果：

![img](https://img-blog.csdn.net/20180713225223704?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

​       **2、投影（projection）**

​       关系R上的投影是从R中选择出若干属性列组成新的关系。记作：

![img](https://img-blog.csdn.net/20180714003257172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       其中A为R中的属性列。投影操作是从列的角度进行运算。
​       【例】 查询学生的姓名和所在系，即求Student关系上学生姓名和所在系两个属性上的投影：

![img](https://img-blog.csdn.net/20180714003427774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```sql
select  Student.Sname,Student.Sdept form Student
```

​       结果如下图所示，投影之后不仅取消了原关系的某些列，而且还可能取消某些[元祖](https://www.baidu.com/s?wd=%E5%85%83%E7%A5%96&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，因为取消了某些属性之后，就可能出现重复行，应取消这些完全相同的行。

![img](https://img-blog.csdn.net/20180713225642432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       【例】查询学生关系Student中都有那些系，即查询关系Student上所在系属性上的投影：

![img](https://img-blog.csdn.net/20180714003448958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```sql
select Student.Sdept form Student
```

​       结果如下图所示，Student关系原来有4个元组，而投影结果取消了重复的CS元组，因此只有三个元组：

![img](https://img-blog.csdn.net/20180713225521297?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **3、除（division）**

​       除法运算是一个复合的二目运算。如果把笛卡尔积看作“乘法”运算，则除法运算可以看作这个“乘法”的逆运算。
​       给定关系R(X，Y)和S(Y，Z)，其中X、Y、Z为属性组。R中的Y与S中的Y可以有不同的属性名，但必须出自相同的域集。R与S的除运算得到一个新的关系P(X)，P是R中满足下列条件的元组在X属性列上的投影：元组在X上的分量值x的像集YX包含S在Y上投影的集合。记作：

![img](https://img-blog.csdn.net/20180714003005981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       其中，![img](https://img-blog.csdn.net/20180714000822826?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)为x在R中的像集，![x=t_{r}\left [ X \right ]](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20x%3Dt_%7Br%7D%5Cleft%20%5B%20X%20%5Cright%20%5D)。显然，除操作是同时从行和列的角度进行运算。

​       根据关系运算的除法定义，可以得出它的运算步骤。
​       (1) 将被除关系的属性分为像集属性和结果属性两部分；与除关系相同的属性属于像集属性；不相同的属性属于结果属性。
​       (2) 在除关系中，对像集属性投影，得到除目标数据集。
​       (3) 将被除关系分组。分组原则是：结果属性值一样的元组分为一组。
​       (4) 逐一考察每个组，如果它的像集属性值中包括目标数据集，则对应的结果属性应属于该除法运算结果集。

 

​       【例】在关系R中，A可以去4个值{a1，a2，a3，a4}，其中：

​       a1的象集为{（b1，c2），（[b2](https://www.baidu.com/s?wd=b2&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，c3），（b2，c1）}；
​       a2的象集为{（b3，c7），（b2，c3）}；
​       a3的象集为{（b4，c6）}；
​       a4的象集为{（b6，c6）}；
​       S在（B，C）上的投影为{（b1，c2），（b2，c1），（b2，c3）}；
​       显然只有a1的象集![img](https://img-blog.csdn.net/20180714111515779?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)包含了S在（B，C）属性组上的投影，所以 R÷S = { a1 }。

![img](https://img-blog.csdn.net/20180713234112790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **4、连接（join）**

​       连接也称为θ连接，关系R与关系S的连接运算是从两个关系的广义笛卡尔积中选取属性间满足一定条件的元组形成一个新的连接：

![img](https://img-blog.csdn.net/20180714092600426?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​         其中： A为包含R中的属性的表达式；
​                     B为包含S中的属性的表达式；
​                     θ通常为关系比较符。

​       **（1）等值连接（equi join）**

​       θ在“=”时的连接为等值连接。它是从关系R和S的广义笛卡尔积中选取A、B属性值相等的那些元组，即等值连接为：

![img](https://img-blog.csdn.net/20180714095040586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **（2）自然连接（natural join）**

​       自然连接是一种特殊的等值链接，它要求两个关系中进行比较的分量必须是相同的属性组，并且在结果中把属性重复的列去掉。即若R和S中具有相同的属性组B，U为R和S的全体属性集合，则自然连接可记作：

![img](https://img-blog.csdn.net/20180714094742756?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       一般的连接操作是从行的角度进行运算，但自然连接还需取消重复列，所以是同时从行和列的角度进行运算。

​       【例】设图中（a）和（b）分别是关系R和关系S，图中（c）为非等值连接![img](https://img-blog.csdn.net/20180714105007713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的结果，图（d）为等值连接![img](https://img-blog.csdn.net/20180714105230981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的结果，图（e）为自然连接![img](https://img-blog.csdn.net/20180714105305134?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的结果：

![img](https://img-blog.csdn.net/20180714104514510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **（3）左连接（left join）**

​       在自然连接的基础上加上左边表上不包含自然连接中所含元组（行）的元组。

```sql
select * from R left join S
```

​       查询结果：

![img](https://img-blog.csdn.net/20180715104957862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **（4）右连接（right join）**

​       在自然连接的基础上加上右边表上不包含自然连接中所含元组（行）的元组。

```sql
select * from R right join S
```

​       查询结果：

![img](https://img-blog.csdn.net/20180715105022460?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​       **（5）外连接（outer join）**

​       外连接=左连接+右连接；

```sql
select * form R outer join S
```

​       查询结果：

![img](https://img-blog.csdn.net/20180715105038100?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MzI4Mzc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 