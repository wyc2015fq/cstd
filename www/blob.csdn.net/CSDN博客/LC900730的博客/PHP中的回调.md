# PHP中的回调 - LC900730的博客 - CSDN博客
2017年05月31日 16:57:15[lc900730](https://me.csdn.net/LC900730)阅读数：409
## 1.全局函数的回调
这里的全局函数是指function定义的函数，不包含在任何对象或者类中。如
```php
function fnCallBack( $msg1 , $msg2 )
{
    echo 'msg1:'.$msg1;
    echo "<br />\n";
    echo 'msg2:'.$msg2;
}
$fnName = "fnCallBack";
$params = array( 'hello' , 'world' );
call_user_func_array( $fnName , $params );
```
使用php内置的函数call_user_func_array来进行调用。第一个参数是一个字符串，表示要调用的函数名，第2个参数是一个数组，表示参数列表，按照顺序依次传递给要调用的函数
## 2.类中静态方法的回调
```php
class MyClass
{
    public static function fnCallBack( $msg1 , $msg2 )
    {
        echo 'msg1:'.$msg1;
        echo "<br />\n";
        echo 'msg2:'.$msg2;
    }
}
$className = 'MyClass';
$fnName = "fnCallBack";
$params = array( 'hello' , 'world' );
call_user_func_array( array( $className , $fnName ) , $params );
```
类中方法没有static的也可以正常调用。(前提是回调函数中没有出现$this)
## 3.对象方法的回调
```php
class MyClass
{
    private $name="abc";
    public function fnCallBack( $msg1='default msg1' , $msg2='default msg2' )
    {
        echo 'object name:'.$this->name;
        echo "<br />\n";
        echo 'msg1:'.$msg1;
        echo 'msg2:'.$msg2;
    }
}
$myobj=new MyClass();
$fnName = "fnCallBack";
$params = array( 'hello' , 'world' );
$myobj->$fnName();
```
调用时成功了，但是无法传递参数。
## create_function
