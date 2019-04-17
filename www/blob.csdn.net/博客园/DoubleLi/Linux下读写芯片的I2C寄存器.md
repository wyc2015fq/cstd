# Linux下读写芯片的I2C寄存器 - DoubleLi - 博客园






要想在Linux下读写芯片的I2C寄存器，一般需要在Linux编写一份该芯片的I2C驱动，关于Linux下如何编写I2C驱动，前一篇文章[《手把手教你写Linux I2C设备驱动》](http://ticktick.blog.51cto.com/823160/760020)已经做了初步的介绍，并且留下了两个疑问尚未解决，第一个是如何对Linux提供的I2C操作函数进行进一步封装，实现对芯片寄存器的读写；另一个是如何在用户空间调用该I2C驱动代码。本文将讨论前一个问题。



首先，我们要了解Linux系统提供的I2C操作函数怎么使用，上篇文章已经提到过，对I2C设备的读写，Linux系统提供了多种接口，这些接口可以在内核的 i2c.h 中找到，这里我主要介绍下面这组读写接口：


extern int i2c_master_send(struct i2c_client *,const char* ,int);    
   
extern int i2c_master_recv(struct i2c_client *,char* ,int);


第一个参数是 i2c_client 对象指针，第二个参数是要传输的数据buffer指针，第三个参数为buffer的大小。



接口函数已经有了，下面我们要解决的问题就是以何种形式/规则去使用这些接口才能正确地读写芯片的相关寄存器。



首先，我们需要查询芯片手册，找到芯片手册中，关于寄存器的I2C读写时序，其实，大多数芯片的I2C寄存器的读写时序都是一样的，下面我还是以手头的TVP5158芯片为例。



首先分析写操作，该芯片的手册中给出的I2C寄存器写时序图如下：



![](http://img1.51cto.com/attachment/201201/111307837.png)



从上图可以看出，真正需要执行写操作的有两处，Step4 和 Step6 ，Step4首先写入寄存器的偏移地址，而Step6则是写入到该寄存器的值。由此已经很清楚了，对于写I2C寄存器，我们需要做的就是给 i2c_master_send 函数传入两个字节的数据即可，第一个字节为寄存器的地址，第二个字节为要写入寄存器的数据。示例如下：


static int tvp5158_i2c_write( struct i2c_client* client,uint8_t reg,uint8_t data)  
{  
    unsigned char buffer[2];  
      
    buffer[0] = reg;  
    buffer[1] = data;  
      
    if( 2!= i2c_master_send(client,buffer,2) ) {  
        printk( KERN_ERR " tvp5158_i2c_write fail! \n" );  
        return -1;  
    }      
    return 0;  
}


其实挺简单的，没有什么复杂的代码。下面再看看读时序。



![](http://img1.51cto.com/attachment/201201/113244444.png)



由上图可以，读时序需要做的操作是，先向I2C总线上写入需要读的寄存器地址，然后读I2C总线上的值。代码写起来也不难，示例如下：


static int tvp5158_i2c_read( struct i2c_client* client,uint8_t reg,uint8_t *data)  
{  
    // write reg addr     
    if( 1!= i2c_master_send(client,®,1) ) {  
        printk( KERN_ERR " tvp5158_i2c_read fail! \n" );  
        return -1;  
    }      
    // wait  
    msleep(10);  
    // read  
    if( 1!= i2c_master_recv(client,data,1) ) {  
        printk( KERN_ERR " tvp5158_i2c_read fail! \n" );  
        return -1;  
    }      
      
    return 0;  
}


到此为止，Linux下具体如何封装读写芯片寄存器的方法已经介绍完毕，其实并不复杂，希望对初学者有所帮助，文中有什么讲述不正确的地方，欢迎留言或者来信[lujun.hust@gmail.com](mailto:lujun.hust@gmail.com)交流。









