# flexpaper 参数 - z69183787的专栏 - CSDN博客
2014年01月22日 13:50:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9311
Flexpaper可能用到如下参数
||**SwfFile***(String)*|需要使用Flexpaper打开的文档|
|----|----|----|
||**Scale***(Number)*|初始化缩放比例，参数值应该是大于零的整数|
||**ZoomTransition***(String)*|Flexpaper中缩放样式，它使用和Tweener一样的样式，默认参数值为easeOut.其他可选值包括: easenone, easeout, linear, easeoutquad|
||**ZoomTime***(Number)*|从一个缩放比例变为另外一个缩放比例需要花费的时间，该参数值应该为0或更大。|
||**ZoomInterval***(Number)*|缩放比例之间间隔，默认值为0.1，该值为正数。|
||**FitPageOnLoad***(Boolean)*|初始化得时候自适应页面，与使用工具栏上的适应页面按钮同样的效果。|
||**FitWidthOnLoad***(Boolean)*|初始化的时候自适应页面宽度，与工具栏上的适应宽度按钮同样的效果。|
||**localeChain***(String)*|设置地区（语言），目前支持以下语言。en_US (English) fr_FR (French) zh_CN (Chinese, Simple) es_ES (Spanish) pt_BR (Brazilian Portugese) ru_RU (Russian) fi_FN (Finnish) de_DE (German) nl_NL (Netherlands) tr_TR (Turkish) se_SE (Swedish) pt_PT (Portugese) el_EL (Greek) da_DN (Danish) cz_CS (Czech) it_IT (Italian) pl_PL (Polish) pv_FN (Finnish) hu_HU (Hungarian)|
||**FullScreenAsMaxWindow***(Boolean)*|当设置为true的时候，单击全屏按钮会打开一个flexpaper最大化的新窗口而不是全屏，当由于flash播放器因为安全而禁止全屏，而使用flexpaper作为独立的flash播放器的时候设置为true是个优先选择。|
||**ProgressiveLoading***(Boolean)*|当设置为true的时候，展示文档时不会加载完整个文档，而是逐步加载，但是需要将文档转化为9以上的flash版本（使用pdf2swf的时候使用-T 9 标签）。|
||**MaxZoomSize***(Number)*|设置最大的缩放比例。|
||**MinZoomSize***(Number)*|最小的缩放比例。|
||**SearchMatchAll***(Boolean)*|设置为true的时候，单击搜索所有符合条件的地方高亮显示。|
||**InitViewMode***(String)*|设置启动模式如"Portrait" or "TwoPage".|
||**ViewModeToolsVisible***(Boolean)*|工具栏上是否显示样式选择框。|
||**ZoomToolsVisible***(Boolean)*|工具栏上是否显示缩放工具。|
||**NavToolsVisible***(Boolean)*|工具栏上是否显示导航工具。|
||**CursorToolsVisible***(Boolean)*|工具栏上是否显示光标工具。|
||**SearchToolsVisible***(Boolean)*|工具栏上是否显示搜索。|
