# SQL Server 2016 函数：CHARINDEX - weixin_33985507的博客 - CSDN博客
2018年06月03日 14:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
- CHARINDEX 返回字符或者字符串在另一个字符串中的起始位置
```
CHARINDEX(expression1,expression2[,start_location]);
```
> - expression1 是要到 expression2 中取寻找的字符串，start_location 是可选参数，是指要在 expression2 的第几个位置开始查找
- CHARINDEX 函数返回的是一个整数，这个整数是 expression1 在 expression2 中的位置，如果 expression2 中没有找到 expression1，则返回 0
- PATINDEX 返回字符或者字符串在另一个字符串中的起始位置
```
PATINDEX(expression1,expression2[,start_location]);
```
> - PATINDEX 函数支持在搜索字符串中使用通配符，所以它的价值体现在要对变化的字符串进行搜索时（类比模糊查询）
```
SELECT CHARINDEX('cd','abcdef');  -- 返回的位置是从 1 开始计数的，所以返回结果位置为 3
SELECT PATINDEX('cd','abcdef');  -- PATINDEX 直接匹配是寻找不到结果的，所以通常情况下都是与 通配符 一起使用
SELECT PATINDEX('abcdef','abcdef');  -- PATINDEX 在不与通配一起使用的情况下要想寻找到结果，查询字符串和被查询字符串必须相同
SELECT PATINDEX('%cd%','abcdef');
SELECT PATINDEX('%ef','abcdef');  -- 所以多数情况下 PATINDEX 更多的是判断字符串是否以某个字符串 开头或者结尾
SELECT PATINDEX('ab%','abcdef');
```
![9538421-32fc322ceafafc97.png](https://upload-images.jianshu.io/upload_images/9538421-32fc322ceafafc97.png)
image.png
```
SELECT *,CHARINDEX('计算机',Description) AS Desc_Location FROM Subject;
SELECT *,CHARINDEX('计算机',Description,10) AS Desc_Location FROM Subject;  -- 指定起始位置从第 10 个字符开始查找
SELECT *,PATINDEX('%是一门%',Description) AS Desc_Location FROM Subject;
SELECT *,PATINDEX('%是一门%',Description) AS Desc_Location FROM Subject;
```
![9538421-65c901796355fbf6.png](https://upload-images.jianshu.io/upload_images/9538421-65c901796355fbf6.png)
image.png
