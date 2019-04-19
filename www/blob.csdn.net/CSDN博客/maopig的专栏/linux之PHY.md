# linux之PHY - maopig的专栏 - CSDN博客
2016年07月06日 14:34:28[maopig](https://me.csdn.net/maopig)阅读数：4520
一.结构体
1.PHY设备
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- struct phy_device {  
- struct phy_driver *drv; //PHY设备驱动
- struct mii_bus *bus;    //对应的MII总线
- struct device dev;  //设备文件
-     u32 phy_id; //PHY ID
- enum phy_state state;   //PHY状态
-     u32 dev_flags;  
-     phy_interface_t interface;  //PHY接口
- int addr;   //PHY 总线地址(0~31)
- int speed;  //速度
- int duplex; //双工模式
- int pause;  //停止
- int asym_pause; //
- int link;     
-     u32 interrupts; //中断使能标志
-     u32 supported;    
-     u32 advertising;  
- int autoneg;  
- int link_timeout;   //026
- int irq;    //中断号
- void *priv; //私有数据
- struct work_struct phy_queue;   //PHY工作队列
- struct delayed_work state_queue;    //PHY延时工作队列
-     atomic_t irq_disable;     
- struct mutex lock;  
- struct net_device *attached_dev;    //网络设备
- void (*adjust_link)(struct net_device *dev);  
- void (*adjust_state)(struct net_device *dev);  
- };  
2.PHY驱动
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- struct phy_driver {  
-     u32 phy_id;     //PHY ID
- char *name;     //PHY名
-     unsigned int phy_id_mask;  
-     u32 features;   //特性
-     u32 flags;  //标记
- int (*config_init)(struct phy_device *phydev);  //配置初始化
- int (*probe)(struct phy_device *phydev);    //探测到 probe方法
- int (*suspend)(struct phy_device *phydev);  //唤醒
- int (*resume)(struct phy_device *phydev);   //挂起
- int (*config_aneg)(struct phy_device *phydev);  //支援(Auto-negotiation)配置
- int (*read_status)(struct phy_device *phydev);  //读支援(Auto-negotiation)状态
- int (*ack_interrupt)(struct phy_device *phydev);    //清中断
- int (*config_intr)(struct phy_device *phydev);  //使能/禁用 中断
- int (*did_interrupt)(struct phy_device *phydev);    //判断是否由中断
- void (*remove)(struct phy_device *phydev);  //移除
- int  (*hwtstamp)(struct phy_device *phydev, struct ifreq *ifr); //时间戳处理
- bool (*rxtstamp)(struct phy_device *dev, struct sk_buff *skb, int type);    //接收时间戳
- void (*txtstamp)(struct phy_device *dev, struct sk_buff *skb, int type);    //发送时间戳
- struct device_driver driver;    //设备驱动文件
- };  
二.设备与驱动的注册函数
1.注册PHY设备
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- int phy_device_register(struct phy_device *phydev)  
- {  
- int err;  
- if (phydev->bus->phy_map[phydev->addr])    //判断PHY是否已经给注册了
- return -EINVAL;  
-     phydev->bus->phy_map[phydev->addr] = phydev;   //添加PHY到总线的phy_map里
-     phy_scan_fixups(phydev);    //执行匹配的fixups
-     err = device_register(&phydev->dev); //注册设备
- if (err) {  
-         pr_err("phy %d failed to register\n", phydev->addr);  
- goto out;  
-     }  
- return 0;  
-  out:  
-     phydev->bus->phy_map[phydev->addr] = NULL;  
- return err;  
- }  
- EXPORT_SYMBOL(phy_device_register);  
PHY的设备一般是动态注册的在注册之前一般会调用get_phy_device函数
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- struct phy_device * get_phy_device(struct mii_bus *bus, int addr)  
- {  
- struct phy_device *dev = NULL;  
-     u32 phy_id;  
- int r;  
-     r = get_phy_id(bus, addr, &phy_id); //获取PHY ID
- if (r)  
- return ERR_PTR(r);  
- if ((phy_id & 0x1fffffff) == 0x1fffffff)  
- return NULL;  
-     dev = phy_device_create(bus, addr, phy_id); //创建PHY设备
- return dev;  
- }  
- EXPORT_SYMBOL(get_phy_device);  
获取PHY ID
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- int get_phy_id(struct mii_bus *bus, int addr, u32 *phy_id)  
- {  
- int phy_reg;  
- //调用PHY的总线也就是mii总线的读方法获取PHY ID
-     phy_reg = bus->read(bus, addr, MII_PHYSID1); //获取PHYS ID1命令
- if (phy_reg < 0)  
- return -EIO;  
-     *phy_id = (phy_reg & 0xffff) << 16;  
-     phy_reg = bus->read(bus, addr, MII_PHYSID2); //获取PHYS ID1命令
- if (phy_reg < 0)  
- return -EIO;  
-     *phy_id |= (phy_reg & 0xffff);  
- return 0;  
- }  
- EXPORT_SYMBOL(get_phy_id);  
创建PHY设备
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- staticstruct phy_device* phy_device_create(struct mii_bus *bus,int addr, int phy_id)  
- {  
- struct phy_device *dev;  
-     dev = kzalloc(sizeof(*dev), GFP_KERNEL);    //分配phy设备内存
- if (NULL == dev)  
- return (struct phy_device*) PTR_ERR((void*)-ENOMEM);  
-     dev->dev.release = phy_device_release;  
-     dev->speed = 0;  //速度
-     dev->duplex = -1;    //双工模式
-     dev->pause = dev->asym_pause = 0;  
-     dev->link = 1;     
-     dev->interface = PHY_INTERFACE_MODE_GMII;    //接口模式GMII
-     dev->autoneg = AUTONEG_ENABLE;   //自动使能
-     dev->addr = addr; //地址
-     dev->phy_id = phy_id; //PHY ID
-     dev->bus = bus;  //mii总线
-     dev->dev.parent = bus->parent;    //父设备
-     dev->dev.bus = &mdio_bus_type;   //总线类型
-     dev->irq = bus->irq != NULL ? bus->irq[addr] : PHY_POLL;   //中断/轮询
-     dev_set_name(&dev->dev, PHY_ID_FMT, bus->id, addr);   //PHY 设备文件名
-     dev->state = PHY_DOWN;   //状态DOWN
-     mutex_init(&dev->lock);  
-     INIT_DELAYED_WORK(&dev->state_queue, phy_state_machine);    //初始化PHY状态机
-     request_module(MDIO_MODULE_PREFIX MDIO_ID_FMT, MDIO_ID_ARGS(phy_id));  
- return dev;  
- }  
2.注册PHY驱动
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- int phy_driver_register(struct phy_driver *new_driver)  
- {  
- int retval;  
-     new_driver->driver.name = new_driver->name;   //驱动名
-     new_driver->driver.bus = &mdio_bus_type; //总线类型
-     new_driver->driver.probe = phy_probe;    //探测函数
-     new_driver->driver.remove = phy_remove;  //移除函数
-     retval = driver_register(&new_driver->driver);   //注册设备驱动
- if (retval) {  
-         printk(KERN_ERR "%s: Error %d in registering driver\n",new_driver->name, retval);  
- return retval;  
-     }  
-     pr_debug("%s: Registered new driver\n", new_driver->name);  
- return 0;  
- }  
- EXPORT_SYMBOL(phy_driver_register);  
3.匹配
PHY设备和PHY驱动的总线类型都是mdio_bus_type
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- struct bus_type mdio_bus_type = {  
-     .name       = "mdio_bus",  
-     .match      = mdio_bus_match,   //匹配方法
-     .pm     = MDIO_BUS_PM_OPS,  
- };  
- EXPORT_SYMBOL(mdio_bus_type);  
匹配函数mdio_bus_match
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- staticint mdio_bus_match(struct device *dev, struct device_driver *drv)  
- {  
- struct phy_device *phydev = to_phy_device(dev); //获取PHY设备
- struct phy_driver *phydrv = to_phy_driver(drv); //获取PHY驱动
- return ((phydrv->phy_id & phydrv->phy_id_mask) ==(phydev->phy_id & phydrv->phy_id_mask));   //比较phy_id
- }  
匹配成功就会调用phy驱动的probe方法,也即是phy_probe
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- staticint phy_probe(struct device *dev)  
- {  
- struct phy_device *phydev;  
- struct phy_driver *phydrv;  
- struct device_driver *drv;  
- int err = 0;  
-     phydev = to_phy_device(dev);    //获取PHY设备
-     drv = get_driver(phydev->dev.driver);  
-     phydrv = to_phy_driver(drv);    //获取PHY驱动
-     phydev->drv = phydrv;    //捆绑一下
- if (!(phydrv->flags & PHY_HAS_INTERRUPT))    //设置中断方式
-         phydev->irq = PHY_POLL;  
-     mutex_lock(&phydev->lock);  
-     phydev->supported = phydrv->features; //设置PHY设备特性
-     phydev->advertising = phydrv->features;   //设置PHY设备特性
-     phydev->state = PHY_READY;   //状态设置为"准备好"
- if (phydev->drv->probe)   //如果驱动有probe方法
-         err = phydev->drv->probe(phydev); //则调用
-     mutex_unlock(&phydev->lock);  
- return err;  
- }  
三.初始化过程
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- staticint __init phy_init(void)  
- {  
- int rc;  
-     rc = mdio_bus_init();   //初始化mdio总线
- if (rc)  
- return rc;  
-     rc = phy_driver_register(&genphy_driver);   //注册通用的PHY设备驱动
- if (rc)  
-         mdio_bus_exit();  
- return rc;  
- }  
初始化过程主要是初始化mdio总线
接着注册通用的PHY设备驱动
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- staticstruct phy_driver genphy_driver = {  
-     .phy_id = 0xffffffff,  
-     .phy_id_mask = 0xffffffff,  
-     .name   = "Generic PHY",  
-     .config_init = genphy_config_init, //初始化函数
-     .features   = 0,  
-     .config_aneg = genphy_config_aneg, //配置 支援(Auto-negotiation)
-     .read_status = genphy_read_status, //读状态
-     .suspend    = genphy_suspend,  
-     .resume = genphy_resume,  
-     .driver = {.owner= THIS_MODULE, },  
- };  
初始化配置方法
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- staticint genphy_config_init(struct phy_device *phydev)  
- {  
- int val;  
-     u32 features;  
- //默认支持特性
-     features = (SUPPORTED_TP | SUPPORTED_MII| SUPPORTED_AUI | SUPPORTED_FIBRE |SUPPORTED_BNC);  
-     val = phy_read(phydev, MII_BMSR);   //读基础状态
- if (val < 0)  
- return val;  
- if (val & BMSR_ANEGCAPABLE) //支持(auto-negotiation)
-         features |= SUPPORTED_Autoneg;  
- if (val & BMSR_100FULL) //100兆全双工
-         features |= SUPPORTED_100baseT_Full;  
- if (val & BMSR_100HALF) //100兆半双工
-         features |= SUPPORTED_100baseT_Half;  
- if (val & BMSR_10FULL)  //10兆全双工
-         features |= SUPPORTED_10baseT_Full;  
- if (val & BMSR_10HALF)  //10兆半双工
-         features |= SUPPORTED_10baseT_Half;  
- if (val & BMSR_ESTATEN) {  
-         val = phy_read(phydev, MII_ESTATUS);    //读扩展状态
- if (val < 0)  
- return val;  
- if (val & ESTATUS_1000_TFULL)   //1000兆全双工
-             features |= SUPPORTED_1000baseT_Full;  
- if (val & ESTATUS_1000_THALF)   //1000兆半双工
-             features |= SUPPORTED_1000baseT_Half;  
-     }  
-     phydev->supported = features;    //PHY特性
-     phydev->advertising = features;  
- return 0;  
- }  
四.PHY状态机
1.状态分类
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- enum phy_state {  
-     PHY_DOWN=0,  
-     PHY_STARTING,   //开始
-     PHY_READY,  //准备好
-     PHY_PENDING,    //挂起
-     PHY_UP,     //开启
-     PHY_AN,     //判断连接状态中 negotiating
-     PHY_RUNNING,    //运行
-     PHY_NOLINK, //开启 未连接
-     PHY_FORCING,    //设置中
-     PHY_CHANGELINK, //连接状态改变
-     PHY_HALTED, //停止
-     PHY_RESUMING    //唤醒
- };  
2.状态机phy_state_machine
在phy_device_create函数中,开启了状态机
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- void phy_state_machine(struct work_struct *work)  
- {  
- struct delayed_work *dwork = to_delayed_work(work);  
- struct phy_device *phydev = container_of(dwork, struct phy_device, state_queue);  
- int needs_aneg = 0;  
- int err = 0;  
-     mutex_lock(&phydev->lock);  
- if (phydev->adjust_state)  
-         phydev->adjust_state(phydev->attached_dev);  
- switch(phydev->state) {  
- case PHY_DOWN:      //关闭((ifconfig eth0 down)
- case PHY_STARTING:  //开始
- case PHY_READY:     //准备好
- case PHY_PENDING:   //挂起
- break;  
- case PHY_UP:    //开启(ifconfig eth0 up)
-             needs_aneg = 1;  
-             phydev->link_timeout = PHY_AN_TIMEOUT;  
- break;  
- case PHY_AN:    //判断连接状态中 negotiating
-             err = phy_read_status(phydev);  
- if (err < 0)  
- break;  
- if (!phydev->link) {  
-                 phydev->state = PHY_NOLINK;  
-                 netif_carrier_off(phydev->attached_dev);  
-                 phydev->adjust_link(phydev->attached_dev);  
- break;  
-             }  
-             err = phy_aneg_done(phydev);  
- if (err < 0)  
- break;  
- if (err > 0) {  
-                 phydev->state = PHY_RUNNING;  
-                 netif_carrier_on(phydev->attached_dev);  
-                 phydev->adjust_link(phydev->attached_dev);  
- 
-             }   
- elseif (0 == phydev->link_timeout--) {  
- int idx;  
-                 needs_aneg = 1;  
- if (phydev->drv->flags & PHY_HAS_MAGICANEG)  
- break;  
-                 idx = phy_find_valid(0, phydev->supported);  
-                 phydev->speed = settings[idx].speed;  
-                 phydev->duplex = settings[idx].duplex;  
-                 phydev->autoneg = AUTONEG_DISABLE;  
-                 pr_info("Trying %d/%s\n", phydev->speed,DUPLEX_FULL ==phydev->duplex ?"FULL" : "HALF");  
-             }  
- break;  
- case PHY_NOLINK:    //开启 未连接
-             err = phy_read_status(phydev);  
- if (err)  
- break;  
- if (phydev->link) {  
-                 phydev->state = PHY_RUNNING;  
-                 netif_carrier_on(phydev->attached_dev);  
-                 phydev->adjust_link(phydev->attached_dev);  
-             }  
- break;  
- case PHY_FORCING:   //设置中
-             err = genphy_update_link(phydev);  
- if (err)  
- break;  
- if (phydev->link) {  
-                 phydev->state = PHY_RUNNING;  
-                 netif_carrier_on(phydev->attached_dev);  
-             }   
- else {  
- if (0 == phydev->link_timeout--) {  
-                     phy_force_reduction(phydev);  
-                     needs_aneg = 1;  
-                 }  
-             }  
-             phydev->adjust_link(phydev->attached_dev);  
- break;  
- case PHY_RUNNING:   //运行
- if (PHY_POLL == phydev->irq)  
-                 phydev->state = PHY_CHANGELINK;  
- break;  
- case PHY_CHANGELINK:    //连接状态改变
-             err = phy_read_status(phydev);  
- if (err)  
- break;  
- if (phydev->link) {  
-                 phydev->state = PHY_RUNNING;  
-                 netif_carrier_on(phydev->attached_dev);  
-             }   
- else {  
-                 phydev->state = PHY_NOLINK;  
-                 netif_carrier_off(phydev->attached_dev);  
-             }  
-             phydev->adjust_link(phydev->attached_dev);  
- if (PHY_POLL != phydev->irq)  
-                 err = phy_config_interrupt(phydev,PHY_INTERRUPT_ENABLED);  
- break;  
- case PHY_HALTED:    //停止
- if (phydev->link) {  
-                 phydev->link = 0;  
-                 netif_carrier_off(phydev->attached_dev);  
-                 phydev->adjust_link(phydev->attached_dev);  
-             }  
- break;  
- case PHY_RESUMING:  //唤醒
-             err = phy_clear_interrupt(phydev);  
- if (err)  
- break;  
-             err = phy_config_interrupt(phydev,PHY_INTERRUPT_ENABLED);  
- if (err)  
- break;  
- if (AUTONEG_ENABLE == phydev->autoneg) {  
-                 err = phy_aneg_done(phydev);  
- if (err < 0)  
- break;  
- if (err > 0) {  
-                     err = phy_read_status(phydev);  
- if (err)  
- break;  
- if (phydev->link) {  
-                         phydev->state = PHY_RUNNING;  
-                         netif_carrier_on(phydev->attached_dev);  
-                     }   
- else
-                         phydev->state = PHY_NOLINK;  
-                     phydev->adjust_link(phydev->attached_dev);  
-                 }   
- else {  
-                     phydev->state = PHY_AN;  
-                     phydev->link_timeout = PHY_AN_TIMEOUT;  
-                 }  
-             }  
- else {  
-                 err = phy_read_status(phydev);  
- if (err)  
- break;  
- if (phydev->link) {  
-                     phydev->state = PHY_RUNNING;  
-                     netif_carrier_on(phydev->attached_dev);  
-                 }   
- else
-                     phydev->state = PHY_NOLINK;  
-                 phydev->adjust_link(phydev->attached_dev);  
-             }  
- break;  
-     }  
-     mutex_unlock(&phydev->lock);  
- if (needs_aneg) //需要自动配置(例如ifconfig eth0 up就会调用)
-         err = phy_start_aneg(phydev);   //开始自动配置
- if (err < 0)  
-         phy_error(phydev);  
-     schedule_delayed_work(&phydev->state_queue, PHY_STATE_TIME * HZ);  
- }  
3.运行ifconfig eth0 up命令的过程
进入分支状态机分支
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- case PHY_UP:    //开启(ifconfig eth0 up)
-             needs_aneg = 1;  
-             phydev->link_timeout = PHY_AN_TIMEOUT;  
- break;  
相应处理
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- if (needs_aneg) //需要自动协商机制(例如ifconfig eth0 up就会调用)
-         err = phy_start_aneg(phydev);   //开始自动配置
调用phy_start_aneg函数
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- int phy_start_aneg(struct phy_device *phydev)  
- {  
- int err;  
-     mutex_lock(&phydev->lock);  
- if (AUTONEG_DISABLE == phydev->autoneg)  
-         phy_sanitize_settings(phydev);  
-     err = phydev->drv->config_aneg(phydev);   //调用驱动的config_aneg方法,默认是genphy_config_aneg
- if (err < 0)  
- goto out_unlock;  
- if (phydev->state != PHY_HALTED) {   //调整修改PHY设备状态
- if (AUTONEG_ENABLE == phydev->autoneg) {  
-             phydev->state = PHY_AN;  
-             phydev->link_timeout = PHY_AN_TIMEOUT;  
-         }   
- else {  
-             phydev->state = PHY_FORCING;  
-             phydev->link_timeout = PHY_FORCE_TIMEOUT;  
-         }  
-     }  
- out_unlock:  
-     mutex_unlock(&phydev->lock);  
- return err;  
- }  
- EXPORT_SYMBOL(phy_start_aneg);  
调用默认的自动协商方法genphy_config_aneg
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- int genphy_config_aneg(struct phy_device *phydev)  
- {  
- int result;  
- if (AUTONEG_ENABLE != phydev->autoneg)  
- return genphy_setup_forced(phydev);  
-     result = genphy_config_advert(phydev);  
- if (result < 0) /* error */
- return result;  
- if (result == 0) {  
- int ctl = phy_read(phydev, MII_BMCR);   //获取状态
- if (ctl < 0)  
- return ctl;  
- if (!(ctl & BMCR_ANENABLE) || (ctl & BMCR_ISOLATE))  
-             result = 1; /* do restart aneg */
-     }  
- if (result > 0)  
-         result = genphy_restart_aneg(phydev);   //重新开启自动协商机制
- return result;  
- }  
- EXPORT_SYMBOL(genphy_config_aneg);  
接着调用genphy_config_aneg
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- int genphy_restart_aneg(struct phy_device *phydev)  
- {  
- int ctl;  
-     ctl = phy_read(phydev, MII_BMCR);   //获取基本状态
- if (ctl < 0)  
- return ctl;  
-     ctl |= (BMCR_ANENABLE | BMCR_ANRESTART);    //使能自动协商机制及支援重启
- /* Don't isolate the PHY if we're negotiating */
-     ctl &= ~(BMCR_ISOLATE);  
-     ctl = phy_write(phydev, MII_BMCR, ctl); //写命令
- return ctl;  
- }  
- EXPORT_SYMBOL(genphy_restart_aneg);  
五.其他常用的api
**[cpp]**[view
 plain](http://blog.csdn.net/orz415678659/article/details/9370813#)[copy](http://blog.csdn.net/orz415678659/article/details/9370813#)
- staticinlineint phy_read(struct phy_device *phydev, u32 regnum);  //PHY读
- staticinlineint phy_write(struct phy_device *phydev, u32 regnum, u16 val);    //PHY写
- void phy_start(struct phy_device *phydev);  //PHY开始
- void phy_stop(struct phy_device *phydev);   //PHY停止
- int phy_init_hw(struct phy_device *phydev); //PHY初始化硬件
- struct phy_device * phy_attach(struct net_device *dev,constchar *bus_id, u32 flags, phy_interface_t interface);    //PHY接上
- void phy_detach(struct phy_device *phydev); //PHY分离
- struct phy_device *phy_find_first(struct mii_bus *bus); //查找mii_bus总线上第一个PHY
- int phy_connect_direct(struct net_device *dev, struct phy_device *phydev,void (*handler)(struct net_device *), u32 flags,phy_interface_t interface);    //PHY直接连接网络设备
- struct phy_device * phy_connect(struct net_device *dev, constchar *bus_id,void (*handler)(struct net_device *), u32 flags,phy_interface_t interface);  //PHY连接网络设备
- void phy_disconnect(struct phy_device *phydev); //PHY断开与网络设备的连接
- int phy_start_interrupts(struct phy_device *phydev);//PHY开始中断
- int phy_stop_interrupts(struct phy_device *phydev); //PHY停止中断
- int phy_ethtool_sset(struct phy_device *phydev, struct ethtool_cmd *cmd);   //ethtool工具sset功能
- int phy_ethtool_gset(struct phy_device *phydev, struct ethtool_cmd *cmd);   //ethtool工具gset功能
- int phy_mii_ioctl(struct phy_device *phydev,struct ifreq *ifr, int cmd);    //通用PHY/mii接口
- void phy_print_status(struct phy_device *phydev);   //PHY打印状态
原文见：[http://blog.csdn.net/orz415678659/article/details/9370813](http://blog.csdn.net/orz415678659/article/details/9370813)
