
# GameObject数组 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[GameObject数组](https://www.cnblogs.com/eniac1946/p/7267399.html)
|Posted on|2017-08-01 11:06|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7267399)|[收藏](#)
|一、声明
|GameObject[] rock =|new|GameObject[|4|];
|二、遍历
|可以先声明一个GameObject的数组GameObject[]， 然后把需要遍历的对象放进去，然后可以用下面两种方式遍历：
|1、for(int i = 0 ; i < 数组名.Length ; i++){};
|2、foreach(GameObject obj in 数组名){};
|三、复制出一个gameobject
|void|Duplicate()
{
GameObject selectobj|= GameObject.Find(|"|Cube|"|);
GameObject clone|=|(GameObject)Instantiate(selectobj, selectobj.transform.position, selectobj.transform.rotation);
i|++|;
clone.name|=clone.name+|i;
}
|四、获取gameobject
|1.GameObject.Find()
|通过场景里面的名子或者一个路径直接获取游戏对象。
|GameObject root = GameObject.Find(“GameObject”);
|我觉得如果游戏对象没再最上层，那么最好使用路径的方法，因为有可能你的游戏对象会有重名的情况，路径用“/”符号隔开即可。
|GameObject root = GameObject.Find(“GameObject/Cube”);
|GameObject.Find()使用起来很方便，但是它有个缺陷如下图所示，就是如果你的这个GameObject天生acive = false的话。那么你用GameObject.Find()是永远也无法获取它的对象的。如果对象都获取不到，那么对象身上脚本啊 组件啊啥的都是获取不到的，变成了没有意义的对象。
|2 .Transform.Find()
|还记得上面我说过用GameObject无法获取天生acive = false的游戏对象，如果你用Transform.Find()的话就可以很好的获取，另外Unity还提供了一个Transform.FindChind()的方法，这个方法未来会被unity废弃，大家最好就别用了，用Transform.Find()可以取代。
|如下代码，我们先获取顶级对象root 。接着用Find()去找它的子节点”xxxx”的对象，无论”xxxx”对象是否active = true 都是可以直接找到对象的。
|GameObject root = GameObject.Find(|"|GameObject|"|);      
GameObject xxxx|=  root.transform.Find(|"|xxxx|"|).gameObject;

          xxxx.SetActive(|true|);
|Find()方法只能直接去找子节点，如果你想找 孙节点，那么可以用”/“符号把层级关系隔开，找起来很方便。同样无论”xxxx”对象是否active = true 都是可以直接找到对象的。
|1
|GameObject|cube|=|root|.|transform|.|Find|(|"xxxx/Cube"|)|.|gameObject|;
|

|值得注意的是，unity规定了比如父节点active = true 并且子节点的 active = true 都满足的情况下 才能全部显示。使用Transform.Find()可以很方便的获取游戏对象，因为有了游戏对象，那么它身上的脚本啊组件啊什么的都可以很方便的获取到。
|但是Transform.Find()必须要保证你的顶级父对象的activity = true。举个例子，你做了一个场景有一些地图你在场景里面预先activie = false了， 你希望在游戏中的某个时间点把它们都打开 setActive(true)
|你可以把“map”节点放在一个active = true的GameObject上，无论是关闭 或者 显示 代码中写起来都很方便。 假如你的map节点就是顶级节点，那么它一旦天生acive = false ，那么你将无法得到它的对象，更无法设置它的属性了。
|GameObject root = GameObject.Find(“GameObject”);
|GameObject map =  root.transform.Find(“map”).gameObject;
|map.SetActive(true);
|3. unity 还提供了几个获取游戏对象的方法，但是我个人觉得使用频率不高，这里也简单说两句。
|GameObject.FindGameObjectsWithTag(“tag”)
|GameObject.FindWithTag(“tag”)
|根据一个标记来获取游戏对象，返回一个 或者 一个数组，我个人觉得这个两个方法没啥用，因为既然需要用到标记那么相比这个游戏对象必然是非常特殊的一个，所以我会把它存在内存中。
|Object.FindObjectOfType
|Object.FindObjectsOfType
|Resources.FindObjectsOfTypeAll
|根据一个类型返回Object，比如 GameObject 、Texture、Animation 、甚至还可以是你自己写的一个脚本 的范型。它找起来很方便，可以返回一个 或者一个数组。 我觉得这几个方法其实游戏中也没啥用，不过在编辑器中使用的确实很频繁，比如你要做批量检查场景的工具，查找场景中有没有使用某个特殊类型的对象。 或者查看内存的占用量，看看当前内存中那些Texture没有被释放掉。 等等。
|还有一个方法，如果你知道自对象的索引，还可以用下面的方法来获取，参数是index的索引。
|transform.GetChild(0)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
