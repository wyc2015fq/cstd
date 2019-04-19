# C++学习：list容器详解 - 数据之美的博客 - CSDN博客
2017年08月22日 13:42:16[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：137
list容器详解
      首先说说STL
      STL就是Standard Template Library，标准模板库。这可能是一个历史上最令人兴奋的工具的最无聊的术语。从根本上说，STL是一些“容器”的集合，这些“容器”有list,vector,set,map等，STL也是[算法](http://lib.csdn.net/base/datastructure)和其他一些组件的集合。这里的“容器”和算法的集合指的是世界上很多聪明人很多年的杰作。
       STL的目的是标准化组件，这样就不用重新开发，可以使用现成的组件。STL现在是C++的一部分，因此不用额外安装什么。**它被内建在你的编译器之内**。因为STL的list是一个简单的容器，所以我打算从它开始介绍STL如何使用。如果你懂得了这个概念，其他的就都没有问题了。另外，list容器是相当简单的，我们会看到这一点。
STL容器可以保存对象，内建对象和类对象。它们会安全的保存对象，并定义我们能够操作的这个对象的接口。放在蛋架上的鸡蛋不会滚到桌上。它们很安全。因此，在STL容器中的对象也很安全。
STL算法是标准算法，我们可以把它们应用在那些容器中的对象上。这些算法都有很著名的执行特性。它们可以给对象排序，删除它们，给它们记数，比较，找出特殊的对象，把它们合并到另一个容器中，以及执行其他有用的操作。
STL iterator就象是容器中**指向对象的指针**。STL的算法使用iterator在容器上进行操作。Iterator设置算法的边界 ，容器的长度，和其他一些事情。举个例子，有些iterator仅让算法读元素，有一些让算法写元素，有一些则两者都行。
 Iterator也决定在容器中处理的方向。
       你可以通过调用容器的成员函数begin()来得到一个指向一个**容器起始位置**的iterator。你可以调用一个容器的 end() 函数来得到过去的最后一个值（就是处理停在那的那个值）。
       这就是STL所有的东西，容器、算法、和允许算法工作在容器中的元素上的iterator。 算法以合适、标准的方法操作对象，并可通过iterator得到容器精确的长度。一旦做了这些，它们就在也不会“跑出边界”。 还有一些其他的对这些核心组件类型有功能性增强的组件，例如函数对象。我们将会看到有关这些的例子，现在 ，我们先来看一看STL的list。
       1. 定义一个list
#include ＜string＞
#include ＜list＞
int main (void) 
{
　list＜string＞ Milkshakes;
　return 0;
}
      这就行了，你已经定义了一个list。简单吗？list＜string＞ Milkshakes这句是你声明了list＜string＞模板类的一个实例，然后就是实例化该类的一个对象。但是我们别急着做这个。在这一步其实你只需要知道你**定义了一个字符串类型的list**。你需要包含提供STL
 list类的头文件#include ＜list＞，注意iostream.h这个头文件已经被STL的头文件放弃了，所以可以不用这个头文件。
      注：在[Linux](http://lib.csdn.net/base/linux)中可以用 **g++ **test1.cpp
 -o test1 来编译这个[测试](http://lib.csdn.net/base/softwaretest)程序。
      2. 使用list的成员函数push_back和push_front插入一个元素到list中：
      现在我们有了一个list容器，我们可以使用它来装东西了。我们将把一个字符串加到这个list里。有一个非常重要的东西叫做list的值类型。值类型就是list中的对象的类型。在这个例子中,这个list的值类型就是字符串,string
 ，这是因为这个list用来放字符串。
#include ＜string＞
#include ＜list＞
int main (void) 
{
　list＜string＞ Milkshakes; //声明了list＜string＞模板类的一个实例
　Milkshakes.push_back("Chocolate"); //实例化
　Milkshakes.push_back("Strawberry");
　Milkshakes.push_front("Lime");
　Milkshakes.push_front("Vanilla");
　return 0;
}
        我们现在有个4个字符串在list中。list的成员函数push_back()把一个对象放到一个list的后面，而 push_front()把对象放到前面。我通常把一些错误信息push_back()到一个list中去，然后push_front()一个标题到list中， 这样它就会在这个错误消息以前打印它了。
      3. list的成员函数empty()
 知道一个list是否为空很重要。如果list为空，empty()这个成员函数返回真。 我通常会这样使用它。通篇程序我都用push_back()来把错误消息放到list中去。然后，通过调用empty() 我就可以说出这个程序是否报告了错误。如果我定义了一个list来放信息，一个放警告，一个放严重错误，
 我就可以通过使用empty()轻易的说出到底有那种类型的错误发生了。 
      我可以整理这些list，然后在打印它们之前，用标题来整理它们，或者把它们排序成类。
/**************************************************************
      //    Using a list to track and report program messages and status 
#include ＜iostream.h＞
#include ＜string＞
#include ＜list＞
int main (void) 
{
#define OK 0 
#define INFO 1
#define WARNING 2
　int return_code;
list＜string＞ InfoMessages;
　list＜string＞ WarningMessages;
　// during a program these messages are loaded at various points
　InfoMessages.push_back("Info: Program started");
　// do work...
　WarningMessages.push_back("Warning: No Customer records have been found");
　// do work...
　return_code = OK; 
if (!InfoMessages.empty()) {
　　// there were info messages
　　InfoMessages.push_front("Informational Messages:");
　　// ... print the info messages list, we'll see how later
　　return_code = INFO;
　}
　if (!WarningMessages.empty()) {
　　// there were warning messages
　　WarningMessages.push_front("Warning Messages:");
　　// ... print the warning messages list, we'll see how later
　　return_code = WARNING; 
　}
　// If there were no messages say so.
　if (InfoMessages.empty() && WarningMessages.empty()) {
　　cout ＜＜ "There were no messages " ＜＜ endl;
　}
　return return_code;
}
******************************************************************/
     4.**用for循环来处理list中的元素**
我们想要遍历一个list，比如打印一个list中的所有对象来看看list上不同操作的结果。要一个元素一个元素的遍历一个list， 我们可以这样做：
这个程序定义了一个iterator（类似指针），MilkshakeIterator。我们把它指向了这个list的第一个元素。 这可以调用Milkshakes.begin()来做到，它会返回一个指向list开头的iterator。然后我们把它和Milkshakes.end()的
 返回值来做比较，当我们到了那儿的时候就停下来。 
容器的end()函数会返回一个指向容器的最后一个位置的iterator。当我们到了那里，就停止操作。我们不能不理容器的end()函数的返回值。我们仅知道它意味着已经处理到了这个容器的末尾，应该停止处理了。 所有的STL容器都要这样做。
　　在上面的例子中，每一次执行for循环，我们就重复引用iterator来得到我们打印的字符串。
在STL编程中，我们在每个算法中都使用一个或多个iterator。我们使用它们来**存取容器中的对象**。 要存取一个给定的对象，我们把一个iterator指向它，然后间接引用这个iterator。 
　　这个list容器，就象你所想的，它不支持在iterator加一个数来指向隔一个的对象。 就是说，我们不能用Milkshakes.begin()+2来指向list中的第三个对象，因为STL的list是以双链的list来实现的， 它不支持随机存取。vector和deque(向量和双端队列)和一些其他的STL的容器可以支持随机存取。 
　　上面的程序打印出了list中的内容。任何人读了它都能马上明白它是怎麽工作的。它使用标准的iterator和标准 的list容器。没有多少程序员依赖它里面装的东西， 仅仅是标准的C++。这是一个向前的重要步骤。这个例子使用STL使我们的软件更加标准。 

/*
|| How to print the contents of a simple STL list. Whew! 
*/
#include ＜iostream.h＞
#include ＜string＞
#include ＜list＞
int main (void) 
{
　list＜string＞ Milkshakes;
list＜string＞::iterator MilkshakeIterator;//为Milkshakes实例容器定义一个iterator指针：MilkshakeIterator
　Milkshakes.push_back("Chocolate");
　Milkshakes.push_back("Strawberry");
　Milkshakes.push_front("Lime");
　Milkshakes.push_front("Vanilla");
　// print the milkshakes打印
　Milkshakes.push_front("The Milkshake Menu");
　Milkshakes.push_back("*** Thats the end ***");
for (MilkshakeIterator=Milkshakes.begin(); MilkshakeIterator!=Milkshakes.end(); ++MilkshakeIterator) 
　{
　　// dereference the iterator to get the element
　　cout ＜＜ *MilkshakeIterator ＜＜ endl; //输出
　} 
}
 接C++学习：list容器详解（一）
**用STL的通用[算法](http://lib.csdn.net/base/datastructure)for_each来处理list中的元素**　　使用STL list和 iterator，我们要初始化、比较和给iterator增量来遍历这个容器。STL通用的for_each 算法能够减轻我们的工作。 
/*
|| How to print a simple STL list MkII
*/
#include ＜iostream.h＞
#include ＜string＞
#include ＜list＞
#include ＜algorithm＞
PrintIt (string& StringToPrint) {
　cout ＜＜ StringToPrint ＜＜ endl;
}
int main (void) {
　list＜string＞ FruitAndVegetables;
　FruitAndVegetables.push_back("carrot");
　FruitAndVegetables.push_back("pumpkin");
　FruitAndVegetables.push_back("potato");
　FruitAndVegetables.push_front("apple");
　FruitAndVegetables.push_front("pineapple");
　for_each (FruitAndVegetables.begin(), FruitAndVegetables.end(), PrintIt);
}
　　在这个程序中我们使用STL的通用算法for_each()来遍历一个iterator的范围，然后调用PrintIt()来处理每个对象。 我们不需要初始化、比较和给iterator增量。for_each()为我们漂亮的完成了这些工作。我们执行于对象上的 操作被很好的打包在这个函数以外了，我们不用再做那样的循环了，我们的代码更加清晰了。 
　　for_each算法引用了iterator范围的概念，这是一个由起始iterator和一个末尾iterator指出的范围。 起始iterator指出操作由哪里开始，末尾iterator指明到哪结束，但是它不包括在这个范围内。   
**用STL的通用算法count()来统计list中的元素个数**　　STL的通用算法count()和count_it()用来给容器中的对象记数。就象for_each()一样，count()和count_if() 算法也是在iterator范围内来做的。 
　　让我们在一个学生测验成绩的list中来数一数满分的个数。这是一个整型的List。 
/*
|| How to count objects in an STL list
*/
#include ＜list＞
#include ＜algorithm＞
#
int main (void) 
{
　list＜int＞ Scores;
　#
　Scores.push_back(100); Scores.push_back(80);
　Scores.push_back(45); Scores.push_back(75);
　Scores.push_back(99); Scores.push_back(100);
　#
　int NumberOf100Scores(0); 
　count (Scores.begin(), Scores.end(), 100, NumberOf100Scores);
　#
　cout ＜＜ "There were " ＜＜ NumberOf100Scores ＜＜ " scores of 100" ＜＜ endl;
}
　　count()算法统计等于某个值的对象的个数。上面的例子它检查list中的每个整型对象是不是100。每次容器中的对象等于100，它就给NumberOf100Scores加1。这是程序的输出： 
There were 2 scores of 100
**用STL的通用算法count_if()来统计list中的元素个数**
　　count_if()是count()的一个更有趣的版本。他采用了STL的一个新组件，函数对象。count_if() 带一个函数对象的参数。函数对象是一个至少带有一个operator()方法的类。有些STL算法作为参数接收 函数对象并调用这个函数对象的operator()方法。 
　　函数对象被约定为STL算法调用operator时返回true或false。它们根据这个来判定这个函数。举个例子会 说的更清楚些。count_if()通过传递一个函数对象来作出比count()更加复杂的评估以确定一个对象是否应该被 记数。在这个例子里我们将数一数牙刷的销售数量。我们将提交包含四个字符的销售码和产品说明的销售记录。 
/*
|| Using a function object to help count things
*/
#include ＜string＞
#include ＜list＞
#include ＜algorithm＞
const string ToothbrushCode("0003");
class IsAToothbrush 
{
　public: 
　　bool operator() ( string& SalesRecord ) 
　　{
　　　return SalesRecord.substr(0,4)==ToothbrushCode;
　　} 
};
int main (void) 
{
　list＜string＞ SalesRecords;
　SalesRecords.push_back("0001 Soap");
　SalesRecords.push_back("0002 Shampoo");
　SalesRecords.push_back("0003 Toothbrush");
　SalesRecords.push_back("0004 Toothpaste");
　SalesRecords.push_back("0003 Toothbrush");
　int NumberOfToothbrushes(0); 
　count_if (SalesRecords.begin(), SalesRecords.end(), 
　IsAToothbrush(), NumberOfToothbrushes);
　cout ＜＜ "There were " 
　＜＜ NumberOfToothbrushes 
　＜＜ " toothbrushes sold" ＜＜ endl;
}
　　这是这个程序的输出： 
　　There were 2 toothbrushes sold 这个程序是这样工作的：定义一个函数对象类IsAToothbrush，这个类的对象能判断出卖出的是否是牙刷 。如果这个记录是卖出牙刷的记录的话，函数调用operator()返回一个true，否则返回false。 
　　count_if()算法由第一和第二两个iterator参数指出的范围来处理容器对象。它将对每个 IsAToothbrush()返回true的容器中的对象增加NumberOfToothbrushes的值。 
　　最后的结果是NumberOfToothbrushes这个变量保存了产品代码域为"0003"的记录的个数，也就是牙刷的个数。 
　　注意count_if()的第三个参数IsAToothbrush()，它是由它的构造函数临时构造的一个对象。你可以把IsAToothbrush类的一个临时对象 传递给count_if()函数。count_if()将对该容器的每个对象调用这个函数。 
**使用count_if()的一个更加复杂的函数对象**　　我们可以更进一步的研究一下函数对象。假设我们需要传递更多的信息给一个函数对象。我们不能通过 调用operator来作到这点，因为必须定义为一个list的中的对象的类型。 然而我们通过为IsAToothbrush指出一个非缺省的构造函数就可以用任何我们所需要的信息来初始化它了。 例如，我们可能需要每个牙刷有一个不定的代码。我们可以把这个信息加到下面的函数对象中： 
/*
|| Using a more complex function object
*/
#include ＜iostream.h＞
#include ＜string＞
#include ＜list＞
#include ＜algorithm＞
class IsAToothbrush 
{
　public:
　　IsAToothbrush(string& InToothbrushCode) : 
　　ToothbrushCode(InToothbrushCode) {}
　　bool operator() (string& SalesRecord) 
　　{
　　　return SalesRecord.substr(0,4)==ToothbrushCode;
　　} 
　private:
　　string ToothbrushCode; 
};
int main (void) 
{
　list＜string＞ SalesRecords;
　SalesRecords.push_back("0001 Soap");
　SalesRecords.push_back("0002 Shampoo");
　SalesRecords.push_back("0003 Toothbrush");
　SalesRecords.push_back("0004 Toothpaste");
　SalesRecords.push_back("0003 Toothbrush");
　string VariableToothbrushCode("0003");
　int NumberOfToothbrushes(0); 
　count_if (SalesRecords.begin(), SalesRecords.end(), 
　IsAToothbrush(VariableToothbrushCode),
　NumberOfToothbrushes);
　cout ＜＜ "There were "
　＜＜ NumberOfToothbrushes 
　＜＜ " toothbrushes matching code "
　＜＜ VariableToothbrushCode
　＜＜ " sold" 
　＜＜ endl;
}
　　程序的输出是： 
　　There were 2 toothbrushes matching code 0003 sold 这个例子演示了如何向函数对象传递信息。你可以定义任意你想要的构造函数，你可以再函数对象中做任何你 想做的处理，都可以合法编译通过。 
　　你可以看到函数对象真的扩展了基本记数算法。 
　　到现在为止，我们都学习了： 
　　　·定义一个list 
　　　·向list中加入元素 
　　　·如何知道list是否为空 
　　　·如何使用for循环来遍历一个list 
　　　·如何使用STL的通用算法for_each来遍历list 
　　　·list成员函数begin() 和 end() 以及它们的意义 
　　　·iterator范围的概念和一个范围的最后一个位置实际上并不被处理这一事实 
　　　·如何使用STL通用算法count()和count_if()来对一个list中的对象记数 
　　　·如何定义一个函数对象 
　　我选用这些例子来演示list的一般操作。如果你懂了这些基本原理，你就可以毫无疑问的使用STL了 建议你作一些练习。我们现在用一些更加复杂的操作来扩展我们的知识，包括list成员函数和STL通用算法。
[](http://blog.csdn.net/mazidao2008/article/details/4802617#)[](http://blog.csdn.net/mazidao2008/article/details/4802617#)[](http://blog.csdn.net/mazidao2008/article/details/4802617#)
