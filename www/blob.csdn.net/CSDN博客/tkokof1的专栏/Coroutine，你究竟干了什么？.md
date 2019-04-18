# Coroutine，你究竟干了什么？ - tkokof1的专栏 - CSDN博客

2013年09月20日 10:48:33[tkokof1](https://me.csdn.net/tkokof1)阅读数：22490


  一 引子

  使用Unity已经有一段时间了，对于Component、GameObject之类的概念也算是有所了解，而脚本方面从一开始就选定了C#，目前来看还是挺明智的：Boo太小众，而且支持有限；JS（或着说UnityScript）的话稍稍自由散漫了些，不太符合我们这些略显严谨的程序猿；相比之下，C#各方面都十分沁人心腑，使用起来还是相当舒畅的 ：）

  就游戏开发而言，Unity也确实为我们减轻了不少开发负担、缩短了很多开发流程，但从开发原理上来讲，使用Unity你仍然避不开许多传统的开发技术，譬如几乎所有游戏程序都有的Update，在Unity里就变成了MonoBehaviour的一个成员方法；而另一个几乎与Update并重的Init方法，在Unity里则被换成了Start。可以这么说，Unity虽然极大的简化了游戏开发流程，但从方法原理上来讲的话，其实他也并没有和传统开发方式存在非常大的差异，Update还是那个Update，Init还是那个Init，只不过换了一个更简单的形式而已~

  依此思路，我持续着自己的Unity学习之路，也逐步验证着自己上述的观点，直到有一天，我遇到了Coroutine ……

  二. Coroutine是什么？

  延时大概是游戏编程中最司空见惯的需求之一：角色移动控制需要延时、事件触发需要延时、甚至开启一个粒子特效有时也需要延时，可以说，延时在游戏开发中几乎无处不在 ：）有鉴于此，很多的游戏引擎对于延时控制都提供了很好的支持，譬如在cocos2d-x中，CCDelayTime就是专门用来干这个的，当然，其他引擎也有自己不同的支持方式，但是从实现层面来讲，基本都是“标记开始时间，Update中持续更新检查”这种方法，从代码上来看，大抵是这么一个样子：

  float delayTime = <time value to delay>;

  float elapsedTime = 0;

  void Update(float frameTime) {

      if (elapsedTime >= delayTime) {

          // delay is over here ...

      }

      else {

          elapsedTime += frameTime;

       }

  }

  而在Unity中，我们自然也可以使用这种方法来进行延时，但是相对而言，这种方法并不是最佳实践，更好的在Unity中实现延时的做法是使用[Coroutine](http://docs.unity3d.com/Documentation/Manual/Coroutines.html)，就代码上来看的话，大概是这个样子：

  IEnumerator DelayCoroutine() {

// work before delay

yield return new WaitForSeconds(<time value to delay>);

// work after delay

}

  StartCoroutine(DelayCoroutine());

  没有什么elapsedTime之类的变量，甚至没有什么Update，你要做的就是写一个以IEnumerator为返回类型的方法，然后在其中使用yield return这种语法来返回一个WaitForSeconds类型的实例，实例的构造参数就是你想要延时的时间，然后在需要的时候，调用StartCoroutine来进行延时即可。

  面对这种从未见过的延时实现方式，虽然代码表达上很容易让人理解，一开始的我却显得有些抵触，首先的一个疑问就是：这Coroutine是什么？从字面意思上来理解，Coroutine应该就是“协程”的意思，而这所谓的“协程”又是什么东西？第一个想到的便是Lua中“[协程](http://www.lua.org/pil/9.1.html)”，Unity中的Coroutine难道也是这个概念吗？另外的，这Unity“协程”跟线程又是一个什么关系，就其可以进行延时而不影响其他逻辑运行这个特性来看，“协程”是否就是C#线程的一个封装呢？第二个疑问就是返回类型IEnumerator，名字奇怪也就罢了，我还需要使用yield return这种奇怪的方式来进行返回，而且貌似WaitForSeconds也并不是一个所谓IEnumerator的类型，怎么就可以正常返回呢？第三个疑问，也是最大的一个疑问就是：虽然WaitForSeconds这个类型的名称意义一目了然，但就实现层面来看，其是如何做到延时这项功能的着实让人摸不着头脑……

  三. Coroutine大概是这个样子的……

  随着自己对C#有了进一步的了解，我才慢慢发现，上面所言的那两个奇怪的IEnumerator和yield return，其实并不是Unity的什么独创，相反，他们却是C#中到处可见的[迭代器](http://msdn.microsoft.com/en-us/library/vstudio/dscyy5s0.aspx)的构造方式（之一），你也许对于迭代器这个东西没什么印象，但实际上，我们可能天天都在使用它！让我们马上来看一个最普遍的迭代器运用：

  int[] array = new int[] {1, 2, 3, 4, 5};

  foreach (int val in array) {

      // do something

  }

  代码非常简单，不过是使用foreach来遍历一个整型数组，而代码中我们早已习以为常的foreach其实就是迭代器的语法糖，在真正的运行代码中，C#的编译器会将上面的代码改头换面成这个样子：

  int[] array = new int[] {1, 2, 3, 4, 5};

  IEnumerator e = array.GetEnumerator();

  while (e.MoveNext()) {

      // do something

  }

  上述代码首先通过array的GetEnumerator方法来获取array的一个“迭代器”，然后通过“迭代器”的MoveNext方法进行依次遍历，而这“迭代器”实际上就是之前那个稍显奇怪的IEnumerator类型！而至于yield return，其实是C# 2.0新引进的一种实现迭代器模式的简便语法，在之前的C# 1.0中，如果要实现一个完整的迭代器，我们必须要分别实现IEnumerable和IEnumerator这两个接口，过程略显枯燥繁琐，而借助yield return，这两个步骤我们都可以省略！譬如我们写下了如下的代码：

IEnumerator Test() {

    yield return 1;

    yield return 2;

    yield return 3;

}

那么C#编译器就会帮你自动生成类似下面的这些代码（不准确，仅作示意）：

public class InnerEnumerable : IEnumerable {

    public class InnerEnumerator : IEnumerator {

        int[] array = new int[] {1, 2, 3};

        int currentIndex = -1;

        public bool MoveNext() {

            ++currentIndex;

            return currentIndex < array.Length;

        }

        public Object Current {

            get { return array[currentIndex]; }

        }

        public void Reset() {

            throw new Exception("unsurport");

        }

    }

public IEnumerator GetEnumerator() {

        return new InnerEnumerator();

    }

}        

IEnumerator Test() {

     InnerEnumerable e = new InnerEnumerable();

     return e.GetEnumerator();

 }

  当然，实际的迭代器代码实现远非如此简单，但原理上基本可以看做是一个有限状态机，有兴趣的朋友可以看看更深入的一些介绍，譬如[这里](http://blogs.msdn.com/b/oldnewthing/archive/2008/08/12/8849519.aspx)和[这里](http://csharpindepth.com/articles/chapter6/iteratorblockimplementation.aspx)。

  OK，让我们继续回到Unity，通过上面的这些分析，我们大概就肯定了这么一点：Unity其实是使用了迭代器来实现延时的，像IEnumerator、yield return等的使用皆是为了配合C#中迭代器的语法，其与什么多线程之类的概念并没有多少关系，但是目前我仍然还是不能理解之前的那个最大疑问：虽然迭代器可以保留运行状态以便下次继续往下运行，但是他本身并没有提供什么机制来达到延时之类的效果，像foreach这种语句，虽然使用了迭代器，但实际上也是一股脑儿运行完毕的，并不存在延时一说，那么在Unity中，为什么简单的返回一个WaitForSeconds就可以呢？

  三 Coroutine原来如此 ：）

  看来答案应该是在WaitForSeconds这个类型身上了~经过简单的一些搜索，我找到了这么[一篇帖子](http://answers.unity3d.com/questions/12412/programming-waitforseconds.html)，内容便是如何自己实现一个简单的WaitForSeconds，大体上的思路便是使用循环yield return null这种方法来达到延时的目的，直接抄一段帖子中的示例代码：

using UnityEngine; 

using System.Collections; 

public class TimerTest : MonoBehaviour { 

    IEnumerator Start () {

        yield return StartCoroutine(MyWaitFunction (1.0f));

        print ("1");

        yield return StartCoroutine(MyWaitFunction (2.0f));

        print ("2");

    }

    IEnumerator MyWaitFunction (float delay) {

        float timer = Time.time + delay;

        while (Time.time < timer) {

            yield return null;

        }

    }

}

  也就是说，如果我们在代码中写下了如下的延时语句：

   yield return WaitForSeconds(1.0f);

  那么在逻辑上，其大概等价于下面的这些语句：

   float timer = Time.time + 1.0f;

   while (Time.time < timer) {

       yield return null;

   }

  而完成这些操作的，很可能便是WaitForSeconds的构造函数，因为每次延时我们都就地生成（new）了一个WaitForSeconds实例。

  然而使用[ILSpy](http://ilspy.net/)查看WaitForSeconds实现源码的结果却又让我迷惑：WaitForSeconds的构造函数非常简单，似乎仅是记录一个时间变量罢了，根本就不存在什么While、yield之类的东西，而其父类YieldInstruction则更简单，就是单纯的一个空类……另外的，WWW这个Unity内建类型的使用方式也同样让我不解：

using UnityEngine;

using System.Collections;

public class Example : MonoBehaviour {

    public string url = "http://images.earthcam.com/ec_metros/ourcams/fridays.jpg";

    IEnumerator Start() {

        WWW www = new WWW(url);

        yield return www;

        renderer.material.mainTexture = www.texture;

    }

}

  在上面的示例代码中，yield return www;这条语句可以做到直到url对应资源下载完毕才继续往下运行（迭代），效果上类似于WaitForSeconds，但是WWW本身却又不像WaitForSeconds那样是个YieldInstruction，而且在使用上也是首先创建实例，然后直接yield 返回引用，按照这种做法，即便WWW的构造函数使用了上面的那种循环yield return null的方法，实际上也达不到我们想要的等待效果；再者便是语法上的一些细节，首先如果我们需要使用yield return的话，返回类型就必须是IEnumerable(<T>)或者IEnumerator(<T>)之一，而C#中的构造函数是没有返回值的，显然不符合这个原则，所以实际上在构造函数中我们无法使用什么yield return，另外的一点，虽然上述帖子中的方法可以实现自己的延时操作，但每次都必须进行StartCoroutine操作（如果没有也起不到延时效果），这一点也与一般的WaitForSeconds使用存在差异……

  后来看到了[这篇文章](http://www.altdevblogaday.com/2011/07/07/unity3d-coroutines-in-detail/)，才大抵让我有所释怀：之前自己的种种猜测都聚焦在类似WaitForSeconds这些个特殊类型之上，一直以为这些类型肯定存在某些个猫腻，但实际上，这些类型（WaitForSeconds、WWW之类）都是“非常正常”的类型，并没有什么与众不同之处，而让他们显得与众不同的，其实是StartCoroutine这个我过去一直忽略的家伙！

  原理其实很简单，WaitForSeconds本身是一个普通的类型，但是在StartCoroutine中，其被特殊对待了，一般而言，StartCoroutine就是简单的对某个IEnumerator 进行MoveNext()操作，但如果他发现IEnumerator其实是一个WaitForSeconds类型的话，那么他就会进行特殊等待，一直等到WaitForSeconds延时结束了，才进行正常的MoveNext调用，而至于WWW或者WaitForFixedUpdate等类型，StartCoroutine也是同样的特殊处理，如果用代码表示一下的话，大概是这个样子：

foreach(IEnumerator coroutine in coroutines)

{

    if(!coroutine.MoveNext())

        // This coroutine has finished

        continue;

    if(!coroutine.Current is YieldInstruction)

    {

        // This coroutine yielded null, or some other value we don't understand; run it next frame.

        continue;

    }

    if(coroutine.Current is WaitForSeconds)

    {

        // update WaitForSeconds time value

    }

    else if(coroutine.Current is WaitForEndOfFrame)

    {

        // this iterator will MoveNext() at the end of the frame

    }

    else /* similar stuff for other YieldInstruction subtypes or WWW etc. */

}

基于上述理论，我们就可以来实现自己的WaitForSeconds了：

首先是CoroutineManager，我们通过他来实现类似于StartCoroutine的功能：

//

//    <maintainer>Hugo</maintainer>

//    <summary>simple coroutine manager class</summary>

//

using UnityEngine;

using System.Collections.Generic;

public class CoroutineManager : MonoBehaviour {

public static CoroutineManager Instance {

    get;

private set;

}

List<System.Collections.IEnumerator> m_enumerators = new List<System.Collections.IEnumerator>();

List<System.Collections.IEnumerator> m_enumeratorsBuffer = new List<System.Collections.IEnumerator>();

void Awake() {

    if (Instance == null) {

    Instance = this;

}

else {

    Debug.LogError("Multi-instances of CoroutineManager");

}

}

void LateUpdate() { 

    for (int i = 0; i < m_enumerators.Count; ++i) {

// handle special enumerator

if (m_enumerators[i].Current is CoroutineYieldInstruction) {

    CoroutineYieldInstruction yieldInstruction = m_enumerators[i].Current as CoroutineYieldInstruction;

if (!yieldInstruction.IsDone()) {

    continue;

}

}

// other special enumerator here ...

// do normal move next

if (!m_enumerators[i].MoveNext()) {

    m_enumeratorsBuffer.Add(m_enumerators[i]);

continue;

}

}

// remove end enumerator

for (int i = 0; i < m_enumeratorsBuffer.Count; ++i) {

    m_enumerators.Remove(m_enumeratorsBuffer[i]);

}

m_enumeratorsBuffer.Clear();

}

public void StartCoroutineSimple(System.Collections.IEnumerator enumerator) {

m_enumerators.Add(enumerator);

}

}

接着便是我们自己的WaitForSeconds了，不过在此之前我们先来实现WaitForSeconds的基类，CoroutineYieldInstruction：

//

//    <maintainer>Hugo</maintainer>

//    <summary>coroutine yield instruction base class</summary>

//

using UnityEngine;

using System.Collections;

public class CoroutineYieldInstruction {

public virtual bool IsDone() {

    return true;

}

}

  很简单不是吗？类型仅有一个虚拟的IsDone方法，上面的CoroutineManager就是依据此来进行迭代器迭代的，OK，该是我们的WaitForSeconds上场了：

//

//    <maintainer>Hugo</maintainer>

//    <summary>coroutine wait for seconds class</summary>

//

using UnityEngine;

using System.Collections;

public class CoroutineWaitForSeconds : CoroutineYieldInstruction {

float m_waitTime;

float m_startTime;

public CoroutineWaitForSeconds(float waitTime) {

m_waitTime = waitTime;

m_startTime = -1;

}

public override bool IsDone() {

// NOTE: a little tricky here

if (m_startTime < 0) {

    m_startTime = Time.time;

}

// check elapsed time

return (Time.time - m_startTime) >= m_waitTime;

}

}

原理非常简单，每次IsDone调用时进行累时，直到延时结束，就这么简单 ：）

写个简单的案例来测试一下：

//

//    <maintainer>Hugo</maintainer>

//    <summary>coroutine test case</summary>

//

using UnityEngine;

using System.Collections;

public class CoroutineTest: MonoBehaviour {

void Start() {

// start unity coroutine

StartCoroutine(UnityCoroutine());

    // start self coroutine

CoroutineManager.Instance.StartCoroutineSimple(SelfCoroutine());

}

IEnumerator UnityCoroutine() {

Debug.Log("Unity coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(5);

Debug.Log("Unity coroutine begin at time : " + Time.time);

}

IEnumerator SelfCoroutine() {

Debug.Log("Self coroutine begin at time : " + Time.time);

yield return new CoroutineWaitForSeconds(5);

Debug.Log("Self coroutine begin at time : " + Time.time);

}

}

效果虽然不如原生的WaitForSeconds那么精确，但也基本符合期望，简单给张截图：

![](https://img-blog.csdn.net/20130920104626046)

四 尾声

  Coroutine这个东西对于我来说确实比较陌生，其中的迭代原理也困扰了我许久，不少抵触情绪也“油然而生”（在此自我反省一下），但是经过简单的一阵子试用，我却赫然发现自己竟然离不开他了！究其原因，可能是其简洁高效的特性深深折服了我，想想以前那些个分散于代码各处的计时变量和事件逻辑，现在统统都可以做成一个个Coroutine，不仅易于理解而且十分高效，我相信不管是谁，在实际使用了Unity中的Coroutine之后，都会对他爱不释手的~ ：）当然，这么好的东西网上自然早以有了非常优秀的[介绍](http://unitygems.com/coroutines/)，有兴趣的朋友可以仔细看看 ：）

  好了，就这样吧，下次再见了~






