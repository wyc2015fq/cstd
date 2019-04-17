# Kernel 中的 GPIO 定义和控制 - DoubleLi - 博客园






最近要深一步用到GPIO口控制，写个博客记录下Kernel层的GPIO学习过程！



**一、概念**

     General Purpose Input Output （通用输入/输出）简称为GPIO，或 总线扩展器。也就是芯片的引脚，当微控制器或芯片组没有足够的I/O端口，或当系统需要采用远端串行通信或控制时，GPIO产品能够提供额外的控制和监视功能。通常在ARM里，所有I/O都是通用的，

每个GPIO端口包含8个管脚，如PA端口是PA0～PA7,而且GPIO口至少有两个寄存器，即"通用IO控制寄存器"与"通用IO数据寄存器"。数据寄存器的各位都直接引到芯片外部，而对这种寄存器中每一位的作用，即每一位的信号流通方向，则可以通过控制寄存器中对应位独立地加以设置。比如，可以设置某个引脚的属性为输入、输出或其他特殊功能。

常用的应该是：高阻输入、推挽输出、开漏输出。

通俗理解为 ：

高阻输入—— 保持高阻抗状态，彻底断开输出，避免干扰，对总线状态不起作用，此时总线可由其他器件占用。

推挽输出——可以输出高,低电平,连接数字器件。

开漏输出——输出端相当于三极管的集电极. 要得到高电平状态需要上拉电阻才行。

软件上就是通过设置IO口的模式，然后控制IO的上拉下拉，写入对应寄存器，通过寄存器控制电路：

上拉寄存器是控制对应端口上拉使能(DE)的。当对应位为0时，设置对应引脚上拉使能，为1时，禁止对应引脚上拉使能。如果上拉寄存器使能，无论引脚功能寄存器如何设置（输入，输出，数据，中断等），对应引脚输出高电平。

**上拉是一个电阻接到一个电压，其实就是增强IO的驱动能力，IO口是高电平。下拉是一个电阻接到地，保证IO口是低电平。**





**二、kernel层调用接口实现**

GPIO操作，在Kernel 2.6.32版本以上提供了gpio口管理的库文件**/kernel/drivers/gpio/gpiolib.c**，里面就是我们需要的接口函数实现！

几个主要的方法：

申请一个pin脚作为gpio口，命名为 * label,如果经过判断空闲的 申请成功了做一些初始的bit位设置。
int gpio_request(unsigned gpio, const char *label)
{
struct gpio_desc	*desc;
struct gpio_chip	*chip;
int	 status = -EINVAL;
unsigned long	 flags;

spin_lock_irqsave(&gpio_lock, flags);
...
if (test_and_set_bit(FLAG_REQUESTED, &desc->flags) == 0)
...
}
释放这个gpio口，还原bit。


void gpio_free(unsigned gpio)
{
	unsigned long		flags;
	struct gpio_desc	*desc;
	struct gpio_chip	*chip;


	might_sleep();
...
clear_bit(FLAG_ACTIVE_LOW, &desc->flags);
clear_bit(FLAG_REQUESTED, &desc->flags);
clear_bit(FLAG_OPEN_DRAIN, &desc->flags);
clear_bit(FLAG_OPEN_SOURCE, &desc->flags);
...
}




设置gpio口为输入模式：
int gpio_direction_input(unsigned gpio)
{
	unsigned long		flags;
	struct gpio_chip	*chip;
	struct gpio_desc	*desc = &gpio_desc[gpio];
	int			status = -EINVAL;


	spin_lock_irqsave(&gpio_lock, flags);
	if (!gpio_is_valid(gpio))
		goto fail;
	chip = desc->chip;
...
	status = chip->direction_input(chip, gpio);
...
}
其中的**chip->direction_input(chip, gpio)为实现！**



设置gpio口为输出模式 value为初始值 0为高电平/1为低电平：


int gpio_direction_output(unsigned gpio, int value)
{
	unsigned long		flags;
	struct gpio_chip	*chip;
	struct gpio_desc	*desc = &gpio_desc[gpio];
	int			status = -EINVAL;


	/* Open drain pin should not be driven to 1 */
	if (value && test_bit(FLAG_OPEN_DRAIN,  &desc->flags))
		return gpio_direction_input(gpio);
...
	status = chip->direction_output(chip, gpio, value);
...
}


其中的**chip->direction_output(chip, gpio, value)为实现！**





设置gpio口的值：


void __gpio_set_value(unsigned gpio, int value)
{
	struct gpio_chip	*chip;

	chip = gpio_to_chip(gpio);
	WARN_ON(chip->can_sleep);
	trace_gpio_value(gpio, 0, value);
	if (test_bit(FLAG_OPEN_DRAIN,  &gpio_desc[gpio].flags))
		_gpio_set_open_drain_value(gpio, chip, value);
	else if (test_bit(FLAG_OPEN_SOURCE,  &gpio_desc[gpio].flags))
		_gpio_set_open_source_value(gpio, chip, value);
	else
		chip->set(chip, gpio - chip->base, value);
}
获取gpio口的值：


int __gpio_get_value(unsigned gpio)
{
	struct gpio_chip	*chip;
	int value;

	chip = gpio_to_chip(gpio);
	WARN_ON(chip->can_sleep);
	value = chip->get ? chip->get(chip, gpio - chip->base) : 0;
	trace_gpio_value(gpio, 1, value);
	return value;
}


 对于有些挂载在I2C，SPI总线上的扩展GPIO，读写操作可能会导致睡眠，因此不能在中断函数中
  使用。使用下面的函数以区别于正常的GPIO


  int gpio_get_value_cansleep(unsigned gpio);//读GPIO
  void gpio_set_value_cansleep(unsigned gpio, int value);//写GPIO




