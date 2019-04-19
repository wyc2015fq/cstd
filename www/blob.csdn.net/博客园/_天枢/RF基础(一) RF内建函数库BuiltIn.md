# RF基础(一) RF内建函数库BuiltIn - _天枢 - 博客园
## [RF基础(一) RF内建函数库BuiltIn](https://www.cnblogs.com/yhleng/p/8074630.html)
2017-12-20 16:48 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8074630)
**Robot framework做为一个测试框架,并不是只能做selenium测试,是支持扩展的, 比如说,你引用requests库就可以做接口测试, 那么无论你用什么库**
**首先要了解, RF本身提供的内建函库BuiltIn中的内建函数 ,这是RF本身带的.下面开始了解一下RF库中都有哪些内建函数,**
![](https://images2017.cnblogs.com/blog/1149221/201712/1149221-20171220163642100-2132259995.png)
1>Convert **系列**函数****
![](https://images2017.cnblogs.com/blog/1149221/201712/1149221-20171220165533412-781887641.png)
2>R**un keyword **系列**函数******
**根据条件决定是否执行关键字**
Run Keyword If
Run Keyword If All Critical Tests Passed
Run Keyword If All Tests Passed
Run Keyword If Any Critical Tests Failed
Run Keyword If Any Tests Failed
Run Keyword If Test Failed
Run Keyword If Test Passed
Run、Keyword If Timeout Occurred
3>**Should 系列**函数**：**
**这些关键字都是用作判断时用的**
Should Be Empty
Should Be Equal
Should Be Equal As Integers 
Should Be Equal As Numbers
Should Be Equal As Strings
Should Be True
Should Contain
Should Contain X Times
Should End With
Should Match
Should Match Regexp 
Should Not Be Empty
Should Not Be Equal
Should Not Be Equal As Integers
Should Not Be Equal As Numbers
Should Not Be Equal As Strings
Should Not Be True
Should Not Contain
Should Not End With
Should Not Match 
Should Not Match Regexp
Should Not Start With
Should Start With
4>**Evaluate 关键字：**
数值运算并得到结果
Evaluate把想要计算的表达式直接传递给Python，Python的计算结果返回
