# linux kernel i2c底层代码跟踪 - weixin_33985507的博客 - CSDN博客
2016年08月01日 23:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
其实跟上次跟的平台总线有关
在arch/arm/mach-mx6/board-mx6q_sabresd.c　文件中
```
1 static void __init mx6_sabresd_board_init(void)                                 
 2 {                                                                               
 3     ．．．
 4         strcpy(mxc_i2c0_board_info[0].type, "wm8962");                              
 5         mxc_i2c0_board_info[0].platform_data = &wm8962_config_data;             
 6                                                                                 
 7         imx6q_add_imx_i2c(0, &mx6q_sabresd_i2c_data);                               
 8         imx6q_add_imx_i2c(1, &mx6q_sabresd_i2c_data);                               
 9         imx6q_add_imx_i2c(2, &mx6q_sabresd_i2c_data);                               
10     ．．．
11 ｝
```
这里对３个i2c总线进行添加
看一下mx6q_sabresd_i2c_data　的原型
```
1 static struct imxi2c_platform_data mx6q_sabresd_i2c_data = {                    
2     //.bitrate = 100000,                                                        
3     .bitrate = 400000,                                                          
4 };
```
设置了i2c　比特率
进里面的代码：
```
1 #define imx6q_add_imx_i2c(id, pdata)    \                                       
2     imx_add_imx_i2c(&imx6q_imx_i2c_data[id], pdata)
```
里面是一个宏定义，很显然，这可能是另外一个人写的代码，里面有个结构体
```
1 extern const struct imx_imx_i2c_data imx6q_imx_i2c_data[] __initconst; 
```
1 struct imx_imx_i2c_data {                                                       
2     int id;                                                                     
3     resource_size_t iobase;                                                     
4     resource_size_t iosize;                                                     
5     resource_size_t irq;                                                        
6 };
```
```
再进imx_add_imx_i2c() ;
```
1 struct platform_device *__init imx_add_imx_i2c(                                 
 2         const struct imx_imx_i2c_data *data,                                    
 3         const struct imxi2c_platform_data *pdata)                               
 4 {                                                                               
 5     struct resource res[] = {                                                   
 6         {                                                                       
 7             .start = data->iobase,                                              
 8             .end = data->iobase + data->iosize - 1,                             
 9             .flags = IORESOURCE_MEM,                                            
10         }, {                                                                    
11             .start = data->irq,                                                 
12             .end = data->irq,                                                   
13             .flags = IORESOURCE_IRQ,                                            
14         },                                                                      
15     };                                                                          
16                                                                                 
17     return imx_add_platform_device("imx-i2c", data->id,                         
18             res, ARRAY_SIZE(res),                                               
19             pdata, sizeof(*pdata));                                             
20 }
```
