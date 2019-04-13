
# hook mono实现Assembly.Load从指定路径读取文件 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[hook mono实现Assembly.Load从指定路径读取文件](https://www.cnblogs.com/eniac1946/p/7485173.html)
|Posted on|2017-09-06 15:49|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7485173)|[收藏](#)
|mono-unity github:|[https://github.com/Unity-Technologies/mono/blob/unity-staging/mono/metadata/assembly.c](https://github.com/Unity-Technologies/mono/blob/unity-staging/mono/metadata/assembly.c)
|此方案需要assemblyname已在GAC中，若不在可以用反射方式读取
|mini/main|.c:|main|()|mono_main_with_options|()|mono_main|()    --mini/driver|.c|mini_init|()    --mini/mini|.c|mono_assembly_open|()    --metadata/assembly|.c
            --|mono_assembly_load_from_full|() --
            ----对文件名进行判断 是否已 file:|//开头
            ----mono_assembly_is_in_gac --判断是否在gac当中|----|mono_assembly_open_from_bundle|()|------|mono_image_open_from_data_with_name|()  --image|.c
            --------|do_mono_image_load|()|main_thread_handler|()|// assembly（也就是bytecode）的编译执行|mini_cleanup|()|mono_image_open_from_data_with_name方法中需要用到file的数据指针，和size，
|所以需要在其上一层Hook，从指定的路径读取file到内存获得指针和size。
|Hook的方法：mono_assembly_open_from_bundle(...)
|原型：（源文件：/mono/metadata/assembly.c）
|MonoImage *
mono_assembly_open_from_bundle (const char *filename, MonoImageOpenStatus *status, gboolean refonly)
{}
|完整代码
|/** 
 * mono_assembly_open_from_bundle:
 * @filename: Filename requested
 * @status: return value
 *
 * This routine tries to open the assembly specified by `filename' from the
 * defined bundles, if found, returns the MonoImage for it, if not found
 * returns NULL
 */
MonoImage *
mono_assembly_open_from_bundle (const char *filename, MonoImageOpenStatus *status, gboolean refonly)
{
	int i;
	char *name;
	MonoImage *image = NULL;
	/*
	 * we do a very simple search for bundled assemblies: it's not a general 
	 * purpose assembly loading mechanism.
	 */
	if (!bundles)
		return NULL;
	name = g_path_get_basename (filename);
	mono_assemblies_lock ();
	for (i = 0; !image && bundles [i]; ++i) {
		if (strcmp (bundles [i]->name, name) == 0) {
			image = mono_image_open_from_data_with_name ((char*)bundles [i]->data, bundles [i]->size, FALSE, status, refonly, name);
			break;
		}
	}
	mono_assemblies_unlock ();
	g_free (name);
	if (image) {
		mono_image_addref (image);
		return image;
	}
	return NULL;
}







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
