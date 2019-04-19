# Linux下regulator驱动程序分析 - xqhrs232的专栏 - CSDN博客
2011年08月27日 22:12:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4498
原文地址::[http://blog.csdn.net/lishuiwang/article/details/6130299](http://blog.csdn.net/lishuiwang/article/details/6130299)
//
/*regulator 是驱动中电源管理的基础设施。要先注册到内核中，然后使用这些电压输出的模块get其regulator,在驱动中的init里，在适当时间中进行电压电流的设置.
与 gpio 差不多？　一样是基础设施？
*/
//
Linux 内核的动态电压和电流控制接口
功耗已经成为电子产品设计的首要考虑。
//
"LDO是 low dropout regulator，意为低压差线性稳压器"，是相对于传统的线性稳压器来说的。传统的线性稳压器，如78xx系列的芯片都要求输入电压要比输出电压高出 2v~3V以上，否则就不能正常工作。但是在一些情况下，这样的条件显然是太苛刻了，如5v转3.3v,输入与输出的压差只有1.7v，显然是不满足条件的。针对这种情况，才有了LDO类的电源转换芯片。生产LDO芯片的公司很多，常见的有ALPHA, Linear(LT), Micrel, National semiconductor,TI等
in:twl4030-poweroff.c
pm_power_off: what it for???
一种称为校准器（regulator）的动态电压和电流控制的方法，很有参考意义和实际使用价值。
//***********************************************************************//
1: 校准器的基本概念
所谓校准器实际是在软件控制下把输入的电源调节精心输出。
2:Consumer 的API
regulator = regulator_get(dev, “Vcc”)；
其中，dev 是设备“Vcc”一个字符串代表，校准器（regulator）然后返回一个指针，也是regulator_put(regulator)使用的。
打开和关闭校准器（regulator）API如下。
int regulator_enable(regulator);
int regulator_disable(regulator);
3: 电压的API
消费者可以申请提供给它们的电压，如下所示。
int regulator_set_voltage(regulator, int min_uV, int max_uV);
在改变电压前要检查约束，如下所示。
regulator_set_voltage(regulator,100000,150000)
电压值下面的设置改变如下所示。
int regulator_get_voltage)struct regulator *regulator);
4:校准器的驱动和系统配置
在实际使用校准器之前，需要按照下面的结构写校准器的驱动程序，然后注册后通知给消费者使用。

//************************* linux regulator 模型******************************************//
static LIST_HEAD(regulator_list);     //整个regulator模型的所有regulator.
static LIST_HEAD(regulator_map_list); //整个regulator模型的map
struct regulator_consumer_supply : 用户支持的接口映射。  supply -> device
struct regulator_state： 用于表示 在整个系统的低功耗状态下的设备状态。
struct regulation_constraints ： 操作约束。
struct regulator_init_data  校准器平台初始化数据。
struct regulator_dev
向内核注册后 得到regulator设备结构
注册：
struct regulator_dev *regulator_register(struct regulator_desc *regulator_desc,struct device *dev, void *driver_data)
反注册：
要使用注册得到 regulator_dev:
void regulator_unregister(struct regulator_dev *rdev)

/* Regulator operating modes.**/
#define REGULATOR_MODE_FAST0x1 //电压可以快速调
#define REGULATOR_MODE_NORMAL0x2 //一般模式
#define REGULATOR_MODE_IDLE0x4 //低load
#define REGULATOR_MODE_STANDBY0x8 //sleep/ standby 状态下 低功耗
例子：
/* VSIM for OMAP VDD_MMC1A (i/o for DAT4..DAT7) */
static struct regulator_init_data zoom2_vsim = { //这个data被置于 对应的 platform_device 的data字段中。
.constraints = {
.min_uV
= 1800000,
.max_uV
= 3000000,
.valid_modes_mask= REGULATOR_MODE_NORMAL  /*这里设置*/
| REGULATOR_MODE_STANDBY,
.valid_ops_mask= REGULATOR_CHANGE_VOLTAGE /*这里设置这个regulator上可进行的操作。调电压，模式可变，可以打开关闭*/
| REGULATOR_CHANGE_MODE
| REGULATOR_CHANGE_STATUS,
},
.num_consumer_supplies  = 1,
.consumer_supplies      = &zoom2_vsim_supply,
};
// regulator 的初始化数据要放到  device 's  platform_data
struct regulator_init_data *init_data = dev->platform_data;

