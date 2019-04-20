# 基于linux-2.6.38.8内核的SDIO/wifi驱动分析(转) - h13 - 博客园
1、sdio接口层解析
SDIO总线
**  SDIO总线 和 USB总线 类似**，SDIO也有两端，其中一端是HOST端，另一端是device端。**所有的 通信 都是 由HOST端 发送 命令 开始的**，Device端只要能解析命令，就可以相互通信。
  CLK信号:**HOST给DEVICE**的 时钟信号，每个时钟周期传输一个命令。
  CMD信号：**双向 的信号，用于传送 命令 和 反应**。
  DAT0-DAT3 信号:四条用于传送的数据线。
  VDD信号:电源信号。
  VSS1，VSS2:电源地信号。
SDIO热插拔原理：
     方法：设置一个 定时器检查 或 插拔中断检测
     硬件：假如GPG10（EINT18）用于SD卡检测
          GPG10 为高电平 即没有插入SD卡
          GPG10为低电平  即插入了SD卡
SDIO命令
      SDIO总线上都是HOST端发起请求，然后DEVICE端回应请求。**sdio命令由6个字节组成**。
1. Command:用于开始传输的命令，是由HOST端发往DEVICE端的。其中命令是通过CMD信号线传送的。
2. Response:回应是DEVICE返回的HOST的命令，作为Command的回应。也是通过CMD线传送的。
3. Data:数据是双向的传送的。可以设置为1线模式，也可以设置为4线模式。数据是通过DAT0-DAT3信号线传输的。
SDIO的每次操作都是由HOST在CMD线上发起一个CMD，对于有的CMD，DEVICE需要返回Response，有的则不需要。
       对于读命令，首先HOST会向DEVICE发送命令，紧接着DEVICE会返回一个握手信号，此时，当HOST收到回应的握手信号后，会将数据放在4位的数据线上，在传送数据的同时会跟随着CRC校验码。当整个读传送完毕后，HOST会再次发送一个命令，通知DEVICE操作完毕，DEVICE同时会返回一个响应。
       对于写命令，首先HOST会向DEVICE发送命令，紧接着DEVICE会返回一个握手信号，此时，当HOST收到回应的握手信号后，会将数据放在4位的数据线上，在传送数据的同时会跟随着CRC校验码。当整个写传送完毕后，HOST会再次发送一个命令，通知DEVICE操作完毕，DEVICE同时会返回一个响应。
sd命令格式
     以IO_SEND_OP_COND命令为例包含以下部分：
**  S（开始位） 总为0**
**     D（方向位） 1 从host到 device （0  从device到host）**
     命令索引：  通过值000101B来
**   填充位    0**
     IO_OCR      运转条件寄存器所支持的VDD的最小值和最大值
     CRC7        7位CRC校验数据
**   E（结束位）  总为1**
     MMC命令总共有40多个，分为class0 ~class7共8类，class0的所有卡必须支持。驱动程序通过发送cmd1、cmd41来区分sd卡和mmc卡，如果发送cmd1返回成功，则为mmc卡，否则发送cmd41返回成功，则为sd卡。
     cmd0   初始化mmc卡
 --------------------------------------
