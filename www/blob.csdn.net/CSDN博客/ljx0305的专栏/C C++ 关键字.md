# C/C++ 关键字 - ljx0305的专栏 - CSDN博客
2009年03月25日 20:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：790
# C/C++ 关键字
## asm 
*语法:*
  asm( "instruction" );
asm允许你在你的代码中直接插入汇编语言指令， 各种不同的编译器为这一个指令允许不一致形式, 比如：
    asm {
      instruction-sequence
    }
or 
    asm( instruction );
    
## auto 
 关键字auto是用来声明完全可选择的局部变量的 
## bool 
关键字bool是用来声明布尔逻辑变量的；也就是说,变量要么是真，要么是假。举个例子： 
    bool done = false;
    while( !done ) {
    ...
    }
    
你也可以查看 data types 这一页. 
## break 
关键字break是用来跳出一个do, for, or while 的循环. 它也可以结束一个 switch 语句的子句,让程序忽略下面的case代码.举个例子:
    while( x < 100 ) {
      if( x < 0 )
        break;
      cout << x << endl;
      x++;
    }
break语句只能跳出本层循环,假如你要跳出一个三重嵌套的循环,你就要使用包含其它的逻辑或者用一个goto语句跳出这个嵌套循环. 
## case 
 在switch里面用来检测匹配 . 
*相关主题:*
**default, switch**
## catch 
 catch 语句通常通过 throw 语句捕获一个异常. 
*相关主题:*
**throw, try**
## char 
关键字char用来声明布尔型变量. 你也可以查看 data types 这一页... 
## class 
*语法:*
  class class-name : inheritance-list {
    private-members-list;    
    protected:
      protected-members-list;
    public:
      public-members-list;
  } object-list;
关键字class允许你创建新的数据类型. *class-name* 就是你要创建的类的名字,并且 *inheritance-list* 是一个对你创建的新类可供选择的定义体的表单.类的默认为私有类型成员,除非这个表单标注在公有或保护类型之下. *object-list* 是一个或一组声明对象.举个例子:
    class Date {
      int Day;
      int Month;
      int Year;
    public:
      void display();
    };
*相关主题:*
**struct, union**
## const 
关键字const用来告诉编译器一个一旦被初始化过的变量就不能再修改.
*相关主题:*
**const_cast**
## const_cast 
*语法:*
  const_cast<type> (object);
关键字const用于移除"const-ness"的数据,目标数据类型必须和原类型相同,目标数据没有被const定义过除外. 
*相关主题:*
**dynamic_cast, reinterpret_cast, static_cast**
## continue 
continue语句用来结束这次循环在一个循环语句中,例如,下面这段代码会显示所有除了10之外0-20的所有数字: 
    for( int i = 0; i < 21; i++ ) {
      if( i == 10 ) {
        continue;
      }
      cout << i << " ";
    }
*相关主题:*
**break, do, for, while**
## default 
switch 语句中的缺省条件. 
*相关主题:*
**case, switch**
## delete 
*语法:*
  delete p;
  delete[] pArray;
delete操作用来释放*p*指向的内存.这个指针先前应该被 new调用过.上面第二种形式用于删除一个数组.
*相关主题:*
**new**
## do 
*语法:*
  do {
    statement-list;
  } while( condition );
do构建一个循环语句表,直到条件为假.注意循环中的语句至少被执行一次,因为判断条件在循环的最后.
*相关主题:*
**for, while**
## double 
关键字double用来声明浮点型变量的精度.  你也可以查看 data types 这一页. 
## dynamic_cast 
*语法:*
  dynamic_cast<type> (object);
关键字dynamic_cast强制将一个类型转化为另外一种类型，并且在执行运行时时检查它保证它的合法性。如果你想在两个互相矛盾的类型之间转化时，cast的返回值将为NULL.
*相关主题:*
**const_cast, reinterpret_cast, static_cast**
## else 
关键字else用在 if 语句中的二中选一. 
## enum 
*语法:*
  enum name {name-list} var-list;
关键字enum用来创建一个包含多个名称元素的名称表.*var-list* 是可选的. 例如: 
    enum color {red, orange, yellow, green, blue, indigo, violet};
    color c1 = indigo;
    if( c1 == indigo ) {
      cout << "c1 is indigo" << endl;
    }
