# linux 标准 GPIO 操作 - DoubleLi - 博客园






Linux 提供了GPIO 操作的 API，具体初始化及注册函数在 driver/gpio/lib_gpio.c 中实现。



#include   

int gpio_request(unsigned gpio, const char *label);

获得并占有 GPIO port 的使用权，由参数 gpio 指定具体 port。非空的label指针有助于诊断。主要告诉内核这块地址被占用了。当其他地方调用同一地址的gpio_request就会报告错误，该地址已经被申请。在/proc/mem有地址占用表描述。先申请在访问的好处是避免资源竞争。

void gpio_free(unsigned gpio)；

释放 GPIO port 的使用权，由参数 gpio 指定具体 port。



int gpio_direction_input(unsigned gpio);

将 GPIO port（由参数 gpio 指定）设为输入。



int gpio_direction_output(unsigned gpio, int value)；

将 GPIO port（由参数 gpio 指定）设为输出，并指定输出电平值（value）。



int gpio_get_value(unsigned gpio);

获得 GPIO port 上的电平值并返回。

void gpio_set_value(unsigned gpio, int value);

设置 GPIO port 上的电平。

int gpio_to_irq(unsigned gpio);

通过获得 gpio port 对应的 irq number。



int gpio_cansleep(unsigned gpio)

这个主要区分是挂载在外部总线上的GPIO控制器，还是内部的GPIO控制器。因为外部总线上的GPIO控制器可以休眠，所以用这个来区分是否是扩展的GPIO，返回1是扩展的GPIO，0的话就是内部的GPIO。



int gpio_get_value_cansleep(unsigned gpio);

void gpio_set_value_cansleep(unsigned gpio, int value);

这些都是对可以休眠扩展的gpio的操作



static inline int gpio_is_valid(int number) 判断GPIO是否有效，有效返回0

int gpio_export(unsigned gpio, booldirection_may_change);

void gpio_unexport();

//在 /sys/class/gpio/下面创建和取消对应的gpio号

int gpio_export_link(struct device *dev, const char *name, unsigned gpio)

//创建到导出GPIO的 sysfs link ，第一个参数是在哪个dev下创建，第二个是参数名字，第三个是gpio编号









