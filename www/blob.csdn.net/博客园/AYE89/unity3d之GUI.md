
# unity3d之GUI - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d之GUI](https://www.cnblogs.com/eniac1946/p/7280390.html)
|Posted on|2017-08-03 16:27|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7280390)|[收藏](#)
|1.Button
|1|void|OnGUI()|2|{|3|if|(GUI.Button(|new|Rect(|80|,|80|,|50|,|20|),|"|up|"|))|4|{flg=|true|; }|5|if|(GUI.Button(|new|Rect(|80|,|120|,|50|,|20|),|"|down|"|))|6|{flg=|false|; }|7|if|(GUI.Button(|new|Rect(|140|,|100|,|50|,|20|),|"|change|"|))|8|{flgChange=!|flgChange; }|9|}
|2.密码登陆
|1|void|OnGUI()|2|{|3|GUI.Label(|new|Rect(Screen.width /|2|-|100|, Screen.height /|2|-|50|,|50|,|20|),|"|ACCOUNT|"|);|4|userName = GUI.TextField(|new|Rect(Screen.width /|2|-|40|, Screen.height /|2|-|50|,|100|,|20|), userName,|15|);|//|15为最大字符串长度|5|//|密码|6|GUI.Label(|new|Rect(Screen.width /|2|-|100|, Screen.height /|2|-|20|,|50|,|20|),|"|PASSWORD|"|);|7|userPassword = GUI.PasswordField(|new|Rect(Screen.width /|2|-|40|, Screen.height /|2|-|20|,|100|,|20|), userPassword,|'|*|'|);|//|'*'为密码遮罩|8|//|信息|9|GUI.Label(|new|Rect(Screen.width /|2|-|100|, Screen.height /|2|+|30|,|100|,|20|), info);|10|//|登录按钮|11|/*|if (GUI.Button(new Rect(80, 80, 50, 20), "登录"))|12|{|13|if (userName == "zuoyamin" && userPassword == "123")|14|{|15|info = "登录成功！";|16|}|17|else|18|{|19|info = "登录失败！";|20|}|21|}|*/|22|}







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
