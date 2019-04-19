# 关于mock - u013366022的专栏 - CSDN博客
2019年04月09日 11:38:38[slitaz](https://me.csdn.net/u013366022)阅读数：51
[Home](https://lelouchhe.github.io/)
[吐槽](https://lelouchhe.github.io/voice)[源码研究](https://lelouchhe.github.io/code)[我的](https://lelouchhe.github.io/mine)[算法挑战](https://lelouchhe.github.io/algorithm)[网络](https://lelouchhe.github.io/network)[科学吧](https://lelouchhe.github.io/science)[文青](https://lelouchhe.github.io/literature)[关于我](https://lelouchhe.github.io/about)
# TDD笔记2
08 Jul 2014 by [LelouchHe](https://lelouchhe.github.io/about.html)
## 关于mock
个人感觉mock是测试中一个比较掌握的点,所以试图单独拿出来说明下
## mock的引入
在开发过程中,会遇到各种各样的依赖关系.如果依赖的仅仅是local的类还好说,至少之间的交互最复杂也莫过于进程间rpc调用了,普通的也就是函数调用的,这些调用一般都比较快,而且稳定,返回结果一般也是可控的.相反,还有一些依赖关系是不可控的,最local的比如本地文件系统的操作,稍微远点的比如远程数据库访问,下游依赖服务调用之类的,这些依赖关系存在3个缺点:
- **响应慢** 本地也好,网络连接也好,相比函数调用而言,都是耗时的复杂操作,测试能被很好的使用,一个标准就是要快(大型c++编译本来就够慢了,测试再慢,活不活了)
- **不稳定** 依赖的服务无法保证在测试期间的稳定性,如果test失败了,究竟是逻辑有误,还是仅仅服务不稳定呢?这个就加大了测试分析的难度(可能就还得看操作日志之类的)
- **无定制** 无法控制服务返回的结果,很多逻辑都无法进行全面测试.比如想看看文件系统出错时的处理,这种情况要想依赖os本身来弄,概率基本可以忽略了,所以这部分逻辑就相当于没有测试过
所以祭出软件开发中的大杀器–**间接层**,将实际的逻辑处理和依赖服务隔离,然后就可以在中间动动手脚了
比如,用一个假的依赖服务模块替换掉真实实现,测试的时候就可以主要关注上层逻辑的处理,需要的依赖服务的结果可以随心所欲的定制,因此各种千奇百怪的情况都可以模拟
这样的一个虚假依赖服务模块,就是一个真实服务的mock,它的引入,就是为了解决上面的3大问题
## 关于mock的疑问
话虽说的好听,但想要达到mock的效果,不仅对系统设计会带来影响,对于模拟的真实服务,也是有一定的问题的
### 系统设计
可以看到,如果要引入mock,那么必然就需要引入新的间接层了,上层逻辑完全的依赖**注入**的接口,下层的实现在真实与mock之间切换,就可以完成对逻辑层面的测试
但这样是存在问题的:
- **有无必要** 基于接口,这是开发的基本原则,但如果对应的实现只有一个的话,很多情况下我们就直接忽略了接口,完全的依赖于唯一的实现了(比如我们内部依赖的MySql和curl之类的).引入间接层,大部分情况下是引入虚的继承结构,性能另说,单就这一个接口只对一个实现,颇有浪费的感觉
- **依赖注入** 依赖的底层服务,是需要注入到逻辑层的使用中,不论是通过初始化,或者随后的set,表示逻辑层,至少和底层服务交互的这个层次,是需要修改结构的.如果依赖的服务很多(比如cache,数据库,curl,tcp连接,配置文件等),这些注入就是问题了
其实在写完上面的问题后,我就了然了,这些问题之所以会存在,应该不是mock带来的,而是系统设计本身有问题,比如基于接口编程,在这里变成了基于实现,bad smell,再比如多依赖,从名字上就违反了单一职责原则(SRP),是应该拆分的,而拆分之后不仅架构上进步,也更容易进行注入测试了
当然,如果是基于历史代码的话,就是另一回事了,这个后续应该还会提到.但不难看出来,从测试的角度出发,是会强迫我们采用一些比较好的设计方案的,这应该也是TDD的关键目标吧
### 定制结果
底层服务有时并不是简单的返回字符串或值,有的时候是非常复杂的二进制格式,虽然是可定制的,但如何定制,怎么定制,同样是个复杂的工程
就比如我们自己的服务,上下游之间的结果至少是k数量级的,里面各种字段各种复杂,有历史的,也有随意添加的,还有不明所以的,试图拼装这样的结果是比较痛苦的,所以测试的时候,一般就偷懒,直接连接测试或线上的真实服务了
一个好的解决方法估计是采用合理的通信协议(伤过心之后才知道这个有多重要),不论是protobuf,thrift还是json,xml,这些其实都不是重点,这些都是手段,是关于如何通信的,真正的关键是通信内容,这点上可能目前还没有太深的研究,还需要进一步的学习
不过这个方法,对于历史系统是无能为力的,此时,可能首先获取一个真实结果,再对其进行适当修改,应该是个还可以的应对方式
如果服务结果本身就不是确定的,或者逻辑层的处理不是确定的,比如都引入了随机性,那么在最后断言时,就需要小心的.这个可以从系统层面解决,将随机性作为依赖的一种注入即可,测试中可以使用确定的随机性mock即可,这是后话了
### 总结
可以看到,mock的种种问题,其实都反映了系统本身存在的问题,对于历史系统,可能就要走上测试-重构之路,对于新系统的话,就完全需要按照TDD循环,以期得到方便测试,同时也设计良好的架构
注意测试在二种情况下扮演的角色,后续还会详细讨论的
## mock的定位
测试時使用mock要记住的一点是,这不是针对mock的测试,而是针对**直接依赖**于mock的上一层的测试,测试的是其同mock之间的交互逻辑
有一段时间,我一直误认为谁做mock,就是测试谁,这个想法是错误的,而且mock的话,不能做为对整个系统服务的测试,而仅仅是针对**直接依赖**的测试,再往上层的话,可以进一步的mock上层对象,或者往系统层面看做集成测试之类的
一定要记住测试对象–**直接依赖**
## 手工mock
mock并不是说一定需要gmock之类的框架才能完成,按照mock的本意,我们大可以手动编写所有的mock
比如如果直接依赖的服务接口是Foo,如下:
- `class Foo {`
- `public:`
- `Foo();`
- `virtual ~Foo();`
- 
- `virtual int some(int a) = 0;`
- `};`
真正实现和mock应该都是继承于Foo的不同实现(一个是真实数据真是逻辑,一个就是mock出来的),比如:
- `class RealFoo : public Foo {`
- `public:`
- `RealFoo();`
- `~RealFoo();`
- 
- `virtual int some(int a) {`
- `// some real job`
- `}`
- `};`
- 
- `class MockFoo : public Foo {`
- `public:`
- `MockFoo();`
- `~MockFoo();`
- 
- `virtual int some(int a) {`
- `// all the weird situations`
- `}`
- `};`
当上层服务试图使用Foo接口時,利用依赖注入的技术,将MockFoo的实现传入,这样,我们就能控制Foo接口的返回,从而测试上层服务对于Foo接口各种情况的使用了
问题的关键在于**依赖注入**和**逻辑实现**
### 依赖注入
我们实现了mock,但上层逻辑中使用不到,也是白搭.一个简单的思路可能是在上层模块中直接使用mock,比如:
- `class Bar {`
- `public:`
- `Bar();`
- `~Bar();`
- 
- `void any() {`
- `shared_ptr<Foo> f = make_shared<MockFoo>();`
- `int a = f.some(1);`
- `// rest`
- `}`
- `};`
但这样的问题在于,Bar是真正实现的代码,如果把仅用于测试的MockFoo加入其中,之后正式提供服务時,怎么办?再删除么?但凡有改动,肯定就有可能引入新的bug
但就Bar的实现而言,其实是非常不好的,Bar明明使用的是接口Foo,但却明确的指名了依赖的额实现MockFoo,显然违背了基于接口编程的意义,所以这地方本身就是需要重构的
构造函数注入/set注入
一个常见的依赖注入的方式,是从构造函数入手,即重构称如下:
- `class Bar {`
- `public:`
- `Bar(shared_ptr<Foo> f) : f_(f) {}`
- `~Bar();`
- 
- `void any() {`
- `int a = f_.some(1);`
- `// rest`
- `}`
- 
- `void set(shared_ptr<Foo> f) {`
- `f_ = f;`
- `}`
- 
- `private:`
- `shared_ptr<Foo> f_;`
- `};`
一般来说,从构造函数注入是比较简明良好的方式,一是我们不会忘记这个依赖参数(否则编译器就不会让我们好过),二是构造函数入口少,而且但凡构造,必然会调用,省去再调用一次set(比如我们忘了调用,然后就悲剧了)
当然,对于无法使用构造函数的场景,比如Bar要能放到容器中,需要存在默认构造函数的,或者需要多次变换依赖实现的,就只能提供set接口了,此时能做的,就是在测试依赖于Bar的模块時,多多mock,确保逻辑的正确性(不要忘记调用set之类)
这种方式比较明了,接口的对应实现是我们手工传递的,实现代码会比较简单,而且不会引入多余代码,原先的代码也会完全覆盖执行,没有任何的测试漏洞
工厂注入
另一种可能的方式是,Bar的实现采用工厂模式,即每次不是直接使用f_,而是通过工厂函数/类來完成,比如
- `class Bar {`
- `public:`
- `Bar();`
- `~Bar();`
- 
- `void any () {`
- `shared_ptr<Foo> f = NewFoo();`
- `int a = f.some(1);`
- `// rest`
- `}`
- 
- `protected:`
- `virtual shared_ptr<Foo> NewFoo() = 0;`
- `};`
上面是一种可能的实现(工厂函数,可以是全局函数,也可以是类本身的函数,看实现而定),真正的RealBar集成自Bar,并实现对应的NewFoo接口即可
这样的解决方案也是很好的,特别是如果Bar原先的实现本身就是利用工厂來完成的,测试中简单的继承一下即可
缺点在于RealBar和专门测试用的TestBar的代码有了区别,就算测试通过,也只能说明TestBar是OK的,二者的差别代码是没有测试的.所以这就要求NewFoo代码非常简单,简单到决定不会有问题,比如
- `class RealBar : public Bar {`
- `protected:`
- `virtual shared_ptr<Foo> NewFoo() {`
- `return make_shared<RealFoo>();`
- `}`
- `};`
- 
- `class TestBar : public Bar {`
- `protected:`
- `virtual shared_ptr<Foo> NewFoo() {`
- `return make_shared<MockFoo>();`
- `}`
- `};`
注意,此处的TestBar不算是mock,最多算是为测试准备的fake,作用不是拿来测试**直接依赖**于Bar的模块(这是mock的真正含义),而是引入Bar**直接依赖**的Foo的mock实现
可以看到,此时的测试代码就比较多了,有MockFoo和TestBar两个
另,这只是工厂实现的一种,同样也可以采用工厂类的方式,再采用构造注入或set注入的方式,就同第一种类似了,只不过我们就增加了一个间接层而已
接口注入
如果模块接口可改的话(这种情况不多),也可以修改any类型,直接调用時注入:
- `class Bar {`
- `public:`
- `void any(shared_ptr<Foo> f) {`
- `int a = f.some(1);`
- `// rest`
- `}`
- `};`
这种情况应该是不常见的,相比较而言,构造函数注入可以通过工厂模式掩盖起来(Bar和BarFactory一般都是一个模块的),但public接口的改动很难不被上层模块发现的,所以这种方式只对非常简单,一次性的实现可行
模板注入
除了利用虚函数达成运行期mock,还可以利用c++特有的模板,來完成编译期mock,不过这个对原有模块的要求就更高了(首先,开发得熟悉模板,其次,得有非使用模板的理由,最后,会用模板,别用错了).比如
- `template<class F>`
- `class Bar {`
- `public:`
- `Bar();`
- `~Bar();`
- 
- `void any() {`
- `F f;`
- `int a = f.some(1);`
- `// rest`
- `}`
- `};`
然后在真实环境下和测试环境下,分别使用不同类型进行特化即可:
- `// 真实环境`
- `Bar<RealFoo> b;`
- 
- `// 测试环境`
- `Bar<MockFoo> b;`
当然,很有可能不是这么直接的特化,转个弯利用工厂进行封装是个不错的选择
在这种情况下,MockFoo和RealFoo就不在是接口Foo的子类了,而是同等级的实现类,接口Foo也不存在于真实代码中,而只是一个Concept或硬性规范了(缺失对应接口,编译期会直接error的)
个人感觉没有很特别的原因,选择模板方式,是有很大风险的,不过如果模块本来就是这么设计的,那采用这个方式也是无可厚非的
### 逻辑实现
上面提到了如何在**不修改原先代码**的基础上,将mock引入系统中.不修改代码,并不是说真的原封不动,针对历史系统的重构可能是不可避免的,对于新系统,一个良好的架构设计也是必须的,这里的**不修改源代码**的意思是,重构或设计之后的代码,是要全部运行的,不能测试的时候一套代码,正式服务又是一套(所以说,工厂注入有些瑕疵)
已经注入之后,剩下的就是如何利用mock來模拟各种情况了,还是以Foo为例,其接口some可以返回很多值,每个值有不同的解释和处理,此时如何來测试呢?
定义多个mock类
一个初级的想法可能是,针对每个返回值都建一个mock,比如
- `class MockFoo1 : public Foo {`
- `public:`
- `virtual int some(int a) {`
- `return 1;`
- `}`
- `};`
- 
- `class MockFoo2 : public Foo {`
- `public:`
- `int some(int a ) {`
- `return 2;`
- `}`
- `};`
这种方式的缺点就是太初级了,有多少种情况,就需要多少个不同的类,大家长的都差不多,就是返回可能略有区别.有太多冗余工作了,一场测试下来,至少十几个是没得跑了
参数化
将上一个方法升级下,不通过类來进行区别,而是通过调整对象的状态,返回合适的值.一个可能的实现方法是:
- `class MockFoo : public Foo {`
- `public:`
- `int n_;`
- 
- `int some(int a) {`
- `return n_;`
- `}`
- `};`
这样,我们想返回多少,就可以通过public來设置多少,这样的好处是只用定义一个类,缺点就是如果mock多个接口的话,就需要有多个public成员变量,每个都需要单独设置才行.当然,每个test也只应该测试一个行为,所以每个test内部应该还是可以接受的
多说一句
为什么单纯的设置接口返回就可以了呢(某些接口可能还有out参数,这个也可能需要修改)?接口实现中复杂的逻辑和交互取哪里呢?
这个问题困扰了我很久了,因为我一直不相信这样就可以了(完全等于没有实现逻辑嘛..).但在进一步的学习中,发现一个道理,这不就是**基于接口编程**的原则么?
也就是说,我不管接口是怎么实现的,只要给定输入,然后给我返回对应输出即可,内部原意是复杂逻辑也好,简单逻辑也好,甚至是每次调用都会打电话给其他rd,让他们手工來跑数据也好,上层是不关心的
输入–>输出
就是接口的全部
(可以看到,gmock的话两个部分都可以控制,非常的强大)
## gmock框架
从上面可以看到,手工mock最大的缺点是工作量大且重复,这样的工作应该交给自动化的框架來处理,下面就以gmock为例來讲解
需要注意的一点是,gmock框架和手工mock没有本质区别,只不过gmock帮我们完成了手工mock的大部分重复操作,而且经过严格测试,保证是没有太大问题的
### 定义mock类
定义mock类和手工mock時类似,只不过需要使用特殊的**MOCK_METHODxx**宏來标注需要测试的接口,同时,不需要提供对应的实现
- `class MockFoo : public Foo {`
- `public:`
- `MOCK_METHOD0(some, int(int));`
- `};`
MOCK_METHODn(name, type),n表示的是接口参数的数量(如果没有就是0),name是接口名称,type是接口的类型(看一下例子就好).mock类中我们关心且需要的接口,都必须使用这个这个宏來定义
此外还有专门定义const函数的MOCK_CONST_METHODn和模板函数的MOCK_METHODn_T,用法和上面那个类似
这里面唯一需要注意的是,如果父类存在相同名称接口重载的情况,我们一般是都需要mock一遍的,否则会有warning说某些接口被隐藏(名称空间覆盖的问题),当然,如果我们确实只对一个感兴趣,可以使用using语句引入父类接口(不过对测试来讲,重载接口应该都需要测试):
- `class MockFoo : public Foo {`
- `public:`
- `using Foo::some;`
- `MOCK_METHOD0(some, int(int));`
- `};`
此处并不需要我们手工來实现这些mock的接口(否则,和手工mock有啥区别),相反,在test中,通过gmock提供的一系列方便的接口,來控制这些接口的行为.这样的好处一是不需要我们编写重复的代码了(想想手工mock時),二来我们只关心接口的输入和输出,从而更快速的來测试依赖于其上的模块代码
### 依赖注入
gmock并没有提供依赖注入相关的接口,所以还是需要按照手工mock的处理,重构历史系统,或者直接以TDD的方式构建良好架构的新系统
依赖注入应该是个比较复杂的问题,现有的一些框架并不能很好的解决c++的相关问题,但这个归根到底还是系统设计问题,这种事情还是自己手动來比较好,毕竟设计的东西,没有一个定值,都是在各种trade off之间权衡
### 逻辑实现
上面提到过,每个test中mock接口的实现,是通过gmock提供的一系列接口完成的,由于接口提供的比较丰富和精准,我们就完全不需要像手工mock一样提供复杂而冗余的实现
主要有下面几个接口
EXPECT_CALL
EXPECT_CALL是gmock提供的一个定制接口逻辑的入口,其基本形式有:
- `EXPECT_CALL(mock_object, method(matchers))`
- `.With(multi_argument_matcher) ?`
- `.Times(cardinality) ?`
- `.InSequence(sequences) *`
- `.After(expectations) *`
- `.WillOnce(action) *`
- `.WillRepeatedly(action) ?`
- `.RetiresOnSaturation(); ?`
后面的?表示最多可以出现1次,*表示可以出现任意次
EXPECT_CALL,如其名字一样,是描述对接口调用的期望的,符合期望,就会成功,否则,就当作失败处理.其后跟着的许多参数都是用来进一步的描述期望细节的
mock_object/method/matchers
接口调用的主体是这三个参数,分别表示:
- **mock_object** 调用的对象
- **method** 该对象调用的方法
- **matchers** 调用参数符合matcher的要求(一种谓词判断)
综合起来,就是说,针对该mock_object的method调用,且参数满足matcher.比如
- `TEST(Bar, Any) {`
- `MockFoo f;`
- `EXPECT_CALL(f, some(1));`
- 
- `Bar b(f);`
- `b.any();`
- `}`
这里期望的调用就是f.some(1),如果有该形式的调用,EXPECT_CALL会成功,否则就会失败,比如实际调用的是f.some(2)
matcher有两种类型,一种是基本类型,比如此处的1,表示必须完全相同才能匹配;另一种是仿函数Functor,使用事先定义好的用来判断关系,比如Eq,Gt之类的(很类似gtest中的matcher),当然,也有自定义的方式,这个后说.比如:
- `TEST(Bar, Any) {`
- `MockFoo f;`
- `EXPECT_CALL(f, some(Gt(1)));`
- 
- `Bar b(f);`
- `b.any();`
- `}`
此时,只有调用f.some(a),a>1时,才会是成功的test,否则就失败
gmock还提供一个通用的matcher,testing::_用来表示所有参数,即使用任意参数调用均可.还有一种指定类型的通用匹配,即testing::A()或testing::An(),表示匹配type类型的任意参数
一个接口可以配合多个EXPECT_CALL一起使用,判断顺序是声明顺序的逆序(类似atexit的调用顺序),通常越通用的越靠前声明,越特殊的越靠后.如果多个EXPECT_CALL匹配,那么由下向上就行选择,最新声明的优先级最高(看,都是逆序)
但这个并无法规定调用的顺序,比如
- `TEST(Bar, Any) {`
- `EXPECT_CALL(f, some(1));`
- `EXPECT_CALL(f, some(2));`
- `}`
无法保证究竟是f.some(1)还是f.some(2)先调用,测试通过只是保证二者均会被调用,但顺序不明.如果要强行指定顺序,就需要用到InSequence的对象来保证,它会保证同它在一个生命周期的EXPECT_CALL会严格按顺序调用,否则就失败
- `TEST(Bar, Any) {`
- `{`
- `InSequence dummy;`
- 
- `EXPECT_CALL(f, some(1));`
- `EXPECT_CALL(f, some(2));`
- `}`
- 
- `{`
- `InSequence dummy;`
- 
- `EXPECT_CALL(f, some(3));`
- `EXPECT_CALL(f, some(4));`
- `}`
- `}`
此时,1/2必须顺序调用,3/4必须顺序调用,但这二者之间的调用关系是不定的(后面还有一些相关的介绍)
With
With的作用和上面提到的matcher类似,都是用来指定接口调用時匹配的参数的,只不过matcher只能单独的指定各自的参数,而With可以指定参数之间的关系,当需要测试的接口参数有特殊要求時,就可以通过With來指定
一般而言,参数关系有2种指定方式:
- **全指定** 即所有参数,从左到右的顺序关系,比如.With(Lt()),就是表明参数依次要小于后面的参数.或者可读性更高点,使用.With(AllArgs(Lt())),也是表达类似意思,只是**AllArgs**可读性更好些(所以推荐后者)
- **单独指定** 即可以单独确定参数间关系,通过Args<a, b, …, n>(matcher)的方式,意思是参数a/b/…/n之间的关系是matcher指定.比如.With(Args<0, 1, 2>(Lt()))就是说参数0/1/2各自小于后面的参数(即argv[0]<argv[1]<argv[2]).Args中间的模板参数是可变的,所以很灵活.如果有多对关系的话,需要用AllOf将所有matcher包含进去,如.With(AllOf(Args<0, 1>(Lt()), Args<1, 2>(Gt()))),就表示argv[0]<argv[1],且argv[1]>argv[2]
需要注意的是,参数的下标是从0开始的
当然,matcher也是可以定制的,原型为`bool Predicate(const testing::tuple& args)`,所有的参数都是打包到这个tuple中的(定制属于比较高级的用法,有机会再讨论)
另,这些叫做matcher,在gmock/gtest内部是通用的,还记得gtest在ASSERT_THAT中用到的matcher么?二者是一样的,比如
- `ASSERT_THAT(1, AllOf(Gt(0), Lt(2)));`
这里使用到了AllOf,类似的还有:
- **AllOf(m1, …, mn)** m1,…,mn必须同时满足
- **AnyOf(m1, …, mn)** m1,…,mn至少有1个满足
- **Not(m)** 不满足m
Times
Times主要用来指定调用次数,如果没有指定,默认情况是调用1次.如果test中调用次数和指定的不一样,test就会失败
Times的参数大体有4种格式:
- **n(数字)** 表示该接口调用只能调用n次
- **AnyNumber()** 表示接口可以调用任意次
- **AtLeast(n)/AtMost(n)** 表示接口可以调用至少/至多n次
- **Between(m, n)** 表示接口可以调用m-n次之间
InSequence/After
上面曾经提到,要想保证调用之间的顺序,需要使用`InSequence dummy`,那么在其生命周期内,一起的EXPECT_CALL就是顺序
但有的时候接口调用之间只是偏序而已,比如有的类,初始化是必须的,但其后接口的调用顺序都是各自独立的,那么使用刚才讲的方法就有问题.此时,就可以使用EXPECT_CALL中的InSequence/After來指定偏序
InSequence是将接口调用绑定到不同的Sequence对象上,同一个Sequence对象上是有序的,不同的则相互独立,比如:
- `TEST(Bar, Any) {`
- `Sequence s1, s2;`
- 
- `EXPECT_CALL(f, some(1))`
- `.InSequence(s1, s2);`
- `EXPECT_CALL(f, some(2))`
- `.InSequence(s1);`
- `EXPECT_CALL(f, some(3))`
- `.InSequence(s2);`
- `EXPECT_CALL(f, some(4))`
- `.InSequence(s2);`
- `EXPECT_CALL(f, some(5));`
- `}`
这个test中,调用顺序就是f.some(1)->f.some(2), f.some(1)->f.some(3)->f.some(4), f.some(5),三个顺序
InSequence是通过指定全部顺序的方式,來确定接口调用的顺序的,而After则通过指定局部关系的方式來确定顺序,即指定本接口调用在某个接口调用之后再调用才可以
每个EXPECT_CALL都是一个Expectation对象,在After中填入对应的Expectation,即表明本调用需要after对应expectations的调用.比如
- `TEST(Bar, Any) {`
- `Expectation x = EXPECT_CALL(f, some(1));`
- `Expectation y = EXPECT_CALL(f, some(2));`
- 
- `EXPECT_CALL(f, some(3))`
- `.After(x, y);`
- `}`
就表明f.some(3)的调用需要在f.some(1)和f.some(2)之后才算成功,否则就是失败
如果需要After多个接口调用,除了将所有调用都写到After参数中外,还可以利用接口调用集合來完成,上面的例子可以简化为:
- `TEST(Bar, Any) {`
- `ExpectationSet xs;`
- `xs += EXPECT_CALL(f, some(1));`
- `xs += EXPECT_CALL(f, some(2));`
- 
- `EXPECT_CALL(f, some(3))`
- `.After(xs);`
- `}`
WillOnce/WillRepeatedly
上面提到了参数匹配,调用次数和顺序,还有一个很重要的方面,没有涉及,即接口的返回(还记得接口的输入–>输出么?),WillOnce/WillRepeatedly和参数中的action就是定制接口的动作的
先问一句,以前我们不写这两个,接口也是有返回的(否则编译就报错了),此时的返回是什么?类似C++中全局或静态变量的初始化,都是默认值.内置类型的话基本就是0/false/NULL,类类型的话应该就是默认构造函数构造的值.这个默认值是可以定制的,不过在EXPECT_CALL层面的话,只能根据返回值类型來定制(另一个接口ON_CALL是可以根据调用來定制,后说)
- `TEST(Bar, Any) {`
- `DefaultValue<int>::Set(1);`
- `EXPECT_CALL(f, some(_));`
- 
- `DefaultValue<int>::Clear();`
- `}`
此时,f.some(_)没有定制动作,返回默认值,新的默认值为1.需要注意的是,Set/Clear是必须配对使用的,缺一不可
再回到WillOnce和WillRepeatedly.二者其实是一样的,只不过WillOnce只是定制一次,调用一次之后就无效了,而WillRepeatedly则是定制剩余的所有次数(也可以看出来,WillRepeatedly最多出现一次).而且,同一个接口调用中,这些动作的执行顺序是从上到下的(也可以看出来,WillRepeatedly必须出现在动作的最后),比如:
- `TEST(Bar, Any) {`
- `EXPECT_CALL(f, some(1))`
- `.WillOnce(Return(1))`
- `.WillOnce(Return(2))`
- `.WillRepeatedly(Return(3));`
- `}`
上面意味着,对于f.some(1),第一次返回1,第二次返回2,剩余调用返回3(先姑且认为Return就是返回值呗)
这两个和Times有时会有冲突,比如规定了Times(1),但写了许多WillOnce.按照gmock的 规定,Times是必须放在前面的,而且优先级较高,多余的WillOnce会被忽略的
action的类型有很多种,很多类型都有后置**Ref**或**Pointee**的变种,表示参数为引用或指针的值
- **ReturnXXX(x)** 返回x.注意,返回的值是当时x的值,x之后的变换不会影响.有变种,表示返回值为引用或指针指向值
- **ReturnArg()** 返回第n个参数值
- **ReturnNew(a1, ..., an)** 返回new T(a1, ..., an),每次返回都会new一个
- **SetArgXXX(v)** 用v设置第n个参数值.只有变种存在,表示第n个参数是引用或指针
- **SaveArgXXX(p)** 将p指向的值设置为第n个参数的值.有变种,表示第n个参数的引用或指针指向值
- **SetArrayArgument(begin, end)** 设置第n个参数指向的数组的值为[begin, end)
- **Invoke(f)** 使用mock的参数来调用f(全局/静态函数,或Functor)
- **Invoke(&object, &class::method)** 使用mock的参数调用object.method.上面2个都有**InvokeWithoutArgs**的变种,表示无参数调用
- **InvokeArgument(a1, ..., an)** 使用参数a1,...,an调用第n个mock参数指向的函数
- **DoAll(a1, …, an)** 从动作a1顺序执行到an
- **WithArg(a)** 使用第n个mock参数调用a
- **WithArgs<n1, …, nk>(a)** 使用第n1,…,nk个mock参数调用a
- **WithoutArgs(a)** 不使用参数调用a.上面3个存在,主要是和Invoke配合使用,Invoke不能挑选参数,只能在其上封装这3个,来做参数筛选
RetiresOnSaturation
相同的接口调用总是从下往上匹配的,匹配成功之后,该接口调用就完成了,一旦有新的调用产生,就会出现匹配次数问题,比如
- `TEST(Bar, Any) {`
- `EXPECT_CALL(f, some(_)); // 1`
- `EXPECT_CALL(f, some(A<int>())); // 2`
- `}`
第一次f.some(1)会匹配调用第2个,但第二次f.some(2)还是会匹配第2个,这样就超出了2的次数限制(也许大家会疑问为什么不匹配第1个,因为2在下面,优先级不同,就算匹配成功过之后,该接口调用并不消失,还是要坚持在匹配判断的第一线的)
此时,就可以使用RetiresOnSaturation,让匹配成功的调用”退休”掉,即不再存在于待匹配接口列表中,比如
- `TEST(Bar, Any) {`
- `EXPECT_CALL(f, some(_)); // 1`
- `EXPECT_CALL(f, some(A<int>())) // 2`
- `.RetiresOnSaturation();`
- `}`
2的动作表明,当2的匹配完全达成之后(比如已经匹配了额定的次数之后),就不再进行匹配了.上面提到的第二次调用,也就顺利的匹配第1个接口调用了
ON_CALL
上面提到过修改接口默认值(DefaultValue::Set()/Clear()),但这个是针对返回值类型的,还有一种是针对接口调用的,就是ON_CALL
- `ON_CALL(mock_object, method(matchers))`
- `.WillByDefault(action);`
ON_CALL表示的是当调用mock_object.method,且参数匹配matchers,默认执行action(含义都与EXPECT_CALL完全一致).基本上很类似与EXPECT_CALL,只是此处只是设置接口调用的默认动作而已
同一匹配接口的ON_CALL,同样是根据由下向上的优先级选择的,最下面的最优先(当然,前提是都匹配才行)
## 总结
可以看到,gmock提供了很多有用的api,以方便我们进行test.更多详细的介绍可以参看gmock的[wiki](http://code.google.com/p/googlemock/w/list),还有很多我们这里没有提及的暗黑用法
其实,通过上面的描述,应该可以了解到,mock是比较简单的,难点在于如何简单快速的引入mock,这个就需要一个优秀的高度可测试的系统设计
还是那句话,以test开始,Simple Design, Simple Implementation.后面我们还会讨论关于设计的相关问题,需要时刻记在心理的是,无法方便test的系统,注定是不稳定的
## 关于设计
看了这么多关于mock的材料,给人的感觉,mock是一种test-after的行为,是用来验证代码已经写成之后的行为的工具
其实不然,mock和普通的test一样,是可以來表达需求的
设想书中的情景,当下需要实现一个http请求的功能,预想到应该有个接口(HttpRequest)來处理,但此时的重点不在于这个接口如何实现,而是如何利用接口实现上面的逻辑.接口自然是一系列行为的集合,但行为背后的实现,是目前不关心的,接口只有输入/输出/副作用等外在影响,而这些都可以通过mock來进行模拟(非实现,只是行为而已).利用mock,先将接口变相的做出来(即可以通过编译),然后再此之上,编写上层逻辑的test和对应逻辑(TDD循环模式),从而完成编码实现
可以看到,此处的mock相当于给了我们一个可控的接口的**初级实现**,只不过这个实现是**行为**意义上的,只是提供给我们可以來编写test和进行测试的一个虚假依赖,这样test就能写能编译了,能验证需求和逻辑实现的正误了
这个和test-after还是有区别的,不是说依赖太慢了,我们换一个mock來测试(当然,这个肯定也是一方面),主要是定义了接口,我们要依赖接口直接测试开发
## 关于测试风格
书上提到了mock的两种风格,一种是”Tell-Don’t-Ask”,即对象之间是委托的关系,彼此之间不暴露过多的信息,另一种是”Classic”,即test不在意这种情况,对象内部的状态可以暴露,只要为了测试方便即可
这两种其实都类似,但在我看来,还是第一种比较恰当.虽然是mock,但其上还是真正的接口,而接口,应该是行为的集合,将来接口的使用方,也仅仅是依赖于行为的预期,那么在真正的测试中,也是要专注于行为,而不是状态.利用gmock工具,很方便的定义接口行为,我们也就不需要关心状态问题了
测试,也是要遵循原则的
## 关于mock使用的细节
mock的使用方法上面已经介绍了很多,但何时使用mock,如何添加mock,这个其实才是mock的关键,下面总结了一些使用上的细节问题,可供参考
- **重构设计** 如果仅仅因为依赖太复杂,想简化这层关系,而引入mock,说明原来的依赖关系有问题,需要拆分.mock是用来测试上层逻辑的正确性的,不是消除依赖的工具;如果同一个mock出现了多次(即在不同的test中都定义了类似的mock),也需要重新审视设计,是不是有DRY问题
- **代码覆盖** 有一些依赖注入的方法(比如工厂类),可能会导致部分实际代码无法被测试集覆盖,而且,如果mock本身也有复杂逻辑,很有可能mock本身也有部分代码无法被测试集覆盖.此时需要编写新的test,或者简化测试逻辑
- **重构测试** 应该把测试代码和实际代码等同起来(二者同属于项目代码集),因此重构工作应该在测试上也进行,清晰的测试集,不仅方便修改代码,同时也能清楚的完成作为需求文档的目标
- **过度复杂** 如果在纠结于复杂的test编写,一个可能的原因在于该test试图完成的测试目标太多,或者系统结构有问题.单个test应该专注于一个行为(一个ASSERT),如有复杂的test,最好拆分成不同级别的多个test.分拆test,对应的则是分拆系统,因为此时待测系统结构复杂,可能才是test复杂的原因
- **表达明确** test的表达能力是最主要的,gmock功能虽多,但不是每个都是必须的,要选择最能表达test意图的工具,來完成对应的测试
