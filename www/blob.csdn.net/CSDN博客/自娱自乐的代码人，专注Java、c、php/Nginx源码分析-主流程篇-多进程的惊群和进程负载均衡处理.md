# Nginx源码分析 - 主流程篇 - 多进程的惊群和进程负载均衡处理 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年08月21日 14:05:28[initphp](https://me.csdn.net/initphp)阅读数：8498
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Linux2.6版本之前还存在对于socket的accept的惊群现象。之后的版本已经解决掉了这个问题。

惊群是指多个进程/线程在等待同一资源时，每当资源可用，所有的进程/线程都来竞争资源的现象。

Nginx采用的是多进程的模式。假设Linux系统是2.6版本以前，当有一个客户端要连到Nginx服务器上，Nginx的N个进程都会去监听socket的accept的，如果全部的N个进程都对这个客户端的socket连接进行了监听，就会造成资源的竞争甚至数据的错乱**。**我们要保证的是，一个链接在Nginx的一个进程上处理，包括accept和read/write事件。****




### Nginx解决惊群和进程负载均衡处理的要点

1. Nginx的N个进程会争抢文件锁，当只有拿到**文件锁**的进程，才能处理**accept的事件**。

2. 没有拿到文件锁的进程，只能处理当前连接对象的**read事件**

3. 当单个进程总的connection连接数达到总数的**7/8**的时候，就不会再接收新的accpet事件。

4. 如果拿到锁的进程能很快处理完accpet，而没拿到锁的一直在等待（等待时延：**ngx_accept_mutex_delay**），容易造成进程忙的很忙，空的很空