Sdio接口驱动
     首先我们来探讨几个重要的数据结构：该结果位于core核心层，主要用于 核心层与主机驱动层 的数据交换处理。/include/linux/mmc/**host.h**
struct mmc_host 用来描述 **卡控制器**
struct mmc_card 用来描述 **卡**
struct mmc_driver 用来描述 **mmc 卡驱动**
struct sdio_func  用来描述 **功能设备**
struct **mmc_host_ops** 用来描述 **卡控制器操作接口函数功能**，用于**从 主机控制器层向 core 层注册操作函数**，从而将core 层与具体的主机控制器隔离。也就是说 core 要操作主机控制器，就用这个 ops 当中给的函数指针操作，不能直接调用具体主控制器的函数。
**编写Host层驱动，可以参考S3C24XX的HOST驱动程序   **/drivers/mmc/host/s3cmci.c 
static struct platform_driver s3cmci_driver = {
     .driver  = {
         .name    = "s3c-sdi",  //名称和平台设备定义中的对应
         .owner   = THIS_MODULE,
         .pm  = s3cmci_pm_ops,
     },
     .id_table = s3cmci_driver_ids,
     .probe        = s3cmci_probe,  //平台设备探测接口函数
     .remove       = __devexit_p(s3cmci_remove),
     .shutdown = s3cmci_shutdown,
};
s3cmci_probe(struct platform_device *pdev)
{
//....
struct mmc_host *mmc;
mmc = mmc_alloc_host(sizeof(struct s3cmci_host), &pdev->dev);  //分配mmc_host结构体
//.....
}
/*注册中断处理函数s3cmci_irq,来处理数据收发过程引起的各种中断*/
request_irq(host->irq, s3cmci_irq, 0, DRIVER_NAME, host) //注册中断处理函数s3cmci_irq
/*注册中断处理s3cmci_irq_cd函数,来处理热拨插引起的中断，中断触发的形式为上升沿、下降沿触发*/
request_irq(host->irq_cd, s3cmci_irq_cd,IRQF_TRIGGER_RISING |IRQF_TRIGGER_FALLING, DRIVER_NAME, host)
mmc_add_host(mmc);  //initialise host hardware //**向MMC core注册host驱动**
----> device_add(&host->class_dev); //添加设备到mmc_bus_type总线上的设备链表中
----> mmc_start_host(host); //启动mmc host
 /*MMC drivers should call this when they detect a card has been inserted or removed.检测sd卡是否插上或移除*/
      ---->mmc_detect_change(host, 0);
         /*Schedule delayed work in the MMC work queue.调度延时工作队列*/
              mmc_schedule_delayed_work(&host->detect, delay);
