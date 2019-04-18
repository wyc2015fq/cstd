# unity-android交互坑指引 - weixin_33985507的博客 - CSDN博客
2018年11月15日 15:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：21
Unity接入Android交互的方式
开始接入最初最好在YouTube一个教程上将unity和Android 的交互基本接入方式走一次，然后明白jar方式如何交互的，会少走许多坑
1.使用gradle的方式则需要使用Androidstudio 打出最终包测试. 主要核心在Android下编写sdk 核心流程：有两种
##### a.unity 下实现唤起sdk  unity 下实现Android sdk回调则完成 剩下的unity去操作
##### b.unity实现唤起sdk, android唤起unity脚本挂载函数(这种其实没必要,而且也不好,sdk管sdk，unity 管 unity 没必要 你调我我调你，所以不采用,当然头铁的要弄个arr去搞应该还是可以)
然后更新的话，每次的更新都需要更新到maven (这块需要看看如何将Android sdk发布到maven)上才能测试新功能，这种方式还比较好一点，主要在Android上做开发.unity 中接入控制游戏界面的业务逻辑结合sdk的回调做界面的处理.这种方式（指的是a这种方式）相对来说思路比较清晰不用处理太多的编译问题(没摸过的话当然坑还是很多的).
（1）在unity中合适的地方唤起调用Android 的渠道
核心类 AndroidJavaClass
```
if (Application.platform == RuntimePlatform.Android)
        {
            using (AndroidJavaClass activityClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer"))
            {
                AndroidJavaObject activityContext = activityClass.GetStatic<AndroidJavaObject>("currentActivity");
          
                Debug.Log("has activity");
            }
            AndroidJavaClass cls = new AndroidJavaClass("com.xxx.rad");
            Debug.Log("has found cls");
            //int res = cls.CallStatic<int>("addSum", 8);
            //Debug.Log("res:" + res.ToString());
            //AndroidJavaClass  instance =  cls.GetStatic<AndroidJavaClass>("instance");
            //string res2 = instance.Call<string>("curinfo");
            //Debug.Log("res2:" + res2.ToString());
            AndroidJavaClass javacontext = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
            AndroidJavaObject activity = javacontext.GetStatic<AndroidJavaObject>("currentActivity");
            if (activity != null)
            {
                AndroidJavaObject instance = cls.CallStatic<AndroidJavaObject>("getInstance");
                instance.Call("Login", true);
            }
            else
            {
                Debug.Log("made");
            }
        }
```
（2）实现Android渠道操作完成的回调
核心类AndroidJavaProxy 脚本需要实现sdk的回调
```
class DateCallback : AndroidJavaProxy
    {
        public DateCallback() : base("android.app.DatePickerDialog$OnDateSetListener") {}
        void onDateSet(AndroidJavaObject  object, int year, int monthOfYear, int dayOfMonth)
        {
            selectedDate = new DateTime(year, monthOfYear + 1, dayOfMonth);
        }
    }
```
2.使用arr方式调用不使用gradle有些麻烦
开发流程则是Androidstudio开发出arr包 ,导入只unity 路径 Assets->Plugins->Andorid目录下 为了测试方便gradle中使用gradle 替换用于方便最开始的快速测试 你可以使用gradle构建自动运行脚本进行unity 插件替换，方便测试在unity下arr是否能很好的工作,这种方式还需要扣除arr中的unity-class.jar包, 同样的是两步 需要unity合适的地方唤起,实现arr包的接口，但这样比较麻烦.测试起来都不太好弄，更麻烦的是更新sdk的时候.测试调试都是很繁琐的事. 这种方式没试过，因为想想各种依赖就很难受，各种依赖版本多的时候，你或许根本不知哪个依赖出问题了.
```
task occurNewArr(type: Copy){
    from "/Users/lotawei/Desktop/unityscene/amuseframwork/build/outputs/aar/amuseframwork-debug.aar"
    into "/Users/lotawei/Documents/unity/unityscene/Assets/Plugins/Android/"
    rename 'amuseframwork-debug','app-debug'
}
task deleteOldArr(type: Delete){
    delete "/Users/lotawei/Desktop/unityscene/amuseframwork/build/outputs/aar/amuseframwork-debug.aar"
    delete  "/Users/lotawei/Documents/unity/unityscene/Assets/Plugins/Android/app-debug.aar"
}
```
其中测试的坑吧 所有方法都是通过字符串反射机制实现的，所以呢所有方法sdk写好的，还请千万不要自己写 一定要copy  paste ， 这种如果是项目比较大的游戏，光是编译打包就会很长时间，遇到这种不熟的建议先摸摸门路 Android unity交互的指南（照着弄吧网上教程很多的，指到南的地方，北斗找不到了hha）.将一些环境摸一下，基础环境能跑通了，最好一次性把所有业务写通吧，不然的话所有的更改发布测试你又得再走一遍(不厚道的笑了笑).
###### Tips:明白一些交互原理机制大概是很重要的对你来说.unity 选择gradle方式导出工程支持的并不会将这些你需要的依赖放进unity整包，因为这种方式目的主要是让你以Android studio来构建你的应用依赖管理,但是arr和lib以及resource相关资源会放进来,因此在这套模式构建下切勿重复打入一样的包.额外说一句unity与Android交互是通过unity-classs.jar进行交互的,打包的时候会自动将unity-classs.jar导入的,因此不要在自己的库里面引入unity的jar包去做unity 的封装,这就是给你自己找麻烦。
###### 可能遇到的问题:gradle 找自己的依赖弄死找不到
```
//一定要放到allprojects 下 这个build是整个工程目录下的gradle 放在buildscript等呢你是找不到的，
//因为unity打出来的包跟新建的Android 项目结构有点不一样, 新建的普通的gradle会有两个一个整个工程的 一个app 对应的gradle. 而unity打出的包只有一个gradle
//然后呢一定记得加上google() 和jcenter()
allprojects {    repositories {        maven {            url "自家的maven库地址"        }        google()        jcenter()        flatDir {            dirs 'libs'        }    }}
dependencies{
        implementation   'xxxx.xxx.xxx:mm:1.0.3'
        
}
```
剩下的注意版本号，最低版本和sdk的兼容了.应该没什么大问题了,然后愉快的写代码加测试吧
