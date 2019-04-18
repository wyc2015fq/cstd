# C++中的cin - aiaiai010101的博客 - CSDN博客

2017年09月17日 20:05:27[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：171



在C中,输入输出要用printf和scanf,这是2个很麻烦的东西,因为在输入数据的同时还要说明数据的类型,如果输入数据较多,那就很麻烦了,所以我们的C++搞了两个更爽的东西cout和cin,来替换它们.首先我们先了解一下这两个东西的来处,它们是来自C++的一个类库叫 " iostream".

iostream是由istream(输入类)和ostream(输出类)派生.所以在iostream中就有了输入和输出的相关对象:

1,cin,表示标准输入(standard input)的istream类对象.cin使我们可以从设备读取数据.

2,cout,表示标准输出(standard output)的ostream类对象.cout使我们可以向设备输出或者写数据.

3,cerr(暂时还没试过,先别理吧)

暂时先介绍那么多,这里我主要想说的是cin.get()和cin.getline(),cin.clear(),cin.sync()等的用法.

首先看看cin.get(),它是一个读取单个字符的方法.

字符变量=cin.get();相当于cin.get(字符变量);

#include <iostream>
usingnamespace std;

int main() 

{ 
charcstr;

        cstr=cin.get();          //读取单个字符,在屏幕输入,也相当于cin.get(cstr);

        cout<<cstr<<endl;  //输出刚刚载入的单个字符

        system("pause");

}

运行程序后,一切正常:

输入:a    输出:a

但当我们输入的不只一个英文字符时,那又会如何呢?

输入:abcd         输出:a

由此可知,它只能读取第一个字符,但如果我们把程序修改成:

int main() 

{ 
charcstr;
charbstr;

        cstr=cin.get();         //读取单个字符,在屏幕输入
bstr=cin.get();  

        cout<<cstr<<bstr<<endl;  //输出刚刚载入的单个字符

        system("pause");

}

我们再输入:abcd    最后输出了:ab

既然cin.get()是读取第一个字符,那bstr为什么不也是a呢?

其实原理是这样的:

在cin这个对象里,有一个储存字符的流,可以想象成缓冲区,但事实上是cin里封装的一个东西.当我们在程序上输入字符后,对象cin获得了我们输入的字符,例如获得abcd,然后再通过.get()把流里面的第一个字符去掉,赋给cstr,这时,cin里储存的流的数据为bcd,而cstr则获得了 a.当我们再次运行bstr=cin.get();时,同理把cin里流的数据的b拿出来给了bstr,此后,cin里面的流的数据为cd,而bstr则为b,所以最后输出时,便能输出ab了.

还有个补充,究竟什么时候才输入数据呢?我们可以再通过上面的代码进行尝试,我们输入单个字母'a',然后按回车,发现并没有输出数据,而是再等待一次输入数据,我们再输入字母'b',按回车后便输出ab了.相信到这里,大家都应该明白了,因为当我们第一次输入a后,通过cstr=cin.get();使 cin里的流没有数据,清空了.所以到第二次要再赋给bstr值时,它找不到数据,要重新再输入数据.由此来看可以知道,当cin里的流数据清空时,便需要重新输入才能赋值.而cin.get()还有个用法:

int main() 

{ 
charcstr;
charbstr;

        cstr=cin.get();         //读取单个字符,在屏幕输入
cin.get();
bstr=cin.get();  

        cout<<cstr<<bstr<<endl;  //输出刚刚载入的单个字符

        system("pause");

}

程序中有3个cin.get(),所以我们尝试输入:abc.   发现输出了:ac

由此能知道,当空回调cin.get();时,cin.get便自动在cin中的流数据中删除一个字母,起了一个删除作用.

对cin.get()有了一定了解之后,对cin.getline()的学习就可以更快了,原理是一致的,但是cin.getline()则是获取一整行文本.以下是cin.getline()原形:

getline(char *line,int size,char='/n')

第一个就是字符指针,第二个是字符长度,第三个1行的结束标识符.

int main() 

{ 
charcstr[200];
cstr.getline(cstr,sizeof(str));    //第三个参数不输入,默认回车为结束标识符

        cout<<cstr<<endl;                 //输出

        system("pause");

}

**本人注释：原文如此,上述代码有问题,很明显应该是cin.getline(cstr,sizeof(cstr));**

这样我们输入一堆英文或数字,然后按回车,就会输出一行刚刚输出的东西了.接下来.我们讨论第三个参数的作用.

int main() 

{ 
charcstr[200];
cstr.getline(cstr,sizeof(str),'X');    //我们以单个英文字母'X'作为终止标识符

        cout<<cstr<<endl;                      //输出

        system("pause");

}

当我们输入一大堆东西,例如

输入:  kkkkkkk(回车)              输出: kkkkkkk(回车)                        

           bbbbbbb(回车)                       bbbbbbb(回车)       

           lllllX                                         lllll

这样X便成了终止符,其原理和cin.get一样.或许我们可以像cin.get那样尝试一下:

int main() 

{ 
charcstr[200];
charbstr[200];
cstr.getline(cstr,sizeof(str),'X');    //我们以单个英文字母'X'作为终止标识符
bstr.getline(bstr,sizeof(btr),'a');

        cout<<"第一行是:"<<cstr<<endl;                      //输出
        cout<<"第二行是:"<<bstr<<endl;  

        system("pause");

}

我们输入:kkkkkkkkk(回车)                                        输出:第一行是:kkkkkkkkk(回车) 

                 oooooooooX(回车)                                                              ooooooooo(回车)

                 bbbbbbbbba(回车)                                               第二行是:(回车)

                                                                                                               bbbbbbbbb

在这里,我在不厌其烦地说一下原理,如果刚刚cin.get()原理看懂的可以跳过.

首先,我们第一次getline会把X前面的字符赋给cstr,然后从cin里的数据流删除,标识符X也删除了,所以输出的cstr如上所示.当我们第二次运行getline时,此时cin里的数据流为(回车)bbbbbbbbba,回车也是一个字符,事实上在数据流里用"/n"表示,接着就按照原来第一次的方法,把标识符'a'前面的字符赋给bstr,然后再删除字符号及标识符.所以输出结果如上.


接下来我们谈谈cin.clear的作用,第一次看到这东西,很多人以为就是清空cin里面的数据流,而实际上却与此相差很远,首先我们看看以下代码:

#include <iostream>
usingnamespace std;

int main() 

{
int a;

        cin>>a;

        cout<<cin.rdstate()<<endl;
if(cin.rdstate() == ios::goodbit)

  {

  cout<<"输入数据的类型正确，无错误！"<<endl;

              }
if(cin.rdstate() == ios_base::failbit)

        {

                cout<<"输入数据类型错误，非致命错误，可清除输入缓冲区挽回！"<<endl;

        }

        system("pause");

}

我们定义要输入到的变量是整型,但如果我们输入了英文字母或者汉字,那就会发生错误,cin里有个方法能检测这个错误,就是cin.rdstate();

当cin.rdstate()返回0(即ios::goodbit)时表示无错误,可以继续输入或者操作,若返回4则发生非致命错误即ios::failbit,则不能继续输入或操作.而cin.clear则可以控制我们此时cin里对这个问题的一个标识.语发如下:

cin.clear(标识符);

标识符号为:
- goodbit 无错误
- Eofbit 已到达文件尾
- failbit 非致命的输入/输出错误，可挽回
- badbit　致命的输入/输出错误,无法挽回

若在输入输出类里.需要加ios::标识符号 

通过cin.clear,我们能确认它的内部标识符,如果输入错误则能重新输入.结合真正的清空数据流方法cin.sync(),请看下例:

#include <iostream>
usingnamespace std;

int main() 

{
int a;
while(1)

        {

                cin>>a;
if(!cin)            
//条件可改写为cin.fail()

                {

                        cout<<"输入有错!请重新输入"<<endl;

                        cin.clear(); 

                        cin.sync();   //清空流

                }
else

                {

                        cout<<a;
break;

                }

        }

        system("pause");

}

上面的cin.clear()默认参数为0,即无错误,正常操作.当我们输入英文字母'k'时,它的状态标识改为fail,即错误,用cout对用户输出信息,再用cin.clear让错误标识改回为0,让我们可以继续输入,再清空流数据继续输入.如果我们没有了cin.clear,则会进入死循环,其过程为我们输入了英文字母,它的状态标识便为fail,当运行到条件判断时,便总是回到错误的条件表示里,并且我们再也没办法输入,因为错误的表示关闭了
 cin,所以会进入死循环.

//--------------------------------------------------------------------------------------------------------------------------------------------------

**本人注释:关于以下部分可以先看看http://www.cnblogs.com/randyma/p/7131818.html**

在前一节中我们有几个例子中提到了cin函数出错，以致不再执行读操作(程序8)。而且我们经常会看到程序中会出现cin.clear(),cin.ignore(), cin.fail()等函数。这些函数都是与cin的错误处理有关的。这一节我们来分析一下cin的错误处理机制，并且学习几个重要的函数：cin.fail(), cin.bad(), cin.good(),
 cin.clear(), cin.ignore()等。

程序执行时有一个标志变量来标志输入的异常状态，其中有三位标志位分别用来标志三种异常信息，他们分别是：failbit，eofbit，badbit。这三个标志位在标志变量中是这样分配的：

____________________________________

|     2     |     1    |     0     |

| failbit | eofbit |   badbit |

|___________|__________|___________|

看一下这几个标志位的作用(引用msdn)：

badbit, to record a loss of integrity of the stream buffer.

eofbit, to record end-of-file while extracting from a stream.

failbit, to record a failure to extract a valid field from a stream.

In addition, a useful value is goodbit, where no bits are set.

接下来我么看几个ios类的数据定义(引用msdn)：

typedef T2 iostate; 

static const iostate badbit, eofbit, failbit, goodbit;

这里ios类定义了这四个常量badbit, eofbit, failbit, goodbit，其实这四个标志常量就是取对应标志位的掩码，也即输入的四种异常情况！

以上四个常量对应的取值为：

ios::badbit    001   输入（输出）流出现致命错误，不可挽回 

ios::eofbit    010   已经到达文件尾

ios::failbit   100   输入（输出）流出现非致命错误，可挽回 

ios::goodbit   000   流状态完全正常, 各异常标志位都为0

我们可以用输出语句来验证这几个常量的值：

cout << ios:: failbit << endl;

cout << ios:: eofbit << endl;

cout << ios:: badbit << endl; 

cout << ios:: goodbit << endl;

输出的结果为：

4

2

1

0

【注意】它们不是failbit、badbit、eofbit、goodbit这四个标记位的存贮变量，而是四个标志四种异常状态的常量，其实他们就相当于取对应状态标志位的掩码。如果标志变量为flag，则flag & failbit 就取得fail标志位。

**本人注释;上边这段说的很好,这是4个标志常量,用来取对应位置的状态的.**

搞清楚了标志位的原理后，我们来看几个关于异常标志的函数：

1、iostate ios::rdstate()

取标志变量的值，我们可以用该函数取得整个标志变量的值，再与前面定义的标志位常量相与就可以获得对应标志位的状态。如：

void TestFlags( ios& x ) // 获得x流的三个标志位状态

{

cout << ( x.rdstate( ) & ios::badbit ) << endl;

cout << ( x.rdstate( ) & ios::failbit ) << endl;

cout << ( x.rdstate( ) & ios::eofbit ) << endl;

cout << endl;

}

2、bool ios::fail() const;

1 or true if rdstate & failbit is nonzero, otherwise 0 or false. (引用msdn)

其中rdstate即通过rdstate()取得的标识变量的值，与failbit相与，即取得failbit标志位的值，如果结果非零则放回true，否则返回false。即该函数返回failbit的状态，将标志位状态通过bool值返回。

3、bool ios::bad() const;

1 or true if rdstate & badbit is nonzero; otherwise 0. (引用msdn)

与fail()相似。

4、bool ios::good() const;

1 or true if rdstate == goodbit (no state flags are set), otherwise, 0 or false. (引用msdn)

改函数取goodbit的情况，即三个标志位都0(即没有任何异常情况)时返回true，否则返回false。

5、void ios::clear(iostate _State=goodbit);

clear ( rdstate() | state ) = setstate(state) ；
该函数用来重置标识变量，_State是用来重置的值，默认为goodbit，即默认时将所有标志位清零。用户也可以传进参数，如：clear(failbit)，这样就将标识变量置为failbit(即：001)。

我们一般是用它的默认值，当cin出现异常，我们用该函数将所有标志位重置。如果cin出现异常，没有重置标志的话没法执行下一次的cin操作。如上一节的程序2的测试二为什么第二次输入操作没有执行？程序8中 cin>>ch 为什么没有执行？都是这个原因！！！

所以经常在程序中使用 cin.clear(), 为了重置错误标志！

6、另外还有一个函数 void ios::setstate(iostate _State);

这个函数也是用来设置标识变量的，但与clear()不同。clear()是将所有标志清零，在置以参数新的标志。而该函数不清零其他的标志，而只是将参数对应的标志位置位。这个函数不是经常使用，这里不再赘述。

在搞清楚了这几个函数后，对cin输入操作的错误处理就有了比较深的了解了。下面我们回过头来看看上一节程序8的测试，因为第一次用getline()读取字符串超长，所以导致出现异常，大家可以查看一下标志位来验证一下！所以会导致后面的 cin>>ch 语句没有执行。那我们利用前面学习的clear()函数来强制重置错误标志，看看会出现什么情况呢？
**程序9：**#include <iostream>

using namespace std;

int main ()

{

char ch, str[20];

cin.getline(str, 5);

cout<<"flag1:"<<cin.good()<<endl;    // 查看goodbit状态，即是否有异常

cin.clear();                         // 清除错误标志

cout<<"flag1:"<<cin.good()<<endl;    // 清除标志后再查看异常状态

cin>>ch; 

cout<<"str:"<<str<<endl;

cout<<"ch :"<<ch<<endl;

return 0;

}
测试输入：

12345[Enter]
输出：

flag1:0 // good()返回false说明有异常

flag2:1 // good()返回true说明，clear()已经清除了错误标志

str:1234

ch :5

【分析】程序执行结束还是只执行了一次读操作，cin>>ch还是没有从键盘读取数据，但是与程序8中不同，这里打印了ch的值为'5'，而且在cin>>ch之前已经清楚了错误标志，也就是cin>>ch的读操作实际上执行了。这就是前面讲的cin读取数据的原理：它是直接从输入缓冲区中取数据的。此例中，第一次输入"12345", 而getline(str, 5)根据参数'5'只取缓冲区中的前4个字符，所以str取的是"1234"，而字符'5'仍在缓冲区中，所以cin>>ch直接从缓冲区中取得数据，没有从键盘读取数据！

也就是当前一次读取数据出错后，如果缓冲区没有清空的话，重置错误标志还不够！要是能将缓冲区的残留数据清空了就好了哦！下面我们再来看一个很重要的函数！

7、basic_istream& ignore(streamsize _Count = 1, int_type _Delim = traits_type::eof());

function: Causes a number of elements to be skipped from the current read position.

Parameters:

_Count, The number of elements to skip from the current read position.

_Delim, The element that, if encountered before count, causes ignore to return and allowing all elements after _Delim to be read. (引用msdn)

这个函数用来丢弃输入缓冲区中的字符，第一参数定义一个数，第二个参数定义一个字符变量。下面解释一下函数是怎样执行的：**函数不停的从缓冲区中取一个字符，并判断是不是_Delim，如果不是则丢弃并进行计数，当计数达到_Count退出，如果是则丢弃字符退出。**例：cin.ignore(5, 'a'); 函数将不断从缓冲区中取一个字符丢弃，直到丢弃的字符数达到5或者读取的字符为'a'。下面我们看个程序例子：
**程序10：**

#include <iostream>

using namespace std;

int main ()

{

cin.ignore(5, 'a');

return 0;

}
测试一输入：
c[enter]

c[enter]

c[enter]

c[enter]

c[enter]

程序结束。

【分析】程序开始时缓冲区是空的，cin.ignore()到缓冲区中取数据，没有则请求从键盘输入，每次从键盘输入一个字符，如果不是'a'则丢弃，所以该测试中共输入了5次，直到计数达到5。

**本人注释:以上这段是错的,实际上输入3个c就结束了,因为回车也算一个字符,这里设置的结束符是'a',所以遇到回车会丢弃并计数加一.这样输入到第3个c实际上就结束了.**

测试二输入：

c[enter]

c[enter]

a[enter]

程序结束。

【分析】前面两个字符不是'a'丢弃且计数没达到5，第三次输入为'a', 丢弃该字符程序结束！

**丢弃一个字符：**

我们看看这个函数的默认值，第一个参数默认为1，第二个参数默认为EOF。所以cin.ignore()就是丢弃缓冲区中的第一个字符，这在程序中也是比较常用的！我们回过头看看程序5，程序5中用cin.get()读取字符，第一次读取时用回车符结束，而get函数不丢弃回车符，所以回车符仍残留在缓冲区中，导致第二次读取数据直接从缓冲区中取得回车符！这与我们最初的用以是不相符的，既然cin.get()不会自动丢弃输入结束时的回车符，这里我们学会了ignore()函数，我们就可以自己手动求其回车符啊！所以程序5可以这样改动：

程序11：

#include <iostream>

using namespace std;

int main()

{

char c1, c2;

cin.get(c1);

        cin.ignore(); // 用该函数的默认情况，丢弃一个字符，即上次输入结束的回车符

cin.get(c2);

cout<<c1<<" "<<c2<<endl;   // 打印两个字符

cout<<(int)c1<<" "<<(int)c2<<endl; // 打印这两个字符的ASCII值

return 0; 

}

测试一输入：

a[Enter]

b[Enter]

输出:

a

b

97 98

【分析】这样程序就正常了！

**清空整个缓冲区：**

其实该函数最常用的方式是这样的，将第一个参数设的非常大，将第二个参数设为'/n'，这样就可以缓冲区中回车符中的所有残留数据，因为一般情况下前面输入残留的数据是没有用的，所以在进行新一次输入操作前将缓冲区中所有数据清空是比较合理。

如：cin.ignore(1024, '/n'); 

或者：cin.ignore(std::numeric_limits<std::streamsize>::max(), '/n'); 


