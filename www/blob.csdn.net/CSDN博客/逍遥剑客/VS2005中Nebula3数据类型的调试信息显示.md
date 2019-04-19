# VS2005中Nebula3数据类型的调试信息显示 - 逍遥剑客 - CSDN博客
2009年08月04日 23:15:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2069
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
用过VS2003的应该知道, STL的容器调试起来非常麻烦, 因为调试时显示的信息非常不直观
VS2005引入一个autoexp.dat, 可以定义数据在调试时显示的格式
详细介绍可以参考:[Writing custom visualizers for Visual Studio 2005](http://www.virtualdub.org/blog/pivot/entry.php?id=120)
这里暂时给出一些Nebula3相关的格式定义:
这些放到[AutoExpand]字段下面:
;------------------------------------------------------------------------------
;  Nebula Device 3 Types
;------------------------------------------------------------------------------
Math::float4=<X,g>, <Y,g>, <Z,g>, <W,g>
Math::quaternion=<X,g>, <Y,g>, <Z,g>, <W,g>
这些放到[Visualizer]字段下面:
;------------------------------------------------------------------------------
;  Nebula Device 3 Types
;------------------------------------------------------------------------------
Core::Rtti{
	preview( $e.name )
}
Util::Array<*>|Util::FixedArray<*>{
	children
	(
		#array
		(
			expr : $c.elements[$i],  
			size : $c.size
		)
	)
			
	preview
	( 
		#( 
			"[", 
			$c.size ,
			"](", 
		
			#array
			(
				expr : $c.elements[$i],  
				size : $c.size
			), 
			")"
		)
	)
}
Util::KeyValuePair<*>{
	preview
	(
		#( 
			"(", $e.keyData, ",", $e.valueData, ")"
		)
	)
}
Util::String{
	preview
	(
		#if($e.heapBuffer != 0)
		(
			[$e.heapBuffer,s]
		)
		#else
		(
			[$e.localBuffer,s]
		)
	)
}
Util::Proxy<*>{
	preview( $e.ptr.ptr )
}
Util::Atom<*>{
	preview( $e.proxy )
}
Array相关的那个还不能用, 我也不清楚是为什么
#array的size: 后写什么都不管用, 直接写个数字却能起效果. 不知道哪位仁兄指点一下? 感激不尽
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090804/ArrayString.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090804/AtomString.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090804/matrix44Array.JPG)
=================================================================
08/05/2009 : 修正Array/FixedArray调试信息无法显示的问题
原因让人很崩溃: Array::size的"size"成员是autoexp的一个关键字, 所以产生了解析错误
把Array::size和FixedArray::size改名叫arraySize解决问题. 脚本更正如下:
Util::Array<*>|Util::FixedArray<*>{
	children
	(
		#array
		(
			expr : $c.elements[$i],  
			size : $c.arraySize
		)
	)
			
	preview
	( 
		#( 
			"[", 
			$c.arraySize ,
			"](", 
		
			#array
			(
				expr : $c.elements[$i],  
				size : $c.arraySize
			), 
			")"
		)
	)
}
现在的效果是不是跟std::vector一样了呢?
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090805/ArrayString2.JPG)
