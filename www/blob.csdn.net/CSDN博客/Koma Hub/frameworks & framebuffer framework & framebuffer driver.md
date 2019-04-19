# frameworks & framebuffer framework & framebuffer driver - Koma Hub - CSDN博客
2019年02月28日 22:44:16[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：21
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
**Table of Contents**
[Example of frameworks](#Example%20of%20frameworks)
[Example of the framebuffer framework](#Example%20of%20the%20framebuffer%20framework)
[Skeleton example](#Skeleton%20example)
# Example of frameworks
![](https://img-blog.csdnimg.cn/2019022822400996.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# Example of the framebuffer framework
- Kernel option **CONFIG_FB**
- Implemented in drivers/video/
- fb.c, fbmem.c, fbmon.c, fbcmap.c, fbsysfs.c, modedb.c, fbcvt.c
- Implements a single character driver (through **file_operations**), registers the major number and allocates minors, denes and implements the user/kernel API
- First part of **include/linux/fb.h**
- Denes the set of operations a framebuffer driver must implement and helper functions for the drivers
- struct **fb_ops**
- Second part of include/linux/fb.h
- Must implement some or all operations dened in struct fb_ops. Those operations are framebuffer-specic.
- **xxx open(), xxx read(), xxx write(), xxx release(), xxx checkvar(), xxx setpar(), xxx setcolreg(), xxx blank(), xxx pan display(), xxx fillrect(), xxx copyarea(), xxx imageblit(), xxx cursor(), xxx rotate(), xxx sync(), xxx get caps()**, etc.
- Must allocate a fb info structure with framebuffer alloc(), set the ->fbops eld to the operation structure, and register the framebuffer device with register framebuffer()
# Skeleton example
```cpp
static int xxx_open(struct fb_info *info, int user) {}
static int xxx_release(struct fb_info *info, int user) {}
static int xxx_check_var(struct fb_var_screeninfo *var, struct fb_info *info) {}
static int xxx_set_par(struct fb_info *info) {}
static struct fb_ops xxx_ops = {
    .owner = THIS_MODULE,
    .fb_open = xxxfb_open,
    .fb_release = xxxfb_release,
    .fb_check_var = xxxfb_check_var,
    .fb_set_par = xxxfb_set_par,
    [...]
};
init()
{
    struct fb_info *info;
    info = framebuffer_alloc(sizeof(struct xxx_par), device);
    info->fbops = &xxxfb_ops;
    [...]
    register_framebuffer(info);
}
```
> 
Thomas Petazzoni
Free Electrons
thomas.petazzoni@free-electrons.com
