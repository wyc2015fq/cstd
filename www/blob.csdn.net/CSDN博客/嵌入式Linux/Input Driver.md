
# Input Driver - 嵌入式Linux - CSDN博客

2015年12月23日 15:41:22[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1221



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
#include <linux/delay.h>
```
```python
#include <linux/device.h>
```
```python
#include <linux/input.h>
```
```python
#include <mach/upmu_hw.h>
```
```python
#include <mach/mt_boot.h>
```
```python
#include <mach/mt_gpio.h>
```
```python
#include <mach/eint.h>
```
```python
#include <cust_eint.h>
```
```python
#include "cust_gpio_usage.h"
```
```python
extern
```
```python
void
```
```python
mt_eint_unmask(
```
```python
unsigned
```
```python
int
```
```python
eint_num);
```
```python
extern
```
```python
void
```
```python
mt_eint_set_polarity(
```
```python
unsigned
```
```python
int
```
```python
eint_num,
```
```python
unsigned
```
```python
int
```
```python
pol);
```
```python
extern
```
```python
void
```
```python
mt_eint_set_hw_debounce(
```
```python
unsigned
```
```python
int
```
```python
eintno,
```
```python
unsigned
```
```python
int
```
```python
ms);
```
```python
extern
```
```python
unsigned
```
```python
int
```
```python
mt_eint_set_sens(
```
```python
unsigned
```
```python
int
```
```python
eintno,
```
```python
unsigned
```
```python
int
```
```python
sens);
```
```python
extern
```
```python
void
```
```python
mt_eint_registration(
```
```python
unsigned
```
```python
int
```
```python
eint_num,
```
```python
unsigned
```
```python
int
```
```python
flag,
```
```python
void
```
```python
(EINT_FUNC_PTR) (
```
```python
void
```
```python
),
```
```python
unsigned
```
```python
int
```
```python
is_auto_umask);
```
```python
struct
```
```python
input_dev *hdy_input_dev;
```
```python
//中断服务函数
```
```python
void
```
```python
hal_eint_interrupt_handler(
```
```python
void
```
```python
)
{
    printk(
```
```python
"===%s=== mhall_pin value=%d\n"
```
```python
,__FUNCTION__,mt_get_gpio_in(GPIO_MHALL_EINT_PIN));
```
```python
/*设置中断触发方式，打开摄像头后产生了一次中断，然后改变中断触发方式，关闭摄像头后又会产生一次关闭摄像头的中断*/
```
```python
if
```
```python
(mt_get_gpio_in(GPIO_MHALL_EINT_PIN))
    {
        mt_eint_set_polarity(CUST_EINT_MHALL_NUM, MT_EINT_POL_NEG);
```
```python
//中断触发方式设置成下降沿
```
```python
//mt_eint_unmask(CUST_EINT_MHALL_NUM);
```
```python
if
```
```python
(test_bit(KEY_N, hdy_input_dev->keybit)){
            input_report_key(hdy_input_dev, KEY_N,
```
```python
0
```
```python
);
            input_sync(hdy_input_dev);
            printk(
```
```python
"===%s================ report key_n 0\n"
```
```python
,__FUNCTION__);
        }
```
```python
else
```
```python
{
            printk(
```
```python
"[AUTOTEST] Not Support KEY_N KEY!!\n"
```
```python
);
        }
    }
```
```python
else
```
```python
{
        mt_eint_set_polarity(CUST_EINT_MHALL_NUM, MT_EINT_POL_POS);
```
```python
//中断触发方式设置成上升沿
```
```python
if
```
```python
(test_bit(KEY_N, hdy_input_dev->keybit)){
            input_report_key(hdy_input_dev, KEY_N,
```
```python
1
```
```python
);
            input_sync(hdy_input_dev);
            printk(
```
```python
"===%s================ report key_n 1\n"
```
```python
,__FUNCTION__);
        }
```
```python
else
```
```python
{
            printk(
```
```python
"[AUTOTEST] Not Support KEY_N KEY!!\n"
```
```python
);
        }
    }
    mt_eint_unmask(CUST_EINT_MHALL_NUM);    
}
```
```python
static
```
```python
int
```
```python
__init hdyrodent_hall_init(
```
```python
void
```
```python
)
{
```
```python
int
```
```python
err = -
```
```python
1
```
```python
;
```
```python
int
```
```python
r=
```
```python
0
```
```python
;
    printk(
```
```python
"%s mhall_pin value=%d start\n"
```
```python
,__FUNCTION__,mt_get_gpio_in(GPIO_MHALL_EINT_PIN));
```
```python
/* initialize and register input device (/dev/input/eventX) */
```
```python
hdy_input_dev = input_allocate_device();
```
```python
if
```
```python
(!hdy_input_dev)
```
```python
return
```
```python
-ENOMEM;
    hdy_input_dev->name =
```
```python
"hdyrodent-hall"
```
```python
;
```
```python
//下面这两句很关键，如果设置不正确，很容易造成按键上报不成功
```
```python
hdy_input_dev->evbit[
```
```python
0
```
```python
] = BIT_MASK(EV_KEY);
```
```python
/*设置按键信息*/
```
```python
hdy_input_dev->keybit[BIT_WORD(KEY_N)] = BIT_MASK(KEY_N);  
    r = input_register_device(hdy_input_dev);
```
```python
if
```
```python
(r) {
        printk(
```
```python
"Error:register input device failed (%d)\n"
```
```python
, r);
        input_free_device(hdy_input_dev);
```
```python
return
```
```python
r;
    }
```
```python
//Init the irq gpio1_interrupt
```
```python
mt_set_gpio_mode(GPIO_MHALL_EINT_PIN, GPIO_MHALL_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_MHALL_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_MHALL_EINT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_MHALL_EINT_PIN, GPIO_PULL_UP);
    msleep(
```
```python
50
```
```python
);
```
```python
/*
    mt_eint_set_hw_debounce
    设置抖动
    mt_eint_registration
    第一个是中断号，触发极性，第二个是设定是否开启抖动，第三个是绑定中断函数，第四个关闭中断
    mt_eint_unmask
    屏蔽中断    
    */
```
```python
mt_eint_set_hw_debounce(CUST_EINT_MHALL_NUM, CUST_EINT_MHALL_DEBOUNCE_CN);
    mt_eint_registration(CUST_EINT_MHALL_NUM, CUST_EINT_MHALL_TYPE, hal_eint_interrupt_handler,
```
```python
0
```
```python
);
    mt_eint_unmask(CUST_EINT_MHALL_NUM);
    printk(
```
```python
"%s end\n"
```
```python
, __FUNCTION__);
```
```python
return
```
```python
0
```
```python
;
}
```
```python
static
```
```python
void
```
```python
__exit hdyrodent_hall_exit(
```
```python
void
```
```python
)
{
    input_free_device(hdy_input_dev);
    input_unregister_device(hdy_input_dev);
    printk(
```
```python
"hdyrodent module cleanup OK!\n"
```
```python
);
}
MODULE_AUTHOR(
```
```python
"329410527@qq.com"
```
```python
);
MODULE_DESCRIPTION(
```
```python
"HDYRODENT HALL MODULE"
```
```python
);
MODULE_LICENSE(
```
```python
"GPL"
```
```python
);
MODULE_VERSION(
```
```python
"ver0.1"
```
```python
);
module_init(hdyrodent_hall_init);
module_exit(hdyrodent_hall_exit);
```

