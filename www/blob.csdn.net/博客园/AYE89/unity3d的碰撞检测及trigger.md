
# unity3d的碰撞检测及trigger - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d的碰撞检测及trigger](https://www.cnblogs.com/eniac1946/p/7232661.html)
|Posted on|2017-07-25 10:30|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7232661)|[收藏](#)
|A、基本概念
|要产生碰撞必须为游戏对象添加刚体（Rigidbody）和碰撞器，刚体可以让物体在物理影响下运动。碰撞体是物理组件的一类，它要与刚体一起添加到游戏对象上才能触发碰撞。如果两个刚体相互撞在一起，除非两个对象有碰撞体时物理引擎才会计算碰撞，在物理模拟中，没有碰撞体的刚体会彼此相互穿过。
|物体发生碰撞的必要条件：
|两个物体都必须带有碰撞器(Collider)，其中一个物体还必须带有Rigidbody刚体。
|在unity3d中，能检测碰撞发生的方式有两种，一种是利用碰撞器，另一种则是利用触发器。
|碰撞器：|一群组件，它包含了很多种类，比如：Box Collider（盒碰撞体），Mesh Collider（网格碰撞体）等，这些碰撞器应用的场合不同，但都必须加到GameObjecet身上。
|触发器|，只需要在检视面板中的碰撞器组件中勾选IsTrigger属性选择框。
![](https://images0.cnblogs.com/i/478792/201408/182033134877508.png)
|触发信息检测：
|1.MonoBehaviour.OnTriggerEnter(Collider collider)当进入触发器
|2.MonoBehaviour.OnTriggerExit(Collider collider)当退出触发器
|3.MonoBehaviour.OnTriggerStay(Collider collider)当逗留触发器
|碰撞信息检测：
|1.MonoBehaviour.OnCollisionEnter(Collision collision) 当进入碰撞器
|2.MonoBehaviour.OnCollisionExit(Collision collision) 当退出碰撞器
|3.MonoBehaviour.OnCollisionStay(Collision collision)  当逗留碰撞器
|B、何时触发？
|下面就是我做的测试情况，都是A物体去撞B物体。
|一、A（碰撞体），B（没有碰撞体，无论有没有刚体），没有触发事件。
|二、A（碰撞体），B（碰撞体），没有触发事件。
|二、A（碰撞体和刚体，开启IsTrigger），B（碰撞体，关闭IsTrigger），双方都能收到触发事件。
|三、A（碰撞体和刚体，关闭IsTrigger），B（碰撞体，开启IsTrigger），双方都能收到触发事件。
|四、A （碰撞体，关闭IsTrigger），B（碰撞体和刚体，开启IsTrigger），没有触发事件。
|五、A （碰撞体，开启IsTrigger），B（碰撞体和刚体，关闭IsTrigger），没有触发事件。
|额外实验：
|六、A（碰撞体和刚体，开启IsTrigger）自由下落，B（碰撞体）撞击A，双方都能收到触发事件。
|根据上面的实验得出，如果要收到触发事件，必须满足如下三个条件：
|1、必须都要有碰撞器组件（Collider），其实上面的碰撞事件同样也需要这个前提条件。
|2、必须有一个物体带刚体组件，并且处于运动状体中（包括主动运动去撞击别人和在运动过程中被别人撞击）。
|3、两个碰撞器中至少有一个开启了IsTrigger。
|此外还有两点：
|一、产生触发事件的两个物体会相互穿越，准确的说是因为开启IsTrigger那个物体会被物理引擎锁忽略掉，所以会产生两个物体穿越的情况。
|二、两个对象要么都收到碰撞事件，要么都收到触发事件，不会出现一个收到触发事件，一个收到碰撞事件的情况。
|还有一个经验点就是，在控制一个物体A去撞另一个物体B时，不要直接去修改物体A的Transform的position属性，因为你会发现如果即使两者发生了碰撞也还是会穿越的，原因就在于我们是直接设置的坐标，如果物体A有刚体组件的话，应该用AddForce，给物体添加一个力，然后让物理引擎去让物体动，此时如果发生了碰撞的话，就不会穿越了。
|碰撞的效果：
|移动的刚体碰撞器VS静止的静态碰撞器
|有碰撞效果，具体为：对于移动的刚体碰撞器在遇到静态碰撞器后会产生物理效果，如反弹、无法前进等；对于静止的静态碰撞器，会产生轻微的碰撞效果，如轻微的晃动，但是不会移动。

|移动的刚体碰撞器VS静止的刚体碰撞器
|有碰撞效果，对于两个物体都出现正常的物理效果，如反弹等。

|移动的静态碰撞器VS静止的运动学刚体碰撞器
|没有任何碰撞效果。运动学刚体碰撞器是不受到任何外力作用的。

|移动的运动学刚体碰撞器VS静止的静态碰撞器
|没有任何碰撞效果。运动学刚体碰撞器是不受到任何外力作用的。

|移动的运动学刚体碰撞器VS静止的刚体碰撞器
|有碰撞效果，但仅刚体碰撞器有碰撞效果，如反弹等，运动学刚体碰撞器不受力的作用。

|移动的刚体碰撞器VS静止的运动学刚体碰撞器
|有碰撞效果，但仅刚体碰撞器有碰撞效果，如反弹等，运动学刚体碰撞器就像一堵墙一样一动不动。

|移动的运动学刚体碰撞器VS静止的运动学刚体碰撞器
|两个太上皇相见了会怎么样呢？没有任何反应……就像两个静态碰撞器一样。
|碰撞信息和触发信息
|对于碰撞时是否会发出碰撞函数（或触发信息），可以见下表。碰撞信息是指OnCollisionEnter() OnCollisionStay()和 OnCollisionExit()这三个函数，而触发信息指的是OnTriggerEnter() OnTriggerStay和OnTriggerExit()三个函数。


|Collision detection occurs and messages are sent upon collision
|碰撞后有碰撞检测并有碰撞信息发出|
|Static Collider
|静态碰撞器|Rigidbody Collider
|刚体碰撞器|Kinematic
|Rigidbody Collider
|运动学刚体碰撞器|Static
|Trigger Collider
|静态触发碰撞器|Rigidbody
|Trigger Collider
|刚体触发碰撞器|Kinematic Rigidbody
|Trigger Collider
|运动学刚体触发碰撞器|
|Static Collider 静态碰撞器|Y|
|Rigidbody Collider 刚体碰撞器|Y|Y|Y|
|Kinematic Rigidbody Collider
|运动学刚体碰撞器|Y|
|Static Trigger Collider
|静态触发碰撞器|
|Rigidbody Trigger Collider
|刚体触发碰撞器|
|Kinematic Rigidbody Trigger Collider
|运动学刚体触发碰撞器|
|Trigger messages are sent upon collision
|碰撞后有触发信息|
|Static Collider
|静态碰撞器|Rigidbody Collider
|刚体碰撞器|Kinematic
|Rigidbody Collider
|运动学刚体碰撞器|Static
|Trigger Collider
|静态触发碰撞器|Rigidbody
|Trigger Collider
|刚体触发碰撞器|Kinematic Rigidbody
|Trigger Collider
|运动学刚体触发碰撞器|
|Static Collider 静态碰撞器|Y|Y|
|Rigidbody Collider 刚体碰撞器|Y|Y|Y|
|Kinematic Rigidbody Collider
|运动学刚体碰撞器|Y|Y|Y|
|Static Trigger Collider 静态触发碰撞器|Y|Y|Y|Y|
|Rigidbody Trigger Collider
|刚体触发碰撞器|Y|Y|Y|Y|Y|Y|
|Kinematic Rigidbody Trigger Collider
|运动学刚体触发碰撞器|Y|Y|Y|Y|Y|Y
|








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