## explicit 
当构造函数被指定为explicit的时候,将不会自动把构造函数作为转换构造函数,这仅仅用在当一个初始化语句参数与这个构造函数的形参匹配的情况.
## extern 
关键字extern用来告知编译器变量在当前范围之外声明过了.被extern语句描述过的变量将分派不到任何空间,因为他们在别的地方被定义过了.
Extern语句频繁的用于在多个文件之间的跨范围数据传递. 
## false 
"false"是布尔型的值. 
*相关主题:*
**bool, true**
## float 
关键字float用来声明浮点型变量. 你也可以查看 data types 这一页. 
## for 
*语法:*
  for( initialization; test-condition; increment ) {
    statement-list;
  }
for构造一个由4部分组成的循环: 
- 初始化, 可以由0个或更多的由逗号分开的初始化语句构成; 
- 判断条件,如果满足该语句循环继续执行; 
- 增量, 可以由0个或更多的由逗号分开的增量语句构成; 
- 语句体,由0个或更多的语句构成,当循环条件成立时他们被执行. 
例如: 
    for( int i = 0; i < 10; i++ ) {
      cout << "i is " << i << endl;
    }
    int j, k;
    for( j = 0, k = 10;
         j < k;
	 j++, k-- ) {
      cout << "j is " << j << " and k is " << k << endl;
    }
    for( ; ; ) {
      // loop forever!
    }
*相关主题:*
**do, while**
## friend 
关键字friend允许类或函数访问一个类中的私有数据. 
## goto 
*语法:*
  goto labelA;
  ...
  labelA:
goto语句可以完成从当前位置到指定标志位的跳转.使用goto语句要考虑有害性,所以它不经常使用. 例如, goto可以用来跳出多重嵌套 for 循环,它比额外的逻辑性跳出更具有时效性. 
*相关主题:*
**break**
## if 
*语法:*
  if( conditionA ) {
    statement-listA;
  }
  else if( conditionB ) {
    statement-listB;
  }
  ...
  else {
    statement-listN;
  }
if 构造一个允许不同的代码在不同的条件下执行的分支机制.*conditions* 是判断语句,*statement-list* . 假如条件为假, else语句块将被执行,所有的else是可选的. 
*相关主题:*
**else, for, while**
## inline 
*语法:*
  inline int functionA( int i ) {
    ...
  }
 inline这个关键字请求编译器扩张一个给定的函数。它向这个函数发出一条插入代码的call。函数里面有静态变量，嵌套的，switches，或者是递归的时候不给予内联。当一个函数声明包含在一个类声明里面时，编译器会尝试的自动把函数内联。
关键字inline请求编译器给一个函数扩展空间,它向这个函数发出一条插入代码的call.函数里面有 static 数据, 循环, switches, 或者是递归的时候不给予内联.当一个函数声明包含在一个类声明里面时,编译器会尝试的自动把函数内联. 
## int 
关键字int 用来声明整型变量. 你也可以查看 data types 这一页. 
## long 
关键字keyword 用来修正数据类型,它用来声明长整型变量. 查看 data types 这一页. 
*相关主题:*
**short**
## mutable 
关键字mutable 忽略所有const 语句.一个属于const对象的mutable 成员可以被修改.
## namespace 
*语法:*
  namespace name {
    declaration-list;
  }
关键字namespace允许你创建一个新的空间.名字由你选择,忽略创建没有命名的名字空间.一旦你创建了一个名字空间,你必须明确地说明它或者用关键字using. 例如: 
    namespace CartoonNameSpace {
      int HomersAge;
      void incrementHomersAge() {
        HomersAge++;
      }
    }
    int main() {
      ...
      CartoonNameSpace::HomersAge = 39;
      CartoonNameSpace::incrementHomersAge();
      cout << CartoonNameSpace::HomersAge << endl;
      ...
    }
## new 
*语法:*
  pointer = new type;
  pointer = new type( initializer );
  pointer = new type[size];
new可以给数据类型分配一个新结点并返回一个指向新分配内存区的首地址. 也可以对它进行初始化.中括号中的 *size* 可以分配尺寸大小.
*相关主题:*
**delete**
## operator 
*语法:*
  return-type class-name::operator#(parameter-list) {
    ...
  }
  return-type operator#(parameter-list) {
    ...
  }
