
# .net表达式计算器（中缀表达式转后缀表达式，支持20多个数学函数，支持函数嵌套,免费共享～） - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[.net表达式计算器（中缀表达式转后缀表达式，支持20多个数学函数，支持函数嵌套,免费共享～）](https://www.cnblogs.com/watsonyin/archive/2010/10/13/neval.html)
最近在网上查了一下表达工计算器的类库，发现Java版本的有一个比较成熟的叫W3EVal，好像是一个IBM工程师写的，.net就很少了（可能是我了解不够多），但投机取巧的实现思路有很多，比如：
（1）将Javasript中代码编译成.net类库，利用Javascript中的eval函数来实现;
（2）利用ScriptControl执行JavaScript脚本实现;
（3）利用DataTable的计算功能实现简单计算;
（4）利用.net动态编译功能来实现等
这些方法在csdn的坛里有讨论，请见：http://topic.csdn.net/u/20070301/13/c8c33bd1-f146-4b44-9882-aab6d430f724.html
心想既然还没有成熟的.net类库，何不自己做一个呢，其实并不难，只要稍有点耐心调试一下就好了。于是参考一编中缀表达式转后缀表达式的论文，很快写了一个，发现效果不错，所以发出来跟大家共享，希望对大家有用。
中缀表达式转后缀表达式的步骤为：
1.新建一个Stack栈，用来存放运算符
2.新建一个post栈，用来存放最后的后缀表达式
3．从左到右扫描中缀表达式：
（1）若读到的是操作数，直接存入post栈，以\#作为数字的结束
（2）若读到的是(,则直接存入stack栈
（3）若读到的是），则将stack栈中(前的所有运算符出栈，存入post栈
（4）若读到的是其它运算符，则将该运算符和stack栈顶运算符作比较：若高于或等于栈顶运算符，则直接存入stack栈，否则将栈顶运算符（所有优先级高于读到的运算符的，不包括括号）出栈，存入post栈。最后将读到的运算符入栈。
4.当扫描完后，stack栈中还在运算符时，则将所有的运算符出栈，存入post栈

计算后缀表达式的值的步骤为：
1.初始化一个空堆栈
2.从左到右读入后缀表达式
3.如果字符是一个操作数，把它压入堆栈。
4.如果字符是个操作符，弹出两个操作数，执行恰当操作，然后把结果压入堆栈。如果您不能够弹出两个操作数，后缀表达式的语法就不正确。
5.到后缀表达式末尾，从堆栈中弹出结果。若后缀表达式格式正确，那么堆栈应该为空。
类库的名称就名为NEval ，感觉还比较健壮，速度很快，支持20个数学函数，很容易扩充。目前还没有表达式合法性检查的功能，有兴趣的朋友可以扩充一下，先谢谢。
代码如下：
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码///<summary>
///表达式计算类。支持数学函数，支持函数嵌套
///作者watsonyin
///开发日期：2010年10月 版本1.0
///</summary>
publicclassNEval
{
publicNEval()
{
}
publicdoubleEval(stringexpr)
{
try
{
stringtmpexpr=expr.ToLower().Trim().Replace("",string.Empty);
returnCalc_Internal(tmpexpr);
}
catch(ExpressionException eex)
{
throweex;
}
catch
{
thrownewException("表达式错误");
}
}
privateRandom m_Random=null;
privatedoubleCalc_Internal(stringexpr)
{
/*
* 1.    初始化一个空堆栈
* 2.    从左到右读入后缀表达式
* 3.    如果字符是一个操作数，把它压入堆栈。
* 4.    如果字符是个操作符，弹出两个操作数，执行恰当操作，然后把结果压入堆栈。如果您不能够弹出两个操作数，后缀表达式的语法就不正确。
* 5.    到后缀表达式末尾，从堆栈中弹出结果。若后缀表达式格式正确，那么堆栈应该为空。
*/
Stack post2=ConvertExprBack(expr);
Stack post=newStack();
while(post2.Count>0)
post.Push(post2.Pop());
Stack stack=newStack();
while(post.Count>0)
{
stringtmpstr=post.Pop().ToString();
charc=tmpstr[0];
LetterType lt=JudgeLetterType(tmpstr);
if(lt==LetterType.Number)
{
stack.Push(tmpstr);
}
elseif(lt==LetterType.SimpleOperator)
{
doubled1=double.Parse(stack.Pop().ToString());
doubled2=double.Parse(stack.Pop().ToString());
doubler=0;
if(c=='+')
r=d2+d1;
elseif(c=='-')
r=d2-d1;
elseif(c=='*')
r=d2*d1;
elseif(c=='/')
r=d2/d1;
elseif(c=='^')
r=Math.Pow(d2, d1);
else
thrownewException("不支持操作符:"+c.ToString());
stack.Push(r);
}
elseif(lt==LetterType.Function)//如果是函数
{
string[] p;
doubled=0;
doubled1=0;
doubled2=0;
inttmpos=tmpstr.IndexOf('(');
stringfuncName=tmpstr.Substring(0, tmpos);
switch(funcName)
{
case"asin":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Asin(d).ToString());
break;
case"acos":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Acos(d).ToString());
break;
case"atan":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Atan(d).ToString());
break;
case"acot":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push((1/Math.Atan(d)).ToString());
break;
case"sin":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Sin(d).ToString());
break;
case"cos":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Cos(d).ToString());
break;
case"tan":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Tan(d).ToString());
break;
case"cot":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push((1/Math.Tan(d)).ToString());
break;
case"log":
SplitFuncStr(tmpstr,2,outp);
d1=double.Parse(p[0]);
d2=double.Parse(p[1]);
stack.Push(Math.Log(d1, d2).ToString());
break;
case"ln":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Log(d, Math.E).ToString());
break;
case"abs":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Abs(d).ToString());
break;
case"round":
SplitFuncStr(tmpstr,2,outp);
d1=double.Parse(p[0]);
d2=double.Parse(p[1]);
stack.Push(Math.Round(d1, (int)d2).ToString());
break;
case"int":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push((int)d);
break;
case"trunc":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Truncate(d).ToString());
break;
case"floor":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Floor(d).ToString());
break;
case"ceil":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Ceiling(d).ToString());
break;
case"random":
if(m_Random==null)
m_Random=newRandom();
d=m_Random.NextDouble();
stack.Push(d.ToString());
break;
case"exp":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Exp(d).ToString());
break;
case"pow":
SplitFuncStr(tmpstr,2,outp);
d1=double.Parse(p[0]);
d2=double.Parse(p[1]);
stack.Push(Math.Pow(d1, d2).ToString());
break;
case"sqrt":
SplitFuncStr(tmpstr,1,outp);
d=double.Parse(p[0]);
stack.Push(Math.Sqrt(d).ToString());
break;
default:
thrownewException("未定义的函数："+funcName);
}
}
}
objectobj=stack.Pop();
returndouble.Parse(obj.ToString());
}
///<summary>
///将函数括号内的字符串进行分割，获得参数列表，如果参数是嵌套的函数，用递归法计算得到它的值
///</summary>
///<param name="funcstr"></param>
///<param name="paramCount"></param>
///<param name="parameters"></param>
privatevoidSplitFuncStr(stringfuncstr,intparamCount,outstring[] parameters)
{
parameters=newstring[paramCount];
inttmpPos=funcstr.IndexOf('(',0);
stringstr=funcstr.Substring(tmpPos+1, funcstr.Length-tmpPos-2);
if(paramCount==1)
{
parameters[0]=str;
}
else
{
intcpnum=0;
intstartPos=0;
intparamIndex=0;
for(inti=0; i<=str.Length-1; i++)
{
if(str[i]=='(')
cpnum++;
elseif(str[i]==')')
cpnum--;
elseif(str[i]==',')
{
if(cpnum==0)
{
stringtmpstr=str.Substring(startPos, i-startPos);
parameters[paramIndex]=tmpstr;
paramIndex++;
startPos=i+1;
}
}
}
if(startPos<str.Length)
{
stringtmpstr=str.Substring(startPos);
parameters[paramIndex]=tmpstr;
}
}
//如果参数是函数， 进一步采用递归的方法生成函数值
for(inti=0; i<=paramCount-1; i++)
{
doubled;
if(!double.TryParse(parameters[i],outd))
{
NEval calc=newNEval();
d=calc.Eval(parameters[i]);
parameters[i]=d.ToString();
}
}
}

