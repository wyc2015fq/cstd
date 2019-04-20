# 实例解析linux内核I2C体系结构（2）（转） - h13 - 博客园
转自：http://www.dzsc.com/data/html/2010-1-27/81710.html
作者：刘洪涛,华清远见嵌入式学院讲师。
　　四、在内核里写i2c设备驱动的两种方式
　　前文介绍了利用/dev/i2c-0在应用层完成对i2c设备的操作，但很多时候我们还是习惯为i2c设备在内核层编写驱动程序。目前内核支持两种编写i2c驱动程序的方式。下面分别介绍这两种方式的实现。这里分别称这两种方式为“Adap[te](http://www.dzsc.com/product/searchfile/10366.html)r方式（LEGACY）”和“Probe方式（new [ST](http://www.dzsc.com/data/iccompany/detail3.html)yle）”。
　　（1） Adapter方式（LEGACY）
　　（下面的实例代码是在2.6.27内核的pca953x.c基础上修改的，原始代码采用的是本文将要讨论的第2种方式，即Probe方式）
　　●    构建i2c_driver
```
staTIc struct i2c_driver pca953x_driver = {
　　.driver = {
　　.name= "pca953x", //名称
　　},
　　.id= ID_pca9555,//id号
　　.attach_adapter= pca953x_attach_adapter, //调用适配器连接设备
　　.detach_client= pca953x_detach_client,//让设备脱离适配器
　　};
```
　　●    注册i2c_driver
```
static int __init pca953x_init(void)
　　{
　　       return i2c_add_driver(&pca953x_driver);
　　}
　　module_init(pca953x_init);
```
　　●    attach_adapter动作
　　执行i2c_add_driver(&pca953x_driver)后会，如果内核中已 经注册了i2c适配器，则顺序调用这些适配器来连接我们的i2c设备。此过程是通过调用i2c_driver中的attach_adapter方法完成 的。具体实现形式如下：
```
static int pca953x_attach_adapter(struct i2c_adapter *adapter)
　　{
　　       return i2c_probe(adapter, &addr_data, pca953x_detect);
　　/*
　　adapter:适配器
　　addr_data:地址信息
　　pca953x_detect：探测到设备后调用的函数
　　*/
　　}
```
　　地址信息addr_data是由下面代码指定的。
```
/* Addresses to scan */
　　static uNSigned short normal_i2c[] = {0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,I2C_CLIENT_END};
　　I2C_CLIENT_INSMOD;
```
　　注意：normal_i2c里的地址必须是你i2c[芯片](http://www.dzsc.com/product/searchfile/6465.html)的地址。否则将无法正确探测到设备。而I2C_ CLIENT_INSMOD是一个宏，它会利用normal_i2c构建addr_data。
　　●    构建i2c_client，并注册字符设备驱动
　　i2c_probe在探测到目标设备后，后调用pca953x_detect，并把当时的探测地址address作为参数传入。
```
static int pca953x_detect(struct i2c_adapter *adapter, int address, int kind)
　　{
　　struct i2c_client *new_client;
　　struct pca953x_chip *chip; //设备结构体
　　int err = 0,result;
　　dev_t pca953x_dev=MKDEV(pca953x_major,0);//构建设备号，根据具体情况设定，这里我只考虑了normal_i2c中只有一个地址匹配的情况。
　　if (!i2c_check_functiONality(adapter, I2C_FUNC_SMBUS_BYTE_DATA| I2C_FUNC_SMBUS_WORD_DATA))//判定适配器能力
　　goto exit;
　　if (!(chip = kzalloc(sizeof(struct pca953x_chip), GFP_KERNEL))) {
　　err = -ENOMEM;
　　goto exit;
　　}
　　/****构建i2c-client****/
　　chip->client=kzalloc(sizeof(struct i2c_client),GFP_KERNEL);
　　new_client = chip->client;
　　i2c_set_clientdata(new_client, chip);
　　new_client->addr = address;
　　new_client->adapter = adapter;
　　new_client->driver = &pca953x_driver;
　　new_client->flags = 0;
　　strlcpy(new_client->nAME, "pca953x", I2C_NAME_SIZE);
　　if ((err = i2c_attach_client(new_client)))//注册i2c_client
　　goto exit_kfree;
　　if (err)
　　goto exit_detach;
　　if(pca953x_major)
　　{
      　　result=register_chrdev_region(pca953x_dev,1,"pca953x");
　　}
　　else{
      　　result=alloc_chrdev_region(&pca953x_dev,0,1,"pca953x");
　　pca953x_major=MAJOR(pca953x_dev);
　　}
　　if (result < 0) {
　　printk(KERN_NOTICE "Unable to get pca953x region, error %d\n", result);
　　return result;
　　}
　　pca953x_setup_cdev(chip,0); //注册字符设备，此处不详解
　　return 0;
　　exit_detach:
　　i2c_detach_client(new_client);
　　exit_kfree:
　　kfree(chip);
　　exit:
　　return err;
　　}
　　i2c_check_functionality用来判定设配器的能力，这一点非常重要。你也可以直接查看对应设配器的能力，如
　　static const struct i2c_algorithm smbus_algorithm = {
　　.smbus_xfer= i801_access,
　　.functionality= i801_func,
　　};
　　static u32 i801_func(struct i2c_adapter *adapter)
　　{
　　return I2C_FUNC_SMBUS_QUICK | I2C_FUNC_SMBUS_BYTE |
　　I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA |
　　I2C_FUNC_SMBUS_BLOCK_DATA | I2C_FUNC_SMBUS_WRITE_I2C_BLOCK
　　| (isich4 ? I2C_FUNC_SMBUS_HWPEC_CALC : 0);
　　}
```
　　●    字符驱动的具体实现
```
struct file_operations pca953x_fops = {
　　.owner = THIS_MODULE,
　　.ioctl= pca953x_ioctl,
　　.open= pca953x_open,
　　.release =pca953x_release,
　　};
```
　　字符设备驱动本身没有什么好说的，这里主要想说一下，如何在驱动中调用i2c设配器帮我们完成数据传输。
　　目前设配器主要支持两种传输方法：smbus_xfer和master_xfer。一般来说，如果设配器支持了master_xfer那么它也可以模拟支持smbus的传输。但如果只实现smbus_xfer，则不支持一些i2c的传输。
　　int (*master_xfer)(struct i2c_adapter *adap,struct i2c_msg *msgs,int num);
　　int (*smbus_xfer) (struct i2c_adapter *adap, u16 addr,
　　unsigned short flags, char read_write,
　　u8 command, int size, union i2c_smbus_data * data);
　　master_xfer中的参数设置，和前面的用户空间编程一致。现在只是要在驱动中构建相关的参数然后调用i2c_transfer来完成传输既可。
```
int i2c_transfer(struct i2c_adapter * adap, struct i2c_msg *msgs, int num)
　　smbus_xfer中的参数设置及调用方法如下：
　　static int pca953x_write_reg(struct pca953x_chip *chip, int reg, uint16_t val)
　　{
　　int ret;
　　ret = i2c_smbus_write_word_data(chip->client, reg << 1, val);
　　if (ret < 0) {
　　dev_err(&chip->client->dev, "failed writing register\n");
　　return -EIO;
　　}
　　return 0;
　　}
```
　　上面函数完成向芯片的地址为reg的[寄存器](http://www.dzsc.com/product/searchfile/19403.html)写一个16bit的数据。i2c_smbus_write_word_data的实现如下：
```
s32 i2c_smbus_write_word_data(struct i2c_client *client, u8 command, u16 value)
　　{
　　union i2c_smbus_data data;
　　data.word = value;
　　return i2c_smbus_xfer(client->adapter,client->addr,client->flags,
　　I2C_SMBUS_WRITE,command,
　　I2C_SMBUS_WORD_DATA,&data);
　　}
```
　　从中可以看出smbus传输一个16位数据的方法。其它操作如：字符写、字符读、字读、块操作等，可以参考内核的i2c-core.c中提供的方法。
　　●    注销i2c_driver
```
static void __exit pca953x_exit(void)
　　{
　　i2c_del_driver(&pca953x_driver);
　　}
　　module_exit(pca953x_exit);
```
　　●    detach_client动作
　　顺序调用内核中注册的适配器来断开我们注册过的i2c设备。此过程通过调用i2c_driver中的attach_adapter方法完成的。具体实现形式如下：
```
static int pca953x_detach_client(struct i2c_client *client)
　　{
　　int err;
　　struct pca953x_chip *data;
　　if ((err = i2c_detach_client(client)))//断开i2c_client
　　return err;
　　data=i2c_get_clientdata(client);
　　cdev_del(&(data->cdev));
　　unregister_chrdev_region(MKDEV(pca953x_major, 0), 1);
　　kfree(data->client);
　　kfree(data);
　　return 0;
　　}
```
　　（2） Probe方式（new style）
　　●    构建i2c_driver
　　和LEGACY方式一样，也需要构建i2c_driver，但是内容有所不同。
```
static struct i2c_driver pca953x_driver = {
　　.driver = {
　　.name= "pca953x",
　　},
　　.probe= pca953x_probe, //当有i2c_client和i2c_driver匹配时调用
　　.remove= pca953x_remove,//注销时调用
　　.id_table= pca953x_id,//匹配规则
　　};
```
　　●    注册i2c_driver
```
static int __init pca953x_init(void)
　　{
　　return i2c_add_driver(&pca953x_driver);
　　}
　　module_init(pca953x_init);
```
　　在注册i2c_driver的过程中，是将driver注册到了i2c_bus_type的[总线](http://www.dzsc.com/product/searchfile/4067.html)上。此[总线](http://wiki.dzsc.com/info/1702.html)的匹配规则是：
```
static const struct i2c_device_id *i2c_match_id(const struct i2c_device_id *id,
　　const struct i2c_client *client)
　　{
　　while (id->name[0]) {
　　if (strcmp(client->name, id->name) == 0)
　　return id;
　　id++;
　　}
　　return NULL;
　　}
```
　　可以看出是利用i2c_client的名称和id_table中的名称做匹配的。本驱动中的id_table为
```
static const struct i2c_device_id pca953x_id[] = {
　　{ "pca9534", 8, },
　　{ "pca9535", 16, },
　　{ "pca9536", 4, },
　　{ "pca9537", 4, },
　　{ "pca9538", 8, },
　　{ "pca9539", 16, },
　　{ "pca9554", 8, },
　　{ "pca9555", 16, },
　　{ "pca9557", 8, },
　　{ "max7310", 8, },
　　{ }
　　};
```
　　看到现在我们应该会有这样的疑问，在Adapter模式中，i2c_client是我们自己构造出来的，而现在的i2c_client是从哪来的呢？看看下面的解释
　　●    注册i2c_b[OA](http://www.dzsc.com/data/iccompany/detail248.html)rd_info
　　对于Probe模式，通常在平台代码中要完成i2c_board_info的注册。方法如下：
```
static struct i2c_board_info __initdata test_i2c_devices[] = {
　　{
　　I2C_BOARD_INFO("pca9555", 0x27),//pca9555为芯片名称，0x27为芯片地址
　　.platform_data = &pca9555_data,
　　}, {
　　I2C_BOARD_INFO("mt9v022", 0x48),
　　.platform_data = &iclink[0], /* With extender */
　　}, {
　　I2C_BOARD_INFO("mt9m001", 0x5d),
　　.platform_data = &iclink[0], /* With extender */
　　},
　　};
　　i2c_register_board_info(0, test_i2c_devices,ARRAY_SIZE(test_i2c_devices)); //注册
```
　　i2c_client就是在注册过程中构建的。但有一点需要注意的是i2c_register_board_info并没有EXPORT_SYMBOL给模块使用。
　　●    字符驱动注册
　　在Probe方式下，添加字符驱动的位置在pca953x_probe中。
```
static int __devinit pca953x_probe(struct i2c_client *client,const struct i2c_device_id *id)
　　{
　　……
　　/****字符设备驱动注册位置****/
　　……
　　return 0;
　　}
```
　　●    注销i2c_driver
```
static void __exit pca953x_exit(void)
　　{
　　i2c_del_driver(&pca953x_driver);
　　}
　　module_exit(pca953x_exit);
```
　　●    注销字符设备驱动
　　在Probe方式下，注销字符驱动的位置在pca953x_remove中。
```
static int __devinit pca953x_remove (struct i2c_client *client)
　　{
　　……
　　/****字符设备驱动注销的位置****/
　　……
　　return 0;
　　}
```
　　●    I2C设备的数据交互方法(即：调用适配器操作设备的方法)和Adapter方式下相同。
“本文由华清远见http://www.embedu.org/index.htm提供”
