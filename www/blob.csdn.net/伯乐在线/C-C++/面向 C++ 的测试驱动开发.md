# 面向 C++ 的测试驱动开发 - 文章 - 伯乐在线
原文出处： [IBM/夏建东](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/index.html)
# 测试驱动开发（TDD）背景及综述
测试驱动开发是 Kent 提出的一种新的软件开发流程，现在已广为人知，这种开发方法依赖于极短重复的开发周期，面对开发需求，开发人员要先开发代码测试用例，这些代码实现的测试用例定义了工程要实现的需求，然后去开发代码快速测试通过这这些用例，这个时候的代码是相对比较粗糙的，只是为了通过这个测试，测试通过以后，这些测试所覆盖的需求就会相对固定下来了，然后随着实现更多的需求，以前实现的那些粗糙的代码的问题会逐步的暴露出来，此时就要用重构来消除重复改进代码设计，因为自动化的测试用例已经框定了相应的需求，这样在代码改进和重构的过程中就不会破坏已实现的需求，实现了安全重构。
从测试驱动开发的流程可以看出来，测试驱动开发仅仅要求一个简单的设计开始实现需求，然后随着软件开发的推进实现有保护重构代码和设计。依赖于 TDD 开发所生成的单元测试用例代码，实现有保护重构是大型的软件开发项目不可以缺少的，代码级别的测试更能有效地提高软件产品的质量。测试驱动开发中的重构过程也是一个使设计逐步完善的过程。 本文的主要目的是使测试驱动开发落到实地，和具体的语言（C++）和单元测试框架结合起来，并用实例展示测试驱动开发的魅力。
# 测试驱动开发的信条
先开发和设计测试代码，再代码实现通过测试，以测试驱动设计实现，开发和设计的过程，得到了快速的反馈，用这些反馈驱动，改进和重构代码设计，是一个有机的开发过程。按照 Kent 的定义，测试驱动开发的原则是：
- 不要写一行代码，除非有一个失败的自动化测试案例要纠正。
- 消除重复的代码，改进设计。
这两个简单的原则，却产生了一些复杂的个体和组的行为，这些隐含的技术行为包括：
- 运行代码对设计决定快速反馈下，实现有机地设计
- 必须自己写自己的测试用例，而不是等待别人帮你写测试代码，那样会花费很长时间
- 必须要有对变更代码快发反应开发环境
- 组件必须要高内聚、低耦合，以使测试简单化。
两个原则还隐含开发任务的顺序：
- 红色（Red）：写些不能够工作的小测试，这个测试甚至不能编译通过。
- 绿色（Green）：快速编写代码使测试通过，不用太在意代码质量只是通过测试。
- 重构（Refactor）：消除开始是只是要通过测试的重复代码，改进设计。
红色（Red）-绿色（Green）-重构（Refactor），这个就是测试驱动开发的座右铭（Mantra）。这种开发方式可以有效的减少代码的缺陷密度，减少 bug 的数量，将大部分的缺陷在代码的开发过程中消除，减少了 QA 测试和质量保证的成本。
按照软件工程的说法，软件缺陷和 bug 发现的越早，所需的更正这些缺陷的成本就会越小。所以在软件的开发阶段，采用测试驱动的开发方法，把测试引入到开发阶段，使测试和质量意识融入到开发的过程中，这对提高软件工程质量非常有帮助。 而且在采用测试驱动开发必然要求所开发的组件、接口、类或方法是可测试的（testable），这就要求开发的组件，接口要遵循组件和类高内聚（Highly Cohesive），组件和组件、类和类之间低耦合（loosely Coupled）原则，这种开发方式生成的代码必然会帮助开发者，在不断的有保护重构的过程中，提高软件架构的设计，使日后的软件维护变得有章可循。
测试驱动开发符合敏捷软件开发的精神，在不断迭代过程中，增量地实现软件需求而这一切开始可以从简单设计开始。
# 单元测试框架比较和筛选
C++技术是一种高级语言，它出现的时间要比 Java 和 C#早得多，但支持像 xUnit 框架的 C++单元测试框架发展起来的比较晚。 C++ 的单元测试框架选择比较多，现在比较流行的 C++测试框架有 Boost Test、UnitTest++、CppTest、Google C++ Testing Framework。 Boost Test，拥有良好的断言功能，对异常控制，崩溃控制方面处理的比较好，也有良好的可以移植性，但结构复杂，不易于掌握。CPPUnit 是开发比较早的单元测试框架，是对 JUnit 的 C++的移植的一种尝试，拥有丰富的断言和期望功能。Google Test C++ 简称 Gtest，是近期发展起来的单元测试框架，对 xUnit 支持的比较好，支持 TDD 的红-绿-重构模式，支持死亡和退出测试，较好的异常测试控制能力，良好的测试报告输出，拥有自动注册测试用例和用例分组等功能，还有和 Gmock 框架的无缝结合，支持基于接口的（抽象类的）Mock 测试-模拟测试。
下表是一个对三种流行 C++单元测试框架的简单比较，Gtest 虽然发展起来的较晚，但丰富功能简单易用，易学，加之移植性较好，是跨平台项目单元测试框架比较好的选择。
##### 表 1.单元测试框架比较
|测试框架支持特性|Gtest|Boost Test|CPPUnit|
|----|----|----|----|
|可移植性|较好|好（依赖于 Boost 库）|较好|
|丰富的断言|优|优|一般|
|丰富的断言信息|优|良好|较差|
|自动检测和注册测试用例|优|良|一般|
|易于扩展断言|易于扩展|一般|一般|
|支持死亡和退出测试（Death 和 Exit）|支持|支持|不支持|
|支持参数化测试（Parameterized test）|支持|支持|不支持|
|支持 Scoped_Trace|支持|不支持|不支持|
|支持选择性执行测试用例|支持|支持|支持|
|丰富的测试报告形式（xml）|支持|支持|支持|
|支持测试用例分组 Suites|支持|支持|支持|
|开源|是|是|是|
|执行速度|快|快|快|
|基于接口的`Mock`测试|通过`Gmock`支持|不支持|不支持|
|易用性|优秀|较复杂|较好|
|支持类型化的参数化测试|支持|不直接支持|不直接支持|
# 测试驱动开发-GTest 简介
Gtest 是基于 xUnit 的 C++单元测试框架，支持自动化案例自动发掘，丰富的断言功能，支持用户自定义断言，支持死亡测试和退出测试，还有异常测试控制，支持值类型和类型化的参数化测试，接口简单易用，对每个测试案例有执行时间的输出，可以帮助分析代码的执行效率，单一接口文件 gtest.h。
图 1 是 Console 模式输出用红和绿表示失败和成功的测试用例，看起来比较符合 TDD 的策略和定义
##### 图 1.GTest 的案例测试结果输出
![GTest 的案例测试结果输出](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/image003.jpg)
Gtest 的断言有两种形式，致命性断言（Fatal Assertion）和非致命性断言（Nonfatal Assertion）。
除了基本的断言形式外，Gtest 还包括一些其他的高级断言形式，比如死亡断言，退出断言测试和异常断言等。
Gtest 还有其他的一些特性，比如类型参数化测试，值类型参数化的测试，测试用例分组，洗牌式测试等，可以参照附录中列出的 Gtest 的官网获取更多的信息。
在测试驱动软件开发的过程中，我们不可避免的要去依赖第三方系统，比如文件系统、第三方库、数据库访问，其他的在线数据的访问等，按照测试驱动开发的快速反馈的原则，如果在单元测试用例中去直接访问这些信息，势必在测试驱动开发过程中会依赖这些资源从而造成访问时间无法控制， 所以单元测试一般应该避免直接访问第三方系统，这就是 Mock 测试的主要目的，用模拟的接口去替换真实的接口，模拟出单元测试需要的第三方数据和接口进而隔离第三方的影响，专注于自己的逻辑实现。Gmock 就是这样一个 Mock 框架，它是类似于 jMock、EasyMock 和 Hamcres ，但是是 C++版本的 Mock 框架。 Gmock 是基于接口的 Mock 框架，在 C++中接口的定义是通过抽象函数和抽象类来实现的，这种要求势必会要求我们尽量遵循基于接口的编程原则，把交互界面上的操作抽象成接口，以便是接口可被模拟 Mock。可以在附录中列出的 Gmock 官网获取更多信息。
# 测试驱动开发的实践
测试驱动开发和敏捷开发是相辅相成的，敏捷开发的需求一般是以故事、产品功能列表，或需求用例的方式给出，拿到这些需求后，开发团队会根据相应的需求文档分析需求，做功能分解，根据功能优先级制定迭代开发计划和测试计划。测试驱动开发可以从两个角度来看，广义的和狭义的。广义的测试驱动开发是从流程上规定测试驱动开发，这种情况下一般要求 QA 走到前面，先根据需求先开发测试用例，这些测试用例会作为功能验收的标准，然后开发人员会根据测试用例做详细的功能设计和编码实现，最后提交给 QA 做功能验收测试。 狭义的测试驱动开发是开发人员拿到功能需求后，先自己开发代码级别的测试用例，然后开发具体的实现通过这些测试用例的一种开发方法。 本文涉及的是第二种，从代码级别开始的，狭义的测试驱动开发。
相信每个人都玩过棋牌游戏，简单起见，为了实践测试驱动开发方法我想开发一款简单的三子棋游戏，如图 2 所示。三子棋的游戏规则很简单，只要是同样的三个棋子连成一条线那么持对应棋子的人就胜出，图中持 O 子棋的人获胜。总结一下三子棋游戏的基本需求：
- 我需要一个 3X3 的棋盘，可以用下三子棋。
- 我需要在棋盘上下棋和获取到棋子。
- 我要能验证和判断是不是三个棋子在同一条线上，以判断是不是有人胜出。
- 我不能放棋子到已被占用的棋位置上。
- 我要能判断是不是棋盘已满并无赢家。
- 我需要能复位棋盘，以便于重新开始下棋。
- 我需要用对记住玩家，以便于我能特例化 Player
- 我需要能保存和加载棋局能力，以便于我能下次回来继续之前的游戏。
##### 图 2.三子棋游戏
![图 2.三子棋游戏](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/image005.jpg)
以上是三子棋游戏的基本需求列表，拿到这些需求后，我会做一些简单解决方案的设计，解决方案包括 4 个子工程（C++ Project），其中一个测试工程 TicTacToeGamingTest，其余三个分别是 TicTacToeLib,TicToeGamingLib 和 TicTacToeConsoleGaming，这三个工程的依赖关系是 TicTacToeConsoleGaming 依赖于 TicToeGaminglib 和 TicTacToeLib，TicToeGamingLib 依赖于 TicTacToeLib。 建好这些工程,有了基本的设计思路后，在测试工程里首先开发的测试代码。
##### 图 3.解决方法设计
![图 3.解决方法设计](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/image007.jpg)
先看第一个需求：
**1.我需要一个 3X3 的棋盘，可以来下三子棋。**
这个需求很简单，现在的棋盘不需要包括任何的逻辑，为了便于测试我需要一个接口去访问它，现在接口是空的，也没有实现，这样一个测试用例就可以满足这个需求：

