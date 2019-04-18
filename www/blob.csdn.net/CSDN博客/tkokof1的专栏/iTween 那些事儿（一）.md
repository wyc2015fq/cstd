# iTween 那些事儿（一） - tkokof1的专栏 - CSDN博客

2014年02月10日 23:39:07[tkokof1](https://me.csdn.net/tkokof1)阅读数：2907


一.引子

  工作中用了不少时间的[iTween](http://itween.pixelplacement.com/index.php)，自己算是有些小小心得，在此随便写写，算作总结吧 ：)

二．iTween的使用

  关于iTween的使用，想必有所了解的朋友自不陌生，网上亦有很多的相关教程供不熟悉的朋友学习了解，这里推荐一下iTween官网的[文档](http://itween.pixelplacement.com/documentation.php)，基本涵盖了我们使用iTween时所需的全部信息，在此容我稍稍重复一下，简单的再写写iTween的基本使用：

  Move操作是游戏中一个基本操作，iTween对此亦提供了不错的支持，大抵上是提供了以下几个API：

  .MoveAdd

  .MoveBy

  .MoveFrom

  .MoveTo

  .MoveUpdate

  就拿MoveTo举例，该接口的作用便是在一段时间以内将目标GameObject由当前位置移动到目标位置，定义上其提供了两个重载版本：

  public static void MoveTo(GameObject target, Vector3 position, float time)

  public static void MoveTo(GameObject target, Hashtable args)

  第一个重载非常直白，方法参数依序便是：目标GameObject、目标位置以及所用时间；但第二个重载就貌似没有这么直接了，第一个参数仍然是目标GameObject，但是第二个参数Hashtable args着实有些让人摸不着头脑，其实对.Net Framework稍有了解的童鞋一定都会知道，Hashtable其实是定义于System.Collections命名空间下的一个容器类型，顾名思义，Hashtable其实就是我们平常一直会遇到的哈希表，而MoveTo接口参数中哈希表所起的作用，其实就是一个参数的容器，也就是说，如果我们先前写了这么一段代码：

  iTween.MoveTo(gameObject, Vector3.zero, 5);

  那么使用如下代码，我们可以达到相同目的：

  Hashtable paramTable = new Hashtable();

  paramTable.Add(“position”, Vector3.zero);

  paramTable.Add(“time”, 5);

  iTween.MoveTo(gameObject, paramTable);

  上述代码略显复杂，使用iTween提供的一个辅助函数Hash(params object[] args) 可以帮助我们简化代码：

  iTween.MoveTo(gameObject, iTween.Hash(

      “position”, Vector3.zero,

      “time”, 5));

  当然，虽然经过了简化，上面代码仍然不及第一个MoveTo重载版本那么简洁，iTween之所以提供了这种略显复杂的重载接口，为的是让使用者可以达到对动画过程的细致控制，就MoveTo而言，除了我们上面提到的目标位置和动画时间之外，还有诸如延迟时间、循环类型甚至回调函数等等参数，如果将他们一一改作重载，势必就会导致组合爆炸，倒不如直接使用一个参数容器统一管理，而实际上，iTween的大部分API都遵循了这个设计原则，上面的MoveTo便是一例，另外的还有诸如ScaleTo、RotateTo等等接口，有兴趣的朋友可以参考官方文档来细致看看 ：）

二. iTween的原理

  简单说了一下使用，接着就让我们来看看iTween的实现原理，仍然以上面的MoveTo举例，让我们再深入看一看iTween是究竟是如何做到让一个GameObject在指定时间内移动到目标位置的：

  自然的，这次我们需要看一看iTween的实现代码了~值得一提的是，iTween的实现方式还是比较奇葩的，整一个C#脚本了事，总计大概7500行代码，通篇也就一个iTween类型实现，个人认为这种实现方式还是值得商榷的……

  好了，闲话少叙，让我们还是以MoveTo为例，来看看他的实现：

  public static void MoveTo(GameObject target, Hashtable args){

      //clean args:

      args = iTween.CleanArgs(args);

  首先iTween使用CleanArgs“清理”了一下刚刚传入的动画参数，至于CleanArgs的具体细节，有兴趣的朋友可以仔细看看，大概的作用就是将参数中的Int32类型和Double类型统统转为Float类型，以及将参数字符串都转为小写形式，总的来说便是统一参数，方便后续处理。

        // ......（省略）

  中间一段iTween特殊处理了Transform类型，我们暂且不管。

        //establish iTween:

args["type"]="move";

args["method"]="to";

Launch(target,args);

}

  最后我们看到iTween在参数表中新添加了“type”和“method”两个参数，而这两个参数便是指示iTween究竟如何执行动画的关键因素，最后iTween调用Launch来真正“启动”动画过程。

  OK，看来Launch应该属于关键函数了，让我们马上来看一看：

    //catalog new tween and add component phase of iTween:

static void Launch(GameObject target, Hashtable args){

if(!args.Contains("id")){

args["id"] = GenerateID();

}

if(!args.Contains("target")){

args["target"] = target;

} 

tweens.Insert(0,args);

target.AddComponent("iTween");

}

  Launch函数其实并不复杂，首先其检查更新了“id”参数和“target”参数，接着向tweens数组中插入了或者说添加了新的“动画”，而这tweens数组其实是iTween的一个类型变量：

  //repository of all living iTweens:

  public static ArrayList tweens = new ArrayList();

  其作用上可以辅助iTween完成一些诸如Pause()、Resume()之类的全局操纵函数，当然他还涉及了一些其他细节，不过对于我们这次的讨论并不十分重要，所以暂时也不管了 ：）

  Launch的最后调用了我们熟悉的AddComponent函数向目标GameObject添加一个iTween组件，看来接下来的故事都应该发生在目标GameObject的这个iTween组件上了。

  我们知道，一个Component被添加至GameObject时，首先会调用组件的Awake()函数，那么，让我们马上来看看iTween的Awake究竟是副什么样子：

    void Awake(){

RetrieveArgs();

        lastRealTime = Time.realtimeSinceStartup; // Added by PressPlay

}

  很简单，没有什么深邃的东西，值得一提的是RetrieveArgs，这个函数很长，但是核心内容却很简单：就是一一取出我们之前设定的各个动画参数，譬如time、name等等，没有设定的一些参数则统一设置为默认值。

  OK，Awake之后便是Start，我们接着来看：

    IEnumerator Start(){

if(delay > 0){

yield return StartCoroutine("TweenDelay");

}

TweenStart();

}

  iTween的Start的使用了协程方式，目的是为了处理delay参数，除此之外，便是调用TweenStart()，而这个方法便是动画执行的核心！

  void TweenStart(){ 

CallBack("onstart");

  首先iTween回调了“onstart”参数指定的GameObject方法，至于Callback函数的细节有兴趣的朋友可以仔细看看，大抵上便是一个SendMessage　：）

if(!loop){//only if this is not a loop

ConflictCheck();

GenerateTargets();

}

　接着iTween调用了ConflictCheck方法和GenerateTargets方法，让我们一一来看：ConflictCheck的作用大致上是防止相似的iTween组件相互冲突，举例而言就是我在上一个MoveTo动画还没有完全结束的时候，又新添了一个MoveTo动画，如果不做特殊处理，这种情况肯定会引起冲突，iTween在这里的解决方法是直接去除其中某个冲突动画，这也给我们在iTween使用上提了一个醒，应该尽量避免上面这种存在冲突的动画使用方式，否则很有可能出现意料之外的动画现象。

  而GenerateTargets则是真正创建动画的地方，大致的内容便是通过“type”和“method”参数（还记得吗？）来创建相应的代理，仍然还是以MoveTo举例，其对应的“type”为“move”，而“method”为“to”，在GenerateTargets方法中，相应的逻辑如下：

  GenerateMoveToTargets();

  apply = new ApplyTween(ApplyMoveToTargets);

  GenerateMoveToTargets实际上就是获取我们设定的目标点位置，而ApplyMoveToTargets则用于实际改变GameObject的位置，在iTween中，这些类似的代理都在GenerateTargets中被创建，并赋值给apply成员。

  好了，iTween中Start的主要内容就是这些了，让我们再来看看Update：

    //non-physics

void Update(){

if(isRunning && !physics){

if(!reverse){

if(percentage<1f){

TweenUpdate();

}else{

TweenComplete(); 

}

}else{

if(percentage>0){

TweenUpdate();

}else{

TweenComplete(); 

}

}

}

}

  逻辑上其实就是根据percentage来调用TweenUpdate和TweenComplete，其他的就没有什么了，OK，最后就让我们来看看这TweenUpdate和TweenComplete的实现：

  void TweenUpdate(){

apply();

CallBack("onupdate");

UpdatePercentage(); 

  }

  TweenUpdate实际上就是执行代理并且处理onupdate回调，当然percentage的更新当然是必不可少的。

  void TweenComplete(){

isRunning=false;

//dial in percentage to 1 or 0 for final run:

if(percentage>.5f){

percentage=1f;

}else{

percentage=0; 

}

//apply dial in and final run:

apply();

if(type == "value"){

CallBack("onupdate"); //CallBack run for ValueTo since it only calculates and applies in the update callback

}

//loop or dispose?

if(loopType==LoopType.none){

Dispose();

}else{

TweenLoop();

}

CallBack("oncomplete");

}

  TweenComplete相对复杂一些，除了像TweenUpdate那般调用代理、处理回调之外，还要额外解决一些诸如loopType、Dispose等逻辑问题，有兴趣的朋友可以仔细看看~

  呼，绕了一大圈，我们总算大概了解了一番iTween的运行机制，如果用简单的几句话来概括的话，大抵就是这个样子：我们通过调用iTween的API（譬如MoveTo）向目标GameObject动态添加了一个iTween组件，而这个组件通过Awake、Start方法创建相应的动画数据，然后在Update（或者FixedUpdate）方法中根据设置参数来改变GameObject相应属性，直到动画结束，iTween组件便自动从GameObject中去除，基本就是这么一个流程，你创建了多少个iTween动画，对应GameObject上也会相应的被添加多少个iTween组件。

  iTween使用上是非常简便的，同样也非常直观，但是我并认为他就是完美无缺的了，在下一篇扯淡中（如果有的话），我会尝试写写他的一些缺点以及一点点可能的改进之处，OK，就这样了~