///<summary>
///将中缀表达式转为后缀表达式
///</summary>
///<param name="expr"></param>
///<returns></returns>
privateStack ConvertExprBack(stringexpr)
{
/*
* 新建一个Stack栈，用来存放运算符
* 新建一个post栈，用来存放最后的后缀表达式
* 从左到右扫描中缀表达式：
* 1.若读到的是操作数，直接存入post栈，以\#作为数字的结束
* 2、若读到的是(,则直接存入stack栈
* 3.若读到的是），则将stack栈中(前的所有运算符出栈，存入post栈
* 4 若读到的是其它运算符，则将该运算符和stack栈顶运算符作比较：若高于或等于栈顶运算符， 则直接存入stack栈，
* 否则将栈顶运算符（所有优先级高于读到的运算符的，不包括括号）出栈，存入post栈。最后将读到的运算符入栈
* 当扫描完后，stack栈中还在运算符时，则将所有的运算符出栈，存入post栈
**/

Stack post=newStack();
Stack stack=newStack();
stringtmpstr;
intpos;
for(inti=0; i<=expr.Length-1; i++)
{
charc=expr[i];
LetterType lt=JudgeLetterType(c, expr, i);
if(lt==LetterType.Number)//操作数
{
GetCompleteNumber(expr, i,outtmpstr,outpos);
post.Push(tmpstr);
i=pos;//+1;
}
elseif(lt==LetterType.OpeningParenthesis)//左括号(
{
stack.Push(c);
}
elseif(lt==LetterType.ClosingParenthesis)//右括号)
{
while(stack.Count>0)
{
if(stack.Peek().ToString()=="(")
{
stack.Pop();
break;
}
else
post.Push(stack.Pop());
}
}
elseif(lt==LetterType.SimpleOperator)//其它运算符
{
if(stack.Count==0)
stack.Push(c);
else
{
chartmpop=(char)stack.Peek();
if(tmpop=='(')
{
stack.Push(c);
}
else
{
if(GetPriority(c)>=GetPriority(tmpop))
{
stack.Push(c);
}
else
{
while(stack.Count>0)
{
objecttmpobj=stack.Peek();
if(GetPriority((char)tmpobj)>GetPriority(c))
{
if(tmpobj.ToString()!="(")
post.Push(stack.Pop());
else
break;
}
else
break;
}
stack.Push(c);
}
}

}
}
elseif(lt==LetterType.Function)//如果是一个函数，则完整取取出函数，当作一个操作数处理
{
GetCompleteFunction(expr, i,outtmpstr,outpos);
post.Push(tmpstr);
i=pos;//+1;
}
}
while(stack.Count>0)
{
post.Push(stack.Pop());
}
returnpost;
}