```
TEST_F(TicTacToeTestFixture,IWantAGameBoard)
{
	IGameBoard *gameBoard=NULL; 
    EXPECT_NO_THROW(gameBoard=new SimpleGameBoard("simpleGame")); 
	EXPECT_TRUE(gameBoard!=NULL); 	
    EXPECT_NO_THROW(delete gameBoard);
}
```
这是第一个测试用例，稍微解释一下。TicTacToeTestFixture 是用于测试的分组的，它是一个类，继承于 Gtest 的 test 类 testing::Test，这个类可以重载 setup 和 teardown 等虚拟函数用于测试准备和清理测试现场。`TEST_F` 是定义测试用例的宏，IWantAGameBoard 是测试的案例的名称，会显示在输出中，测试用例很简单，只是只是保证能创建和析构 SimpleGameBoard 实例，并无异常抛出。这个测试用例现在是不能编译通过的，因为 IGameBoard 接口和 SimplegameBoard 都还没有声明和定义,接下来为了使这个案例通过，我在 TicTacToeLib 工程里，声明和定义 IGameBoard 和 SimpleGameBoard 类，IGameBoard 是纯抽象类，抽象了所有对棋盘的操作。引入声明到测试工程中，编译通过并运行，现在完成了第一测试用例，尽管测试的 IGameBoard 和 SimpleGameBoard 还是空的。可以看一下输出：
##### 图 4 .测试用例输出
![图 4 .测试用例输出](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/image009.jpg)
**2.我需要在棋盘上下棋和获取到棋子**
这个需求能使棋手在棋盘上把棋子放到想要的位置上并能查看指定棋盘位置上的棋子，棋盘是 3×3。实现这个需求也很简单，我只要在 IGameBoard 接口上添加两个函数然后在 SimpleGameBoard 里实现这两个函数就可以满足这个需求：

