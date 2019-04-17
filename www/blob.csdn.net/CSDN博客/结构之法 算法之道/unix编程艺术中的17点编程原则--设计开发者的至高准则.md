# unix编程艺术中的17点编程原则--设计开发者的至高准则 - 结构之法 算法之道 - CSDN博客





2011年01月13日 20:47:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：13530标签：[编程																[unix																[算法																[数据结构																[优化																[optimization](https://so.csdn.net/so/search/s.do?q=optimization&t=blog)





Unix编程艺术中的17点编程哲学原则

  ---设计开发者的至高准则





译者:July   二零一一年一月十三日。

参考文献：The Art of Unix Programming 
By Eric Steven Raymond 


博主说明：本文是依据unix编程艺术一书的英文版，第一章部分章节，所做的翻译。
翻译过程中，参考了其中文版(姜宏等译)。若有更好的翻译意见，欢迎留言提议。
---------------------------------------


**一、unix编程艺术一书介绍**本书主要介绍了Unix系统领域中的设计和开发哲学、思想文化体系、原则与经验，
由公认的Unix编程大师、开源运动领袖人物之一Eric S. Raymond倾力多年写作而成。



包括Unix设计者在内的多位领域专家也为本书贡献了宝贵的内容。
本书内容涉及社群文化、软件开发设计与实现，覆盖面广、内容深邃，完全展现了作者极其深厚的经验积累和领域智慧。


**二、unix哲学基础**unix管道的发明人、unix传统的奠基人之一Doug McIlroy在<<unix的四分之一世纪>>中，
这样总结unix的哲学：
编写的一个程序只做一件事，并做好。编写的程序要相互协作，
同时，要能处理文本流，因为那是最普通(或最基本)的接口。



Rob Pike，是最伟大的c语言大师之一，他在<<Notes on C Programming>>一书中，
从各个不同的角度，阐述了unix哲学的几个原则：
原则1：你无法断定程序会在哪个地方耗费它的运行时间。瓶颈常常出现在意想不到的地方，
所以，别急着胡乱找个地方，乱改一通，除非你确实已经找到并证实问题的症结所在。



原则2：估量。在你还没有对程序代码进行整体估量之前，尤其是还没找到最耗时的那部分之前，
别去天真的想着优化速度。



原则3：花俏的算法通常在n比较小时，很慢。因为，花俏而不切实际的算法的常数复杂度很大。
除非你能确定n一定是很大的，否则，不要去冒昧的使用花俏算法(即使n很大，也要优先考虑原则2)。



原则4：花俏的算法比简单而切实际的算法更容易出错(bug)、且更难实现(维护)。所以，非情不得已时，
尽量采用简单的算法，与简单的数据结构相配合。



原则5:数据压倒一切。如果，你已经选择了正确的数据结构并且把一切都组织的井井有条，
那么，正确高效的算法不言而喻。编程的核心在数据结构，而不在算法。(仅代表Rob Pike个人观点)。



Ken Thompson，unix最初版本的设计者，和实践者，面对上述Rob Pike的原则4时，说道：
如果你拿不准，就穷举吧。


**三、unix哲学原则**更多的unix哲学艺术，不是由先哲们口头所阐述的，而是由他们所作的一切，包括unix本身，所体现出来

的。从整体上来说，可以概括为以下**17**点原则：

1、Rule of Modularity: Write simple parts connected by clean interfaces.
1、模块原则：尽量使用简洁的接口套和简单的组件。
正如，Brian Kernighan 曾经说过，"计算机编程的本质就是尽量控制(降低)程序的复杂度"。
设计一个程序，调试纠错往往占了大部分的开发时间，最终弄出一个拿得出手的可用系统，
与其说是才华横溢的设计成果，还不如说是一路磕磕碰碰的结果。



编写复杂而庞大的软件，唯一的方法，就是控制好，并降低程序的整体复杂度，用清晰的接口把若干的模块组合成一个复杂软件。

这就是，模块原则，即尽量模块化，各模块之间，减少耦合，如此独立，才不会在改动时，牵一处，而动全身。


2、Rule of Clarity: Clarity is better than cleverness.
2、清晰原则:清晰胜于取巧。
写的程序，是给计算机执行的，但却是给人看的，如程序后期的维护。
一个艰深晦涩的程序，将使后期的维护、修正、甚至性能提升、算法优化等工作，都将重重受阻。



所以，编写程序的时候，时刻牢记：编写清晰易懂的程序，添加良好有用的注释。
于人于己，皆有利。


3、Rule of Composition: Design programs to be connected to other programs.
3、组合原则：设计时，要考虑连接组合。
考虑尽量让程序彼此之间能通信，让程序具有组合性，彼此模块独立。


4、Rule of Separation: Separate policy from mechanism; separate interfaces from engines.
4、分离原则：策略同机制分离，接口同引擎分离。
如果，将策略同机制，杂糅成一团，将会有俩个负面影响，
一、是策略变得死板，难以适应用户需求的改变；二、这样做的同时，也将意味着任何策略的改变都将极有可能动摇到整个机制。



而，相反，将策略同机制，俩者分离的话，就可能在尝试新策略的时候，不会打破原有机制。
且，我还能更容易的为机制编写出较好的测试用例。


5、Rule of Simplicity: Design for simplicity; add complexity only where you must.
5、简洁原则：设计尽可能简洁，复杂度能低则低。
有的程序员，常常喜欢捣一些错综复杂的东西，来显示或满足自己的优越感、虚荣感。
程序员都很聪明，因自己有足够的能力玩转一些复杂而抽象的东西，而引以为傲，这本无可厚非。



但与此同时，他们在玩弄复杂、故弄玄虚的之后，必将深感厌恶程序之纠错调试工作。
因为，故弄玄虚的代价，就是一堆代价高昂的废品。

所以，最好的方式是，简洁为美。一切，以简洁至上。


6、Rule of Parsimony: Write a big program only when it is clear by demonstration that 

nothing else will do.
6、吝啬原则：除非别无他法，否则，不要去编写庞大的程序。
这点，跟上第5点差不多，以简洁为美，不要刻意去编写庞大而复杂的程序。


7、Rule of Transparency: Design for visibility to make inspection and debugging easier.
7、透明原则：设计要透明可见，以便审查和调试。
充分考虑透明性、显见性、简洁性。


8、Rule of Robustness: Robustness is the child of transparency and simplicity.
8、健壮原则：健壮的程序源于透明与简洁。
程序越简洁，越透明，程序就越健壮。


9、Rule of Representation: Fold knowledge into data so program logic can be stupid and 

robust.
9、表示(法)原则：把知识叠入数据，以求逻辑结构质朴而健壮。
建模，以求逻辑结构清晰。


10、Rule of Least Surprise: In interface design, always do the least surprising thing.
10、通俗原则：接口设计，避免标新立异。
程序的好坏由用户来评判，最简单易用、最通俗易懂的程序，往往是最受用户欢迎的程序。


11、Rule of Silence: When a program has nothing surprising to say, it should say nothing.
11、缄默原则：如果一个程序没什么好挑剔的，那就保持沉默。
程序不要有多余冗杂的部分，尽量简洁为上，不需要的多余功能，就不要有。


12、Rule of Repair: When you must fail, fail noisily and as soon as possible.
12、补救原则：出现异常时，马上退出并适当给出足够的出错信息。
尽可能让程序以一种容易诊断出错误的方式终止掉。


13、Rule of Economy: Programmer time is expensive; conserve it in preference to machine 

time.
13、经济原则：宁可多花机器一分，也不浪费程序员一秒。
时刻记住，程序员的时间是宝贵的，不要无故浪费一分一秒。


14、Rule of Generation: Avoid hand-hacking; write programs to write programs when you can.
14、生成原则：避免手工hack(直译了)，尽可能编写程序，让程序去生成程序。
程序规格越简单，设计者就越容易做对。


15、Rule of Optimization: Prototype before polishing. Get it working before you optimize it.
15、优化原则：雕刻前先要有模型，跑之前，要先学会走。
先制作原型，再精雕细琢。优化之前，先确保能用。否则，一切美妙的优化，都是白搭。
或者如，极限编程大师 Kent Beck所说，先求运行，再求正确，最后求快。



不要一味的去考虑那些蝇头小利的所谓效率提升。过早优化，往往成为一切万恶之根源。


16、Rule of Diversity: Distrust all claims for "one true way".
16、多样原则：绝不要去相信什么所谓"不二法则"的言论。
即使最出色的软件，也常常会受限于设计者的想象力。
爱因斯坦曾说过一句话，这个世界缺乏的不是技术，而是想象力。



没有人能聪明到把所有的东西，都能事先预言安排好。


17、Rule of Extensibility: Design for the future, because it will be here sooner than you 

think.
17、扩展原则：设计要着眼于未来，因为有时未来来的要比想象中的快。
永远不要认为自己的设计已经完美了，可以终止优化、或提升了。


所以，要为数据格式，和代码留下可扩展的空间，后期你自会发现，当初的选择是明智的。

因此，设计要着眼于未来。
ok，更多请参考原书。完。
---------------------------------



**作者声明：本人July对本博客所有文章和资料享有版权，转载或引用任何文章、资料请注明出处。向您的厚道致敬。谢谢。July、二零一一年一月十三日。**](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)




