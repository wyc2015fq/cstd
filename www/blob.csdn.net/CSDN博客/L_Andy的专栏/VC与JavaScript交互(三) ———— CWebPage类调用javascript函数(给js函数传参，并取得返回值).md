# VC与JavaScript交互(三) ———— CWebPage类调用javascript函数(给js函数传参，并取得返回值) - L_Andy的专栏 - CSDN博客

2014年10月11日 18:56:37[卡哥](https://me.csdn.net/L_Andy)阅读数：4042


①需要一个别人写好的类CWebPage，将其对于的两个文件WebPage.h和WebPage.cpp添加到工程中。

②添加WebBrowser控件，在视图/对话框类的头文件中#include
 "webbrowser2.h"，增加成员变量m_webBrowser，调用Navigate()函数加载一个html页面。

③在对话框或者视图的实现文件中，加入#include "WebPage.h"，在按钮的响应函数中就可以调用javascript函数了，具体代码如下：

CWebPage  web;

web.SetDocument(m_webBrowser.GetDocument());

CComVariant  varResult; 
 //存放返回值

              //注意：参数要用字符串类型传递过去

web.CallJScript("TansTo", m_latitude,
 m_longitude, &varResult);  //调用JS中的函数TansTo()，并传递2个参数(纬度，经度)，将返回值写入varResult

if( varResult. dblVal == 122.211 )  //
 double型对应dblVal，整形对应intVal

MessageBox(“成功从js中取得变量值”);

④在html文件的script标签中定义对应的函数，具体代码如下：

<script type="text/javascript">

function TansTo(lat, lng)//转到指定位置，有两个参数，纬度和经度

{

map.panTo(new GLatLng(lat, lng));//谷歌地图的API，跳转到坐标指定地点

return 122.211 ;//返回一个double型的值给varResult

}

</script>

注意：

(1)上面javascript中panTo()函数是google map
 API中的一个函数，可根据不同需要自定义相关函数。
(2)以上实现不用管CWebPage类起什么作用，直接用就行，如果想详细了解内容，请搜索有关“COM组件自动化”“IDispatch接口”的文章。