```
virtual void PutChess(int x,int y,char chess)=0; 
virtual char GetChess(int x,int y)=0 ;
       有了这个思路，我想这样设计这个测试用例：
TEST_F(TicTacToeTestFixture,PutandGetChess)
{
char xChess='X'; 
char yChess='Y';
IGameBoard *gameBoard=new SimpleGameBoard("simpleBoard"); 
gameBoard->PutChess(0,0,xChess); 
gameBoard->PutChess(2,2,yChess); 
EXPECT_EQ(xChess,gameBoard->GetChess(0,0));
EXPECT_EQ(yChess,gameBoard->GetChess(2,2)); 
delete gameBoard; 
}
```
试着编译这个测试工程，失败，原因是没有实现这两个函数，接下来我回到 TicTacToeLib 工程去声明和定义这两个函数。为了实现这两个功能，在 SimpleGameBoard 定义 private 数据：`vector`char``> data_;用于 保存棋子和位置信息，为了简单，棋子用 Char 类型来表示，位置信息和 data_向量的下标对应,如棋盘位置（2，2）对应的是 data_[2*3+2]这个位置，数据是安行存放的。两个函数的实现是：``

```
void SimpleGameBoard::PutChess( int x,int y,char chess )
{
	assert(x<xMaxDim&&y<yMaxDim); 
int xy=x*3+y; 
if(data_.size()==0)initboard_(); 
	data_[xy]=chess; 
}
char SimpleGameBoard::GetChess( int x,int y )
{
	assert(x<xMaxDim&&y<yMaxDim); 
	assert(data_.size()==yMaxDim*xMaxDim); 
return data_[x*3+y];
}
```
initboard_()是个 protected 函数，用于初始化 data_。 现在可以重现编译和运行测试工程，结果如下：
##### 图 5 .测试用例输出
![图 5 .测试用例输出](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/image011.jpg)
有了两个测试用例的实现，并且运行是绿色，继续下个需求。
**3.我要能验证和判断是不是三个棋子在同一条线上，以判断是不是有人胜出**
这个需求用于判断三个棋子是否已经在一条线上，如果是的话，那么持对应棋子的棋手就会胜出，这个测试用例可以这样设计：

