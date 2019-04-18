# 【收藏】Unity3D中脚本的执行顺序和编译顺序 - fighting！！！ - CSDN博客
2017年09月01日 14:23:35[dujiahei](https://me.csdn.net/dujiahei)阅读数：208
                
Unity3D中脚本的执行顺序和编译顺序 - Champ_Keh - 博客园
http://www.cnblogs.com/champ/p/execorder.html
在Unity中可以同时创建很多脚本，并且可以分别绑定到不同的游戏对象上，它们各自都在自己的生命周期中运行。与脚本有关的也就是编译和执行啦，本文就来研究一下Unity中脚本的编译和执行顺序的问题。
### 事件函数的执行顺序
先说一下执行顺序吧。 
官方给出的脚本中事件函数的执行顺序如下图： 
[](http://images.cnblogs.com/cnblogs_com/champ/628988/o_exeOrder.png)
我们可以做一个小实验来测试一下： 
在Hierarchy视图中创建三个游戏对象，在Project视图中创建三条脚本，如下图所示，然后按照顺序将脚本绑定到对应的游戏对象上： 
![struct](http://images.cnblogs.com/cnblogs_com/champ/628988/o_struct.png)
三条脚本的代码完全一样，只是做了一点名称上的区分： 
```
`using``UnityEngine;`
`using``System.Collections;`
`public``class`
```
Scring0
 : MonoBehaviour
```
`{`
`    ``void``Awake()`
`    ``{`
`        ``Debug.Log(`
```
"Script0
 ======= Awake"
```
`);`
`    ``}`
`    ``bool`
```
isUpdate
 =
```
`false``;`
`    ``void``Update()`
`    ``{`
`        ``if``(!isUpdate)`
`        ``{`
`            ``Debug.Log(`
```
"Script0
 ======= Update"
```
`);`
`            `
```
isUpdate
 =
```
`true``;`
`        ``}`
`    ``}`
`    ``bool`
```
isLateUpdate
 =
```
`false``;`
`    ``void``LateUpdate()`
`    ``{`
`        ``if``(!isLateUpdate)`
`        ``{`
`            ``Debug.Log(`
```
"Script0
 ======= LateUpdate"
```
`);`
`            `
```
isLateUpdate
 =
```
`true``;`
`        ``}`
`    ``}`
`}`
```
播放游戏，看看它们的执行顺序。如下图所示，Awake、Update、LateUpdate，无论运行游戏多少次，它们的执行顺序是完全一样的。 
![exeOrderInstance01](http://images.cnblogs.com/cnblogs_com/champ/628988/o_exeOrderInstance01.png)
接着我们再做一个测试，把Script0的Update方法注释掉！！ 
```
`using``UnityEngine;`
`using``System.Collections;`
`public``class`
```
Script0
 : MonoBehaviour
```
`{`
` `
`    ``void`
```
Awake
 ()
```
`    ``{`
`        ``Debug.Log(`
```
"Script0
 ========= Awake"
```
`);`
`    ``}`
` `
```
// 
 bool isUpdate = false;
```
```
// 
 void Update ()
```
```
// 
 {
```
```
//     
 if(!isUpdate)
```
```
//     
 {
```
```
//         
 Debug.Log("Script0 ========= Update");
```
```
//         
 isUpdate = true;
```
```
//     
 }
```
```
// 
 }
```
` `
`    ``bool`
```
isLateUpdate
 =
```
`false``;`
`    ``void``LateUpdate()`
`    ``{`
`        ``if``(!isLateUpdate)`
`        ``{`
`            ``Debug.Log(`
```
"Script0
 ========= LateUpdate"
```
`);`
`            `
```
isLateUpdate
 =
```
`true``;`
`        ``}`
`    ``}`
`}`
```
再次运行游戏，看看它的结果。脚本的执行顺序和以前完全一样，Script0即便删除掉了Update方法，但是它也不会直接执行LateUpdate方法，而是等待Script1和Script2中的Update方法都执行完毕以后，再去执行所有的LateUpdate方法。 
![exeOrderInstance02](http://images.cnblogs.com/cnblogs_com/champ/628988/o_exeOrderInstance02.png)
通过这两个例子，我们就可以很清楚地断定，Unity后台是如何执行脚本的了。每个脚本的Awake、Start、Update、LateUpdate、FixedUpdate等等，所有的方法在后台都会被汇总到一起： 
```
`后台的Awake()`
`{`
`    `
```
//
 这里暂时按照上图中的脚本执行顺序，后面会谈到其实可以自定义该顺序的
```
`    ``脚本2中的Awake();`
`    ``脚本1中的Awake();`
`    ``脚本0中的Awake();`
`}`
```
后台的方法Awake、Update、LateUpdate等等，都是按照顺序，等所有游戏对象上脚本中的Awake执行完毕之后，再去执行Start、Update、LateUpdate等方法的。 
```
`后台的Update()`
`{`
`    `
```
//
 这里暂时按照上图中的脚本执行顺序，后面会谈到其实可以自定义该顺序的
```
`    ``脚本2中的Update();`
`    ``脚本1中的Update();`
`    ``脚本0中的Update();`
`}`
```
### 脚本的执行顺序
然后我们来看看这样一种情况：在脚本0的Awake方法中创建一个立方体对象，然后在脚本2的Awake方法中去获取这个立方体对象。代码如下： 
```
```
//
 Script0.cs
```
`using``UnityEngine;`
`using``System.Collections;`
`public``class`
```
Script0
 : MonoBehaviour
```
`{`
`    ``void`
```
Awake
 ()
```
`    ``{`
`        ``GameObject.CreatePrimitive(PrimitiveType.Cube);`
`    ``}`
`}`
```
//
 Script2.cs
```
`using``UnityEngine;`
`using``System.Collections;`
`public``class`
```
Script2
 : MonoBehaviour
```
`{`
`    ``void`
```
Awake
 ()
```
`    ``{`
`        `
```
GameObject
 go = GameObject.Find(
```
`"Cube"``);`
`        ``Debug.Log(go.name);`
`    ``}`
`}`
```
如果脚本的执行顺序是先执行Script0，然后再执行Script2，那么Script2中的Awake就可以正确地获取到该立方体对象；可是如果脚本的执行顺序是先执行Script2，然后是Script0，那么Script2肯定会报空指针错误的。
那么实际项目中的脚本会非常多，它们的先后执行顺序我们谁也不知道(有人说是按照栈结构来执行的，即后绑定到游戏对象上的脚本先执行。这一点可以从上面的例子中得到，但官方并没有这么说，还得进一步深入研究)。但一般的，建议在Awake方法中创建游戏对象或Resources.Load(Prefab)对象，然后在Start方法中去获取游戏对象或者组件，因为事件函数的执行顺序是固定的，这样就可以确保万无一失了。 
另外，Unity也提供了一个方法来设置脚本的执行顺序，在Edit -> Project Settings -> Script Execution Order菜单项中，可以在Inspector面板中看到如下图所示： 
![Inspector](http://images.cnblogs.com/cnblogs_com/champ/628988/o_Inspector.png)
点击右下角的"+"将弹出下拉窗口，包括游戏中的所有脚本。脚本添加完毕后，可以用鼠标拖动脚本来为脚本排序，脚本名后面的数字也越小，脚本越靠上，也就越先执行。其中的Default Time表示没有设置脚本的执行顺序的那些脚本的执行顺序。 
![Inspector2](http://images.cnblogs.com/cnblogs_com/champ/628988/o_Inspector2.png)
按照上面这张图的设置，我们再来看一下控制台的输出结果，来确认一下我们的设置是否起作用(注意：把Script0脚本中的Update方法取消注释)： 
![exeOrderInstance03](http://images.cnblogs.com/cnblogs_com/champ/628988/o_exeOrderInstance03.png)
### 脚本的编译顺序
关于脚本的编译顺序很是头疼，官方的说法有点模糊，请看官方的解释： 
![phaseFour](http://images.cnblogs.com/cnblogs_com/champ/628988/o_phaseFour.png)
由于脚本的编译顺序会涉及到特殊文件夹，比如上面提到的Plugins、Editor还有Standard Assets等标准的资源文件夹，所以脚本的放置位置就非常重要了。下面用一个例子来说明不同文件夹中的脚本的编译顺序：
![fileStruct](http://images.cnblogs.com/cnblogs_com/champ/628988/o_fileStruct.png)
实际上，如果你细心的话会发现，如果在你的项目中建立如上图所示的文件夹层次结构时，编译项目之后会在项目文件夹中生成一些文件名中包含Editor、firstpass这些字样的项目文件。比如按照上图的文件夹结构，我们打开项目文件夹来看一下产生的项目文件是什么样的？ 
![fileStruct01](http://images.cnblogs.com/cnblogs_com/champ/628988/o_fileStruct01.png)
下面就来详细探讨一下这些个字样是什么意思？它们与脚本的编译顺序有着怎样的联系？
#### 1、首先从脚本语言类型来看，Unity3d支持3种脚本语言，都会被编译成CLI的DLL
如果项目中包含有C#脚本，那么Unity3d会产生以Assembly-CSharp为前缀的工程，名字中包含”vs”的是产生给Vistual Studio使用的，不包含”vs”的是产生给MonoDevelop使用的。 
|项目中的脚本语言|工程前缀|工程后缀|
|----|----|----|
|C#|Assembly-CSharp|csproj|
|UnityScript|Assembly-UnityScript|unityproj|
|Boo|Assembly-Boo|booproj|
如果项目中这三种脚本都存在，那么Unity将会生成3种前缀类型的工程。
#### 2、对于每一种脚本语言，根据脚本放置的位置(其实也部分根据脚本的作用，比如编辑器扩展脚本，就必须放在Editor文件夹下)，Unity会生成4中后缀的工程。其中的firstpass表示先编译，Editor表示放在Editor文件夹下的脚本。
在上面的示例中，我们得到了两套项目工程文件：分别被Virtual Studio和MonoDevelop使用(后缀包不包含vs)，为简单起见，我们只分析vs项目。得到的文件列表如下： 
Assembly-CSharp-filepass-vs.csproj 
Assembly-CSharp-Editor-filepass-vs.csproj 
Assembly-CSharp-vs.csproj 
Assembly-CSharp-Editor-vs.csproj
根据官方的解释，它们的编译顺序如下： 
(1)所有在Standard Assets、Pro Standard Assets或者Plugins文件夹中的脚本会产生一个Assembly-CSharp-filepass-vs.csproj文件，并且先编译； 
(2)所有在Standard Assets/Editor、Pro Standard Assets/Editor或者Plugins/Editor文件夹中的脚本产生Assembly-CSharp-Editor-filepass-vs.csproj工程文件，接着编译； 
(3)所有在Assets/Editor外面的，并且不在(1),(2)中的脚本文件(一般这些脚本就是我们自己写的非编辑器扩展脚本)会产生Assembly-CSharp-vs.csproj工程文件，被编译； 
(4)所有在Assets/Editor中的脚本产生一个Assembly-CSharp-Editor-vs.csproj工程文件，被编译。
之所以按照这样建立工程并按此顺序编译，也是因为DLL间存在的依赖关系所决定的。
好了，到此为止，我们可以很容易地判断出上面举的实例中，脚本的编译顺序(实际上，我已经把顺序写在了脚本的文件名中了)
