# SQL语句之order by 、group by、having、where - weixin_33985507的博客 - CSDN博客
2016年03月29日 22:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**百度知道：**
1.order by是 按字段进行排序.. 字段后面可跟desc降序..asc升序..默认为升序
2.group by是进行分组查询
3.having和where都属于条件过滤 
区别在于一般having是和group by连用... group by...having... 表示**先分组再条件过滤**
而如果在**group by前面有where**，则是表示**先条件过滤再分组 **
这个在实际中特殊的查询会影响到查询结果。
[PS](https://www.baidu.com/s?wd=PS&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dWnWcdmhm3nADduADsujPB0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPHmYPWcvPjmz): 这几条关键字是有先后顺序的，where...group by...having...order by  都是可选，但是如果全部写出来，必须是这个顺序。
================================言归正传！
## 一、Where
select * from tableName where id="2012";
就是查找tableName中，id=2012的记录。
这里的where对查询的结果进行了筛选。只有满足where子句中条件的记录才会被查询出来。
## 二、Group By
![](https://img-my.csdn.net/uploads/201208/07/1344347652_5110.jpg)
根据上表，需求：求出Apple在China,Japan,USA的平均价格，你怎么办？
可以这么做：select avg(price) from tablename where fruitname="apple";
Group By 一般是和一些聚合函数一起使用，比如上面我们用到的求平均的函数avg，还有求和sum，求个数count，求最大max，求最小min。
对于上表，求每种水果的最大的价格：select fruitname,productplace,max(price) from tablename group by fruitname
Group By 还有一个重要的合作对象，就是having。
## 三、Having
用Group By 进行分组后，如何对分组后的结果进行筛选呢？having可解决这个难题。
1.首先看一个例子：求平均价格在3.0以上的水果
如果使用：
select fruitname,avg(price) from tablename where avg(price)>=3.0 group by fruitname ;
这样能否达到我们的要求呢？
答案是否定的，因为where子句不能使用聚合函数。为了解决这个问题，我们来用下我们的杀手锏，他就是Having；
改写如下：select fruitname,avg(price) from tablename group by fruitname having avg(price)>=3.0;
2.我们继续看Having的另外一个匪夷所思
select fruitname,avg(price) from tablename group by fruitname having price<2.0;
这个查询的结果你们觉得会是什么呢？
没错，就是 orange 0.8 ；只有这一条记录
为什么呢？为什么Apple没有被查到呢，因为apple的有的价格超出了2.0.
另外运算符in也可以用在having 子句。
select fruitname,avg(price) from tablename group by fruitname having fruitname in （"orange","apple"）;
## 四、Order By
Order By是对查询的结果进行一个再排序的过程，一般放在查询语句的最后，可以是单列，也可以实现多列的排序。
分为升序asc和降序desc，默认的为升序。
Order By单列的排序比较简单，多列的也不麻烦。
select * from tablename group by friutname order by fruitname asc,price desc.
