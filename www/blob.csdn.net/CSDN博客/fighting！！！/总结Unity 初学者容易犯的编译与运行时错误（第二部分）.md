# 总结Unity 初学者容易犯的编译与运行时错误（第二部分） - fighting！！！ - CSDN博客
2017年09月03日 10:20:46[dujiahei](https://me.csdn.net/dujiahei)阅读数：527
                
总结Unity 初学者容易犯的编译与运行时错误（第二部分） - 刘老师讲Unity - 51CTO技术博客
http://liuguozhu.blog.51cto.com/9142031/1691362/
文章 [原始出处](http://liuguozhu.blog.51cto.com/9142031/1691362) 、作者信息和本声明。否则将追究法律责任。[http://liuguozhu.blog.51cto.com/9142031/1691362](http://liuguozhu.blog.51cto.com/9142031/1691362)
本人在做线上与线下Unity教学中，经常遇到一些常见的疑问，为了更好的快速进行答复，我平时注意收集了一些典型
的错误与疑问，且附有个人解答，供大家交流之用。不足之处，请大家指明。
    如果后期总结的易错误点，量比较大时我会进行分类处理，更加方便大家查询。
1： Ngui 和 UGUI 用法是一样的么？现在游戏公司还在用NGUI 还是用Unity内置的UI系统?
答： 如果你是Unity学习者，建议你NGUI/uGUI都学习，因为目前旧项目一般都用NGUI, 现在立项的公司很多考虑用uGUI ，毕竟第1是官方技术且是开源的技术，而是后继会不断发展，有潜力。
2： unity3d的 Terrain上无法烘焙树木的阴影，该如何解决？
答：Detail meshes 本身不参与烘焙过程，但是其会对地形的lightmap进行采样。所以，需要对原生的lightmap进行修改，添加阴影。
3： uGUI 中 slider和scorllbar这么相似，分别用在那些地方啊？
答：解释如下： Slider 控件适合做项目中的“音量调节”、“生命数值”显示等方面，注重数值展示方面。ScrollBar 控件一般不建议单独使用，属于“基础性”控件，适合与其他控件组合成功能更加强大的“复合控件”，例如我在视频中讲解的“ScrollRect”控件。（作为用户选择部分）
4:  unity做个项目，安装在手机上运行一会卡死，都不知道从哪里入手找bug，求各位大神帮忙。。。
答：你自己开发一个移动设备上的找Bug的机制： 如下操作： 第1： 使用Application.persistentDataPath 作为保存数据的路径。 自己写一个调试类 （例如： Debuger.cs） 第2：把需要在程序中切入的调试代码，最终写入txt 文件（或者XML文件）
 第3： 在手机死机后，没有关系。重新进入后，进入自己开发的一个专门显示txt 或者XML 中内容的页面，查看程序运行到哪里，最终找到原因。
这种机制原理，适用于所有的移动平台的调试。
5 this.gameObject.transform.position= new Vector3(1F, 2F, 3F); 老师，gameObject和transform,position都是属性，他们之间怎么能引用呢，不是应该通过对象和类来引用的吗?
答：transform 是GameObject 的属性。 而GameObject是不能直接应用 transform 的，没有这个静态属性。（Unity规定）。 所以我们通过this.gameObject. 。 获得这个脚本所在对象（返回GameObject）.然后再获取transform。
6： uGUI 中 老师，我怎么拉不动那中心点，总是拉着整个button或text？
答：你拉不动那个中心点，原因是Unity快捷栏中的“Center”在起作用，它保证中线点就一直在控件的中心位置。 所以请把“Center”点击改为“Pivot”就可以移动控件中心点了。
7： C# 有高内聚低耦合的概念吗？
答："高内聚低耦合"是开发中的一个重要设计与开发原则，C#作为国际上最流行的开发语言之一，当然具备。 本质上其实与语言无关，这是一种开发的宗旨、思想。
8： unity5 可以使用visualstudio 开发代码，哪可以使用Eclipse开发吗？
答：目前Unity 不支持Eclipse 作为代码编辑器插件开发项目。
9： 数组定义好长度之后，想要改变长度怎么办？
答：数组的特点就是，一点定义了长度，就不能更改。如果你要用可变长数组，就学习“Unity编程基础篇”，里面讲解了ArrayList集合。它的长度是可变的集合。其本质内部实现也是用数组，进行多个数组相互倒换实现的。
10： 旋转的局部坐标系与世界坐标系的区别是什么？
答：局部坐标旋转是指以父物体为参考，进行旋转。而世界坐标系以“坐标陀螺”来进行旋转。类似的情况例如：东、南、西、北、是世界坐标系。而前、后、左、右是局部坐标系
11： uGUI 怎么改变Canvas的屏幕大小?
答：两种方法：1：可以推进摄像机，如果是2D开发，则把摄像机的Size 设置的更小些。 2：可以使用代码方式对Canvas 的Scele 做缩放处理。
12: 怎样保存游戏进度? Save 和 Load功能？
答：保存游戏的进度本质就是数据持久化技术的应用。例如我视频中说的Unity提供的Playerfab 。可以把当前玩家的位置信息、状态（分数、血量、**）等保存且持久化即可。如果是复杂的大量数据保存，建议用XML作对象与XML的串行化。具体可以看看我下月发行的《Unity3d/2d
 游戏开发零基础到精通》的，“数据持久化”章节有详细论述。
13: 请问：我想通过脚本实现自定义换肤，如何实现比较好？如何利用Sprite Pakcer打包？
答：
//动态追加控件 GameObject GoCloneObject = newGameObject("Button"); //添加精灵（外观处理） Image image= GoCloneObject.AddComponent<Image>(); Button btn =GoCloneObject.AddComponent<Button>(); image.overrideSprite=Resources.Load("Textures/Sprites/Emoticon
 - Frown", typeof(Sprite))as Sprite;
以上代码可以实现动态生成控件，且自定义“换肤”。UGUI 中严格来说，我们不需要“Sprite Pakcer打包 ”
14: 用vs2013打开创建的C#脚本，调试的时候显示：无法直接启动带有类库输出类型的项目，请在此解决方案中添加一个引用库项目的可执行项目。将这个可执行项目设置为启动项目！我已经设置了那个启动项目，但是还是调试错误?
答：是这样的。 VS只是我们写代码的工具，提供了类方法提示、方法高亮显示等功能。但是脚本的运行，还是需要运行Unity的Play 按钮的。
15: 老师我的VS 是Blend forstudio2013,在unity编辑器设置为VS打开C#脚本时，报错打开的不是有效的项目，这该怎么办啊
答：建议使用VS 2013(官方默认的是MonoDevelop)。你说的VS 版本可能不兼容。
16: 老师，我在unity5.0里面，想播放动画，this.animation.play ();这样不行~
答：API 有所更改。this.GetComponent<Animation>().Play();更多Unity5.0 内容API更新请首先看我博客：[http://liuguozhu.blog.51cto.com/9142031/1631190](http://liuguozhu.blog.51cto.com/9142031/1631190)  
 。
17: text.rectTransform.rect.Set(0, 0, 417, 2*(j * 52.5f + 300));为什么没有效果呢？如果要设置text的hright属性要怎么办？
答：这样定义：txt_Obj.rectTransform.sizeDelta = newVector2(200, 200);//改变宽度与高度。
18: 老师，我问一个问题，我在game视图16:10 Portrait(10:16)的视图下运行游戏，场景里的物体运动都是正常的但是当我把game视图切换到free aspect的时候，场景里的运动物体就都有了“残影”，就像是屏幕没有刷新一样，这个是什么问题呢，我的摄像机用的是正交模式就像是屏幕不刷新了，每一帧都停留到了屏幕上？
答：
1： 这种情况，先考虑在你角色的背后，加一个背景贴图。
2： ngui的图集打的不能太大。
19： 老师，Unity中“预加载”什么意思，如何实现？
答： “预加载”一般分为两种情况：
第1种情况： 转换场景的时候，我们可以使用异步场景加载的技术，来实现。目的是给玩家一个显示画面，在系统后台加载场景数据，等加载完毕，显示整个场景。代码举例如下：
    public class ScenesLoading : MonoBehaviour{
        public Slider   Sli_Progressbar;                   //调用进度条
        private AsyncOperation async;                      //异步对象
        private float curProgress;                         //当前进度
        void Start ()
        {
            StartCoroutine(loadScene());
        }
        //异步读取场景
        IEnumerator loadScene()
        {
            //async =Application.LoadLevelAsync(ConvertNames.SceneNames[GlobalParameterManager.LoadingScenesName]);           
            async =Application.LoadLevelAsync(ConvertNames.GetInstance().ScenesToName(GlobalParameterManager.LoadingScenesName));
            curProgress = async.progress;
            yield return async;
        }
        void Update()
        {             
            if (curProgress <= 1)
            {
                curProgress += 0.01f;
            }
            Sli_Progressbar.value =curProgress;
        }
    }
第2种情况：
  在同一场景中实现“预加载”。
 如果一个场景非常庞大，势必要进行优化。优化最简单的思路可以如下：
1>  遮挡剔除。
2>  LOD 层级细节。
3>  “层消隐”处理
如果以上步骤还不明显的话，可以人为的把整个大场景，划分为几个部分，当“角色”（主角）要进入一块“区域”的时候，可以提前3-20秒进行场景的加载，当“角色”离开这片区域时，我们则可以销毁这片“区域”，以节省计算机或者移动端设备的内容空间。 这就是“预加载”技术。核心代码如下：
        void Start()
        {
            //启动加载大型建筑模型
           StartCoroutine("LoadBigBuilding");
        }
        //本场景进入若干秒后，启动“预加载”大型场景
        IEnumerator LoadBigBuilding()
        {
            yield return newWaitForSeconds(intLoadTime);
            GameObjectgoBuilding_A=Resources.Load("Prefabs/Scenes_A") as GameObject;
            GameObject goCloneBuildingA =GameObject.Instantiate(goBuilding_A, Vector3.zero, Quaternion.identity) asGameObject;
        }
20：  刘老师，There are 2 audio listeners in the scene. Please ensure there is always exactly one audio listener in the scene.控制台的消息怎么删除，不出现这个
答： 这个警告信息告诉我们，场景中存在了2个“音频监听器”，这是Unity不允许的。 解决方法很简单，你只需要把其中一个摄像机的监听给禁止即可。例如下图：
![wKioL1XpZsSTgNibAAJYqdf6RjI654.jpg](http://s3.51cto.com/wyfs02/M00/72/9E/wKioL1XpZsSTgNibAAJYqdf6RjI654.jpg)
21：角色控制器能不能触发OnTriggerEnter等方法？
答：角色控制器（CharacterController）不能应用OnTriggerEnter 等方法，如果需要检测CC(CharacterController),需要对方通过名称或者Tag来触发检测。如果需要CC给刚体施加“力”，可以用  OnControllerColliderHit() 方法实现。
（关于其他更多的初学者常见Unity疑问与解答，后面还会继续总结，敬请期待.）
本文出自 “[刘老师讲Unity](http://liuguozhu.blog.51cto.com/)” 博客，请务必保留此出处[http://liuguozhu.blog.51cto.com/9142031/1691362](http://liuguozhu.blog.51cto.com/9142031/1691362)
