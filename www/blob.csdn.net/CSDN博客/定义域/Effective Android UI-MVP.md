# Effective Android UI - MVP - 定义域 - CSDN博客
置顶2016年02月24日 16:31:36[定义域](https://me.csdn.net/u010278882)阅读数：946
> 
FROM: [http://www.slideshare.net/PedroVicenteGmezSnch/effective-android-ui-english](http://www.slideshare.net/PedroVicenteGmezSnch/effective-android-ui-english)
![MVP](https://img-blog.csdn.net/20160224155038291)
![PV](https://img-blog.csdn.net/20160224155054055)
MVP是一种软件设计模式，来自MVC，用于构建用户界面。其中，表示器需要实现全部展示逻辑和数据转换（为了发送适合的信息给视图）。作为UI层和业务逻辑层间的抽象。
![Problem and Usage](https://img-blog.csdn.net/20160224155102571)
MVP 主要解决的问题与测试、解耦、避免重复相关。
MVP 的使用提高了代码的可测试性，因为能够在不执行框架代码的情况下，测试所有的UI代码，仅需要进行单元测试就行。要实现这点，所有的展示逻辑都要转移到表示器中。
![核心](https://img-blog.csdn.net/20160224155111776)
MVP的一个核心是让**表示器中没有Android依赖**。通过从表示器中移除所有Android依赖，就能让表示器的测试很容易。
![实现](https://img-blog.csdn.net/20160224155124995)
使用MVP，`Activity Fragment`和自定义视图需要实现一个表示器**“视图”接口**，然后配置为表示器的视图。
视图去包含“视图”接口中的实现代码，并关联用户动作和表示器。
“视图”接口抽象了表示器中要使用的视图的实现。
![实现例子](https://img-blog.csdn.net/20160224155137274)
![生命周期](https://img-blog.csdn.net/20160224155145995)
使用 MVP 还是得将组件的**生命周期**与表示器关联起来，以便表示器知道视图是否做好工作的准备。
如果需要的话，还需要一些方法来更新表示器，比如”onSavedInstanceState”。
![代理](https://img-blog.csdn.net/20160224155155574)
要实现用户事件 - 比如按钮点击 - 实现接口的实现要将事件**代理**给表示器，由其进行决策。
![导航](https://img-blog.csdn.net/20160224155204977)
对于Activity或Fragment中的**导航**实现，可以抽取为“导航器”。
使用导航器可以避免当用户动作导致Activity打开或者任务栈修改时，视图实现和表示器间的循环调用。
实现这一点，需要通过依赖注入注入 Activity Context。
