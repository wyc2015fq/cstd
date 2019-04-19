# [C指针] 用图表解读C声明：Unscrambling C Declarations by Diagram - Koma Hub - CSDN博客
2019年02月01日 21:33:58[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：50
在本节中，我们将展示一个带有编号步骤的图表。如果您按照步骤进行，从一个开始并遵循引导箭头，任意复杂性的C声明可以快速翻译为英语（也可以是任意复杂性）。我们将通过忽略图中的typedef来简化声明。若要读取typedef，请忽略“typedef”一词来翻译声明。如果它转换为“p is a…”，那么现在只要您想声明它所转换的类型，就可以使用名称“p”。
> 
In this section we present a diagram with numbered steps. If you proceed in steps,starting at one and following the guide arrows, a C declaration of arbitrary complexity can quickly be translated into English (also of arbitrary complexity). We'll simplify declarations by ignoring typedefs in the diagram. To read a typedef, translate the declaration ignoring the word "typedef". If it translates to "p is a…", you can now use the name "p" whenever you want to declare something of the type to which it translates.
### 用于C声明的魔术解码器环（Magic Decoder Ring for C Declarations）
C中的声明以粗体字阅读，即从右到左和从左到右交替阅读。谁会想到会有一个特殊的词来形容呢！从左边读取时找到的第一个标识符开始。当我们将声明中的一个令牌与图匹配时，我们将其从进一步考虑中删除。在每一点上，我们首先看右边的令牌，然后看左边的令牌。当一切都被清除后，工作就完成了。
> 
Declarations in C are read boustrophedonically,i.e. alternating right-to-left with left-to right. And who'd have thought there would be a special word to describe that! Start at the first identifier you find when reading from the left. When we match a token in our declaration against the diagram, we erase it from further consideration. At each point we look first at the token to the right, then to the left. When everything has been erased, the job is done.
让我们尝试几个使用图表解读声明的例子。假设我们想知道我们的第一个代码示例意味着什么：
> 
Let's try a couple of examples of unscrambling a declaration using the diagram. Say we want to figure out what our first example of code means:
```cpp
char* const *(*next)();
```
在我们解读这份宣言时，我们逐渐“清除”我们已经处理过的部分，以便我们能确切地看到还有多少。同样，记住const的意思是“只读”。仅仅因为它表示常量，它并不一定意味着常量。
> 
As we unscramble this declaration, we gradually "white out" the pieces of it that we have already dealt with, so that we can see exactly how much remains. Again, remember const means "read-only". Just because it says constant, it doesn't necessarily mean constant.
该过程在表中表示。在每个步骤中，我们要处理的声明部分都用粗体打印。从第一步开始，我们将继续执行这些步骤。
> 
The process is represented in Table. In each step, the portion of the declaration we are dealing with is printed in bold type. Starting at step one, we will proceed through these steps.
### 解读C声明的步骤（Steps in Unscrambling a C Declaration）
|Declaration Remaining			(start at leftmost identifier)|Next Step to Apply|Result|
|----|----|----|
|char * const *(*next )();|step 1|say "next is a…"|
|char * const *(*) ();|step 2,3|doesn't match, go to next step, say "next is a…"|
|char * const *(***** )();|step 4|doesn't match, go to next step|
|char * const *(* )();|step 5|asterisk matches, say "pointer to …", go to step 4|
|char * const *() ();|step 4|"(" matches up to ")", go to step 2|
|char * const * () ;|step 2|doesn't match, go to next step|
|char * const * () ;|step 3|say "function returning…"|
|char * const * ;|step 4|doesn't match, go to next step|
|char * const * ;|step 5|say "pointer to…"|
|char * const ;|step 5|say "read-only…"|
|char * ;|step 5|say "pointer to…"|
|char ;|step 6|say "char"|

![](https://img-blog.csdnimg.cn/20190201213218998.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
然后把它们放在一起阅读：
“下一个是指向返回指向只读指针char的函数的指针”，我们已经完成了。现在让我们尝试一个更复杂的例子。
> 
Then put it all together to read:
"next is a pointer to a function returning a pointer to a read-only pointer-to-char" and we're done. Now let's try a more complicated example.
```cpp
char *(*c[10])(int **p);
```
尝试按照与上一个示例相同的方式完成这些步骤。本章末尾给出了一些步骤，让您有机会自己尝试并比较您的答案。
> 
Try working through the steps in the same way as the last example. The steps are given at the end of this chapter, to give you a chance to try it for yourself and compare your answer.
