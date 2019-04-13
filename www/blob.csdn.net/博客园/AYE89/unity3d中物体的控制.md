
# unity3d中物体的控制 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d中物体的控制](https://www.cnblogs.com/eniac1946/p/7269418.html)
|Posted on|2017-08-01 16:53|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7269418)|[收藏](#)
|一、|[物体的循环移动和旋转](http://blog.csdn.net/limu693992297/article/details/52121236)
|思路：通过对时间的计算，每隔一段时间让物体旋转，实现来回移动。
|float|TranslateSpeed =|0.02f|;|float|TranslateSpeedTime =|0.1f|;|void|Update () {
        TranslateSpeedTime|+=|0.1f|;
        transform.Translate(Vector3.forward|*|TranslateSpeed);|if|(TranslateSpeedTime >|150.0f|)
        {
            transform.Rotate(|0|,|180|,|0|);
            TranslateSpeedTime|=|0.1f|;
        }
    }
|1.首先给物体定义一个初始速度和初始的时间。
|2.然后使时间递增。
|3.通过Translate函数使物体移动。
|4.Vector3.forward 是向前移动的意思，==Vector3（0，0，1）
|*Vector3.up 向上 具体可查看API
|5.if判断，规定一个时间，如若TranslateSpeedTime达到这个时间，让物体沿着Y轴旋转
|并且重置时间，继续调用Update
|即可实现物体的重复移动并旋转。
|让gameobject始终面对镜头
|actor1.transform.rotation =|Camera.main.transform.rotation; 
actor1.transform.Rotate(|0|,|180|,|0|)
|二、|GameObject隐藏/显示
|1）
|gameObject.SetActive (false);
|gameObject.SetActive (true);
|2）
|GameObject.renderer.enabled|//是控制一个物体是否在屏幕上渲染或显示 而物体实际还是存在的 只是想当于隐身 而物体本身的碰撞体还依然存在的
|//API已更新为GameObject.GetComponent<Renderer>().enabled = true/false;  注意FBX没有renderer属性，不能用此方法
|GameObject.Destroy()|//表示移除物体或物体上的组件 代表销毁该物体 实际上该物体的内存并没有立即释放，而是等到这一帧的结束才会真正销毁
|GameObject.SetActive()
|//是否在场景中停用该物体,设置|gameObject.SetActive(false)，则你在场景中用find找不到该物体
|//如果该物体有子物体 你要用|SetActiveRecursively(false) 来控制是否在场景中停用该物体（递归的）
|//SetActive(false)的时候，物体不再渲染，释放占用资源
|Camera.cullingMask|//设置相机的渲染层次，在不需要某个物体的时候，cullingMask中将此物体的layer去掉，但是前提是要规划好layer，不能影响其他不希望隐藏掉的物体。 GameObject.|transform.position = FAR_AWAY|//设置一个无限远的位置，再不需要的时候就将物体移动至这个位置，但是这样物体并没有释放，占用的所有资源都会继续占用
|3）|优化：避免频繁调用GameObject.SetActive
|昨天又从主程那学到一招Unity+NGUI性能优化方法。我们游戏的某些逻辑会在一帧内频繁调用GameObject.SetActive，显示或隐藏一些对象，数量达到一百多次之多。这类操作的CPU开销很大（尤其是NGUI的UIWidget在激活的时候会做很多初始化工作），而且会触发大量GC。后来我们改变了显示和隐藏对象的方法——让对象一直保持激活状态（activeInHierarchy为true），而原来的SetActive(false)改为将对象移到屏幕外，SetActive(true)改为将对象移回屏幕内。这样性能就好多了。
|三、外部加载组件Object
|1)加载外部AssetBundle
|1|string|text =|"|bonus|"|;|2|string|path =|"|D:\\bonus|"|;|3|AssetBundle assetBundle =|AssetBundle.LoadFromFile(path);|4|Object original =|assetBundle.LoadAsset(text);|5|GameObject gameObject = Object.Instantiate(original)|as|GameObject;|6|gameObject.transform.position = Camera.main.transform.position + Camera.main.transform.forward *|5.0f|;|7|8|gameObject.transform.localEulerAngles =|new|Vector3(0f, 0f, 0f);|9|gameObject.transform.localScale =|new|Vector3(3f, 3f, 3f);|10|gameObject.name =|text;|11|12|//|添加绑定脚本|13|gameObject.AddComponent<MYActivity>();
|2)加载一个纹理
|1|Texture2D LoadImageFrom(|string|path,|int|width=|400|,|int|height=|400|)|2|{|3|//|创建文件读取流|4|FileStream fileStream =|new|FileStream(path, FileMode.Open, FileAccess.Read);|5|fileStream.Seek(|0|, SeekOrigin.Begin);|6|//|创建文件长度缓冲区|7|byte|[] bytes =|new|byte|[fileStream.Length];|8|//|读取文件|9|fileStream.Read(bytes,|0|, (|int|)fileStream.Length);|10|//|释放文件读取流|11|fileStream.Close();|12|fileStream.Dispose();|13|fileStream =|null|;|14|15|//|创建Texture|16|Texture2D texture =|new|Texture2D(width, height);|17|texture.LoadImage(bytes);|18|return|texture;|19|}
|3）加载一个外部脚本/DLL
|1|var|assembly = System.Reflection.Assembly.LoadFile(|@"|D:\color.dll|"|);|2|//|var assembly = System.Reflection.Assembly.Load("Color");  //path：    projectname\project_Data\Managed\|3|var|type = assembly.GetType(|"|Color|"|);|4|GameObject tGO =|new|GameObject(|"|tGo|"|);|5|tGO.AddComponent(type);
|四、调用其他脚本函数
|第一种，被调用脚本函数为static类型，调用时直接用  脚本名.函数名()。很不实用……
|第二种，GameObject.Find("脚本所在物体名").SendMessage("函数名");  此种方法可以调用public和private类型函数
|第三种，GameObject.Find("脚本所在物体名").GetComponent<脚本名>().函数名();此种方法只可以调用public类型函数







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
