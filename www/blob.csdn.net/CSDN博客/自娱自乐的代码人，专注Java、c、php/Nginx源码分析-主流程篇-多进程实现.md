# Nginx源码分析 - 主流程篇 - 多进程实现 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年08月21日 09:15:33[initphp](https://me.csdn.net/initphp)阅读数：3352
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









默认情况下，Nginx都是多进程的运行模式。Nginx和Memcached不一样，是多进程的模式。采用多进程模式最大的好处：

1. 每个进程的资源独立

2. 不需要添加各种繁琐的锁了




### Nginx多进程实现的流程图

![](https://img-blog.csdn.net/20160821091508128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




### Nginx多进程具体实现

#### 1. ngx_master_process_cycle 进入多进程模式

ngx_master_process_cycle方法主要做了两个工作：

1. 主进程进行信号的监听和处理

2. 开启子进程



```cpp
/**
 * Nginx的多进程运行模式
 */
void ngx_master_process_cycle(ngx_cycle_t *cycle) {
	char *title;
	u_char *p;
	size_t size;
	ngx_int_t i;
	ngx_uint_t n, sigio;
	sigset_t set;
	struct itimerval itv;
	ngx_uint_t live;
	ngx_msec_t delay;
	ngx_listening_t *ls;
	ngx_core_conf_t *ccf;

	/* 设置能接收到的信号 */
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigaddset(&set, SIGALRM);
	sigaddset(&set, SIGIO);
	sigaddset(&set, SIGINT);
	sigaddset(&set, ngx_signal_value(NGX_RECONFIGURE_SIGNAL));
	sigaddset(&set, ngx_signal_value(NGX_REOPEN_SIGNAL));
	sigaddset(&set, ngx_signal_value(NGX_NOACCEPT_SIGNAL));
	sigaddset(&set, ngx_signal_value(NGX_TERMINATE_SIGNAL));
	sigaddset(&set, ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
	sigaddset(&set, ngx_signal_value(NGX_CHANGEBIN_SIGNAL));

	if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
		ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
				"sigprocmask() failed");
	}

	sigemptyset(&set);

	size = sizeof(master_process);

	for (i = 0; i < ngx_argc; i++) {
		size += ngx_strlen(ngx_argv[i]) + 1;
	}

	/* 保存进程标题 */
	title = ngx_pnalloc(cycle->pool, size);
	if (title == NULL) {
		/* fatal */
		exit(2);
	}

	p = ngx_cpymem(title, master_process, sizeof(master_process) - 1);
	for (i = 0; i < ngx_argc; i++) {
		*p++ = ' ';
		p = ngx_cpystrn(p, (u_char *) ngx_argv[i], size);
	}

	ngx_setproctitle(title);

	/* 获取核心配置 ngx_core_conf_t */
	ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

	/* 启动工作进程 - 多进程启动的核心函数 */
	ngx_start_worker_processes(cycle, ccf->worker_processes,
			NGX_PROCESS_RESPAWN);
	ngx_start_cache_manager_processes(cycle, 0);

	ngx_new_binary = 0;
	delay = 0;
	sigio = 0;
	live = 1;

	/* 主线程循环 */
	for (;;) {

		/* delay用来设置等待worker推出的时间，master接受了退出信号后，
		 * 首先发送退出信号给worker，而worker退出需要一些时间*/
		if (delay) {
			if (ngx_sigalrm) {
				sigio = 0;
				delay *= 2;
				ngx_sigalrm = 0;
			}

			ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
					"termination cycle: %M", delay);

			itv.it_interval.tv_sec = 0;
			itv.it_interval.tv_usec = 0;
			itv.it_value.tv_sec = delay / 1000;
			itv.it_value.tv_usec = (delay % 1000) * 1000;

			if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
				ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
						"setitimer() failed");
			}
		}

		ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "sigsuspend");

		/* 等待信号的到来，阻塞函数 */
		sigsuspend(&set);

		ngx_time_update();

		ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
				"wake up, sigio %i", sigio);

		/* 收到了SIGCHLD信号，有worker退出(ngx_reap == 1) */
		if (ngx_reap) {
			ngx_reap = 0;
			ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "reap children");

			live = ngx_reap_children(cycle);
		}

		if (!live && (ngx_terminate || ngx_quit)) {
			ngx_master_process_exit(cycle);
		}

		/* 中止进程  */
		if (ngx_terminate) {
			if (delay == 0) {
				delay = 50;
			}

			if (sigio) {
				sigio--;
				continue;
			}

			sigio = ccf->worker_processes + 2 /* cache processes */;

			if (delay > 1000) {
				ngx_signal_worker_processes(cycle, SIGKILL);
			} else {
				ngx_signal_worker_processes(cycle,
						ngx_signal_value(NGX_TERMINATE_SIGNAL));
			}

			continue;
		}

		/* 退出进程 */
		if (ngx_quit) {
			ngx_signal_worker_processes(cycle,
					ngx_signal_value(NGX_SHUTDOWN_SIGNAL));

			ls = cycle->listening.elts;
			for (n = 0; n < cycle->listening.nelts; n++) {
				if (ngx_close_socket(ls[n].fd) == -1) {
					ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
							ngx_close_socket_n " %V failed", &ls[n].addr_text);
				}
			}
			cycle->listening.nelts = 0;

			continue;
		}

		/* 收到SIGHUP信号 重新初始化配置 */
		if (ngx_reconfigure) {
			ngx_reconfigure = 0;

			if (ngx_new_binary) {
				ngx_start_worker_processes(cycle, ccf->worker_processes,
						NGX_PROCESS_RESPAWN);
				ngx_start_cache_manager_processes(cycle, 0);
				ngx_noaccepting = 0;

				continue;
			}

			ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reconfiguring");

			cycle = ngx_init_cycle(cycle);
			if (cycle == NULL) {
				cycle = (ngx_cycle_t *) ngx_cycle;
				continue;
			}

			ngx_cycle = cycle;
			ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx,
					ngx_core_module);
			ngx_start_worker_processes(cycle, ccf->worker_processes,
					NGX_PROCESS_JUST_RESPAWN);
			ngx_start_cache_manager_processes(cycle, 1);

			/* allow new processes to start */
			ngx_msleep(100);

			live = 1;
			ngx_signal_worker_processes(cycle,
					ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
		}

		/* 当ngx_noaccepting==1时，会把ngx_restart设为1，重启worker  */
		if (ngx_restart) {
			ngx_restart = 0;
			ngx_start_worker_processes(cycle, ccf->worker_processes,
					NGX_PROCESS_RESPAWN);
			ngx_start_cache_manager_processes(cycle, 0);
			live = 1;
		}

		/* 收到SIGUSR1信号，重新打开log文件 */
		if (ngx_reopen) {
			ngx_reopen = 0;
			ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reopening logs");
			ngx_reopen_files(cycle, ccf->user);
			ngx_signal_worker_processes(cycle,
					ngx_signal_value(NGX_REOPEN_SIGNAL));
		}

		/* SIGUSER2，热代码替换 */
		if (ngx_change_binary) {
			ngx_change_binary = 0;
			ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "changing binary");
			ngx_new_binary = ngx_exec_new_binary(cycle, ngx_argv);
		}

		/* 收到SIGWINCH信号不在接受请求，worker退出，master不退出 */
		if (ngx_noaccept) {
			ngx_noaccept = 0;
			ngx_noaccepting = 1;
			ngx_signal_worker_processes(cycle,
					ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
		}
	}
}
```

#### 2. ngx_start_worker_processes 创建工作进程

通过循环创建N个子进程。每个子进程都有独立的内存空间。

子进程的个数由Nginx的配置：ccf->worker_processes决定



```cpp
/**
 * 创建工作进程
 */
static void ngx_start_worker_processes(ngx_cycle_t *cycle, ngx_int_t n,
		ngx_int_t type) {
	ngx_int_t i;
	ngx_channel_t ch;

	ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "start worker processes");

	ngx_memzero(&ch, sizeof(ngx_channel_t));

	ch.command = NGX_CMD_OPEN_CHANNEL;

	/* 循环创建工作进程  默认ccf->worker_processes=8个进程，根据CPU个数决定   */
	for (i = 0; i < n; i++) {

		/* 打开工作进程  （ngx_worker_process_cycle 回调函数，主要用于处理每个工作线程）*/
		ngx_spawn_process(cycle, ngx_worker_process_cycle,
				(void *) (intptr_t) i, "worker process", type);

		ch.pid = ngx_processes[ngx_process_slot].pid;
		ch.slot = ngx_process_slot;
		ch.fd = ngx_processes[ngx_process_slot].channel[0];

		ngx_pass_open_channel(cycle, &ch);
	}
}
```



#### 3. ngx_spawn_process fork工作进程

ngx_spawn_process方法主要用于**fork**出各个工作进程。具体主要看fork过程的代码。



```cpp
/* fork 一个子进程 */
    pid = fork();

    switch (pid) {

    case -1:
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      "fork() failed while spawning \"%s\"", name);
        ngx_close_channel(ngx_processes[s].channel, cycle->log);
        return NGX_INVALID_PID;

    case 0:
    	/* 如果pid fork成功，则调用 ngx_worker_process_cycle方法 */
        ngx_pid = ngx_getpid();
        proc(cycle, data);
        break;

    default:
        break;
    }
```



#### 4. ngx_worker_process_cycle 子进程的回调函数

ngx_worker_process_cycle为子进程的回调函数，一切子进程的工作从这个方法开始。

Nginx的进程最终也是有事件驱动的，所有这个方法中，最终会调用**ngx_process_events_and_timers**事件驱动的核心函数。



```cpp
/**
 * 子进程 回调函数
 * 每个进程的逻辑处理就从这个方法开始
 */
static void ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data) {
	ngx_int_t worker = (intptr_t) data;

	ngx_process = NGX_PROCESS_WORKER;
	ngx_worker = worker;

	/* 工作进程初始化 */
	ngx_worker_process_init(cycle, worker);

	ngx_setproctitle("worker process");

	/* 进程循环 */
	for (;;) {

		/* 判断是否是退出的状态，如果退出，则需要清空socket连接句柄 */
		if (ngx_exiting) {
			ngx_event_cancel_timers();

			if (ngx_event_timer_rbtree.root
					== ngx_event_timer_rbtree.sentinel) {
				ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "exiting");

				ngx_worker_process_exit(cycle);
			}
		}

		ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "worker cycle");

		/* 事件驱动核心函数 */
		ngx_process_events_and_timers(cycle);

		if (ngx_terminate) {
			ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "exiting");

			ngx_worker_process_exit(cycle);
		}

		/* 如果是退出 */
		if (ngx_quit) {
			ngx_quit = 0;
			ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
					"gracefully shutting down");
			ngx_setproctitle("worker process is shutting down");

			if (!ngx_exiting) {
				ngx_exiting = 1;
				ngx_close_listening_sockets(cycle);
				ngx_close_idle_connections(cycle);
			}
		}

		/* 如果是重启 */
		if (ngx_reopen) {
			ngx_reopen = 0;
			ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reopening logs");
			ngx_reopen_files(cycle, -1);
		}
	}
}
```

#### 5. ngx_worker_process_init 工作进程初始化



```cpp
/**
 * 工作进程初始化
 */
static void ngx_worker_process_init(ngx_cycle_t *cycle, ngx_int_t worker) {
	sigset_t set;
	ngx_int_t n;
	ngx_uint_t i;
	ngx_cpuset_t *cpu_affinity;
	struct rlimit rlmt;
	ngx_core_conf_t *ccf;
	ngx_listening_t *ls;

	/* 配置环境变量 */
	if (ngx_set_environment(cycle, NULL) == NULL) {
		/* fatal */
		exit(2);
	}

	/* 获取核心配置 */
	ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

	if (worker >= 0 && ccf->priority != 0) {
		if (setpriority(PRIO_PROCESS, 0, ccf->priority) == -1) {
			ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
					"setpriority(%d) failed", ccf->priority);
		}
	}

	if (ccf->rlimit_nofile != NGX_CONF_UNSET) {
		rlmt.rlim_cur = (rlim_t) ccf->rlimit_nofile;
		rlmt.rlim_max = (rlim_t) ccf->rlimit_nofile;

		if (setrlimit(RLIMIT_NOFILE, &rlmt) == -1) {
			ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
					"setrlimit(RLIMIT_NOFILE, %i) failed", ccf->rlimit_nofile);
		}
	}

	if (ccf->rlimit_core != NGX_CONF_UNSET) {
		rlmt.rlim_cur = (rlim_t) ccf->rlimit_core;
		rlmt.rlim_max = (rlim_t) ccf->rlimit_core;

		if (setrlimit(RLIMIT_CORE, &rlmt) == -1) {
			ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
					"setrlimit(RLIMIT_CORE, %O) failed", ccf->rlimit_core);
		}
	}

	/* 设置UID GROUPUID */
	if (geteuid() == 0) {
		if (setgid(ccf->group) == -1) {
			ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
					"setgid(%d) failed", ccf->group);
			/* fatal */
			exit(2);
		}

		if (initgroups(ccf->username, ccf->group) == -1) {
			ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
					"initgroups(%s, %d) failed", ccf->username, ccf->group);
		}

		if (setuid(ccf->user) == -1) {
			ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
					"setuid(%d) failed", ccf->user);
			/* fatal */
			exit(2);
		}
	}

	/* 设置CPU亲和性 */
	if (worker >= 0) {
		cpu_affinity = ngx_get_cpu_affinity(worker);

		if (cpu_affinity) {
			ngx_setaffinity(cpu_affinity, cycle->log);
		}
	}

#if (NGX_HAVE_PR_SET_DUMPABLE)

	/* allow coredump after setuid() in Linux 2.4.x */

	if (prctl(PR_SET_DUMPABLE, 1, 0, 0, 0) == -1) {
		ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
				"prctl(PR_SET_DUMPABLE) failed");
	}

#endif

	/* 切换工作目录 */
	if (ccf->working_directory.len) {
		if (chdir((char *) ccf->working_directory.data) == -1) {
			ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
					"chdir(\"%s\") failed", ccf->working_directory.data);
			/* fatal */
			exit(2);
		}
	}

	sigemptyset(&set);

	/* 清除所有信号 */
	if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) {
		ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
				"sigprocmask() failed");
	}

	srandom((ngx_pid << 16) ^ ngx_time());

	/*
	 * disable deleting previous events for the listening sockets because
	 * in the worker processes there are no events at all at this point
	 */
	/* 清除sokcet的监听 */
	ls = cycle->listening.elts;
	for (i = 0; i < cycle->listening.nelts; i++) {
		ls[i].previous = NULL;
	}

	/* 对模块初始化  */
	for (i = 0; cycle->modules[i]; i++) {
		if (cycle->modules[i]->init_process) {
			if (cycle->modules[i]->init_process(cycle) == NGX_ERROR) {
				/* fatal */
				exit(2);
			}
		}
	}

	/**
	 *将其他进程的channel[1]关闭，自己的channel[0]关闭
	 */
	for (n = 0; n < ngx_last_process; n++) {

		if (ngx_processes[n].pid == -1) {
			continue;
		}

		if (n == ngx_process_slot) {
			continue;
		}

		if (ngx_processes[n].channel[1] == -1) {
			continue;
		}

		if (close(ngx_processes[n].channel[1]) == -1) {
			ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
					"close() channel failed");
		}
	}

	if (close(ngx_processes[ngx_process_slot].channel[0]) == -1) {
		ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
				"close() channel failed");
	}

#if 0
	ngx_last_process = 0;
#endif

	/**
	 * 给ngx_channel注册一个读事件处理函数
	 */
	if (ngx_add_channel_event(cycle, ngx_channel, NGX_READ_EVENT,
			ngx_channel_handler) == NGX_ERROR) {
		/* fatal */
		exit(2);
	}
}
```



下一章节，我们会讲解**Nginx的惊群和进程间的负载均衡处理。**



















