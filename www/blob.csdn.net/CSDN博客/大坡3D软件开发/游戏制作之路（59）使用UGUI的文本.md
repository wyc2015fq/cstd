
# 游戏制作之路（59）使用UGUI的文本 - 大坡3D软件开发 - CSDN博客

2019年01月31日 14:15:31[caimouse](https://me.csdn.net/caimouse)阅读数：141


在前面学习过使用NGUI的内容，可以兼容旧的游戏代码，但是Unity 5.0之后使用UGUI作为主要游戏界面开发，下面就来学习UGUI相关界面的操作内容，比如游戏里经常要显示分数，那么就来学习一下怎么样实现UGUI来显示分数的过程。
先要创建一个空的3D工程，如下图：
![](https://img-blog.csdnimg.cn/20190131140818856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
接着下来创建一个画布Canvas，画布对象定义了GUI的基本显示区域，它是一个容器，可以在它上面放置按钮、文本和其它部件。如下图来创建一个画布：
![](https://img-blog.csdnimg.cn/2019013114084969.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在主菜单里选择GameObject/UI/Canvas，就可以创建一个画布如下：
![](https://img-blog.csdnimg.cn/20190131140942715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
这时候发现左边Hierarchy里添加了Canvas和EventSystem对象，当你选中画布对象之后，再按下键盘上F键，就可以把画布调整到合适的大小，这时看到画布上显示一个很大的矩形。如果你切换到Game窗口里并不能看到这个画布，因为它在游戏运行之后是不会看得到的，它是一个逻辑组件。接着下来要先修改画布的大小，进行如下设置：
![](https://img-blog.csdnimg.cn/20190131141033708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里选择一个简单的模式，UI Scale Mode选择Scale With Screen Size，表示用户界面根据目标分辨率进行自动拉伸，以保持整体的外观和视觉效果，这是一个简单快捷的UI模式，如果想保持高像素，不要使用这个模式。同时设置屏幕默认分辨率为1920X1080的大小。接着下来在画布上创建一个文本显示控件，如下图：
![](https://img-blog.csdnimg.cn/20190131141102362.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在主菜单里GameObject/UI/Text，就可以创建一个文本显示控件，如下图：
![](https://img-blog.csdnimg.cn/20190131141133974.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里发现文字显示很小，不仔细地查看，是看不见它的。那么就需要调整这个文本显示框的字体大小了，如下图进行调整：
![](https://img-blog.csdnimg.cn/20190131141200252.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
首先选中Text，然后按键盘F键，再按下左上角拉伸模式，然后就可以调整场景里的文本显示框的大小，再调整右边的字体大小，就可以设置合适的文本显示了。在这里把字体大小修改为60，接着下来要调整文本在游戏窗口的右上角进行显示，如下图：
![](https://img-blog.csdnimg.cn/201901311412281.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
先要在A处选择移动工具，然后选中B处文本框，再在C处拖动文本框到画布的右上角，就可以把这个分数显示在右上角了，在Game窗口里查看如下：
![](https://img-blog.csdnimg.cn/2019013114125717.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里终于看到文本显示在右上角上面了，不过这里把颜色也修改为白色显示了。接着下来就是把游戏里的分数显示到这个文本里，为了测试显示，把脚本添加在摄像机里，如下图：
![](https://img-blog.csdnimg.cn/20190131141329514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里添加了脚本Score.cs，脚本里添加一个文本变量，如下修改代码：
```python
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Score : MonoBehaviour {
    public Text ScoreText = null;
    public static int ScoreTotal = 0;
    float Count = 0;
	// Use this for initialization
	void Start () {
        ScoreText.text = ScoreTotal.ToString();
    }
	
	// Update is called once per frame
	void Update () {
        Count += Time.deltaTime;
        if (Count > 3)
        {
            Count = 0;
            ScoreTotal++;
        }
        ScoreText.text = ScoreTotal.ToString();
    }
}
```
最后需要在界面上设置ScoreText变量与UI上文本Text进行关联，如下拖动：
![](https://img-blog.csdnimg.cn/20190131141425314.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
设置这一步之后，就完成了整个UI与代码的关联，就可以进入play模式查看UI显示分数了，如下图：
![](https://img-blog.csdnimg.cn/20190131141450807.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里看到右上角显示分数，并且不断地变化，其它相应的UI组件也这样设置上去显示。
[https://blog.csdn.net/caimouse/article/details/51749579](https://blog.csdn.net/caimouse/article/details/51749579)


