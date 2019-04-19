# PHP中的Closure闭包 - LC900730的博客 - CSDN博客
2017年07月10日 18:27:19[lc900730](https://me.csdn.net/LC900730)阅读数：687
## Closure
#### 1.什么是Closure
在PHP 4最早称为callback，PHP5.3引入了closure与anonymous function。为了避免由于PHP的弱类型本质，让我们误以为传入的是一般类型参数，5.4中特别为传入的callback加上callable的类型限定；所以在PHP中，callback与closure、anonymous function与callable，事实上是同一个事情，底层都是Closure物件。 
简单上来将，一个匿名function是一个对象，作为一个对象，它属于哪一个类呢?自php 5中引入闭包后，匿名函数对象就属于这个Closure。即PHP中用于代表匿名函数的类，匿名函数会产生这个类型的对象；
### Callback
将函数名称以字符串方式传入，透过call_user_func或者call_user_func_array()去执行以该字符串为名称的函数，类似于C语音的函数指针；
### Anonymous function
没有名称的函数，可被赋值给变量。
```php
$anonyFunc=function($name){
        return "hello ".$name;
    }
    echo $anonyFunc("lc");
```
当我们使用$anony_func时,$anony_func变量是一个Closure物件，Closure::__invoke()魔术方法在物件加上()后，就会调用起作用，执行anonymous function； 
以上可以使用__invoke()方式执行
```php
$anonyFunc=function($name){
        return "hello ".$name;
    }
    echo $anonyFunc->__invoke("lc");
```
如：array_map()第一个参数要求是callback，在PHP没有anonymous function之前，只能是事先写一个函数，然后将函数名字以字符串传递给array_map。这样虽然可行，但是callback通常只使用一次，为了这种一次的函数另外独立出一个函数似乎不妥，那么采用匿名函数就比较合适。
```
var_dump(function (){
    return "hello world";
});
//输出如下
class Closure#1 (0) {}
```
#### 2.Closure常用方法
Closure::_construct   禁止实例化的构造函数 
Closure::bind      复制一个闭包，绑定指定的$this对象和类作用域 
Closure::bindTo    复制当前闭包对象，绑定指定的$this对象和类作用域 
Closure::fromCallable() (php>=7.1.0)
1.将匿名函数在普通函数里面当做参数传入，也可以被返回，这就实现了一个闭包； 
2.把匿名函数当做参数传递
#### 3.Closure::bind
Closure::bind(Closure $closure,object $this [, mixed $newscope=’static’])
参数1：需要绑定 匿名函数
参数2：需要绑定到匿名函数的对象，或者null创建未绑定的闭包
参数3：想要绑定给闭包的类作用域，或’static’表示不改变。如果传入一个对象，则使用这个对象的类型名。类作用域用来决定在闭包中$this对象的私有、包含方法的可见性。
上述是php手册的描述,初学者还是有点困惑，因此稍微了解了一下具体用法。
```php
class A{
    private static $sfoo=1;
    private $ifoo=2;
    private $pfoo=3;
    public function test(Closure $next){
        $next();
    }
    public function test2(){
        echo "test2";
    }
}
class B{
}
$aaa=new A();
$cl1=static function(){
    return A::$sfoo;
};
$cl2=function(){
    return $this->ifoo;
};
$cl3=function(){
    return $this->pfoo;
};
$cl4=function(){
    return A::$ifoo;
};
$bbb=new A();
//第二个参数为object或者为null(创建未绑定的闭包)
$bcl1=Closure::bind($cl1,null);  
$bcl2=Closure::bind($cl1,null);
```
这个闭包函数访问了A类的私有静态属性；类的私有属性只有在类的内部才能访问；但是为什么$bcl1就能够正常访问呢？原因就是第三个参数’A’。如果第三个参数是一个对象呢？
```php
//做对比，加入第三个参数是'A'的情况
$bcl1=Closure::bind($cl1,null,'A');
$bcl3=Closure::bind($cl1,null,new A);
$bcl4=Closure::bind($cl1,null,$aaa);
//结果如下：
echo $bcl1();    //1
echo $bcl3();    //1
echo $bcl4();    //1
```
那么是否可以认为，无论传入该类的实例对象还是该类名，就代表着这个闭包有着整个类的访问控制权限呢？而传入static，则不会提升闭包的访问控制权限，以前的访问控制权限规则依然没有改变，类的外面依然无法访问类内的private； 
此外，在静态闭包无法绑定实例 
$bcl5=Closure::bind($cl1,new A()); 
fatal error：cannot bind an instance to a static closure  
### 3.1 bind的第二个参数
关于bind的第二个参数为object或者为null的情况
```php
class A{
    private static $sfoo=1;
    private $ifoo=2;
    public $pfoo=3;
    public function test(Closure $next){
        $next();
    }
    public function test2(){
        echo "test2";
    }
}
class B{
}
$aaa=new A();
$cc=function(){
    return $this->pfoo;
}
$bcl2=Closure::bind($cc,$aaa);
//$cc中用到了$this,必须有$this的上下文环境
$bcl2();    //3
//因此如果是下面
$bcl3=Closure::bind($cc,null);
$bcl3();  //fatal error using $this when not in obj context
```
因此bind的第二个参数为null还是onject，取决于第一个闭包是否用到了$this的上下文环境。 
绑定的对象决定了函数中的$this的取值。
### 3.2 bind的第三个参数
bind的第三个参数：mixed类型的类作用域，决定了这个匿名函数中能够调用哪些私有和保护的方法。也就是说$this可以调用的方法，即这个$this可以调用的方法与属性，与这个scope一致。 
第三个参数如果是类实例对象与类的名称都代表着这个闭包有类作用域，如果是static则表示这个闭包与外部变量作用域一样，不能访问类的私有以及保护方法。 
mixed类型在PHP中也就是没有类型限定的意思，缺省情况下第三个参数是个字符串，值是‘static’；
```php
class A{
    private static $sfoo=1;
    private $ifoo=2;
    public $pfoo=3;
    public function test(Closure $next){
        $next();
    }
    public function test2(){
        echo "test2";
    }
    private function test3(){
    echo "private fun3";
    }
}
class B{
}
$aaa=new A();
$ccl1=function(){
    return $this->pfoo;
}
$ccl2=function(){
    return $this->ifoo;    
}
$ccl3=function(){
    return $this->test3();
}
$bcl=Closure($ccl1,$aaa);
$bcl2=Closure($ccl1,$aaa);
echo $bcl1(); //3
echo $bcl2(); //Fatal error:Cannot access private proterty 
$bcl=Closure($ccl1,$aaa,'A');
$bc2=Closure($ccl1,$aaa,$aaa);
$bc3=Closure($ccl1,$aaa,'static');
$bc4=Closure($ccl1,$aaa,'B');
echo $bcl1(); //3
echo $bcl2(); //3
echo $bcl3(); //Fatal error
echo $bcl4(); //Fatal error
$bc5=Closure($ccl1,$aaa,$aaa);
$bc6=Closure($ccl1,$aaa,'A');
$bc7=Closure($ccl1,$aaa,'static');
echo($bc5());     //private test
echo($bc6());     //private test
echo($bc7());    //Fatal error
//上述测试了不同类作用域下对私有属性和私有方法，如果第三个参数是static(省略情况下默认static)，那么作用域不会提升，不能访问类的私有方法。如果是类名或者类对象，那么该闭包就有着类作用域，可以访问类的私有及受保护的方法；
echo  $bcl1();  //3
echo  $bcl2(); //Fatal error:Cannot access private proterty
```