搜索host->detected得到以下信息：
/drivers/mmc/core/host.c
INIT_DELAYED_WORK(&host->detect, mmc_rescan);
mmc_rescan(struct work_struct *work)
---->mmc_bus_put(host);//card 从bus上移除时，释放它占有的总线空间
/*判断当前mmc host控制器是否被占用,当前mmc控制器如果被占用,那么  host->claimed = 1;否则为0
*如果为1,那么会在while(1)循环中调用schedule切换出自己,当占用mmc控制器的操作完成之后,执行 *mmc_release_host()的时候,会激活登记到等待队列&host->wq中的其他程序获得mmc主控制器的使用权
*/
mmc_claim_host(host);
      mmc_rescan_try_freq(host, max(freqs[i], host->f_min)；
/**/
static int mmc_rescan_try_freq(struct mmc_host *host, unsigned freq)
{
     …………………………………………..
     /* Order's important: probe SDIO, then SD, then MMC */
     if (!mmc_attach_sdio(host))
         return 0;
     if (!mmc_attach_sd(host))
         return 0;
     if (!mmc_attach_mmc(host))
         return 0;
………………………………………….
}
mmc_attach_sdio(struct mmc_host *host)  //匹配sdio接口卡
--->mmc_attach_bus(host, &mmc_sdio_ops);
     /*当card与总线上的驱动匹配，就初始化card*/
mmc_sdio_init_card(host, host->ocr, NULL, 0); 
--->card = mmc_alloc_card(host, NULL);//分配一个card结构体
          mmc_set_bus_mode(host, MMC_BUSMODE_PUSHPULL); //设置mmc_bus的工作模式
struct sdio_func   *sdio_func[SDIO_MAX_FUNCS]; //SDIO functions (devices)
sdio_init_func(host->card, i + 1);
     --->func = sdio_alloc_func(card); //分配struct sdio_fun（sdio功能设备）结构体
          mmc_io_rw_direct（）;
          card->sdio_func[fn - 1] = func;
mmc_add_card(host->card);  //将具体的sdio设备挂载到mmc_bus_types 总线
sdio_add_func(host->card->sdio_func[i]); //将sdio功能设备挂载到sdio_bus_types总线
 附：mmc_card结构体中sdio_func[]赋值为分配的sdio_fun结构体，通过fun结构体与wireless驱动层建立联系
--------------------------------------------------------------------------------
mmc_attach_sd(struct mmc_host *host)  //匹配sd卡
---> mmc_sd_attach_bus_ops(host);
mmc_sd_init_card(host, host->ocr, NULL);//检测、初始化sd卡
mmc_add_card(struct mmc_card *card) // Register a new MMC card with the driver model.以驱动模式注册一个新mmc卡
mmc_claim_host(host);
mmc_attach_mmc(struct mmc_host *host)  //匹配mmc卡
---> mmc_attach_bus_ops(host);
     mmc_init_card(host, host->ocr, NULL);//检测、初始化mmc
     mmc_add_card(host->card);
     mmc_claim_host(host);
struct mmc_request {
     struct mmc_command *cmd;
     struct mmc_data    *data;
     struct mmc_command *stop;
     void *done_data;   /* completion data */
     void (*done)(struct mmc_request *);/* completion function */
};
/*将val写到addr地址*/
#define writel(val, addr) outl((val), (unsigned long)(addr))
/*从addr地址处，获取信息*/
#define readl(addr) inl((unsigned long)(addr))
/*发送请求*/
s3cmci_send_request(struct mmc_host *mmc)
---->struct mmc_request *mrq = host->mrq;
/* Clear command, data and fifo status registers
 *Fifo clear only necessary on 2440, but doesn't hurt on 2410
 */清除相应状态寄存器
     writel(0xFFFFFFFF, host->base + S3C2410_SDICMDSTAT);
     writel(0xFFFFFFFF, host->base + S3C2410_SDIDSTA);
     writel(0xFFFFFFFF, host->base + S3C2410_SDIFSTA);
---->s3cmci_setup_data(host, cmd->data);
        writel(0, host->base + S3C2410_SDIDCON); //写SDIDCON寄存器
           /* add to IMASK register */  添加相应中断
        imsk = S3C2410_SDIIMSK_FIFOFAIL | S3C2410_SDIIMSK_DATACRC |
            S3C2410_SDIIMSK_DATATIMEOUT | S3C2410_SDIIMSK_DATAFINISH;
        enable_imask(host, imsk);  //使能中断
        writel(0x0000FFFF, host->base + S3C2410_SDITIMER); //设置sditimer寄存器的状态
        writel(0xFF, host->base + S3C2410_SDIPRE);        //设置sdipre SDI预分频寄存器的状态
---->if (s3cmci_host_usedma(host))  //判断host的传送模式：dma（直接内存访问模式） 或 pio（CPU执行I/O端口指令来进行数据的读写的数据交换模式）
         res = s3cmci_prepare_dma(host, cmd->data);
     else
         res = s3cmci_prepare_pio(host, cmd->data);
          ---->s3cmci_prepare_dma(struct s3cmci_host *host, struct mmc_data *data)
              s3cmci_dma_setup(host, rw ? S3C2410_DMASRC_MEM : S3C2410_DMASRC_HW); //设置dma模式的属性
              s3c2410_dma_ctrl(unsigned int channel, enum s3c2410_chan_op op)     //对dma传输模式的进行相关控制函数
                   s3c2410_dma_start(chan);    //开始
                   s3c2410_dma_dostop(chan);   //停止
                   s3c2410_dma_flush(chan);    //刷新
                   s3c2410_dma_started(chan);
        ---->s3cmci_prepare_pio(host, cmd->data);
              do_pio_write(host);
enable_imask(host, S3C2410_SDIIMSK_TXFIFOHALF);  //使能Tx FIFO half interrupt 中断
---------------------------------------------------------------------------------------
/*host控制器发送命令*/
s3cmci_send_command(struct s3cmci_host *host,struct mmc_command *cmd)
---->writel(cmd->arg, host->base + S3C2410_SDICMDARG); //将命令写到sdicarg寄存器中
     writel(ccon, host->base + S3C2410_SDICMDCON);   //将ccon写到sdiccon（sdi控制寄存器）中
s3cmci_irq(int irq, void *dev_id)  //设置sdi传输的相关中断处理
---->mmc_signal_sdio_irq(host->mmc);  //sdio 中断信号
           wake_up_process(host->sdio_irq_thread); //唤醒
当插拔SDIO设备，会触发中断通知到CPU，然后执行卡检测中断处理函数在这个中断服务函数中，mmc_detect_change->mmc_schedule_delayed_work(&host->detect,delay), INIT_DELAYED_WORK(&host->detect, mmc_rescan)会调度mmc_rescan函数延时调度工作队列，这样也会触发SDIO设备的初始化流程，检测到有效的SDIO设备后，会将它注册到系统中去。
static irqreturn_t s3cmci_irq_cd(int irq, void *dev_id)
{
     struct s3cmci_host *host = (struct s3cmci_host *)dev_id;
     ........
     mmc_detect_change(host->mmc, msecs_to_jiffies(500));
     return IRQ_HANDLED;
}
2、wifi驱动解析
Drivers/net/wireless/libertas/if_sdio.c
Sdio设备的驱动由sdio_driver结构体定义，sdio_register_driver函数将该设备驱动挂载到sdio_bus_type总线上。
/* SDIO function device driver*/
struct sdio_driver {
     char *name;  //设备名
     const struct sdio_device_id *id_table; //设备驱动ID
     int (*probe)(struct sdio_func *, const struct sdio_device_id *);//匹配函数
     void (*remove)(struct sdio_func *);
     struct device_driver drv;
};
/*if_sdio.c*/
static struct sdio_driver if_sdio_driver = {
     .name         = "libertas_sdio",
     .id_table = if_sdio_ids,  //用于设备与驱动的匹配
     .probe        = if_sdio_probe,
     .remove       = if_sdio_remove,
     .drv = {
         .pm = &if_sdio_pm_ops,
     },
};
/**
 *   sdio_register_driver - register a function driver
 *   @drv: SDIO function driver
 */
int sdio_register_driver(struct sdio_driver *drv)
{
     drv->drv.name = drv->name;
     drv->drv.bus = &sdio_bus_type;  //设置driver的bus为sdio_bus_type
     return driver_register(&drv->drv);
}
static struct bus_type sdio_bus_type = {
     .name         = "sdio",
     .dev_attrs    = sdio_dev_attrs,
     .match        = sdio_bus_match,
     .uevent       = sdio_bus_uevent,
     .probe        = sdio_bus_probe,
     .remove       = sdio_bus_remove,
     .pm      = SDIO_PM_OPS_PTR,
};
注意：设备或者驱动注册到系统中的过程中，都会调用相应bus上的匹配函数来进行匹配合适的驱动或者设备，对于sdio设备的匹配是由sdio_bus_match和sdio_bus_probe函数来完成。
static int sdio_bus_match(struct device *dev, struct device_driver *drv)
{
     struct sdio_func *func = dev_to_sdio_func(dev);
     struct sdio_driver *sdrv = to_sdio_driver(drv);
     if (sdio_match_device(func, sdrv))
         return 1;
     return 0;
}
static const struct sdio_device_id *sdio_match_device(struct sdio_func *func,
     struct sdio_driver *sdrv)
{
     const struct sdio_device_id *ids;
     ids = sdrv->id_table;
              if (sdio_match_one(func, ids))
                   return ids;
}
由以上匹配过程来看，通过匹配id_table 和 sdio_driver设备驱动中id，来匹配合适的驱动或设备。最终会调用.probe函数，来完成相关操作。
If_sdio_probe函数分析
     Linux网络设备驱动中的重要数据结构：struct net_device 和  struct net_device_ops
     sdio_register_driver(&if_sdio_driver);  //注册sdio_driver结构体
card->workqueue = create_workqueue("libertas_sdio");  //创建工作队列
INIT_WORK(&card->packet_worker, if_sdio_host_to_card_worker);
card->model == fw_table[i].model //检测是否支持wifi卡
sdio_claim_host(func);
ret = sdio_enable_func(func); // enables a SDIO function for usage
                                     使能sdio功能设备
/* claim the IRQ for a SDIO function 注册中断*/
ret = sdio_claim_irq(func, if_sdio_interrupt);
priv = lbs_add_card(card, &func->dev);
----> wdev = lbs_cfg_alloc(dmdev); // 分配一个无线网络设备结构体，并初始化
      lbs_init_adapter(priv) // initialize adapter structure 初始化网络适配器
      dev = alloc_netdev(0, "wlan%d", ether_setup); //分配一个net_device结构体，并对其成员赋值
      SET_NETDEV_DEV(dev, dmdev);
      wdev->netdev = dev;
      priv->dev = dev;
      dev->netdev_ops = &lbs_netdev_ops;
      init_waitqueue_head(&priv->waitq); //初始化等待队列头
priv->main_thread = kthread_run(lbs_thread, dev, "lbs_main");//创建内核线程lbs_thread，它的重要功能为：It handles all events generated by firmware, RX data received from firmware and TX data sent from kernel.即通过固件产生收发事件，从固件接受数据包，发送数据包给kernel。
     ret = lbs_start_card(priv);
     /* gets the HW spec from the firmware and sets some basic parameters.*/
     ---->ret = lbs_setup_firmware(priv);// determined the firmware capabities.
设置fireware固件的功能
         lbs_cfg_register(priv) 
         ----> ret = register_netdev(priv->dev);//注册net_device网络设备
     priv->card = card;
     priv->hw_host_to_card = if_sdio_host_to_card;  //发送数据
     priv->enter_deep_sleep = if_sdio_enter_deep_sleep;
     priv->exit_deep_sleep = if_sdio_exit_deep_sleep;
     priv->reset_deep_sleep_wakeup = if_sdio_reset_deep_sleep_wakeup;
发送数据包
if_sdio_host_to_card（struct lbs_private *priv,u8 type, u8 *buf, u16 nb）
----> packet = kzalloc(sizeof(struct if_sdio_packet) + size,GFP_ATOMIC);
struct if_sdio_packet {
     struct if_sdio_packet  *next;
     u16           nb;
     u8            buffer[0] __attribute__((aligned(4)));  //缓冲区为4字节对齐
};
      memcpy(packet->buffer + 4, buf, nb);  //复制buf的数据到packet
      queue_work(card->workqueue, &card->packet_worker);
     static const struct net_device_ops lbs_netdev_ops = {
     .ndo_open          = lbs_dev_open,
     .ndo_stop     = lbs_eth_stop,
     .ndo_start_xmit        = lbs_hard_start_xmit, //数据包发送函数
     .ndo_set_mac_address   = lbs_set_mac_address,
     .ndo_tx_timeout    = lbs_tx_timeout,
     .ndo_set_multicast_list = lbs_set_multicast_list,
     .ndo_change_mtu        = eth_change_mtu,
     .ndo_validate_addr = eth_validate_addr,
};
lbs_hard_start_xmit（）//检测发送条件 和 启动数据包的发送
----> netif_stop_queue(priv->mesh_dev); //当发送队列为满或因其他原因来不及发送当前上层传送下来的数据包，则调用此函数阻止上层继续向网络设备驱动传递数据包。当数据包被发送完成后Tx结束的中断处理中，应该调用netif_wake_queue(priv->mesh_dev);唤醒被阻塞的上层，以启动继续向网络设备驱动传送数据包。
netdev_tx_t lbs_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
        ……………………………………
     netif_stop_queue(priv->dev);
     if (priv->tx_pending_len) {
         /* This can happen if packets come in on the mesh and eth
            device simultaneously -- there's no mutual exclusion on
            hard_start_xmit() calls between devices. */
         lbs_deb_tx("Packet on %s while busy\n", dev->name);
         ret = NETDEV_TX_BUSY;
         goto unlock;
     }
      …………………………….
         txpd = (void *)priv->tx_pending_buf;    //将发送buf的地址赋给txpd
     /*将要发送的数据拷贝到priv->tx_pending_buf缓冲区*/
memcpy(&txpd[1], p802x_hdr, le16_to_cpu(txpd->tx_packet_length));
unlock:
     wake_up(&priv->waitq);  //唤醒等待队列
}
     由以上函数得知，当发送队列为满或因其他原因来不及发送当前上层传递下来的数据包，就会调用netif_stop_queue阻止上层继续向网络设备驱动发送数据包。priv->tx_pending_len为skb中要发送数据的长度，当它不为0时，就会调用wake_up(&priv->waitq)来唤醒等待队列头&priv->waitq。
     搜索priv->waitq得知，priv->waitq等待队列头(drivers\net\wireless\libertas\main.c)  