" 重点"
//*************************** twl4030 regulator 的实现  ****************************//
struct twlreg_info ： 关联到驱动与设备的数据。驱动的私有数据，driver_data.
struct twlreg_info {
/* start of regulator's PM_RECEIVER control register bank */
u8
base;
/* twl4030 resource ID, for resource control state machine */
u8
id;
/* FIXED_LDO voltage */
u8
deciV;
/* voltage in mV = table[VSEL]; table_len must be a power-of-two */
u8
table_len;
const u16*table;
/* used by regulator core */
struct regulator_descdesc;
};
//一个驱动 (platform_driver)对应所有的regulator, 每个regulator在内核中表示为一个platform_device. 所以调用了许多次的probe
//在probe中，从全局数组中取出自定义的数据结构：　twlreg_info, 然后从platform_device中取出初始化数据（device's platform_data）,然后就可以向内核注册一个 regulator(会返回一个regulator_dev:类)。
"struct regulator_dev *regulator_register(struct regulator_desc *regulator_desc,struct device *dev, void *driver_data)"
1: 
1: 每个regulator 的  "twlreg_info" 被 存于 regulator_dev 的 device 's driver_data, twlreg_info中的 table 存支持的电压值。
2:两种操作结构"regulator_ops"：twl4030fixed_ops(电压固定),  twl4030ldo_ops（电压可调）
3：传入每个 ops中的操作函数 的参数是:  regulator_dev.
4: 在  twl4030-core.c中，在添加 twl4030的 i2c_client时，添加了所有的　regulator 的　platform_device.
5: 在驱动初始化注册中，内核会自动去匹配 bus上的所有符合的 platform_device（代表regulator，init_data 放于device's platform_data）, 所以一个驱动就可以匹配到 bus上所有对应的　regulator.
6: 每匹配一个 regulator的　platform_device时，就会从中，platform_device的　device 中的 platform_data　中得到　regulator的初始化数据：　regulator_init_data
7: 取出初始化数据后，　设置一下. "设置，即是当设备支持一定的功能后，由驱动来控制时，要匹配驱动能做到的功能。that is 两者　与（＆）　一下"
　　然后，从regulator模块的全局数组"twl4030_regs[]" 中取出　twlreg_info 
if (twl4030_regs[i].desc.id != pdev->id)
continue;
info = twl4030_regs + i;
就可以向内核注册这个 regulator了：
"rdev = regulator_register(&info->desc, &pdev->dev, info);"
"注册后返回一个　regulator_dev": 然后把这个rdev输入到　platform_device 's device 's driver_data... 主要用于 remove中？？？
regulator_dev：是一个类，其父是 本regulator的platform_device 's device.
8: // regulator_ops 存入了 regulator_desc, 然后注册时，desc 关联到了 regulator_dev-> desc = desc.
生成 regualator_dev, 设置数据，注册sysfs，然后：　constraints, attribute, supply　,comsumer device.
struct regulator_dev *regulator_register(struct regulator_desc *regulator_desc,struct device *dev, void *driver_data)
9:
另：
大部分的regulator的platform_device生成如下：
每个regulator设备注册到内核的时机是在　twl4030-core.c　中。
把regulator的初始化数据，放到新生成的platform_device 　's device 's platform_data...
然后注册 platform_device　到内核中。
usb部分的 regulator生成如下：(这里明显是由于ＯＭＡＰ不一定是有ＵＳＢ？？，然后在其ｂｏａｒｄ-ｚｏｏｍ２.ｃ中，"没有设置每个初始化数据"（包括：ｓｕｐｐｌｙ,ｃｏｎｓｔｒａｉｎｔｓ(ｍｉｎ,ｍａｘ,ｍａｓｋ)）)
而对于usb模块的 regulator, 先生成对应的platform_device, 返回了platform_device->device, 然后利用这个device去形成多个：　regulator_consumer_supply.dev = device.
然后用这些 regulator_consumer_supply, 来做comsumer并生成　platform_device.注册
//**************************　使用regulator　***************************//
regulator已经向内核模型注册了，但如何使用呢，怎样调节电压？？
//注册后，内核中就已经有对应的regulator对应的 supply consumer. 然后在驱动中任何地方调用接口即可：
//regulator_get, regulator_enable, regulator_disable, regulator_set_voltage, regulator_get_voltage......
1: 在twl4030-usb模块中，会用到　usb对应的　reuglator,,   fixed_ops
twl->usb3v1 = regulator_get(twl->dev, "usb3v1");
regulator_enable(twl->usb3v1);
regulator_disable(twl->usb1v5);
2:
regulator = regulator_get(dev, “Vcc”)；
其中，dev 是设备“Vcc”一个字符串代表，校准器（regulator）然后返回一个指针，也是regulator_put(regulator)使用的。
打开和关闭校准器（regulator）API如下。
int regulator_enable(regulator);
int regulator_disable(regulator);
 电压的API
