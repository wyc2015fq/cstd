# Node.js中 Date 对象使用样例 - cocoonyang的专栏 - CSDN博客





2017年04月05日 08:55:38[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：3785








## Date对象

     Date 对象用于处理日期和时间。





## 创建 Date 对象

1)  默认初始化方法. 例如: 


```java
var theDate = new Date();
```
Date 对象会自动把当前日期和时间保存为其初始值。




2) 使用标准的日期格式字符串初始化. 例如: 


```java
var myDate = new Date("2012-02-15 21:00:00");
console.log('Date: ',   myDate );
```
Results:
Date: Fri Feb 15 2012 21:00:00 GMT+0800 (中国标准时间)




3) 用从1970年1月1日00:00:00 UTC开始计算的毫秒数作为参数初始化. 例如:  

```java
var myDate = new Date(1491313260313);
console.log('Date: ',   myDate );
```

Results:
`Date: Tue Apr 04 2017 21:41:00 GMT+0800 (中国标准时间)`

## 提取、设置Date对象各分量

提取Date对象个分量

```java
var myDate = new Date("2012-02-15 21:00:00");  
console.log( myDate );  

// getFullYear()
// 从 Date 对象返回年。
console.log( "        Year:", myDate.getFullYear() ); 

// getMonth() 
// 从 Date 对象返回月份 (0 ~ 11)。
console.log( "       Month:", myDate.getMonth() ); 

// getDate()
// 从 Date 对象返回一个月中的某一天 (1 ~ 31)。
console.log( "        Date:", myDate.getDate() ); 

// getDay()
// 从 Date 对象返回一周中的某一天 (0 ~ 6)。
console.log( "         Day:", myDate.getDay() ); 

// getHours()
// 返回 Date 对象的小时 (0 ~ 23)。
console.log( "        Hour:", myDate.getHours() ); 

// getMinutes()
// 返回 Date 对象的分钟 (0 ~ 59)。
console.log( "      Minute:", myDate.getMinutes() ); 

// getSeconds()
// 返回 Date 对象的秒数 (0 ~ 59)。
console.log( "      Second:", myDate.getSeconds() ); 

// getMilliseconds()
// 返回 Date 对象的毫秒(0 ~ 999)。
console.log( " Millisecond:", myDate.getMilliseconds() );
```

  Results: 

![](https://img-blog.csdn.net/20180226122309486)





设置Date对象个分量


```java
var myDate = new Date();

// setFullYear()
// 设置 Date 对象的年。
myDate.setFullYear(2017)

// getMonth() 
// 设置 Date 对象返回月份 (0 ~ 11)。
myDate.setMonth(3)

// setDate()
// 设置 Date 对象返回一个月中的某一天 (1 ~ 31)。
myDate.setDate(10) 

// setHours()
// 设置 Date 对象的小时 (0 ~ 23)。
myDate.setHours(20) 

// setMinutes()
// 设置 Date 对象的分钟 (0 ~ 59)。
myDate.setMinutes(10)

// setSeconds()
// 设置 Date 对象的秒数 (0 ~ 59)。
myDate.setSeconds(12)

// setMilliseconds()
// 设置 Date 对象的毫秒(0 ~ 999)。
myDate.setMilliseconds(123)

console.log( myDate );
```
Results:
![](https://img-blog.csdn.net/20180226124107155)


Note: 没有 setDay()方法







Reference:

[1] http://www.w3school.com.cn/jsref/jsref_obj_date.asp









