# JS的replace 使用 第二个参数为函数 - z69183787的专栏 - CSDN博客
2017年01月03日 15:55:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2334
replace() 方法的参数 replacement 可以是函数而不是字符串。在这种情况下，每个匹配都调用该函数，它返回的字符串将作为替换文本使用
replace() 方法的参数 replacement 可以是函数而不是字符串。在这种情况下，每个匹配都调用该函数，它返回的字符串将作为替换文本使用。该函数的第一个参数是匹配模式的字符串。接下来的参数 是与模式中的子表达式匹配的字符串，可以有 0 个或多个这样的参数。接下来的参数是一个整数，声明了匹配在 stringObject 中出现的位置。最后一个参数是 stringObject 本身。 
下文展示了几种javascript正则表示式的repalce方式，有些方式我们很少在别的地方看到，如第二种和第三方中方法。 
//下面的例子用来获取url的两个参数，并返回urlRewrite之前的真实Url 
复制代码代码如下:
var reg=new RegExp("(http://www.qidian.com/BookReader/)(\\d+),(\\d+).aspx","gmi"); 
var url="http://www.qidian.com/BookReader/1017141,20361055.aspx"; 
//方式一,最简单常用的方式 
var rep=url.replace(reg,"$1ShowBook.aspx?bookId=$2&chapterId=$3"); 
alert(rep); 
//方式二 ,采用固定参数的回调函数 
var rep2=url.replace(reg,function(m,p1,p2,p3){return p1+"ShowBook.aspx?bookId="+p3+"&chapterId="+p3}); 
alert(rep2); 
//方式三，采用非固定参数的回调函数 
var rep3=url.replace(reg,function(){var args=arguments; return args[1]+"ShowBook.aspx?bookId="+args[2]+"&chapterId="+args[3];}); 
alert(rep3); 
方法2：
m:http://www.qidian.com/BookReader/1017141,20361055.aspx
p1,p2,p3:
http://www.qidian.com/BookReader/
1017141
20361055
//方法四 
//方式四和方法三很类似, 除了返回替换后的字符串外，还可以单独获取参数 
复制代码代码如下:
var bookId; 
var chapterId; 
function capText() 
{ 
var args=arguments; 
bookId=args[2]; 
chapterId=args[3]; 
return args[1]+"ShowBook.aspx?bookId="+args[2]+"&chapterId="+args[3]; 
} 
var rep4=url.replace(reg,capText); 
alert(rep4); 
alert(bookId); 
alert(chapterId); 
//除了使用replace方法获取正则表示式的分组外，还可以使用test ,exec方法获取分组，只是手法有所不同而已 
复制代码代码如下:
var reg2=new RegExp("(http://www.qidian.com/BookReader/)(\\d+),(\\d+).aspx","gmi"); 
var m=reg2.exec("http://www.qidian.com/BookReader/1017141,20361055.aspx"); 
var s=""; 
//获取所有的分组 
复制代码代码如下:
for (i = 0; i < m.length; i++) { 
s = s + m[i] + "\n"; 
} 
alert(s); 
bookId=m[2]; 
chapterId=m[3]; 
alert(bookId); 
alert(chapterId); 
//使用test方法获取分组 
复制代码代码如下:
var reg3=new RegExp("(http://www.qidian.com/BookReader/)(\\d+),(\\d+).aspx","gmi"); 
reg3.test("http://www.qidian.com/BookReader/1017141,20361055.aspx"); 
//获取三个分组 
复制代码代码如下:
alert(RegExp.$1); 
alert(RegExp.$2); 
alert(RegExp.$3); 
var str="www.baidu.com"; 
//str.format("好","q") 
str.replace(new RegExp("(\\.)(bai)du","g"),function(){ 
for(var i=0;i<arguments.length;i++) 
{ 
document.write(arguments[i]+"<br/>"); 
} 
document.write("-------------------------------------------------<br/>"); 
}); 
两个例子(证明，replace传入正则参数和字符传参数结果不同)： 
alert("123".replace("1",function(){var un;return un;})); //弹出undefined23 
alert("123".replace(new RegExp("1"),function(){var un;return un;})); //弹出23 
