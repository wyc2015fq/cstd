# Razor深入系列3体系结构概述 - weixin_33985507的博客 - CSDN博客
2011年01月02日 21:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

目录
1 再续前缘
2 体系结构
3 核心类介绍
## 1再续前缘
     话说上一篇我们在一个大大的图片中结束了。这次我们继续从图中关键的一个函数那里继续看进去。:P
![image](https://images.cnblogs.com/cnblogs_com/facingwaller/201101/20110102205323782.png)
我们知道里面一定就是写着具体他是如何把混搭@代码和HTML给分离出来的
方法了。
进去之后是System.Web.Razor.Parser类了。
在这个类里面定义了
internal ParserBase CodeParser { get; private set; } 基类
internal MarkupParser MarkupParser { get; private set; }HTML分析者
public bool DesignTimeMode { get; set; } 表示是否是设计时。
他的方法主要是两个
**public virtual void Parse(LookaheadTextReader input, ParserVisitor visitor) {**
public virtual Task CreateParseTask(TextReader input,   
                                    ParserVisitor consumer) {
后一个我还不清楚其具体使用。
主要是前一个用于分析代码，如下图所示。
在这里我们发现这个PARSER也是一个空壳，
他主要是调用
MarkupParser.ParseDocument();
方法来分析的。
这里涉及到了
两个新的类一个是ParserContext
另外一个是ParserVisitor
**可以假设(ParserContext)context承载了所有的分析器和内容等。实际的分析是由MarkupParser.ParseDocument(); **
**最终执行的返回结果由visitor带出去。**
2体系结构
有了上面的认识，让我们从去看类的结构图，从抽象基类的属性和行为中获得一些启示。
![image](https://images.cnblogs.com/cnblogs_com/facingwaller/201101/201101022053232111.png)
一副好图往往胜过千言万语。
## 3核心类介绍
### 1ParserContext类
由上面分析，我们知道这个类很复杂。
1承载了内容的本体。即CSHTML里面的内容。Source
2承载了代码剖析类。CodeParser
3承载了HTML剖析类。MarkupParser
4代码生成器ParserVisitor
从他的方法中我们可以知道他的作用
1操作内容。但仅限自己的内容
> 
他是如何把XX.CSHTML变成 XXX.CS文件的呢？
**从他生成的XX.CS文件，我们可以看到，他是把所有HTML使用简单WRITE语句输出。**
**把C#代码直接作为类里面的内容而进行编译。**
我们知道他是逐个读取字符通过上下文判断其语意转换成.CS文件的。
返回当的字符。
判断是否到内容的结尾。
2管理其他类
1ActiveParser 来切换代码剖析类。
2_visitorStack来管理代码生成类
private Stack<ParserVisitor> _visitorStack = new Stack<ParserVisitor>();
3为visitor服务
OnComplete
OnError
OutputSpan输出剖析好的代码或HTML给代码生成类
等。
2RazorParser及其派生类
![image](https://images.cnblogs.com/cnblogs_com/facingwaller/201101/201101022053251107.png)
  1ParserContent是在为visitor打工一样，ParserContent也是在为ParserContent做事。我们可以看到左边的ParserBase拥有
很多和右边context重复的属性。 
```
// Simple wrapper properties for commonly used context properties
//注释中说是对content公用属性的简单封装。 
protected bool HaveContent { get { return Context.HaveContent; } } 
protected bool InTemporaryBuffer { get { return Context.InTemporaryBuffer; } } 
protected bool DesignTimeMode { get { return Context.DesignTimeMode; } } 
protected bool EndOfFile { get { return Context.EndOfFile; } } 
protected char CurrentCharacter { get { return Context.CurrentCharacter; } }////当前的字符 
protected SourceLocation CurrentLocation { get { return Context.CurrentLocation; } }
//细心一点你会发现对其简单封装的方法也很有多。
// Simple wrapper methods for commonly used context methods 
       protected IDisposable StartBlock(BlockType type, bool outputCurrentAsTransition = true) { 
           return Context.StartBlock(type, outputCurrentAsTransition); 
       }
       protected void EndBlock() { Context.EndBlock(); } 
       protected void Output(Span span) { Context.OutputSpan(span); }
       protected void OnError(SourceLocation location, string message) { 
           Context.OnError(location, message); 
       }
       protected void OnError(SourceLocation location, string message, params object[] args) { 
           Context.OnError(location, message, args); 
       }
       protected void End(SpanFactory spanFactory) { 
           End(spanFactory, false); 
       }
       protected void End(SpanFactory spanFactory, bool allowEmpty) { 
           if (HaveContent || allowEmpty) { 
               if (Context.InTemporaryBuffer) { 
                   throw new InvalidOperationException(XXXXX);
               } 
               End(spanFactory(Context)); 
           } 
       }
       protected void End(Span span) { 
           Context.OutputSpan(span); 
           Context.ResetBuffers(); 
       }
```
     由此可知，PARSERBASE及其基础类只是ParserContent的辅助方法。但是对于所有content，内容的操作还是全部由ParserConetnt 操作。
2他的工作是执行具体的分析工作。
由他的派生类来觉得谁是C#代码谁是HTML。并包装成SynTaxTreeNode类以供visitor生成代码，当然他并不知道谁是visitor，他只是简单的把他交给了ParserContent
## 3ParserVisitor类
他是用来生成代码的。他的对外宣称自己的工作：凡是下面的事都能搞定。
public abstract void VisitStartBlock(BlockType type);   
     public abstract void VisitSpan(Span span);    
     public abstract void VisitEndBlock(BlockType type);    
     public abstract void VisitError(RazorError err);    
     public abstract void OnComplete();
## 4SyntaxTreeNode类
他本身并不做事，但大家却通过它来进行交流和沟通。
并通过它来分类这是代码，那是注释等。
他主要有两个概念比较重要
1 Block:主要是一个集合。包含自身和父类的一个集合。
```
public IEnumerable<SyntaxTreeNode> Children { get; private set; }
```
本身并不标注是什么类型。
2 Span ：这里表明了具体的类型如注释，HTML或代码。
```
public SpanKind Kind { get; set; }
```
这个属性表明
```
public enum SpanKind { 
      Transition, 
      MetaCode, 
      Comment, 
      Code, 
      Markup 
  }
```
![image](https://images.cnblogs.com/cnblogs_com/facingwaller/201101/201101022053255501.png)
以上简单的说明了一下里面一层的体系结构。各个主要基类的作用及其一些主要需要清楚的概念。穿插了一些他们之间是如何沟通的。
下一次再写他是如何具体分析的之类的吧:)
希望能给正在分析源码的同学和在看RAZOR源码的同学一点帮助。
RAZOR系列参考
[http://www.cnblogs.com/facingwaller/category/273752.html](http://www.cnblogs.com/facingwaller/category/273752.html)
源码分析系列
[http://www.cnblogs.com/facingwaller/category/267772.html](http://www.cnblogs.com/facingwaller/category/267772.html)
