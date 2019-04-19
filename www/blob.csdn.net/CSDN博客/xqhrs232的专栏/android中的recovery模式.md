# android中的recovery模式 - xqhrs232的专栏 - CSDN博客
2012年03月27日 14:39:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1089标签：[android																[cmd																[file																[command																[patch																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=patch&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/armlinux_fans/article/details/6124450](http://blog.csdn.net/armlinux_fans/article/details/6124450)
这里使用基于freescale的imx_r9.2 patch的android源码分析.
android中的recovery模式，作为android的开发者或者用户都应该清楚和了解。android利用recovery模式完成整个系统的升级和恢复出厂设置。
这里先讲讲怎么进入recovery模式，这个工作是由bootloader完成。freescale使用的uboot.
bootloader每次启动的时候都要执行./lib_arm/board.c的代码：
   464    #ifdef CONFIG_ANDROID_RECOVERY
   465        check_recovery_mode();
   466    #endif
该函数位于./board/freescale/common/recovery.c，代码如下：
    98    /* export to lib_arm/board.c */
    99    void check_recovery_mode(void)
   100    {
   101        if (check_key_pressing())
   102            setup_recovery_env();
   103        else if (check_recovery_cmd_file()) {
   104            puts("Recovery command file founded!/n");
   105            setup_recovery_env();
   106        }
   107    }
line 101和line 103指出有两种方式可以执行setup_recovery_env()进入recovery模式。
我们接下来看看这三个函数到底干了些什么，
check_key_pressing也在board/freescale/common/recovery.c中，
    32    int check_key_pressing(void)
    33    {
...
    38    
    39        puts("Detecting HOME+POWER key for recovery .../n");
...
    45    
    46        for (i = 0; i < keys; i++) {
    47            if (test_key(CONFIG_POWER_KEY , &key_info[i]))
    48                state |= PRESSED_HOME;
    49            else if (test_key(CONFIG_HOME_KE Y, &key_info[i]))
    50                state |= PRESSED_POWER;
    51        }
...
    55        if ((state & RECOVERY_KEY_MASK) == RECOVERY_KEY_MASK)
    56            return 1;
    57    
    58        return 0;
    59    }
该函数用于探测HOME+POWER键，若同时按下，函数返回1，进入recovery模式。
   893    int check_recovery_cmd_file(void)
   894    {
...
   938                filelen = ext2fs_open(CONFIG_ANDROID_RECOVERY_CMD_FILE );
...
   955        return (filelen > 0) ? 1 : 0;
   956    
   957    }
 该函数探测宏CONFIG_ANDROID_RECOVERY_CMD_FILE，表示的文件是否存在，若存在返回1，进入recovery模式。
该宏定义在./include/configs/mx51_bbg_android.h
  115    #define CONFIG_ANDROID_RECOVERY_CMD_FILE "/recovery/command"
接下来看看setup_recovery_env()函数，做了些什么事情。
    70    void setup_recovery_env(void)
    71    {
    72        char *env, *boot_args, *boot_cmd;
    73        int bootdev = get_boot_device();
    74    
   75        boot_cmd = supported_reco_envs[bootdev].cmd;
    76        boot_args = supported_reco_envs[bootdev].args;
    77    
    78        if (boot_cmd == NULL) {
    79            printf("Unsupported bootup device for recovery/n");
    80            return;
    81        }
    82    
    83        printf("setup env for recovery../n");
    84    
    85        env = getenv("bootargs_android_recovery");
    86        /* Set env to recovery mode */
    87        /* Only set recovery env when these env not exist, give user a
    88         * chance to change their recovery env */
    89        if (!env)
90            setenv("bootargs_android_recovery", boot_args);
    91    
    92        env = getenv("bootcmd_android_recovery");
    93        if (!env)
 94            setenv("bootcmd_android_recovery", boot_cmd);
    95        setenv("bootcmd", "run bootcmd_android_recovery");
    96    }
该函数主要用来设置bootargs_android_recovery和bootcmd_android_recovery环境变量，我们看看这两个环境变量到底设置成了什么，
请看代码：board/freescale/mx51_bbg/mx51_bbg.c
   878    struct reco_envs supported_reco_envs[BOOT_DEV_NUM] = {
...
   888         .cmd = CONFIG_ANDROID_RECOVERY_BOOTCMD_MMC ,
889         .args = CONFIG_ANDROID_RECOVERY_BOOTARGS_MMC,
...
   891    };
上面line 888和line 889的宏定义在include/configs/mx51_bbg_android.h:
   109    #define CONFIG_ANDROID_RECOVERY_BOOTARGS_MMC /
   110        "setenv bootargs ${bootargs} init=/init 
root=/dev/mmcblk0p4 " /
   111        "rootfs=ext4 di1_primary"
   112    #define CONFIG_ANDROID_RECOVERY_BOOTCMD_MMC  /
   113        "run bootargs_base bootargs_android_recovery ;"    /
   114        "mmc read 0 ${loadaddr} 0x800 0x1800;bootm"
看到这里终于了解了，到底怎么进入recovery模式，实际上就是通过bootloader传递参数给内核，让kernel挂载recovery分区便启动了recovery模式。
