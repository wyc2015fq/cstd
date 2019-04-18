# obs-studio源码阅读笔记三 - tifentan的专栏 - CSDN博客

2018年12月19日 11:43:29[露蛇](https://me.csdn.net/tifentan)阅读数：148


今天研究一下obs-studio的ui构建。

上次看到OBSApp::OBSInit()是真正建立ui的入口。

```
bool OBSApp::OBSInit()
{
	ProfileScope("OBSApp::OBSInit");

	setAttribute(Qt::AA_UseHighDpiPixmaps);

	//主要是初始化libobs模块，里面注册了scene跟group两个模块
	if (!StartupOBS(locale.c_str(), GetProfilerNameStore()))
		return false;

#ifdef _WIN32
	bool browserHWAccel = config_get_bool(globalConfig, "General",
			"BrowserHWAccel");

	obs_data_t *settings = obs_data_create();
	obs_data_set_bool(settings, "BrowserHWAccel", browserHWAccel);
	obs_apply_private_data(settings);
	obs_data_release(settings);

	blog(LOG_INFO, "Browser Hardware Acceleration: %s",
			browserHWAccel ? "true" : "false");
#endif

	blog(LOG_INFO, "Portable mode: %s",
			portable_mode ? "true" : "false");

	setQuitOnLastWindowClosed(false);

	//主窗口
	mainWindow = new OBSBasic();

	mainWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	connect(mainWindow, SIGNAL(destroyed()), this, SLOT(quit()));

	mainWindow->OBSInit();

	connect(this, &QGuiApplication::applicationStateChanged,
			[this](Qt::ApplicationState state)
			{
				ResetHotkeyState(
					state != Qt::ApplicationActive);
			});
	ResetHotkeyState(applicationState() != Qt::ApplicationActive);
	return true;
}
```

可见OBSBasic就是我要找的主窗口了。

qt不是很熟悉的我，疑惑没用vs的qt插件的obs怎么构建的qt界面。同时可看到obs里面也生成了像ui_OBSBasic.h这种qt生成的中间文件。

研究了一下发现用到了vs里面的自定义生成工具。

看一下OBSBasic.ui的属性（vcproj文件也能看到）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219113628808.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RpZmVudGFu,size_16,color_FFFFFF,t_70)

从脚本里面可看到用了uic这个qt工具生成的。

总的来说，ui的生成跟qt插件一样，先生成ui对应的头文件，再引用这个头文件生成项目。

qt插件估计也是这么干的，以后或许可以考虑不安装qt插件了（手动狗头）！