```
TEST_F(TicTacToeTestFixture,JugeThreeInLine)
{
IGameBoard *gameBoard=new SimpleGameBoard("simpleBoard"); 
IGameBoard *gameBoard2=new SimpleGameBoard("simpleboard2"); 
char xChess='x',yChess='o'; 
gameBoard->PutChess(0,0,xChess); gameBoard2->PutChess(0,1,yChess); 
gameBoard->PutChess(1,1,xChess); gameBoard2->PutChess(1,1,yChess); 
gameBoard->PutChess(2,2,xChess); gameBoard2->PutChess(2,1,yChess); 
EXPECT_TRUE(gameBoard->CheckWinOut(xChess));
EXPECT_TRUE(gameBoard2->CheckWinOut(yChess)); 
EXPECT_FALSE(gameBoard->CheckWinOut(yChess));
EXPECT_FALSE(gameBoard2-)CheckWinOut(xChess)); 
delete gameBoard; 
delete gameBoard2; 
}
```
设计是这样的，为简单，我把判断棋子胜出的函数 CheckWinOut 定义到接口 IGameBoard 中，并在 SimpleGameBoard 中实现它，实现如下：

```
bool SimpleGameBoard::CheckWinOut(char chess)
{
return IsThreeInLine_(chess); 
}
```
IsThreeInLine_是受保护的成员函数，它会扫描棋盘的行，列和对角线看是否指定的棋子在一条线上，如果有三个棋子在一条线上，则说明有人胜出。编译运行测试，绿色通过。 继续下一个需求。
**4.我不能放棋子到已被占用的棋位置上。**
这个需求是个验证性需求，要保证棋子不能重叠和覆盖已在棋盘上的棋子，实现这个需求我只要重构现有的代码加上避免棋子重叠的逻辑。只要避免在 PutChess 时候，检查是否指定的位置是否已有棋子，如果是简单的抛出异常即可。有了这些基本的思路，我开始设计测试用例。

