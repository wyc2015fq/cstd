# 编写高效的C程序与C代码优化 - 文章 - 伯乐在线
原文出处： [codeproject](http://www.codeproject.com/Articles/6154/Writing-Efficient-C-and-C-Code-Optimization)   译文出处：[CodingWu的博客](http://www.cnblogs.com/archimedes/p/writing-efficient-c-and-code-optimization.html)
虽然对于优化C代码有很多有效的指导方针，但是对于彻底地了解编译器和你工作的机器依然无法取代，通常，加快程序的速度也会加大代码量。这些增加的代码也会影响一个程序的复杂度和可读性，这是不可接受的，比如你在一些小型的设备上编程，例如：移动设备、PDA……，这些有着严格的内存限制，于是，在优化的座右铭是:写代码在内存和速度都应该优化。
## 整型数 / Integers
在我们知道使用的数不可能是负数的时候，应该使用unsigned int取代int，一些处理器处理整数算数运算的时候unsigned int比int快，于是，在一个紧致的循环里面定义一个整型变量，最好这样写代码：

C
```
register unsigned int variable_name;
```
然而，我们不能保证编译器会注意到那个register关键字，也有可能，对某种处理器来说，有没有unsigned是一样的。这两个关键字并不是可以在所有的编译器中应用。记住，整形数运算要比浮点数运算快得多，因为处理器可以直接进行整型数运算，浮点数运算需要依赖于外部的浮点数处理器或者浮点数数学库。我们处理小数的时候要精确点些（比如我们在做一个简单的统计程序时），要限制结果不能超过100，要尽可能晚的把它转化成浮点数。
## 除法和余数 / Division and Remainder
在标准的处理器中，根据分子和分母的不同，一个32位的除法需要20-140个时钟周期来执行完成，等于一个固定的时间加上每个位被除的时间。
Time (分子/ 分母) = C0 + C1* log2 (分子/分母)
= C0 + C1 * (log2 (分子) – log2 (分母)).
现在的ARM处理器需要消耗20+4.3N个时钟周期，这是一个非常费时的操作，要尽可能的避免。在有些情况下，除法表达式可以用乘法表达是来重写。比方说，(a/b)>c可以写成a>(c*b),条件是我们已经知道b为非负数而且b*c不会超过整型数的取值范围。如果我们能够确定其中的一个操作数为unsigned，那么使用无符号除法将会更好，因为它要比有符号除法快得多。
## 合并除法运算和取余运算 / Combining division and remainder
在一些情况下，除法运算和取余运算都需要用到，在这种情况下，编译器会将除法运算和取余运算合并，因为除法运算总是同时返回商和余数。如果两个运算都要用到，我们可以将他们写到一起。

C
```
typedef unsigned int uint;
uint div32u (uint a) {
     return a / 32;
}
int div32s (int a) {
     return a / 32;
}
```
这两种除法都会避免调用除法函数，另外，无符号的除法要比有符号的除法使用更少的指令。有符号的除法要耗费更多的时间，因为这种除法是使最终结果趋向于零的，而移位则是趋向于负无穷。
## 取模运算的替换 / An alternative for modulo arithmetic
我们一般使用取余运算进行取模，不过，有时候使用 if 语句来重写也是可行的。考虑下面的两个例子：

C
```
uint modulo_func1 (uint count)
{
    return (++count % 60);
}
uint modulo_func2 (uint count)
{
    if (++count >= 60)
        count = 0;
    return (count);
}
```
第二个例子要比第一个更可取，因为由它产生的代码会更快，注意：这只是在count取值范围在0 – 59之间的时候才行。
但是我们可以使用如下的代码（笔者补充）实现等价的功能：

C
```
uint modulo_func3 (uint count)
{
    if (++count >= 60)
        count %= 60;
    return (count);
}
```
## 使用数组索引 / Using array indices
假设你要依据某个变量的值，设置另一个变量的取值为特定的字符，你可能会这样做：

C
```
switch(queue) {
    case 0 :   letter = 'W';
        break;
    case 1 :   letter = 'S';
        break;
    case 2 :   letter = 'U';
        break;
}
```
或者这样：

C
```
if(queue == 0)
    letter = 'W';
else if ( queue == 1 )
    letter = 'S';
else
    letter = 'U';
```
有一个简洁且快速的方式是简单的将变量的取值做成一个字符串索引，例如：

C
```
static char *classes = "WSU";
letter = classes[queue];
```
##  全局变量 / Global variables
全局变量不会被分配在寄存器上，修改全局变量需要通过指针或者调用函数的方式间接进行。所以编译器不会将全局变量存储在寄存器中，那样会带来额外的、不必要的负担和存储空间。所以在比较关键的循环中，我们要不使用全局变量。
如果一个函数要频繁的使用全局变量，我们可以使用局部变量，作为全局变量的拷贝，这样就可以使用寄存器了。条件是本函数调用的任何子函数不使用这些全局变量。
举个例子：

C
```
int f(void);
int g(void);
int errs;
void test1(void)
{
    errs += f();
    errs += g();
}
void test2(void)
{
    int localerrs = errs;
    localerrs += f();
    localerrs += g();
    errs = localerrs;
}
```
可以看到test1()中每次加法都需要读取和存储全局变量errs，而在test2()中，localerrs分配在寄存器上，只需要一条指令。
## 使用别名 / Using Aliases
考虑下面的例子：

C
```
void func1( int *data )
{
    int i;
    for(i = 0; i < 10; i++)
        anyfunc(*data, i);
}
```
即使*data从来没有变化，编译器却不知道anyfunc()没有修改它，于是程序每次用到它的时候，都要把它从内存中读出来，可能它只是某些变量的别名，这些变量在程序的其他部分被修改。如果能够确定它不会被改变，我们可以这样写：

C
```
void func1( int *data )
{
int i;
int localdata;
localdata = *data;
for(i=0; i<10; i++)
anyfunc(localdata, i);
}
```
这样会给编译器优化工作更多的选择余地。
## 活跃变量和泄漏 / Live variables and spilling
寄存器的数量在每个处理器当中都是固定的，所以在程序的某个特定的位置，可以保存在寄存器中的变量的数量是有限制的。有些编译器支持“生命周期分割”（live-range splitting），也就是说在函数的不同部分，变量可以被分配到不同的寄存器或者内存中。变量的生存范围被定义成：起点是对该变量的一次空间分配，终点是在下次空间分配之前的最后一次使用之间。在这个范围内，变量的值是合法的，是活的。在生存范围之外，变量不再被使用，是死的，它的寄存器可以供其他变量使用，这样，编译器就可以安排更多的变量到寄存器当中。
可分配到寄存器的变量需要的寄存器数量等于经过生命范围重叠的变量的数目，如果这个数目超过可用的寄存器的数量，有些变量就必须被暂时的存储到内存中。这种处理叫做“泄漏(spilling)”。
编译器优先释放最不频繁使用的变量，将释放的代价降到最低。可以通过以下方式避免变量的“释放”：
- 限制活跃变量的最大数目：通常可以使用简单小巧的表达式，在函数内部不使用太多的变量。把大的函数分割成更加简单的、更加小巧的多个函数，也可能会有所帮助。
- 使用关键字register修饰最经常使用的变量：告诉编译器这个变量将会被经常用到，要求编译器使用非常高的优先级将此变量分配到寄存器中。尽管如此，在某些情况下，变量还是可能被泄漏。
## 变量类型 / Variable Types
C编译器支持基本的变量类型：char、short、int、long(signed、unsigned)、float、double。为变量定义最恰当的类型，非常重要，因为这样可以减少代码和数据的长度，可以非常显著的提高效率。
## 局部变量 / Local variables
如果可能，局部变量要避免使用char和short。对于char和short类型，编译器在每次分配空间以后，都要将这种局部变量的尺寸减少到8位或16位。这对于符号变量来说称为符号扩展，对无符号变量称为无符号扩展。这种操作是通过将寄存器左移24或16位，然后再有符号（或无符号的）右移同样的位数来实现的，需要两条指令（无符号字节变量的无符号扩展需要一条指令）。
这些移位操作可以通过使用int和unsigned int的局部变量来避免。这对于那些首先将数据调到局部变量然后利用局部变量进行运算的情况尤其重要。即使数据以8位或16位的形式输入或输出，把他们当作32位来处理仍是有意义的。
我们来考虑下面的三个例子函数：

C
```
int wordinc (int a)
{ 
    return a + 1;
}
short shortinc (short a)
{ 
    return a + 1;
}
char charinc (char a)
{ 
    return a + 1;
}
```
他们的运算结果是相同的，但是第一个代码片断要比其他片断运行的要快。
## 指针 / Pointers
如果可能，我们应该使用结构体的引用作为参数，也就是结构体的指针，否则，整个结构体就会被压入堆栈，然后传递，这会降低速度。程序适用值传递可能需要几K字节，而一个简单的指针也可以达到同样的目的，只需要几个字节就可以了。
如果在函数内部不会改变结构体的内容，那么就应该将参数声明为const型的指针。举个例子：

C
```
void print_data_of_a_structure (const Thestruct  *data_pointer)
{
     ...printf contents of the structure...
}
```
这个例子代码告知编译器在函数内部不会改变外部结构体的内容，访问他们的时候，不需要重读。还可以确保编译器捕捉任何修改这个只读结构体的代码，给结构体以额外的保护。
## 指针链 / Pointer chains
指针链经常被用来访问结构体的信息，比如，下面的这段常见的代码：

C
```
typedef struct { int x, y, z; } Point3;
typedef struct { Point3 *pos, *direction; } Object;
void InitPos1(Object *p)
{
    p->pos->x = 0;
    p->pos->y = 0;
    p->pos->z = 0;
}
```
代码中，处理器在每次赋值操作的时候都要重新装载p->pos，因为编译器不知道p->pos->x不是p->pos的别名。更好的办法是将p->pos缓存成一个局部变量，如下：

C
```
void InitPos2(Object *p)
{ 
    Point3 *pos = p->pos;
    pos->x = 0; 
    pos->y = 0;
    pos->z = 0;
}
```
另一个可能的方法是将Point3结构体包含在Object结构体中，完全避免指针的使用。
## 条件的执行 / Conditional Execution
条件执行主要用在if语句中，同时也会用到由关系运算(<,==,>等)或bool运算(&&, !等)组成的复杂的表达式。尽可能的保持if和else语句的简单是有好处的，这样才能很好的条件化。关系表达式应该被分成包含相似条件的若干块。
下面的例子演示了编译器如何使用条件执行：

C
```
int g(int a, int b, int c, int d)
{
    if(a > 0 && b > 0 && c < 0 && d < 0)  //分组化的条件被捆绑在一起
        return a + b + c + d;
    return -1;
}
```
条件被分组，便以其能够条件化他们。
## Boolean表达式和范围检查 / Boolean Expressions & Range checking
有一种常见的boolean表达式被用来检查是否一个变量取值在某个特定的范围内，比方说，检查一个点是否在一个窗口内。

C
```
bool PointInRectangelArea (Point p, Rectangle *r)
{
    return (p.x >= r->xmin && p.x < r->xmax && p.y >= r->ymin && p.y < r->ymax);
}
```
这里还有一个更快的方法：把(x >= min && x < max) 转换成 (unsigned)(x-min) < (max-min). 尤其是min为0时，更为有效。下面是优化后的代码：

C
```
bool PointInRectangelArea (Point p, Rectangle *r)
{
    return ((unsigned) (p.x - r->xmin) < r->xmax && (unsigned) (p.y - r->ymin) < r->ymax);
}
```
## Boolean表达式&与零的比较 / Boolean Expressions & Compares with zero
在比较(CMP)指令后，相应的处理器标志位就会被设置。这些标志位也可以被其他的指令设置，诸如MOV, ADD, AND, MUL, 也就是基本的数学和逻辑运算指令（数据处理指令）。假如一条数据处理指令要设置这些标志位，那么N和Z标志位的设置方法跟把数字和零比较的设置方法是一样的。N标志位表示结果是不是负数，Z标志位表示结果是不是零。
在C语言中，处理器中的N和Z标志位对应的有符号数的关系运算符是x < 0, x >= 0, x == 0, x != 0，无符号数对应的是x == 0, x != 0 (or x > 0)。
C语言中，每用到一个关系运算符，编译器就会产生一个比较指令。如果关系运算符是上面的其中一个，在数据处理指令紧跟比较指令的情况下，编译器就会将比较指令优化掉。比如：

C
```
int aFunction(int x, int y)
{
    if (x + y < 0)
        return 1;
    else
        return 0;
}
```
这样做，会在关键循环中节省比较指令，使代码长度减少，效率增加。C语言中没有借位(carry)标志位和溢出(overflow)标志位的概念，所以如果不使用内嵌汇编语言，要访问C和V标志位是不可能的。尽管如此，编译器支持借位标志位（无符号数溢出），比方说：

C
```
int sum(int x, int y)
{
     int res;
     res = x + y;
     if ((unsigned) res < (unsigned) x) // carry set?  //
        res++;
     return res;
}
```
## 惰性评估计算 / Lazy Evaluation Exploitation
在类似与这样的 if(a>10 && b=4) 语句中, 确保AND表达式的第一部分最有可能为false, 结果第二部分极有可能不被执行.
用switch() 代替if…else…，在条件选择比较多的情况下，可以用if…else…else…，像这样：

C
```
if( val == 1)
    dostuff1();
else if (val == 2)
    dostuff2();
else if (val == 3)
    dostuff3();
```
使用switch可以更快：

C
```
switch( val )
{
    case 1: dostuff1(); break;
    case 2: dostuff2(); break;
    case 3: dostuff3(); break;
}
```
在if语句中，即使是最后一个条件成立，也要先判断所有前面的条件是否成立。Switch语句能够去除这些额外的工作。如果你不得不使用if…else，那就把最可能的成立的条件放在前面。
## 二分分解 / Binary Breakdown
把判断条件做成二进制的风格，比如，不要使用下面的列表：

C
```
if(a == 1) { 
    } else if(a == 2) { 
    } else if(a == 3) { 
    } else if(a == 4) { 
    } else if(a == 5) { 
    } else if(a == 6) { 
    } else if(a == 7) { 
    } else if(a == 8) { 
    }
}
```
而采用：

C
```
if(a <= 4) { 
    if(a == 1) { 
    } else if(a == 2) { 
    } else if(a == 3) { 
    } else if(a == 4) { 
    } 
} else { 
    if(a == 5) { 
    } else if(a == 6) { 
    } else if(a == 7) { 
    } else if(a == 8) { 
    } 
}
```
甚至：

C
```
if(a <= 4) { 
    if(a <= 2) { 
        if(a == 1) { 
                /* a is 1 */ 
        } else { 
                /* a must be 2 */ 
        } 
    } else { 
        if(a == 3) { 
                /* a is 3 */ 
        } else { 
                /* a must be 4 */ 
        } 
    } 
} else { 
    if(a <= 6) { 
        if(a == 5) { 
                /* a is 5 */ 
        } else { 
                /* a must be 6 */ 
        } 
    } else { 
        if(a == 7) { 
                /* a is 7 */ 
        } else { 
                /* a must be 8 */ 
        } 
    } 
}
```
慢速、低效：

C
```
c = getch();
switch(c){
    case 'A': {
        do something;  
        break;  
    } 
    case 'H': {
        do something;
        break;
    }  
    case 'Z': { 
        do something; 
        break; 
    }
}
```
快速、高效：

C
```
c = getch();
switch(c) {
    case 0: {
        do something;
        break;
    }  
    case 1: {
        do something; 
        break;
    } 
    case 2: {
        do something; 
        break; 
    }
}
```
以上是两个case语句之间的比较
## switch语句和查找表 / Switch statement vs. lookup tables
switch语句通常用于以下情况：
- 调用几个函数中的一个
- 设置一个变量或返回值
- 执行几个代码片断中的一个
如果case表示是密集的，在使用switch语句的前两种情况中，可以使用效率更高的查找表。比如下面的两个实现汇编代码转换成字符串的例程：

C
```
char * Condition_String1(int condition) {
    switch(condition) {
         case 0: return "EQ";
         case 1: return "NE";
         case 2: return "CS";
         case 3: return "CC";
         case 4: return "MI";
         case 5: return "PL";
         case 6: return "VS";
         case 7: return "VC";
         case 8: return "HI";
         case 9: return "LS";
         case 10: return "GE";
         case 11: return "LT";
         case 12: return "GT";
         case 13: return "LE";
         case 14: return "";
         default: return 0;
    }
}
char * Condition_String2(int condition) {
    if((unsigned) condition >= 15) return 0;
    return
          "EQNECSCCMIPLVSVCHILSGELTGTLE" +
           3 * condition;
}
```
第一个例程需要240个字节，第二个只需要72个。
## 循环终止 / Loop termination
如果不加留意地编写循环终止条件，就可能会给程序带来明显的负担。我们应该尽量使用“倒数到零”的循环，使用简单的循环终止条件。循环终止条件相对简单，程序在执行的时候也会消耗相对少的时间。拿下面两个计算n!的例子来说，第一个例子使用递增循环，第二个使用递减循环。

C
```
int fact1_func (int n)
{
    int i, fact = 1;
    for (i = 1; i <= n; i++)
        fact *= i;
    return (fact);
}
int fact2_func(int n)
{
    int i, fact = 1;
    for (i = n; i != 0; i--)
        fact *= i;
    return (fact);
}
```
结果是，第二个例子要比第一个快得多。
## 更快的for()循环 / Faster for() loops
这是一个简单而有效的概念，通常情况下，我们习惯把for循环写成这样：

C
```
for( i = 0;  i < 10;  i++){ ... }
```
i 值依次为：0,1,2,3,4,5,6,7,8,9
在不在乎循环计数器顺序的情况下，我们可以这样：

C
```
for( i = 10;  i--; ) { ... }
```
i 值依次为: 9,8,7,6,5,4,3,2,1,0,而且循环要更快
这种方法是可行的，因为它是用更快的i–作为测试条件的，也就是说“i是否为非零数，如果是减一，然后继续”。相对于原先的代码，处理器不得不“把i减去10，结果是否为非零数，如果是，增加i，然后继续”，在紧密循环(tight loop)中，这会产生显著的区别。
这种语法看起来有一点陌生，却完全合法。循环中的第三条语句是可选的（无限循环可以写成这样for(;;)）,下面的写法也可以取得同样的效果：

C
```
for(i = 10;  i;  i--){}
```
或者:

C
```
for(i = 10;  i != 0;  i--){}
```
我们唯一要小心的地方是要记住循环需要停止在0（如果循环是从50-80，这样做就不行了），而且循环的计数器为倒计数方式。
另外，我们还可以把计数器分配到寄存器上，可以产生更为有效的代码。这种将循环计数器初始化成循环次数，然后递减到零的方法，同样适用于while和do语句。
## 混合循环/ Loop jamming
在可以使用一个循环的场合，决不要使用两个。但是如果你要在循环中进行大量的工作，超过处理器的指令缓冲区，在这种情况下，使用两个分开的循环可能会更快，因为有可能这两个循环都被完整的保存在指令缓冲区里了。

C
```
//原先的代码
for(i = 0; i < 100; i++){
    stuff();
}
for(i = 0; i < 100; i++){
    morestuff();
}        
//更好的做法
for(i = 0; i < 100; i++){
    stuff();
    morestuff();
}
```
## 函数循环 / Function Looping
调用函数的时候，在性能上就会付出一定的代价。不光要改变程序指针，还要将那些正在使用的变量压入堆栈，分配新的变量空间。为了提高程序的效率，在程序的函数结构上，有很多工作可以做。保证程序的可读性的同时，还要尽量控制程序的大小。
如果一个函数在一个循环中被频繁调用，就可以考虑将这个循环放在函数的里面，这样可以免去重复调用函数的负担，比如：

C
```
for(i = 0 ; i < 100 ; i++) 
{ 
    func(t,i); 
}
void func(int w, d) 
{ 
    lots of stuff. 
}
```
可以写成：

C
```
func(t);
void func(w) 
{ 
    for(i = 0; i < 100; i++) { 
        //lots of stuff. 
    } 
}
```
## 展开循环 / Loop unrolling
为了提高效率，可以将小的循环解开，不过这样会增加代码的尺寸。循环被拆开后，会降低循环计数器更新的次数，减少所执行的循环的分支数目。如果循环只重复几次，那它完全可以被拆解开，这样，由循环所带来的额外开销就会消失。
比如:

C
```
for(i = 0; i < 3; i++){ 
    something(i);
}
//更高效的方式：
something(0);
something(1);
something(2);
```
因为在每次的循环中，i 的值都会增加，然后检查是否有效。编译器经常会把这种简单的循环解开，前提是这些循环的次数是固定的。对于这样的循环：

C
```
for(i = 0; i <  limit; i++) { ... }
```
就不可能被拆解，因为我们不知道它循环的次数到底是多少。不过，将这种类型的循环拆解开并不是不可能的。
与简单循环相比，下面的代码的长度要长很多，然而具有高得多的效率。选择8作为分块大小，只是用来演示，任何合适的长度都是可行的。例子中，循环的成立条件每八次才被检验一次，而不是每次都要检验。如果需要处理的数组的大小是确定的，我们就可以使用数组的大小作为分块的大小（或者是能够整除数组长度的数值）。不过，分块的大小跟系统的缓存大小有关。

C
```
#include<stdio.H> 
#define BLOCKSIZE (8) 
int main(void)
{ 
    int i = 0; 
    int limit = 33;  /* could be anything */ 
    int blocklimit;
    /* The limit may not be divisible by BLOCKSIZE, 
      go as near as we can first, then tidy up.
     */ 
    blocklimit = (limit / BLOCKSIZE) * BLOCKSIZE;
    /* unroll the loop in blocks of 8 */ 
    while(i < blocklimit) { 
        printf("process(%d)\n", i); 
        printf("process(%d)\n", i+1); 
        printf("process(%d)\n", i+2); 
        printf("process(%d)\n", i+3); 
        printf("process(%d)\n", i+4); 
        printf("process(%d)\n", i+5); 
        printf("process(%d)\n", i+6); 
        printf("process(%d)\n", i+7); 
        /* update the counter */ 
        i += 8; 
    } 
    /* 
     * There may be some left to do.
     * This could be done as a simple for() loop, 
     * but a switch is faster (and more interesting) 
     */ 
    if( i < limit ) 
    { 
        /* Jump into the case at the place that will allow
         * us to finish off the appropriate number of items. 
         */ 
        switch( limit - i ) 
        { 
            case 7 : printf("process(%d)\n", i); i++; 
            case 6 : printf("process(%d)\n", i); i++; 
            case 5 : printf("process(%d)\n", i); i++; 
            case 4 : printf("process(%d)\n", i); i++; 
            case 3 : printf("process(%d)\n", i); i++; 
            case 2 : printf("process(%d)\n", i); i++; 
            case 1 : printf("process(%d)\n", i); 
        }
    } 
    return 0;
}
```
## 计算非零位的个数 / counting the number of bits set
例1：测试单个的最低位，计数，然后移位。

C
```
//example1
int countbit1(uint n)
{
    int bits = 0;
    while (n != 0) {
        if(n & 1) bits++;
            n >>= 1;
    }
      return bits;
}
```
例2：先除4，然后计算被4处的每个部分。循环拆解经常会给程序优化带来新的机会。

C
```
//example - 2
int countbit2(uint n)
{
    int bits = 0;
    while (n != 0) {
        if (n & 1) bits++;
        if (n & 2) bits++;
        if (n & 4) bits++;
        if (n & 8) bits++;
            n >>= 4;
    }
    return bits;
}
```
## 尽早地退出循环 / Early loop breaking
通常没有必要遍历整个循环。举例来说，在数组中搜索一个特定的值，我们可以在找到我们需要值之后立刻退出循环。下面的例子在10000个数字中搜索-99。

C
```
found = FALSE; 
for(i=0;i<10000;i++) 
{ 
    if(list[i] == -99) { 
         found = TRUE; 
    } 
} 
if(found) printf("Yes, there is a -99. Hooray!\n");
```
这样做是可行的，但是不管这个被搜索到的项目出现在什么位置，都会搜索整个数组。跟好的方法是，再找到我们需要的数字以后，立刻退出循环。

C
```
found = FALSE; 
for(i = 0; i < 10000; i++) 
{ 
    if( list[i] == -99 ) { 
        found = TRUE; 
        break; 
    } 
} 
if( found ) printf("Yes, there is a -99. Hooray!\n");
```
如果数字出现在位置23上，循环就会终止，忽略剩下的9977个。
## 函数设计 / Function Design
保持函数短小精悍，是对的。这可以使编译器能够跟高效地进行其他的优化，比如寄存器分配。
## 调用函数的开销 / Function call overhead
对处理器而言，调用函数的开销是很小的，通常，在被调用函数所进行的工作中，所占的比例也很小。能够使用寄存器传递的函数参数个数是有限制的。这些参数可以是整型兼容的（char,short,int以及float都占用一个字），或者是4个字以内的结构体（包括2个字的double和long long）。假如参数的限制是4，那么第5个及后面的字都会被保存到堆栈中。这会增加在调用函数是存储这些参数的，以及在被调用函数中恢复这些参数的代价。

C
```
int f1(int a, int b, int c, int d) { 
    return a + b + c + d;
}
int g1(void) {
    return f1(1, 2, 3, 4);
}
int f2(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}
ing g2(void) {
    return f2(1, 2, 3, 4, 5, 6);
}
```
g2函数中，第5、6个参数被保存在堆栈中，在f2中被恢复，每个参数带来2次内存访问。
## 最小化参数传递的开销 / Minimizing parameter passing overhead
为了将传递参数给函数的代价降至最低，我们可以：
尽可能确保函数的形参不多于四个，甚至更少，这样就不会使用堆栈来传递参数。
如果一个函数形参多于四个，那就确保在这个函数能够做大量的工作，这样就可以抵消由传递堆栈参数所付出的代价。
用指向结构体的指针作形参，而不是结构体本身。
把相关的参数放到一个结构里里面，然后把它的指针传给函数，可以减少参数的个数，增加程序的可读性。
将long类型的参数的个数降到最小，因为它使用两个参数的空间。对于double也同样适用。
避免出现参数的一部分使用寄存器传输，另一部分使用堆栈传输的情况。这种情况下参数将被全部压到堆栈里。
避免出现函数的参数个数不定的情况。这种情况下，所有参数都使用堆栈。
## 叶子函数 / Leaf functions
如果一个函数不再调用其他函数，这样的函数被称为叶子函数。在许多应用程序中，大约一半的函数调用是对叶子函数的调用。叶子函数在所有平台上都可以得到非常高效的编译，因为他们不需要进行参数的保存和恢复。在入口压栈和在出口退栈的代价，跟一个足够复杂的需要4个或者5个参数的叶子函数所完成的工作相比，是非常小的。如果可能的话，我们就要尽量安排经常被调用的函数成为叶子函数。函数被调用的次数可以通过模型工具（profiling facility）来确定。这里有几种方法可以确保函数被编译成叶子函数：
- 不调用其他函数：包括那些被转换成调用C语言库函数的运算，比如除法、浮点运算。
- 使用关键字__inline修饰小的函数。
## 内联函数 / Inline functions
对于所有调试选项，内嵌函数是被禁止的。使用inline关键字修饰函数后，跟普通的函数调用不同，代码中对该函数的调用将会被函数体本身代替。这会使代码更快，另一方面它会影响代码的长度，尤其是内嵌函数比较大而且经常被调用的情况下。

C
```
__inline int square(int x) {
    return x * x;
}
#include <MATH.H>
double length(int x, int y){
    return sqrt(square(x) + square(y));
}
```
使用内嵌函数有几个优点：
- 没有调用函数的开销。
因为函数被直接代替，没有任何额外的开销，比如存储和恢复寄存器。
- 更低的参数赋值开销。
参数传递的开销通常会更低，因为它不需要复制变量。如果其中一些参数是常量，编译器还可以作进一步的优化。
内嵌函数的缺点是如果函数在许多地方被调用，将会增加代码的长度。长度差别的大小非常依赖于内嵌函数的大小和调用的次数。
仅将少数关键函数设置成内嵌函数是明智的。如果设置得当，内嵌函数可以减少代码的长度，一次函数调用需要一定数量的指令，但是，使用优化过的内嵌函数可以编译成更少的指令。
## 使用查找表 / Using Lookup Tables
有些函数可以近似成查找表，这样可以显著的提高效率。查找表的精度一般比计算公式的精度低，不过在大多数程序中，这种精度就足够了。
许多信号处理软件（比如MODEM调制软件）会大量的使用sin和cos函数，这些函数会带来大量的数学运算。对于实时系统来说，精度不是很重要，sin/cos查找表显得更加实用。使用查找表的时候，尽量将相近的运算合并成一个查找表，这样要比使用多个查找表要更快和使用更少的空间。
## 浮点运算 / Floating-Point Arithmetic
尽管浮点运算对于任何处理器来讲都是很费时间的，有的时候，我们还是不得不用到浮点运算，比方说实现信号处理。尽管如此，编写浮点运算代码的时候，我们要牢记：
- 浮点除法是慢的
除法要比加法或者乘法慢两倍，我们可以把被一个常数除的运算写成被这个数的倒数乘（比如，x=x/3.0写成x=x*(1.0/3.0)）。倒数的计算在编译阶段就被完成。
- 使用float代替double
Float型变量消耗更少的内存和寄存器，而且因为它的低精度所以具有更高的效率。在精度足够的情况下，就要使用float。
- 不要使用先验函数（transcendental functions），
先验函数（比如sin，cos，log）是通过使用一系列的乘法和加法实现的，所以这些运算会比普通的乘法慢10倍以上。
- 简化浮点表达式
编译器在整型跟浮点型混合的运算中不会进行太多的优化。比如3 * (x / 3) 不会被优化成x，因为浮点运算通常会导致精度的降低，甚至表达式的顺序都是重要的： (a + b) 　　  + c 不等于 a + (b + c)。因此，进行手动的优化是有好处的。
不过，在特定的场合下，浮点运算的效率达不到指定的水平，这种情况下，最好的办法可能是放弃浮点运算，转而使用定点运算。当变量的变化范围足够的小，定点运算要比浮点运算精度更高、速度更快。
## 其他的技巧 / Misc tips
一般情况下，可以用存储空间换取时间。你可以缓存那些经常用到的数据，而不是每次都重新计算、或者重新装载。比如sin/cos表，或者伪随机数的表（如果你不是真的需要随机数，你可以在开始的时候计算1000个，在随后的代码中重复利用就是了）
尽量少的使用全局变量。
将一个文件内部的变量声明成静态的，除非它有必要成为全局的。
不要使用递归。递归可以使代码非常整齐和美观，但会产生大量的函数调用和开销。
访问单维数组要比多维数组快
使用#defined宏代替经常用到的小函数。
## 引用/References
- [Writing Efficient C for ARM](http://www.arm.com/pdfs/DAI0034A_efficient_c.pdf)- Document number: ARM DAI 0034A
- Issued: January 1998
- Copyright Advanced RISC Machines Ltd. (ARM) 1998
- [Richard’s C Optimization page](http://www.rddvs.com/FasterC/) OR: How to make your C, C++ or Java program run faster with little effort.
- [Code Optimization Using the GNU C Compiler](http://www.tldp.org/LDP/LG/issue71/joshi.html) By Rahul U Joshi.
- [Compile C Faster on Linux](http://www.cs.princeton.edu/software/lcc/doc/linux.html) [Christopher W. Fraser (Microsoft Research), David R. Hanson (Princeton University)]
- CODE OPTIMIZATION – COMPILER [[1](http://www.ibiblio.org/pub/languages/fortran/ch1-10.html)] [[2](http://www.ibiblio.org/pub/languages/fortran/ch1-9.html)][Thanks to Craig Burley for the excellent comments. Thanks to Timothy Prince for the note on architectures with Instruction Level Parallelism].
- [An Evolutionary Analysis of GNU C Optimizations](http://www.coyotegulch.com/acovea/) [Using Natural Selection to Investigate Software Complexities by Scott Robert Ladd. Updated: 16 December 2003]
## 其他网络资源 / Other URLs
[http://www.xs4all.nl/~ekonijn/loopy.html](http://www.xs4all.nl/~ekonijn/loopy.html)
[http://www.public.asu.edu/~sshetty/Optimizing_Code_Manual.doc](http://www.public.asu.edu/~sshetty/Optimizing_Code_Manual.doc)
[http://www.abarnett.demon.co.uk/tutorial.html](http://www.abarnett.demon.co.uk/tutorial.html)
