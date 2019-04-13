
# linux kernel编译 undefined reference to `__ae - 嵌入式Linux - CSDN博客

2018年12月29日 10:00:18[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：43



# 问题代码
`ktime_t calltime, delta, rettime;
 unsigned long long duration;
 calltime = ktime_get();

/*获取当前时间*/
rettime = ktime_get();
/*计算时间差 ktime_sub(A,B) 计算A-B的时间差 返回纳秒*/
delta = ktime_sub(rettime,calltime);
/*转换成微秒*/
duration = (unsigned long long) ktime_to_ns(delta) >> 10;
printk("[%s] running %lld  usecs %d ms at %d\n",__FUNCTION__, duration,(unsigned int)(duration/1000),__LINE__);`
# 编译出错
`/rk3288_pad/kernel/drivers/input/sensors/sensor-dev.c:505: undefined reference to `__aeabi_uldivmod'
/data/weiqifa/rk3288_pad/kernel/drivers/input/sensors/sensor-dev.c:529: undefined reference to `__aeabi_uldivmod'
Makefile:997: recipe for target 'vmlinux' failed
make: *** [vmlinux] Error 1`
# 原因
在大多数32位CPU上，必须使用慢库函数实现64位除法。为了防止编译器生成非常慢的代码，Linux不实现这些功能。
如果要进行64位除法，则必须明确执行这些操作。使用`do_div()`从`<asm/div64.h>`

# 解决
`unsigned long long duration;
	unsigned int durms;
	calltime = ktime_get();
	if (rate < 5)
		rate = 5;
	else if (rate > 200)
		rate = 200;
	dev_info(&client->dev, "set sensor poll time to %dms\n", rate);
	/* work queue is always slow, we need more quickly to match hal rate */
	if (sensor->pdata->poll_delay_ms == (rate - 5))
	{
		/*获取当前时间*/
		rettime = ktime_get();
		/*计算时间差 ktime_sub(A,B) 计算A-B的时间差 返回纳秒*/
		delta = ktime_sub(rettime,calltime);
		/*转换成微秒*/
		duration = (unsigned long long) ktime_to_ns(delta) >> 10;
		durms = (unsigned int)duration/1000;
		printk("[%s] running %lld  usecs %d ms at %d\n",__FUNCTION__, duration,durms,__LINE__);
		return 0;
	}`这样会丢掉32bit.直接使用函数`do_div可能会更好`
`参考`
[https://blog.csdn.net/Tommy_wxie/article/details/8883488](https://blog.csdn.net/Tommy_wxie/article/details/8883488)


