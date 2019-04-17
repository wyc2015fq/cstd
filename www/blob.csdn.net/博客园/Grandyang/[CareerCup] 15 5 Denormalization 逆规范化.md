# [CareerCup] 15.5 Denormalization 逆规范化 - Grandyang - 博客园







# [[CareerCup] 15.5 Denormalization 逆规范化](https://www.cnblogs.com/grandyang/p/5362007.html)







15.5 What is denormalization? Explain the pros and cons.



逆规范化Denormalization是一种通过添加冗余数据的数据库优化技术，可以帮助我们减少关系数据库中耗时的交Join。在一般的规范化的数据库中，我们将数据存在不同的表中是为了减少冗余数据，所以我们会尝试着每条数据在数据库中只有一份。

比如说，在一个规范化的数据库中，我们有Courses表和Teachers表，每个Courses表的一项都会保存teacherID，但是没有teacherName，当我们想要返回Course和teacherName时，我们需要联合两个表。当老师需要改名时，我们只需要修改Teachers表，这是这样做的好处，但是，当表很大的时候，我们联合两表就会很耗时。

逆规范化这里就有用武之地了，我们容忍部分冗余数据和更新表所需多余的一些工作，以此换来快速高效的检索和较少的交操作。很多大公司同时使用规范化和拟规范化数据库。



拟规范的优点：

- 检索数据更加快速由于交减少了。

- 检索可能更加简单了，因为不用联合多个表。

拟规范的缺点：

- 更新和插入操作更费事了。

- 更新和插入的脚本更加难写了。

- 数据可能不一致了。

- 由于存在数据冗余，更占空间了。



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












