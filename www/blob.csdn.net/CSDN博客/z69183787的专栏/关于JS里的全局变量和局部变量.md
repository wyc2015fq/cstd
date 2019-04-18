# 关于JS里的全局变量和局部变量 - z69183787的专栏 - CSDN博客
2013年12月28日 12:44:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1041
代码：
<script>
var scope = "全局变量";
function test()
{
alert(scope); 
var scope = "局部变量";
alert(scope); 
}
test();
alert(scope);
</script>
运行结果：
1、输出的是： undefined    （因为‘scope’在test函数中已经被重新定义了，局部变量在test函数中将全局有效，即在scope还没定义时，全局变量scope依然被覆盖。）
2、输出的是： 局部变量
3、输出的是： 全局变量
