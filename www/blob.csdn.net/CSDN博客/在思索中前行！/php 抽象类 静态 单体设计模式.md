# php 抽象类 静态 单体设计模式 - 在思索中前行！ - CSDN博客





2014年06月21日 22:27:10[_Tham](https://me.csdn.net/txl16211)阅读数：739标签：[面向对象																[oop																[静态																[单体](https://so.csdn.net/so/search/s.do?q=单体&t=blog)
个人分类：[php开发](https://blog.csdn.net/txl16211/article/category/2231073)





php oop----抽象类

抽象类机制使得子类可共用基类的某些信息，具体细节会留给子类，典型用在这样情形中，抽象类并不定义全部的方法，部分方法的实现推迟到子类继承抽象类时。它是介于接口和具体类间的一种构造元，接口中的操作都不给出实现---仅是声明，具体类中的所有操作都实现了。（我这里用到了操作和方法两个概念，他们的关系是：方法实现操作，操作是一个对象对外声称自己会提供的功能，方法是对这种声称的实现）。

可认为抽象类是部分类（partial classe），一般情况是一个抽象类中，抽象方法和具体方法的实现共存，但极端情况可以是：抽象类定义的所有方法可以全部是抽象方法或具体方法。不管哪种情况抽象类都必须被继承，不可以直接实例化他们（这一点像接口），抽象方法是缺乏方法体的方法原型（方法声明，或称方法签名），它包括：访问级别，function关键字，方法名称以及参数信息。方法签名不需要大括号，它很想C语言中的方法声明如：

 public function prototypeName($param);



注意一下规则：

1.任何类即使含有一个抽象方法的声明，该类就必须声明为抽象类；

2.任何抽象方法在实现时必须声明一个和抽象方法访问级别相同或更差的访问级别，这是因为有一个面向对象原则：LSP，其内容是子类应无条件替换父类的角色，这个原则决定与父类同名的方法其访问级别应更宽泛，比如比protected访问级别更宽泛的是public，比其更严格的访问级别是private；

3.不可用new关键字来直接实例化一个抽象的类。

4.任何声明为抽象的方法都不可以包含方法体，即使是空方法体：{//no code here}。

5.可以扩展一个抽象的类，但并不实现所有的抽象方法，这时这个子类也声明为抽象的，这样我们可以构建一个抽象类层次结构。（仅当实现抽象父类声明的所有方法签名时这个子类才不是抽象类）

来看例子：

<?php

//定义一个抽象类

Abstract class EnvContext{

    /**

     * 该抽象方法或取特定键所对应的值

     * 配置文件中以键/值对的形式保存配置信息。

     */

    abstract function getValueOf($keyStr);

}

?>

这是我定义的一个抽象类，该类用来获取相关的配置信息，但目前还没有实现。

以下是实现（扩展上面的抽象类）：

<?php

//这是对EnvContext抽象类的一个实现：

class MyEnvContext extends EnvContext{

    private $keyAndValues;


    //构造器方法

    public function __construct(){

       $this->keyAndValues = Array();

       $this->keyAndValues['key']='ok';


    }


    function getValueOf($keyStr){

        return $this->keyAndValues['key'];


        #return 'ok!';

    }

}

$EnvContext=new MyEnvContext();

echo "key 的值是".$EnvContext->getValueOf('key');

?>

注意php中oop技术，关于继承是单根的，一个类不可以继承多个类或抽象类，但可用接口技术来模拟这种需求。（单根继承的类层次结构图是一颗树，但像C++中的多继承会使这个继承层次图变为一个非树型结构（图或“格”）），虽然不可以同时继承多个类但我们可以同时实现多个接口。如果你熟悉设计模式的话那么告诉你可以使用抽象类实现模板方法。

********************************************************************************

接口——interface

接口也是一种构造元，像类一样OOP中我们用这些构造元来搭建更大规模的结构（对象，组件，子系统，系统），接口中声明的方法都没有被实现。接口同抽象类一样，其中的含有方法的原型，但不同于抽象类的是不可以含有一个完整的方法定义（就是说其中的方法全部是原型，没有一个是被完全实现的（都不带方法体））。

我们使用interface关键字来定义一个接口（就像class关键字的用法一样）。有的设计者喜欢在接口名字前冠以前缀I 字符来区分类名和接口名。现在的.net中全部使用这种命名习惯

比如： interface IMyInterface{

      function someFunc();

       }

接口的实现类，用implements关键字声明实现某个接口，如果实现多个接口那么接口间使用逗号隔开：

    class InterfaceImpl   implements IMyInterface1,IMyInterface2[,IMyInterfacei]*{



}

我们还可以同时扩展某个类并实现某些接口：

class SomeClass extends BaseClassOrAbstactClass implements IMyInterface[,IMytnterfacei]{

}

某个类一次仅能扩展（继承）一个类或抽象类但可以实现（implements）多个接口，这样就可以达到多继承的目的。

判断一个对象是否是一个类型的实例可以使用instanceof比较操作符：

$someVar instanceof SomeType/SomeAbstractClass/SomeInterfaceName

该语句返回一个布尔值。用法如：

if($varObj instanceof IMyInterface){

   $varObj->someFunc();

}else{

   //不是IMyInterface的子类.....

}

因为php不是一个强类型语言（尽管提供了少量的强类型支持），一个变量在使用时，特别是从一个方法参数中传递过来时，我们不能确切知道它的类型，很有必要判断一下该参数的类型，只有是期望类型时才可以安全地调用某个方法。



DBC（designe by contract）——契约式编程设计

这种编程方式，是接口设计先行于类设计的，比如大家听到的面向接口设计，使用这种技术我们在构建应用系统时能够标识我们将要实现的功能。类似于架构师先构造一个计划，先于构造系统。

接口可形象地理解为对象的通讯协议，整个系统认为是一个对象图，对象间的联系就是方法调用，对对象发消息（对象通讯）就是调用对象的方法，而方法签名就是通讯协议（如果你从事过底层的套接字编程，那么那种通讯协议也是用来联通两个系统或应用的协议）。

大型系统，或多人开发的系统，开发组间往往先定义各个模块的接口，毕竟完全实现各个模块的方法的用时一般高于定义一个接口的用时，先定义接口会使我们在一个高抽象层次尽快看到系统的概貌，我们可以尽可能早的测试系统，比如如果只是实现了一部分接口那么我们可以用一些桩（用空方法体实现所有的接口操作声明）或者模拟对象（这些对象并不是真正的实现，他们只是根据接口的要求返还某个模拟的值）混同已经实现的模块来测试整个系统，不必等到所有的模块都实现后才测试。并且实现完成时使用接口规则很容易测试一个类的实现是否符合规范，最后对同一个接口的实现使得这些实现可以互相更换而不影响全局功能。总之面向接口的设计是一种很好的设计实践，很多大的企业都采用这种开发模式。







php 静态变量，成员和方法

静态变量，也被称为类变量，他的生存期比方法作用域的变量长，我们可以用静态变量来记录某个方法被调用的次数。方法级的变量生存期一般跟栈有关，而对象和类变量的作用域跟堆有关。可以认为静态变量的生存期等同于应用程序的生存期，且静态变量一般是全局可访问的。以上所说的是在别的语言中静态变量的知识。但是php中的静态变量跟C语言类似：

<?php

function staticVarTest(){

    //声明一个静态变量,和一个局部变量

    static $callTimes=0;

    $localVar=0;


   // echo "staticVarTest be called!";


    $callTimes++;

    $localVar++;

    echo "localVar 局部变量的值是$localVar <br>";

    echo "function staticVarTest has been called $callTimes 次<br>";

}

//以下调用上面的这个方法

staticVarTest();

staticVarTest();

staticVarTest();

staticVarTest();

?>

以上的这个方法其中定义了一个静态变量和局部变量，通过运行你可以发现局部变量在每次方法调用时都被赋予了原始值，而静态变量具有记忆效果，另外这里的静态变量仅能在本方法中访问，而全局变量是所有方法都可以访问的，同时被多个方法访问会导致安全问题，并且你可能还需要导入定义全局变量的文件。所以在某些情况下静态变量还是有用的。

接下来看看类中使用static关键字定义的变量或方法的特征。

一个类的静态成员是属于这个类的，如果你认为一个类能够概括所有该类的对象特征那么类变量就可以概括这个类的特征，比如 人类是伟大的，我是一个人。这句话里伟大的是形容人类的而不一定是形容某个人类的实例（我）。类实例一般是被该类的所有对象共享的，如你我他都应该为人类是伟大的而感到自豪。一定要区分实例变量和类变量，实例方法和类方法，一个类定义中带有static修饰符的变量或方法就是类级别的成员，否则就是实例级别的。只有类方法和类变量才可以用::域解析符，并且静态类成员要使用$符号。

class MyObject {

public static $myStaticVar=0;

function myMethod() {

self::$myStaticVar += 1;

echo self::$myStaticVar . "\n";

}

}

$varObj=new MyObject();

$varObj->myMethod();

注意到访问静态类变量我们使用了::符号，和C++语言中的一样 他被称为域作用解析符/域解析符（scope resolution operator）并且使用了$self关键字，$this表示对对象自己的引用，而$self表示对本类的引用。双冒号::还可用在parent关键字上。

比如你想在子类中调用某个基类的方法，（特别是增量式的扩充基类某个同名方法时）我们可以这样：

class MySubObject extends MyObject{


   public function someFunc(){

        echo 'someFunc of MySubObject been called!';

       parent::myMethod();

    }

//该方法覆盖父类的方法，并且重用了父类的同名方法

public function myMethod(){

        parent::myMethod();

        echo '<br>this code add from subClass';

    }


}

$varObj2=new MySubObject();

$varObj2->someFunc();

$varObj=new MySubObject();

$varObj->myMethod();

如果同时在父类和子类中定义了同名的静态变量那么父类的静态变量会被覆盖，情形同方法覆盖，如果还行引用父类中的变量那么使用parent::$staticVarOfParent。

注意点：类静态成员不能访问实例成员。但反之可以。

类中的静态成员，也被称为类成员，或共享成员（变量或方法）

另一个类成员的用途是，避免使用new实例化类的实例。类方法也是设计工具类的常用手段，我们不需要实例化该类而直接调用该类的方法。试想当一个类仅有一个实例时，那么类与对象的概念有何区别？（类是对多个具有共同特点的对象的抽象，但是如果存在一个和任何东西都没有共同点的对象，你如何描述这个类/对象，这是对象和类会合一？）

<?php

class Employee{

    static $EmpCount;//雇员数量


    public function __construct(){


        ++self::$EmpCount;

       echo "<br>当前的雇员数是".self::$EmpCount;

    }


}

new Employee();

new Employee();

echo "<br>公司有雇员：".Employee::$EmpCount."个";

?>

***

静态方法：

静态方法的使用规则与静态变量类似:

class Employee{

    static $EmpCount;//雇员数量


    public static function GetEmployeeCount(){

        return self::$EmpCount;

    }



    public function __construct(){


        ++self::$EmpCount;

       echo "<br>当前的雇员数是".self::$EmpCount;

    }


}

//下面直接调用该类的静态方法：

echo "<br>公司有雇员：".Employee::GetEmployeeCount()."个";

注意到我们可以使用权限控制修饰符于各种声明的前面（类，变量，方法），当然静态成员可以是私有的，这样只有 该类成员间才可互相访问。

php6还引入了static关键字来协助对类静态成员的访问。有兴趣可以查询相关知识。

关于使用静态成员是存在争议的，至于何时用决定于你自己，这就是全局访问性引起的问题。任何威力巨大的武器都是双刃剑。一些人建议从不使用他们，但是我们在OOP框架中总是见到类静态成员的使用，他们主要用在IOC控制反转上。

***

php 单体设计模式

*

关于设计模式是什么，能干什么，这里不讨论，已有太多的资料讲述他们，我这里只介绍如何在php中使用其中的个别模式。

单体模式：使用该模式保证应用中仅有该类的唯一一个实例，没有任何方法会创建出相应类的多个实例来。

我们使用三点来实现该模式：

1.相应类必须有一个私有的构造方法，（如果提供了多个构造方法，那么确保所有的构造方法都是私有），这样类的使用者无法new出多个实例，私有访问权限使得该构造方法仅对本类有用。

2.使用一个静态成员来保存这个唯一的实例。

3.提供一个公共的静态方法来返回这个唯一的实例。

对于公共对象模型中提供的__clone方法我们也必须阻止它拷贝，仍旧返回唯一的那个实例，或干脆使克隆方法私有。

*

**

单体例子：

<?php

/**

* 该单体类保证，整个应用中不会出现多个数据库

* 的连接，只有总是使用这个类来获取连接而不是

* 底层提供的方法

* */

class MySqlConn{

    //该静态变量用来维护唯一的该类实例

    private static $Singleton;

    public static function GetInstance(){

        //下面的判断和

        //if(!is_null(self::$singleton) 等价 

        //都是判断静态变量是否被赋值过 

        if(!(self::$Singleton instanceof self)){

              // self::$Singleton=new self();

              self::$Singleton=new MySqlConn();

       }


         return self::$Singleton;


    }

    //阻止外界使用者克隆

    private function __clone(){}

    //以上是实现一个单体模式所要进行的共有逻辑

    /*以下的设计同正常类设计，仅构造方法私有而已

      所以如果想把一个类改为单体类，那么加入以上

      逻辑并，私有所有构造方法，这里的构造方法没有

      参数，你可以考虑构造方法带参数的单体设计如何实现

    */

    private $db_conn;


    private function __construct(){

        static $callTimes=0;

        $callTimes++;

        echo "第 $callTimes 次调用MySqlConn的构造方法";


        //连接mysql数据库

        $this->db_conn=mysql_connect("localhost",'root','mysql') 

        or die('连接数据库失败'.mysql_error($this->db_conn));

        //使用数据库test

        //$this->db_conn->select_db('test');

        mysql_selectdb('test',$this->db_conn);

        echo '<br>对象的构造方法调用成功!';

    }


    public function query($sqlStr){

        return mysql_query($sqlStr,$this->db_conn);

    }


}

$dbConn = MySqlConn::GetInstance();

if (is_null($dbConn)) 

     echo '<br>实例化对象失败';

if($dbConn instanceof MySqlConn){

    echo '<br>实例化成功';

}

$result=$dbConn->query('select * from tbl_jokes');



?>

注意单体类有时和纯静态类功能类似，设计一个纯静态类的方法是：

1.使得所有的成员（变量和方法）都为静态的（static）；

2.提供一个私有的构造方法:private function __construct(){},这样我们就无法new该类，如果没有这一步，那么任何类如果没有提供构造方法系统会给我们提供一个默认的构造方法，我们仍能够new出该类的实例，一旦我们提供了一个构造方法——不论带参与否，那么系统将不再为我们提供构造方法。这一点在许多语言中都类似，如C++，java等](https://so.csdn.net/so/search/s.do?q=静态&t=blog)](https://so.csdn.net/so/search/s.do?q=oop&t=blog)](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)