init_waitqueue_head(&priv->waitq);
priv->main_thread = kthread_run(lbs_thread, dev, "lbs_main"); //创建内核线程lbs_thread
lbs_thread(void *data)  //kernel thread
---->add_wait_queue(&priv->waitq, &wait);  //将等待队列添加到wait等待队列中
     /* Execute the next command */
     if (!priv->dnld_sent && !priv->cur_cmd)
              lbs_execute_next_command(priv);  
     priv->hw_host_to_card(priv, MVMS_DAT,priv->tx_pending_buf,priv->tx_pending_len);
注意：hw_host_to_card函数将带txpd头的packet通过sdio接口发送到wifi芯片。搜索priv->hw_host_to_card得到drivers\net\wireless\libertas\If_sdio.c中的
priv->hw_host_to_card = if_sdio_host_to_card; 
if_sdio_host_to_card(struct lbs_private *priv,u8 type, u8 *buf, u16 nb)
----> packet = kzalloc(sizeof(struct if_sdio_packet) + size,GFP_ATOMIC); //分配套接字缓冲区
      memcpy(packet->buffer + 4, buf, nb); //复制数据到网络缓冲区（skb）
      queue_work(card->workqueue, &card->packet_worker);  //工作队列
     搜索card->packet_worker得到：card->workqueue = create_workqueue("libertas_sdio");
