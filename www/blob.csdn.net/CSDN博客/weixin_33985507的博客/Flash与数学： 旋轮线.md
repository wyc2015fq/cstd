# Flash与数学： 旋轮线 - weixin_33985507的博客 - CSDN博客
2010年07月31日 09:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
  昨晚在书中看了一种线，波浪形状的，细细看了一下，这种曲线叫旋轮线。它在工业的领域当中有广泛的作用。最近的发现，数学几何曲线大部分应用都经历过历史的验证，也许我们不知不觉已经用了。对于这些理论，真的很佩服历史的先人给我们遗留下来的宝物。
摘录书中的题目： 
 书上题目如此：一个圆在一直线上无滑动地滚动，求圆周上一点的轨迹？
![旋轮线](http://hi.csdn.net/attachment/201007/31/0_1280541440bQjb.gif)
  根据推导求证，繁琐的过程，可以参考大学几何解析可以发现这种曲线求证，它采用 是向量求证推导，有兴趣可以去看看。这种曲线名称叫做旋轮线，又叫摆线。
 它的坐标参数方程
                      x=a（angle-sin(angle))
                      y=a(1-cos(angle))           其中角度值(angle在正无穷和负无穷之间的)
   按老套的做法，绘制这样的曲线也不是难事，我们仅仅需要借用它的坐标值lineTo我们的线。
![](http://hi.csdn.net/attachment/201007/31/0_1280539684O9RX.gif)
简单的代码：
package
{
	 //创建旋轮线
	import flash.display.Sprite;
	import flash.events.*;
	import flash.display.Shape;
	public class Main extends Sprite
	{
		private var pen:Shape=new Shape();//创建画笔用于绘图
		private var startX:Number=10;//绘制图形的开始点x坐标
		private var startY:Number=200;//绘制图形开始点的y坐标
		private var radius:Number=10;
		public function Main()
		{
			addChild(pen);
			CreatCycloid(10);
		}
		
		//创建旋轮线
		private function CreatCycloid(n:int):void
		{
			pen.graphics.lineStyle(1);
			pen.graphics.moveTo(startX,startY);
			for(var i:int=0;i<=360*n;i++)
			{
				var x1:Number=startX+radius*(i*Math.PI/180-Math.sin(-i*Math.PI/180));//利用公式绘制
				var y1:Number=startY+radius*(1-Math.cos(-i*Math.PI/180));
				pen.graphics.lineTo(x1,y1);			
			}
		}
		
		
	}
	
	
}
简单的结束之后，结合动画过程，让其产生一个运动过程。看看绘画过程是如何的。
使用addEventListener(Event.ENTER_FRAME,Run); 监听让其内部进行绘制过程，在这个过程，我们让角度有规律递增，但是如果希望让效果能够快些看到，可以对其角度递增加大，不过lineTo出来的图形会显得没那么精确和圆滑。
package 
{
	//创建旋轮线
	import flash.display.Sprite;
	import flash.events.*;
	import flash.display.Shape;
	public class Main extends Sprite
	{
		private var pen:Shape=new Shape();//创建画笔用于绘图
		private var startX:Number=10;//绘制图形的开始点x坐标
		private var startY:Number=200;//绘制图形开始点的y坐标
		private var radius:Number=10; //半径
		private var angle:int=0;//角度
		public function Main()
		{
			addChild(pen);
			pen.graphics.lineStyle(1);
			pen.graphics.moveTo(startX,startY);
			addEventListener(Event.ENTER_FRAME,Run);
		}
		private function Run(event:Event):void
		{
			angle+=1;
			CreatCycloid(angle);
		}
		//创建旋轮线
		private function CreatCycloid(angle:int):void
		{
			var x1:Number=startX+radius*(angle*Math.PI/180-Math.sin(-angle*Math.PI/180));//利用公式绘制
			var y1:Number=startY+radius*(1-Math.cos(-angle*Math.PI/180));
			pen.graphics.lineTo(x1,y1);
		}
	}
}
除了这个外，还有外旋轮线和内旋轮线之分。它们滚动的轨迹有有不一样，对产生的图形也不一样。
故此，在纯粹的绘图过程中，还是有必要去深入去理解这种曲线背后的意义，这样也不仅仅为了观看而已。
