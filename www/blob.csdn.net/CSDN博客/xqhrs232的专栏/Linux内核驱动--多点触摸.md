# Linux内核驱动--多点触摸 - xqhrs232的专栏 - CSDN博客
2011年09月05日 16:30:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：927
原文地址::[http://blog.chinaunix.net/space.php?uid=20378444&do=blog&id=139386](http://blog.chinaunix.net/space.php?uid=20378444&do=blog&id=139386)
简介 
　　为了使用功能强大的多点触控设备，就需要一种方案去上报用户层所需的详细的手指触摸数据。这个文档所描述的多点触控协议可以让内核驱动程序向用户层上报任意多指的数据信息。
　　使用说明
　　单点触摸信息是以ABS承载并按一定顺序发送，如BTN_TOUCH、ABS_X、ABS_Y、SYNC。而多点触摸信息则是以ABS_MT承载并按一定顺序发送，如ABS_MT_POSITION_X、ABS_MT_POSITION_Y，然后通过调用input_mt_sync()产生一个 SYN_MT_REPORT event来标记一个点的结束，告诉接收方接收当前手指的信息并准备接收其它手指的触控信息。最后调用 input_sync()函数上报触摸信息开始动作并告诉接收方开始接收下一系列多点触摸信息。
　　协议定义了一系列ABS_MT事件，这些事件被分为几大类，充许只应用其中的一部份，多点触摸最小的事件集中应包括ABS_MT_TOUCH_MAJOR、ABS_MT_POSITION_X和 ABS_MT_POSITION_Y，以此来实现多点触摸。如果设备支持ABS_MT_WIDTH_MAJOR这个事件，那么此事件可以提供手指触摸接触面积大小。触摸方向等信息可以由ABS_MT_TOUCH_MINOR, ABS_MT_WIDTH_MINOR and ABS_MT_ORIENTATION提供。ABS_MT_TOOL_TYPE提供触摸设备的类别，如手或是笔或是其它。最后有些设备可能会支持ABS_MT_TRACKING_ID，用来支持硬件跟踪多点信息，即该点属于哪一条线等。
下面是两点触摸支持的最小事件集序列：
ABS_MT_TOUCH_MAJOR 
ABS_MT_POSITION_X
ABS_MT_POSITION_Y
SYN_MT_REPORT      //上报第一个点
ABS_MT_TOUCH_MAJOR
ABS_MT_POSITION_X
ABS_MT_POSITION_Y
SYN_MT_REPORT      //上报第二个点
SYN_REPORT         //开始动作
Event 原语
“接触”一词用来描述一个物体直接碰到另一个物体的表面。
ABS_MT_TOUCH_MAJOR描述了主接触面的长轴，它和X，Y同一个单位，如果一个面的分辨率为X*Y，则ABS_MT_TOUCH_MAJOR的最大值为sqrt(X^2+Y^2)
ABS_MT_TOUCH_MINOR描述了接触面的短轴，如果接触面是圆形，它可以不用。
ABS_MT_WIDTH_MAJOR描述了接触工具的长轴
ABS_MT_WIDTH_MINOR描述了接触工具的短轴
ABS_MT_TOUCH_MAJOR := max(X, Y) 
ABS_MT_TOUCH_MINOR := min(X, Y)
ABS_MT_ORIENTATION := bool(X > Y)
以上四个参数可以用来生成额外的触摸信息，如ABS_MT_TOUCH_MAJOR/ABS_MT_WIDTH_MAJOR的比率可以用来描述压力。
ABS_MT_ORIENTATION
ABS_MT_POSITION_X接触面的中心点X坐标
ABS_MT_POSITION_Y接触面的中心点Y坐标
ABS_MT_TOOL_TYPE描述接触工具类型，很多内核驱动无法区分此参数如手指及笔，如果是这样，该参数可以不用，协议目前支持MT_TOOL_FINGER和MT_TOOL_PEN两种类型。
ABS_MT_BLOB_ID形状集ID，集合几个点以描述一个形状，很多驱动没有形状属性，此参数可以不用。
ABS_MT_TRACKING_ID描述了从接触开始到释放的整个过程的集合，如果设备不支持，此参数可是不用。
　　触摸轨迹
　　仅有少数设备可以明触的标识真实的 trackingID，多数情况下 trackingID只能来标识一次触摸动作的过程。
　　手势
　　多点触摸指定的应用是创建手势动作， TOUCH和 WIDTH参数经常用来区别手指的压力和手指间的距离，另外 MINOR类的参数可以用来区别设备的接触面的大小（点接触还是面接触),ORIENTATION可以产生旋转事件。
参考代码：
(1)注册多点触摸设备
ts->input_dev = input_allocate_device();
        if (ts->input_dev == NULL) {
                ret = -ENOMEM;
                printk(KERN_ERR "Failed to allocate input device\n");
                goto err_input_dev_alloc_failed;
        }
        ts->input_dev->name = "MT-touchscreen";
        set_bit(EV_SYN, ts->input_dev->evbit);
        set_bit(EV_KEY, ts->input_dev->evbit);
        set_bit(EV_ABS, ts->input_dev->evbit);
        set_bit(BTN_TOUCH, ts->input_dev->keybit);
        max_x=0x77b;
        max_y=0xb38;
        input_set_abs_params(ts->input_dev, ABS_X, 0, max_x, 0, 0);
        input_set_abs_params(ts->input_dev, ABS_Y, 0, max_y, 0, 0);
        input_set_abs_params(ts->input_dev, ABS_PRESSURE, 0, 255, 0, 0);
        input_set_abs_params(ts->input_dev, ABS_TOOL_WIDTH, 0, 15, 0, 0);
        input_set_abs_params(ts->input_dev, ABS_MT_POSITION_X, 0, max_x, 0, 0);
        input_set_abs_params(ts->input_dev, ABS_MT_POSITION_Y, 0, max_y, 0, 0);
        input_set_abs_params(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
        input_set_abs_params(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0, 15, 0, 0);
        ret = input_register_device(ts->input_dev);
        if (ret) {
                printk(KERN_ERR "Unable to register %s input device\n", ts->input_dev->name);
                goto err_input_register_device_failed;
        }
(2) 报点
if(finger==2)
{
        input_report_key(ts->input_dev,ABS_MT_TRACKING_ID,0);
        input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, f1R);
        input_report_abs(ts->input_dev, ABS_MT_POSITION_X, f1x);
        input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, f1y);
        input_mt_sync(ts->input_dev);
        input_report_key(ts->input_dev,ABS_MT_TRACKING_ID,1);
        input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, f2z);
        input_report_abs(ts->input_dev, ABS_MT_POSITION_X, f2x);
        input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, f2y);
        input_mt_sync(ts->input_dev)
        input_sync(ts->input_dev);
}
else if(finger==1)
{
        input_report_key(ts->input_dev,ABS_MT_TRACKING_ID,0);
        input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, f1R);
        input_report_abs(ts->input_dev, ABS_MT_POSITION_X, f1x);
        input_report_abs(ts->input_dev, ABS_MT_POSITION_Y, f1y);
        input_mt_sync(ts->input_dev);
        input_sync(ts->input_dev);
}
文章出处：飞诺网([www.firnow.com):http://dev.firnow.com/course/6_system/linux/Linuxjs/20101230/550212.html](http://blog.chinaunix.net/link.php?url=http://www.firnow.com%29%3Ahttp%3A%2F%2Fdev.firnow.com%2Fcourse%2F6_system%2Flinux%2FLinuxjs%2F20101230%2F550212.html)