INIT_WORK(&card->packet_worker, if_sdio_host_to_card_worker);
if_sdio_host_to_card_worker(struct work_struct *work)
----> sdio_writesb(card->func, card->ioport,packet->buffer, packet->nb); //将io端口、sk_buff的数据包信息写到card->func中。
      ----> sdio_io_rw_ext_helper(func, 1, addr, 0, src, count); //function： Split an arbitrarily sized data transfer into several IO_RW_EXTENDED commands. 一个任意大小的数据传输分裂成几IO_RW_EXTENDED命令。
           ----> mmc_io_rw_extended(func->card, write,func->num, addr, incr_addr, buf,blocks, func->cur_blksize);
mmc_io_rw_extended(struct mmc_card *card, int write, unsigned fn,unsigned addr, int incr_addr, u8 *buf, unsigned blocks, unsigned blksz)
----> struct mmc_request mrq;
      struct mmc_command cmd;
      struct mmc_data data;
      mmc_wait_for_req(card->host, &mrq); //mmc_host等待响应请求
      ----> mmc_start_request(host, mrq); //开始执行请求
           ----> host->ops->request(host, mrq);
           wait_for_completion(&complete); //等待请求的完成
注意：struct mmc_host *host，struct mmc_host_ops *ops; void  (*request)(struct mmc_host *host, struct mmc_request *req);因为请求已经通过sdio接口发送到card上，所以我们可以联想到s3cmci.c函数中的struct mmc_host_ops s3cmci_ops 结构体中.request     = s3cmci_request成员。
static void s3cmci_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
     struct s3cmci_host *host = mmc_priv(mmc);
     host->status = "mmc request";
     host->cmd_is_stop = 0;
     host->mrq = mrq;
     /*发出一个请求前先要检测一下卡是否存在，否则提交到块设备层*/
     if (s3cmci_card_present(mmc) == 0) {
         dbg(host, dbg_err, "%s: no medium present\n", __func__);
         host->mrq->cmd->error = -ENOMEDIUM;
         mmc_request_done(mmc, mrq);  //若卡不存在，马上结束请求
     } else
         s3cmci_send_request(mmc);
}
由函数得知，s3cmci_card_present(mmc)判断卡是否存在，若存在则发送命令请求。
s3cmci_send_request(struct mmc_host *mmc)
/* Clear command, data and fifo status registersFifo clear only necessary on 2440, but doesn't hurt on 2410*/清空sdi命令状态寄存器、数据状态寄存器和fifo状态寄存器。
---->writel(0xFFFFFFFF, host->base + S3C2410_SDICMDSTAT);
     writel(0xFFFFFFFF, host->base + S3C2410_SDIDSTA);
     writel(0xFFFFFFFF, host->base + S3C2410_SDIFSTA);
     s3cmci_setup_data(host, cmd->data);  //进行数据请求处理设置，主要是数据控制寄存器
     ----> writel(dcon, host->base + S3C2410_SDIDCON);//将dcon写到sdidcon 数据控制寄存器中
     s3cmci_host_usedma(host)   //判断发送模式是dma还是pri
     mmc_request_done(mmc, mrq); //请求处理完成
     /*开始发送命令*/
     s3cmci_send_command(host, cmd);
     ----> writel(cmd->arg, host->base + S3C2410_SDICMDARG); //写sdi command argument register寄存器
         writel(ccon, host->base + S3C2410_SDICMDCON);  //写sdicmdcon命令控制寄存器
