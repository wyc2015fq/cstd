# libusb在arm上的编译————转 - h13 - 博客园
涉及到很多库的依赖问题。
gphoto2 : libusb, libexif, libgphoto
ptpcam : libusb, libptp
经过一天的摸索，终于成功将ptpcam移植到了mini2440的板子上，由于还没有相机，目前还不知道是否成功。但是程序运行没问题。下面记下此次移植的过程：
1.下载libptp软件包：[http://sourceforge.net/projects/libptp/files/](http://sourceforge.net/projects/libptp/files/)
   下载libusb-0.1.12软件包：[http://sourceforge.net/projects/libusb/files/](http://sourceforge.net/projects/libusb/files/)
2.分别解压
3.进入libusb-0.1.12，交叉编译：
    mkdir install
    ./configure --build=i686-linux --host=arm-linux --prefix=`pwd`/install
   make
    make install
说明一下：--build=i686-linux表示该软件在x86平台上被编译
                 --host=arm-linux表示该软件编译完成后在arm平台上运行
                 --prefix后面为软件安装目录
注：移植好libusb之后可以测试一下该包工作是否正常，这样可以用来排除故障。一下是一个基于libusb的测试程序，如果可以检测到端口表示包没问题。
``
```
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
void print_endpoint(struct usb_endpoint_descriptor *endpoint)
{
printf(" bEndpointAddress: %02xh\n", endpoint->bEndpointAddress);
printf(" bmAttributes: %02xh\n", endpoint->bmAttributes);
printf(" wMaxPacketSize: %d\n", endpoint->wMaxPacketSize);
printf(" bInterval: %d\n", endpoint->bInterval);
printf(" bRefresh: %d\n", endpoint->bRefresh);
printf(" bSynchAddress: %d\n", endpoint->bSynchAddress);
}
void print_altsetting(struct usb_interface_descriptor *interface)
{
int i;
printf(" bInterfaceNumber: %d\n", interface->bInterfaceNumber);
printf(" bAlternateSetting: %d\n", interface->bAlternateSetting);
printf(" bNumEndpoints: %d\n", interface->bNumEndpoints);
printf(" bInterfaceClass: %d\n", interface->bInterfaceClass);
printf(" bInterfaceSubClass: %d\n", interface->bInterfaceSubClass);
printf(" bInterfaceProtocol: %d\n", interface->bInterfaceProtocol);
printf(" iInterface: %d\n", interface->iInterface);
for (i = 0; i < interface->bNumEndpoints; i++)
print_endpoint(&interface->endpoint[i]);
}
void print_interface(struct usb_interface *interface)
{
int i;
for (i = 0; i < interface->num_altsetting; i++)
print_altsetting(&interface->altsetting[i]);
}
void print_configuration(struct usb_config_descriptor *config)
{
int i;
printf(" wTotalLength: %d\n", config->wTotalLength);
printf(" bNumInterfaces: %d\n", config->bNumInterfaces);
printf(" bConfigurationValue: %d\n", config->bConfigurationValue);
printf(" iConfiguration: %d\n", config->iConfiguration);
printf(" bmAttributes: %02xh\n", config->bmAttributes);
printf(" MaxPower: %d\n", config->MaxPower);
for (i = 0; i < config->bNumInterfaces; i++)
print_interface(&config->interface[i]);
}
int main(void)
{
struct usb_bus *bus;
struct usb_device *dev;
usb_init();
usb_find_busses();
usb_find_devices();
printf("bus/device idVendor/idProduct\n");
for (bus = usb_busses; bus; bus = bus->next) {
for (dev = bus->devices; dev; dev = dev->next) {
int ret, i;
char string[256];
usb_dev_handle *udev;
printf("%s/%s %04X/%04X\n", bus->dirname, dev->filename,
dev->descriptor.idVendor, dev->descriptor.idProduct);
udev = usb_open(dev);
if (udev) {
if (dev->descriptor.iManufacturer) {
ret = usb_get_string_simple(udev, dev->descriptor.iManufacturer, string, sizeof(string));
if (ret > 0)
printf("- Manufacturer : %s\n", string);
else
printf("- Unable to fetch manufacturer string\n");
}
if (dev->descriptor.iProduct) {
ret = usb_get_string_simple(udev, dev->descriptor.iProduct, string, sizeof(string));
if (ret > 0)
printf("- Product : %s\n", string);
else
printf("- Unable to fetch product string\n");
}
if (dev->descriptor.iSerialNumber) {
ret = usb_get_string_simple(udev, dev->descriptor.iSerialNumber, string, sizeof(string));
if (ret > 0)
printf("- Serial Number: %s\n", string);
else
printf("- Unable to fetch serial number string\n");
}
usb_close (udev);
}
if (!dev->config) {
printf(" Couldn't retrieve descriptors\n");
continue;
}
for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
print_configuration(&dev->config[i]);
}
}
return 0;
}
```
编译的时候记得加上-lusb选项，交叉编译还要加上libusb包的路径-I/.../include和-L/.../lib
4.进入libptp目录：
    mkdir install
    下面进行交叉编译的时候系统会去寻找libusb库，过程是这样的：首先找到/usr/bin/libusb-config文件，根据该文件中的路径进行 库的寻找。所以这里的libusb-config一定要替换成我们之前自己编译的那个，在install/bin下面。覆盖前记得备份，以便恢复。
    ./configure --build=i686-linux --host=arm-linux --prefix=`pwd`/install
注：这一步应该会出错。用以下命令代替：
step1 
执行：echo "ac_cv_func_malloc_0_nonnull=yes" > arm-linux.cache
step2
./configure --prefix=`pwd`/install --build=i686-linux --host=arm-linux --cache-file=`pwd`/arm-linux.cache
或者一条命令搞定：
./configure --prefix=`pwd`/install --build=i686-linux --host=arm-linux ac_cv_func_malloc_0_nonnull=yes
   make
    make install
安装完成。
5.接下来就是把在install文件夹中生成的文件拷贝到开发板上去。在开发板上建立目录/ptpcam ，将install里面的东西全部拷贝过去。还有就是要将libusb库也拷到板子上。我的是拷贝到了/lib下面。搞定！
这个问题搞了我好多天。都没搞定。
在编译的时候总是有错。。我当时用的是fc7.。后来改用fc12.。。fc12下便宜的arm-linux-gcc开始用的是4.0.0.。安装的时候不知道什么原因中间卡死！夭折了。
不过改了profile的东西，arm-linux-gcc还是能用的。至少编译一个个helloword.c是没有问题。上不libusb后就不行。。
不过最终还是怪我，好好libusb0.1.12不用，去下了一个libusb.1.6.x的。自以为事的认为新的版本是最好的。。（最终发现是在这个地方出了问题。）
不知道是什么原因就到处找原因：因为其显示是没有usb.h...所以我就很笨很笨的用了locate命令来搜索（这个命令也是临时学的。。想用find。不会用。。后来的事实告诉我locate就没把我放在在/tmp/中的usb.h找出来）。
看到了内核中有usb.h，就想当然的在编译的时候-I到内核的include中。。结果错误一大堆。在网上找也找不原因。乱七陈糟。
再然后，换arm-linux-gcc的版本为4.3.2.。友善的用的这个。。。
错误依然！！。
百思不得其解，，，后来无赖的换成了libusb0.1.12。。ok。。编译通过！！
想了下。。可能是./configure --build=i686-linux --host=arm-linux --prefix=`pwd`/install不一定适合新板本libusb的安装。
其给我readme又没认真看。。。
