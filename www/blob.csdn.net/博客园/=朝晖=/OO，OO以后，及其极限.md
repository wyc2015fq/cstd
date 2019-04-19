# OO，OO以后，及其极限 - =朝晖= - 博客园
# [OO，OO以后，及其极限](https://www.cnblogs.com/dhcn/p/7105508.html)
1、什么是软件开发?
　　软件开发的过程就是人们使用各种计算机语言将人们关心的现实世界映射到计算机世界的过程；
　　现在的计算机的数学理论基础是由计算机的开山鼻祖，大名鼎鼎的图灵于1937年提出的图灵机模型。 随后不到十年，电子计算机就诞生了(1945)。(埃尼亚克) 
　　我们都知道，它当时的主要任务之一就是用于导弹弹道轨迹的计算。当时的软件开发(如果可以称之为软件开发的话)与现在的大不相同。为了算一道题，要有人事先把完成加减乘除等各类运算的部件像搭积木那样搭起来，如果换一道题，则要把这些部件分解开来，根据新的要求重新搭建，效率极低(与现在比)。
　　现代电子计算机的体系结构及实际计算模型来自冯.诺依曼的思想。 1946年他和他的同事们发现了埃尼亚克的缺陷，发表了一份报告，提出了程序放入内存，顺序执行的思想，这样，当算一道新题时就只需采取改变计算机中的程序的“软”的方法。
　　英国的科学家维尔克斯实现了冯.诺依曼的思想，领导研制了“艾克萨克”，在技术上比埃尼亚克来了个大的飞跃。
　　因此，现在的计算机通常被称为冯.诺依曼计算机。 
　　软件开发的历史也从此正式开始。
　　早期程序员们使用机器语言来进行编程运算；随着编译技术的出现，人们设计了许多更高级别的语言；这些语言摆脱了机器语言繁琐的细节，更接近于人的自然语言，迅速流行开来。
　　据统计，全世界的高级语言起码有几千种，但从可计算性的角度看，它们的计算能力都等价于图灵机。 已经证明，一个计算机语言，只要除了赋值语句之外，还包括顺序语句，条件语句和循环语句，它的计算能力即相当于图灵机。 这里当然要排除其他技术因素的影响，如程序长度，变量个数，数据精度等。
　　由于图灵机的想法是把问题转化为一步一步按规则执行的机械求解过程，各种计算机语言也不过都是某种形式语言，因此软件开发的过程实质上就是程序员们对客观世界问题域的形式化的过程。
　　程序员们先建立问题的模型(形式化)，再用计算机语言加以合适的表达，最后再输入计算机里进行计算。
2 从 PO(面向过程)到OO(面向对象)
　　从上一节的论述我们知道:
　　软件开发的过程就是人们使用各种计算机语言将人们关心的现实世界(问题域)映射到计算机世界的过程；如下图:
　　现实世界问题域 → 建立模型(借助某种建模思想) → 编程实现(借助某种编程语言) → 计算机世界执行求解
　　在谈论OO之前，先让我们回顾一下高级语言的历史。
　　最早的高级语言大约诞生于1945年，是德国人楚译为他的Z-4计算机设计的Plan Calcul，比第一台电子计算机还早几个月；在电子计算机上实现的第一个高级语言是美国尤尼法克公司于1952年研制成功的Short Code；而真正得到推广使用，至今仍在流行的第一个高级语言是美国的计算机科学家巴科斯设计，并于1956年首先在IBM公司的计算机上实现的FORTRAN语言。
　　早期的高级语言主要是应用于科学和工程计算，其代表作有FORTRAN和ALGOL60。计算机进入商业和行政管理领域以后，出现了COBOL和 RPG等便于商界使用的语言。近年来，这类语言和数据库技术，图形界面技术(可视化编程)，面向对象的思想及RAD(Rapid Application Devolopment)的概念相结合，形成了一批更方便使用的所谓第四代语言(4GL)，如Powerbuilder，Delphi，VB等。 
　　前一类应用于科学和工程计算的大型语言相对来说更为基础，因而也更为灵活，应用范围更为广泛。 在FORTRAN，BASIC(学习型)之后，自70年代以来，模块特征更明显，简单易用，可靠性强的PASCAL异军突起，在世界范围内广泛流行。 但进入80年代以后，它的地位又逐渐为更实用的C语言替代。 到现在，C语言的地位已相当于一种“高级汇编语言”了。 
　　80年代后期，面向对象的语言开始浮出水面，C++借助使用C语言的庞大程序员队伍，一举建立了面向对象语言的老大地位。从而OO的思想正式统治了整个软件开发界。C++的流行甚至使得美国军方从80年开始大力扶植的Ada语言还未及推广便胎死腹中了。 
　　90年代以后，计算机世界更是发生了天翻地覆的变化，原先的单机平台让位于WEB，“网络就是计算机”，新的语言不仅要是OO的，Visual的，更要是基于WEB的。 Java语言借INTERNET的东风，横空出世，一夜红遍天下，变化之快令人瞠目结舌。
　　上述这些变化，表面是不同的编程工具在城头变幻大王旗，但其背后反映的却是一种更为深刻的认识论的改变，即你是用何种观点来认识这个世界的？
　　在PO中，程序员们分析了问题域之后，得到一个PO的模型，其中常见的词语是变量，函数，过程等；
　　PO: 现实世界 → 流程图(变量，函数) → PO语言 → 执行求解
　　在OO中，程序员们先得到一个OO的模型，其中常见的词语是类，对象，方法，消息等；
　　OO: 现实世界 → 类图(对象，方法) → OO语言 → 执行求解 
　　我们可以看到:
　　PO更接近于计算机世界的物理实现；OO则更符合于人们的认识习惯；
　　如果说，软件危机的根源在于人们认识表达的过程(不断反复，逐步深化)和冯.诺依曼计算机的实现过程(顺序执行)之间存在巨大鸿沟的话，那么从PO到OO意味这这鸿沟在逐渐缩小；
　　OO作为一种思想及编程语言，为软件开发的整个过程——从分析设计到coding——提供了一个完整解决方案。 OO堪称是软件发展取得的里程碑式的伟大成就，人们无不为之喝彩！
　　一提到OO，我们将会联想到C++，smalltalk，simula，联想到Use Case，ROSE，Mr Jacboson 以及Rational公司，但却很少有人会提及维特跟斯坦(Wittgenstein)。 然而，正是这位充满传奇色彩的哲学家，不仅第一次清楚地表达了OO的思想，更进一步提及了OO以后的事情，及其最后的极限所在。
　　在目前这个变幻莫测的时代重温并认真思考一下这位先哲的思想，或许能使我们更加清醒，更明确前方道路的漫长。
3 维特跟斯坦(Wittgenstein)与OO
　　维特跟斯坦是本世纪乃至人类哲学史上最伟大的哲学家之一。 他生前只于1922年出版了一本著作——《逻辑哲学论》(Tractatus Logico-Philosophicus)。 在该书中，他阐述了一种世界观，或者说一种认识世界的观点，这种观点，在六七十年后的今天，终于由一种哲学思想沉淀到技术的层面上来，成为计算机业界的宠儿，这就是“OO”，Object-Oriented，面向对象，或物件导向(港台用语)。
　　让我们先扯得远点儿，(可能扯得确实远了点)，先谈谈这个人，再谈他 《逻辑哲学论》一书
中的 OO 思想。
　　对西方哲学感兴趣的朋友都知道，在西方哲学史上发生过两次深刻的转向。 一次是由“本体论”向“认识论”的转向；另一次是由“认识论”到“方法论”的转向。
　　古希腊罗马时代的西方哲学，被称为“本体论”的时代，那时哲学家们最关心的是世界的本源问题，即世界是由什么组成的？世界的本质是什么？等等这些探究世界根本的问题。 从泰勒斯的“万物皆水”到“四元素”说，“原子论”，再到伯拉图的“绝对理念”，都对这一问题提出了各自的见解。
　　基督教借助信仰的影响力量(犹太教传统)和世俗的政治专制力量(罗马传统)，以上帝这一概念对此问题作出终结性回答，持异议者将有生命危险，布鲁诺即因此被烧死。当然，基督教也出了几个杰出的哲学家，他们继承了希腊的理性传统，混合了圣经的思想，构造了宏伟的基督教哲学体系。 他们的代表人物是奥古斯丁和托马斯.阿奎拿。 阿奎拿曾提出了证明上帝存在的五种方法(希腊理性传统) 。
　　中世纪之后，随着文艺复兴运动的兴起，人们开始从上帝的阴影下走出，发现了自身的作用，即人的价值。 以笛卡尔的“我思故我在”为标志，哲学开始了由“本体论”向“认识论”的转变；这一时期哲学关心的是人的认识的问题，具体的流派以各自侧重的不同常分为“唯理论”和“经验论”，前者注重理论演绎，以笛卡尔等人为代表；后者则注重感觉和经验，代表人物是洛克，培根等。 
　　科学其实就是融合了“唯理论”和“经验论”的长处而发展壮大的。 既强调试验和观察，运用归纳法，也重视利用数学和逻辑从归纳的结果出发，构造理论框架，演绎出新的科学定理。 
　　到了康德以后，尤其是本世纪初，哲学的核心问题开始由“认识论”转向“方法论”，哲学关心的核心问题是认识如何实现的问题。这一时期，哲学家们开始认识到语言对于人的认识的特殊地位，他们纷纷从研究语言出发来研究哲学。 其中甚至有些人极端的认为以前的所有哲学问题不过是语言的问题而已。
　　哲学的这次转向有时也被称为是“语言的转向” ，新的哲学观点被冠以“语言哲学”或“分析哲学”的名称。
　　“语言哲学” 的发展经历了两个阶段，或者说可以被进一步分为两个学派：“逻辑分析派”和“日常语言派”。维特跟斯坦的引人瞩目之处在于：他在前期持逻辑分析的观点，后期则持日常语言的观点。
　　总结一下上面论述的要点，现在我们知道: 
　　西方哲学的发展，经历了两次变革，一次是认识论转向；一次是语言转向；第一次转向使哲学的基础从本体论和形而上学变为认识论，从研究超验的存在物转到研究认识的主体和主客体关系；第二次转向把对主客体的关系的研究变成了对主体间的交流和传达问题的研究。把对主体的研究从观念和思想的领域转到了语言的领域(语句及其意义)；这两次转向的代表人物分别是笛卡尔和维特跟斯坦。 
　　下面是维特跟斯坦(Wittgenstein)的小传。
　　维特跟斯坦1889年生于维也纳。 青年时期的教育首先使他成为一名航空专业工科研究生，1908-1911，曼彻斯特。23岁时他到著名的剑桥大学学习哲学，成为罗素的学生。据说维特跟斯坦当时正为自己未来的发展方向感到迷茫，工科还是哲学？一天晚上他带着自己的一些手稿(杂感之类的东西)来找罗素，要罗素读一下，然后判断自己是否是学哲学的料，如果不行的话，就准备卷铺盖走人。 罗素拿起他的手稿，只看了头一句话，就坚决地对他说，你不学哲学太可惜了之类的话。 维特跟斯坦这才坚定了投身哲学的决心。
　　在第一次世界大战期间(1914-1918)，维特跟斯坦参加奥地利军队，后来被俘。 正是在这段时间里，他完成了自己的成名作，《逻辑哲学论》(Tractatus Logico-Philosophicus)。 该书于1922年以英文出版，是他生前出版的唯一的一本著作。
　　在这本书中，维特跟斯坦阐述了自己的"逻辑原子论"的哲学观点，成为该学派的代表人物之一。 其他著名的“逻辑原子主义者”包括罗素，怀海特，弗雷格等。该书的出版开创了哲学研究的新方向。
　　完成这本书后，维特跟斯坦认为自己已经解决了所有的哲学问题，于是他就真的放弃了哲学，先是作了一个花匠，后来作了一名小学教师。 需要说明的是，他家很富有，但他主动放弃了对巨额遗产的继承权，自愿过一种朴素简单的生活。
　　大约10年的时间，维特跟斯坦脱离了哲学界。 但后来，他对自己在《逻辑哲学论》中表述的哲学的可靠性产生了深深的怀疑。于是，在1929年他重新返回剑桥，成为三一学院的老师。1939 年他得到了教授的职称。但在第二次世界大战期间，他却以一名医生的身份为军队服务。1947年维特跟斯坦从剑桥退休，定居于爱尔兰。这一时期他对哲学的思考形成了他后期的哲学思想。1949年他去了趟美国，当年回来时不幸身患癌症，1951年在牛津与世长辞。
　　死后，他后期的思考被整理出版，《哲学研究》(Philosophical Investigations)，《关于确定性》(On Certainty) 等等，开创了语言哲学研究的另一个新的方向。
　　以上我罗里罗唆，主要是介绍了维特跟斯坦在哲学史上的地位，及其颇具传奇色彩的一生。兴之所至，请大家见谅。
　　下面我们进入主题，谈一谈《逻辑哲学论》 及其中表述的OO思想。 
4 《逻辑哲学论》中的OO
　　《逻辑哲学论》是维特根斯坦生前出版的唯一著作，也是他的成名之作。书中的观点反映了他前期的哲学思考，通常被冠以“逻辑原子论”的标签。 维特根斯坦在前期认为：传统的形而上学的混乱和困惑是由于误用语言所致；把哲学从“语言病”中解放出来，是哲学的重要任务。 他力图建立“经过语言分析的澄清活动”的本体论和认识论；维特根斯坦甚至宣称：哲学的唯一任务就是治疗其所患的语言病，一旦这个目的达到，哲学就没有别的事要作了，也就没有必要存在了。
　　维特根斯坦认为他的《逻辑哲学论》正是在上述意义上确凿无疑的从根本上解决了哲学的基本问题。他的基本解决方法是：为思想的表述划一条界限，分清什么是能够说的，什么是不能说的。 他认为，凡是能够说的东西，都可以由经验科学说清楚，对于不能说的，则应保持沉默。
　　《逻辑哲学论》一书只有薄薄的30页纸。书的结构也很独特，有点像古龙的小说？共分7节，
以如下结构排列:
1 The world is everything that is the case
1.1 The world is the totality of facts，not of things
1.1.1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx(省略)
1.1.2 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx(省略)
............
............
1.2 The world divides into facts.
1.2.1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx(省略)
1.2.2 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx(省略) 
............
............ 
2 What is the case，the fact，is the existence of atomic facts.
2.1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx(省略)
............
............
7 Whereof one cannot speak，thereof one must be silent.
　　该书语言简洁，结构独特，内涵丰富，略带神秘色彩，因此有人称之为西方的“道德经”。
　　下面我们将进入此书，体会一下这本在20年代出版的书中蕴含的 OO 思想(中英文对照)。
　　说明: 中文是我自己翻译的，不妥之处，敬请指教。
　　该书第一节第一句话劈头说到: 
1 世界就是所有为真的一切。 
( The world is everything that is the case) 
　　这里，“is the case” 即 “is true or correct”，也就是我们在数理逻辑中所说的那个“真”，是世界的实际情况。 
　　接下来是对 1 这句话的进一步说明：
1.1 世界是所有事实，而非事物，的总和。 
(The world is the totality of facts，not of things )
　　对 1.1 这句话又作了进一步说明：
1.11 世界是由事实所决定的，并且由全部事实所决定。
(The world is determined by the facts, and by these being all the facts.)
1.12 因为事实的总和既断定了什么是真的，也断定了什么不是真的。
(For the totality of facts determines both what is the case, and also all that is not the case.) 
1.13 逻辑空间中的事实(所有为真的)就是整个世界
(The facts in logical space are the world) 
1.2 世界可以分解为事实
( The world divides into facts。)
1.2.1 任一事实或者为真，或者不为真，其它所有事实皆然。
(Any one can either be the case or not be the case, and everything else remains the same.)
　　*******************************************************************
　　小结1: 1 主要想说明 --- 世界由实际发生的一切事实(facts)所组成。
　　*******************************************************************
　　第二节
2 那些为真的事实，是由原子事实组成的。
(What is the case，the fact, is the existence of atomic facts.)
2.01 一个原子事实是多个对象(或实体，事物)的组合。( object !!)
(An atomic fact is a combination of objects(entities，things.)
2.011 事物的一个基本性质就是，它可以成为某个原子事实的一个组成部分
(It is essential to a thing that it can be a constitute part of an atomic fact)
......
2.013 每个事物都是，正如假设的那样，处于一个由可能的原子事实空间之中；我可以
想象这样的空间为空，我却无法想象一个无原子事实空间可依的事物。
(Everything is, as it were, in a space of possible atomic facts; I can think of this space as empty, but not of the thing without the space.)
2.014 对象包含了事务的全部状态的可能性(?)
(Objects contain the possibility of all states of affairs) 
.......
2.02 对象是简单的(基本的)
(The Object is simple.)
2.0201 每个关于复杂事物的陈述都可以分解为对其组成部分的陈述，而且能被分解成能完全描述这些复杂事物的陈述。
(Every statement about complexes can be analysed into a statement about their constituent parts, and into those propositions which completely describe the complexse.)
2.021 对象形成了世界的基础。因而它们不会是复合物。
(Objects form the substance of the world.  Therefore they cannnot be compound.)
2.022 尽管我们主观想象中的世界和现实的客观世界是不同的，但是它们之间显然有些东西——一种模式——是相同的。
(It is clear that however different from the real one an imagined world may be
it must have somthing -- a form -- in common with the real world。)
2.023 这种固定不变的模式由对象组成。
(This fixed form consists of the objects)
　　例如: 我们在头脑中想象出来 "一个人骑着一辆自行车" 这幅情景和客观世界中实际发生的情景可能是不是完全相同的，但它们二者之间都具有一种相同的结构: 人——骑——自行车正是由于这个相同的结构，我们的主观世界才和客观世界发生了一致的联系，我们的表达才成为可能。 这个结构，是由更简单的对象组成的，如人，自行车。
......
2.026 只有存在对象，才能有一个固定模式的世界的存在。
(Only if there are objects can there be a fixed form of the world)
2.03 在原子事实中，对象如一条锁链相互连接。
(In the atomic fact objects hang one in another，like the links of a chain)
2.031 在原子事实中，对象以某种确定的方式相连。
(In the atomic fact the objects are combined in a difinite way)
2.032 原子事实中对象相互连接的方式即是原子事实的结构
(The way in which objects hang together in the atomic fact is 
the structure of the atomic fact)
......
2.034 事实的结构存在于原子事实的结构之中 
(The structure of the fact consists in the structures of the atomic facts)
2.04 存在的原子事实的总和即是世界
(The totality of existent atomic facts is the world)
2.05 存在的原子事实的总和也确定了那些原子事实不存在
(The totality of existence atomic facts also determines which atomic facts do not exist) 
......
2.1 我们为自己产生事实的图画。
(We make to ourselves picturesof facts) 
　　说明: 指我们的主观世界中产生一幅客观世界的图画，即主观对客观的认识图景。
2.12 那图画是现实的模型
The picture is a model of reality.
2.13 对象(现实世界中的)对应于那图画中的各元素。 
To the objects correspond in the picture the elememts of the picture.
2.131 那图画中的各元素代表对象(现实世界中的)
The elements of picture stand，in the picture，for the objects.
2.14 那图画的存在是由于这样一个事实: 它的各元素之间以某种固定方式相结合。
The picture consists in the fact that its elements are combined 
with one another in a difinite way.
2.141 那图画即是一个事实
The picture is a fact.
2.15 那图画的各元素以某种固定方式相结合，代表了事物(现实世界的)也是如此相结合的。 
那图画的各元素的结合方式称为该图画的结构，该结构也称为该图画的表达模式。
That the elements of the picture are combined with one another in a difinite way，
represents that the things are so combined with one another. This connexion of the elements of the picture is called its structure，and the possibility of this structure is called the form of representation of the picture.
......
2.1511 这样，那图画与现实就联系起来了。 那图画就逼近现实了。
Thus the picture is linked with reality；it reaches up to it.
2.1512 它就象是现实按比例缩小的图形。
It is like a scale applied to reality.
......
2.16 事实要想表达为图画，该图画和它之间必定要存在某种共同之处。
In order to be a picture a fact must have somthing in common with what it pictures.
......
2.17 为了能够代表现实---正确或错误的---图画必须与现实保持一致的东西，是它的表达模式。
What the picture must have in common with reality in order to be able to represent after its manner---rightly or falsely -- is its form of representation.
......
2.172 那图画却无法表达它自己的表达模式。 它只是将其径直显现出来。
The picture，however，cannot represent its form of representation. It shows it forth.
......
2.18 为了能够代表现实---正确或错误的---无论何种模式的图画，它必须与现实保持一致的东西，是逻辑结构，即: 现实世界的模式。
What every picture，of whatever form，must have in common with reality in order to be able to represent it at all---rightly or falsely -- is the logical form，that is，the form of reality.
2.181 如果那表达模式是逻辑的模式，那么那图画也可以称为一幅逻辑图。
If the form of representation is the logical form，then the picture is called a logical picture.
2.182 每一副图也是一幅逻辑图。 (另一方面，并非每副图都是空间图)
Every picture is also a logical picture.(On the other hand，for example，not every pivture is spatial)
　　说明: 主要强调此处的 picture(与现实世界逻辑上同构) 与 通常我们所说的picture(现实世界的空间结构描述图)不同。 
2.19 逻辑图可以描述现实世界。
The logical picture can dipict the world.
2.2 那图画与它所描述的具有相同的逻辑表达模式
The picture has the logical form of representation in common with what it pictures.
2.21 那图画与现实或者一致，或者不一致；它或者正确，或者错误；或者为真，或者为假。
The picture agrees with reality or not；it is right or wrong；true or false.
　　说明：“1998年5月12号北京下雨了” 和 “1998年5月12号北京没下雨” 都是一幅picture，它们的逻辑结构，或者说表达模式，完全相同，但其真值不同。
2.22 那图画通过表达模式来描述世界，独立于它的真假。
The picture represents what it represents ，indepently of its truth or falsehood，
through the form of representation.
2.221 那图画表达的是它的实际意思。
What the picture represents is its senses.
2.222 它的实际意思与现实相符或者不相符，从而它为真或者为假。
In the agreement or disagreement of its sense with reality，
its truth or falsity consists.
2.223 要想知道图画是真还是假，必须将其与现实世界进行比较。
In order to discover whether the picture is true or false we must compare 
it with reality.
......
　　******************************************************************************
　　小结2: 2　主要说明 --- 事实(facts)由原子事实(atomic facts)所组成；
　　　　　　　原子事实(atomic facts)由更基本的对象(objects)所组成；
　　　　　　　我们的关于外部世界的主观描述图画，与它所描述的外部世界具有相同的逻辑
　　　　　　　结构；注: (这即是相当于软件开发中的"建模") 
　　******************************************************************************
　　第三节 
3 事实的逻辑图画就是思想。 
The logical picture of the facts is the thought.
3.01 所有真的思想的总和就构成了整个现实世界一幅图画。
The totality of true thoughts is a picture of the world.
3.02 ......所有能被想象的也是可能的。
......What is thinkable is also possible.
3.03 我们不能想象出不合逻辑的东西，因为那样一来我们将不得不进行不合逻辑的思维。
We cannot think anything unlogical，for otherwise we should have to think unlogically.
......
　　******************************************************************************
　　小结3: 3　主要说明 --- 我们的思想也是以上述图画方式来反映客观世界的
　　　　　　　我们的思想与客观世界是逻辑同构的
　　******************************************************************************
　　第四节 
4 思想是有意义的命题
The thought is the significant proposition.
4.01 命题是现实的图画。 命题是我们想象的现实的模型。
The proposition is a picture of reality.
The proposition is a model of the reality as we think it is.
......
4.1 一个命题给出了一个原子事实的存在或是不存在。
A porposition presents the existence and non-existence of atomiac facts.
4.11 真命题的总体就是整个自然科学
The totality of true propositions is the total natural science.
......
4.112 哲学的目标就是澄清思想。
　　　哲学不是一个理论，而是一种活动。哲学工作就是去阐明。
　　　哲学的结果不是一些"哲学命题"，而是使命题清晰。
The object of philosophy is the logical clarification of thoughts.
Philosophy is not a theory but an activity.
A philosophy work consists essentially of elucidations.
The results of philosophy is not a number of "philosophy propositions"，
but to make propositions clear.
......
4.113 哲学划定自然科学可以争论的范围。
Philosophy limits the disputable sphere of natural science.
4.114 它应该划定什么是可思考的，因而也就划定了什么是不可思考的。
It should limit the unthinkable and thereby the unthinkable.
4.115 它将清楚地指出什么是可以言说的，从而也就指出了什么是不可言说的。
It will mean the unspeakable by clearly displaying the speakable.
4.116 凡是可以被思考的，都可以被清楚地思考；
　　　凡是可以被言说的，都可以被清楚地言说；
Everything that can be thought at all can be thought clearly.
Everything that can be said can be said clearly.
......
　　*******************************************************************************
　　小结4: 4　主要说明 --- 详细介绍了思想如何借助逻辑来反映世界；
　　　　　　　指出了哲学的目标就是澄清思想，哲学不是一个理论，而是一种活动。
　　　　　　　它应该划定什么是可思考的，因而也就划定了什么是不可思考的。
　　*******************************************************************************
　　第五节
5 命题是基本命题的真值函数
Propositions are truth-functions of elementary propositions.
......
5.6 我的语言的极限就是我的世界的极限
The limits of my language mean the limits of my world.
5.6.1 逻辑充满整个世界: 世界的极限也就是逻辑的极限所在
Logic fills the world: the limits of the world are also its limits.
......
我们无法思考的东西，我们就不能思考: 因此我们不能说出我们无法思考的东西。 
What we cannot think，that we cannot think；we cannot therefore say What we cannot think.
......
5.6.3 我即是世界。(小宇宙) 
I am the world。(microcosm)
　　说明: 吾心即是宇宙。 :-) 
　　*******************************************************************************
　　小结5: 5　主要说明 --- 进一步阐明了作者的逻辑原子主义立场
　　　　　　　指出人只有借助语言才能思考，主观只有借助语言才能反映客观世界
　　　　　　　语言的极限就是世界的极限
　　*******************************************************************************
　　第六节
......
6.41 世界的意义必定存在于世界之外。
The sense of the world must lie outside the world.
6.42 因而也就没有什么伦理学命题。
Hence also there can be no ethical propositions.
6.421 显然伦理学是无法表述的。 伦理学是超越现实的。 (伦理学和美学是一样的)
It is clear that ethics cannot be expressed.
Ethics is transcendental. (Ethics and aesthetics are one.)
......
6.432 世界如何存在，是与更高级的东西毫不相干的；上帝并不在世界中显现。
How the world is ，is completely indifferent for what is higher；
God does not reveal himself in the world.
6.44 世界如何存在，并不神秘；神秘的是它的存在本身。 
Not how the world is ，is the mystical，but it is.
......
6.52 我们感到即便是所有的自然科学问题都被解答了，生活的意义仍然未被触及丝毫。
当然是没有任何问题可言了，这就是答案。
We feel that even if all possible scientific questions be answered，then
the problem of life have still not been touched at all. Of course there
is then no question left，and just this is the answer.
6.521 生活的意义问题的解决办法就是消除该问题。
The solution of the problem of life is seen in the vanishing of this problem.
6.522 实际上存在着不可表达的东西；这显示了它的存在；它是神秘的。
There is indeed the inexpressible。 This shows itself；it is the mystical.
　　说明: 生活的意义是什么? 这个问题在维特根斯坦看来是不可言说的，是无法用现实世界的事实去实证的，因而是无意义的问题，应予以消除。
6.53 哲学的正确方法应该是这样的。 除了能够说的东西之外——如自然科学的命题，即与哲学无关的问题——什么都不说；然后当某人想说出一些形而上的东西时，向他指出它的命题中存在着一些毫无意义的符号。 这方法可能不太令人满意——他会感到我们不是在教他哲学——但这是唯一严格的正确方法。
The right method of philosophy would be this. To say nothing except what can be said，i.e. the propositions of natural science，i.e. something that have nothing to do with pilosophy: and then always，when someone else wished to say something metaphysical，to demonstrate to him that he has given no meaning to certain signs in his rpopositions. This method would be unsatisfying to the other---he would not have the feeling that we are teaching him philosophy---but it would be the only strictly correct method.
6.54 我的观点是以这种方式表明的：最终真正理解了我的观点的人，当他通过它们，越过它们并高于它们的时候。会发现它们是毫无意义的，(这样说吧，他必须在爬过梯子之后将其扔掉)
My propositions are elucidatory in this way: he who understands me finally recognises them as senseless，when he has climbed out through them，on them，over them.
(He must so to speak throw away the ladder，after he has climbed up on it)
　　*******************************************************************************
　　小结6: 6　主要说明 --- 阐明了作者的哲学观
　　　　　　　哲学的正确方法应该是这样的。 除了能够说的东西之外----什么都不说；
　　*******************************************************************************
　　第七节
7 对于那些不可言说的，必须保持沉默。
Whereof one cannot speak，thereof one must be silent. 
　　最后作者以一句意味深长的名言戛然而止。 
　　《逻辑哲学论》一书，先是开门见山地提出了作者作为一个"逻辑原子论"者的世界观。 然后作者进而提出了自己对哲学及哲学研究方法的见解。最后略带神秘气息的点睛之句结束：
　　“对于那些不可言说的，必须保持沉默。”
　　让我们做一个简单的总结吧。
　　在该书中，作者提出了如下的观点：
　　＊世界可以分解为事实 ( The world divides into facts.)
　　＊事实是由原子事实(atomic facts)组成的。
　　＊一个原子事实是多个对象(objects)的组合。 
　　＊对象是简单的(基本的) The Object is simple。
　　＊对象形成了世界的基础。 
　　即: 世界---事实----原子事实----对象，这就是我想指出的《逻辑哲学论》中的 OO。
　　当然，《逻辑哲学论》中除了这个OO的思想，还包含了更多内容。
　　请看后续文章《OO，OO之后，及其极限 5----OO之后是什么？》，及《OO，OO之后，及其极限 6------最后的极限》
　　< 未完，待续 >
5. OO之后是什么？
　　通过上节的讨论我们了解了维特根斯坦在《逻辑哲学论》一书中提出的OO的思想。即存在如下的认识链： 世界 → 事实 → 原子事实 → 对象(object)。本节我们讨论OO之后是什么这个问题。我们将从历史的和逻辑的角度来进行推理，看看能得出什么样的结论。
　　一、 从软件工程的发展历史来看
　　1969年NATO会议之后，“软件危机”成为人们关注的焦点。为迎接软件危机的挑战，人们进行了不懈的努力。这些努力大致上是沿着两个方向同时进行的。一是从管理的角度，希望实现软件开发过程的工程化。这方面最为著名的成果就是提出了大家都很熟悉的“瀑布式”生命周期模型。它是在60年代末“软件危机”后出现的第一个生命周期模型。如下所示。
　　分析 → 设计 → 编码 → 测试 → 维护
　　后来，又有人针对该模型的不足，提出了快速原型法、螺旋模型、喷泉模型等对“瀑布式”生命周期模型进行补充。现在，它们在软件开发的实践中被广泛采用。
　　这方面的努力，还使人们认识到了文档的标准以及开发者之间、开发者与用户之间的交流方式的重要性。一些重要文档格式的标准被确定下来，包括变量、符号的命名规则以及原代码的规范格式。
　　值得一提的是，在开发者之间、开发者与用户之间的交流方式这方面，由于Internet的出现提供了一种全新的交流手段，也产生了一种基于Internet的全新的开发方式，即OSS(Open Source Software)。 其代表作有Linux(操作系统)，Apeche(web server)，Sendmail(Mail server)等。OSS是一种极有前途的开发方式，借Internet发展的大潮，它势必会对整个软件开发模型产生难以估量的影响。近期的IEEE软件工程专刊将为次出一期专题特刊。1999年1月份也将举行关于Linux/OSS的国际研讨会。各家计算机公司纷纷对此作出反应，微软也意识到了来自OSS的巨大威胁，在8月底提出了一份内部保密的文件，专门提出了自己的应对策略，可谓是山雨欲来风满楼。有兴趣者请对此保持关注。
　　软件工程发展的第二个方向，侧重与对软件开发过程中分析、设计的方法的研究。这方面的第一个重要成果就是在70年代风靡一时的结构化开发方法，即PO（面向过程的开发或结构话方法）。 PO是人们在用计算机世界来表达现实世界时，追求过程话、模块化、封装以及更高的抽象的结果。 人们用计算机来映射现实世界时，最低层的实现无非是靠数字电路技术产生的高电平与低电平信号。用数学的语言来表示，就是像　010101000010111 这样的二进制串。这样的抽象层次是极低的，远离了自然语言，对一般人是不可理解的。人们把这些二进制串分块定义，提出了字节、ASCII码这样的更高抽象层次的概念，使之对应于自然语言的一个个字母。在此基础再借助某种形式语言，抽象出变量、表达式、运算、语句等概念。在这个层面上，一般经过训练的程序员已经可以比较不那么痛苦地进行软件开发了。下一步的抽象就产生了PO。 在PO中，人们关注的是如何用函数和过程来实现对现实世界的模拟，将其映射到计算机世界之中。 OO是这种抽象层次不断提高的过程的自然发展结果，它采用类和对象的概念，把变量以及对变量进行操作的函数和过程封装在一起，用这种更高一级的抽象来表达客观世界。通常，一个对象包含一些属性和方法，它对应于自然语言中一个有意义的名词，描述了现实世界中的一个物体（物理实体）或概念（抽象实体）。 这个抽象层次如下图所示：
　　计算机世界中的抽象层次
　　＊XO(X?-Oriented） 最高的抽象层次
　　＊OO(对象、类)
　　＊PO(过程、函数、变量)
　　＊变量、运算、表达式、语句
　　＊字节(4位、8位、16位、32位、64位)
　　＊二进制串 0101011110001 最低的抽象层次
　　从上图及以上的讨论我们知道，软件工程的发展历史就是人们不断追求更高的抽象、封装和模块化的历史。OO当然不会是历史的终结。尽管不能精确得到OO之后是什么，我们至少可以推知，OO之后的XO，必然将是比OO更高一级的抽象。它所依赖的核心概念必然高于并包容对象这一概念。正如对象高于并包容了函数和变量一样。
　　二、 从维特根斯坦在《逻辑哲学论》一书中的思想来看 
　　前面我们已经知道，维特根斯坦在《逻辑哲学论》 一书中提出了如下思想：
　　＊世界可以分解为事实 ( The world divides into facts.)
　　＊事实是由原子事实(atomic facts)组成的。
　　＊一个原子事实是多个对象(objects)的组合。 
　　＊对象是简单的(基本的) The Object is simple。
　　＊对象形成了世界的基础。 
　　即： 世界---事实----原子事实----对象这样一个从整体到局部、从抽象到具体的认识之链。在这个层次中，对象作为最基本的模块，构成了整个认识大厦的基石。对象通过相互之间的复杂的关联构成了整个世界。这个观点也是面向对象理论的基石。
　　上一段我们通过对计算机世界的分析得出，OO之后的发展，必将产生比OO更高一级的抽象的概念用于编程。按照维特根斯坦的思想可以很自然地得出结论：这个概念就是事实(FACT)。即面向对象之后是面向事实，OO之后是FO（Fact-Oriented）。
　　那么，什么是事实(fact)？让我们再次重温一下维特根斯坦的观点。
　　首先，世界是所有事实的总和。世界可以分解为事实。任一事实或者为真，或者不为真。（即事实应返回一个布尔变量，或为TRUE，或为FALSE）
　　其次，那些为真的事实，是由原子事实(atomic facts)组成的。
　　原子事实和简单对象的关系是这样的：一个原子事实是多个对象(object)的组合。在原子事实中，对象以某种确定的方式相连。原子事实中对象相互连接的方式即是原子事实的结构。 也就是说，原子事实是有结构的，这种结构被维特根斯坦看作是现实世界的模式(form)。
　　这里，维特根斯坦提出了几个重要的概念：事实(fact)、原子事实(atomic fact)和模式(form)。
　　如果我们以上的推理过程正确的话，我们将得出一个结论：面向对象(OO)之后是面向事实(FO)，在面向事实(FO)的分析、设计和编程中，我们将主要围绕事实(fact)、原子事实(atomic fact)和模式(form)这些基本概念来组织我们的软件开发活动。在人们认识世界的层次结构中，这些概念是比对象(object)和类(class)更高一级的抽象。
6. 最后的极限
　　OO之后如果FO，那么我们当然要问，FO之后又是什么？再往后哪？有没有一个最后的极限？若有，极限何在？这是个很困难的问题，不过还是让我们继续思考下去，看看能有些什么结果，尽管它可能会是十分愚蠢和幼稚。
　　我们知道，软件的实质是人们以计算机编程语言为桥梁，将客观感知世界映射于计算机世界中去，以解决人们在客观感知世界中要解决的问题。这里牵涉三个主要的范畴：
　　客观感知世界--------计算机编程语言----------计算机世界
　　(1)计算机世界
　　在计算机世界中，可以想象的是，单位面积的一块芯片上可以集成的晶体管数目总会有个上限，摩尔定律早晚要失效。另外，现在多数电脑采用的冯.诺依曼结构也需要被突破。（如并行计算的研究）
　　(2)计算机编程语言
　　现在的计算机编程语言，都是某种类型的形式语言。要解决一个问题，首先我们必须为它构造算法。也就是说，对于没有算法的问题，我们是束手无策的。这类问题被成为是“不可计算的”。它们的解不能由图灵机来产生。这是我们面临的一个极限。
　　那么一个问题有了算法，是不是就一定可以解决哪？很遗憾，答案是不。对于那些NP（非多项式）问题，尤其是NP完备的问题，尽管存在算法，我们仍然是无法在多项式时间内解决的。著名的“哈密顿线路问题”和“旅行推销员问题”都属于这类问题。它们的共同特征是，当问题的规模（或复杂度）线形增加时，解决该问题所要化的时间将呈指数上升，以至于它们尽管理论上是可求解的，但实际上却是不可能的。因为要花费的时间可能已经超过了宇宙的寿命。这也是我们面临的一个极限。
　　还需要指出，现在的计算机语言作为形式语言的一种，也必然具有形式语言自身所有的局限性。数学的发展史上，曾经有三次危机，第三次危机是有罗素的一个“佯谬”引发的悖论危机。为解决此危机，希尔伯特提出了宏伟的形式化规划，企图使数学一劳永逸地处于无可争辩的安全基础之上。可是，1931年奥地利25岁的天才数学逻辑学家歌德尔提出了一条划时代的定理，粉碎了希尔伯特形式化的美梦。哥德尔定理向我们展示，公理系统本身的协调性的陈述在被编码成适当的算术命题后，将成为一道这样“不能决定的”不能决定的命题。想象一个表达式既不为真，也不为假，它的真假对一个形式系统（计算机）而言它是不可判定的。因此在这方面，哥德尔定理是我们面临的又一个极限。
　　(3)客观感知世界
　　现在让我们假设软件工程已经发展到了这样一个理想的境界，有一天我们实现了自然语言编程，是否就万事大吉了？换句话，自然语言是否能很好地描述、表达客观感知世界？
　　维特根斯坦在《逻辑哲学论》里已经指出了。
　　＊世界的意义必定存在于世界之外；
　　＊显然伦理学是无法表述的。 伦理学是超越现实的；
　　＊实际上存在着不可表达的东西；这显示了它的存在；它是神秘的。
　　（见《逻辑哲学论》第六节）
　　也就是说，外部世界中存在一些我们可以感知却无法用语言来表达的东西。
　　他接着说：
　　“对于那些不可言说的，必须保持沉默。”（Whereof one cannot speak，thereof one must be silent）
　　这句话，成为我们最后的极限。
　　在讨论完这三个范畴中我们将面临的主要问题后，我们最后来讨论一下人的主观世界对软件及软件开发的影响。
7. 极限之外的空间
　　世界可以分为主观想象世界和客观可感知世界，或者说是内世界和外世界。其中，主观想象世界是计算机从未涉足的空间。这是属于我们个人的私有空间，充斥着我们自己离奇古怪的幻想和起伏不定的情绪。它具有极大的跳跃性和不确定性，常常以直觉、想象的方式进行着自己的活动。它们通常是语言所不能表达的。
　　尽管这个内世界是如此的复杂、混乱、难以捉摸，但它却是我们与生俱来的一部分。
　　我们就是这样的人。
　　就是这样的人从事着软件开发的工作。
　　这样的人开发出来的软件究竟在多大程度上是对客观世界不折不扣的映射？还是这个映射过程已经被扭曲或掺杂了？软件开发者的这个主观想象世界，他们的欢乐、沮丧、奇想、恐惧、希望等等，究竟是怎样影响着软件开发的质量？
　　人们是要用计算机来复制整个世界？还是来复制人本身？
　　人们是在扮演上帝的角色吗？
　　这样的问题我实在不知该如何去解答，甚至觉得根本无从下手。
　　如果你还有足够的耐心读到这里，请你也来想一想，希望它们不会使你头疼。
　　:)
　　最后，我将引用我从网上偶尔发现的一篇文章中的话作为结束语：
　　“自以为是的立法者(人类)犯下了一个大错，以为自己能定义整个软件开发过程。他们既不了解其开始也不了解其结果。学术界试了一下，然后就知难而退了。商业界则对其毫无办法。大型软件开发商们斥巨资来制造代码，明明想要扮演上帝的角色，却还装作自己是人类的公仆。所有人都在盯着镜子里的自己。计算机却在一旁偷偷的笑。”
　　“ The self-righteous，legalists have made a big mistake in thinking they can define the software development process. They understand neither its beginnings or its consequences. The academic community tried it briefly and then gave up. The business community never cracked the code. The giant software houses are pouring enormous energy into process of creating code，playing God，and pretending to be servants of mankind. All are looking at themselves in the mirror. The computer is laughing back.” 
----《完》----
后记：
　　大概4月份的时候，开始写这个题目，一口气写了4篇，贴到水母清华的软工板上。后来主要因为懒，就停了下来。多亏有几位网友来信催促和鼓励，才使我有机会逼自己坐下来完成最后的3篇。这里特向他们表示感谢！ 尤其要感谢seraph和xiaobo。
　　看屏幕上的字的确是件累人的事，所以如果你竟然有耐性看到了这里，我一定要向你表示佩服了。 :)
　　最后说明，本文不是一个正式版，只是一个学习和思考的心得体会，行文仓促，也没有仔对照细修改。文中的错误和不恰当的翻译均由我全部负责，欢迎指正。
AKA的waterbird
1998年10月7日0点29分
--------------------------dhcn读后按：本来想联系作者说这些的， 时代久远，凭一个ID也很难找到了。关于作者最后的FO的假设，我觉得作者一是对维特根斯坦的逻辑分析哲学面向的基本问题有误会，而是对这个东西基本的发展方向判断有误，一个更普适更抽象的范式，应该是更精微的。而且是很难世出的学术进展。