数据包接收
     网络设备接收数据的主要方法是由中断引发设备的中断处理函数，中断处理函数判断中断的类型，如果为接收中断，则读取接收到的数据，分配sk_buff数据结构和数据缓冲区，并将接收的数据复制到数据缓存区，并调用netif_rx()函数将sk_buff传递给上层协议。
搜索if_sdio_interrupt，可知道它是在if_sdio.c文件中if_sdio_probe()函数中sdio_claim_irq(func, if_sdio_interrupt) ，func->irq_handler = if_sdio_interrupt。当s3cmci_irq中断处理函数的S3C2410_SDIIMSK_SDIOIRQ 中断被触发时将调用if_sdio_interrupt()函数，进行接收数据。
#define S3C2410_SDIIMSK_SDIOIRQ   (1<<12)  由s3c2410芯片手册，sd host receives SDIO interrupter from the card (for SDIO) ,0 = disable 1 = interrupt enable.
/*中断处理函数，来处理数据收发过程引起的各种中断*/
s3cmci_irq()
----> mmc_signal_sdio_irq(host->mmc);  //中断信号
     ----> wake_up_process(host->sdio_irq_thread); //唤醒线程sdio_irq_thread
搜索sdio_irq_thread可知，Sdio_irq.c (drivers\mmc\core)
host->sdio_irq_thread = kthread_run(sdio_irq_thread, host, "ksdioirqd/%s",mmc_hostname(host));
sdio_irq_thread（）线程处理函数
----> process_sdio_pending_irqs(host->card); //挂起中断
      ----> if (func->irq_handler) {func->irq_handler(func);}
