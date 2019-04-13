
# unity3d动态创建一个文本 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d动态创建一个文本](https://www.cnblogs.com/eniac1946/p/7553063.html)
|Posted on|2017-09-19 17:43|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7553063)|[收藏](#)
|2D文本需要Canvas和EventSystem，最好使用Editor来添加；
|动态显示一个文本，采用3D Text的方式：
|GameObject text = new GameObject();
        text.transform.position = Camera.main.transform.position + Camera.main.transform.forward * 5.0f;
        //text.transform.position = Vector3.zero;
        text.AddComponent<TextMesh>();
        var style = text.GetComponent<TextMesh>();
        style.text = "3D Text";
        style.fontSize = 50;
        //显示大小与fontSize、characterSize、距离有关
        style.characterSize = 0.05f; 
        style.anchor = TextAnchor.MiddleCenter;
|TextMesh结构（属性）如下：
![](https://images2017.cnblogs.com/blog/1181483/201709/1181483-20170919174158853-1601272728.png)








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
