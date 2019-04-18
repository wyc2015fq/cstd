# iTween那些事儿（二） - tkokof1的专栏 - CSDN博客

2014年02月11日 20:09:15[tkokof1](https://me.csdn.net/tkokof1)阅读数：4386


一．引子

  上次我们简单浏览了一番iTween的使用和原理，这次我们换个角度，转而看看iTween目前存在的一些缺陷以及一点点可能的改进之处，当然，这些所谓的缺陷或者改进都仅是我的一家之言，不足信也~ ：）

二. iTween的缺点

  1. 参数类型不够安全

  问题主要出在iTween对于Hashtable的使用上，上篇我们提到了iTween使用Hashtable来作为接口参数传递的媒介容器，而Hashtable本身仅使用最松散的System.Object类型来管理内部的键值数据，自然就会产生不少安全隐患，考虑以下代码：

  iTween.ScaleFrom(gameObject, iTween.Hash(

      "scale", 2, 

      "time", 1, 

      "islocal", true));

  明眼人可能马上就发现了问题：scale参数对应的数值应该为Vector3或者Transform，而上面代码中却给出了一个整数2，而相关代码的编译甚至连一个警告都看不到，我们能得到的可能就是运行期iTween的一个警告以及出人意料的动画效果……不知你对于这个问题看法如何，我本人确实曾在这个问题上栽过跟头，当时不经意间也写出了类似上面一般的代码，然后便是一段很久很久的代码排查……

  2. 字符串传参不够健壮

  iTween采用了字符串的方式来传递控制参数，譬如“position”便是位置，“time”就是时间，虽然直观方便，但是也至少存在不够健壮的问题，考虑以下代码：

  iTween.MoveTo(gameObject, iTween.Hash(

      "positoin", Vector3.zero, 

      "time", 1, 

      "islocal", true));

  代码编译没有任何问题，参数类型似乎也没什么错误，甚至在运行过程中可能都看不到一个警告，但是相应的gameObject就是不会按照“指示”来进行移动！问题出在我们错误的将“position”参数输入成了“positoin”，由于是字符串的关系，编译器自然不会有任何抱怨，甚至于iTween都仅会认为这是一个他不认识的参数而加以忽略，只剩下我们对着奇怪的动画现象百思不解……

  3. 运行机制有待改进

  正如上篇所说，iTween使用向GameObject动态添加Component的方式来实现相应的动画表现，这种运行机制在简单情况下并没有什么问题，但是当我们面对游戏场景中存在大量的动画物体，或者说某个物体需要大量的动画控制的时候，iTween这种运行方式所带来的内存消耗、效率损失可能就有些让人难以接受了，试想我们仅仅为了新增一个scale动画属性，就必须要承担整个MonoBehaviour组件所带来的影响，着实有些得不偿失，换个角度，如果我们可以通过某种方式将这些动画控制集中管理，即通过譬如单个组件来统一管理GameObject的各个缓动属性，提高效率、节省内存的同时，还能做到集中管理，岂不快哉？

  4. 代码实现不够高效

  目前iTween版本（我使用的是2.0.45版本）实现中仍然存在一些效率不高的代码，譬如上篇提到的回调处理，iTween内部使用了SendMessage的方法来进行实现，改以代理方式实现我觉得应该更高效，也更整洁 ：）

  5. 代码实现仍然存在一些细微Bug

  目前iTween版本实现中也依旧存在一些细微Bug，我所看到的大概有以下两个问题：

  A. iTween在销毁自身的时候，会依据iTween组件的id属性来更新一些内部状态，而这个id属性也是iTween在向GameObject添加Component时内部生成的，也就是说id这个属性对于我们使用者来说是完全透明的，按理不会出什么问题，但是iTween在生成这个id时使用了简单的随机算法，个人感觉并不能完全做到id生成的唯一性，因此可能产生id冲突，造成内部状态错误，个人认为改用譬如GUID之类的实现方式应该更好~

  B. 另一个Bug可能就是一个“笔误”了，问题出在iTween对于冲突组件的处理上，按照上篇提到的说法，iTween会将冲突组件进行剔除，譬如以下代码：

using UnityEngine;

class iTweenBugTest : MonoBehaviour {

void Start() { 

iTween.MoveTo(gameObject, iTween.Hash(

            “name”, “MoveToFirst”,

            “position”, Vector3.zero,

            “time”, 5

        ));

iTween.MoveTo(gameObject, iTween.Hash(

            “name”, “MoveToSecond”,

            “position”, Vector3.zero,

            “delay”, 1

            “time”, 5

        ));

}

}

如果你在Unity中运行以上脚本，你会发现初始状态时，GameObject会被添加两个iTween组件，分别名为“MoveToFirst”和“MoveToSecond”，但是1s过后，iTween组件就只剩下“MoveToSecond”了，这便是iTween对于冲突组件的特殊处理，但是，如果你改以运行以下代码：

using UnityEngine;

class iTweenBugTest : MonoBehaviour {

void Start() {

    iTween.ValueTo(gameObject, iTween.Hash(

            “name”, “ValueTo”,

            “from”, 1,

            “to”, 2,

            “onupdate”, “OnUpdate”,

            “time”, 5

        ));

iTween.MoveTo(gameObject, iTween.Hash(

            “name”, “MoveToFirst”,

            “position”, Vector3.zero,

            “time”, 5

        ));

iTween.MoveTo(gameObject, iTween.Hash(

            “name”, “MoveToSecond”,

            “position”, Vector3.zero,

            “delay”, 1

            “time”, 5

        ));

}

void OnUpdate() {

}

}

  那么你就会发现原本应该被剔除的“MoveToFirst”却自始至终完好无损了，原因应该是iTween实现上的一个疏漏，有兴趣的朋友可以仔细看看相关的ConflictCheck函数 ：）

  6. 实用功能仍然缺乏

  在游戏中，一个物体的动画并不是离散独立的，往往都是几个动画所组成的序列，所以一个健壮的Sequence实现基本是所有游戏必不可少的组件，譬如cocos2d-x就为此提供了CCSequence，但在这点上，iTween却并没有提供类似功能，我们目前只能借助oncomplete回调，或者coroutine之类的机制来模拟实现，让人感觉着实不便~

  *7 代码实现风格不佳

  这一点个人倾向多一点，可能自己OO接触比较多了，对于iTween这种“整一块”的实现方式不是特别认同，感觉身材臃肿、逻辑交纵以外，难以扩展也是一大问题；再者iTween代码实现中有着非常浓重的C#1.0味道，可能是历史原因造成，个人也不是很喜欢~

二. iTween的改进

  说是改进，其实是算不得的，上面提到的很多问题如果真要修改，很多也都是“伤筋动骨”的活儿，真搞起来可能还得不偿失，在此我仅仅写了一个适用于iTween的Hashtable，意在减轻参数类型不够安全等问题带来的影响，实现很简单，就直接贴代码了：

using System;

using System.Collections;

using UnityEngine;

class iTweenHashtable {

public delegate void iTweenCallback();

public delegate void iTweenCallbackParam(System.Object param);

Hashtable m_innerTable = new Hashtable();

public iTweenHashtable Name(string name) {

m_innerTable["name"] = name;

    return this;

}

public iTweenHashtable From(float val) {

    m_innerTable["from"] = val;

return this;

}

public iTweenHashtable From(double val) {

    m_innerTable["from"] = val;

return this;

}

public iTweenHashtable From(Vector3 val) {

    m_innerTable["from"] = val;

return this;

}

public iTweenHashtable From(Vector2 val) {

    m_innerTable["from"] = val;

return this;

}

public iTweenHashtable From(Color val) {

    m_innerTable["from"] = val;

return this;

}

public iTweenHashtable From(Rect val) {

    m_innerTable["from"] = val;

return this;

}

public iTweenHashtable To(float val) {

    m_innerTable["to"] = val;

return this;

}

public iTweenHashtable To(double val) {

    m_innerTable["to"] = val;

return this;

}

public iTweenHashtable To(Vector3 val) {

    m_innerTable["to"] = val;

return this;

}

public iTweenHashtable To(Vector2 val) {

    m_innerTable["to"] = val;

return this;

}

public iTweenHashtable To(Color val) {

    m_innerTable["to"] = val;

return this;

}

public iTweenHashtable To(Rect val) {

    m_innerTable["to"] = val;

return this;

}

public iTweenHashtable Amount(Vector3 amount) {

m_innerTable["amount"] = amount;

    return this;

}

public iTweenHashtable Space(Space space) {

    m_innerTable["space"] = space;

return this;

}

public iTweenHashtable Position(Vector3 position) {

    m_innerTable["position"] = position;

return this;

}

public iTweenHashtable Rotation(Vector3 rotation) {

    m_innerTable["rotation"] = rotation;

return this;

}

public iTweenHashtable Scale(Vector3 scale) {

    m_innerTable["scale"] = scale;

return this;

}

public iTweenHashtable IsLocal(bool isLocal) {

    m_innerTable["islocal"] = isLocal;

return this;

}

public iTweenHashtable Time(float time) {

    m_innerTable["time"] = time;

return this;

}

public iTweenHashtable Time(double time) {

    m_innerTable["time"] = time;

return this;

}

public iTweenHashtable Speed(float speed) {

    m_innerTable["speed"] = speed;

return this;

}

public iTweenHashtable Speed(double speed) {

    m_innerTable["speed"] = speed;

return this;

}

public iTweenHashtable Delay(float delay) {

    m_innerTable["delay"] = delay;

return this;

}

public iTweenHashtable Delay(double delay) {

    m_innerTable["delay"] = delay;

return this;

}

public iTweenHashtable EaseType(iTween.EaseType easeType) {

    m_innerTable["easetype"] = easeType;

return this;

}

public iTweenHashtable LoopType(iTween.LoopType loopType) {

m_innerTable["looptype"] = loopType;

return this;

}

public iTweenHashtable OnStart(iTweenCallback onStart) {

    m_innerTable["onstart"] = onStart.Method.Name;

var target = onStart.Target as MonoBehaviour;

AssertUtil.Assert(target != null);

m_innerTable["onstarttarget"] = target.gameObject;

return this;

}

public iTweenHashtable OnStart(iTweenCallbackParam onStart, System.Object param) {

    m_innerTable["onstart"] = onStart.Method.Name;

var target = onStart.Target as MonoBehaviour;

AssertUtil.Assert(target != null);

m_innerTable["onstarttarget"] = target.gameObject;

// NOTE: seems iTween can not handle this correct ...

//       in iTween.CleanArgs, it just do raw element access

AssertUtil.Assert(param != null);

m_innerTable["onstartparams"] = param;

return this;

}

public iTweenHashtable OnUpdate(iTweenCallback onUpdate) {

    m_innerTable["onupdate"] = onUpdate.Method.Name;

var target = onUpdate.Target as MonoBehaviour;

AssertUtil.Assert(target != null);

m_innerTable["onupdatetarget"] = target.gameObject;

return this;

}

public iTweenHashtable OnUpdate(iTweenCallbackParam onUpdate, System.Object param) {

    m_innerTable["onupdate"] = onUpdate.Method.Name;

var target = onUpdate.Target as MonoBehaviour;

AssertUtil.Assert(target != null);

m_innerTable["onupdatetarget"] = target.gameObject;

// NOTE: seems iTween can not handle this correct ...

//       in iTween.CleanArgs, it just do raw element access

AssertUtil.Assert(param != null);

m_innerTable["onupdateparams"] = param;

return this;

}

public iTweenHashtable OnComplete(iTweenCallback onComplete) {

    m_innerTable["oncomplete"] = onComplete.Method.Name;

var target = onComplete.Target as MonoBehaviour;

AssertUtil.Assert(target != null);

m_innerTable["oncompletetarget"] = target.gameObject;

    return this;

}

public iTweenHashtable OnComplete(iTweenCallbackParam onComplete, System.Object param) {

    m_innerTable["oncomplete"] = onComplete.Method.Name;

var target = onComplete.Target as MonoBehaviour;

AssertUtil.Assert(target != null);

m_innerTable["oncompletetarget"] = target.gameObject;

// NOTE: seems iTween can not handle this correct ...

//       in iTween.CleanArgs, it just do raw element access

AssertUtil.Assert(param != null);

m_innerTable["oncompleteparams"] = param;

    return this;

}

public iTweenHashtable IgnoreTimeScale(bool ignoreTimeScale) {

    m_innerTable["ignoretimescale"] = ignoreTimeScale;

return this;

}

public void Clear() {

    m_innerTable.Clear();

}

public static implicit operator Hashtable(iTweenHashtable table) {

return table.m_innerTable;

}

}

  再贴个使用范例，有兴趣的朋友可以看看：

using UnityEngine;

class iTweenTest : MonoBehaviour {

void Start() {

iTweenHashtable paramsTable = new iTweenHashtable();

paramsTable.Name("move_to_test").

        Delay(5).

    Position(new Vector3(10, 0, 0)).Time(5).IsLocal(false).

    OnStart(OnStart).

    OnComplete(OnComplete);

iTween.MoveTo(gameObject, paramsTable);

}

void OnStart() {

    Debug.Log("start time : " + Time.time);

}

void OnComplete() {

    Debug.Log("end time : " + Time.time);

}

}

  网上还有另一个与iTween相似的Unity插件：[Hotween](http://www.holoville.com/hotween/)，相比iTween，Hotween的设计和实现就相对OO的多，其官网上还有一些其与iTween比较的文字，有兴趣的朋友可以仔细看看，个人认为如果项目允许的话，可以考虑使用Hotween来代替iTween，或者说结合使用两者，毕竟就像Hotween作者自己所说那样，Hotween和iTween的定位是有所差异的，提供的功能也是各有不同的，并不存在哪个取代哪个的问题~ ：）

  OK，今天就扯这么多了，有机会下次再见了~