if_sdio_interrupt(struct sdio_func *func) //中断处理函数
----> if_sdio_card_to_host(card);  //host 控制器从card接收数据
static int if_sdio_card_to_host(struct if_sdio_card *card)
{
     size = if_sdio_read_rx_len(card, &ret);
     ret = if_sdio_wait_status(card, IF_SDIO_IO_RDY);
     type = card->buffer[2] | (card->buffer[3] << 8);
     ………………………………………………………………
     switch (type) {
     case MVMS_CMD:
         ret = if_sdio_handle_cmd(card, card->buffer + 4, chunk - 4);
     case MVMS_DAT:
         ret = if_sdio_handle_data(card, card->buffer + 4, chunk - 4);
     case MVMS_EVENT:
         ret = if_sdio_handle_event(card, card->buffer + 4, chunk - 4);
     }
         …………………………………………………….
}
     该函数主要完成向上层上交命令、数据、事件。
/*向上层上交数据包*/
if_sdio_handle_data(struct if_sdio_card *card,u8 *buffer, unsigned size)
----> skb = dev_alloc_skb(MRVDRV_ETH_RX_PACKET_BUFFER_SIZE + NET_IP_ALIGN); 分配sk_buff套接字缓冲区
      lbs_process_rxed_packet(card->priv, skb);
      ----> skb->protocol = eth_type_trans(skb, dev); //获取网络协议类型
           netif_rx(skb); //根据网络协议类型将数据包交给上层处理
-------------------------------------------------------------------------------------------
当sdio卡拔除时，驱动会调用该函数，完成相应操作。如释放占有的资源，禁止func功能函数，释放host。
if_sdio_remove(struct sdio_func *func)
---->lbs_stop_card(card->priv);
     lbs_remove_card(card->priv);
     ---->kthread_stop(priv->main_thread);  //终止内核线程
         lbs_free_adapter(priv);
          lbs_cfg_free(priv);
          free_netdev(dev);
     flush_workqueue(card->workqueue);  //刷新工作队列
     destroy_workqueue(card->workqueue);
     sdio_claim_host(func);
     sdio_release_irq(func);
     sdio_disable_func(func);
     sdio_release_host(func);
----------------------------------------------------------------------------------
自：[http://blog.csdn.net/wh_19910525/article/details/7392518](http://blog.csdn.net/wh_19910525/article/details/7392518)
