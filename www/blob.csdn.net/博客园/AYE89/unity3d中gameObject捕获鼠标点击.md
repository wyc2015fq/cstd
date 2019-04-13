
# unity3d中gameObject捕获鼠标点击 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d中gameObject捕获鼠标点击](https://www.cnblogs.com/eniac1946/p/7240011.html)
|Posted on|2017-07-26 15:38|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7240011)|[收藏](#)
|gameObject需加上Colider
|一、在update中（推荐）
|void|Update () {|//|左键|if|(Input.GetMouseButtonDown(|0|)) disFlag =|true|;|//|右键|if|(Input.GetMouseButtonDown(|1|)) disFlag =|true|;|//|中键|if|(Input.GetMouseButtonDown(|2|)) disFlag =|true|;
    }
|二、利用|OnMoseOver事件
|在MonoBehaviour类中，有许多预制的事件。这里我们来看看OnMoseOver事件，它的功能其实和Update()方法|很类似。它们的区别在于Update(）在整个游戏运行需要按照Unity内设置的时间来执行，OnMouseOver事件则是当鼠标悬浮在GUI元素和碰撞体上响应。所以我们可以把OnMouseOver看作是update的“子集”！
|void|OnMouseOver()
{|//|触发的标志|}







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
