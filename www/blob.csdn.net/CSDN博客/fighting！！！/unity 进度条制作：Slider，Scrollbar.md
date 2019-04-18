# unity 进度条制作：Slider，Scrollbar - fighting！！！ - CSDN博客
2017年08月29日 09:42:45[dujiahei](https://me.csdn.net/dujiahei)阅读数：11344
1.Unity UGUI基础之Slider、Scrollbar - 神码编程 - CSDN博客 http://blog.csdn.net/qq992817263/article/details/51754463
Slider（滑动条）：是一个主要用于形象的拖动以改变目标值的控件，他的最恰当应用是用来改变一个数值，最大值和最小值自定义，拖动滑块可在此之间改变，例如改变声音大小。
![](https://img-blog.csdn.net/20160624170907880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Scrollbar（滚动条）：是一个主要用于形象的拖动以改变目标比例的控件，他的最恰当应用是用来改变一个整体值变为他的指定百分比例，最大值1（100%），最小值0（0%），拖动滑块可在此之间改变，例如改变滚动视野的显示区域。
![](https://img-blog.csdn.net/20160624170940925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
一、Slider组件
![](https://img-blog.csdn.net/20160624170958519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Fill Rect(填充矩形)：滑块与最小值方向所构成的填充区域所要使用的填充矩形，如果滑动条的作用只是用于改变指定值，那么此选项建议置空，这个相比于Scrollbar所多出来的属性主要用于标识从最小值变化到当前值所经过的变化区域，如果用做进度条（显示任务进行进度）的话，这个属性是比Scrollbar多出来的一个优势。
Handle Rect（操作条矩形）：当前值处于最小值与最大值之间比例的显示范围，也就是整个滑条的最大可控制范围。
Direction（方向）：滑动条的方向，从左至右，从上至下还是其他的。
Min Value(最小值)：滑动条的可变化最小值。
Max Value（最大值）：滑动条的可变化最大值。
Whole Numbers（变化值为整型）：勾选此项，拖动滑动条将按整型数（最小为1）进行改变指定值。
Value（值）：当前滑动条对应的值。
On Value Changed：值改变时触发消息。
二、Scrollbar组件：
![](https://img-blog.csdn.net/20160624171021241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Handle Rect（操作条矩形）：当前值处于最小值与最大值之间比例的显示范围，也就是整个滑条的最大可控制范围。
Direction（方向）：滚动条的方向，从左至右，从上至下还是其他的。
Value(值)：当前滚动条对应的值。
Size（操作条矩形长度）：操作条矩形对应的缩放长度。
Numbers Of Steps（指定可滚动的位置数量）：滚动条可滚动的位置数目，为0和1时不生效（事实上只有0个可滚动位置或1个可滚动位置那还叫滚动条吗），例如设为2，则拖动滚动条时滚动条只会处在最小值的位置和最大值的位置，因为他的可滚动位置只有2个，例如设为3，则拖动滚动条时滚动条只会处在最小值的位置、最大值的位置以及中间位置，因为他的可滚动位置只有3个。
On Value Changed：值改变时触发消息。
2.Unity中Slider组件的学习 - 杨勇博客之家 - CSDN博客 http://blog.csdn.net/yy763496668/article/details/51579033
我们下面的一个例子就是通过控制Cube的大小来认识一下Slider 组件 
首先创建一个Slider组件，你在Slider下面可以看到三个子控件，Background就是图中绿色的部分，Fill Area–>Fill 就是红色的部分，粉色的就是移动的小圆滑块。
![这里写图片描述](https://img-blog.csdn.net/20160603144208543)
选中Slider 我们在Inspector面板中，有很多属性： 
1.Derection是用来设置滑块的方向的，我们次案例中的滑块的方向是从左到右的。 
Left To Right 从左到右 
Right To Left 从右到左 
Bottom To Top 从下到上 
Top To Bottom 从上到下
2.MinValue 和MaxValue 
MinValue 和MaxValue 是控制滑块移动的范围 
在我们这个案例中，我们是从1-5的，也就是说MinValue 是1，MaxValue 是100
3.WholeNumber 
WholeNumber用来控制滑块值是否限定为整数数值
4.Value 
Value 就是滑块当前的数值
【注】：在On Value Change 事件被调用的时候 
每当滑块的数值由于拖动被改变时调用，float类型的值会被传递无论WholeNumber属性是否启用。
下面来看一我的程序吧
```
using UnityEngine;
using System.Collections;
using UnityEngine.UI;
public class SliderDemo : MonoBehaviour {
    public GameObject cube;
    public Slider slider;
    // Use this for initialization
    void Start () {
    }
    // Update is called once per frame
    void Update () {
    }
    public void CubeChangeScale() {
        cube.transform.localScale = new Vector3(slider.value, slider.value, slider.value);
    }
}
```
3.【Unity3D_UGUI速成班】——06.Slider - xxxhhhyxy的博客 - CSDN博客 http://blog.csdn.net/xxxhhhyxy/article/details/69053309
界面上的交互除了按钮和文字图片之外，还应该有进度条或者血条。Slider正是这样一个UI控件。
--------------------------
## 6.Slider
## 在Hierarchy栏目下右键后点击UI、再点击Slider。那么在Hierarchy栏目下会多出携带了Slider的Canvas控件和EventSystem控件。Slider下还有Background、Fill Area、Handle SlideArea三个子模块。
![](https://img-blog.csdn.net/20170403221334103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHh4aGhoeXh5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
一、Background
代表了进度条的底色，其实质是处于Sliced模式下的UGUI Image，因为只有Sliced模式才能保证进度条两端的弧度。当Slider的值为0时，呈现的全是Background的颜色，这里就不截图啦。
二、Fill Area
代表Slider值不为0时、非0部分的颜色。
Fill Area其实是个空的UGUI物体，之所以强调它是UGUI，是因为它有Rect Transform，并且其由Rect Transform确定的范围将是进度条可以活动的最大范围。
Fill作为Fill Area的子物体才是这部分的实质，同样是Sliced模式下的UGUI Image。
三、Handle Slide Area
代表了Slider上的进度钮。
和Fill Area一样，Handle Slide Area本身也只是确定进度钮范围的UGUI空物体。
Handle作为其子物体，是一个Simple模式下的UGUI Image。
现在笔者将一个30%的Slider的Background设为黑色，Fill设为红色，Handle设为蓝色，大家看着就明白了。
![](https://img-blog.csdn.net/20170403221434026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHh4aGhoeXh5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
一、Slider
Slider在Inspector中呈现如图：
依次有Interactable、Transition、Navigation、Fill Rect、Handle Rect、Direction、Min Value、Max Value、Whole Numbers、Value和On Value Change（）
![](https://img-blog.csdn.net/20170403221503105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHh4aGhoeXh5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
①  Interactable：是否允许Slider交互的bool变量
②  Transition：和UGUI Button部分是一样的，大家可以往前几讲翻一翻；但这里的区别在于Target Graphic设成了Handle，也就是说这是针对进度钮的、不同条件下的颜色光照。
换而言之，Handle的最终呈现，同时取决于它自己的Image组件和二级父物体Slider上的Transition光照。令Handle为蓝色，令Slider的Normal Color为黄色，那么最终呈现的Handle将是绿色。
③  Navigation：和UGUI Button中的用法相同。Slider和Button之间可以向相互“导航。”被Navigation当下选中的Slider或者Button会处于Highlighted-高亮状态。
④  Fill Rect：该进度条的已填充部分所绑定的Rect组件。能拖进这里的必须要有Rect Transform，即UGUI物体。
⑤  Handle Rect：该进度条的进度钮。
说明了Fill 和Handle都不是绝对的，大家可以试着互换一下二者；笔者不敢试，太邪恶了。
⑥  Direction：Slider的演进方向，默认是（LeftTo Right）从左往右刷满，还可以选的有：
         Right to Left（从右往左）、BottomTo Top（从底至顶）、Top To Bottom（从顶到底）
⑦  Min/Max Value：设定进度条数值的极限值，默认最小为0，最大为1，支持Float类型。
⑧  Whole Numbers：勾选之后，Slider的Value值只能取整数，通常用于MaxValue > 1的情况。
⑨  Value：Slider的当前值。
⑩  On Value Changed（）
已经有了UGUI Button的经验，这里就不难推想一定也是个事件系统。但是Slider的事件系统有严格的限制：此处的方法（函数）的参数必须为float，返回值为void。
好了，好久没有接触UGUI的代码了，让我们再动动手吧。
**[csharp]**[view
 plain](http://blog.csdn.net/xxxhhhyxy/article/details/69053309#)[copy](http://blog.csdn.net/xxxhhhyxy/article/details/69053309#)
- using UnityEngine;  
- using System.Collections;  
- using UnityEngine.UI;  
- publicclass teachSlider : MonoBehaviour {  
- public Slider m_slider;  
- public Image m_image;  
- bool run = true;  
- float  timeCounter = 0;  
- publicvoid f_Change(float i)  
-     {  
-         m_image.transform.eulerAngles = new Vector3(0, 0, i * 360);//当Value值改变时、使m_image不停旋转。
- //    Debug.Log("进度条当前值" + i);
-     }  
- // Use this for initialization
- void Start () {  
- 
-     }  
- 
- // Update is called once per frame
- void Update () {  
- if (!Input.GetMouseButton(0))  
-         {  
- if (timeCounter < 5)  
-             {  
-                 m_slider.value = timeCounter / 5;  //Value是可以代码控制的。
-                 timeCounter += Time.deltaTime;  
-             }  
- else
-             {  
-                 timeCounter = 0;  
-                 m_slider.direction = Slider.Direction.RightToLeft;//5s后，更改Slider的演进方向
-             }  
-         }  
- else
-         {  
-             timeCounter = m_slider.value *5;  
-         }  
-     }  
- }  
这里和UGUIButton中一样，需要将脚本挂到某个物体上，再将物体添加到Slider的On Value Changed事件链中，并选中我们写好的方法函数；脚本的一大堆方法中，符合事件链条件的方法会自然出现在最顶端。
![](https://img-blog.csdn.net/20170403225742018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHh4aGhoeXh5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里向大家展示了Slider中的事件链的模板、以及在代码中可控的Value和direction，这里可以通过鼠标拖动Slider来改变进度。
![](https://img-blog.csdn.net/20170403230634568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHh4aGhoeXh5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
要点：
①f_Change（）方法添加到事件链后，就能够响应人为拖动来修改Value值，可是Update里的timeCounter也会时刻修改Value值，所以笔者才加入了 if ，以鼠标左键点击为判断条件，否则会拖不动Slider，体会一下。
②注意看：在运行5秒、Slider改变方向后，其实右端有一部分不大正常。这是因为默认的Fill Area并不是完全填满整个Slider，右端有个留给Handle的小缺口，大家可以点开来看看（Left,right）=（5,15）。在代码中改动direction后，Fill Area其实并未改变；真正万无一失的办法自然是直接在Slider控件上更改、或者将Fill Area左右相等，例如令Left、=Right=5。
![](https://img-blog.csdn.net/20170406211923386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHh4aGhoeXh5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
