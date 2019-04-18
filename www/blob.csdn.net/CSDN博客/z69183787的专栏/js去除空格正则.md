# js去除空格正则 - z69183787的专栏 - CSDN博客
2012年10月25日 14:14:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1019
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script>
function delHtmlTag(str)
{
var str=str.replace(/<\/?[^>]*>/gim,"");//去掉所有的html标记
        var result=str.replace(/(^\s+)|(\s+$)/g,"");//去掉前后空格
        return  result.replace(/\s/g,"");//去除文章中间空格
}
</script>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
方法二
```java
var aa=vv.replace(/\s/g,"")
```
```java
// JS去掉左边空格 
function lTrim(str) 
{ 
    if ((typeof(str) != "string") || !str) 
    { 
        return "";  
    } 
    return str.replace(/(^\s*)/g, "");  
}
```
```java
// JS去掉右边空格 
function rTrim(str) 
{ 
    if ((typeof(str) != "string") || !str) 
    { 
        return ""; 
    } 
    return str.replace(/(\s*$)/g, "");  
}
```
```java
// JS去掉两边空格 
function trimStr(str) 
{  
    if ((typeof(str) != "string") || !str) 
    { 
        return "";  
    } 
    return str.replace(/(^\s*)|(\s*$)/g, "");  
}
```
上面这个方法是去除空格的方法~~ 那么我们经常会遇到类似的需求，你永远不知道用户会输什么，那么你只有尽量去避免他输入你不想要的。上面的改动下也可以变成验证存在
空格给出提示
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//验证内容是否包含空格
 function checkTextSpace(obj,temp){
var reg=/(^\s+)|(\s+$)/g;
var alertValue="输入内容包含空格，请出新输入!";
//temp用来标识内容是否允许存在空格1为可存在0为不存在
     if(temp==1){
         reg=/(^\s{5,})|(\s{5,}$)|(\s{5,})/g;
         alertValue="内容中连续输入空格数超过5个,请重新输入！";
     }
if(reg.test(obj.value)){
        alert(alertValue);
        obj.focus();
return false;
    }
 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上面的这段代码是我遇到一个需求所改，你也可以自己更改,我来简单解释下(高手勿喷):
^匹配字符串的开始
$匹配字符串的结束
/s匹配任意空白字符
/(^\s+)|(\s+$)/g 这个正则就是匹配内容中是否包含空格，不管前后还是中间，都能匹配到
/(^\s{5,})|(\s{5,}$)|(\s{5,})/g 这个正则是我做另一个验证改的,主要是匹配连续输入空格数
\s{5,} 这个代表匹配5次或者更多次
\s* 这个代表重复0次或者更多次
\s+这个代表重复1次或者更多次
\s?这个代表重复0次或者1次
