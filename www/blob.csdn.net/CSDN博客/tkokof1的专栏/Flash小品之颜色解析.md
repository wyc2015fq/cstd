# Flash小品之颜色解析 - tkokof1的专栏 - CSDN博客

2011年12月14日 16:49:16[tkokof1](https://me.csdn.net/tkokof1)阅读数：609


    Flash小品之颜色解析    

  不知不觉已经用了一阵子的AS3，渐渐的对Flash有了不少好感，从一个游戏爱好者的角度出发，我个人更喜欢将Flash看做是一套完备的2D游戏制作引擎，平日兴致所致之时，自己便用它来随便搞些东西，开发效率上确实提高了不少，很多开发上的细节Flash基本都已经提供实现，再加上其完善的编辑功能，我个人非常推崇将其用于原型游戏的开发，有兴趣的朋友不妨一试 ：）   

  近来有机会处理到了Bitmap相关的内容，尤其是颜色部分的处理，感觉Flash提供的方法，譬如BitmapData.getPixel，BitmapChannel等等，都不是特别方便直观，尤其是在需要处理各个颜色分量的情况下。鉴于Flash内置的颜色模式遵循‘a r g b’的排列方式，自己又暂时找不到特别好的处理方法（有识之士请告知一下 ：）），于是自己又重造了一次车轮，在此为记，有兴趣的朋友可以看一看：

```
/*! 
    \file ARGBColor.as
    \brief this is simple color class for color converting
*/

package  
{
	
	public class ARGBColor
	{
		// color value
        private var a:uint;
		private var r:uint;
		private var g:uint;
		private var b:uint;
		
		// default constructor
		public function ARGBColor(value:uint=0)
		{
			SetColor(value);
		}
		
		//! set color value
		public function SetColor(value:uint):void
		{
		    splitColor(value);
		}
		
		//! set color value
		public function SetColorARGB(a:uint, r:uint, g:uint, b:uint):void
		{
			SetA(a);
			SetR(r);
			SetG(g);
			SetB(b);
		}
		
		//! get color value
		public function GetColor():uint
		{
			return mergeColor();
		}
		
		//! set alpha value
		public function SetA(a_:uint):void
		{
		    a = a_;
			a = clamp(a, 0, 0xFF);
		}
		
		//! get alpha value
		public function GetA():uint
		{
			return a;
		}
		
		//! set red value
		public function SetR(r_:uint):void
		{
			r = r_;
			r = clamp(r, 0, 0xFF);
		}
		
		//! get red value
		public function GetR():uint
		{
			return r;
		}
		
		//! set green value
		public function SetG(g_:uint):void
		{
			g = g_;
			g = clamp(g, 0, 0xFF);
		}
		
		//! get green value
		public function GetG():uint
		{
			return g;
		}
		
		//! set blue value
		public function SetB(b_:uint):void
		{
			b = b_;
			b = clamp(b, 0, 0xFF);
		}
		
		//! get blue value
		public function GetB():uint
		{
			return b;
		}
		
		// split color
		private function splitColor(value:uint):void
		{
		    a = ((value >> 24) & 0xFF); // seems in Flash '>>' is signed shfiting
			r = ((value >> 16) & 0xFF);
			g = ((value >> 8) & 0xFF);
			b = (value & 0xFF);
		}
		
		// merge color
		private function mergeColor():uint
		{
			return (a << 24) | (r << 16) | (g << 8) | b;
		}
		
		// clamp helper function
		private function clamp(val:Number, min:Number, max:Number):Number
		{
            return Math.max(min, Math.min(max, val));
        }
		
	}
	
}
```