消费者可以申请提供给它们的电压，如下所示。
int regulator_set_voltage(regulator, int min_uV, int max_uV);
在改变电压前要检查约束，如下所示。
regulator_set_voltage(regulator,100000,150000)
//************************************************//

/**
 * struct regulator_desc - Regulator descriptor
 *
 * Each regulator registered with the core is described with a structure of
 * this type.
 *
 * @name: Identifying name for the regulator.
 * @id: Numerical identifier for the regulator.
 * @n_voltages: Number of selectors available for ops.list_voltage().
 * @ops: Regulator operations table.
 * @irq: Interrupt number for the regulator.
 * @type: Indicates if the regulator is a voltage or current regulator.
 * @owner: Module providing the regulator, used for refcounting.
 */
struct regulator_desc {
const char *name;
int id;
unsigned n_voltages;
struct regulator_ops *ops;
int irq;
enum regulator_type type;
struct module *owner;
};
enum regulator_status {
REGULATOR_STATUS_OFF,
REGULATOR_STATUS_ON,
REGULATOR_STATUS_ERROR,
/* fast/normal/idle/standby are flavors of "on" */
REGULATOR_STATUS_FAST,
REGULATOR_STATUS_NORMAL,
REGULATOR_STATUS_IDLE,
REGULATOR_STATUS_STANDBY,
};
/*
 * struct regulator_dev
 *
 * Voltage / Current regulator class device. One for each regulator.
 */
struct regulator_dev {
struct regulator_desc *desc;
int use_count;
/* lists we belong to */
struct list_head list; /* list of all regulators */
struct list_head slist; /* list of supplied regulators */
/* lists we own */
struct list_head consumer_list; /* consumers we supply */
struct list_head supply_list; /* regulators we supply */
struct blocking_notifier_head notifier;
struct mutex mutex; /* consumer lock */
struct module *owner;
struct device dev;
struct regulation_constraints *constraints;
struct regulator_dev *supply;/* for tree */
void *reg_data;/* regulator_dev data */
};
/*
 * struct regulator
 *
 * One for each consumer device.
 */
struct regulator {
struct device *dev;
struct list_head list;
int uA_load;
int min_uV;
int max_uV;
int enabled; /* count of client enables */
char *supply_name;
struct device_attribute dev_attr;
struct regulator_dev *rdev;
};
/*
 * struct regulator_map
 *
 * Used to provide symbolic supply names to devices.
 */
struct regulator_map {
struct list_head list;
struct device *dev;
const char *supply;
struct regulator_dev *regulator;
};
/**
 * struct regulator_state - regulator state during low power syatem states
 *
 * This describes a regulators state during a system wide low power state.
 *
 * @uV: Operating voltage during suspend.
 * @mode: Operating mode during suspend.
 * @enabled: Enabled during suspend.
 */
struct regulator_state {
int uV;
/* suspend voltage */
unsigned int mode; /* suspend regulator operating mode */
int enabled; /* is regulator enabled in this suspend state */
};
/**
 * struct regulation_constraints - regulator operating constraints.
 *
 * This struct describes regulator and board/machine specific constraints.
 *
 * @name: Descriptive name for the constraints, used for display purposes.
 *
 * @min_uV: Smallest voltage consumers may set.
 * @max_uV: Largest voltage consumers may set.
 *
 * @min_uA: Smallest consumers consumers may set.
 * @max_uA: Largest current consumers may set.
 *
 * @valid_modes_mask: Mask of modes which may be configured by consumers.
 * @valid_ops_mask: Operations which may be performed by consumers.
 *
 * @always_on: Set if the regulator should never be disabled.
 * @boot_on: Set if the regulator is enabled when the system is initially
 *           started.
 * @apply_uV: Apply the voltage constraint when initialising.
 *
 * @input_uV: Input voltage for regulator when supplied by another regulator.
 *
 * @state_disk: State for regulator when system is suspended in disk mode.
 * @state_mem: State for regulator when system is suspended in mem mode.
 * @state_standby: State for regulator when system is suspended in standby
 *                 mode.
 * @initial_state: Suspend state to set by default.
 */
struct regulation_constraints {
char *name;
/* voltage output range (inclusive) - for voltage control */
int min_uV;
int max_uV;
/* current output range (inclusive) - for current control */
int min_uA;
int max_uA;
/* valid regulator operating modes for this machine */
unsigned int valid_modes_mask;
/* valid operations for regulator on this machine */
unsigned int valid_ops_mask;
/* regulator input voltage - only if supply is another regulator */
int input_uV;
/* regulator suspend states for global PMIC STANDBY/HIBERNATE */
struct regulator_state state_disk;
struct regulator_state state_mem;
struct regulator_state state_standby;
suspend_state_t initial_state; /* suspend state to set at init */
/* constriant flags */
unsigned always_on:1;/* regulator never off when system is on */
unsigned boot_on:1;/* bootloader/firmware enabled regulator */
unsigned apply_uV:1;/* apply uV constraint iff min == max */
};
/**
 * struct regulator_consumer_supply - supply -> device mapping
 *
 * This maps a supply name to a device.
 *
 * @dev: Device structure for the consumer.
 * @supply: Name for the supply.
 */
