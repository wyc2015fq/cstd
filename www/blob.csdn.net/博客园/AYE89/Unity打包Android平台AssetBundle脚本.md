
# Unity打包Android平台AssetBundle脚本 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Unity打包Android平台AssetBundle脚本](https://www.cnblogs.com/eniac1946/p/7521666.html)
|Posted on|2017-09-14 17:14|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7521666)|[收藏](#)
|注意，BuildTarget tagetPlatform参数为|BuildTarget.Android
|using UnityEngine;
using UnityEditor;
public class BuildAssetBundlesExample: MonoBehaviour
{
    [MenuItem("Example/Build Asset Bundles")]
    static void BuildABs()
    {
        // Put the bundles in a folder called "ABs" within the Assets folder.
        BuildPipeline.BuildAssetBundles("Assets/ABs", BuildAssetBundleOptions.None, BuildTarget.Android);
    }
}
|BuildTarget 编译目标
|类型：Enumeration
|Target build platform.
|目标编译平台。
|Note: This is an editor class. To use it you have to place your script in Assets/Editor inside your project folder. Editor classes are in the UnityEditor namespace so for C\# scripts you need to add "using UnityEditor;" at the beginning of the script.
|注意：这是一个编辑器类，如果想使用它你需要把它放到工程目录下的Assets/Editor文件夹下。编辑器类在UnityEditor命名空间下。所以当使用C\#脚本时，你需要在脚本前面加上 "using UnityEditor"引用。
|参见：|[BuildPipeline.BuildPlayer](http://www.ceeger.com/Script/BuildPipeline/BuildPipeline.BuildPlayer.html)|.
|Values|值
|[WebPlayer](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.WebPlayer.html)
|Build a web player. // 编译一个网络播放器。
|[WebPlayerStreamed](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.WebPlayerStreamed.html)
|Build a streamed web player. // 编译一个流播放器。
|[DashboardWidget](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.DashboardWidget.html)
|Build an OS X Dashboard widget.
|编译一个OS X Dashboard widget（仪表板小工具）。
|[StandaloneOSXUniversal](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.StandaloneOSXUniversal.html)
|Build an OS X standalone. // 编译一个OS X 独立版。
|[StandaloneOSXPPC](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.StandaloneOSXPPC.html)
|Build an OS X standalone (PowerPC only).
|编译一个OS X 独立版（仅PowerPC处理器）。
|[StandaloneOSXIntel](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.StandaloneOSXIntel.html)
|Build an OS X standalone (Intel only).
|编译一个OS X 独立版（仅Intel处理器）。
|[StandaloneWindows](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.StandaloneWindows.html)
|Build a Windows standalone. // 编译一个Windows独立版。
|[iPhone](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.iPhone.html)
|Build an iPhone player // 编译一个iPhone播放器。
|[PS3](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.PS3.html)
|Build a PS3 Standalone // 编译一个PS3独立版。
|[XBOX360](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.XBOX360.html)
|Build a XBox Standalone // 编译一个XBox独立版。
|[Android](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.Android.html)
|Build an Android .apk standalone app
|编译一个安卓.apk独立版应用。
|[Wii](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.Wii.html)
|Build a Wii standalone // 编译一个Wii独立版。
|[StandaloneWindows64](http://www.ceeger.com/Script/Editor_Enumerations/BuildTarget/BuildTarget.StandaloneWindows64.html)
|Build a Windows 64-bit standalone. // 编译Windows独立版64位。








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