```
TEST_F(TicTacToeTestFixture,BizException_Occupied){
    IGameBoard *gameBoard=new SimpleGameBoard("simple board"); 
char xChar='X',yChar='0'; 
EXPECT_NO_THROW(gameBoard->PutChess(0,0,xChar)); 
<strong>EXPECT_THROW(gameBoard->PutChess(0,0,xChar),ChessOverlapException); </strong>
EXPECT_NO_THROW(gameBoard->PutChess(2,2,yChar)); 
<strong>EXPECT_THROW(gameBoard->PutChess(2,2,yChar),ChessOverlapException); </strong>
delete gameBoard; 
}
```
ChessOverlapException 是我将要实现的一个异常类，这个是在棋手试图放棋子到已有棋子的棋盘位置上时要抛出的异常。测试用例中，我在（0，0）和（2，2）这两个位置上放同样的棋子以触发这个异常。为了编译通过，我开始实现 ChessOverlapException。 ChessOverlapException 继承自 std::exception 我重载了 what 函数返回相应的异常信息。 把这个异常类的定义引入的测试工程中，编译通过运行测试，但却得到了红色 Red，案例失败：
图 6.测试用例输出
![图 6.测试用例输出](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/image013.jpg)
原因是我还没有重构 PutChess 函数以加入避免棋子被被覆盖的代码。现在来重构 PutChess 函数：

```
void SimpleGameBoard::PutChess( int x,int y,char chess )
{
assert(x<xMaxDim&&y<yMaxDim); 
int xy=x*3+y; 
if(data_.size()==0){
		initboard_(); 
		data_[xy]=chess;
return ; 
	}
if(data_[xy]!='+') {
throw ChessOverlapException("chess overlap!");
	}
else data_[xy]=chess;
}
```
重新编译测试工程并运行得到绿色 Green 通过。继续下一个需求。
**5.我要能判断是不是棋盘已满并无赢家。 **
这个需求用于判断是否是和棋的情况，棋盘满了但并无赢家，这是可能出现的一种情况，这个实现设计可以有两种方式. 一是重构 CheckWinOut 函数，使返回值携带更多的信息，比如和棋，有人胜出等。二是定义一个独立的函数去判断棋盘的当前状态。第一种方案较合理，开始设计这种方案的测试用例：

```
EST_F(TicTacToeTestFixture,IsEndedInADraw)
{
char xChess='X',yChess='O'; 
IGameBoard *gameBoard=new SimpleGameBoard("simpleBoard");
gameBoard->PutChess(0,0,yChess);gameBoard->PutChess(0,1,xChess);gameBoard->PutChess(0,2,yChess);
gameBoard->PutChess(1,0,xChess);gameBoard->PutChess(1,1,yChess);gameBoard->PutChess(1,2,yChess);
gameBoard->PutChess(2,0,xChess);gameBoard->PutChess(2,1,yChess);gameBoard->PutChess(2,2,xChess);
<strong>GameBoardStatus status=gameBoard->CheckWinOut(yChess);
EXPECT_TRUE(status==GAMEDRAW); </strong>
<strong>GameBoardStatus status2=gameBoard->CheckWinOut(xChess); EXPECT_TRUE(status2==GAMEDRAW);</strong> 
delete gameBoard; 
}
```
以上的测试用例可以看出， 我设计了和棋的棋局，并想重构 CheckWinout 函数，使其返回枚举类型 GameBoardStatus 以表示棋局的状态，其中 GAMEDRAW 表示和棋状态。为了使工程能编译通过，开始定义这个枚举类型并重构 CheckWinOut 函数。实现所有设计，经过几次的 Red 失败，最终 形成代码：

