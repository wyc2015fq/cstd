
# mono_image_open_from_data_with_name原型 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[mono_image_open_from_data_with_name原型](https://www.cnblogs.com/eniac1946/p/7477323.html)
|Posted on|2017-09-05 10:27|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7477323)|[收藏](#)
|mono4.5
|[https://github.com/Unity-Technologies/mono](https://github.com/Unity-Technologies/mono)
|查看mono源码：
|//PATH: /mono/metadata/image.c
MonoImage *
mono_image_open_from_data_with_name (char *data, guint32 data_len, gboolean need_copy, MonoImageOpenStatus *status, gboolean refonly, const char *name)
{}
|cpp形式可以如下
|void*
mono_image_open_from_data_with_name (char *data, unsigned int data_len, int need_copy, int *status, int refonly, const char *name)
{}
|其中MonoImage定义
|//Path: /mono/metadata/metadata-internals.h
struct _MonoImage {
	/*
	 * The number of assemblies which reference this MonoImage though their 'image'
	 * field plus the number of images which reference this MonoImage through their 
	 * 'modules' field, plus the number of threads holding temporary references to
	 * this image between calls of mono_image_open () and mono_image_close ().
	 */
	int   ref_count;
	void *raw_data_handle;
	char *raw_data;
	guint32 raw_data_len;
	guint8 raw_buffer_used    : 1;
	guint8 raw_data_allocated : 1;
\#ifdef USE_COREE
	/* Module was loaded using LoadLibrary. */
	guint8 is_module_handle : 1;
	/* Module entry point is _CorDllMain. */
	guint8 has_entry_point : 1;
\#endif
	/* Whenever this is a dynamically emitted module */
	guint8 dynamic : 1;
	/* Whenever this is a reflection only image */
	guint8 ref_only : 1;
       /*
       .....................
      */
}
|数据类型说明：
|gunichar|is defined as|typedef guint32 gunichar
|guint32|is defined as|typedef unsigned int guint32
|gboolean|is defined as|typedef gint gboolean
|gint|is defined as|typedef int gint
|gxxx是标准glib类型（C\#使用）
|定义可以查询：|[http://web.mit.edu/barnowl/share/gtk-doc/html/glib/glib-Basic-Types.html　](http://web.mit.edu/barnowl/share/gtk-doc/html/glib/glib-Basic-Types.html)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
