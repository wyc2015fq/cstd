
# 游戏制作之路（60）使用UGUI的按钮 - 大坡3D软件开发 - CSDN博客

2019年02月01日 10:44:14[caimouse](https://me.csdn.net/caimouse)阅读数：146


前面学习了文本显示，接着下来最常用的就是按钮了，比如构造游戏的菜单就需要使用按钮来组成，还有一些游戏里的建筑物，背包触发按钮等等。
继续使用前面的例子，在这个基础上来添加一个按钮，如下图来添加一个按钮：
![](https://img-blog.csdnimg.cn/20190201104109729.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在主菜单GameObject/UI/Button里创建一个按钮，如下图:
![](https://img-blog.csdnimg.cn/20190201104140601.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
创建完这个按钮之后，可能这个按钮如前面的文本框一样，显示并不符合大小，就可以使用左上角的工具来调整按钮的大小，同时要记得调整按钮的文本框大小，否则显示的文字会过小，进行合适的调整之后，就会显示如上图一样。可以看到按钮是由一个按钮类和文本类构成的，显示的文字是由文本类来负责。如下图所示：
![](https://img-blog.csdnimg.cn/20190201104212771.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
如果要想修改按钮的文本显示，就必须修改按钮类子对象文本框对象。接着下来，在Score.cs脚本里添加一个按钮消息响应函数，如下：
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
    public void TestBtn()
    {
        Debug.Log("TestBtn");
    }
}
```
在这里添加了TestBtn()函数，这个函数是绑定在主摄像机对象上面，因此要按下图来关联按钮与函数：
![](https://img-blog.csdnimg.cn/2019020110431158.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
首先在A处选中按钮，接着在右边滚动显示合适位置，在B处点击加号增加事件处理函数，在C处选择主摄像机对象，在D处选择上面脚本里的函数TestBtn,这样就可以把点击事件与函数执行绑定一起了，运行play模式之下，点击按钮，就会看到如下显示：
![](https://img-blog.csdnimg.cn/20190201104338868.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
到这里就完成了按钮显示与事件响应。
[https://blog.csdn.net/caimouse/article/details/51749579](https://blog.csdn.net/caimouse/article/details/51749579)

