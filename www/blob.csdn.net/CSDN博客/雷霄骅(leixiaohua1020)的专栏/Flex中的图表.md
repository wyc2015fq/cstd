# Flex中的图表 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月06日 20:16:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：34









1、设计思路

 （1）运用导航条，实现多个图在同一界面；

 （2）实现各种不同的图

2、设计源码

chart.mxml：





**[html]**[view plain](http://blog.csdn.net/you23hai45/article/details/12254487)[copy](http://blog.csdn.net/you23hai45/article/details/12254487)[print](http://blog.csdn.net/you23hai45/article/details/12254487)[?](http://blog.csdn.net/you23hai45/article/details/12254487)

- <?xmlversion="1.0"encoding="utf-8"?>
- <s:Applicationxmlns:fx="http://ns.adobe.com/mxml/2009"
- xmlns:s="library://ns.adobe.com/flex/spark"
- xmlns:mx="library://ns.adobe.com/flex/mx"
- width="100%"height="100%"
- xmlns:charts="mx.charts.*"
- xmlns:series="mx.charts.series.*">
- <s:layout>
- <s:BasicLayout/>
- </s:layout>
- <fx:Declarations>
- 
- </fx:Declarations>
- 
- <fx:Script>
- <![CDATA[
- importmx.collections.ArrayCollection;
- 
- [Bindable]
- privatevarchartArray:ArrayCollection=newArrayCollection([
- {total:"星期一",apple:"78454",banana:"89454",orange:"45122",peach:"65323"},
- {total:"星期二",apple:"57854",banana:"65454",orange:"56122",peach:"78845"},
- {total:"星期三",apple:"56454",banana:"51544",orange:"45121",peach:"95656"},
- {total:"星期四",apple:"95659",banana:"65645",orange:"32323",peach:"42122"},
- {total:"星期五",apple:"54555",banana:"56566",orange:"56232",peach:"56512"},
- {total:"星期六",apple:"54452",banana:"12454",orange:"78454",peach:"26565"},
- {total:"星期日",apple:"23346",banana:"56129",orange:"56444",peach:"23566"},
- ]);
- 
- [Bindable]
- privatevarcandArray:ArrayCollection=newArrayCollection([
- {Date:"2013-01-01",Open:40.75,High:40.75,Low:40.24,Close:40.31},
- {Date:"2013-01-02",Open:39.98,High:40.78,Low:39.97,Close:40.34},
- {Date:"2013-01-03",Open:40.38,High:40.66,Low:40.12,Close:40.63},
- {Date:"2013-01-04",Open:40.49,High:40.99,Low:40.32,Close:40.98},
- {Date:"2013-01-05",Open:40.13,High:40.46,Low:39.65,Close:39.95},
- {Date:"2013-01-06",Open:39.00,High:39.50,Low:38.78,Close:38.62},
- {Date:"2013-01-07",Open:38.68,High:39.34,Low:37.75,Close:38.84},
- {Date:"2013-01-08",Open:38.76,High:38.76,Low:38.03,Close:38.12},
- {Date:"2013-01-09",Open:37.98,High:37.98,Low:36.56,Close:36.69},
- {Date:"2013-01-10",Open:36.61,High:37.56,Low:36.48,Close:36.86}
- ]);
- ]]>
- </fx:Script>
- 
- <mx:TabNavigatorwidth="100%"height="100%"paddingBottom="10"paddingLeft="10"paddingRight="10"paddingTop="10">
- <s:NavigatorContentlabel="饼图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <charts:PieChartid="pie"dataProvider="{chartArray}"height="90%"width="100%"showDataTips="true">
- <charts:series>
- <series:PieSeriesdisplayName="总收入"field="apple"nameField="total"/>
- </charts:series>
- </charts:PieChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="30%"/>
- <charts:LegenddataProvider="{pie}"direction="horizontal"/>
- </mx:HBox>
- 
- </mx:VBox>
- </s:NavigatorContent>
- <s:NavigatorContentlabel="柱状图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <charts:ColumnChartid="column"dataProvider="{chartArray}"showDataTips="true"width="100%"height="90%"fontSize="16">
- <charts:horizontalAxis>
- <charts:CategoryAxiscategoryField="total"displayName="星期"/>
- </charts:horizontalAxis>
- <charts:series>
- <series:ColumnSeriesdisplayName="苹果"yField="apple"xField="total"/>
- <series:ColumnSeriesdisplayName="香蕉"yField="banana"xField="total"/>
- <series:ColumnSeriesdisplayName="橘子"yField="orange"xField="total"/>
- <series:ColumnSeriesdisplayName="桃子"yField="peach"xField="total"/>
- </charts:series>
- </charts:ColumnChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="45%"/>
- <charts:LegenddataProvider="{column}"height="25"fontSize="16"/>
- </mx:HBox>
- </mx:VBox>
- </s:NavigatorContent>
- <s:NavigatorContentlabel="面积图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <charts:AreaChartid="area"dataProvider="{chartArray}"fontSize="16"width="100%"height="90%"showDataTips="true">
- <charts:horizontalAxis>
- <charts:CategoryAxiscategoryField="total"displayName="星期"/>
- </charts:horizontalAxis>
- <charts:series>
- <series:AreaSeriesdisplayName="苹果"yField="apple"xField="total"/>
- <series:AreaSeriesdisplayName="香蕉"yField="banana"xField="total"/>
- <series:AreaSeriesdisplayName="橘子"yField="orange"xField="total"/>
- <series:AreaSeriesdisplayName="桃子"yField="peach"xField="total"/>
- </charts:series>
- </charts:AreaChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="45%"/>
- <charts:LegenddataProvider="{area}"height="25"fontSize="16"/>
- </mx:HBox>
- </mx:VBox>
- </s:NavigatorContent>
- <s:NavigatorContentlabel="折线图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <charts:LineChartid="line"dataProvider="{chartArray}"fontSize="16"width="100%"height="90%"showDataTips="true">
- <charts:horizontalAxis>
- <charts:CategoryAxiscategoryField="total"displayName="星期"/>
- </charts:horizontalAxis>
- <charts:series>
- <series:LineSeriesdisplayName="苹果"yField="apple"xField="total"/>
- <series:LineSeriesdisplayName="香蕉"yField="banana"xField="total"/>
- <series:LineSeriesdisplayName="橘子"yField="orange"xField="total"/>
- <series:LineSeriesdisplayName="桃子"yField="peach"xField="total"/>
- </charts:series>
- </charts:LineChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="45%"/>
- <charts:LegenddataProvider="{line}"height="25"fontSize="16"/>
- </mx:HBox>
- </mx:VBox>
- </s:NavigatorContent>
- <s:NavigatorContentlabel="条状图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <charts:BarChartid="bar"dataProvider="{chartArray}"fontSize="16"width="100%"height="90%"showDataTips="true">
- <charts:verticalAxis>
- <charts:CategoryAxiscategoryField="total"displayName="星期"/>
- </charts:verticalAxis>
- <charts:series>
- <series:BarSeriesdisplayName="苹果"xField="apple"yField="total"/>
- <series:BarSeriesdisplayName="香蕉"xField="banana"yField="total"/>
- <series:BarSeriesdisplayName="橘子"xField="orange"yField="total"/>
- <series:BarSeriesdisplayName="桃子"xField="peach"yField="total"/>
- </charts:series>
- </charts:BarChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="45%"/>
- <charts:LegenddataProvider="{bar}"height="25"fontSize="16"/>
- </mx:HBox>
- </mx:VBox>
- </s:NavigatorContent>
- <s:NavigatorContentlabel="气泡图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <charts:BubbleChartid="bubble"dataProvider="{chartArray}"fontSize="16"showDataTips="true"width="100%"height="90%">
- <charts:series>
- <series:BubbleSeriesdisplayName="苹果"yField="apple"radiusField="banana"/>
- <series:BubbleSeriesdisplayName="橘子"yField="orange"radiusField="banana"/>
- <series:BubbleSeriesdisplayName="桃子"yField="peach"radiusField="banana"/>
- </charts:series>
- </charts:BubbleChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="45%"/>
- <charts:LegenddataProvider="{bubble}"height="25"/>
- </mx:HBox>
- </mx:VBox>
- </s:NavigatorContent>
- <s:NavigatorContentlabel="蜡烛图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <charts:CandlestickChartid="candlestick"dataProvider="{candArray}"fontSize="16"width="100%"height="90%">
- <charts:verticalAxis>
- <mx:LinearAxisid="haxis"baseAtZero="true"title="Price"/>
- </charts:verticalAxis>
- <charts:horizontalAxis>
- <mx:CategoryAxiscategoryField="Date"title="Date"displayName="日期"/>
- </charts:horizontalAxis>
- <charts:horizontalAxisRenderers>
- <mx:AxisRendereraxis="{haxis}"canDropLabels="true"/>
- </charts:horizontalAxisRenderers>
- <charts:series>
- <series:CandlestickSeriesopenField="Open"
- highField="High"
- lowField="Low"
- closeField="Close"/>
- </charts:series>
- </charts:CandlestickChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="45%"/>
- <charts:LegenddataProvider="{candlestick}"height="25"/>
- </mx:HBox>
- </mx:VBox>
- </s:NavigatorContent>
- <s:NavigatorContentlabel="基址图"width="100%"height="100%">
- <mx:VBoxwidth="100%"height="100%">
- <mx:PlotChartid="plot"dataProvider="{chartArray}"fontSize="16"showDataTips="true"width="100%"height="90%">
- <mx:horizontalAxis>
- <mx:CategoryAxiscategoryField="total"displayName="星期"/>
- </mx:horizontalAxis>
- <mx:series>
- <mx:PlotSeriesdisplayName="苹果"yField="apple"xField="total"/>
- <mx:PlotSeriesdisplayName="香蕉"yField="banana"xField="total"/>
- <mx:PlotSeriesdisplayName="橘子"yField="orange"xField="total"/>
- <mx:PlotSeriesdisplayName="桃子"yField="peach"xField="total"/>
- </mx:series>
- </mx:PlotChart>
- <mx:HBoxwidth="100%">
- <s:Labelwidth="45%"/>
- <charts:LegenddataProvider="{plot}"height="25"/>
- </mx:HBox>
- </mx:VBox>
- </s:NavigatorContent>
- </mx:TabNavigator>
- </s:Application>


```
<?xml version="1.0" encoding="utf-8"?>
<s:Application xmlns:fx="http://ns.adobe.com/mxml/2009" 
			   xmlns:s="library://ns.adobe.com/flex/spark" 
			   xmlns:mx="library://ns.adobe.com/flex/mx"
			   width="100%" height="100%" 
			   xmlns:charts="mx.charts.*" 
			   xmlns:series="mx.charts.series.*">
	<s:layout>
		<s:BasicLayout/>
	</s:layout>
	<fx:Declarations>
		
	</fx:Declarations>
	
	<fx:Script>
		<![CDATA[
			import mx.collections.ArrayCollection;
			
			[Bindable]
			private var chartArray:ArrayCollection = new ArrayCollection([
				{total:"星期一",apple:"78454",banana:"89454",orange:"45122",peach:"65323"},
				{total:"星期二",apple:"57854",banana:"65454",orange:"56122",peach:"78845"},
				{total:"星期三",apple:"56454",banana:"51544",orange:"45121",peach:"95656"},
				{total:"星期四",apple:"95659",banana:"65645",orange:"32323",peach:"42122"},
				{total:"星期五",apple:"54555",banana:"56566",orange:"56232",peach:"56512"},
				{total:"星期六",apple:"54452",banana:"12454",orange:"78454",peach:"26565"},
				{total:"星期日",apple:"23346",banana:"56129",orange:"56444",peach:"23566"},
			]);
			
			[Bindable]
			private var candArray:ArrayCollection = new ArrayCollection([
				{ Date: "2013-01-01", Open: 40.75,  High: 40.75, Low: 40.24, Close:40.31},
				{ Date: "2013-01-02", Open: 39.98,  High: 40.78, Low: 39.97, Close:40.34},
				{ Date: "2013-01-03", Open: 40.38,  High: 40.66, Low: 40.12, Close:40.63},
				{ Date: "2013-01-04", Open: 40.49,  High: 40.99, Low: 40.32, Close:40.98},
				{ Date: "2013-01-05", Open: 40.13,  High: 40.46, Low: 39.65, Close:39.95},
				{ Date: "2013-01-06", Open: 39.00,  High: 39.50, Low: 38.78, Close:38.62}, 
				{ Date: "2013-01-07", Open: 38.68,  High: 39.34, Low: 37.75, Close:38.84}, 
				{ Date: "2013-01-08", Open: 38.76,  High: 38.76, Low: 38.03, Close:38.12}, 
				{ Date: "2013-01-09", Open: 37.98,  High: 37.98, Low: 36.56, Close:36.69},                       
				{ Date: "2013-01-10", Open: 36.61,  High: 37.56, Low: 36.48, Close:36.86} 
			]);
 		]]>
	</fx:Script>
	
	<mx:TabNavigator width="100%" height="100%" paddingBottom="10" paddingLeft="10" paddingRight="10" paddingTop="10">
		<s:NavigatorContent label="饼图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<charts:PieChart id="pie" dataProvider="{chartArray}" height="90%" width="100%" showDataTips="true">
					<charts:series>
						<series:PieSeries displayName="总收入" field="apple" nameField="total"/>
					</charts:series>
				</charts:PieChart>
				<mx:HBox width="100%">
					<s:Label width="30%"/>
					<charts:Legend dataProvider="{pie}" direction="horizontal"/>
				</mx:HBox>
				
			</mx:VBox>
		</s:NavigatorContent>
		<s:NavigatorContent label="柱状图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<charts:ColumnChart  id="column" dataProvider="{chartArray}" showDataTips="true" width="100%" height="90%" fontSize="16">
					<charts:horizontalAxis>
						<charts:CategoryAxis categoryField="total" displayName="星期"/>
					</charts:horizontalAxis>
					<charts:series>
						<series:ColumnSeries displayName="苹果" yField="apple" xField="total"/>
						<series:ColumnSeries displayName="香蕉" yField="banana" xField="total"/>
						<series:ColumnSeries displayName="橘子" yField="orange" xField="total"/>
						<series:ColumnSeries displayName="桃子" yField="peach" xField="total"/>
					</charts:series>
				</charts:ColumnChart>
				<mx:HBox width="100%">
					<s:Label width="45%"/>
					<charts:Legend dataProvider="{column}" height="25" fontSize="16"/>
				</mx:HBox>
			</mx:VBox>
		</s:NavigatorContent>
		<s:NavigatorContent label="面积图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<charts:AreaChart id="area" dataProvider="{chartArray}" fontSize="16" width="100%" height="90%" showDataTips="true">
					<charts:horizontalAxis>
						<charts:CategoryAxis categoryField="total" displayName="星期"/>
					</charts:horizontalAxis>
					<charts:series>
						<series:AreaSeries displayName="苹果" yField="apple" xField="total"/>
						<series:AreaSeries displayName="香蕉" yField="banana" xField="total"/>
						<series:AreaSeries displayName="橘子" yField="orange" xField="total"/>
						<series:AreaSeries displayName="桃子" yField="peach" xField="total"/>
					</charts:series>
				</charts:AreaChart>
				<mx:HBox width="100%">
					<s:Label width="45%"/>
					<charts:Legend dataProvider="{area}" height="25" fontSize="16"/>
				</mx:HBox>
			</mx:VBox>
		</s:NavigatorContent>
		<s:NavigatorContent label="折线图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<charts:LineChart id="line" dataProvider="{chartArray}" fontSize="16" width="100%" height="90%" showDataTips="true">
					<charts:horizontalAxis>
						<charts:CategoryAxis categoryField="total" displayName="星期"/>
					</charts:horizontalAxis>
					<charts:series>
						<series:LineSeries displayName="苹果" yField="apple" xField="total"/>
						<series:LineSeries displayName="香蕉" yField="banana" xField="total"/>
						<series:LineSeries displayName="橘子" yField="orange" xField="total"/>
						<series:LineSeries displayName="桃子" yField="peach" xField="total"/>
					</charts:series>
				</charts:LineChart>
				<mx:HBox width="100%">
					<s:Label width="45%"/>
					<charts:Legend dataProvider="{line}" height="25" fontSize="16"/>
				</mx:HBox>
			</mx:VBox>
		</s:NavigatorContent>
		<s:NavigatorContent label="条状图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<charts:BarChart id="bar" dataProvider="{chartArray}" fontSize="16" width="100%" height="90%" showDataTips="true">
					<charts:verticalAxis>
						<charts:CategoryAxis categoryField="total" displayName="星期"/>
					</charts:verticalAxis>
					<charts:series>
						<series:BarSeries displayName="苹果" xField="apple" yField="total"/>
						<series:BarSeries displayName="香蕉" xField="banana" yField="total"/>
						<series:BarSeries displayName="橘子" xField="orange" yField="total"/>
						<series:BarSeries displayName="桃子" xField="peach" yField="total"/>
					</charts:series>
				</charts:BarChart>
				<mx:HBox width="100%">
					<s:Label width="45%"/>
					<charts:Legend dataProvider="{bar}" height="25" fontSize="16"/>
				</mx:HBox>
			</mx:VBox>
		</s:NavigatorContent>
		<s:NavigatorContent label="气泡图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<charts:BubbleChart id="bubble" dataProvider="{chartArray}" fontSize="16" showDataTips="true" width="100%" height="90%">
					<charts:series>
						<series:BubbleSeries displayName="苹果" yField="apple" radiusField="banana"/>
						<series:BubbleSeries displayName="橘子" yField="orange" radiusField="banana"/>
						<series:BubbleSeries displayName="桃子" yField="peach" radiusField="banana"/>
					</charts:series>
				</charts:BubbleChart>
				<mx:HBox width="100%">
					<s:Label width="45%"/>
					<charts:Legend dataProvider="{bubble}" height="25"/>
				</mx:HBox>
			</mx:VBox>
		</s:NavigatorContent>
		<s:NavigatorContent label="蜡烛图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<charts:CandlestickChart id="candlestick" dataProvider="{candArray}" fontSize="16" width="100%" height="90%">
					<charts:verticalAxis>
						<mx:LinearAxis id="haxis" baseAtZero="true" title="Price"/>
					</charts:verticalAxis>
					<charts:horizontalAxis>
						<mx:CategoryAxis categoryField="Date" title="Date" displayName="日期"/>
					</charts:horizontalAxis>
					<charts:horizontalAxisRenderers>
						<mx:AxisRenderer axis="{haxis}" canDropLabels="true"/>
					</charts:horizontalAxisRenderers>
					<charts:series>
						<series:CandlestickSeries  openField="Open" 
												   highField="High" 
												   lowField="Low" 
												   closeField="Close"/>
					</charts:series>
				</charts:CandlestickChart>
				<mx:HBox width="100%">
					<s:Label width="45%"/>
					<charts:Legend dataProvider="{candlestick}" height="25"/>
				</mx:HBox>
			</mx:VBox>
		</s:NavigatorContent>
		<s:NavigatorContent label="基址图" width="100%" height="100%">
			<mx:VBox width="100%" height="100%">
				<mx:PlotChart id="plot" dataProvider="{chartArray}" fontSize="16" showDataTips="true" width="100%" height="90%">
					<mx:horizontalAxis>
						<mx:CategoryAxis categoryField="total" displayName="星期"/>
					</mx:horizontalAxis>
					<mx:series>
						<mx:PlotSeries displayName="苹果" yField="apple" xField="total"/>
						<mx:PlotSeries displayName="香蕉" yField="banana" xField="total"/>
						<mx:PlotSeries displayName="橘子" yField="orange" xField="total"/>
						<mx:PlotSeries displayName="桃子" yField="peach" xField="total"/>
					</mx:series>
				</mx:PlotChart>
				<mx:HBox width="100%">
					<s:Label width="45%"/>
					<charts:Legend dataProvider="{plot}" height="25"/>
				</mx:HBox>
			</mx:VBox>
		</s:NavigatorContent>
	</mx:TabNavigator>
</s:Application>
```


3、运行结果



（1）饼图

![](https://img-blog.csdn.net/20131003150903625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）柱状图

![](https://img-blog.csdn.net/20131003150943234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）面积图

![](https://img-blog.csdn.net/20131003151015156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（4）折线图

![](https://img-blog.csdn.net/20131003151038203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（5）条状图

![](https://img-blog.csdn.net/20131003151108171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（6）气泡图

![](https://img-blog.csdn.net/20131003151217437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（7）蜡烛图

![](https://img-blog.csdn.net/20131003151306531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（8）基址图



![](https://img-blog.csdn.net/20131003151329765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91MjNoYWk0NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

原文地址：[http://blog.csdn.net/you23hai45/article/details/12254487](http://blog.csdn.net/you23hai45/article/details/12254487)






