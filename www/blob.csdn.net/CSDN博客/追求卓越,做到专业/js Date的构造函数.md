
# js Date的构造函数 - 追求卓越,做到专业 - CSDN博客


2012年11月05日 16:54:53[Waldenz](https://me.csdn.net/enter89)阅读数：3755


用js创建日期,其中传入日期字符串:
```python
var lsDate = new Date("2012-11-05 16:08:17");   
   document.write(lsDate.toDateString());
```
在chrome中显示正常为: Mon Nov 05 2012
但是在IE中显示为: NaN
第一次见识到浏览器的之间的差异.
所以只能改变方法(这种方法各浏览器都可以成功):

```python
var b="2012-11-05 16:08:17";
   var arr=b.split(" ")[0].split("-");
   document.write(arr[0]+","+arr[1]+","+arr[2]);
   document.write("<br />");
   var da=new Date(arr[0],arr[1]-1,arr[2]);
   document.write(da.toDateString());
   document.write("<br />");
```
显示结果为: 2012,11,05
Mon Nov 5 2012
注:new Date(year,month,day)中month是从0开始的,即0表示1月,10表示11月.


