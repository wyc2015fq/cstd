# JS的toFixed方法出错 重写 - z69183787的专栏 - CSDN博客
2014年03月29日 08:01:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5031
toFixed() 方法可把 Number 四舍五入为指定小数位数的数字。通俗点就是传入的参数是几，那么我们的小数就保留几位。
eg: alert(0.056.toFixed(2));-->0.06
但是使用过程中发现了一个小问题 ，就是0.0056.toFixed(2)结果却为0.00
遂重写toFixed()
代码如下：
Js代码  ![收藏代码](http://wjlvivid.iteye.com/images/icon_star.png)
- //重写toFixed方法
-     Number.prototype.toFixed=function(len)  
-     {  
- var tempNum = 0;  
- var s,temp;  
- var s1 = this + "";  
- var start = s1.indexOf(".");  
- 
- //截取小数点后,0之后的数字，判断是否大于5，如果大于5这入为1
- 
- if(s1.substr(start+len+1,1)>=5)  
-         tempNum=1;  
- 
- //计算10的len次方,把原数字扩大它要保留的小数位数的倍数
- var temp = Math.pow(10,len);  
- //求最接近this * temp的最小数字
- //floor() 方法执行的是向下取整计算，它返回的是小于或等于函数参数，并且与之最接近的整数
-         s = Math.floor(this * temp) + tempNum;  
- return s/temp;  
- 
-     }  
现在再试试alert(0.0056.toFixed(2));结果就为0.01啦！