![](https://img-blog.csdn.net/20160821144404775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 具体的实现

#### ngx_process_events_and_timers 进程事件分发器

此方法为进程实现的核心函数。主要作用：事件分发；惊群处理；简单的负载均衡。




**负载均衡：**

1.  当事件配置初始化的时候，会设置一个全局变量：ngx_accept_disabled = ngx_cycle->connection_n / 8 - ngx_cycle->free_connection_n;


2.  当ngx_accept_disabled为正数的时候，connection达到连接总数的7/8的时候，就不再处理新的连接accept事件，只处理当前连接的read事件




**惊群处理：**


1. 通过**ngx_trylock_accept_mutex**争抢文件锁，拿到文件锁的，才可以处理accept事件。

2. **ngx_accept_mutex_held**是拿到锁的一个标志，当拿到锁了，flags会被设置成**NGX_POST_EVENTS**，这个标志会在事件处理函数**ngx_process_events**中将所有事件（accept和read）放入对应的**ngx_posted_accept_events和ngx_posted_events**队列中进行**延后处理**。

3. 当没有拿到锁，调用事件处理函数**ngx_process_events**的时候，可以明确都是read的事件，所以可以直接调用事件ev->handler方法回调处理。

4. 拿到锁的进程，接下来会优先处理**ngx_posted_accept_events**队列上的accept事件，处理函数：**ngx_event_process_posted**

5. 处理完accept事件后，就将文件锁释放

6. 接下来处理**ngx_posted_events**队列上的read事件，处理函数：**ngx_event_process_posted**

****



```cpp
/**
 * 进程事件分发器
 */
void ngx_process_events_and_timers(ngx_cycle_t *cycle) {
	ngx_uint_t flags;
	ngx_msec_t timer, delta;

	if (ngx_timer_resolution) {
		timer = NGX_TIMER_INFINITE;
		flags = 0;

	} else {
		timer = ngx_event_find_timer();
		flags = NGX_UPDATE_TIME;

#if (NGX_WIN32)

		/* handle signals from master in case of network inactivity */

		if (timer == NGX_TIMER_INFINITE || timer > 500) {
			timer = 500;
		}

#endif
	}

	/**
	 * ngx_use_accept_mutex变量代表是否使用accept互斥体
	 * 默认是使用，可以通过accept_mutex off;指令关闭；
	 * accept mutex 的作用就是避免惊群，同时实现负载均衡
	 */
	if (ngx_use_accept_mutex) {

		/**
		 * 	ngx_accept_disabled = ngx_cycle->connection_n / 8 - ngx_cycle->free_connection_n;
		 * 	当connection达到连接总数的7/8的时候，就不再处理新的连接accept事件，只处理当前连接的read事件
		 * 	这个是比较简单的一种负载均衡方法
		 */
		if (ngx_accept_disabled > 0) {
			ngx_accept_disabled--;

		} else {
			/* 获取锁失败 */
			if (ngx_trylock_accept_mutex(cycle) == NGX_ERROR) {
				return;
			}

			/* 拿到锁 */
			if (ngx_accept_mutex_held) {
				/**
				 * 给flags增加标记NGX_POST_EVENTS，这个标记作为处理时间核心函数ngx_process_events的一个参数，这个函数中所有事件将延后处理。
				 * accept事件都放到ngx_posted_accept_events链表中，
				 * epollin|epollout普通事件都放到ngx_posted_events链表中
				 **/
				flags |= NGX_POST_EVENTS;

			} else {

				/**
				 * 1. 获取锁失败，意味着既不能让当前worker进程频繁的试图抢锁，也不能让它经过太长事件再去抢锁
				 * 2. 开启了timer_resolution时间精度，需要让ngx_process_change方法在没有新事件的时候至少等待ngx_accept_mutex_delay毫秒之后再去试图抢锁
				 * 3. 没有开启时间精度时，如果最近一个定时器事件的超时时间距离现在超过了ngx_accept_mutex_delay毫秒，也要把timer设置为ngx_accept_mutex_delay毫秒
				 * 4. 不能让ngx_process_change方法在没有新事件的时候等待的时间超过ngx_accept_mutex_delay，这会影响整个负载均衡机制
				 * 5. 如果拿到锁的进程能很快处理完accpet，而没拿到锁的一直在等待，容易造成进程忙的很忙，空的很空
				 */
				if (timer == NGX_TIMER_INFINITE
						|| timer > ngx_accept_mutex_delay) {
					timer = ngx_accept_mutex_delay;
				}
			}
		}
	}

	delta = ngx_current_msec;

	/**
	 * 事件调度函数
	 * 1. 当拿到锁，flags=NGX_POST_EVENTS的时候，不会直接处理事件，
	 * 将accept事件放到ngx_posted_accept_events，read事件放到ngx_posted_events队列
	 * 2. 当没有拿到锁，则处理的全部是read事件，直接进行回调函数处理
	 * 参数：timer-epoll_wait超时时间  (ngx_accept_mutex_delay-延迟拿锁事件   NGX_TIMER_INFINITE-正常的epollwait等待事件)
	 */
	(void) ngx_process_events(cycle, timer, flags);

	delta = ngx_current_msec - delta;

	ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
			"timer delta: %M", delta);
	/**
	 * 1. ngx_posted_accept_events是一个事件队列，暂存epoll从监听套接口wait到的accept事件
	 * 2. 这个方法是循环处理accpet事件列队上的accpet事件
	 */
	ngx_event_process_posted(cycle, &ngx_posted_accept_events);

	/**
	 * 如果拿到锁，处理完accept事件后，则释放锁
	 */
	if (ngx_accept_mutex_held) {
		ngx_shmtx_unlock(&ngx_accept_mutex);
	}

	if (delta) {
		ngx_event_expire_timers();
	}

	/**
	 *1. 普通事件都会存放在ngx_posted_events队列上
	 *2. 这个方法是循环处理read事件列队上的read事件
	 */
	ngx_event_process_posted(cycle, &ngx_posted_events);
}
```

#### ngx_trylock_accept_mutex 获取accept锁

1. ngx_accept_mutex_held是拿到锁的唯一标识的全局变量。

2. 当拿到锁，则调用ngx_enable_accept_events，将新的connection加入event事件上

3. 如果没有拿到锁，则调用ngx_disable_accept_events。



```cpp
/**
 * 获取accept锁
 */
ngx_int_t ngx_trylock_accept_mutex(ngx_cycle_t *cycle) {
	/**
	 * 拿到锁
	 */
	if (ngx_shmtx_trylock(&ngx_accept_mutex)) {

		ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
				"accept mutex locked");

		/* 多次进来，判断是否已经拿到锁 */
		if (ngx_accept_mutex_held && ngx_accept_events == 0) {
			return NGX_OK;
		}

		/* 调用ngx_enable_accept_events，开启监听accpet事件*/
		if (ngx_enable_accept_events(cycle) == NGX_ERROR) {
			ngx_shmtx_unlock(&ngx_accept_mutex);
			return NGX_ERROR;
		}

		ngx_accept_events = 0;
		ngx_accept_mutex_held = 1;

		return NGX_OK;
	}

	ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
			"accept mutex lock failed: %ui", ngx_accept_mutex_held);

	/**
	 * 没有拿到锁，但是ngx_accept_mutex_held=1
	 */
	if (ngx_accept_mutex_held) {
		/* 没有拿到锁，调用ngx_disable_accept_events，将accpet事件删除 */
		if (ngx_disable_accept_events(cycle, 0) == NGX_ERROR) {
			return NGX_ERROR;
		}

		ngx_accept_mutex_held = 0;
	}

	return NGX_OK;
}
```



#### ngx_enable_accept_events 和 ngx_disable_accept_events



```cpp
/**
 * 开启accept事件的监听
 * 并将accept事件加入到event上
 */
static ngx_int_t ngx_enable_accept_events(ngx_cycle_t *cycle) {
	ngx_uint_t i;
	ngx_listening_t *ls;
	ngx_connection_t *c;

	ls = cycle->listening.elts;
	for (i = 0; i < cycle->listening.nelts; i++) {

		c = ls[i].connection;

		if (c == NULL || c->read->active) {
			continue;
		}

		if (ngx_add_event(c->read, NGX_READ_EVENT, 0) == NGX_ERROR) {
			return NGX_ERROR;
		}
	}

	return NGX_OK;
}

/**
 * 关闭accept事件的监听
 * 并将accept事件从event上删除
 */
static ngx_int_t ngx_disable_accept_events(ngx_cycle_t *cycle, ngx_uint_t all) {
	ngx_uint_t i;
	ngx_listening_t *ls;
	ngx_connection_t *c;

	ls = cycle->listening.elts;
	for (i = 0; i < cycle->listening.nelts; i++) {

		c = ls[i].connection;

		/* 如果c->read->active，则表示是活跃的连接，已经被使用中 */
		if (c == NULL || !c->read->active) {
			continue;
		}

#if (NGX_HAVE_REUSEPORT)

		/*
		 * do not disable accept on worker's own sockets
		 * when disabling accept events due to accept mutex
		 */

		if (ls[i].reuseport && !all) {
			continue;
		}

#endif

		/* 删除事件 */
		if (ngx_del_event(c->read, NGX_READ_EVENT,
				NGX_DISABLE_EVENT) == NGX_ERROR) {
			return NGX_ERROR;
		}
	}

	return NGX_OK;
}
```

#### ngx_event_process_posted 事件队列处理

对ngx_posted_accept_events或ngx_posted_events队列上的accept/read事件进行回调处理。



```cpp
/**
 * 处理事件队列
 *
 */
void ngx_event_process_posted(ngx_cycle_t *cycle, ngx_queue_t *posted) {
	ngx_queue_t *q;
	ngx_event_t *ev;

	while (!ngx_queue_empty(posted)) {

		q = ngx_queue_head(posted);
		ev = ngx_queue_data(q, ngx_event_t, queue);

		ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
				"posted event %p", ev);

		ngx_delete_posted_event(ev);

		/* 事件回调函数 */
		ev->handler(ev);
	}
}
```



#### ngx_process_events 事件的核心处理函数

这个方法，我们主要看epoll模型下的**ngx_epoll_process_events**方法（ngx_epoll_module.c）

1. 如果抢到了锁，则会将accpet/read事件放到队列上延后处理。

2. 没有抢到锁的进程都是处理当前连接的read事件，所以直接进行处理。




```cpp
/* 读取事件 EPOLLIN */
        if ((revents & EPOLLIN) && rev->active) {

#if (NGX_HAVE_EPOLLRDHUP)
            if (revents & EPOLLRDHUP) {
                rev->pending_eof = 1;
            }

            rev->available = 1;
#endif

            rev->ready = 1;

            /* 如果进程抢到锁，则放入事件队列 */
            if (flags & NGX_POST_EVENTS) {
                queue = rev->accept ? &ngx_posted_accept_events
                                    : &ngx_posted_events;

                ngx_post_event(rev, queue);

            } else {
            	/* 没有抢到锁，则直接处理read事件*/
                rev->handler(rev);
            }
        }
```




下一章节开始，我们会进入Nginx的事件模块。















