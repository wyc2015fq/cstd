# perl中my与local的区别 - Orisun - 博客园







# [perl中my与local的区别](https://www.cnblogs.com/zhangchaoyang/articles/2767445.html)





转载：[http://blog.sina.com.cn/s/blog_4fcdb9280100l46v.html](http://blog.sina.com.cn/s/blog_4fcdb9280100l46v.html)

my: lexically scoped variable
local:Dynamically scoped variable



内部->外部：

（1）my和local都只在一个block里有效，出去就失效；
（2）但是local的变量可以继续在这个block中调用的子程序中存在；
（3）如果有与外界同名的变量，两者在block退出后都不影响外界同名变量；

外部->内部：

（1）外部设置my、local、缺省均队内有效，但是同名变量外部my，在block内部local是不允许的。因为二者在block中调用的子程序中均有效，会冲突。

（2）如果在一个block中有一个**my修饰的变量**和外界的一个变量同名，而且又需要在这个block中使用外界变量时，两个办法：
第一个办法，用main的package修饰这个变量名，$main::global
第二个办法，用our修饰，our $global，那么该block中接下来出现的所有$global都是外界的global

（3）编写脚本时，注意作用域，防止外部影响内部。




#### perl学习笔记1---local与my用法


      在函数定义中，使用my关键字来确保用户定义函数中所使用的变量名不与任何其它同名的全局变量冲突，它使其后面的变量成为通常语言所谓的local变量。缺省情况下，perl函数中的变量是全局变量。 local变量先save再store

$x = 9;
sub mm{
    print "$x\n";
}
sub gg_my{
    my $x = 11;
    print "call-my $x\n";
    mm;
}
sub gg_local {
    local $x = 20;
    print "call-local $x\n";
    mm;
}
gg_my;
gg_local;

print $x;

输出结果是：

call_my 11
9
call_local 20
20

9

可以看出，local函数在创建全局变量的一个局部实例后即可将它传递给任何子例程并继续进行局部初始赋值。运算符my以与local函数同样的方法创建一次局部实例，然而，由my创建的局部实例不能传递给其它子例程，传递给子例程进行运算的将是全局值而非局部引用值。

当locla所在的子例程结束后，全局变量的值又恢复为初始定义值。