关键字operator 用于重载函数.在上面语法中用特殊符(#)描述特征的操作将被重载.假如在一个类中,类名应当被指定.对于一元的操作, *parameter-list* 应当为空, 对于二元的操作,在operator右边的 *parameter-list* 应当包含操作数 (在operand 左边的被当作 this通过). 
对于不属于重载函数的operator成员,在左边的操作数被作为第一个参数,在右边的操作数被当作第二个参数被通过.
你不能用**#**, **##**, **.**, **:**, **.***, 或者 **?** 标志重载. 
## private 
属于私有类的数据只能被它的内部成员访问,除了friend 使用.关键字private 也能用来继承一个私有的基类,所有的公共和保护成员的基类可以变成私有的派生类.
*相关主题:*
**protected, public**
## protected 
保护数据对于它们自己的类是私有的并且能被派生类继承.关键字keyword 也能用于指定派生,所有的公共和保护成员的基类可以变成保护的派生类.
*相关主题:*
**private, public**
## public 
在类中的公共数据可以被任何人访问.关键字public也能用来指定派生,所有的公共和保护成员的基类可以变成保护的派生类.
*相关主题:*
**private, protected**
## register 
关键字register 请求编译器优化它定义的变量,并且通常这种优化比人工优化的好.
*相关主题:*
**auto**
## reinterpret_cast 
*语法:*
  reinterpret_cast<type> (object);
reinterpret_cast 操作能把一种数据类型改变成另一种.它应当被用在两种不可调和的指针类型之间.
*相关主题:*
**const_cast, dynamic_cast, static_cast**
## return 
*语法:*
  return;
  return( value );
return 语句可以从当前函数跳转到调用该函数的任何地方.返回值是任意的.一个函数可以有不止一个返回语句.
## short 
关键字short用来修正数据类型,用来声明短整型变量.查看 data types 这一页. 
*相关主题:*
**long**
## signed 
关键字signed 用来修正数据类型,用来声明符号字符型型变量. 查看 data types 这一页. 
*相关主题:*
**unsigned**
## sizeof 
sizeof 操作用来用字节计算右边表达式并返回字节数.
## static 
static 数据类型用来给变量创建永久存储空间.静态变量在函数间调用时保持他们的值不变.当用在一个类中时,所有要用到静态变量的时候这个类将把这个变量镜像过去.
## static_cast 
*语法:*
  static_cast<type> (object);
关键字static_cast 用来在两个不同类型之间进行强制转换,并且没有运行时间检查.
*相关主题:*
**const_cast, dynamic_cast, reinterpret_cast**
## struct 
*语法:*
  struct struct-name : inheritance-list {
    public-members-list;    
    protected:
      protected-members-list;
    private:
      private-members-list;
  } object-list;
Structs 类似于 classes, struct中的成员更像是类中的公共成员.在C中,structs 仅能包含数据并不允许有继承表. 例如: 
    struct Date {
      int Day;
      int Month;
      int Year;
    };
*相关主题:*
**class, union**
## switch 
*语法:*
  switch( expression ) {
    case A:
      statement list;
      break;
    case B:
      statement list;
      break;
    ...
    case N:
      statement list;
      break;
    default:
      statement list;
      break;
  }
switch语句允许你通过一个表达式判断许多数值,它一般用来在多重循环中代替if()...else if()...else if()... 语句. break 语句必须在每个case 语句之后, 负责循环将执行所有的case语句.default case 是可选的.假如所有的case都不能匹配的话,他将和defult case匹配. 例如: 
    char keystroke = getch();
    switch( keystroke ) {
      case 'a':
      case 'b':
      case 'c':
      case 'd':
        KeyABCDPressed();
        break;
      case 'e':
        KeyEPressed();
        break;
      default:
        UnknownKeyPressed();
        break;
    }
*相关主题:*
**break, case, default, if**
## template 
*语法:*
  template <class data-type> return-type name( parameter-list ) {
    statement-list;
  }
Templates能用来创建一个对未知数据类型的操作的函数模板.这个通过用其它数据类型代替一个占位符*data-type* 来实现. 例如: 
    template<class X> void genericSwap( X &a, X &b ) {
      X tmp;
  
      tmp = a;
      a = b;
      b = tmp;
    }     
    int main(void) {
      ...
      int num1 = 5;
      int num2 = 21;
      cout << "Before, num1 is " << num1 << " and num2 is " << num2 << endl;
      genericSwap( num1, num2 );
      cout << "After, num1 is " << num1 << " and num2 is " << num2 << endl;
      char c1 = 'a';
      char c2 = 'z';
      cout << "Before, c1 is " << c1 << " and c2 is " << c2 << endl;
      genericSwap( c1, c2 );
      cout << "After, c1 is " << c1 << " and c2 is " << c2 << endl;
      ...
      return( 0 );
    }
## this 
关键字 this 指向当前对象.所有属于一个 class 的函数成员都有一个this 指向. 
*相关主题:*
**class**
## throw 
*语法:*
  try {
    statement list;
  }
  catch( typeA arg ) {
    statement list;
  }
  catch( typeB arg ) {
    statement list;
  }
  ...
  catch( typeN arg ) {
    statement list;
  }
throw 在C++体系下用来处理异常.同try 和 catch 语句一起使用, C++ 处理异常的系统给程序一个比较可行的机制用于错误校正.当你通常在用 try 去执行一段有潜在错误的代码时.在代码的某一处,一个**throw** 语句会被执行, 这将会从try的这一块跳转到catch的那一块中去. 例如: 
    try {
      cout << "Before throwing exception" << endl;
      throw 42;
      cout << "Shouldn't ever see this" << endl;
    }  
    catch( int error ) {
      cout << "Error: caught exception " << error << endl;
    }
*相关主题:*
**catch, try**
## true 
"true"是布尔型的值. 
*相关主题:*
**bool, false**
## try 
try 语句试图去执行由异常产生的代码. 查看 throw 语句获得更多细节. 
*相关主题:*
**catch, throw**
## typedef 
*语法:*
  typedef existing-type new-type;
关键字typedef 允许你从一个现有的类型中创建一个新类型. 
## typeid 
*语法:*
  typeid( object );
typeid 操作返回给一个**type_info** 定义过的对象的那个对象的类型. 
## typename 
关键字typename能用来在中 template 描述一个未定义类型或者代替关键字class.
## union 
*语法:*
  union union-name {
    public-members-list;    
    private:
      private-members-list;
  } object-list;
Unions 类似于 classes,除了所有的成员分享同一内存外它的缺省值更像公共类型. 例如: 
    union Data {
      int i;
      char c;
    };
*相关主题:*
**class, struct**
## unsigned 
关键字keyword 用来修正数据类型,它用来声明无符整型变量. 查看 data types 这一页. 
*相关主题:*
**signed**
## using 
关键字keyword 用来在当前范围输入一个namespace. 
*相关主题:*
**namespace**
## virtual 
*语法:*
  virtual return-type name( parameter-list );
  virtual return-type name( parameter-list ) = 0;
关键字virtual 能用来创建虚函数,它通常不被派生类有限考虑.但是假如函数被作为一个纯的虚函数 (被=0表示)时, 这种情况它一定被派生类有限考虑. 
## volatile 
关键字volatile在描述变量时使用,阻止编译器优化那些以valatile修饰的变量,volatile被用在一些变量能被意外方式改变的地方,例如:抛出中断,这些变量若无volatile可能会和编译器执行的优化 相冲突.
## void 
关键字keyword 用来表示一个函数不返回任何值,或者普通变量能指向任何类型的数据. Void 也能用来声明一个空参数表. 你也可以查看 data types 这一页. 
## wchar_t 
关键字 wchar_t 用来声明字符变量的宽度. 你也可以查看 data types 这一页. 
## while 
*语法:*
  while( condition ) {
    statement-list;
  }
关键字 while 用于一个只要条件未真就执行*statement-list*的循环体.注意假如起始条件为false,*statement-list* 将不被执行. (你可以用一个do循环来保证statement-list至少被执行一次.) 例如: 
    bool done = false;
    while( !done ) {
      ProcessData();
      if( StopLooping() ) {
        done = true;
      }
    }
引用:http://blog.csdn.net/xxyakoo/archive/2008/10/27/3161040.aspx
