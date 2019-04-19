# 强大的PropertyGrid - 逍遥剑客 - CSDN博客
2009年07月04日 22:05:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：30082
 PropertyGrid, 做工具一定要用这东西.....
把要编辑的对象看成类的话, 所有要编辑的属性就是成员
嗯嗯, 最近看了几眼[Ogitor](http://www.ogre3d.org/wiki/index.php/Ogitor), 它对于PropertyGrid的使用就很不错
所有要编辑的对象(灯光, 模型, 粒子等等)都有一个共同的基类, 每当选中一个可编辑对象时, 右边的属性框里就显示出当前对象的属性...(公司那个编辑器要多土就有多土-_-)
尽管Ribbon界面看起来很酷, 我还是对MFC提不起兴趣来...
.net里的PropertyGrid更方便, 一点一点来:
属性自动绑定:
ref class Human
{
public:
	Human()
	{
		this->Name = "(None)";
		this->Age = 0;
		this->IsMale = false;
	}
	property String^ Name;
	property int Age;
	property bool IsMale;
};
只需要一句
			this->propertyGrid1->SelectedObject = gcnew Human();
它就能自动识别出Human类中的property, 并且自动关联到PropertyGrid中:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090704/PropertyGird_AutoBind.JPG)
对属性进行分类并加注释:
ref class Human
{
public:
	Human()
	{
		this->Name = "(None)";
		this->Age = 0;
		this->IsMale = false;
		this->SkinColor = Color::Yellow;
	}
	[CategoryAttribute("常规"), DescriptionAttribute("名字")]
	property String^ Name;
	[CategoryAttribute("常规"), DescriptionAttribute("年龄")]
	property int Age;
	[CategoryAttribute("外观"), DescriptionAttribute("性别")]
	property bool IsMale;
	[CategoryAttribute("外观"), DescriptionAttribute("肤色")]
	property Color SkinColor;
};
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090704/PropertyGird_Category.JPG)
太爽啦~颜色自己就能识别........
弄个Image类型居然还能自己选择文件...NB啊
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090705/PropertyGird_Image.JPG)
除了基本类型之外, Font, Size, Color等复杂类型也可以支持, 那么自定义类型呢?
如果只是像上面那样放上的话, 只会得到个灰色不可编辑的东西~
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090704/PropertyGird_Custom.JPG)
要想让PropertyGrid能够展开Vector3属性, 指定一下TypeConverter就可以了:
[TypeConverterAttribute(ExpandableObjectConverter::typeid)]
ref struct Vector3
{
	property float X;
	property float Y;
	property float Z;
	virtual String^ ToString() override
	{
		return String::Format("({0}, {1}, {2})", this->X, this->Y, this->Z);
	}
};
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090705/PropertyGird_Expand.JPG)
对于枚举类型, PropertyGrid会自动显示成下拉框. 把性别改成枚举看看:
	enum struct SexType
	{
		Male,
		Female
	};
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090705/PropertyGird_Enum.JPG)
另外, 还可以弹出自定义的编辑界面, 比如随时间变化的曲线啦(经常用来做效果...)
这个, 暂时没需求, 不实现了, 有兴趣的参考:[Getting the Most Out of the .NET Framework PropertyGrid Control](http://msdn.microsoft.com/en-us/library/aa302326.aspx)
