# Perl语言--一些关于赋值、引用的东西 - 默槑 - 博客园







# [Perl语言--一些关于赋值、引用的东西](https://www.cnblogs.com/modaidai/p/6759319.html)





　　　　　　　　　　　　　　　引用详解

一、定义引用有两种：

　　　　　　　　1、用斜线“\”

　　　　　　　　2、匿名引用

1、用反斜线的引用：数组、哈希、标量

　　数组的引用

　　　　my @array = (1,2,3);

　　　　my $aref = \@array;

　　哈希的引用：

　　　　my %hash = ("wo"=>"women","ni"=>"nimen");

　　　　my $href = \%hash;

　　标量的引用：

　　　　my $scalar = 1;

　　　　my $scalarrrr= \$scalar;

2、使用匿名引用：

匿名**数组**引用：要用中括号:"[]"

　　　　　　　　$aref = [1,"we","ig",undef,809];

匿名哈希引用：要用大括号："{}"

　　　　　　　　$href = {we=>1,ig=>2};

二、使用引用：

1、第一种方法：

定义了就该使用了，被引用的是什么类型的，显示的时候就需要加上什么样的符号，如数据要在前面加上@
my $scalar = 1 ;
my @array = (1, 2, 3) ;
my %hash = ('zdd' => 30, 'autumn' => 27) ;

my $sref = \$scalar ;   # scalar reference
my $aref = \@array ;    # array reference
my $href = \%hash ;     # hash reference

# 方法一
print $$sref, "\n" ;　　# 用$sref代替sref
print @$aref, "\n" ;   # 用$aref代替aref 
print %$href, "\n" ;   # 用$href代替href


访问具体的值就都换成$$,注意是$$$$$$,重要的事情说好几遍
print $$aref[2], "\n" ;
print $$href{'zdd'}, "\n" ;


2、第二种方法

与普通变量的访问方法相比，假设变量原来的名字是name，则现在用{$name}来代替name。
@a 　　　　等同于　　　　@{$aref} 　　　　#用其实很简单，就是很容易混淆　　　　　　　　reverse@a　　等同于　　reverse @{$aref} 　　　　　#需要反复记忆，才能活学活用$a[3] 　　　等同于　　　　${$aref}[3] 　　　　　　　#记不住的话可能看代码都会蒙圈

$a[3] =17;   等同于　　　　${$aref}[3] =17 　　　　#我现在也没记住呢


同理，哈希引用的使用方法如下。
%h 　　　　　等同于　　　　%{$href} 　　　　　　　　keys%h　　　　等同于　　　　　　keys%{$href} 　

$h{'red'}  　　等同于　　　　　　${$href}{'red'} 　　

$h{'red'} =17 　　等同于　　　 ${$href}{'red'} =17 　 


3、第三种方法：不适用于标量
前两种很繁琐，第三种会很简洁，使用箭头符号："->"，一定注意这个箭头后面的括号是啥，数组要用中，哈希用大
$aref->[]  　　　　#数组解引用
$href->{}  　　　　#哈希解引用
$href->()　　　　　#子过程解引用

举个栗子：$aref->[0] = 3;
$href->{name} = "autumn";　　　

三、实战分析
看一下这些玩意到底是什么玩意

先看正常的数组是怎么定义和访问的：

@a = ([1, 2, 3],[4, 5, 6],[7, 8, 9])    
  #定义一个二维数组
　#我们知道[1, 2, 3]定义了一个（1， 2， 3）的匿名引用，所以数组a实际上
　#包含三个元素，每个元素是一个引用，该引用指向一个数组，所以我们可以
　#用下面的方法来访问数组元素（注意，下标从0开始）

多维数组的另一个写法如下：
my $aref = [1, [2, 3], [4, 5, 6]] ;
print $aref->[0] , "\n" ; 　　　　　　　#输出1
print $aref->[1][1], "\n" ; 　　　　　　#输出3
print $aref->[2][0], "\n" ;　　　　　　 #输出4



这两者的区别有以下几点：
- 前者是真正的数组，所以定义变量是使用@，后者是指向匿名数组的引用，所以定义的时候使用$
- 前者的数组元素是匿名数组，而外层数组则是实体数组，后者无论元素还是外层数组都是匿名数组
- 前者可以用$a[x][y]的形式访问，而后者只能用解引用的方式访问，即$a->[x][y]的形式。



四、说一说引用的赋值

$aref1 = $aref2     #使得这两个东西，也就是$aref1和$aref2，指向同一个数组

蛋是，你现在想把$aref1指向的那个数组拷贝一份给$aref2的话，使用这种方法：

　　　　　　　$aref2 = [@{$aref1}];　　#在中括号里面进行解引用，而中括号以解引用后的数组为内容

　　　　　　　　　　　　　　　　　　　　　#生成了一个新的匿名数组，赋值给$aref2

注意：不能不加中括号，由于等号左边是标量，等号右边的数组会被解释为标量环境，

　　　所以最后$aref2得到的是数组元素个数，而不是元素本身

　　　但加上中括号就可以了，perl会知道这个是一个匿名数组的赋值
　　$aref2 = @{$aref1}；     #得到的会是元素个数，而不是数组本身

五、判断一个变量是否是引用
使用ref函数就可以，如果变量是引用就返回TRUE，否则为假。实际上他还会返回引用对象的类型，比如HASH或者ARRAY
　　　　my $aref1 = [1,2,0];
　　　　print ref #aref1;　　　　　　#返回ARRAY
还可以这样用：
　　　　if (ref $aref1) {
　　　　　　print "true"."\n";　　　　#返回true
}


六、判断两个引用是否指向一同个目标
可以用eq，这将以字符串的形式判断，也可以使用==

看下面就知道是不是男人，额，不是不是，就知道是怎么用的了

看下面：
my $aref1 = [1, 2, 0] ;　　　　　　　　　　　　
my $aref2 = $aref1 ;　　　　　　　　　　　　　　　　　　
print $aref1, "\n" ;　　　　　　　　　　　　　　　　
print $aref2, "\n" ;　　　　　　　　　　　　　　　　　　

if ($aref1 eq $aref2) {
    print "reference equal\n" ;
}
if($aref1 == $aref2) {
    print "reference equal\n" ;
}


产生如下输出

ARRAY(0x248bec)
ARRAY(0x248bec)
reference equal (eq)
reference equal (==)

















