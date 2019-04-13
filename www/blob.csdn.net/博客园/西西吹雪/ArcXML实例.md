
# ArcXML实例 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcXML实例](https://www.cnblogs.com/watsonyin/archive/2005/12/31/308749.html)
下面是一些运行中实际用到的arcxml，对于分析arcxml的朋友可能有一点作用。放在这里作为一个备忘录吧。
请求的地址
http://localhost/servlet/com.esri.esrimap.Esrimap?ServiceName=wyxt&ClientVersion=4.0&Form=True&Encode=False
取得服务器信息
<?xml version="1.0" encoding="UTF-8" ?><ARCXML version="1.1">
<REQUEST>
<GET_SERVICE_INFO renderer="false" extensions="true" fields="true" />
</REQUEST>
</ARCXML>
请求地图
<?xml version="1.0" encoding="UTF-8" ?><ARCXML version="1.1">
<REQUEST>
<GET_IMAGE>
<PROPERTIES>
<ENVELOPE minx="392855.56" miny="2495906.28" maxx="505526.65" maxy="2648193.25" />
<IMAGESIZE height="524" width="798" />
<LAYERLIST >
<LAYERDEF id="11" visible="true" />
<LAYERDEF id="10" visible="true" />
<LAYERDEF id="9" visible="true" />
<LAYERDEF id="8" visible="true" />
<LAYERDEF id="7" visible="true" />
<LAYERDEF id="6" visible="true" />
<LAYERDEF id="5" visible="true" />
<LAYERDEF id="2" visible="true" />
<LAYERDEF id="4" visible="true" />
<LAYERDEF id="3" visible="true" />
<LAYERDEF id="1" visible="true" />
<LAYERDEF id="0" visible="true" />
</LAYERLIST>
<BACKGROUND color="255,255,254" transcolor="255,255,254" />
</PROPERTIES>
<LAYER type="acetate" name="theScaleBar">
<OBJECT units="pixel">
<SCALEBAR screenlength="159" coords="478 3" fontcolor="0,0,0" fontstyle="Regular" barcolor="128,128,128" mapunits="meters" scaleunits="meters" antialiasing="True" fontsize="9" barwidth="5" overlap="False"  />
</OBJECT>
<OBJECT units="pixel">
<TEXT coords="552 10" label="1:738184">
<TEXTMARKERSYMBOL fontstyle="Regular" fontsize="8" fontcolor="0,0,0" antialiasing="True" blockout="255,255,255" overlap="false" />
</TEXT>
</OBJECT>
</LAYER>
</GET_IMAGE>
</REQUEST>
</ARCXML>
最简单的获取地图
<?xml version="1.0" encoding="UTF-8" ?><ARCXML version="1.1">
<REQUEST>
<GET_IMAGE>
<PROPERTIES>
</PROPERTIES></GET_IMAGE>
</REQUEST>
</ARCXML>
条件查询
<ARCXML version="1.1">
<REQUEST>
<GET_FEATURES outputmode="xml" geometry="false" envelope="true" checkesc ="true" featurelimit="25" beginrecord="1">
<LAYER id="7" /><SPATIALQUERY subfields="\#ALL\#" where="WYDWDM  =  &apos;A100000000000008&apos;" /></GET_FEATURES></REQUEST></ARCXML>
查询得到的返回值
<?xml version="1.0" encoding="UTF-8"?><ARCXML version="1.1"><RESPONSE><FEATURES><FEATURE><FIELDS MBBSM="1000000045" NAME="棠下小区" ID="B665" WYDWDM="" RKSJ="" SHAPE_AREA="277385.76235" SHAPE_LEN="2470.19760051" \#SHAPE\#="[Geometry]" \#ID\#="36" /></FEATURE><FEATURECOUNT count="1" hasmore="false" /></FEATURES></RESPONSE></ARCXML>
查询得到的返回值2
<?xml version="1.0" encoding="UTF-8"?><ARCXML version="1.1"><RESPONSE><FEATURES><FEATURE><FIELDS MBBSM="1000000001" NAME="粤和居" ID="B676" WYDWDM="A100000000000727" RKSJ="" SHAPE_AREA="3034.13515" SHAPE_LEN="425.433519442" \#SHAPE\#="[Geometry]" \#ID\#="2" /></FEATURE><FEATURE><FIELDS MBBSM="1000000046" NAME="仙湖名苑" ID="B502" WYDWDM="" RKSJ="" SHAPE_AREA="10027.4236485" SHAPE_LEN="404.379855209" \#SHAPE\#="[Geometry]" \#ID\#="37" /></FEATURE><FEATURECOUNT count="2" hasmore="false" /></FEATURES></RESPONSE></ARCXML>




posted on2005-12-31 11:07[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=308749)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
