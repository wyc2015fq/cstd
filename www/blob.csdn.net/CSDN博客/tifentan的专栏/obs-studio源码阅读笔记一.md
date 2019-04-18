# obs-studio源码阅读笔记一 - tifentan的专栏 - CSDN博客

2018年12月14日 11:35:16[露蛇](https://me.csdn.net/tifentan)阅读数：127


# 安装源码

根据wiki来
[https://github.com/obsproject/obs-studio/wiki/install-instructions#windows-build-directions](https://github.com/obsproject/obs-studio/wiki/install-instructions#windows-build-directions)

# 安装browser插件

根据这个项目来
[https://github.com/obsproject/obs-browser](https://github.com/obsproject/obs-browser)

# 源码文档

[https://obsproject.com/docs/](https://obsproject.com/docs/)

# 基本架构
- 跨平台，qt界面库，使用cmake构建工程
- windows下分模块所有模块都生成dll，运行程序是obs项目
- main函数入口在于obs-app.cpp

注释如下：

```
int main(int argc, char *argv[])
{
#ifndef _WIN32
	signal(SIGPIPE, SIG_IGN);
#endif

#ifdef _WIN32
	obs_init_win32_crash_handler(); //初始化自定义崩溃处理代码，调试过将一些栈和内存打印出来，暂时不知道有没进一步使用的可能
	SetErrorMode(SEM_FAILCRITICALERRORS); //设置一些错误处理模式
	load_debug_privilege(); //将程序设置为执行权限
	base_set_crash_handler(main_crash_handler, nullptr); //将自己的崩溃处理函数注册到处理代码，主要是往crashs文件夹里面写log文件
#endif

	base_get_log_handler(&def_log_handler, nullptr);

#if defined(USE_XDG) && defined(IS_UNIX)
	move_to_xdg();
#endif
	
	/*int*p = 0;
	int i = *p;
	int b = i;
	printf("%d",b);*/

	
	obs_set_cmdline_args(argc, argv);

	for (int i = 1; i < argc; i++) {
		if (arg_is(argv[i], "--portable", "-p")) {
			portable_mode = true;

		} else if (arg_is(argv[i], "--multi", "-m")) {
			multi = true;

		} else if (arg_is(argv[i], "--verbose", nullptr)) {
			log_verbose = true;

		} else if (arg_is(argv[i], "--always-on-top", nullptr)) {
			opt_always_on_top = true;

		} else if (arg_is(argv[i], "--unfiltered_log", nullptr)) {
			unfiltered_log = true;

		} else if (arg_is(argv[i], "--startstreaming", nullptr)) {
			opt_start_streaming = true;

		} else if (arg_is(argv[i], "--startrecording", nullptr)) {
			opt_start_recording = true;

		} else if (arg_is(argv[i], "--startreplaybuffer", nullptr)) {
			opt_start_replaybuffer = true;

		} else if (arg_is(argv[i], "--collection", nullptr)) {
			if (++i < argc) opt_starting_collection = argv[i];

		} else if (arg_is(argv[i], "--profile", nullptr)) {
			if (++i < argc) opt_starting_profile = argv[i];

		} else if (arg_is(argv[i], "--scene", nullptr)) {
			if (++i < argc) opt_starting_scene = argv[i];

		} else if (arg_is(argv[i], "--minimize-to-tray", nullptr)) {
			opt_minimize_tray = true;

		} else if (arg_is(argv[i], "--studio-mode", nullptr)) {
			opt_studio_mode = true;

		} else if (arg_is(argv[i], "--allow-opengl", nullptr)) {
			opt_allow_opengl = true;

		} else if (arg_is(argv[i], "--help", "-h")) {
			std::cout <<
			"--help, -h: Get list of available commands.\n\n" << 
			"--startstreaming: Automatically start streaming.\n" <<
			"--startrecording: Automatically start recording.\n" <<
			"--startreplaybuffer: Start replay buffer.\n\n" <<
			"--collection <string>: Use specific scene collection."
				<< "\n" <<
			"--profile <string>: Use specific profile.\n" <<
			"--scene <string>: Start with specific scene.\n\n" <<
			"--studio-mode: Enable studio mode.\n" <<
			"--minimize-to-tray: Minimize to system tray.\n" <<
			"--portable, -p: Use portable mode.\n" <<
			"--multi, -m: Don't warn when launching multiple instances.\n\n" <<
			"--verbose: Make log more verbose.\n" <<
			"--always-on-top: Start in 'always on top' mode.\n\n" <<
			"--unfiltered_log: Make log unfiltered.\n\n" <<
			"--allow-opengl: Allow OpenGL on Windows.\n\n" <<
			"--version, -V: Get current version.\n";

			exit(0);

		} else if (arg_is(argv[i], "--version", "-V")) {
			std::cout << "OBS Studio - " << 
				App()->GetVersionString() << "\n";
			exit(0);
		}
	}
	//portable_mode = true;
	//multi = true;
	//opt_start_replaybuffer = true;
#if !OBS_UNIX_STRUCTURE
	if (!portable_mode) {
		portable_mode =
			os_file_exists(BASE_PATH "/portable_mode") ||
			os_file_exists(BASE_PATH "/obs_portable_mode") ||
			os_file_exists(BASE_PATH "/portable_mode.txt") ||
			os_file_exists(BASE_PATH "/obs_portable_mode.txt");
	}
#endif

	upgrade_settings();//初始化一些设置，主要是根据一些主设置去安排好一下相关的设置项，另外写入后面要用的json文件中

	fstream logFile;

	curl_global_init(CURL_GLOBAL_ALL);
	int ret = run_program(logFile, argc, argv); //主要的处理流程，下节分析。

	blog(LOG_INFO, "Number of memory leaks: %ld", bnum_allocs());
	base_set_log_handler(nullptr, nullptr);
	return ret;
}
```