```
GameBoardStatus SimpleGameBoard::CheckWinOut(char chess)
{
if(IsThreeInLine_(chess)){
return GAMEMWINOUT;
	}
else if(IsEndedInADraw_()){
return GAMEDRAW; 
	}
else{
return GAMERUNNING; 
	}
}
```
其中那个 IsEndedInADraw_是个受保护的成员函数，用于检测是否和棋。 在调通这个测试用例的过程中，我也更新了测试`JugeThreeInLine。因为重构 ChecWinOut 改变了返回类型。`
**6.我需要能复位棋盘，以便于重新开始下棋。**
**7.我需要用对记住玩家，以便于我能特例化 Player。**
6 和 7 需求的测试案例和实现比较比较简单，不在赘述，7 的要求是要建立玩家 Player，这个主要是说要能实例化玩家。可以看附带的工程。
**8.我需要能保存和加载棋局能力，以便于我能下次回来继续之前的游戏**。
这个需求是一个合理的需求，玩家可以保存和继续回来玩游戏，他的测试用例可以这样设计：

```
TEST_F(TicTacToeTestFixture,SaveTheBoard)
{
IGameBoard * gameBoard=new SimpleGameBoard("simpleBoard"); 
char xChess='x',yChess='o'; 
gameBoard->PutChess(0,0,xChess); 
gameBoard->PutChess(1,2,yChess); 
IGameIO *gameIO=new SimpleGameIO(); 
EXPECT_NO_THROW(gameIO->save(gameBoard,"somewhere")); 
delete gameBoard; 
delete gameIO;
}
TEST_F(TicTacToeTestFixture,LoadTheBoard)
{
IGameBoard * gameBoard=new SimpleGameBoard("simpleBoard"); 
char xChess='x',yChess='o'; 
gameBoard->PutChess(0,0,xChess); 
gameBoard->PutChess(1,2,yChess); 
IGameIO *gameIO=new SimpleGameIO(); 
EXPECT_NO_THROW(gameIO->save(gameBoard,"somewhere")); 
IGameBoard *game=gameIO->load("somewhere"); 
EXPECT_EQ(xChess,game->GetChess(0,0)); 
EXPECT_EQ(yChess,game->GetChess(1,2)); 
EXPECT_EQ('+',game->GetChess(2,2));
delete game;
```


```
delete gameBoard; 
delete gameIO; 
}
```
这里用两个测试用例来覆盖这个需求，一个是保存棋盘，一个是加载棋盘。由这个测试用例可以看到，要通过这个测试，必须要定义 IGameIO 接口和 SimpeGameIO 类。 保存棋盘的媒介是文件。按照 TDD 的开发要求，测试单元本身最好是脱离对第三方系统的依赖，但测试中必然会用到第三方系统，解决这些问题的方法有几种。创建第三方系统的 Stub 类或是 FakedObject，第三种选择是 Mock 框架，如 Gmock。 Gmock 的设计理念是基于接口的，只要是第三方访问提供的是接口，这些访问就可以可以被用 Gmock 模拟。可以看参考文献获取更多的信息。 限于篇幅不再赘述。一下是完成所有测试用例的测试结果。
##### 图 7.测试用例输出
![图 7.测试用例输出](http://www.ibm.com/developerworks/cn/linux/1401_xiajd_ctdd/image015.jpg)
或许你会注意到有些测试用例的设计，只是以点盖面，如果想要更多的验证点可以借助于 Gtest 提供的参数化测试设计测试数据，然后去测试实现的类和逻辑。 还有死亡测试的用例，可以在参考资源中的 Gtest 资源中查看。
# 结论
C++中实现测试驱动开发 TDD 之前是很困难的事。 但有了类似于 xUnit 的 Gtest 和 Gmock 测试框架，在 C++工程中实现 TDD 也变得很享受。测试驱动开发是一个很好的工具，它可以帮助开发者实现有机开发，在需求的实现过程中快速得到反馈，另一个好处是测试驱动开发可以使开发人员更加重视需求和测试，以测试用例为中心，这样势必会产生更好代码。从软件工程的角度来说，测试驱动开发的实践应用会大幅度的提高软件开发的质量，用代码级别的测试用例来覆盖和保障程序的健壮性更能保障整个软件产品的开发质量。
测试驱动开发的座右铭模式：红色-绿色-重构，然后重复这个直到开发完成为止，是一个自我确认和有保护代码重构的过程。采用测试驱动开发的模式的软件产品，产生的单元测试代码，从代码级别测试覆盖了软件的需求，使以后的代码重构更安全可靠。
