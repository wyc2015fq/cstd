# javascript实现将数字转成千分位的方法小结【5种方式】 - z69183787的专栏 - CSDN博客
2017年09月11日 10:41:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1130
这篇文章主要介绍了javascript实现将数字转成千分位的方法,结合实例形式分析了5种常用的数字转换的常用方法,涉及字符串与正则操作的相关技巧,需要的朋友可以参考下
本文实例讲述了javascript实现将数字转成千分位的方法。分享给大家供大家参考，具体如下：
![](http://files.jb51.net/file_images/article/201612/20161211121347007.jpg?20161111121424)
尽管离过年还有两个月之久，春运抢票的战斗已经打响了，悲剧的是我还没抢到票，看到某浏览器上的数字时，想到一个经典面试题，没错，就是数字转千分位。如将数字87463297转成87，463，297，方法有很多种，我这里只想到5种。
1、利用正则的零宽度正预测先行断言(?=exp)，名字有点难记，意思是它断言自身出现的位置的后面能匹配表达式exp，对此概念还不明白的可以戳这里，这里不做过多解释。数字千分位的特点是，第一个逗号后面数字的个数是3的倍数，正则：/(\d{3})+$/；第一个逗号前最多可以有1至3个数字，正则：/\d{1,3}/。加起来就是/\d{1,3}(\d{3})+$/，分隔符要从前往后加，就要将前面的数字“87”替换成“87，”，为什么是87不是874？因为874后面只剩下5位数字，在632后加一个分隔符后，将只剩下97，不符合千分位要求，所以第一个分隔符后面的数字位数必须是3的倍数。要匹配数字87，又要保证87后面数字位数是3的倍数，并且要将匹配的87替换成“87，”，就要用到(?=exp)，这里先定义一个变量var
 str = "87463297";
// 零宽断言
console.info( str.replace(/\d{1,3}(?=(\d{3})+$)/g,function(s){
  return s+','
}) )
2、利用正则的子项来替换，跟第1种方法类似。
// 子项
console.info( str.replace(/(\d{1,3})(?=(\d{3})+$)/g,function($1){
  return $1=$1+','
}) )
3、先将字符串转成数组，利用reverse反转数组后每3个数字后添加一个分隔符“,”，到字符串末尾除外，之后转回字符串。
// 利用字符串和数组方法
console.info( str.split("").reverse().join("").replace(/(\d{3})+?/g,function(s){
  return s+",";
}).replace(/,$/,"").split("").reverse().join("") )
4、利用while循环拼接字符串每隔3个数字加一个分隔符，首尾不加
// 利用循环拼接字符串每隔3个加一个分隔符
var result="",
  index = 0,
  len = str.length-1;
while(len>=0) {
  index%3===0&&index!==0 ? result+=","+str[len] : result+=str[len];
  len--;
  index++;
};
result=result.split("").reverse().join("");
console.info(result);
5、利用while循环在数组里push分隔符，首尾不加
// 利用while循环在数组里push分隔符
var result="",
  index = 0,
  len = str.length,
  i = len-1,
  arr = str.split("");
while(len-index>0){
  len>=index&&len-index!==len && arr.splice(len-index,0,",");
  index+=3;
  i-=4;
};
console.log(arr.join(""));
结语：第1种方法是最简洁的，性能也是最好的，推荐使用。顺便附上所有例子的[demo源码下载](http://xiazai.jb51.net/201612/yuanma/number_format_5_method_demo(jb51.net).rar)，如果还有其它更好更方便的方法请告诉我，谢谢！
