# C#语法小知识（二十三）重载运算符 - 凯奥斯的注释 - CSDN博客





2016年11月29日 22:49:23[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2083
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# C#里使用Operator关键字定义静态方法来重载运算符。

例如：



```
public class TestPoint
{
	public TestPoint(int x_, int y_)
	{
		x = x_;
		y = y_;
	}
	public int x;
	public int y;
	public static TestPoint operator++ (TestPoint pt)
	{
		return new TestPoint (pt.x + 1, pt.y + 1);
	}
	public static TestPoint operator-- (TestPoint pt)
	{
		return new TestPoint (pt.x - 1, pt.y - 1);
	}
	public static TestPoint operator+ (TestPoint pt1, TestPoint pt2)
	{
		return new TestPoint (pt1.x + pt2.x, pt1.y + pt2.y);
	}
	public static bool operator== (TestPoint pt1, TestPoint pt2)
	{
		return pt1.x == pt2.x && pt1.y == pt2.y;
	}
	public static bool operator!= (TestPoint pt1, TestPoint pt2)
	{
		return !(pt1 == pt2);
	}
	public override string ToString()
	{
		return string.Format("{0} , {1}", this.x, this.y);
	}
	public int this[int idx]
	{
		get {
			if (idx == 0) {
				return x;
			} else if (idx == 1) {
				return y;
			} else {
				throw new System.IndexOutOfRangeException ();
			}
		}
		set { 
			if (idx == 0) {
				x = value;
			} else if (idx == 1) {
				y = value;
			} else {
				throw new System.IndexOutOfRangeException ();
			}
		}
	}
}
```

使用：




```
var tp1 = new TestPoint (1, 2);
		Console.WriteLine (tp1++);
		Console.WriteLine (++tp1);
		Console.WriteLine (tp1);
		var tp2 = new TestPoint (10, 20);
		Console.WriteLine (tp1 + tp2);
		tp1 += tp2;
		Console.WriteLine (tp1);
		tp1 [1] = 55;
		Console.WriteLine (tp1);
		Console.WriteLine (tp2[0]);
		Console.WriteLine (tp1 != tp2);
```

|[+](https://msdn.microsoft.com/zh-cn/library/k1a63xkz.aspx)[-](https://msdn.microsoft.com/zh-cn/library/wch5w409.aspx)[!](https://msdn.microsoft.com/zh-cn/library/f2kd6eb2.aspx)[~](https://msdn.microsoft.com/zh-cn/library/d2bd4x66.aspx)[++](https://msdn.microsoft.com/zh-cn/library/36x43w8w.aspx)[--](https://msdn.microsoft.com/zh-cn/library/wc3z3k8c.aspx)[true](https://msdn.microsoft.com/zh-cn/library/eahhcxk2.aspx)[false](https://msdn.microsoft.com/zh-cn/library/67bxt5ee.aspx)|这些一元运算符可以进行重载。|
|----|----|
|[+](https://msdn.microsoft.com/zh-cn/library/k1a63xkz.aspx)[-](https://msdn.microsoft.com/zh-cn/library/wch5w409.aspx)[*](https://msdn.microsoft.com/zh-cn/library/z19tbbca.aspx)[/](https://msdn.microsoft.com/zh-cn/library/3b1ff23f.aspx)[%](https://msdn.microsoft.com/zh-cn/library/0w4e0fzs.aspx)[&](https://msdn.microsoft.com/zh-cn/library/sbf85k1c.aspx)[|](https://msdn.microsoft.com/zh-cn/library/kxszd0kx.aspx)[^](https://msdn.microsoft.com/zh-cn/library/zkacc7k1.aspx)[<<](https://msdn.microsoft.com/zh-cn/library/a1sway8w.aspx)[>>](https://msdn.microsoft.com/zh-cn/library/xt18et0d.aspx)|这些二元运算符可以进行重载。|
|[==](https://msdn.microsoft.com/zh-cn/library/53k8ybth.aspx)[!=](https://msdn.microsoft.com/zh-cn/library/3tz250sf.aspx)[<](https://msdn.microsoft.com/zh-cn/library/z5wecxwa.aspx)[>](https://msdn.microsoft.com/zh-cn/library/yxk8751b.aspx)[<=](https://msdn.microsoft.com/zh-cn/library/hx063734.aspx)[>=](https://msdn.microsoft.com/zh-cn/library/a59bsyk4.aspx)|比较运算符可以进行重载如果进行重载，则必须成对进行重载。即如果重载 `==` ，也必须重载`!=` ， 反之亦然。对于 `<` 和 `>` 以及 `<=` 和 `>=` 也是同理。|
|[&&](https://msdn.microsoft.com/zh-cn/library/2a723cdk.aspx)[||](https://msdn.microsoft.com/zh-cn/library/6373h346.aspx)|逻辑运算符无法进行重载，但是它们使用 & 和 `|` 来计算。|
|[[]](https://msdn.microsoft.com/zh-cn/library/a3hd7ste.aspx)|索引运算符无法进行重载。但是可以定义索引器（参考[C#语法小知识（六）属性与索引器](http://blog.csdn.net/ecidevilin/article/details/52525080)）。|
|[(T)x](https://msdn.microsoft.com/zh-cn/library/0z4503sa.aspx)|强制转换运算符无法进行重载。但是可以使用[explicit](https://msdn.microsoft.com/zh-cn/library/xhbhezf4.aspx) 和 [implicit](https://msdn.microsoft.com/zh-cn/library/z5z9kes2.aspx)定义转换运算符。（参考[C#语法小知识（二十四）自定义类型转换](http://blog.csdn.net/ecidevilin/article/details/53414856)）|
|[+=](https://msdn.microsoft.com/zh-cn/library/sa7629ew.aspx)[-=](https://msdn.microsoft.com/zh-cn/library/2y9zhhx1.aspx)[*=](https://msdn.microsoft.com/zh-cn/library/s2bkaksf.aspx)[/=](https://msdn.microsoft.com/zh-cn/library/d31sybc9.aspx)[%=](https://msdn.microsoft.com/zh-cn/library/ydwa9zh0.aspx)[&=](https://msdn.microsoft.com/zh-cn/library/e669ax02.aspx)[|=](https://msdn.microsoft.com/zh-cn/library/h5f1zzaw.aspx)[^=](https://msdn.microsoft.com/zh-cn/library/0zbsw2z6.aspx)[<<=](https://msdn.microsoft.com/zh-cn/library/ayt2kcfb.aspx)[>>=](https://msdn.microsoft.com/zh-cn/library/23as4533.aspx)|赋值运算符无法进行重载。但是它们使用对应的非赋值运算符来计算（例如+=使用了+）。|
|[=](https://msdn.microsoft.com/zh-cn/library/sbkb459w.aspx)[.](https://msdn.microsoft.com/zh-cn/library/6zhxzbds.aspx)[?:](https://msdn.microsoft.com/zh-cn/library/ty67wk28.aspx)[??](https://msdn.microsoft.com/zh-cn/library/ms173224.aspx)[->](https://msdn.microsoft.com/zh-cn/library/s8bz4d5h.aspx)[=>](https://msdn.microsoft.com/zh-cn/library/bb311046.aspx)[f(x)](https://msdn.microsoft.com/zh-cn/library/0z4503sa.aspx)[as](https://msdn.microsoft.com/zh-cn/library/cscsdfbt.aspx)[is](https://msdn.microsoft.com/zh-cn/library/scekt9xw.aspx)[checked](https://msdn.microsoft.com/zh-cn/library/74b4xzyw.aspx)[unchecked](https://msdn.microsoft.com/zh-cn/library/a569z7k8.aspx)[default](https://msdn.microsoft.com/zh-cn/library/xwth0h0d.aspx)[delegate](https://msdn.microsoft.com/zh-cn/library/0yw3tz5k.aspx)[new](https://msdn.microsoft.com/zh-cn/library/51y09td4.aspx)[sizeof](https://msdn.microsoft.com/zh-cn/library/eahchzkf.aspx)[typeof](https://msdn.microsoft.com/zh-cn/library/58918ffs.aspx)|这些运算符无法进行重载。|









