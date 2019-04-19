# Interpreter 模式详解--设计模式（23） - fanyun的博客 - CSDN博客
2016年07月09日 08:45:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3363
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Interpreter 模式的来源：**
     Interpreter(解释器)模式是一种特殊的[设计模式](http://baike.baidu.com/view/66964.htm)，它建立一个解释器（Interpreter），对于特定的计算机程序设计语言，用来解释预先定义的文法。简单地说，Interpreter模式是一种简单的语法解释器构架。解释器模式属于行为模式，给定一个语言，定义它的文法的一种表示，并定义一个解释器，这个解释器使用该表示来解释语言中的句子。
**Interpreter 模式作用：**
正如其名，此模式大多用来解释一些(自定义的)独特语法，例如某些游戏开发引擎中读取XML文件，或是WindowsPhone开发中的XAML文件，都是使用此模式来进行的。与其说是一种模式，不如说是一种具有通用规范的行为更为准确。
**Interpreter 模式UML结构图如图1所示：**
![](https://img-blog.csdn.net/20160708163955393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Interpreter 模式构成：**
(1).抽象表达式角色(AbstractExpression)： 声明一个抽象的解释操作，这个接口为所有具体表达式角色都要实现的。
(2).终结符表达式角色(TerminalExpression)： 实现与文法中的元素相关联的解释操作，通常一个解释器模式中只有一个终结符表达式，但有多个实例对应不同的终结符，
(3).终结符就是语言中用到的基本元素：一般不能再被分解，如: x -> xa， 这里a是终结符，因为没有别的规则可以把a变成别的符号，不过x可以变成别的符号，所以x是非终结符。
(4).非终结符表达式角色(NonterminalExpression)： 文法中的每条规则对应于一个非终结表达式， 非终结表达式根据逻辑的复杂程度而增加，原则上每个文法规则都对应一个非终结符表达式。
(5).环境角色(Context)：包含解释器之外的一些全局信息。
**Interpreter ****模式代码示例：**
Interpreter.h
```cpp
#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_
#include<string>
typedef std::string Context ;
class AbstractExpression
{
public:
	AbstractExpression(){}
	~AbstractExpression(){}
	virtual void Interpreter(const Context& context)=0;
};
class TerminalExpression : public AbstractExpression
{
public:
	TerminalExpression(char kc);
	~TerminalExpression(){}
	virtual void Interpreter(const Context& context);
private:
	char keyChar;
};
class NonterminalExpression : public AbstractExpression
{
public:
	NonterminalExpression(AbstractExpression* ae);
	~NonterminalExpression(){}
	virtual void Interpreter(const Context& context);
private:
	AbstractExpression* terminalExpression;
};
#endif
```
Interpreter.cpp
```cpp
#include "Interpreter.h"
#include<iostream>
TerminalExpression::TerminalExpression(char kc)
{
	keyChar = kc;
}
void TerminalExpression::Interpreter(const Context& context)
{
	int length = 0;
	while (context[length] != '\0')
	{
		length++;
	}
	int numberArray[100];
	int index = 0;
	for(int i = 0;i < length;i++)
	{
		if(keyChar  == context[i])
			numberArray[index++] = i;
	}
	for(int i = 0;i < index;i++)
	{
		std::cout<<numberArray[i];
	}
	std::cout<<std::endl;
}
NonterminalExpression::NonterminalExpression(AbstractExpression* ae)
{
	terminalExpression = ae;
}
void NonterminalExpression::Interpreter(const Context& context)
{
	std::cout<<"The password is: ";
	terminalExpression->Interpreter(context);
}
```
Main.cpp
```cpp
#include "Interpreter.h"
#include<iostream>
using namespace std;
int main()
{
	AbstractExpression* terminal = new TerminalExpression('c');
	AbstractExpression* nonTerminal = new NonterminalExpression(terminal);
	//First test.
	string contextFirst = "character of the cars in this chart.";
	cout<<"Context: "<<contextFirst<<endl;
	nonTerminal->Interpreter(contextFirst);
	cout<<endl;
	//Second test.
	terminal = new TerminalExpression('o');
	nonTerminal = new NonterminalExpression(terminal);
	string contextSecond = "Thanks everyone for your long time supporting.";
	cout<<"Context: "<<contextSecond<<endl;
	nonTerminal->Interpreter(contextSecond);
	return 0;
}
```
Interpreter 模式适用性：
(1).可以将一个需要解释执行的语言中的句子表示为一个抽象语法树。
(2). 一些重复出现的问题可以用一种简单的语言来进行表达。
(3).一个语言的文法较为简单。
(4).执行效率不是关键问题。（注：高效的解释器通常不是通过直接解释抽象语法树来实现的，而是需要将它们转换成其他形式，使用解释器模式的执行效率并不高。）
**Interpreter 模式优缺点总结：**
**Interpreter 模式优点：**
(1).易于改变和扩展文法。由于在解释器模式中使用类来表示语言的文法规则，因此可以通过继承等机制来改变或扩展文法。
(2).每一条文法规则都可以表示为一个类，因此可以方便地实现一个简单的语言。
(3).实现文法较为容易。在抽象语法树中每一个表达式节点类的实现方式都是相似的，这些类的代码编写都不会特别复杂，还可以通过一些工具自动生成节点类代码。
(4).增加新的解释表达式较为方便。如果用户需要增加新的解释表达式只需要对应增加一个新的终结符表达式或非终结符表达式类，原有表达式类代码无须修改，符合“开闭原则”。
**Interpreter 模式缺点：**
(1).对于复杂文法难以维护。在解释器模式中，每一条规则至少需要定义一个类，因此如果一个语言包含太多文法规则，类的个数将会急剧增加，导致系统难以管理和维护，此时可以考虑使用语法分析程序等方式来取代解释器模式。
(2).执行效率较低。由于在解释器模式中使用了大量的循环和递归调用，因此在解释较为复杂的句子时其速度很慢，而且代码的调试过程也比较麻烦。
**Interpreter 模式使用总结：**
        尽量不要在重要模块中使用解释器模式，因为维护困难。在项目中，可以使用脚本语言来代替解释器模式。
