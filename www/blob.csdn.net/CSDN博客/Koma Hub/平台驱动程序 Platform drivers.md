# 平台驱动程序: Platform drivers - Koma Hub - CSDN博客
2019年03月02日 11:08:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：60

# Platform drivers
> - On embedded systems, devices are often not connected through a bus allowing enumeration, hotplugging, and providing unique identiers for devices.
- However, we still want the devices to be part of the device model.
- The solution to this is the platform driver / platform device infrastructure.
- The platform devices are the devices that are directly connected to the CPU, without any kind of bus.
- 在嵌入式系统中，设备通常不通过总线连接，从而允许枚举、热插拔和为设备提供唯一标识。
- 但是，我们仍然希望设备成为设备模型的一部分。
- 解决方案是平台驱动程序/平台设备基础结构。
- 平台设备是直接连接到CPU的设备，没有任何类型的总线。
## Initialization of a platform driver
> 
Example of the iMX serial port driver, in **drivers/serial/imx.c**. The driver instantiates a **platform_driver **structure:
驱动程序/serial/imx.c中的imx串行端口驱动程序示例。驱动程序实例化平台驱动程序结构：
```cpp
static struct platform_driver serial_imx_driver = {
    .probe = serial_imx_probe,
    .remove = serial_imx_remove,
    .driver = {
        .name = "imx-uart",
        .owner = THIS_MODULE,
    },
};
```
And registers/unregisters it at init/cleanup:
```cpp
static int __init imx_serial_init(void)
{
    platform_driver_register(&serial_imx_driver);
}
static void __ext imx_serial_cleanup(void)
{
    platform_driver_unregister(&serial_imx_driver);
}
```
> 
As platform devices cannot be detected dynamically, they are statically dened:
- by direct instantiation of platform device structures, as done on ARM
- by using a device tree, as done on PowerPC
Example on ARM, where the instantiation is done in the board specic code (arch/arm/mach-imx/mx1ads.c)
由于无法动态检测平台设备，因此它们被静态拒绝：
- 通过直接实例化平台设备结构，如ARM
- 通过使用设备树，就像在PowerPC上一样
ARM上的示例，其中实例化是在板的特定代码（**arch/arm/mach imx/mx1ads.c**）中完成的。
```cpp
static struct platform_device imx_uart1_device = {
    .name = "imx-uart",
    .id = 0,
    .num_resources = ARRAY_SIZE(imx_uart1_resources),
    .resource = imx_uart1_resources,
    .dev = {
        .platform_data = &uart_pdata,
    }
};
```
The matching between a device and the driver is simply done using the name.
## Registration of platform devices平台设备注册
The device is part of a list:
```cpp
static struct platform_device *devices[] __initdata = {
    &cs89x0_device,
    &imx_uart1_device,
    &imx_uart2_device,
};
```
And the list of devices is added to the system during the board initialization
```cpp
static void __init mx1ads_init(void)
{
    [...]
    platform_add_devices(devices, ARRAY_SIZE(devices));
    [...]
}
MACHINE_START(MX1ADS, "Freescale MX1ADS")
    [...]
    .init_machine = mx1ads_init,
MACHINE_END
```
## The resource mechanism资源机制
> - Each device managed by a particular driver typically uses different hardware resources: dierent addresses for the I/O registers, different DMA channel, dierent IRQ line, etc.
- These informations can be represented using the kernel struct resource, and an array of resources is associated to a platform device denition.
- 由特定驱动程序管理的每个设备通常使用不同的硬件资源：I/O寄存器的dierent地址、不同的DMA通道、dierent irq线路等。
- 这些信息可以使用内核结构资源表示，并且一组资源与平台设备关联。
```cpp
static struct resource imx_uart1_resources[] = {
    [0] = {
        .start = 0x00206000,
        .end = 0x002060FF,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = (UART1_MINT_RX),
        .end = (UART1_MINT_RX),
        .flags = IORESOURCE_IRQ,
    },
};
```
## The platform_data mechanism
> - In addition to the well-dened resources, some driver require driver-specic conguration for each platform device
- These can be specied using the platform_data eld of the struct device
- As it is a void * pointer, it can be used to pass any type of data to the driver
- In the case of the iMX driver, the platform data is a struct imxuart_platform_data structure, referenced from the platform_device structure
- 除了资源匮乏之外，一些驱动程序还需要为每个平台设备指定驱动程序。
- 这些可以使用结构设备的平台数据字段指定
- 因为它是一个void*指针，所以可以将任何类型的数据传递给驱动程序。
- 在IMX驱动程序的情况下，平台数据是一个结构imxuart_平台_数据结构，从平台_设备结构引用。
```cpp
static struct imxuart_platform_data uart_pdata = {
    .flags = IMXUART_HAVE_RTSCTS,
};
```
# Driver-specific data structure
> - Typically, device drivers subclass the type-specic data structure that they must instantiate to register their device to the upper layer framework
- For example, serial drivers subclass uart_port, network drivers subclass netdev, framebuffer drivers subclass fb_info
- This inheritance is done by aggregation or by reference
- 通常，设备驱动程序子类化类型特定的数据结构，它们必须实例化该数据结构，以便将设备注册到上层框架中。
- 例如，串行驱动程序子类uart_port、网络驱动程序子类netdev、帧缓冲区驱动程序子类fb_info
- 此继承是通过聚合或引用完成的
```cpp
struct imx_port {
    struct uart_port port;
    struct timer_list timer;
    unsigned int old_status;
    int txirq,rxirq,rtsirq;
    unsigned int have_rtscts:1;
    unsigned int use_irda:1;
    unsigned int irda_inv_rx:1;
    unsigned int irda_inv_tx:1;
    unsigned short trcv_delay; /* transceiver delay */
    struct clk *clk;
};
```
## probe() method for platform devices
Just like the usual probe() methods, it receives the platform_device pointer, uses different utility functions to find the corresponding resources, and registers the device to the corresponding upper layer.
```cpp
static int serial_imx_probe(struct platform_device *pdev)
{
    struct imx_port *sport;
    struct imxuart_platform_data *pdata;
    void __iomem *base;
    struct resource *res;
    sport = kzalloc(sizeof(*sport), GFP_KERNEL);
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    base = ioremap(res->start, PAGE_SIZE);
    sport->port.dev = &pdev->dev;
    sport->port.mapbase = res->start;
    sport->port.membase = base;
    sport->port.type = PORT_IMX,
    sport->port.iotype = UPIO_MEM;
    sport->port.irq = platform_get_irq(pdev, 0);
    sport->rxirq = platform_get_irq(pdev, 0);
    sport->txirq = platform_get_irq(pdev, 1);
    sport->rtsirq = platform_get_irq(pdev, 2);
    [...]
    sport->port.fifosize = 32;
    sport->port.ops = &imx_pops;
    sport->clk = clk_get(&pdev->dev, "uart");
    clk_enable(sport->clk);
    sport->port.uartclk = clk_get_rate(sport->clk);
    imx_ports[pdev->id] = sport;
    pdata = pdev->dev.platform_data;
    if (pdata && (pdata->flags & IMXUART_HAVE_RTSCTS))
    sport->have_rtscts = 1;
    ret = uart_add_one_port(&imx_reg, &sport->port);
    if (ret)
    goto deinit;
    platform_set_drvdata(pdev, &sport->port);
    return 0;
}
```
# Other non-dynamic busses
> - In addition to the special platform bus, there are some other busses that do not support dynamic enumeration and identication of devices. For example: I2C and SPI.
- For these busses, a list of devices connected to the bus is hardcoded into the board-specic informations and is registered using **i2c_register_board_info**() or **spi_register_board_info**(). The binding between the device is also done using a string identier.
- 除了特殊平台总线之外，还有一些其他总线不支持设备的动态枚举和标识。例如：I2c和SPI。
- 对于这些总线，连接到总线的设备列表被硬编码到板的特定信息中，并使用i2c_register_board_info（）或spi_register_board_info（）进行注册。设备之间的绑定也可以使用字符串标识符来完成。
```cpp
static struct i2c_board_info pcm038_i2c_devices[] = {
    { I2C_BOARD_INFO("at24", 0x52),
    .platform_data = &board_eeprom, },
    { I2C_BOARD_INFO("pcf8563", 0x51), },
    { I2C_BOARD_INFO("lm75", 0x4a), }
};
static void __init pcm038_init(void) {
    [...]
    i2c_register_board_info(0, pcm038_i2c_devices,
    ARRAY_SIZE(pcm038_i2c_devices));
    [...]
}
```
# Typical organization of a driver
> - Denes a **driver-specific data structure** to keep track of per-device state, this structure often subclass the type-specic structure for this type of device
- Implements a set of **helper functions**, interrupt handlers, etc.
- Implements some or all of the **operations**, as specied by the framework in which the device will be subscribed
- Instantiate the **operation table**
- Denes a probe() **method **that allocates the \state" **structure**, initializes the device and registers it to the upper layer framework. Similarly denes a corresponding remove() method
- Instantiate a **SOMEBUS_driver **structure that references the probe() and remove() methods and give the bus infrastructure some way of binding a device to this driver (by name, by identier, etc.)
- In the **driver initialization function**, register as a device driver to the bus-specic infrastructure. In the driver cleanup function, unregister from the bus-specic infrastructure.
- 拒绝特定于驱动程序的数据结构以跟踪每个设备的状态，此结构通常子类化此类型设备的类型特定结构
- 实现一组助手函数、中断处理程序等。
- 实现设备订阅框架指定的部分或全部操作
- 实例化操作表
- 拒绝probe（）方法，该方法分配\state“结构，初始化设备并将其注册到上层框架。同样，拒绝相应的remove（）方法
- 实例化引用probe（）和remove（）方法的some bus_驱动程序结构，并为总线基础结构提供将设备绑定到此驱动程序的某种方法（按名称、按标识符等）。
- 在驱动程序初始化功能中，注册为总线特定基础结构的设备驱动程序。在驱动程序清理功能中，从总线特定的基础结构中注销。
# Conclusion
> - The Linux kernel now has a coherent and uniform model to organize busses, drivers and devices. This model, and the Linux kernel in general, uses some concept of object-oriented programming to structure the code.
- The organization of device drivers has been greatly simplied and unied by using this model. Functionalities such as udev have been made possible using this unied model.
- Linux内核现在有了一个一致的统一模型来组织总线、驱动程序和设备。这个模型和Linux内核通常使用面向对象编程的一些概念来构造代码。
- 该模型大大简化和统一了设备驱动程序的组织。使用这种统一的模型可以实现诸如udev之类的功能。
> 
Free Electrons. Kernel, drivers and embedded Linux development, consulting, training and support. http://free-electrons.com
