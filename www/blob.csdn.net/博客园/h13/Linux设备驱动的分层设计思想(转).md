# Linux设备驱动的分层设计思想(转) - h13 - 博客园
##### 1.1 设备驱动核心层和例化
在面向对象的程序设计中，可以为某一类相似的事物定义一个基类，而具体的事物可以继承这个基类中的函数。如果对于继承的这个事物而言，其某函数的实现与基类一致，那它就可以直接继承基类的函数；相反，它可以重载之。这种面向对象的设计思想极大地提高了代码的可重用能力，是对现实世界事物间关系的一种良好呈现。
Linux内核完全由C语言和汇编语言写成，但是却频繁用到了面向对象的设计思想。在设备驱动方面，往往为同类的设备设计了一个框架，而框架中的核心层则实现了该设备通用的一些功能。同样的，如果具体的设备不想使用核心层的函数，它可以重载之。举个例子：
return_type core_funca(xxx_device * bottom_dev, param1_type param1, param1_type param2)
{
if (bottom_dev->funca)
return bottom_dev->funca(param1, param2);
/* 核心层通用的funca代码 */
...
}
上述core_funca的实现中，会检查底层设备是否重载了funca()，如果重载了，就调用底层的代码，否则，直接使用通用层的。这样做的好处是，核心层的代码可以处理绝大多数该类设备的funca()对应的功能，只有少数特殊设备需要重新实现funca()。
再看一个例子：
return_type core_funca(xxx_device * bottom_dev, param1_type param1, param1_type param2)
{
/*通用的步骤代码A */
...
bottom_dev->funca_ops1();
/*通用的步骤代码B */
...
bottom_dev->funca_ops2();
/*通用的步骤代码C */
...
bottom_dev->funca_ops3();
}
上述代码假定为了实现funca()，对于同类设备而言，操作流程一致，都要经过“通用代码A、底层ops1、通用代码B、底层ops2、通用代码 C、底层ops3”这几步，分层设计明显带来的好处是，对于通用代码A、B、C，具体的底层驱动不需要再实现，而仅仅只关心其底层的操作ops1、 ops2、ops3。
图1明确反映了设备驱动的核心层与具体设备驱动的关系，实际上，这种分层可能只有2层（图1的a），也可能是多层的（图1的b）。
![clip_image002](http://www.linuxdriver.cn/blog_images/Linux_532/clip_image002_thumb.jpg)
图1 Linux设备驱动的分层
这样的分层化设计在Linux的input、RTC、MTD、I2 C、SPI、TTY、USB等诸多设备驱动类型中屡见不鲜。下面的2节以input和RTC为例先行进行一番说明，当然，后续的章节会对几个大的设备类型对应驱动的层次进行更详细的分析。
##### 1.2 输入设备驱动
输入设备（如按键、键盘、触摸屏、鼠标等）是典型的字符设备，其一般的工作机理是底层在按键、触摸等动作发送时产生一个中断（或驱动通过timer 定时查询），然后CPU通过SPI、I2 C或外部存储器总线读取键值、坐标等数据，放入1个缓冲区，字符设备驱动管理该缓冲区，而驱动的read()接口让用户可以读取键值、坐标等数据。
显然，在这些工作中，只是中断、读值是设备相关的，而输入事件的缓冲区管理以及字符设备驱动的file_operations接口则对输入设备是通用的。基于此，内核设计了输入子系统，由核心层处理公共的工作。Linux内核输入子系统的框架如图2所示。
![clip_image004](http://www.linuxdriver.cn/blog_images/Linux_532/clip_image004_thumb.jpg)
图2 Linux输入设备驱动的分层
输入核心提供了底层输入设备驱动程序所需的API，如分配/释放一个输入设备：
struct input_dev *input_allocate_device(void);
void input_free_device(struct input_dev *dev);
input_allocate_device()返回的是1个input_dev的结构体，此结构体用于表征1个输入设备。
注册/注销输入设备用的如下接口：
int __must_check input_register_device(struct input_dev *);
void input_unregister_device(struct input_dev *);
报告输入事件用的如下接口：
/* 报告指定type、code的输入事件 */
void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value); 
/* 报告键值 */
void input_report_key(struct input_dev *dev, unsigned int code, int value);
/* 报告相对坐标 */
void input_report_rel(struct input_dev *dev, unsigned int code, int value);
/* 报告绝对坐标 */
void input_report_abs(struct input_dev *dev, unsigned int code, int value);
/* 报告同步事件 */
void input_sync(struct input_dev *dev);
而所有的输入事件，内核都用统一的数据结构来描述，这个数据结构是input_event，形如代码清单7。
代码清单7 input_event结构体
1 struct input_event {
2 struct timeval time;
3 __u16 type;
4 __u16 code;
5 __s32 value;
6 };
drivers/input/keyboard/gpio_keys.c基于input架构实现了一个通用的GPIO按键驱动。该驱动基于 platform_driver架构，名为“gpio-keys”。它将硬件相关的信息（如使用的GPIO号，电平等）屏蔽在板文件 platform_device的platform_data中，因此该驱动可应用于各个处理器，具有良好的跨平台性。代码清单8列出了该驱动的 probe()函数。
代码清单8 GPIO按键驱动的probe()函数
1 static int __devinit gpio_keys_probe(struct platform_device *pdev)
2 {
3 struct gpio_keys_platform_data *pdata = pdev->dev.platform_data;
4 struct gpio_keys_drvdata *ddata;
5 struct input_dev *input;
6 int i, error;
7 int wakeup = 0;
8
9 ddata = kzalloc(sizeof(struct gpio_keys_drvdata) +
10 pdata->nbuttons * sizeof(struct gpio_button_data),
11 GFP_KERNEL);
**12 input = input_allocate_device();**
13 if (!ddata || !input) {
14 error = -ENOMEM;
15 goto fail1;
16 }
17 
18 platform_set_drvdata(pdev, ddata);
19 
20 input->name = pdev->name;
21 input->phys = "gpio-keys/input0";
22 input->dev.parent = &pdev->dev;
23 
24 input->id.bustype = BUS_HOST;
25 input->id.vendor = 0x0001;
26 input->id.product = 0x0001;
27 input->id.version = 0x0100;
28 
29 ddata->input = input;
30 
31 for (i = 0; i < pdata->nbuttons; i++) {
32 struct gpio_keys_button *button = &pdata->buttons[i];
33 struct gpio_button_data *bdata = &ddata->data[i];
34 int irq;
35 unsigned int type = button->type ?: EV_KEY;
36 
37 bdata->input = input;
38 bdata->button = button;
39 setup_timer(&bdata->timer,
40 gpio_check_button, (unsigned long)bdata);
41 
42 ...
43 error = request_irq(irq, gpio_keys_isr,
44 IRQF_SAMPLE_RANDOM | IRQF_TRIGGER_RISING |
45 IRQF_TRIGGER_FALLING,
46 button->desc ? button->desc : "gpio_keys",
47 bdata);
48 if (error) {
49 ...
50 }
51 
52 if (button->wakeup)
53 wakeup = 1;
54 
**55 input_set_capability(input, type, button->code);**
56 }
57 
**58 error = input_register_device(input);**
59 if (error) {
60 pr_err("gpio-keys: Unable to register input device, "
61 "error: %d\n", error);
62 goto fail2;
63 }
64 
65 device_init_wakeup(&pdev->dev, wakeup);
66 
67 return 0;
68 ...
69 }
上述代码的第12行分配了1个输入设备，第20~27行初始化了该input_dev的一些属性，第58行注册了这个输入设备。第31~56行则申请了此GPIO按键设备需要的中断号，并初始化了timer。第55行设置此输入设备可告知的事情。
在注册输入设备后，底层输入设备驱动的核心工作只剩下在按键、触摸等人为动作发生的时候，报告事件。代码清单9列出了GPIO按键中断发生时的事件报告代码。
代码清单9 GPIO按键中断发生时的事件报告
1 static void gpio_keys_report_event(struct gpio_button_data *bdata)
2 {
3 struct gpio_keys_button *button = bdata->button;
4 struct input_dev *input = bdata->input;
5 unsigned int type = button->type ?: EV_KEY;
6 int state = (gpio_get_value(button->gpio) ? 1 : 0) ^ button->active_low;
7
**8 input_event(input, type, button->code, !!state);**
**9 input_sync(input);**
10 }
11 
12 static irqreturn_t gpio_keys_isr(int irq, void *dev_id)
13 {
14 struct gpio_button_data *bdata = dev_id;
15 struct gpio_keys_button *button = bdata->button;
16 
17 BUG_ON(irq != gpio_to_irq(button->gpio));
18 
19 if (button->debounce_interval)
20 mod_timer(&bdata->timer,
21 jiffies + msecs_to_jiffies(button->debounce_interval));
22 else
23 gpio_keys_report_event(bdata);
24 
25 return IRQ_HANDLED;
26 } 
第8行是报告键值，而第9行是1个同步事件，暗示前面报告的消息属于1个消息组。譬如用户在报告完X坐标后，又报告Y坐标，之后报告1个同步事件，应用程序即可知道前面报告的X、Y这2个事件属于1组，它会将2者联合起来形成1个（X,Y）的坐标。
代码清单8第2行获取platform_data，而platform_data实际上是定义GPIO按键硬件信息的数组，第31行的for循环工具这些信息申请GPIO并初始化中断，对于LDD6140电路板而言，这些信息如代码清单10。
代码清单10 LDD6410开发板GPIO按键的platform_data
1 static struct gpio_keys_button ldd6410_buttons[] = {
2 {
3 .gpio = S3C64XX_GPN(0),
4 .code = KEY_DOWN,
5 .desc = "Down",
6 .active_low = 1,
7 },
8 {
9 .gpio = S3C64XX_GPN(1),
10 .code = KEY_ENTER,
11 .desc = "Enter",
12 .active_low = 1,
13 .wakeup = 1,
14 },
15 {
16 .gpio = S3C64XX_GPN(2),
17 .code = KEY_HOME,
18 .desc = "Home",
19 .active_low = 1,
20 },
21 {
22 .gpio = S3C64XX_GPN(3),
23 .code = KEY_POWER,
24 .desc = "Power",
25 .active_low = 1,
26 .wakeup = 1,
27 },
28 {
29 .gpio = S3C64XX_GPN(4),
30 .code = KEY_TAB,
31 .desc = "Tab",
32 .active_low = 1,
33 },
34 {
35 .gpio = S3C64XX_GPN(5),
36 .code = KEY_MENU,
37 .desc = "Menu",
38 .active_low = 1,
39 },
40 };
41 
42 static struct gpio_keys_platform_data ldd6410_button_data = {
43 .buttons = ldd6410_buttons,
44 .nbuttons = ARRAY_SIZE(ldd6410_buttons),
45 };
46 
47 static struct platform_device ldd6410_device_button = {
48 .name = "gpio-keys",
49 .id = -1,
50 .dev = {
51 .platform_data = &ldd6410_button_data,
52 }
53 };
##### 1.3 RTC设备驱动
RTC（实时钟）借助电池供电，在系统掉电的情况下依然可以行走。它通常还具有产生周期中断以及产生闹钟（alarm）中断的能力，是一种典型的字符设备。作为一种字符设备驱动，RTC需要有file_operations中接口函数的实现，如open()、release()、read()、 poll()、ioctl()等，而典型的IOCTL包括RTC_SET_TIME、RTC_ALM_READ、RTC_ALM_SET、 RTC_IRQP_SET、RTC_IRQP_READ等，这些对于所有的RTC是通用的，只有底层的具体实现是设备相关的。
因此，drivers/rtc/rtc-dev.c实现了RTC驱动通用的字符设备驱动层，它实现了file_opearations的成员函数以及一些关于RTC的通用的控制代码，并向底层导出rtc_device_register()、rtc_device_unregister()用于注册和注销RTC；导出rtc_class_ops结构体用于描述底层的RTC硬件操作。这一RTC通用层实现的结果是，底层的RTC驱动不再需要关心RTC 作为字符设备驱动的具体实现，也无需关心一些通用的RTC控制逻辑，图3表明了这种关系。
![clip_image006](http://www.linuxdriver.cn/blog_images/Linux_532/clip_image006_thumb.jpg)
图3 Linux RTC设备驱动的分层
drivers/rtc/rtc-s3c.c实现了S3C6410的RTC驱动，其注册RTC以及绑定的rtc_class_ops的代码如代码清单11。
代码清单11 S3C6410 RTC驱动的rtc_class_ops实例与RTC注册
**1 static const struct rtc_class_ops s3c_rtcops = {**
2 .open = s3c_rtc_open,
3 .release = s3c_rtc_release,
4 .ioctl = s3c_rtc_ioctl,
5 .read_time = s3c_rtc_gettime,
6 .set_time = s3c_rtc_settime,
7 .read_alarm = s3c_rtc_getalarm,
8 .set_alarm = s3c_rtc_setalarm,
9 .irq_set_freq = s3c_rtc_setfreq,
10 .irq_set_state = s3c_rtc_setpie,
11 .proc = s3c_rtc_proc,
12 };
13 
14 static int s3c_rtc_probe(struct platform_device *pdev)
15 {
16 ...
**17 rtc = rtc_device_register("s3c", &pdev->dev, &s3c_rtcops,**
18 THIS_MODULE);
19 ...
20 }
[http://21cnbao.blog.51cto.com/109393/336263](http://21cnbao.blog.51cto.com/109393/336263)
