# 【整理】Linux驱动中，probe函数何时被调用 - xqhrs232的专栏 - CSDN博客
2010年12月13日 16:15:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：590
原文地址::
[http://www.rosoo.net/a/201005/9455.html](http://www.rosoo.net/a/201005/9455.html)
【整理】Linux驱动中，probe函数何时被调用 
声明：以下主要内容参考自：关于struct device_driver结构中的probe探测函数的调用 [http://blog.chinaunix.net/u2/71164/showart.php?id=1361188](http://blog.chinaunix.net/u2/71164/showart.php?id=1361188)
用SourceInsight跟踪：
从driver_register看起，此处我的这里是：
int driver_register(struct device_driver * drv)
{
if ((drv->bus->probe && drv->probe) ||
     (drv->bus->remove && drv->remove) ||
     (drv->bus->shutdown && drv->shutdown)) {
   printk(KERN_WARNING "Driver '%s' needs updating - please use bus_type methods/n", drv->name);
}
klist_init(&drv->klist_devices, NULL, NULL);
return bus_add_driver(drv);
}
klist_init不相关，不用管他，具体再去看bus_add_driver：
int bus_add_driver(struct device_driver *drv)
{
1.先kobject_set_name(&drv->kobj, "%s", drv->name);
2.再kobject_register(&drv->kobj)
3.然后调用了：driver_attach(drv)
}
int driver_attach(struct device_driver * drv)
{
return bus_for_each_dev(drv->bus, NULL, drv, __driver_attach);
}
真正起作用的是__driver_attach：
static int __driver_attach(struct device * dev, void * data)
{
。。。
if (!dev->driver)
   driver_probe_device(drv, dev);
。。。
}
int driver_probe_device(struct device_driver * drv, struct device * dev)
{
。。。
//1.先是判断bus是否match：
if (drv->bus->match && !drv->bus->match(dev, drv))
   goto done;
//2.再具体执行probe：
ret = really_probe(dev, drv);
。。。
}
really_probe才是我们要找的函数：
static int really_probe(struct device *dev, struct device_driver *drv)
{
。。。
//1.先是调用的驱动所属总线的probe函数：
if (dev->bus->probe) {
   ret = dev->bus->probe(dev);
   if (ret)
    goto probe_failed;
} else if (drv->probe) {
//2.再调用你的驱动中的probe函数：
   ret = drv->probe(dev);
   if (ret)
    goto probe_failed;
}
。。。
}
其中，drv->probe(dev)，才是真正调用你的驱动实现的具体的probe函数。
也就是对应此文标题所问的，probe函数此时被调用。
