
# dynamic_debug动态打印kernel日志 - 嵌入式Linux - CSDN博客

2015年03月03日 15:34:54[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：3398


1、修改代码如下：
`--- a/kernel/drivers/gosodrv/touchscreen/ft5x06_ts.c
+++ b/kernel/drivers/gosodrv/touchscreen/ft5x06_ts.c
@@ -402,6 +402,7 @@ static void ft5x0x_ts_pen_irq_work(struct work_struct *work)
        struct ft5x06_ts_data *data = container_of(work, struct ft5x06_ts_data,
                                          pen_event_work);
        //printk("ft5x0x_ts_pen_irq_work\n");
+       pr_debug("%s: dev_dbg test owen.wei ==============\n", __func__);
        ret = ft5x06_handle_touchdata(data);
        if (ret)
        pr_err("%s: handling touchdata failed\n", __func__);`
2、USB连接上手机
adb root 获取root权限
255|root@android:/ \# mkdir data/debugfs建立debugfs日志文件
mkdir data/debugfs
root@android:/ \# mount -t debugfs none /data/debugfs 挂载到这个日志文件里去
mount -t debugfs none /data/debugfs
root@android:/ \#echo -n "file ft5x06_ts.c +p" > /data/debugfs/dynamic_debug/control  打开ft5x06_ts.c下面的pr_debug日志输出
/data/debugfs/dynamic_debug/control                                          <
root@android:/ \#
3、kernel要打开enabe debugfs, enable CONFIG_DYNAMIC_DEBUG
4、示例
Examples ========
// enable the message at line 1603 of file svcsock.c
nullarbor:~ \# echo -n 'file svcsock.c line 1603 +p' > <debugfs>/dynamic_debug/control 打开某个文件某一行的日志
// enable all the messages in file svcsock.c  打开某个文件的所有日志
nullarbor:~ \# echo -n 'file svcsock.c +p' > <debugfs>/dynamic_debug/control
// enable all the messages in the NFS server module 打开某个模块的所有日志
nullarbor:~ \# echo -n 'module nfsd +p' > <debugfs>/dynamic_debug/control
// enable all 12 messages in the function svc_process() 打开某个函数的所有日志
nullarbor:~ \# echo -n 'func svc_process +p' > <debugfs>/dynamic_debug/control
// disable all 12 messages in the function svc_process() 关闭某个文件的所有日志
nullarbor:~ \# echo -n 'func svc_process -p' > <debugfs>/dynamic_debug/control
// enable messages for NFS calls READ, READLINK, READDIR and READDIR+.
nullarbor:~ \# echo -n 'format "nfsd: READ" +p' > <debugfs>/dynamic_debug/control


