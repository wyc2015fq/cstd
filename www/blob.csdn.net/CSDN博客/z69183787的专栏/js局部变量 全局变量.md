# js局部变量/全局变量 - z69183787的专栏 - CSDN博客
2013年12月28日 12:52:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：858
在js中如果用var来定义一个变量，如果该变量被定义在function或者其他循环体内，则变量为局部变量，并且该变量不会受到同名全局变量的影响，也不会影响同名的全局变量。如果不用var来定义变量而直接拿一个变量来使用则该变量会被自己定义为全局变量，无论该变量被定义在任何位置，如果在function或者循环体内用这种方式来定义变量，则该变量为全局变量若function或循环体外已经定义了同名的全局变量，则会直接引用function或循环体外定义的全局变量。
注意：js中，一个函数里面若有循环体，则在循环体中定义的变量作于域会是整个函数，这一点根其他高级语言不一样。下面例子中的testVar()，在for循环中定义了一个局部变量i，该i的作用域就为整个函数体。
例子：
function testNoVar()
{
    for(i=0;i<10;i++)
    {
        if(i==5) break;
    }
    alert("testNoVar "+i);
}
function testVar()
{
    for(var i=0;i<10;i++)
    {
        if(i==6) break;
    }
    alert("testVar "+i);
}
testNoVar();
try
{
    alert("behind testNoVar i value :"+i);
}
catch(e)
{
    alert("behind testNoVar i value :"+e);
}
testVar();
try
{
    alert("behind testVar i value :"+i);
}
catch(e)
{
    alert("behind testVar i value :"+e);
}
testNoVar中循环体内的i没有用var来定义，该i为全局变量。而testVar中的循环体用var定义了i，该i为局部变量，于是它不受到前面定义的全局变量的影响，所以该例子打印结果的先后循序为：
"testNoVar 5" -> "behind testNoVar i value :5" -> "testVar 6" -> "behind testVar i value :5"
