# Coroutine，你究竟干了什么？（小续） - tkokof1的专栏 - CSDN博客

2013年10月17日 11:35:33[tkokof1](https://me.csdn.net/tkokof1)阅读数：11509


[前篇](http://blog.csdn.net/tkokof1/article/details/11842673)中讲了一些自己关于Coroutine的理解，后来陆陆续续的又想到了一些，在此简单记录一下，内容不多，故作“小”续吧 ：）

  之前的示例程序基本实现了一个自己的WaitForSeconds，功能上及使用上都与Unity自建的WaitForSeconds类似，另外的，如果我们进一步考虑WaitForFixedUpdate和WaitForEndOfFrame这类的YieldInstruction，我们是否也可以自己实现？其中的原理又是什么呢？

  先让我们来看一张MonoBehaviour的生命周期图（外国友人所做，原帖在[这里](http://www.richardfine.co.uk/2012/10/unity3d-monobehaviour-lifecycle/)）：

![](https://img-blog.csdn.net/20131017113444093)

  从图中我们可以看到，Unity对于MonoBehaviour中Coroutine的处理，实际上并不是简单的单一步骤，而是采取了类似分阶段处理的方式：即在每次FixedUpdate之后进行WaitForFixedUpdate的处理，每次Update之后进行WaitForSeconds的处理，而在每一帧结束之前，进行WaitForEndOfFrame的处理。那么依此而看，先前示例中简单的LateUpdate模拟就有些捉襟见肘了：WaitForSeconds可能勉强还可以应付，但是像WaitForFixedUpdate以及WaitForEndOfFrame这类YieldInstruction，这种方式便应付不来了……

  不过如果从原理上来讲的话，上面的那些YieldInstruction还是相似的，问题都集中于何时调用IEnumerator的MoveNext()操作而已，如WaitForSeconds，就是在其延时结束之后调用MoveNext()，而像WaitForFixedUpdate，便是在每次FixedUpdate之后才调用MoveNext()，另外的WaitForEndOfFrame，则是在每帧结束之前调用对应的IEnumerator的MoveNext()。如果需要实际实现的话，目前想到的一种方法便是创建几种不同类型的CoroutineManager，譬如WaitForSecondsCoroutineManager、WaitForFixedUpdateCoroutineManager等等，用于在不同更新时刻处理其中的IEnumerator，原理上都是控制IEnumerator MoveNext()的实际，在此我并没有加以实现，一是自己懒惰了些（……），二是Unity本身也未明显提供用以实现这类处理的回调方法，不过有兴趣的朋友倒是可以试试 ：）

  另外想到的一个问题就是StartCoroutine的嵌套问题，即在某一个Coroutine中又以yield return的方式开启了另一个Coroutine，例如以下代码：

void Start() {

// start unity coroutine

StartCoroutine(UnityCoroutine());

}

IEnumerator UnityCoroutine() {

Debug.Log("Unity coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(2);

yield return StartCoroutine(InnerUnityCoroutine());

Debug.Log("Unity coroutine end at time : " + Time.time);

}

IEnumerator InnerUnityCoroutine() {

Debug.Log("Inner Unity coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(2);

Debug.Log("Inner Unity coroutine end at time : " + Time.time);

}

  那么“外层”的UnityCoroutine只有在“内层”的InnerUnityCoroutine“执行”完毕之后才会继续“执行”，程序结果大概是这个样子：

![](https://img-blog.csdn.net/20131017113437625)

  在此简单一提，必须要采用yield return的方式才可以实现这种嵌套执行效果，简单的一个StartCoroutine只能创建一个“平行”的Coroutine，并不会产生嵌套效果，譬如如下代码：

void Start() {

// start unity coroutine

StartCoroutine(UnityCoroutine());

}

IEnumerator UnityCoroutine() {

Debug.Log("Unity coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(2);

StartCoroutine(InnerUnityCoroutine());

Debug.Log("Unity coroutine end at time : " + Time.time);

}

IEnumerator InnerUnityCoroutine() {

Debug.Log("Inner Unity coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(2);

Debug.Log("Inner Unity coroutine end at time : " + Time.time);

}

  那么最终的执行结果便会是这个样子：

![](https://img-blog.csdn.net/20131017113446500)

  OK，那么如果我们需要自己实现这种嵌套机制，应该怎么做呢？实际上还是在特殊类型判断上做文章！在Unity中，每次调用StartCoroutine都会返回一个特殊的Coroutine类型，而这个类型便是我们实现嵌套Coroutine的突破口！好了，首先让我们实现一下自己的Coroutine类型：

//

//    <maintainer>Hugo</maintainer>

//    <summary>coroutine simple class</summary>

//

using UnityEngine;

using System.Collections.Generic;

public class CoroutineSimple {

public Stack<System.Collections.IEnumerator> innerEnumeratorStack;

public CoroutineSimple(Stack<System.Collections.IEnumerator> enumeratorStack) {

    innerEnumeratorStack = enumeratorStack;

}

}

  此处的Stack<System.Collections.IEnumerator>可能有些奇怪，实际上也确实如此（汗……），使用这种方式也仅是为了降低一点代码复杂度，因为我们现在要支持嵌套的Coroutine，于是使用了Stack<System.Collections.IEnumerator>这种类型，即每次我们都仅是处理某个Stack<System.Collections.IEnumerator>顶部的IEnumerator，一旦发现有嵌套的Coroutine产生，那便直接将对应的IEnumerator压栈，可能这么说还是有些许模糊，看一下新的CoroutineManager代码应该会更清晰一些：

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

List<Stack<System.Collections.IEnumerator>> m_enumerators = new List<Stack<System.Collections.IEnumerator>>();

List<Stack<System.Collections.IEnumerator>> m_enumeratorsBuffer = new List<Stack<System.Collections.IEnumerator>>();

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

// handle CoroutineYieldInstruction here

if (m_enumerators[i].Peek().Current is CoroutineYieldInstruction) {

    CoroutineYieldInstruction yieldInstruction = m_enumerators[i].Peek().Current as CoroutineYieldInstruction;

if (!yieldInstruction.IsDone()) {

    continue;

}

}

// do normal move next

if (!m_enumerators[i].Peek().MoveNext()) {

m_enumerators[i].Pop();

if (m_enumerators[i].Count <= 0) {

        m_enumeratorsBuffer.Add(m_enumerators[i]);

}

continue;

}

if (m_enumerators[i].Peek().Current is CoroutineSimple) {

    // here we just push new enumerator

CoroutineSimple coroutineSimple = m_enumerators[i].Peek().Current as CoroutineSimple;

m_enumerators[i].Push(coroutineSimple.innerEnumeratorStack.Peek());

// NOTE: very tricky

// TODO: clear

m_enumerators.Remove(coroutineSimple.innerEnumeratorStack);

}

// other special enumerator here ...

}

// remove end enumerator

for (int i = 0; i < m_enumeratorsBuffer.Count; ++i) {

    m_enumerators.Remove(m_enumeratorsBuffer[i]);

}

m_enumeratorsBuffer.Clear();

}

public CoroutineSimple StartCoroutineSimple(System.Collections.IEnumerator enumerator) {

Stack<System.Collections.IEnumerator> newEnumeratorStack = new Stack<System.Collections.IEnumerator>();

newEnumeratorStack.Push(enumerator);

m_enumerators.Add(newEnumeratorStack);

return new CoroutineSimple(newEnumeratorStack);

}

}

  代码味道不是很好，还有不少Tricky，但是也算基本实现了嵌套的Coroutine处理，写个简单的“单元测试”：

  void Start() {

// start unity coroutine

StartCoroutine(UnityCoroutine());

    // start self coroutine

CoroutineManager.Instance.StartCoroutineSimple(SelfCoroutine());

}

IEnumerator UnityCoroutine() {

Debug.Log("Unity coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(5);

yield return StartCoroutine(InnerUnityCoroutine());

Debug.Log("Unity coroutine end at time : " + Time.time);

}

IEnumerator SelfCoroutine() {

Debug.Log("Self coroutine begin at time : " + Time.time);

yield return new CoroutineWaitForSeconds(5);

yield return CoroutineManager.Instance.StartCoroutineSimple(InnerSelfCoroutine());

Debug.Log("Self coroutine end at time : " + Time.time);

}

IEnumerator InnerUnityCoroutine() {

    Debug.Log("Inner Unity Coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(5);

yield return StartCoroutine(DeepUnityCoroutine());

Debug.Log("Inner Unity Coroutine end at time : " + Time.time);

}

IEnumerator DeepUnityCoroutine() {

    Debug.Log("Deep Unity Coroutine begin at time : " + Time.time);

yield return new WaitForSeconds(5);

Debug.Log("Deep Unity Coroutine end at time : " + Time.time);

}

IEnumerator InnerSelfCoroutine() {

Debug.Log("Inner Self Coroutine begin at time : " + Time.time);

yield return new CoroutineWaitForSeconds(5);

yield return CoroutineManager.Instance.StartCoroutineSimple(DeepSelfCoroutine());

Debug.Log("Inner Self Coroutine end at time : " + Time.time);

}

IEnumerator DeepSelfCoroutine() {

Debug.Log("Deep Self Coroutine begin at time : " + Time.time);

yield return new CoroutineWaitForSeconds(5);

Debug.Log("Deep Self Coroutine end at time : " + Time.time);

}

  执行结果基本如人所料：

![](https://img-blog.csdn.net/20131017113450828)

 不过平心而论，虽然上面的示例实现了类似的Coroutine效果，但是从很多方面来看，Unity自建的Coroutine控制都更好更细致，自己实现的Coroutine则相对粗糙了不少，而且不要忘了，我们目前还不支持WaitForFixedUpdate、WaitForEndOfFrame、WWW这类的特殊类型呢！当然我们可以继续精化程序，但是我毕竟不是一个热爱重造车轮的人，上面的那些个代码也仅是为了学习一下Coroutine的原理，仅此而已，那么就让我们暂时到此为止吧~ ：）

  下次再见了~~~

