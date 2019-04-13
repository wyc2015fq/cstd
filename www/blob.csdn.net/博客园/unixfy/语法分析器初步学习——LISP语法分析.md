
# 语法分析器初步学习——LISP语法分析 - unixfy - 博客园
# [程序最美（寻路）](https://www.cnblogs.com/unixfy/)
你还在坚持练习你的技术吗？运动员天天训练，音乐家也会演练更难的曲章。你呢？
## [语法分析器初步学习——LISP语法分析](https://www.cnblogs.com/unixfy/p/3323308.html)
语法分析器初步学习——LISP语法分析
本文参考自vczh的《[如何手写语法分析器](http://www.cppblog.com/vczh/archive/2008/06/15/53373.aspx)》。
LISP的表达式是按照前缀的形式写的，比如(1+2)*(3+4)在LISP中会写成(*(+ 1 2)(+ 3 4))，1 + 2会写成(+ 1 2)。
LISP语言的语法如下形式：
1.Operator = “+” | “-” | “*” | “/”
2.Expression = <数字> | ”(”Expression”)” | “(”Operator Expression Expression”)”
我们根据以上两条语法规则来写代码：
//LISP语法分析器\#include <iostream>\#include<string>usingnamespacestd;//检测是否是空白符boolIsBlank(charch)
{returnch ==''|| ch =='\t';
}//检测Text是否是Stream的前缀//如果是前缀，则返回true，并将pos前移Text.size()个字符//如果不是前缀，则返回false//此函数一开始会过滤掉Stream开头的空格boolIsPrefix(conststring& Stream,int& pos,conststring&Text)
{intread =pos;//过滤空白符while(IsBlank(Stream[read]))
    {++read;
    }//不能写为：//while (IsBlank(Stream[read++]));//因为这样写会导致read至少加1if(Stream.substr(read, Text.size()) == Text)//如果是前缀{
        pos= read +Text.size();returntrue;
    }else{returnfalse;
    }
}//检测Stream开头是否是操作符+、-、*、///是的话，函数返回实际的操作符，并将pos便宜到操作符之后//否则返回0//判断语法1：Operator = “+” | “-” | “*” | “/”charIsOperator(conststring& Stream,int&pos)
{if(IsPrefix(Stream, pos,"+")|| IsPrefix(Stream, pos,"-")|| IsPrefix(Stream, pos,"*")|| IsPrefix(Stream, pos,"/"))//如果开头是操作符{returnStream[pos -1];//如果是的话，pos已经向前偏移了}else{return0;
    }
}//表达式结构体structExpression
{intResult;//返回表达式结果stringError;//返回错误信息，没错误则为空intStart;//错误发生的位置Expression() : Result(0), Start(0) {}
};//检测Stream开头是否是数字，如果是，则将pos便宜到数字之后//函数返回Expression//判断语法2中的第一部分：Expression = <数字>Expression GetNumber(conststring& Stream,int&pos)
{
    Expression Result;boolGotNumber =false;intread =pos;//过滤空白符while(IsBlank(Stream[read]))
    {++read;
    }while(true)
    {//依次读入一个字符charch =Stream[read];if(ch >='0'&& ch <='9')
        {
            Result.Result= Result.Result *10+ ch -'0';
            GotNumber=true;++read;
        }else{break;
        }
    }if(GotNumber)
    {
        pos=read;
    }else{
        Result.Error="这里需要数字";
        Result.Start=read;
    }returnResult;
}//检测Stream开头是否是表达式//如果是，则将pos前移到表达式后//实现语法2：Expression = <数字> | “(”Expression“)” | “(”Operator Expression Expression“)”Expression GetExpression(conststring& Stream,int&pos)
{intread =pos;//检测开头是否是数字//语法2第一部分：Expression = <数字>Expression Result =GetNumber(Stream, read);if(!Result.Error.empty())//如果开头不是数字{if(IsPrefix(Stream, read,"("))//检测是否"("开头{//将Result的Error清空Result.Error.clear();charOperator =0;if((Operator = IsOperator(Stream, read)) !=0)//如果是操作符，语法2第三部分：Expression = “(”Operator Expression Expression“)”{//获取左参数//递归调用Expression left =GetExpression(Stream, read);if(!left.Error.empty())
                {returnleft;
                }//保持当前readintrightRead =read;//获取右参数//递归调用Expression right =GetExpression(Stream, read);if(!right.Error.empty())
                {returnright;
                }//根据操作符Operator进行计算switch(Operator)
                {case'+':
                    Result.Result= left.Result +right.Result;break;case'-':
                    Result.Result= left.Result -right.Result;break;case'*':
                    Result.Result= left.Result *right.Result;break;case'/':if(right.Result ==0)//除数为0{
                        Result.Error="除数为0";
                        Result.Start=rightRead;
                    }else{
                        Result.Result= left.Result /right.Result;
                    }break;default://这种情况不会发生，因为前提是Operator，所以只有+、-、*、/四种情况Result.Error ="未知的操作符";
                    Result.Start=read;returnResult;
                }
            }else//如果不是操作符//语法2的第二部分：Expression = “(”Expression“)”{//获取表达式Result =GetExpression(Stream, read);//如果获取失败，则直接返回if(!Result.Error.empty())
                {returnResult;
                }
            }//检测是否有配套的")"if(!IsPrefix(Stream, read,")"))
            {
                Result.Error="此处缺少右括号";
                Result.Start=read;
            }
        }
    }//如果没有出错，则更新posif(Result.Error.empty())
    {
        pos=read;
    }//检测是否有配套")"时，如果不存在，可以直接将Result返回//这样在后面就不用检测是否出错了，因为前面凡是出错的情况//都返回了，这样就不用检测了，而直接更新pos: pos = readreturnResult;
}//测试intmain()
{while(true)
    {stringStream;
        cout<<"输入一个LISP表达式"<<endl;
        getline(cin, Stream);intpos =0;if(IsPrefix(Stream, pos,"exit"))
        {break;
        }
        pos=0;
        Expression Result=GetExpression(Stream, pos);if(!Result.Error.empty())
        {
            cout<<"表达式错误"<<endl;
            cout<<"位置："<< Result.Start <<endl;
            cout<<"错误信息："<< Result.Error <<endl;
        }else{
            cout<<"结果："<< Result.Result <<endl;
        }
    }return0;
}
![](https://images0.cnblogs.com/blog/463570/201309/15215635-049d8197403f4949a3a9faa0a2ee6432.jpg)
下面对程序代码解释如下：
**数据结构**
程序中用string型的字符串Stream来存储用户输入的表达式，int型的pos 作为当前扫描的位置。
Expression结构体用来标识语法二中的Expression，Expression结构体可以标识<数字> | ”(”Expression”)” | “(”Operator Expression Expression”)”三种形式的任意一种。Result元素用来记录正确表达式时的计算结果，Error用来当语法分析表达式时遇到的错误时，将错误信息记录下来。Start用来记录错误的位置，准确来讲是用来记录最后正确的下一个位置，因为错误字符串前的空白符会被忽略。
**函数**
**IsBlank**：用来检测char字符是否为空白符。
**IsPrefix**：用来检测Text字符串是否是Stream从pos起头的前缀。
**IsOperator**：用来检测Stream从pos起头，是否是以操作符+、-、*、/开头的。
**GetNumber**：用来检测Stream从pos起头，是否是以数字开头的。
**GetExpression**：用来检测Stream从pos开头，是否以表达式开头。
语法分析器的关键在于写出正确的语法，然后根据语法导出代码。语法定义中存在递归定义，代码中相应地也会出现递归调用。代码应该一一对应的对应到语法定义。
**设计的巧妙之处**
pos和read：pos用来标识扫描的位置，pos只用来记录正确分析的位置。在IsPrefix、GetNumber、GetExpression等函数中，都会在定义一个read，初始化为pos，用来作为实际的扫描游标，只有在正确分析后，read才会用来更新pos，否则read会返回给Result的Start，用来记录错误发生的位置，并且直接返回Result。用read来代替pos的内部操作，可以防止当分析失败时，还要讲pos还原的问题。
Expression结构体：Expression结构包含三个元素，Start和Error字段可以很地记录错误发生的位置以及错误信息。这样可以很好的处理发生错误的情况。
以上是最为简单的递归下降语法分析，更多的语法分析方面的知识有待进一步学习。




posted on2013-09-15 21:58[unixfy](https://www.cnblogs.com/unixfy/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=3323308)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)







### 导航
[博客园](https://www.cnblogs.com/)
[首页](https://www.cnblogs.com/unixfy/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/unixfy)
[订阅](https://www.cnblogs.com/unixfy/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 - 85文章 - 0评论 - 8引用 - 0
公告

Powered by:
博客园
Copyright © unixfy
