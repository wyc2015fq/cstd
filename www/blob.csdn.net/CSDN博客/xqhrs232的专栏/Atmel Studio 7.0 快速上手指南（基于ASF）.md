# Atmel Studio 7.0 快速上手指南（基于ASF） - xqhrs232的专栏 - CSDN博客
2019年04月01日 17:46:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：31
原文地址::[https://blog.csdn.net/hubinbin595959/article/details/79150027](https://blog.csdn.net/hubinbin595959/article/details/79150027)
相关文章
1、Atmel Studio 7.0 安装说明----[https://jingyan.baidu.com/article/93f9803f02e48ce0e46f5535.html](https://jingyan.baidu.com/article/93f9803f02e48ce0e46f5535.html)
2、AVR Studio简易入门教程----[https://wenku.baidu.com/view/b65551fb0029bd64783e2ce5.html](https://wenku.baidu.com/view/b65551fb0029bd64783e2ce5.html)
3、[https://wenku.baidu.com/view/7015f90a7cd184254b353503.html](https://wenku.baidu.com/view/7015f90a7cd184254b353503.html)
就在最近，Atmel终于推出了新版本IDE——Atmel Studio 7.0，该版本采用了微软最新的 Visual Studio 2015 平台，在速度、性能和代码视觉风格上都体现的淋淋尽致，用起来非常顺手，下面将结合实例，介绍[Atmel](http://microchip.eefocus.com/tag/term/Atmel/m/article)Studio 7.0 的使用方法。  
      第1步：打开Atmel Studio 7.0
![1.1](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326330.png)
       第2步：新建工程项目
![2.2](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326329.png)
![2.3](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326331.png)
       第3步：选择芯片型号
![3](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326332.png)
![3.3](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326333.png)
       第4步：添加[ASF](http://microchip.eefocus.com/tag/term/ASF/m/article) 驱动库
![4](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326334.png)
![4.4](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326336.png)
![4.5](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326335.png)
       第5步：查看ASF驱动使用说明
![5](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326337.png)
![5,6](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326338.png)
       第6步：编写代码
![6](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326339.png)
源码：
        #include <asf.h> //包含Atmel MCU软件库头文件
        #define LED                PIN_PC27 //定义LED所使用的IO口为 PC27
        #define KEY                PIN_PC01 //定义KEY所使用的IO口为 PC01
        void port_init(void); //声明函数体
        /************************************************************************/
        //* IO 初始化                                                                     
        /************************************************************************/
        void port_init(void)
        {
                struct port_config config_port_pin;
                port_get_config_defaults(&config_port_pin);
                config_port_pin.direction  = PORT_PIN_DIR_OUTPUT; //配置IO口方向为输出
                port_pin_set_config(LED, &config_port_pin); //初始化LED对应IO口
                config_port_pin.direction  = PORT_PIN_DIR_INPUT; //配置IO口方向为输入
                config_port_pin.input_pull = PORT_PIN_PULL_UP; //配置IO口上拉
                port_pin_set_config(KEY, &config_port_pin); //初始化KEY对应IO口
        }
        /************************************************************************/
        //* 主程序
        /************************************************************************/
        int main (void)
        {
                system_init(); //系统初始化
                /* Insert application code here, after the board has been initialized. */
                port_init(); //IO初始化
                while(1)
                {
                        if (port_pin_get_input_level(KEY) == 0) //KEY按下，LED对应IO电平 = 0
                        {
                                port_pin_set_output_level(LED, 0);
                        }
                        else
                        {
                                port_pin_set_output_level(LED, 1);
                        }
                }
        }
       第7步：编译并生产烧录文件
![7](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326341.png)
![7.2](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326340.png)
       第8步：烧录文件到开发板
![8](http://upload.semidata.info/sns.eefocus.com/atmel/article/media/2016/07/08/326342.png)
程序包下载及安装说明：[http://atmel.eefocus.com/module/forum/thread-4617-1-1.html](http://atmel.eefocus.com/module/forum/thread-4617-1-1.html)
更多Atmel及科技资讯请关注：  
Atmel中文官网：[http://www.atmel.com/zh/cn/](http://www.atmel.com/zh/cn/)
Atmel技术论坛：[http://atmel.eefocus.com/](http://atmel.eefocus.com/)
Atmel中文博客：[http://blog.sina.com.cn/u/2253031744](http://blog.sina.com.cn/u/2253031744)
Atmel新浪微博：[http://www.weibo.com/atmelcn](http://www.weibo.com/atmelcn)
