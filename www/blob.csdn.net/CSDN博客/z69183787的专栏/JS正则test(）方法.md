# JS正则test(）方法 - z69183787的专栏 - CSDN博客
2014年01月05日 17:11:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：54206
每个正则表达式都有一个 lastIndex 属性，用于记录上一次匹配结束的位置，包括exec方法。
var re = /^[1-9]\d{4,10}$/gi;
var str = "123456";
alert(re.test(str));     //返回true
str = "1234567";
alert(re.test(str));     //返回false
解决方法：
var re = /^[1-9]\d{4,10}$/gi;
var str = "123456";
alert(re.test(str));      //返回true
// 执行上面的 test 之后
我们可以弹出
alert(re.lastIndex); // 弹出6
即表示上一次在第6个字符之后结束
那么下一次再调用 test 的时候， 就会从第6个字符之后继续搜索
解决办法，将正则表达式的 lastIndex 属性设置为0 
具体代码如下：
<script type="text/javascript">
var re = /^[1-9]\d{4,10}$/gi;
var str = "123456";
alert(re.test(str));      //返回true
str = "1234567";
re.lastIndex=0;
alert(re.test(str));      //返回true
</script>
或：
 正则模式去掉全局模式。
不要这么麻烦的，把g去掉就行了，g就是继续往下的意思，不是必要别加g
改成var re = /^[1-9]\d{4,10}$/i;var str = "123456";
alert(re.test(str));      //返回true
str = "1234567";
alert(re.test(str));      //返回true
