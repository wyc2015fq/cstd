# Android上实现柱状图表 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年04月29日 19:49:06[gloomyfish](https://me.csdn.net/jia20003)阅读数：15316








**Android上实现柱状图算法实现**

**第一步：**

获取Android设备的屏幕大小

**第二步：**

在View对象中使用Canvas绘制蓝色边框与白色背景XY轴两条线，代码如下

**第三步：**

绘制柱状图标题

**第四步：**

根据数据集计算出每个系列数据所占X轴的大小，来绘制X 数据名称

**第五步：**

根据数据集计算出数据单元大小，并将数据单元映射为像素单元，绘制出标尺单位与

背景虚线

**第六步：**

根据数据集的值来计算出柱状图的高度，以及柱状图的宽度大小，映射为像素值以后

完成绘制。

**程序效果图：**

![](https://img-my.csdn.net/uploads/201204/29/1335700297_2087.png)


**技术点详解：**

**在View中获取Android设备屏幕大小的方法为：**



```java
// get default screen size from system service
WindowManager wm = (WindowManager) this.getContext().getSystemService(Context.WINDOW_SERVICE);
Display display = wm.getDefaultDisplay();
int width = display.getWidth();
```
**在Activity中获取Android设备屏幕大小的方法为：**


```java
DisplayMetrics displaymetrics = new DisplayMetrics();
 getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
 int height = displaymetrics.heightPixels;
 int wwidth = displaymetrics.widthPixels;
```
**计算X轴中每个系列所占大小的代码为：**


```java
int count = series.getSeriesCount();
int xUnit = (width - 2 - xOffset)/count;
```
**其中xOffset, yOffset值计算公式如下：**


```java
int xOffset = (int)(width * 0.1);
int yOffset = (int)(height * 0.1);
```
**计算每个系类中，每个柱状图之间缝隙大小的为：**


```java
int barWidth = (int)(xUnit/Math.pow(itemList.size(),2));
int startPos = xOffset + 2 + xPadding + xUnit*i;
int interval = barWidth/2;
```
其中barWidth表示每个柱状矩形的宽度，interval表示同一数据系列中表示

每个矩形之间的间隔。



**另外一些技巧：**

1.在起始位置填充额外的长度大小，让柱状图不会紧贴Y轴，看上去更美观

默认的xPadding值等于10， int xPadding = 10;

2.使用反锯齿功能，让图形看上去更柔和，启用反锯齿功能的代码为：



```java
myPaint.setAntiAlias(true);
```
3.当要填充一个矩形时候设置Paint的Style



```java
myPaint.setStyle(Style.FILL);
```
当要绘制一个边框矩形时候设置Paint的Style



```java
myPaint.setStyle(Style.STROKE);
```
4.如何绘制虚线(dotted line, dash line),使用DashPathEffect对象

本文中的实现代码如下：



```java
myPaint.setStyle(Style.STROKE);
myPaint.setStrokeWidth(1);
myPaint.setColor(Color.LTGRAY);
myPaint.setPathEffect(new DashPathEffect(new float[] {1,3}, 0));
```

**相关的Class说明：**
DataSeries对象用来构造数据集，根据key来得到对应的数据系列。源代码如下：



```java
package com.gloomyfish;

import java.util.HashMap;
import java.util.List;

public class DataSeries {
	private HashMap<String, List<DataElement>> map;
	
	public DataSeries() {
		map = new HashMap<String, List<DataElement>>();
	}
	
	public void addSeries(String key, List<DataElement> itemList) {
		map.put(key, itemList);
	}
	
	public List<DataElement> getItems(String key) {
		return map.get(key);
	}
	
	public int getSeriesCount() {
		return map.size();
	}
	
	public String[] getSeriesKeys() {
		return map.keySet().toArray(new String[0]);
	}

}
```
DataElement数据元素，属性有数据名称，值大小，显示颜色等，源代码如下：



```java
package com.gloomyfish;

public class DataElement {
	
	public DataElement(String name, float value, int color) {
		this.itemName = name;
		this.value = value;
		this.color = color;
	}
	public String getItemName() {
		return itemName;
	}
	public void setItemName(String itemName) {
		this.itemName = itemName;
	}
	public float getValue() {
		return value;
	}
	public void setValue(float value) {
		this.value = value;
	}
	
	public void setColor(int color) {
		this.color = color;
	}
	
	public int getColor() {
		return this.color;
	}
	
	private String itemName;
	private int color;
	private float value;
}
```
BarChartPanel独立的组件，继承自Android View对象，是柱状图的图形组件，可以为

任何版本的Android使用。源代码如下：



```java
package com.gloomyfish;

import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;

public class BarChartPanel extends  View{
	
	private String plotTitle;
	private DataSeries series;
	public final static int[] platterTable = new int[]{Color.RED, Color.BLUE, Color.GREEN, Color.YELLOW, Color.CYAN};
	public BarChartPanel(Context context, String plotTitle) {
		this(context);
		this.plotTitle = plotTitle;
	}
	
	public BarChartPanel(Context context) {
		super(context);
	}
	
	public void setSeries(DataSeries series) {
		this.series = series;
	}
	
	@Override  
    public void onDraw(Canvas canvas) { 
		
		// get default screen size from system service
		WindowManager wm = (WindowManager) this.getContext().getSystemService(Context.WINDOW_SERVICE);
		Display display = wm.getDefaultDisplay();
		int width = display.getWidth();
		
		// remove application title height
		int height = display.getHeight() - 80; 
		System.out.println("width = " + width);
		System.out.println("height = " + height);
		
		// draw background
		Paint myPaint = new Paint();
		myPaint.setColor(Color.BLUE);
		myPaint.setStrokeWidth(2);
		canvas.drawRect(0, 0, width, height, myPaint);
		myPaint.setColor(Color.WHITE);
		myPaint.setStrokeWidth(0);
		canvas.drawRect(2, 2, width-2, height-2, myPaint);
		
		// draw XY Axis
		int xOffset = (int)(width * 0.1);
		int yOffset = (int)(height * 0.1);
		System.out.println("xOffset = " + xOffset);
		
		myPaint.setColor(Color.BLACK);
		myPaint.setStrokeWidth(2);
		canvas.drawLine(2+xOffset, height-2-yOffset, 2+xOffset, 2, myPaint);
		canvas.drawLine(2+xOffset, height-2-yOffset, width-2, height-2-yOffset, myPaint);
		
		// draw text title
		myPaint.setAntiAlias(true);
		myPaint.setStyle(Style.FILL);
		canvas.drawText(plotTitle, (width-2)/4, 30, myPaint);
		
		// draw data series now......
		if(series == null) {
			getMockUpSeries();
		}
		int xPadding = 10;
		if(series != null) {
		int count = series.getSeriesCount();
		int xUnit = (width - 2 - xOffset)/count;
			String[] seriesNames = series.getSeriesKeys();
			for(int i=0; i<seriesNames.length; i++) {
				canvas.drawText(seriesNames[i], xOffset + 2 + xPadding + xUnit*i, height-yOffset + 10, myPaint);
			}
			
			// Y Axis markers
			float min = 0, max = 0;
			for(int i=0; i<seriesNames.length; i++) {
				List<DataElement> itemList = series.getItems(seriesNames[i]);
				if(itemList != null && itemList.size() > 0) {
					for(DataElement item : itemList) {
						if(item.getValue() > max) {
							max = item.getValue();
						}
						if(item.getValue() < min) {
							min = item.getValue();
						}
					}
				}
			}
			
			int yUnit = 22;	
			int unitValue = (height-2-yOffset)/yUnit;
			myPaint.setStyle(Style.STROKE);
			myPaint.setStrokeWidth(1);
			myPaint.setColor(Color.LTGRAY);
			myPaint.setPathEffect(new DashPathEffect(new float[] {1,3}, 0));
			float ymarkers = (max-min)/yUnit;
			NumberFormat nf = NumberFormat.getInstance();
			nf.setMinimumFractionDigits(2);
			nf.setMaximumFractionDigits(2);
			for(int i=0; i<20; i++) {
				canvas.drawLine(2+xOffset, height-2-yOffset - (unitValue * (i+1)), width-2, height-2-yOffset - (unitValue * (i+1)), myPaint);
			}
			
			// clear the path effect
			myPaint.setColor(Color.BLACK);
			myPaint.setStyle(Style.STROKE);
			myPaint.setStrokeWidth(0);
			myPaint.setPathEffect(null); 
			for(int i=0; i<20; i++) {
				float markValue = ymarkers * (i+1);
				canvas.drawText(nf.format(markValue), 3, height-2-yOffset - (unitValue * (i+1)), myPaint);
			}
			
			// draw bar chart now
			myPaint.setStyle(Style.FILL);
			myPaint.setStrokeWidth(0);
			String maxItemsKey = null;
			int maxItem = 0;
			for(int i=0; i<seriesNames.length; i++) {
				List<DataElement> itemList = series.getItems(seriesNames[i]);
				int barWidth = (int)(xUnit/Math.pow(itemList.size(),2));
				int startPos = xOffset + 2 + xPadding + xUnit*i;
				int index = 0;
				int interval = barWidth/2;
				if(itemList.size() > maxItem) {
					maxItemsKey = seriesNames[i];
					maxItem = itemList.size();
				}
				for(DataElement item : itemList) {
					myPaint.setColor(item.getColor());
					int barHeight = (int)((item.getValue()/ymarkers) * unitValue);
					canvas.drawRect(startPos + barWidth*index + interval*index, height-2-yOffset-barHeight, 
							startPos + barWidth*index + interval*index + barWidth, height-2-yOffset, myPaint);
					index++;
				}
			}
			
			List<DataElement> maxItemList = series.getItems(maxItemsKey);
			int itemIndex = 0;
			int basePos = 10;
			for(DataElement item : maxItemList) {
				myPaint.setColor(item.getColor());
				canvas.drawRect(basePos + itemIndex * 10, height-yOffset + 15, basePos + itemIndex * 10 + 10, height-yOffset + 30, myPaint);
				myPaint.setColor(Color.BLACK);
				canvas.drawText(item.getItemName(), basePos + (itemIndex+1) * 10, height-yOffset + 25, myPaint);
				itemIndex++;
				basePos = basePos + xUnit*itemIndex;
			}
		}
	}
	
    
    public DataSeries getMockUpSeries() {
    	series = new DataSeries();
    	List<DataElement> itemListOne = new ArrayList<DataElement>();
    	itemListOne.add(new DataElement("shoes",120.0f, platterTable[0]));
    	itemListOne.add(new DataElement("jacket",100.0f, platterTable[1]));
    	series.addSeries("First Quarter", itemListOne);
    	
    	List<DataElement> itemListTwo = new ArrayList<DataElement>();
    	itemListTwo.add(new DataElement("shoes",110.0f, platterTable[0]));
    	itemListTwo.add(new DataElement("jacket",50.0f, platterTable[1]));
    	series.addSeries("Second Quarter", itemListTwo);
    	
    	List<DataElement> itemListThree = new ArrayList<DataElement>();
    	itemListThree.add(new DataElement("shoes",100.0f, platterTable[0]));
    	itemListThree.add(new DataElement("jacket",280.0f, platterTable[1]));
    	series.addSeries("Third Quarter", itemListThree);
    	
    	List<DataElement> itemListFour = new ArrayList<DataElement>();
    	itemListFour.add(new DataElement("shoes",120.0f, platterTable[0]));
    	itemListFour.add(new DataElement("jacket",100.0f, platterTable[1]));
    	series.addSeries("Fourth Quarter", itemListFour);
    	return series;
    }

}
```
BarChartDemoActivity测试该组件的Android Activity启动类。



```java
package com.gloomyfish;

import android.app.Activity;
import android.os.Bundle;

public class BarChartDemoActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new BarChartPanel(this, "Quarter Vs. sales volume"));
    }

}
```
**转载请务必注明出处**


