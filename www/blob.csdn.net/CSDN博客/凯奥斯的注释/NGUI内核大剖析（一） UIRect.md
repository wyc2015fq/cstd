# NGUI内核大剖析（一）  UIRect - 凯奥斯的注释 - CSDN博客





2016年09月03日 14:19:54[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2587
所属专栏：[NGUI内核大剖析](https://blog.csdn.net/column/details/12924.html)









# **UIRect是UIPanel和UIWidget的基类，研究好前者，才能更方便的研究后两者，所以我将它做为本系列的第一篇。**

## **（注：本系列讨论的NGUI版本为3.10.0）**

首先要知道的一点是，UIRect是一个abstract class，也就是抽象类，它不能被实例化。它为UIPanel和UIWidget提供了一些公用的方法，这些方法通过AnchorPoint设置后两者的矩形区域。




在UIRect的最开始，我们看到有一个Inner Class叫做AnchorPoint。结构很简单，类函数的算法也都很简单，具体的用途我们暂时不讲，后面会提到。唯一要注意的是，在set方法里，我们看到：




`this.absolute = Mathf.FloorToInt(absolute + 0.5f);`
这其实就等同于




`this.absolute = Mathf.RoundToInt(absolute);`
接着往下看


```
/// <summary>
	/// Left side anchor.
	/// </summary>

	public AnchorPoint leftAnchor = new AnchorPoint();

	/// <summary>
	/// Right side anchor.
	/// </summary>

	public AnchorPoint rightAnchor = new AnchorPoint(1f);

	/// <summary>
	/// Bottom side anchor.
	/// </summary>

	public AnchorPoint bottomAnchor = new AnchorPoint();

	/// <summary>
	/// Top side anchor.
	/// </summary>

	public AnchorPoint topAnchor = new AnchorPoint(1f);
```





定义了四个AnchorPoint类型的变量，分别对应左右下上四个位置的Anchor Point，也就是锚点。具体是什么，我们就要结合在Unity Editor里面的应用来看了。

新建一个widget，把Anchors里的Type改成Unified或者Advanced，我们就可以自行调节Anchor Point了。Unified和Advanced区别在于，Unified统一指定一个Transform为target，而Advanced可以为左右下上每一个Anchor Point指定一个Transform为target。我们这里以Unified为例：![](https://img-blog.csdn.net/20160903150410627)

如图所示，以UI Root为Target，所谓Target其实就是参考系的载体，真正的参考系是图中的Target‘s Center也就是UI Root的中心位置。可以这么理解，UIWidget的左边在UI Root（水平）中心点的x坐标-50的位置，右边在UI Root（水平）中心点的x坐标+50的位置，Bottom和Top与之类似。

通过调节Left,Right,Bottom,Top后面的值，我们可以看到UIWidget的Position和Size在变化，而Scene里面所示的UIWidget的区域也随之增大减小。

至于参考系下拉菜单里的其他选项，其实都是取Target的不同位置（Set To Current Position是一个快捷方法，把参考系拉倒和UIWidget当前位置重叠的UIRoot的位置上），以达到UI可以根据屏幕大小自适应变化的目的。当然，前面说“位置”，其实并不准确，准确的来讲其实是位置除以尺寸，我们姑且称之为参考系比例。

关于屏幕自适应，有机会的话，我们会在后续的文章中讲到。

回到代码中，看SetAnchor及其几个重载的方法，我们拿参数列表最长的为例


```
/// <summary>
	/// Anchor this rectangle to the specified transform.
	/// </summary>

	public void SetAnchor (GameObject go,
		float left, int leftOffset,
		float bottom, int bottomOffset,
		float right, int rightOffset,
		float top, int topOffset)
	{
		Transform t = (go != null) ? go.transform : null;

		leftAnchor.target = t;
		rightAnchor.target = t;
		topAnchor.target = t;
		bottomAnchor.target = t;

		leftAnchor.relative = left;
		rightAnchor.relative = right;
		bottomAnchor.relative = bottom;
		topAnchor.relative = top;

		leftAnchor.absolute = leftOffset;
		rightAnchor.absolute = rightOffset;
		bottomAnchor.absolute = bottomOffset;
		topAnchor.absolute = topOffset;

		ResetAnchors();
		UpdateAnchors();
	}
```





target对应的就是Editor中的target，relative对应的是参考系比例（center为0.5f,left和bottom为0,right和top为1），absolute对应的就是相对坐标。

设置完参数之后，就重置并更新Anchors。


我们回头看AnchorPoint这个Inner Class里的几个方法。

Set不必多说就是设置参考系比例和相对坐标。

SetToNearest实在UIRectEditor里用到，给出相对于三个参考系比例（默认0,0.5f,1）的相对坐标，取最小值所对应的参考系比例。

SetHorizontal根据某个transform计算出新的横向偏移量（参考系比例不变）。SetVertical同理。

GetSide获取相对于某个transform的坐标。一般会调用UIRect的GetSides，后面我们会讲到。





回到之前的编辑器截图




![](https://img-blog.csdn.net/20160903150410627)





我们还差了一个Execute的参数没有讲，这个对应了下面这段代码


```
public enum AnchorUpdate
	{
		OnEnable,
		OnUpdate,
		OnStart,
	}

	/// <summary>
	/// Whether anchors will be recalculated on every update.
	/// </summary>

	public AnchorUpdate updateAnchors = AnchorUpdate.OnUpdate;
```







AnchorUpdate是个枚举类，定义了Anchor的三种更新方式，虽然是三种，但其实更新的代码都在Update中。通过布尔值mUpdateAnchors或者updateAnchors == AnchorUpdate.OnUpdate来判断是否需要更新。

OnStart是只更新一次，因为mUpdateAnchors默认值是true，OnEnable是在GameObject.setActive(true)或者这个组件enabled=true的时候调用OnEnable，令mUpdateAnchors = true 而OnUpdate是每个逻辑帧都会更新。





接下来是ResetAnchor


```
/// <summary>
	/// Ensure that all rect references are set correctly on the anchors.
	/// </summary>

	public void ResetAnchors ()
	{
		mAnchorsCached = true;

		leftAnchor.rect		= (leftAnchor.target)	? leftAnchor.target.GetComponent<UIRect>()	 : null;
		bottomAnchor.rect	= (bottomAnchor.target) ? bottomAnchor.target.GetComponent<UIRect>() : null;
		rightAnchor.rect	= (rightAnchor.target)	? rightAnchor.target.GetComponent<UIRect>()	 : null;
		topAnchor.rect		= (topAnchor.target)	? topAnchor.target.GetComponent<UIRect>()	 : null;

		mCam = NGUITools.FindCameraForLayer(cachedGameObject.layer);

		FindCameraFor(leftAnchor);
		FindCameraFor(bottomAnchor);
		FindCameraFor(rightAnchor);
		FindCameraFor(topAnchor);

		mUpdateAnchors = true;
	}
```







ResetAnchors为每个Anchor找到target上的UIRect，并且找到本GameObject对应的Camera，最后为每个Anchor找到target对应的Camera（targetCamera）。rect我们会在UpdateAnchors里讲到。mCam在cameraRayDistance和GetSides里用到（UIPanel也用到了，后面讲到UIPanel的时候可能会讲到它），代码不看，只看注释：




`/// Helper function that returns the distance to the camera's directional vector hitting the panel's plane.`
（cameraRayDistance）简单来讲就是camera和panel之间的距离。


```
/// Get the sides of the rectangle relative to the specified transform.
/// The order is left, top, right, bottom.
```






（GetSides）简单来讲就是获得矩形四条边相对于某个transfrom的坐标（x或y）







targetCamera是为了GetLocalPos这个方法准备的，我们也只看注释：

`/// Helper function that gets the specified anchor's position relative to the chosen transform.`
获取某个anchor相对于某个transform的位置




UpdateAnchors这个方法里其实没什么，真正执行的是UpdateAnchorsInternal这个方法


```
/// <summary>
	/// Update anchors.
	/// </summary>

	protected void UpdateAnchorsInternal (int frame)
	{
		mUpdateFrame = frame;
		mUpdateAnchors = false;

		bool anchored = false;

		if (leftAnchor.target)
		{
			anchored = true;
			if (leftAnchor.rect != null && leftAnchor.rect.mUpdateFrame != frame)
				leftAnchor.rect.Update();
		}

		if (bottomAnchor.target)
		{
			anchored = true;
			if (bottomAnchor.rect != null && bottomAnchor.rect.mUpdateFrame != frame)
				bottomAnchor.rect.Update();
		}

		if (rightAnchor.target)
		{
			anchored = true;
			if (rightAnchor.rect != null && rightAnchor.rect.mUpdateFrame != frame)
				rightAnchor.rect.Update();
		}

		if (topAnchor.target)
		{
			anchored = true;
			if (topAnchor.rect != null && topAnchor.rect.mUpdateFrame != frame)
				topAnchor.rect.Update();
		}

		// Update the dimensions using anchors
		if (anchored) OnAnchor();
	}
```







记录mUpdateFrame是为了防止重复计算，在setAnchor或OnEnabled之后，同一帧的Update就不会再调用UpdateAnchorsInternal了。

然后调用每个anchor的rect（在ResetAnchors中取到的）的Update方法。

最后调用OnAnchor方法，OnAnchor是个abstract方法，具体实现在UIPanel和UIWidget（还有UIWidget的衍生类UILabel）里面，根据参考系和相对位置，计算位置和尺寸。






UIRect大概就讲到这里了，总结一下：UIRect是为UI组件描述其位置和尺寸信息，通过AnchorPoint来实现，根据目标参考系和相对位置来计算矩形区域的每一个边的位置。

~~(⊙o⊙)…我好像不是很善于总结...~~




最后可以加一个tips：

在OnEnable的时候会调用OnInit，UIRect的OnInit到没有什么可说的。

UIPanel里的OnInit在一堆初始化之中，有一句mRebuild = true，这会导致这个UIPanel里的所有UIDrawCall重建（具体我们会在UIPanel里讲到）。

UIWidget的OnInit有一句RemoveFromPanel()，并会调用OnUpdate（通过Update()），又会重新创建（寻找）panel（会导致这个Widget所属的UIDrawCall会被重建，甚至所有UIDrawCall重建），并会调用Invalidate更新Visibility和FinalAlpha，并且这个方法是递归的，意思就是所有的子Widget都会更新一遍。

所以GameObject.setActive还是慎重使用。








