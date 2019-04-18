# 游戏中的有限状态机(InGems) - gauss的专栏 - CSDN博客
2011年12月05日 12:21:37[gauss](https://me.csdn.net/mathlmx)阅读数：211标签：[游戏																[input																[delete																[im																[框架																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=im&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
这是GAMEGEMS中的第三章的第一部分，番的不好。你可以直接阅读原文。原本以为这是人工智能的部分，看到一半才发现只是一个简单的框架。如果你想学人工智能，这里没有，就不要浪费时间了。由于本人水平有限，其中难免会出现原则性的错误，希望指正。
关键字：有限状态机、状态、输入、状态转换、输出状态当前状态
一个有限状态机类
在这篇文章中，我们创建了一个通用的有限状态机（FSM）的C++类。有限状态机是计算机科学和数学理论的抽象，它在许多的方面是很有用处的。这里我们不去讲解有限状态机的理论上的知识。而是讲如何实现一个“有限状态机”，“有限状态机”在游戏的人工智能方面是很有用处的。
“有限状态机”是由有限的状态组成的一个机制。一个“状态”就是一个状况。你考虑一下门；它的“状态”有“开”或“关”以及“锁”与“未锁”。
对于一个“有限状态机”，它应该有一个“输入”。这个“输入”可以影响“状态转换”。“有限状态机”应该有一个简单（或复杂）的状态转换函数，这个函数可以决定什么状态可以变成“当前状态”。
这个当前的新状态被称为“有限状态机”的“状态转换”的“输出状态”。如果你对这个概念有些迷惑，就把“门”做为理解“有限状态机”的例子。当一个“门”处于“关闭”状态和“锁”状态，当你输入了“使用钥匙”时，门的状态可以变成“未锁”状态（即“状态转换”的输出状态，也就是门的当前状态）。当你输入了“使用手”时，门的状态可以转换成“开”的状态。当门处于“开”的状态时，我们输入“使用手”时，会使门的状态重新回到“关”的状态。当“门”处于“关”的状态时，我们输入“使用钥匙”时，这将会使门重新回到“锁”的状态。当门处于“锁”的状态，我们输入“使用手”，就不能把门的状态转换到“开”的状态，门仍然会保持“锁”的状态。还有，当门处于“开”的状态时，我们输入“使用钥匙”是不能把门的状态转换成“锁”的状态的。
总之，“有限状态机”是一个有限的状态组成的，其中的一个状态是“当前状态”。“有限状态机”可以接受一个“输入”，这个“输入”的结果将导致一个“状态转换”的发生（即从“当前状态”转换到“输出”状态）。这个“状态转换”是基于“状态转换函数”的。状态转换完成之后，“输出状态”即变成了“当前状态”。
输入 状态转换函数
当前状态-----》状态转换-------------》输出状态（当前状态）
那么，人们是如何将这个概念应用到游戏的AI系统中的呢？有限状态机的功能很多：管理游戏世界、模拟NPC的思维、维护游戏状态、分析玩游戏的人的输入，或者管理一个对象的状态。
假如在一个冒险游戏中有一个NPC，名字可以叫MONSTER。我们可以先假设这个MONSTER在游戏中有如下的状态：BERSERK、RAGE、MAD、ANNOYED以及UNCARING。（前几个状态不好分别）。假设，MONSTER对于不同的状态可以执行不同的操作，并且假设你已经有了这些不同操作的代码。我们这时可以使用“有限状态机”来模拟这个MONSTER的行为了。只要我们给出不同的“输入”，MONSTER就会做出不同的反应。我们再来指出这些“输入”是什么：PLAYER SEEN、PLAYER ATTACKS、PLAYERGONE、MONSTER
 HURT、MONSTER HEALED。这样我们可以得到一个状态转换的表格，如下：游戏状态转换表：
当前状态 输入 输出状态
UNCARING PLAYER SEEN ANNOYED
UNCARING PLAYER ATTACKS MAD
MAD MONSTER HURT RAGE
MAD MONSTER HEALED UNCARING
RAGE MONSTER HURT BERSERK
RAGE MONSTER HEALED ANNOYED
BERSERK MONSTER HURT BERSERK
BERSERK MONSTER HEALED RAGE
ANNOYED PLAYER GONE UNCARING
ANNOYED PLAYER ATTACKS RAGE
ANNOYED MONSTER HEALED UNCARING
根据上面的这个表格，我们可以很容易的画出一个MONSTER的“状态转换图”，MONSTER的每一个状态就是图中的顶点。
因此，根据当前状态和对FSM的输入，MONSTER的状态将被改变。这时根据MONSTER的状态执行相应操作的代码（假设已经实现）将被执行，这时MONSTER好像是具备了人工智能。显然，我们可以定义更多的“状态”，写出更多的“输入”，写出更多的“状态转换”，这样，MONSTER可以表现的更真实，生动，当然，这些游戏的规则问题应该是策划制定的。
FSMclass以及FSMstate
现在，我们如何把这些方法变成现实？使用FSMclass和它的组成部分FSMstate可以实现这些想法。
定义FSMstate
class FSMstate
{
    unsigned m_usNumberOfTransition; //状态的最大数
    int* m_piInputs; //为了转换而使用的输入数组
    int* m_piOutputState; //输出状态数组
    int m_iStateID; //这个状态的唯一标识符
public:
   //一个构造函数，可以接受这个状态的ID和它支持的转换数目
    FSMstate(int iStateID,unsigned usTransitions);
   //析构函数，清除分配的数组
    ~FSMstate();
   //取这个状态的ID
    int GetID(){return m_iStateID;}
   //向数组中增加状态转换
    void AddTransition(int iInput,int iOutputID);
   //从数组中删除一个状态转换
    void DeleteTransition(int iOutputID);
   //进行状态转换并得到输出状态
    int GetOutput(int iInput);
};
对这个类的分析：
功能：主要是实现与一个状态相关的各种操作。我们前面假设了MONSTER的各种状态：
#define STATE_ID_UNCARING 1
#define STATE_ID_MAD 2
#define STATE_ID_RAGE 3
#define STATE_ID_BERSERK 4
#define STATE_ID_ANNOYED 5
状态转换所需的输入有：
#define INPUT_ID_PLAYER_SEEN 1
#define INPUT_ID_PLAYER_ATTACK 2
#define INPUT_ID_PLAYER_GONE 3
#define INPUT_ID_MONSTER_HURT 4
#define INPUT_ID_MONSTER_HEALED 5
以上是五个状态的标识符。
我们就要声明5个FSMstate的实例，每一个实例代表一个状态和与之有关的操作。假设我们先处理状态STATE_ID_MAD
类成员变量m_iStateID就等于STATE_ID_MAD类成员变量m_usNumberOfTransition就是可由这个状态转换成的状态的个数，前面有一个表，其中有两个状态可以由这个状态产生，它们分别是STATE_ID_UNCARING和STATE_ID_RAGE。
这时，m_usNumberOfTransition等于2。
m_piInputs是一个指针变量，它保存与这个状态相关的输入，在前面的表中我们知道与STATE_ID_MAD相关的输入为
INPUT_ID_MONSTER_HURT和INPUT_ID_MONSTER_HEALED，因此m_piInputs中存放的是这两个数据。
而m_piOutputState存放的是与STATE_ID_MAD相关的状态，即STATE_ID_RAGE和STATE_ID_UNCARING，这样，m_piOutputState中存放的数据便是这两个值。
以上是对成员变量的解释，下面解释成员函数：
构造函数
FSMstate::FSMstate(int iStateID,unsigned usTransitions)
{
    if(!usTransitions) //如果给出的转换数量为0，就算为1
        m_usNumberOfTransitions=1;
    else
        m_usNumberOfTransitions=usTransitions;
   //将状态的ID保存起来
    m_iStateID=iStateID;
   //分配内存空间
    try
    {
        m_piInputs=new int[m_usNumberOfTransitions];
        for(int i=0;i<m_usNumberOfTransitions;++i)
            m_piInputs[i]=0;
    }
    catch(...)
    {
        throw;
    }
    try
    {
        m_piOutputState=new int[m_usNumberOfTransition];
        for(int i=0;i<m_usNumberOfTransitions;++i)
            m_piOutputState[i]=0;
    }
    catch(...)
    {
        delete [] m_piInputs;
        throw;
    }
}
这就是构造函数，在FSMstate类中共有四个成员变量，在这个函数中全部被初始化了。FSMstate是一个类，是否还记得MONSTER的状态（如MAD、UNCARING）。这个类就是实现对MONSTER的一个状态的管理的。假如这个状态是STATE_ID_MAD， 与这个状态相关的状态有两个，上面已经讲过了。这时我们给成员变量赋值，在这个具体例子中它们的值如下：
m_usNumberOfTransition=2
m_piInput[0]=0;
m_piInput[1]=0;
m_piOutputState[0]=0;
m_piOutputState[1]=0;
m_iStateID=STATE_ID_MAD;
析构函数：
FSMState::~FSMState()
{
    delete [] m_piInputs;
    delete [] m_piOutputState;
}
析构函数将动态分配的存储空间释放了。
void FSMstate::AddTransition(int iInput,int iOutputID)
{
    for(int i=0;i<m_usNumberOfTransitions;++i)
        if(!m_piOutputState[i]) break;
            if(i<m_usNumberOfTransition)
            {
                m_piOutputState[i]=iOutputID;
                m_piInputs[i]=iInput;
            }
}
这个函数给两个前面构造函数动态分配的空间加入数据，首先要找到两个数组中找到适当的位置，之后，如果位置是合法的
我们就可以把数据加入这两个数组中。因为STATE_ID_MAD与两个状态有关，因此，我们可以调用两次这个函数，把这两个状态加入到类中：
AddTransition(INPUT_ID_MONSTER_HURT,STATE_ID_RAGE);
AddTransition(INPUT_ID_MONSTER_HEALED,STATE_ID_UNCARING)
这样，与状态STATE_ID_MAD相关的“状态”和“输入”也加入了。
void FSMstate::DeleteTransition(int iOutputID)
{
    // 遍历每一个输出状态
    for(int i=0;i<m_usNumberOfTransitions;++i)
    {
        //如果找到输出状态，退出循环
        if(m_piOutputState[i]==iOutputID)
            break;
    }
   //如果没有找到输出状态，返回
    if(i>=m_usNumberOfTransitions)
        return;
   //将输出状态的内容置0
    m_piInputs[i]=0;
    m_piOutputState[i]=0;
   //被删除的输出状态的后面的输出状态前移
    for(;i<(m_usNumberOfTransition-1);++i)
    {
        if(!m_piOUtputState[i])
            break;
        m_piInputs[i]=m_piInputs[i+1];
        m_piOutputState[i]=m_piOutputState[i+1];
    }
   //最后面的输出状态置0
    m_piInputs[i]=0;
    m_piOutputState[i]=0;
}
这个函数是要删除与一个状态相关的输出状态。设一个状态STATE_ID_MAD，与之相关的状态有两个STATE_ID_RAGE,STATE_ID_UNCARING，当然这是经过初始化以及前面的添加状态函数之后，产生了这两个相关的状态。你想删除哪一个？如果你想删除相关的输出状态，只要在删除函数中指出那个状态即可，例如：
DeleteTransition(STATE_ID_RAGE);
你就可以删除输出状态STATE_ID_RAGE了。
int FSMstate::GetOutput(int iInput)
{
   //先给输出状态赋值（如果未找到与输入对应的输出状态时，返回这个值）
    int iOutputID=m_iStateID;
   //遍历输出状态
    for(int i=0;i<m_usNumberOfTransitions;++i)
    {
        //如果没找到，退出循环
        if(!m_piOutputState[i])
            break;
        //如果找到了与“输入”相对应的“输出状态”，进行赋值。
        if(iInput==m_piInputs[i])
        {
            iOutputID=m_piOutputState[i];
            break;
        }
    }
   //返回“输出状态”
    return(iOutputID);
}
这个函数功能是返回与“输入”相对应的“输出状态”的标识。如果没有与“输入”相对应的“输出状态”，返回原来的状态，如果有与之对应的“输出状态”，返回这个状态的ID。
下面定义的是FSMclass，这个类用于维护FSMstate对象集合。
class FSMclass
{
    State_Map m_map; //包括了状态机的所有状态
    int m_iCurrentState; //当前状态的ID
public:
    FSMclass(int iStateID); //初始化状态
    ~FSMclass()
   //返回当前状态ID
    int GetCurrentState() {return m_iCurrentState;}
   //设置当前状态ID
    void SetCurrentState(int iStateID) {m_iCurrentState=iStateID;}
    //返回FSMstate对象指针
    FSMstate* GetState(int iStateID);
   //增加状态对象指针
    void AddState(FSMstate* pState);
   //删除状态对象指针
    void DeleteState(int iStateID);
   //根据“当前状态”和“输入”完成“状态”的转换。
    int StateTransition(int iInput);
};
FSMclass::m_map是FSMstate对象的集合，是从STL<map>中实现的。
FSMclass::m_iCurrentState是FSMstate对象的状态标识，是“有限状态机”的当前状态。
FSMclass::GetCurrentState()可以用之访问当前的FSMstate对象的状态的标识符。
FSMclass::SetCurrentState()可以设置当前FSMstate对象的状态的标识符。
FSMclass::GetState()可以取得有限状态机中的任何FSMstate对象的指针。
FSMclass::AddState()增加有限状态机中的FSMstate对象。
FSMclass::DeleteState()删除有限状态机中的FSMstate对象
FSMclass::StateTransition()初始化状态转换，根据输入返回输出状态。
这个类使用了STL，我不知道它怎么用：）。听说是高人才使用它，高人起码要写过上万行的代码。因此不能详细介绍这个类了
总之，可以这么理解这两个类FSMstate,FSMclass.FSMstate代表了一个状态以及和状态相关的数据和操作。如在MONSTER中有五个状态，我们就要声明五个类的对象，每个对象中包括了与这个状态相关的状态，输入和各种转换函数。可以说FSMstate是对每一个状态的封装（包括相关数据和操作），游戏中的对象有多少状态，就要声明多少个FSMstate对象。而FSMclass则是对这若干个FSMstate对象（这个例子中MONSTER有五个状态）进行的封装。在FSMclass中指明了若干个FSMstate中哪一个是当前的MONSTER拥有的状态并且可以设置，得到以及删除状态，并且可以进行状态间的转换。
总之：游戏中的MONSTER有多少状态，游戏中就要声明多少的FSMstate对象，每一个FSMstate对象包括了与特定的状态相关的数据和操作。而FSMclass只有一个，它用于协调若干个FSMstate之间的关系和操作。
下面是如何在游戏中使用两个类的例子：
首先是创建FSMstate对象（若干个），有多少状态就要循环多少次，下面是增加STATE_ID_UNCARING状态的例子：
FSMstate* pFSMstate=NULL;
//创建状态
try
{
   //第一个参数是增加状态的标识，第二个参数指明了与这个
    //状态相关的状态的个数。
    pFSMstate=new FSMstate(STATE_ID_UNCARING,2);
}
catch(...)
{
    throw;
}
//之后给这个状态加入相关的“输入”和“输出状态”
pFSMstate->AddTransition(INPUT_ID_PLAYER_SEEN,STATE_ID_ANNOYED);
pFSMstate->AddTransition(INPUT_ID_PLAYER_ATTACKS,STATE_ID_MAD);
这个函数指明了与特定状态相关的“输入”和“输出状态”
比如第一个函数，它表明如果我要输入一个INPUT_ID_PLAYER_SEEN，这时就会产生一个输出状态，STATE_ID_ANNOYED。
我们应该为每一个状态做上面的事情，这里就略过了。之后我们要声明一个FSMclass对象，用于协调上面的FSMstate对象之间的关系。
try
{
    m_pFSMclass=new FSMclass(STATE_ID_UNCARING);
}
catch(...)
{
    throw;
}
上面指明了MONSTER的当前状态是STATE_ID_UNCARING最后将FSMstate对象分别加入到FSMclass中。
下面介绍如何使用FSMclass
使用十分简单，只要我们给出一个“输入”，之后，我们便可以得到一个“输出状态”，根据这个“输出状态”我们执行相应的操作，最后，把这个“输出状态”变成MONSTER的当前状态。
在游戏中发生了一些事情，如玩游戏的人指出他控制的人进攻MONSTER（用鼠标点击了MONSTER），这时会产生一个“输入”iInputID=INPUT_ID_PLAYER_ATTACK;
这时，我们调用状态转换函数：
m_iOutputState=m_pFSMclass->StateTransition(iInputID);
这时，我们的“输入”对MONSTER产生了刺激，产生了一个“输出状态”。这时我们根据这个输出状态调用相应的代码执行就可以了，这时的MONSTER好像有应反了，我们说它有了简单的智能。
if(m_iOutputState==STATE_ID_MAD)
{
    //some code for the monster to act mad
}
当然，我们也应该把其它状态执行的操作也写出来，但只写一个就可以了。使用这个状态机就是这么简单。总之，FSMclass不是全部的人工智能，相反，它只是一个框架，一个开始智能需要的还很多。只要你可以分出“状态”，并且知道什么“输入”产生什么“输出状态”就可以了，当然这是一个游戏的规则，策划应当完成这个部分