privateLetterType JudgeLetterType(charc,stringexpr,intpos)
{
stringop="*/^";
if((c<='9'&&c>='0')||(c=='.'))//操作数
{
returnLetterType.Number;
}
elseif(c=='(')
{
returnLetterType.OpeningParenthesis;
}
elseif(c==')')
{
returnLetterType.ClosingParenthesis;
}
elseif(op.IndexOf(c)>=0)
{
returnLetterType.SimpleOperator;
}
elseif((c=='-')||(c=='+'))//要判断是减号还是负数
{
if(pos==0)
returnLetterType.Number;
else
{
chartmpc=expr[pos-1];
if(tmpc<='9'&&tmpc>='0')//如果前面一位是操作数
returnLetterType.SimpleOperator;
elseif(tmpc==')')
returnLetterType.SimpleOperator;
else
returnLetterType.Number;
}
}
else
returnLetterType.Function;
}
privateLetterType JudgeLetterType(charc)
{
stringop="+-*/^";
if((c<='9'&&c>='0')||(c=='.'))//操作数
{
returnLetterType.Number;
}
elseif(c=='(')
{
returnLetterType.OpeningParenthesis;
}
elseif(c==')')
{
returnLetterType.ClosingParenthesis;
}
elseif(op.IndexOf(c)>=0)
{
returnLetterType.SimpleOperator;
}
else
returnLetterType.Function;
}
privateLetterType JudgeLetterType(strings)
{
charc=s[0];
if((c=='-')||(c=='+'))
{
if(s.Length>1)
returnLetterType.Number;
else
returnLetterType.SimpleOperator;
}
stringop="+-*/^";
if((c<='9'&&c>='0')||(c=='.'))//操作数
{
returnLetterType.Number;
}
elseif(c=='(')
{
returnLetterType.OpeningParenthesis;
}
elseif(c==')')
{
returnLetterType.ClosingParenthesis;
}
elseif(op.IndexOf(c)>=0)
{
returnLetterType.SimpleOperator;
}
else
returnLetterType.Function;
}
///<summary>
///计算操作符的优先级
///</summary>
///<param name="c"></param>
///<returns></returns>
privateintGetPriority(charc)
{
if(c=='+'||c=='-')
return0;
elseif(c=='*')
return1;
elseif(c=='/')//除号优先级要设得比乘号高，否则分母可能会被先运算掉
return2;
else
return2;
}
///<summary>
///获取完整的函数表达式
///</summary>
///<param name="expr"></param>
///<param name="startPos"></param>
///<param name="funcStr"></param>
///<param name="endPos"></param>
privatevoidGetCompleteFunction(stringexpr,intstartPos,outstringfuncStr,outintendPos)
{
intcpnum=0;
for(inti=startPos; i<=expr.Length-1; i++)
{
charc=expr[i];
LetterType lt=JudgeLetterType(c);
if(lt==LetterType.OpeningParenthesis)
cpnum++;
elseif(lt==LetterType.ClosingParenthesis)
{
cpnum--;//考虑到函数嵌套的情况，消除掉内部括号
if(cpnum==0)
{
endPos=i;
funcStr=expr.Substring(startPos, endPos-startPos+1);
return;
}

}
}
funcStr="";
endPos=-1;
}
///<summary>
///获取到完整的数字
///</summary>
///<param name="expr"></param>
///<param name="startPos"></param>
///<param name="numberStr"></param>
///<param name="endPos"></param>
privatevoidGetCompleteNumber(stringexpr,intstartPos,outstringnumberStr,outintendPos)
{
charc=expr[startPos];
for(inti=startPos+1; i<=expr.Length-1; i++)
{
chartmpc=expr[i];
if(JudgeLetterType(tmpc)!=LetterType.Number)
{
endPos=i-1;
numberStr=expr.Substring(startPos, endPos-startPos+1);
return;
}
}
numberStr=expr.Substring(startPos);
endPos=expr.Length-1;
}
}

///<summary>
///可以检测到的表达式错误的Exception
///</summary>
publicclassExpressionException : Exception
{
publicoverridestringMessage
{
get
{
returnbase.Message;
}
}
}
///<summary>
///字符类别
///</summary>
publicenumLetterType
{
Number,
SimpleOperator,
Function,
OpeningParenthesis,
ClosingParenthesis
}

调用代码如下：
string expr = txtExpression.Text;
NEval neval = new NEval();
return neval.Eval(expr);




posted on2010-10-13 14:24[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=1850079)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
