
# linux usb学习笔记 - 嵌入式Linux - CSDN博客

2016年05月27日 17:02:24[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：676


## USB 设备基础知识
## usb端点
**usb端点用一个结构体来描述**
USB 端点在内核中使用结构 struct usb_host_endpoint 来描述. 这个结构包含真实的端点信息在另一个结构中, 称为 struct usb_endpoint_descriptor. 后者包含所有的 USB-特定 数据, 以设备自身特定的准确格式. 驱动关心的这个结构的成员是:
bEndpointAddress
这是这个特定端点的 USB 地址. 还包含在这个 8-位 值的是端点的方向. 位掩码USB_DIR_OUT 和USB_DIR_IN 可用来和这个成员比对, 来决定给这个端点的数据是到设备还是到主机.
bmAttributes
这是端点的类型. 位掩码 USB_ENDPOINT_XFERTYPE_MASK 应当用来和这个值比对,来决定这个端点是否是 USB_ENDPOINT_XFER_ISOC, USB_ENDPOINT_XFER_BULK, 或者是类型 USB_ENDPOINT_XFER_INT. 这些宏定义了同步, 块, 和中断端点, 相应地.LINUX DEVICE DRIVERS,3RD EDITION
wMaxPacketSize
这是以字节计的这个端点可一次处理的最大大小. 注意驱动可能发送大量的比这个值大的数据到端点, 但是数据会被分为 wMaxPakcetSize 的块, 当真正传送到设备时. 对于高速设备, 这个成员可用来支持端点的一个高带宽模式, 通过使用几个额外位在这个值的高位部分. 关于如何完成的细节见 USB 规范.
bInterval
如果这个端点是中断类型的, 这个值是为这个端点设置的间隔, 即在请求端点的中断之间的时间. 这个值以毫秒表示.这个结构的成员没有一个”传统” Linux 内核的命名机制. 这是因为这些成员直接对应于USB 规范中的名子. USB 内核程序员认为使用规定的名子更重要, 以便在阅读规范时减少混乱, 不必使这些名子对Linux 程序员看起来熟悉
在usb.h这个文件里面
```python
/**
 * struct usb_host_endpoint - host-side endpoint descriptor
```
```python
and
```
```python
queue
 *
```
```python
@desc
```
```python
: descriptor
```
```python
for
```
```python
this
```
```python
endpoint, wMaxPacketSize
```
```python
in
```
```python
native
```
```python
byteorder
 *
```
```python
@ss_ep_comp
```
```python
: SuperSpeed companion descriptor
```
```python
for
```
```python
this
```
```python
endpoint
 *
```
```python
@urb_list
```
```python
: urbs queued to
```
```python
this
```
```python
endpoint; maintained
```
```python
by
```
```python
usbcore
 *
```
```python
@hcpriv
```
```python
:
```
```python
for
```
```python
use
```
```python
by
```
```python
HCD; typically holds hardware dma queue head (QH)
 *
```
```python
with
```
```python
one
```
```python
or
```
```python
more transfer descriptors (TDs) per urb
 *
```
```python
@ep_dev
```
```python
: ep_device
```
```python
for
```
```python
sysfs info
 *
```
```python
@extra
```
```python
: descriptors following
```
```python
this
```
```python
endpoint
```
```python
in
```
```python
the configuration
 *
```
```python
@extralen
```
```python
: how many bytes
```
```python
of
```
```python
"extra"
```
```python
are valid
 *
```
```python
@enabled
```
```python
: URBs may be submitted to
```
```python
this
```
```python
endpoint
 *
 * USB requests are always queued to a given endpoint, identified
```
```python
by
```
```python
a
 * descriptor within an active interface
```
```python
in
```
```python
a given USB configuration.
 */
struct usb_host_endpoint {
    struct usb_endpoint_descriptor      desc;
    struct usb_ss_ep_comp_descriptor    ss_ep_comp;
    struct list_head        urb_list;
```
```python
void
```
```python
*hcpriv;
    struct ep_device        *ep_dev;
```
```python
/* For sysfs info */
```
```python
unsigned char *extra;
```
```python
/* Extra descriptors */
```
```python
int extralen;
    int enabled;
};
```
## usb接口
USB 端点被绑在接口中. USB 接口只处理一类 USB 逻辑连接, 例如一个鼠标, 一个键盘,或者一个音频流. 一些 USB 设备有多个接口, 例如一个 USB 扬声器可能有 2 个接口:一个 USB 键盘给按钮和一个 USB 音频流. 因为一个 USB 接口表示基本的功能, 每个USB 驱动控制一个接口; 因此, 对扬声器的例子, Linux 需要 2 个不同的驱动给一个硬件设备。
还是在usb.h文件里面

```python
/**
 * struct usb_interface - what usb device drivers talk
```
```python
to
```
```python
* @altsetting: array
```
```python
of
```
```python
interface structures, one
```
```python
for
```
```python
each alternate
 *  setting
```
```python
that
```
```python
may be selected.  Each one includes a
```
```python
set
```
```python
of
```
```python
*  endpoint configurations.  They will be
```
```python
in
```
```python
no particular order.
 * @cur_altsetting:
```
```python
the
```
```python
current altsetting.
 * @num_altsetting:
```
```python
number
```
```python
of
```
```python
altsettings defined.
 * @intf_assoc: interface association descriptor
 * @minor:
```
```python
the
```
```python
minor
```
```python
number
```
```python
assigned
```
```python
to
```
```python
this interface,
```
```python
if
```
```python
this
 *  interface
```
```python
is
```
```python
bound
```
```python
to
```
```python
a driver
```
```python
that
```
```python
uses
```
```python
the
```
```python
USB major
```
```python
number
```
```python
.
 *  If this interface
```
```python
does
```
```python
not
```
```python
use
```
```python
the
```
```python
USB major, this field should
 *  be unused.  The driver should
```
```python
set
```
```python
this value
```
```python
in
```
```python
the
```
```python
probe()
 *  function
```
```python
of
```
```python
the
```
```python
driver,
```
```python
after
```
```python
it
```
```python
has been assigned a minor
 *
```
```python
number
```
```python
from
```
```python
the
```
```python
USB core
```
```python
by
```
```python
calling usb_register_dev().
 * @condition: binding state
```
```python
of
```
```python
the
```
```python
interface:
```
```python
not
```
```python
bound, binding
 *  (
```
```python
in
```
```python
probe()), bound
```
```python
to
```
```python
a driver,
```
```python
or
```
```python
unbinding (
```
```python
in
```
```python
disconnect())
 * @sysfs_files_created: sysfs attributes exist
 * @ep_devs_created: endpoint child pseudo-devices exist
 * @unregistering: flag
```
```python
set
```
```python
when
```
```python
the
```
```python
interface
```
```python
is
```
```python
being unregistered
 * @needs_remote_wakeup: flag
```
```python
set
```
```python
when
```
```python
the
```
```python
driver requires remote-wakeup
 *  capability during autosuspend.
 * @needs_altsetting0: flag
```
```python
set
```
```python
when a
```
```python
set
```
```python
-interface request
```
```python
for
```
```python
altsetting
```
```python
0
```
```python
*  has been deferred.
 * @needs_binding: flag
```
```python
set
```
```python
when
```
```python
the
```
```python
driver should be re-probed
```
```python
or
```
```python
unbound
 *  following a reset
```
```python
or
```
```python
suspend operation
```
```python
it
```
```python
doesn't support.
 * @dev: driver model's view
```
```python
of
```
```python
this device
 * @usb_dev:
```
```python
if
```
```python
an interface
```
```python
is
```
```python
bound
```
```python
to
```
```python
the
```
```python
USB major, this will point
 *
```
```python
to
```
```python
the
```
```python
sysfs representation
```
```python
for
```
```python
that
```
```python
device.
 * @pm_usage_cnt: PM usage counter
```
```python
for
```
```python
this interface
 * @reset_ws: Used
```
```python
for
```
```python
scheduling resets
```
```python
from
```
```python
atomic context.
 * @reset_running:
```
```python
set
```
```python
to
```
```python
1
```
```python
if
```
```python
the
```
```python
interface
```
```python
is
```
```python
currently
```
```python
running
```
```python
a
 *      queued reset so
```
```python
that
```
```python
usb_cancel_queued_reset() doesn't
```
```python
try
```
```python
to
```
```python
*      remove
```
```python
from
```
```python
the
```
```python
workqueue when
```
```python
running
```
```python
inside
```
```python
the
```
```python
worker
 *      thread. See __usb_queue_reset_device().
 * @resetting_device: USB core reset
```
```python
the
```
```python
device, so use alt setting
```
```python
0
```
```python
as
```
```python
*  current; needs bandwidth alloc
```
```python
after
```
```python
reset.
 *
 * USB device drivers attach
```
```python
to
```
```python
interfaces
```
```python
on
```
```python
a physical device.  Each
 * interface encapsulates a single high level function, such
```
```python
as
```
```python
feeding
 * an audio stream
```
```python
to
```
```python
a speaker
```
```python
or
```
```python
reporting a change
```
```python
in
```
```python
a volume control.
 * Many USB devices only have one interface.  The protocol used
```
```python
to
```
```python
talk
```
```python
to
```
```python
* an interface's endpoints can be defined
```
```python
in
```
```python
a usb
```
```python
"class"
```
```python
specification,
 *
```
```python
or
```
```python
by
```
```python
a product's vendor.  The (default) control endpoint
```
```python
is
```
```python
part
```
```python
of
```
```python
*
```
```python
every
```
```python
interface,
```
```python
but
```
```python
is
```
```python
never listed among
```
```python
the
```
```python
interface's descriptors.
 *
 * The driver
```
```python
that
```
```python
is
```
```python
bound
```
```python
to
```
```python
the
```
```python
interface can use standard driver model
 * calls such
```
```python
as
```
```python
dev_get_drvdata()
```
```python
on
```
```python
the
```
```python
dev member
```
```python
of
```
```python
this structure.
 *
 * Each interface may have alternate settings.  The initial configuration
 *
```
```python
of
```
```python
a device sets altsetting
```
```python
0
```
```python
,
```
```python
but
```
```python
the
```
```python
device driver can change
 *
```
```python
that
```
```python
setting using usb_set_interface().  Alternate settings are often
 * used
```
```python
to
```
```python
control
```
```python
the
```
```python
use
```
```python
of
```
```python
periodic endpoints, such
```
```python
as
```
```python
by
```
```python
having
 * different endpoints use different amounts
```
```python
of
```
```python
reserved USB bandwidth.
 * All standards-conformant USB devices
```
```python
that
```
```python
use isochronous endpoints
 * will use them
```
```python
in
```
```python
non-default settings.
 *
 * The USB specification says
```
```python
that
```
```python
alternate setting numbers must
```
```python
run
```
```python
from
```
```python
*
```
```python
0
```
```python
to
```
```python
one
```
```python
less than
```
```python
the
```
```python
total
```
```python
number
```
```python
of
```
```python
alternate settings.  But
```
```python
some
```
```python
* devices manage
```
```python
to
```
```python
mess this up,
```
```python
and
```
```python
the
```
```python
structures aren't necessarily
 * stored
```
```python
in
```
```python
numerical order anyhow.  Use usb_altnum_to_altsetting()
```
```python
to
```
```python
* look up an alternate setting
```
```python
in
```
```python
the
```
```python
altsetting array based
```
```python
on
```
```python
its
```
```python
number
```
```python
.
 */
struct usb_interface {
    /* array
```
```python
of
```
```python
alternate settings
```
```python
for
```
```python
this interface,
     * stored
```
```python
in
```
```python
no particular order */
    struct usb_host_interface *altsetting;
    struct usb_host_interface *cur_altsetting;  /*
```
```python
the
```
```python
currently
                     * active alternate setting */
    unsigned num_altsetting;    /*
```
```python
number
```
```python
of
```
```python
alternate settings */
    /* If there
```
```python
is
```
```python
an interface association descriptor
```
```python
then
```
```python
it
```
```python
will
```
```python
list
```
```python
*
```
```python
the
```
```python
associated interfaces */
    struct usb_interface_assoc_descriptor *intf_assoc;
    int minor;          /* minor
```
```python
number
```
```python
this interface
```
```python
is
```
```python
* bound
```
```python
to
```
```python
*/
    enum usb_interface_condition condition;     /* state
```
```python
of
```
```python
binding */
    unsigned sysfs_files_created:
```
```python
1
```
```python
; /*
```
```python
the
```
```python
sysfs attributes exist */
    unsigned ep_devs_created:
```
```python
1
```
```python
; /* endpoint
```
```python
"devices"
```
```python
exist */
    unsigned unregistering:
```
```python
1
```
```python
;   /* unregistration
```
```python
is
```
```python
in
```
```python
progress */
    unsigned needs_remote_wakeup:
```
```python
1
```
```python
; /* driver requires remote wakeup */
    unsigned needs_altsetting0:
```
```python
1
```
```python
;   /* switch
```
```python
to
```
```python
altsetting
```
```python
0
```
```python
is
```
```python
pending */
    unsigned needs_binding:
```
```python
1
```
```python
;   /* needs delayed unbind/rebind */
    unsigned reset_running:
```
```python
1
```
```python
;
    unsigned resetting_device:
```
```python
1
```
```python
;    /*
```
```python
true
```
```python
: bandwidth alloc
```
```python
after
```
```python
reset */
    struct device dev;      /* interface specific device info */
    struct device *usb_dev;
    atomic_t pm_usage_cnt;      /* usage counter
```
```python
for
```
```python
autosuspend */
    struct work_struct reset_ws;    /*
```
```python
for
```
```python
resets
```
```python
in
```
```python
atomic context */
};
```
## usb配置
linux 描述 USB 配置使用结构 struct usb_host_config 和整个 USB 设备使用结构struct usb_device. USB 设备驱动通常不会需要读写这些结构的任何值, 因此它们在这里没有详细定义. 好奇的读者可在内核源码树的文件 include/linux/usb.h 中找到对它们的描述

```python
/**
 * struct usb_host_config - representation
```
```python
of
```
```python
a device's configuration
 * @desc:
```
```python
the
```
```python
device's configuration descriptor.
 * @
```
```python
string
```
```python
: pointer
```
```python
to
```
```python
the
```
```python
cached
```
```python
version
```
```python
of
```
```python
the
```
```python
iConfiguration
```
```python
string
```
```python
,
```
```python
if
```
```python
*  present
```
```python
for
```
```python
this configuration.
 * @intf_assoc:
```
```python
list
```
```python
of
```
```python
any interface association descriptors
```
```python
in
```
```python
this config
 * @interface: array
```
```python
of
```
```python
pointers
```
```python
to
```
```python
usb_interface structures, one
```
```python
for
```
```python
each
 *  interface
```
```python
in
```
```python
the
```
```python
configuration.  The
```
```python
number
```
```python
of
```
```python
interfaces
```
```python
is
```
```python
stored
 *
```
```python
in
```
```python
desc.bNumInterfaces.  These pointers are valid only
```
```python
while
```
```python
the
```
```python
*
```
```python
the
```
```python
configuration
```
```python
is
```
```python
active.
 * @intf_cache: array
```
```python
of
```
```python
pointers
```
```python
to
```
```python
usb_interface_cache structures, one
 *
```
```python
for
```
```python
each interface
```
```python
in
```
```python
the
```
```python
configuration.  These structures exist
 *
```
```python
for
```
```python
the
```
```python
entire life
```
```python
of
```
```python
the
```
```python
device.
 * @extra: pointer
```
```python
to
```
```python
buffer containing all extra descriptors associated
 *
```
```python
with
```
```python
this configuration (those preceding
```
```python
the
```
```python
first
```
```python
interface
 *  descriptor).
 * @extralen:
```
```python
length
```
```python
of
```
```python
the
```
```python
extra descriptors buffer.
 *
 * USB devices may have multiple configurations,
```
```python
but
```
```python
only one can be active
 *
```
```python
at
```
```python
any
```
```python
time
```
```python
.  Each encapsulates a different operational environment;
 *
```
```python
for
```
```python
example, a dual-speed device would have separate configurations
```
```python
for
```
```python
* full-speed
```
```python
and
```
```python
high-speed operation.  The
```
```python
number
```
```python
of
```
```python
configurations
 * available
```
```python
is
```
```python
stored
```
```python
in
```
```python
the
```
```python
device descriptor
```
```python
as
```
```python
bNumConfigurations.
 *
 * A configuration can
```
```python
contain
```
```python
multiple interfaces.  Each corresponds
```
```python
to
```
```python
* a different function
```
```python
of
```
```python
the
```
```python
USB device,
```
```python
and
```
```python
all are available whenever
 *
```
```python
the
```
```python
configuration
```
```python
is
```
```python
active.  The USB standard says
```
```python
that
```
```python
interfaces
 * are supposed
```
```python
to
```
```python
be numbered
```
```python
from
```
```python
0
```
```python
to
```
```python
desc.bNumInterfaces-
```
```python
1
```
```python
,
```
```python
but
```
```python
a lot
 *
```
```python
of
```
```python
devices
```
```python
get
```
```python
this wrong.  In addition,
```
```python
the
```
```python
interface array
```
```python
is
```
```python
not
```
```python
* guaranteed
```
```python
to
```
```python
be sorted
```
```python
in
```
```python
numerical order.  Use usb_ifnum_to_if()
```
```python
to
```
```python
* look up an interface entry based
```
```python
on
```
```python
its
```
```python
number
```
```python
.
 *
 * Device drivers should
```
```python
not
```
```python
attempt
```
```python
to
```
```python
activate
```
```python
configurations.  The choice
 *
```
```python
of
```
```python
which configuration
```
```python
to
```
```python
install
```
```python
is
```
```python
a policy decision based
```
```python
on
```
```python
such
 * considerations
```
```python
as
```
```python
available power, functionality provided,
```
```python
and
```
```python
the
```
```python
user's
 * desires (expressed
```
```python
through
```
```python
userspace tools).  However, drivers can call
 * usb_reset_configuration()
```
```python
to
```
```python
reinitialize
```
```python
the
```
```python
current configuration
```
```python
and
```
```python
* all
```
```python
its
```
```python
interfaces.
 */
struct usb_host_config {
    struct usb_config_descriptor    desc;
    char *
```
```python
string
```
```python
;       /* iConfiguration
```
```python
string
```
```python
,
```
```python
if
```
```python
present */
    /* List
```
```python
of
```
```python
any Interface Association Descriptors
```
```python
in
```
```python
this
     * configuration. */
    struct usb_interface_assoc_descriptor *intf_assoc[USB_MAXIADS];
    /*
```
```python
the
```
```python
interfaces associated
```
```python
with
```
```python
this configuration,
     * stored
```
```python
in
```
```python
no particular order */
    struct usb_interface *interface[USB_MAXINTERFACES];
    /* Interface information available even when this
```
```python
is
```
```python
not
```
```python
the
```
```python
* active configuration */
    struct usb_interface_cache *intf_cache[USB_MAXINTERFACES];
    unsigned char *extra;   /* Extra descriptors */
    int extralen;
};
```