**三、gpiolib.c关联芯片接口**



**以上为gpiolib.c的基本方法都是向下调用到对应芯片的gpio实现！**

**所以每个方法里面的实现都是通过 struct gpio_chip*chip 这个指针调用结构体中关联的相关接口！**



下面是gplolib怎么关联到特定的芯片（mach-at91）的几个主要方法：

mach-at91的/kernel/arch/arm/mach-at91/gpio.c中的


static struct at91_gpio_chip gpio_chip[] = {
	AT91_GPIO_CHIP("A", 0x00 + PIN_BASE, 32),
	AT91_GPIO_CHIP("B", 0x20 + PIN_BASE, 32),
	AT91_GPIO_CHIP("C", 0x40 + PIN_BASE, 32),
	AT91_GPIO_CHIP("D", 0x60 + PIN_BASE, 32),
	AT91_GPIO_CHIP("E", 0x80 + PIN_BASE, 32),
};



gpiolib.c中的接口与mach-at91函数接口对应关系如下：


#define AT91_GPIO_CHIP(name, base_gpio, nr_gpio)			\
	{								\
		.chip = {						\
			.label		  = name,			\
			.direction_input  = at91_gpiolib_direction_input, \
			.direction_output = at91_gpiolib_direction_output, \
			.get		  = at91_gpiolib_get,		\
			.set		  = at91_gpiolib_set,		\
			.dbg_show	  = at91_gpiolib_dbg_show,	\
			.base		  = base_gpio,			\
			.ngpio		  = nr_gpio,			\
		},							\
	}
从上面可以看到 在gpiolib.c中的方法调用芯片（mach-at91）的映射关系：

比如：**gpio_direction_output() 设置gpio口为输出模式中最终的实现调用chip->direction_output(chip, gpio, value)。**

**从上面可以看出来，也就是调用mach-at91的at91_gpiolib_direction_output()!**

其它的类似。





gpiolib.c中的


int gpiochip_add(struct gpio_chip *chip)
{
	unsigned long	flags;
	int		status = 0;
	unsigned	id;
	int		base = chip->base;


	if ((!gpio_is_valid(base) || !gpio_is_valid(base + chip->ngpio - 1))
			&& base >= 0) {
		status = -EINVAL;
		goto fail;
	}


	spin_lock_irqsave(&gpio_lock, flags);
....
}


添加到结构体：


struct gpio_desc {
	struct gpio_chip	*chip;
	unsigned long		flags;
/* flag symbols are bit numbers */
#define FLAG_REQUESTED	0
#define FLAG_IS_OUT	1
#define FLAG_RESERVED	2
...
}
这就保存到了 chip 指针。

这一部分在Kernel初始化的时候调用执行！







四、芯片（mach-at91）gpio口的接口

**由gpiolib.c根据映射调用对应接口**


static int at91_gpiolib_direction_output(struct gpio_chip *chip,
					 unsigned offset, int val)
{
	struct at91_gpio_chip *at91_gpio = to_at91_gpio_chip(chip);
	void __iomem *pio = at91_gpio->regbase;
	unsigned mask = 1 << offset;

	__raw_writel(mask, pio + (val ? PIO_SODR : PIO_CODR));
	__raw_writel(mask, pio + PIO_OER);
	return 0;
}
定义为输出模式，初始设置val 上拉还是下拉 ， 写入数据寄存器。


#define PIO_SODR	0x30	/* Set Output Data Register */
#define PIO_CODR	0x34	/* Clear Output Data Register */
#define PIO_ODR	 0x14	/* Output Disable Register */
#define PIO_PDSR	 0x3c	/* Pin Data Status Register */


同样设置为输入：


static int at91_gpiolib_direction_input(struct gpio_chip *chip,
					unsigned offset)
{
	struct at91_gpio_chip *at91_gpio = to_at91_gpio_chip(chip);
	void __iomem *pio = at91_gpio->regbase;
	unsigned mask = 1 << offset;

	__raw_writel(mask, pio + PIO_ODR);
	return 0;
}




设置GPIO口的value 值 0或者1,已经设置为输出模式下：
static void at91_gpiolib_set(struct gpio_chip *chip, unsigned offset, int val)
{
	struct at91_gpio_chip *at91_gpio = to_at91_gpio_chip(chip);
	void __iomem *pio = at91_gpio->regbase;
	unsigned mask = 1 << offset;

	__raw_writel(mask, pio + (val ? PIO_SODR : PIO_CODR));
}


读寄存器获取该GPIO口的值：


static int at91_gpiolib_get(struct gpio_chip *chip, unsigned offset)
{
	struct at91_gpio_chip *at91_gpio = to_at91_gpio_chip(chip);
	void __iomem *pio = at91_gpio->regbase;
	unsigned mask = 1 << offset;
	u32 pdsr;

	pdsr = __raw_readl(pio + PIO_PDSR);
	return (pdsr & mask) != 0;
}


大体流程就这样了。

撰写不易，转载请注明出处[http://blog.csdn.net/jscese/article/details/16823519](http://blog.csdn.net/jscese/article/details/16823519)









