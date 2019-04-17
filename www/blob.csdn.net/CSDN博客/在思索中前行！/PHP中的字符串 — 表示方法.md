# PHP中的字符串 — 表示方法 - 在思索中前行！ - CSDN博客





2014年05月09日 22:14:34[_Tham](https://me.csdn.net/txl16211)阅读数：1793








```php

```
Strings



一个 [string](http://docs.php.net/manual/en/language.types.string.php)是一连串的字符, 其中的字符与一个byte相同。也就是说PHP只支持一个256的字符集，因此本质上不支持Unicode编码，关于Unicode阅读 [utf8_encode()](http://docs.php.net/manual/en/function.utf8-encode.php)和 [utf8_decode()](http://docs.php.net/manual/en/function.utf8-decode.php)。

**注意: 一个字符串的大小决定与计算机内存的大小，理想情况下，字符串可以无限大。**


**语法**


一个 [string](http://docs.php.net/manual/en/language.types.string.php) 可以使用下面四种方法表示：
- [single
 quoted](http://docs.php.net/manual/en/language.types.string.php#language.types.string.syntax.single)
- [double
 quoted](http://docs.php.net/manual/en/language.types.string.php#language.types.string.syntax.double)
- [heredoc
 syntax](http://docs.php.net/manual/en/language.types.string.php#language.types.string.syntax.heredoc)
- [nowdoc
 syntax](http://docs.php.net/manual/en/language.types.string.php#language.types.string.syntax.nowdoc) (从 PHP 5.3.0开始使用)
**Single quoted （单引号表示法）**


定义一个 [string](http://docs.php.net/manual/en/language.types.string.php)最简单的方法是将字符串放到一对单引号当中（字符‘）。

字符串本身当中的单引号可以使用反斜线(\)转义，
 单个反斜线可以使用两个反斜线输出(\\)。除此之外，其它左右的反斜线将会被当作正常字符处理：也就是说，以前经常用的转义字符会原样输出。如\r\n等，不会再有特殊意义。 

注意: 与[double-quoted](http://docs.php.net/manual/en/language.types.string.php#language.types.string.syntax.double) 和 [heredoc](http://docs.php.net/manual/en/language.types.string.php#language.types.string.syntax.heredoc) 不同，特殊的变量和转义序列在单引号表示法中 不会 有特殊的意义。



```php
<?php
echo 'this is a simple string';

echo 'You can also have embedded newlines in 
strings this way as it is
okay to do';

// Outputs: Arnold once said: "I'll be back"
echo 'Arnold once said: "I\'ll be back"';

// Outputs: You deleted C:\*.*?
echo 'You deleted C:\\*.*?';

// Outputs: You deleted C:\*.*?
echo 'You deleted C:\*.*?';

// Outputs: This will not expand: \n a newline
echo 'This will not expand: \n a newline';

// Outputs: Variables do not $expand $either
echo 'Variables do not $expand $either';
?>
```

Double quoted （双引号表示法）





如果字符串使用双引号表示，PHP则会将特殊转义序列按照他们对应的意义进行翻译：


|序列|意义|
|----|----|
|\n|linefeed (LF or 0x0A (10) in ASCII)|
|\r|carriage return (CR or 0x0D (13) in ASCII)|
|\t|horizontal tab (HT or 0x09 (9) in ASCII)|
|\v|vertical tab (VT or 0x0B (11) in ASCII) (since PHP 5.2.5)|
|\f|form feed (FF or 0x0C (12) in ASCII) (since PHP 5.2.5)|
|\\|backslash|
|\$|dollar sign|
|\"|double-quote|
|\[0-7]{1,3}|the sequence of characters matching the regular expression is a character in octal notation|
|\x[0-9A-Fa-f]{1,2}|the sequence of characters matching the regular expression is a character in hexadecimal notation|



除了上面这些，转义其它任何字符将会像单引号表示法一样——反斜线也会被打印。 PHP
 5.1.1之前,
 t\{$var} 当中的反斜线不会被打印。

双引号最重要的功能是，双引号当中的变量名字会被相应的值替换。详细介绍阅读[string
 parsing](http://docs.php.net/manual/en/language.types.string.php#language.types.string.parsing)。
Heredoc


第三种界定字符串的技术是使用 heredoc ：在<<<操作符之后，添写一个标识符，然后新起一行
 是字符串本身的内容，字符串本身结束后再新起一行以相同的标识符结束。

必须
 顶格写（）。同样，标识符的命名必须符合PHP当中其它标识符命名的规则。必须只包含字母、数字和下划线，而且不能以数字开头。

结束标识符 必须 顶格写（从行的第一列开始）。同样，标识符的命名必须符合PHP当中其它标识符命名规则。必须只包含字母、数字和下划线，而且不能以数字开头。
警告

一定要注意，除了 可能 包含一个分号外，结束标识符前不能还有其它字符。也就是说此标识符不能被缩进，而且分号前不含有空格或tab格。在结束标识符前面第一个字符应该是系统定义的换行符，在Unix和Mac OS X 中是\n。结束标识符后必须也是换行符。


如果不符合此规则，结束标识符不会被识别为字符串结束的标志，PHP会继续向下搜寻，如果一直到文件的结束还没有找到一个合适的结束标识符，那么PHP便会在最后一行产生一个语法错误。


Heredocs 不能初始化类的属性。但是从PHP 5.3 开始，只有当heredocs中含有变量的时候这种初始化方法是不正确的。


Example #1 错误的例子


<?php
class foo {

    public $bar = <<<EOT
bar

    EOT;

}

?>





Heredoc
 对字符串的处理与双引号表示法类似，但是对双引号的处理是一个例外。也就是说，双引号在heredoc当中不需要被转义，但是在上述转义字符表中的其它转义字符可以使用。变量会被展开，但在heredoc中表示一个复杂的变量的时候需要小心。



Example #2 Heredoc string quoting example


<?php

$str = <<<EOD
Example of string

spanning multiple lines

using heredoc syntax.
EOD;

/* More complex example, with variables. */
class foo
{

    var $foo;

    var $bar;


    function foo()

    {
$this->foo = 'Foo';
$this->bar = array('Bar1', 'Bar2', 'Bar3');

    }

}

$foo = new foo();
$name = 'MyName';


echo <<<EOT
My name is "$name". I am printing some $foo->foo.

Now, I am printing some {$foo->bar[1]}.

This should print a capital 'A': \x41
EOT;
?>




上边例子的输出:



My name is "MyName". I am printing some Foo. Now, I am printing some Bar2. This should print a capital 'A': A



 也可以使用Heredoc给函数传递参数：


Example #3 Heredoc in arguments example


<?php

var_dump(array(<<<EOD
foobar!
EOD

));
?>










＃As of PHP 5.3.0, it's possible to initialize static variables and class properties/constants using the Heredoc syntax:＃


自PHP 5.3.0开始，可以使用Heredoc初始化静态变量和类的属性／常量。


Example #4 Using Heredoc to initialize static values


<?php
// Static variables
function foo()

{

    static $bar = <<<LABEL
Nothing in here...
LABEL;

}

// Class properties/constants
class foo
{

    const BAR = <<<FOOBAR
Constant example
FOOBAR;


    public $baz = <<<FOOBAR
Property example
FOOBAR;

}
?>




＃Starting with PHP 5.3.0, the opening Heredoc identifier may optionally be enclosed in double quotes:＃


从PHP 5.3.0 开始，起始标识符可以包含在双引号当中：


Example #5 Using double quotes in Heredoc


<?php
echo <<<"FOOBAR"
Hello World!
FOOBAR;
?>



> 
注意:


Heredoc 从 PHP 4 开始被支持。












Nowdoc
像heredoc与双引号的对应相似，nowdoc与单引号对应。nowdoc的定义与heredoc相似，但是在nowdoc中 没有解析操作 。 这种结构对于嵌入PHP代码或者其它不需要被转义的代码来说是理想的。它与SGML的<![CDATA[]>同样，具有声明一个不需要被解析的代码块的功能。

nowdoc也以<<<作为标识，但是其后声明的标识符需要使用单引号。例如：<<<'EOT"。对heredoc的所有规则对nowdoc来说都有效，特别是那些对结束标识符的规则。


Example #6 Nowdoc string quoting example


<?php

$str = <<<'EOD'
Example of string

spanning multiple lines

using nowdoc syntax.
EOD;

/* More complex example, with variables. */
class foo
{

    public $foo;

    public $bar;


    function foo()

    {
$this->foo = 'Foo';
$this->bar = array('Bar1', 'Bar2', 'Bar3');

    }

}

$foo = new foo();
$name = 'MyName';


echo <<<'EOT'
My name is "$name". I am printing some $foo->foo.

Now, I am printing some {$foo->bar[1]}.

This should not print a capital 'A': \x41
EOT;
?>




上边列子的输出：



My name is "$name". I am printing some $foo->foo. Now, I am printing some {$foo->bar[1]}. This should not print a capital 'A': \x41



> 
注意:

与heredocs不同的是，nowdocs可以在任何静态数据环境中使用。一个经典的例子就是初始化类的属性和常量：


Example #7 Static data example


<?php
class foo {

    public $bar = <<<'EOT'
bar
EOT;

}
?>






> 
注意:

从 PHP 5.3.0 开始添加对nowdoc的支持。





变量解析
当一个字符串被指定在双引号或者heredoc当中的时候，变量将会被解析。

有两种语法类型：一个是简单的和一个是复杂的。简单的语法是最平常的，并且特别方便。它提供的方式可以毫不费力的在字符串当中嵌入一个变量、数组的值，或者一个类的属性。

PHP 4当中介绍了复杂语法类型，可以通过表达式周围的花括号辨认出这种方法。

简单句法规则

如果遇到了一个dollar 符号 ($) ，解析器会尽量最大长度的识别出一个正确变量名字。闭合的花括号可以明确的指定变量名字的边界。 



<?php

$juice = "apple";


echo "He drank some $juice juice.".PHP_EOL;

echo "He drank some juice made of $juices."; //错误，juices是一个正确的变量名所以juices不会被识别
?>



上述程序输出：


He drank some apple juice. He drank some juice made of .




同样地，数组的下标和对象的属性也会被解析。数组的下标以方括号为标识（]），对象属性的解析规则与普通变量类似。


Example #8 Simple syntax example


<?php

$juices = array("apple", "orange", "koolaid1" => "purple");


echo "He drank some $juices[0] juice.".PHP_EOL;

echo "He drank some $juices[1] juice.".PHP_EOL;

echo "He drank some juice made of $juice[0]s.".PHP_EOL; // 不工作
echo "He drank some $juices[koolaid1] juice.".PHP_EOL;


class people {

    public $john = "John Smith";

    public $jane = "Jane Smith";

    public $robert = "Robert Paulsen";


    public $smith = "Smith";

}

$people = new people();


echo "$people->john drank some $juices[0] juice.".PHP_EOL;

echo "$people->john then said hello to $people->jane.".PHP_EOL;

echo "$people->john's wife greeted $people->robert.".PHP_EOL;

echo "$people->robert greeted the two $people->smiths."; //
 不工作
?>




上述程序的输出：



He drank some apple juice. He drank some orange juice. He drank some juice made of s. He drank some purple juice. John Smith drank some apple juice. John Smith then said hello to Jane Smith. John Smith's wife
 greeted Robert Paulsen. Robert Paulsen greeted the two .




对于任何复杂的变量，应该使用复杂句法规则。


复杂句法规则


不是因为句法复杂而叫做复杂，而是因为这种方法可以表示复杂的表达式。


字符串当中的任何标量变量、数组元素或者对象属性都可以使用这种方法表示。这种方法要求把字符串中的变量使用花括号括起来。只有当{和$紧挨着的时候才是有效的表达。可以使用"{\$"表示字符串{$。通过例子加以说明：



<?php
// 显示所有错误
error_reporting(E_ALL);

$great = 'fantastic';

// {后有空格，因此输出：This is { fantastic}
echo "This is { $great}";

// 正确表示，输出: This is fantastic
echo "This is {$great}";

echo "This is ${great}";

// 有效
echo "This square is {$square->width}00 centimeters broad."; 


// 有效, quoted keys only work using the curly brace syntax
echo "This works: {$arr['key']}";


// Works
echo "This works: {$arr[4][3]}";

//  错误，PHP会寻找一个名字为foo的常量，但是foo没定义
echo "This is wrong: {$arr[foo][3]}"; 

// 有效
echo "This works: {$arr['foo'][3]}";

// Works.
echo "This works: " . $arr['foo'][3];


echo "This works too: {$obj->values[3]->name}";


echo "This is the value of the var named $name: {${$name}}";


echo "This is the value of the var named by the return value of getName(): {${getName()}}";


echo "This is the value of the var named by the return value of \$object->getName(): {${$object->getName()}}";

// 无效，输出： This is the return value of getName(): {getName()}
echo "This is the return value of getName(): {getName()}";
?>




也可以使用这种方法访问一个类的属性。



<?php
class foo {

    var $bar = 'I am bar.';

}

$foo = new foo();
$bar = 'bar';
$baz = array('foo', 'bar', 'baz', 'quux');

echo "{$foo->$bar}\n";

echo "{$foo->$baz[1]}\n";
?>



The above example will output:



I am bar.

I am bar.



> 
注意：


在{$}中的函数、方法调用、静态类变量和常量从PHP 5开始支持。然而，只有当在变量的名字已经被定义的时候这种方式才有效。







<?php
// Show all errors.
error_reporting(E_ALL);


class beers {

    const softdrink = 'rootbeer';

    public static $ale = 'ipa';

}

$rootbeer = 'A & W';
$ipa = 'Alexander Keith\'s';

// This works; outputs: I'd like an A & W
echo "I'd like an {${beers::softdrink}}\n";

// This works too; outputs: I'd like an Alexander Keith's
echo "I'd like an {${beers::$ale}}\n";
?>






字符串当中存取和修改字符







字符串中的字符可以通过数组的形式进行存取和修改，下标0代表第一个字符。此时，可以把字符串想象成包含若干字符的数组。函数substr()和substr_replace()可以一次修改多个字符。







> 
注意：: [String](http://docs.php.net/manual/en/language.types.string.php)s大括号也适用与这种方法，如字符串中，$str{42}和$str[42]代表的同一个字符。


注意


越界访问会使字符串使用空格填充。Non-integer types are converted to integer.非法的下标引起 E_NOTICE. 写入时，负数下标引起 E_NOTICE 错误，但读取的时候读到的是空字符。 Only the first character of an assigned string is used. Assigning
 empty string assigns NULL byte.（这句翻译的真是乱七八糟）



Example #9 Some string examples


<?php
// Get the first character of a string
$str = 'This is a test.';
$first = $str[0];

// Get the third character of a string
$third = $str[2];

// Get the last character of a string.
$str = 'This is still a test.';
$last = $str[strlen($str)-1]; 

// Modify the last character of a string
$str = 'Look at the sea';
$str[strlen($str)-1] = 'e';

?>



> 
注意：


使用[]和{}访问其他类型的变量（不包括数组或者对象）返回 NULL。






Useful functions and operators
多个字符串可以使用 '.' (dot) 操作符连接在一起。注意 '+' (addition) 不能 用于实现这种功能。 更多内容阅读 [String
 operators](http://docs.php.net/manual/en/language.operators.string.php) 。


有大量的函数可以操作和控制字符串。


阅读 [string functions section](http://docs.php.net/manual/en/ref.strings.php) 学习基本功能函数， 阅读 [regular
 expression functions](http://docs.php.net/manual/en/ref.regex.php) 或者 [Perl-compatible regular expression
 functions](http://docs.php.net/manual/en/ref.pcre.php) 学习高级应用。


也有许多函数用来处理URL字符串，相关详细介绍情阅读 [functions for URL strings](http://docs.php.net/manual/en/ref.url.php)， 还有许多函数是用来加密解密的如[mcrypt](http://docs.php.net/manual/en/ref.mcrypt.php) 和 [mhash](http://docs.php.net/manual/en/ref.mhash.php)。


最后, 不要忘了阅读 [character type functions](http://docs.php.net/manual/en/ref.ctype.php) 。


Converting to string

使用强制类型转换或者 [strval()](http://docs.php.net/manual/en/function.strval.php) 函数可以把某个值转换为字符串。在需要字符串的时候字符串的转换会自动完成。使用 [echo()](http://docs.php.net/manual/en/function.echo.php) 或者 [print()](http://docs.php.net/manual/en/function.print.php) 或者与字符串进行比较的变量都会发生这种转换，,
 在 [Types](http://docs.php.net/manual/en/language.types.php) 和 [Type
 Juggling](http://docs.php.net/manual/en/language.types.type-juggling.php) 章节中对此做了具体介绍。还有 [settype()](http://docs.php.net/manual/en/function.settype.php) 函数。


布尔类型的 TRUE 会转成字符串 "1"。布尔类型的FALSE 被转换成字符串 "" (空字符串)。 这种转换是可逆的。


一个整数或者是浮点数会按照原样转成字符串（包括浮点数的阶部分）。

#   测试了一下，(4.1E+6)被转换成了4100000                                                                                                  

#   还是不太明白浮点数是怎么转的

> 
注意：


The decimal point character is defined in the script's locale (category LC_NUMERIC). See the [setlocale()](http://docs.php.net/manual/en/function.setlocale.php) function.




[数组](http://docs.php.net/manual/en/language.types.array.php)类型的变量会被转换成[字符串](http://docs.php.net/manual/en/language.types.string.php)"Array"；因此，[echo()](http://docs.php.net/manual/en/function.echo.php) 和[print()](http://docs.php.net/manual/en/function.print.php) 不能直接输出如组的内容。查看单个元素可以使用这种结构：  echo
 $arr['foo']。

[对象](http://docs.php.net/manual/en/language.types.object.php) 在
 PHP 4 中被转换成字符串 "Object". 阅读下面章节可以学习如何在调试程序时打印合适的值。可以使用 [get_class()](http://docs.php.net/manual/en/function.get-class.php) 函数获取一个类的名字。自
 PHP 5 开始， 适当的时候可以使用[__toString](http://docs.php.net/manual/en/language.oop5.magic.php) 方法。

[Resource](http://docs.php.net/manual/en/language.types.resource.php)s
 会转化成 "Resource id #1"这种结构的字符串，1 表示的是PHP运行的时候指派的唯一的数字。Do not rely upon this structure; it is subject to change. To get a [resource](http://docs.php.net/manual/en/language.types.resource.php)'s
 type, use the [get_resource_type()](http://docs.php.net/manual/en/function.get-resource-type.php) function.

NULL 总是转换成空字符串。


如上所述，直接转换 [array](http://docs.php.net/manual/en/language.types.array.php), [object](http://docs.php.net/manual/en/language.types.object.php),
 或者 [resource](http://docs.php.net/manual/en/language.types.resource.php) 到 [string](http://docs.php.net/manual/en/language.types.string.php) 不会生成对变量值任何有用的信息。学习函数 [print_r()](http://docs.php.net/manual/en/function.print-r.php) 和 [var_dump()](http://docs.php.net/manual/en/function.var-dump.php) 可以获取关于变量内容的知识。


PHP中大多数的数值可以转换成字符串永久存储。这种技术叫做串行化， 可以通过 [serialize()](http://docs.php.net/manual/en/function.serialize.php) 函数实现。If
 the PHP engine was built with [WDDX](http://docs.php.net/manual/en/ref.wddx.php) support, PHP values
 can also be serialized as well-formed XML text.


String conversion to numbers




如果字符串当中不包括 '.', 'e', 或者 'E' 字符，并且数字值符合整数类型限制（ PHP_INT_MAX 中定义），字符串将会被转换成一个整形数，其它情况，字符串会被转换成浮点数。



数字的值是字符串前面部分确定的，如果一个字符串以一个正确格式的数字形式的字符开头，那么在转换的时候，这个数字就是变量的值，否则，字符串将会被转换成0。数字形的字符串：

符号（可选）＋数字＋阶符（可选）＋数字（可选）



<?php

$foo = 1 + "10.5";                // $foo is float (11.5)
$foo = 1 + "-1.3e3";              // $foo is float (-1299)
$foo = 1 + "bob-1.3e3";           // $foo is integer (1)
$foo = 1 + "bob3";                // $foo is integer (1)
$foo = 1 + "10 Small Pigs";       // $foo is integer (11)
$foo = 4 + "10.2 Little Piggies"; // $foo is float (14.2)
$foo = "10.0 pigs " + 1;          // $foo is float (11)
$foo = "10.0 pigs " + 1.0;        // $foo is float (11)     
?>




更多内容可以在Unix中查阅 strtod(3) 手册。


可以使用下面的语句测试该例子：



<?php
echo "\$foo==$foo; type is " . gettype ($foo) . "<br />\n";
?>




PHP不会像C那样将字符按照ASCII码转换成数字。可以使用 [ord()](http://docs.php.net/manual/en/function.ord.php) 和 [chr()](http://docs.php.net/manual/en/function.chr.php) 函数在
 ASCII 码和字符之间进行转换。

















