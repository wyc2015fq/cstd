# unity 真机调试 - javalzy - 博客园
# [unity 真机调试](https://www.cnblogs.com/javalzy/p/7703954.html)
[【Unity3D】Android和ios真机调试测Profiler](http://blog.csdn.net/swj524152416/article/details/53466413)
http://blog.csdn.net/swj524152416/article/details/53466413
# [Unity5.2.1上Android真机调试环境配置](http://www.cnblogs.com/BobbyWeaver/p/5103530.html)
http://www.cnblogs.com/BobbyWeaver/p/5103530.html
# Unity真机调试:查看安卓和ios的Profile(内含安卓，ios打包教程)
http://www.zhimengzhe.com/IOSkaifa/327034.html
//==========================
mac 真机联调注意事项
1. 代码Build要有如下设置（BuildOptions.Development | BuildOptions.ConnectWithProfiler）
  BuildPipeline.BuildPlayer(editorScenesArr, finalPath, pd.buildTarget, BuildOptions.AcceptExternalModificationsToPlayer | BuildOptions.Development | BuildOptions.ConnectWithProfiler);
2. BuildSetting中的 OtherSetting，中勾选：Enable Internal Profiler*
3. adb forward tcp:34999 localabstract:Unity-com.miquwan.blasting
   中的包名要使用正确。
4. 现次联接不上时，尝试：
a. 手机游戏运行中--重启unity---重开通道--profiler激活 Active profiler

