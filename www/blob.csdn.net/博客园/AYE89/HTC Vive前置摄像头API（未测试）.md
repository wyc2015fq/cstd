
# HTC Vive前置摄像头API（未测试） - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[HTC Vive前置摄像头API（未测试）](https://www.cnblogs.com/eniac1946/p/7358481.html)
|Posted on|2017-08-14 16:18|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7358481)|[收藏](#)
|/*WebCamTexture：网络摄像头材质
WebCamTexture.Play()  播放；
WebCamTexture.Pause()  暂停；
WebCamTexture.Stop()  停止；*/
//经测试此代码可以使用，当你绑定到游戏物体时尽可以了、
using unityEngine;
using System.Collections;
public class Test : MonoBehaviour {
public string deviceName;//这个虽然public，但无需为其绑定变量，直接运行，默认调用，显示本地摄像机的名称
WebCamTexture webCam;
  
  
// Use this for initialization
void Start () {
 WebCamDevice[] devices = WebCamTexture.devices;
 deviceName = devices[0].name;
 webCam = new WebCamTexture(deviceName, 400, 300, 12);
 renderer.material.mainTexture = webCam;
 webCam.Play();
}
  
// Update is called once per frame
void Update () {
  
}
}
|上述代码，使用标准化的webcam
|更多参考：|[unity3d 中控制手机前后摄像头切换](http://blog.csdn.net/cxtcxt2005/article/details/8163685)
|2.steam论坛中介绍方法：
|It's all wrapped up on the SteamVR_TrackedCamera script included with the SteamVR plugin on the Unity Asset Store:|[http://u3d.as/content/valve-corporation/steam-vr-plugin](https://steamcommunity.com/linkfilter/?url=http://u3d.as/content/valve-corporation/steam-vr-plugin)
|[https://github.com/ValveSoftware/steamvr_unity_plugin/blob/master/Assets/SteamVR/Scripts/SteamVR_TrackedCamera.cs](https://steamcommunity.com/linkfilter/?url=https://github.com/ValveSoftware/steamvr_unity_plugin/blob/master/Assets/SteamVR/Scripts/SteamVR_TrackedCamera.cs)
|We also include a test script to demonstrate usage:
|[https://github.com/ValveSoftware/steamvr_unity_plugin/blob/master/Assets/SteamVR/Extras/SteamVR_TestTrackedCamera.cs](https://steamcommunity.com/linkfilter/?url=https://github.com/ValveSoftware/steamvr_unity_plugin/blob/master/Assets/SteamVR/Extras/SteamVR_TestTrackedCamera.cs)
|3. 用OpenVR的API打开，此法不是在unity环境中
|4.自定义的plugin
|It's a standard webcam/capture device. I'd imagine you can use the built in webcam texture system from unity to access it.
|I personally use my own plugin (|[http://popmovie.xyz/](http://popmovie.xyz/)|&|[https://www.assetstore.unity3d.com/\#!/content/59097](https://www.assetstore.unity3d.com/#!/content/59097)|) but it's nothing special, anything that uses mediafoundation to capture webcams should be able to cope with it.







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
