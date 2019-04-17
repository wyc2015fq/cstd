# 解决OCX （ActiveX）事件注册IE事件回调 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月09日 09:37:48[boonya](https://me.csdn.net/boonya)阅读数：818








OCX在事件注册方面C#版本一般实现可能有问题——光使用事件代理注册事件是不行的，需要注册允许COM通信。

##### 引文讲述



                        引文地址：[https://social.msdn.microsoft.com/Forums/ie/en-US/47eccb01-8089-4653-a0ce-45e9ced5e228/addeventlistener-for-activex-events-in-ie11?forum=iewebdevelopment](https://social.msdn.microsoft.com/Forums/ie/en-US/47eccb01-8089-4653-a0ce-45e9ced5e228/addeventlistener-for-activex-events-in-ie11?forum=iewebdevelopment)



[](https://social.msdn.microsoft.com/Forums/Account/Login?ReturnUrl=https%3A%2F%2Fsocial.msdn.microsoft.com%3A443%2FForums%2Fie%2Fen-US%2F47eccb01-8089-4653-a0ce-45e9ced5e228%2Faddeventlistener-for-activex-events-in-ie11%3Fforum%3Diewebdevelopment%26prof%3Drequired)

In IE11, the attachEvent has been dropped in favour of addEventListener.

Nevertheless, when I replace all the attachEvent instances that were used to bind to events triggered by the activeX, such events are no longer triggered.

Here are some html code examples that illustrate the problem. 

(I have created a simple activeX based on an MFC activeX control that simply triggers an event inside OnCreate. This event sends 2 strings as parameters. If you need the code or even the ocx file for the activeX, please let me know.)

**Test 1:**
<!DOCTYPE html><html><head><title>TestEvent Example HTML</title><metacharset="utf-8"><metahttp-equiv="X-UA-Compatible"content="IE=edge"><metaname="description"content=""><metaname="viewport"content="width=device-width, initial-scale=1"><scriptlanguage='javascript'for="testAxEvent"event="testEvent(szType, szValue)">// Test 1 - statically load the script (This is the basis for the hack)// Works on IE8, IE9, IE10 and IE11var MSG = document.getElementById("msg");
			MSG.innerHTML = szType +" : "+ szValue;</script></head><body><table><tr><td><OBJECTID='testAxEvent'CLASSID='CLSID:BA19A985-C93E-491B-B801-2EB7F903C8CE'codebase="testAxEvent.cab"></OBJECT></td></tr><tr><tdheight='30'></td></tr><tr><tdalign=center><fontsize=5><divid='msg'></div></font></tr></table></body></html>


**Test 2:**


<!DOCTYPE html><html><head><title>TestEvent Example HTML</title><metacharset="utf-8"><metahttp-equiv="X-UA-Compatible"content="IE=edge"><metaname="description"content=""><metaname="viewport"content="width=device-width, initial-scale=1"></head><body><table><tr><td><OBJECTID='testAxEvent'CLASSID='CLSID:BA19A985-C93E-491B-B801-2EB7F903C8CE'codebase="testAxEvent.cab"width='120'height='80'></OBJECT></td></tr><tr><tdheight='30'></td></tr><tr><tdalign=center><fontsize=5><divid='msg'></div></font></tr></table></body><script>// Test 2 - attachEvent// Works on IE8, IE9 and IE10// Does not work on IE11function onTestEvent(szType, szValue){var MSG = document.getElementById("msg");
			MSG.innerHTML = szType +" : "+ szValue;}var elem = document.getElementById("testAxEvent");
		elem.attachEvent("testEvent", onTestEvent);</script></html>

**Test 3:**


<!DOCTYPE html><html><head><title>TestEvent Example HTML</title><metacharset="utf-8"><metahttp-equiv="X-UA-Compatible"content="IE=edge"><metaname="description"content=""><metaname="viewport"content="width=device-width, initial-scale=1"></head><body><table><tr><td><OBJECTID='testAxEvent'CLASSID='CLSID:BA19A985-C93E-491B-B801-2EB7F903C8CE'codebase="testAxEvent.cab"width='120'height='80'></OBJECT></td></tr><tr><tdheight='30'></td></tr><tr><tdalign=center><fontsize=5><divid='msg'></div></font></tr></table></body><script>function onTestEvent(szType, szValue){var MSG = document.getElementById("msg");
			MSG.innerHTML = szType +" : "+ szValue;}var elem = document.getElementById("testAxEvent");// Test 3 - addEventListener// Does not work on IE11 !
		elem.addEventListener("testEvent", onTestEvent,true);
		elem.addEventListener("testEvent", onTestEvent,false);</script></html>

Test 1 uses the "for" tag to setup the callback to the activeX event. This is successful on all versions IE8, 9, 10 and 11.

Test 2 uses attachEvent to bind the callback to the object element. The callback is correctly called on IE8, 9 and 10 but not on IE11, as was expected.

Test 3 replaces attachEvent with addEventListener. Still the event is not triggered on IE11.

What am I missing here? Am I not using the correct addEventListener syntax? Does addEventListener supports binding to activeX events at all? Do I need to make changes to the calling conventions on the activeX side?

##### 解决方案

C#处理方法：[https://blog.csdn.net/richerg85/article/details/7399081](https://blog.csdn.net/richerg85/article/details/7399081)

**下面说一下关键步骤：**

1、生产自己的ActiveX控件，相关的Guid需要注意。

2、定义自定义事件：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CvNetVideo.Event
{
   
    /// <summary>
    /// 全屏事件代理
    /// </summary>
    public delegate void FullScreenEvent(bool isFullScreen);

    /// <summary>
    /// 直播结束事件代理
    /// </summary>
    public delegate void RealTimeLiveAllStopEvent();

    /// <summary>
    /// 控件双击事件代理
    /// </summary>
    public delegate void ControlDoubleClickEvent(bool isFullScreen);


    [GuidAttribute("1A585C4D-3371-48dc-AF8A-AFFECC1B0967")]
    [InterfaceTypeAttribute(ComInterfaceType.InterfaceIsIDispatch)]
    public interface ControlEvents
    {
        [DispIdAttribute(0x001)]
        void OnFullScreenEvent(bool isFullScreen);
        [DispIdAttribute(0x002)]
        void OnRealTimeLiveAllStopEvent();
        [DispIdAttribute(0x003)]
        void OnControlDoubleClickEvent(bool isFullScreen);
    }
}
```

3、控件注册引用COM

```
[Guid("2769FF99-A41F-4AC4-99AD-9CABEB8F83D4")]
    //指示应用该属性的对象对COM可见
    [ComVisible(true)]
    //将事件接收接口连接到托管类
    [ComSourceInterfacesAttribute(typeof(ControlEvents))]
    public partial class UCVideo : ActiveXControl, ACDCallBack, ChejiCallBack, MPlayerCallBack
    {
        #region 控件与IE代理事件
        // 全屏或退出全屏事件
        public event FullScreenEvent OnFullScreenEvent;
        // 直播退出事件
        public event RealTimeLiveAllStopEvent OnRealTimeLiveAllStopEvent;
        // 控件双击事件
        public event ControlDoubleClickEvent OnControlDoubleClickEvent;
        #endregion
```

4、引用实例

```
/// <summary>
        /// 全屏或关闭全屏(IE控制全屏)
        /// </summary>
        /// <param name="isFullScreen"></param>
        /// <returns></returns>
        public bool FullScreen(bool isFullScreen)
        {
            // 操作全屏或关闭全屏
            OpenOrCloseFullScreen(isFullScreen);
            // 全屏事件回调注册
            OnFullScreenEvent?.Invoke(isFullScreen);
            return isFullScreen;
        }
```

5、IE调用注册事件

```
<object id="cSharpActiveX" classid="clsid:2769FF99-A41F-4AC4-99AD-9CABEB8F83D4"></object>

    <script language="javascript" for="cSharpActiveX" event="OnFullScreenEvent(isFullScreen)" type="text/javascript">
        alert(isFullScreen ? "全屏事件" : "退出全屏事件");
    </script>
```
MFC可以参考：[https://blog.csdn.net/wu_tongyu/article/details/39530349](https://blog.csdn.net/wu_tongyu/article/details/39530349)[](http://www.jb51.net/article/55294.htm)