struct regulator_consumer_supply {
struct device *dev;/* consumer */
const char *supply;/* consumer supply - e.g. "vcc" */
};
/**
 * struct regulator_desc - Regulator descriptor
 *
 * Each regulator registered with the core is described with a structure of
 * this type.
 *
 * @name: Identifying name for the regulator.
 * @id: Numerical identifier for the regulator.
 * @n_voltages: Number of selectors available for ops.list_voltage().
 * @ops: Regulator operations table.
 * @irq: Interrupt number for the regulator.
 * @type: Indicates if the regulator is a voltage or current regulator.
 * @owner: Module providing the regulator, used for refcounting.
 */
struct regulator_desc {
const char *name;
int id;
unsigned n_voltages;
struct regulator_ops *ops;
int irq;
enum regulator_type type;
struct module *owner;
};
/**
 * struct regulator_init_data - regulator platform initialisation data.
 *
 * Initialisation constraints, our supply and consumers supplies.
 *
 * @supply_regulator_dev: Parent regulator (if any).
 *
 * @constraints: Constraints.  These must be specified for the regulator to
 *               be usable.
 * @num_consumer_supplies: Number of consumer device supplies.
 * @consumer_supplies: Consumer device supply configuration.
 *
 * @regulator_init: Callback invoked when the regulator has been registered.
 * @driver_data: Data passed to regulator_init.
 */
struct regulator_init_data {
struct device *supply_regulator_dev; /* or NULL for LINE */
struct regulation_constraints constraints;
int num_consumer_supplies;
struct regulator_consumer_supply *consumer_supplies;
/* optional regulator machine specific init */
int (*regulator_init)(void *driver_data);
void *driver_data;/* core does not touch this */
};
/*
 * Regulator operation constraint flags. These flags are used to enable
 * certain regulator operations and can be OR'ed together.
 *
 * VOLTAGE:  Regulator output voltage can be changed by software on this
 *           board/machine.
 * CURRENT:  Regulator output current can be changed by software on this
 *           board/machine.
 * MODE:     Regulator operating mode can be changed by software on this
 *           board/machine.
 * STATUS:   Regulator can be enabled and disabled.
 * DRMS:     Dynamic Regulator Mode Switching is enabled for this regulator.
 */
#define REGULATOR_CHANGE_VOLTAGE0x1
#define REGULATOR_CHANGE_CURRENT0x2
#define REGULATOR_CHANGE_MODE0x4
#define REGULATOR_CHANGE_STATUS0x8
#define REGULATOR_CHANGE_DRMS0x10
/*
 * Regulator operating modes.
 *
 * Regulators can run in a variety of different operating modes depending on
 * output load. This allows further system power savings by selecting the
 * best (and most efficient) regulator mode for a desired load.
 *
 * Most drivers will only care about NORMAL. The modes below are generic and
 * will probably not match the naming convention of your regulator data sheet
 * but should match the use cases in the datasheet.
 *
 * In order of power efficiency (least efficient at top).
 *
 *  Mode       Description
 *  FAST       Regulator can handle fast changes in it's load.
 *             e.g. useful in CPU voltage & frequency scaling where
 *             load can quickly increase with CPU frequency increases.
 *
 *  NORMAL     Normal regulator power supply mode. Most drivers will
 *             use this mode.
 *
 *  IDLE       Regulator runs in a more efficient mode for light
 *             loads. Can be used for devices that have a low power
 *             requirement during periods of inactivity. This mode
 *             may be more noisy than NORMAL and may not be able
 *             to handle fast load switching.
 *
 *  STANDBY    Regulator runs in the most efficient mode for very
 *             light loads. Can be used by devices when they are
 *             in a sleep/standby state. This mode is likely to be
 *             the most noisy and may not be able to handle fast load
 *             switching.
 *
 * NOTE: Most regulators will only support a subset of these modes. Some
 * will only just support NORMAL.
 *
 * These modes can be OR'ed together to make up a mask of valid register modes.
 */
#define REGULATOR_MODE_FAST0x1
#define REGULATOR_MODE_NORMAL0x2
#define REGULATOR_MODE_IDLE0x4
#define REGULATOR_MODE_STANDBY0x8
