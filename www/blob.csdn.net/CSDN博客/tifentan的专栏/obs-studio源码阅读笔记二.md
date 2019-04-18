# obs-studio源码阅读笔记二 - tifentan的专栏 - CSDN博客

2018年12月18日 14:26:44[露蛇](https://me.csdn.net/tifentan)阅读数：73


**跟着上篇提到的https://obsproject.com/docs/文档看一遍第一章OBS Studio Backend Design能对整个项目的总体框架有个印象了。**

对主要的入口函数run_program注释如下：

```
static const char *run_program_init = "run_program_init";
static int run_program(fstream &logFile, int argc, char *argv[])
{
	int ret = -1;
	
	//这个是一个队列，暂时不知道干嘛的，搞的复杂些为了自动释放
	auto profilerNameStore = CreateNameStore();
	
	//这是实现退出函数时执行全局profiler释放操作
	//这个profiler模块是一个记录执行过程信息的模块，例如记录各种操作耗时等
	//，最后释放时会打印这些信息
	std::unique_ptr<void, decltype(ProfilerFree)>
		prof_release(static_cast<void*>(&ProfilerFree),
				ProfilerFree);

	profiler_start();
	profile_register_root(run_program_init, 0);

	//像是一个局部的profiler,构造函数里面记录了开始时间，可能会跟全局的进行
	//交互？现看到是析构时将信息写入全局profiler
	ScopeProfiler prof{run_program_init};

	//qt的api，作用如函数名
	QCoreApplication::addLibraryPath(".");

	//派生自QApplication，是qt的UI启动类
	OBSApp program(argc, argv, profilerNameStore.get());
	try {
		bool created_log = false;

		//初始化一些跟全局配置，语言，用到的目录，主题等
		program.AppInit();
		delete_oldest_file(false, "obs-studio/profiler_data");

		OBSTranslator translator;
		program.installTranslator(&translator);

#ifdef _WIN32
		/* --------------------------------------- */
		/* check and warn if already running       */

		bool cancel_launch = false;
		bool already_running = false;
		RunOnceMutex rom = GetRunOnceMutex(already_running);

		if (!already_running) {
			goto run;
		}

		if (!multi) {
			QMessageBox::StandardButtons buttons(
					QMessageBox::Yes | QMessageBox::Cancel);
			QMessageBox mb(QMessageBox::Question,
					QTStr("AlreadyRunning.Title"),
					QTStr("AlreadyRunning.Text"), buttons,
					nullptr);
			mb.setButtonText(QMessageBox::Yes,
					QTStr("AlreadyRunning.LaunchAnyway"));
			mb.setButtonText(QMessageBox::Cancel, QTStr("Cancel"));
			mb.setDefaultButton(QMessageBox::Cancel);

			QMessageBox::StandardButton button;
			button = (QMessageBox::StandardButton)mb.exec();
			cancel_launch = button == QMessageBox::Cancel;
		}

		if (cancel_launch)
			return 0;

		if (!created_log) {
			create_log_file(logFile);
			created_log = true;
		}

		if (multi) {
			blog(LOG_INFO, "User enabled --multi flag and is now "
					"running multiple instances of OBS.");
		} else {
			blog(LOG_WARNING, "================================");
			blog(LOG_WARNING, "Warning: OBS is already running!");
			blog(LOG_WARNING, "================================");
			blog(LOG_WARNING, "User is now running multiple "
					"instances of OBS!");
		}

		/* --------------------------------------- */
run:
#endif
		//_asm   int   3
		if (!created_log) {
			create_log_file(logFile);
			created_log = true;
		}

		if (argc > 1) {
			stringstream stor;
			stor << argv[1];
			for (int i = 2; i < argc; ++i) {
				stor << " " << argv[i];
			}
			blog(LOG_INFO, "Command Line Arguments: %s", stor.str().c_str());
		}

		//界面初始化，加载模块并初始化
		if (!program.OBSInit())
			return 0;

		//将局部profile同步到全局profiler
		prof.Stop();

		//开启ui事件循环
		return program.exec();

	} catch (const char *error) {
		blog(LOG_ERROR, "%s", error);
		OBSErrorBox(nullptr, "%s", error);
	}

	return ret;
}
```

