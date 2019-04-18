# jQuery新增函数之jQuery.isNumeric() - z69183787的专栏 - CSDN博客
2013年01月16日 15:14:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3761
语法：
- jQuery.isNumeric( value )
说明：1.7版本中添加，判断它的参数是否是一个数字。
返回：true/false
$.isNumeric函数用于检查其参数是否是一个数值。如果是，则返回true。否则返回false。这个参数可以是任何类型的。
功能和$.isNaN()恰好相反，$.isNaN()在1.7版本中已经弃用。
示例：
- $.isNumeric("-10");// true
- $.isNumeric(16);// true
- $.isNumeric(0xFF);// true
- $.isNumeric("0xFF");// true
- $.isNumeric("8e5");// true (exponential notation string)
- $.isNumeric(3.1415);// true
- $.isNumeric(+10);// true
- $.isNumeric(0144);// true (octal integer literal)
- $.isNumeric("");// false
- $.isNumeric({});// false (empty object)
- $.isNumeric(NaN);// false
- $.isNumeric(null);// false
- $.isNumeric(true);// false
- $.isNumeric(Infinity);// false
- $.isNumeric(undefined);// false
