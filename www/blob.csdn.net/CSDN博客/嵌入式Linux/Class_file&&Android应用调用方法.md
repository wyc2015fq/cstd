
# Class_file&&Android应用调用方法 - 嵌入式Linux - CSDN博客

2015年12月28日 15:09:42[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：759



```python
java代码调用 fos.
```
```python
write
```
```python
这里面是
```
```python
byte
```
```python
类型的
```
```python
+
```
```python
private
```
```python
static
```
```python
final
```
```python
String LEDBLPATH=
```
```python
"/sys/class/hdyrodent_charger_led/brightness"
```
```python
;
+
```
```python
private
```
```python
final
```
```python
void
```
```python
SetChargerLedBrightness
```
```python
(String path,
```
```python
int
```
```python
brightness)
+    {
+
```
```python
try
```
```python
{
+                java.io.FileOutputStream fos =
```
```python
new
```
```python
+                java.io.FileOutputStream(
```
```python
new
```
```python
java.io.File(path));
+                fos.write(String.valueOf(brightness).getBytes());
+                fos.flush();
+                fos.close();
+                Slog.e(TAG,
```
```python
"--------->SetChargerLedBrightness"
```
```python
+LEDBLPATH+
```
```python
"brightness"
```
```python
+brightness);
+             }
```
```python
catch
```
```python
(Exception e){
+             e.printStackTrace();
+         }
+    }
+
```
```python
private
```
```python
static
```
```python
final
```
```python
String LEDFEPATH=
```
```python
"/sys/class/hdyrodent_charger_led/frequency"
```
```python
;
+
```
```python
private
```
```python
final
```
```python
void
```
```python
SetChargerLedfrequency
```
```python
(String path,
```
```python
int
```
```python
frequency)
+    {
+
```
```python
try
```
```python
{
+                java.io.FileOutputStream fos =
```
```python
new
```
```python
+                java.io.FileOutputStream(
```
```python
new
```
```python
java.io.File(path));
+                fos.write(String.valueOf(frequency).getBytes());
+                fos.flush();
+                fos.close();
+                Slog.e(TAG,
```
```python
"--------->SetChargerLedfrequency"
```
```python
+LEDFEPATH+
```
```python
"frequency"
```
```python
+frequency);
+             }
```
```python
catch
```
```python
(Exception e){
+             e.printStackTrace();
+         }
+    }
```
*驱动*
```python
/*
 * drivers/leds/leds-mt65xx.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 * Hydrodent weiqifa modify add
 *
 */
```
```python
#include <linux/module.h>
```
```python
#include <linux/platform_device.h>
```
```python
#include <linux/delay.h>
```
```python
#include <linux/string.h>
```
```python
#include <linux/ctype.h>
```
```python
#include <linux/leds.h>
```
```python
#include <linux/leds-mt65xx.h>
```
```python
#include <linux/workqueue.h>
```
```python
#include <linux/wakelock.h>
```
```python
#include <linux/slab.h>
```
```python
#include <linux/spinlock.h>
```
```python
//#include
```
```python
<
```
```python
cust_leds.h
```
```python
>
```
```python
//#include
```
```python
<
```
```python
cust_leds_def.h
```
```python
>
```
```python
#include <mach/mt_pwm.h>
```
```python
//#include
```
```python
<
```
```python
mach
```
```python
/
```
```python
mt_pwm_hal.h
```
```python
>
```
```python
//#include
```
```python
<
```
```python
mach
```
```python
/
```
```python
mt_gpio.h
```
```python
>
```
```python
#include <mach/pmic_mt6329_hw_bank1.h>
```
```python
#include <mach/pmic_mt6329_sw_bank1.h>
```
```python
#include <mach/pmic_mt6329_hw.h>
```
```python
#include <mach/pmic_mt6329_sw.h>
```
```python
#include <mach/upmu_common_sw.h>
```
```python
#include <mach/upmu_hw.h>
```
```python
//#include
```
```python
<
```
```python
mach
```
```python
/
```
```python
mt_pmic_feature_api.h
```
```python
>
```
```python
//#include
```
```python
<
```
```python
mach
```
```python
/
```
```python
mt_boot.h
```
```python
>
```
```python
#include <leds_hal.h>
```
```python
//#include
```
```python
<
```
```python
linux
```
```python
/
```
```python
leds_hal.h
```
```python
>
```
```python
#include "leds_drv.h"
```
```python
static struct class *hdyrodent
```
```python
_pwm_
```
```python
class=NULL;
int show_value=123;
/
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
 * scnprintf是linux下面一个函数，这个函数把后面 的值格式话转化成字符串
 * 并在adb 下面显示出来
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
/
static ssize
```
```python
_t hdyrodent_
```
```python
pwm
```
```python
_show(struct device *dev,struct device_
```
```python
attribute
```
```python
*attr, char *
```
```python
buf)
{
```
```python
printk("%s\n", __FUNCTION__);
```
```python
return scnprintf(buf, PAGE_SIZE, "%d\n", show_value);
```
```python
}
/
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
 * echo　pwm这个节点的时候就会调用下面这个函数，echo "12" > pwm 
 * 那么value的值就是12 可以通过这样设置pwm的数值
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
/
static ssize
```
```python
_t hdyrodent_
```
```python
pwm
```
```python
_store(struct class *cls, struct class_
```
```python
attribute
```
```python
*attr, const  char *
```
```python
_buf, size_
```
```python
t _count)
{
```
```python
int value=0;
```
```python
sscanf(_buf, "%d", &value);
```
```python
sscanf(_buf, "%d", &show_value);//把值传给show_value这样　cat的值就是echo 进去的值了
```
```python
printk("%s: value: %d _count:%d\n", __FUNCTION__, value,_count);
```
```python
return _count;
```
```python
}

/
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
 * __ATTR的第一个参数是在sys文件系统里面显示的名字
　* ０６６６是这个节点的属性，0666表示是可读可写
　* hdyrodent
```
```python
_pwm_
```
```python
show 是cat 这个文件的时候调用的函数
　* hdyrodent
```
```python
_pwm_
```
```python
store 是echo的时候调用的函数
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
/
static struct class
```
```python
_attribute hdyrodent_
```
```python
attr[] = {
```
```python
__ATTR(pwm,0666, hdyrodent_
```
```python
pwm
```
```python
_show, hdyrodent_
```
```python
pwm_store),
```
```python
__ATTR_
```
```python
NULL,
};

static int
```
```python
__init hdyrodent_
```
```python
pwm_init(void)
{
  int ret;
  int i = 0;
  printk("%s start\n",
```
```python
__FUNCTION__
```
```python
);
```
```python
//用class_create在sys/class/下面生成sys文件系统
```
```python
hdyrodent
```
```python
_pwm_
```
```python
class=class
```
```python
_create(THIS_
```
```python
MODULE,"hdyrodent");
  if(IS
```
```python
_ERR(hdyrodent_
```
```python
pwm_class))
  {
```
```python
printk("create hdyrodent module fail \n");
```
```python
return PTR_ERR(hdyrodent_pwm_class);;
```
```python
}
  for (i = 0 ; NULL != attr
```
```python
_name(hdyrodent_
```
```python
attr[i]);i++)
  {
```
```python
ret = class_create_file(hdyrodent_pwm_class, &hdyrodent_attr[i]);
```
```python
if (0 != ret)
```
```python
{
```
```python
printk("creat %s class file fail\n",attr_name(hdyrodent_attr[i]));
```
```python
break;
```
```python
}
```
```python
}
  printk("%s end\n",
```
```python
__FUNCTION__
```
```python
);
  return 0;
}
static void
```
```python
__exit hdyrodent_
```
```python
pwm_exit(void)
{
```
```python
int i = 0;
```
```python
printk("hdyrodent module cleanup start.\n");
```
```python
for (i = 0 ; NULL != attr_name(hdyrodent_attr[i]);i++)
```
```python
{
```
```python
class_remove_file(hdyrodent_pwm_class, &hdyrodent_attr[i]);
```
```python
}
```
```python
class_destroy(hdyrodent_pwm_class);
```
```python
printk("hdyrodent module cleanup OK!\n");
```
```python
}
MODULE_AUTHOR("329410527@qq.com");
MODULE_DESCRIPTION("HDYRODENT PWM MODULE");
MODULE_LICENSE("GPL");
MODULE_VERSION("ver0.1");
module
```
```python
_init(hdyrodent_
```
```python
pwm_init);
module
```
```python
_exit(hdyrodent_
```
```python
pwm_exit);
```

