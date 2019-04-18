# ASOC驱动框架分析 - TaigaComplex求职中 - 博客园







# [ASOC驱动框架分析](https://www.cnblogs.com/TaigaCon/archive/2012/12/08/2808215.html)





　　  ASOC(ＡＬＳＡ　ｓｙｓｔｅｍ　ｏｎ　ｃｈｉｐ）。是由ｗｏｌｆｓｏｎ公司做的一个针对嵌入式移动设备的声音驱动的架构。支持三种不同的ＣＯＤＥＣ接口：ＡＣ９７（Ｉｎｔｅｌ），Ｉ２Ｓ（Ｐｈｉｌｉｐｓ），ＰＣＭ接口。ＡＳＯＣ在实现上区分了具体的平台和ＣＯＤＥＣ，从而使得同一个ＣＯＤＥＣ芯片可以在不同的体系结构中使用。而相关的与体系统结构相关的部分主要实现ＤＭＡ的传输和控制管理。

　　所谓ALSA，（Advansed Linux Sound Architecture，高级Linux声音架构），是Linux 2.6 版本内核的声音子系统。

　　2.4版本的声音子系统叫OSS（Open Sound System，开放声音系统），现以去除，不过为了兼容，ALSA会提供OSS的接口（设备节点）。



　　本驱动框架分析基于mini2440开发板，其中配备了片上系统s3c2440，声卡Uda1341。

　　分析包括的文件有：

　　　　　  arch\arm\mach-s3c2440\Mach-mini2440.c　　（平台设备）

　　　　　　sound\soc\s3c24xx\S3c24xx_uda134x.c　　（平台驱动）

　　　　　　sound\soc\Soc-core.c　　　　　　　　　　　　（ALSA主模块）

　　　　　　sound\soc\codecs\Uda134x.c　　　　　　　　（声音控制）

　　　　　　sound\soc\s3c24xx\S3c24xx-i2s.c　　　　　 （i2s初始化）

　　　　　　sound\soc\s3c24xx\S3c24xx-pcm.c　　　　   （音频码流传输）



**arch\arm\mach-s3c2440\Mach-mini2440.c**

文件设置了各种各样的mini2440平台设备，当我们需要注册平台驱动的时候，会去比对该文件中设备的名称跟我们自己驱动的设备名称，发现同名设备则调用驱动内的probe函数。

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


    static struct platform_device s3c24xx_uda134x = {
        .name = "s3c24xx_uda134x",
        .dev = {
            .platform_data    = &s3c24xx_uda134x_data,
        }
    };
    
    static struct s3c24xx_uda134x_platform_data s3c24xx_uda134x_data = {
        .l3_clk = S3C2410_GPB(4),
        .l3_data = S3C2410_GPB(3),
        .l3_mode = S3C2410_GPB(2),
        .model = UDA134X_UDA1341,
    };
```



**sound\soc\s3c24xx\S3c24xx_uda134x.c**

设置了跟Mach-mini2440.c中同名的驱动"s3c24xx_uda134x"

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


S3c24xx_uda134x.c    //平台驱动
    static struct platform_driver s3c24xx_uda134x_driver = {
        .probe  = s3c24xx_uda134x_probe,
        .remove = s3c24xx_uda134x_remove,
        .driver = {
            .name = "s3c24xx_uda134x",
            .owner = THIS_MODULE,
        },
    };
```



发现同名设备后调用probe函数

probe函数内部创建了名为“soc-audio”的声卡设备，然后用s3c24xx_uda134x_snd_devdata设置设备信息

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


s3c24xx_uda134x_probe(struct platform_device *pdev)
        s3c24xx_uda134x_setup_pin(s3c24xx_uda134x_l3_pins->l3_data,"data")    //初始化l3所用到的gpio
        s3c24xx_uda134x_setup_pin(s3c24xx_uda134x_l3_pins->l3_clk,"clk")
        s3c24xx_uda134x_setup_pin(s3c24xx_uda134x_l3_pins->l3_mode,"mode")

        s3c24xx_uda134x_snd_device = platform_device_alloc("soc-audio", -1);    //创建声卡dev（名字为soc-audio）
        platform_set_drvdata(s3c24xx_uda134x_snd_device,&s3c24xx_uda134x_snd_devdata);
        platform_device_add(s3c24xx_uda134x_snd_device);
```



s3c24xx_uda134x_snd_devdata是对整个声卡结构体的封装

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


static struct snd_soc_device s3c24xx_uda134x_snd_devdata = {
    .card = &snd_soc_s3c24xx_uda134x,
    .codec_dev = &soc_codec_dev_uda134x,
    .codec_data = &s3c24xx_uda134x,
};
```

其中的snd_soc_s3c24xx_uda134x联系起声卡驱动各个模块

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


static struct snd_soc_card snd_soc_s3c24xx_uda134x = {
        .name = "S3C24XX_UDA134X",
        .platform = &s3c24xx_soc_platform,        //S3c24xx-pcm.c,向用户空间提供pcm操作函数
        .dai_link = &s3c24xx_uda134x_dai_link,        //关联codec（uda134x）跟cpu（s3c24xx）
        .num_links = 1,
    };
```

dai（digital audio interface，数字音频接口），dai_link包含了两个dai接口：uda134x_dai 跟 s3c24xx_i2s_dai

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


    static struct snd_soc_dai_link s3c24xx_uda134x_dai_link = {
        .name = "UDA134X",
        .stream_name = "UDA134X",
        .codec_dai = &uda134x_dai,        //Uda134x.c,创建声卡设备
        .cpu_dai = &s3c24xx_i2s_dai,        //S3c24xx-i2s.c,初始化i2s
        .ops = &s3c24xx_uda134x_ops,
    };
```





**sound\soc\Soc-core.c**

开始创建声卡驱动，作为总线设备驱动框架下的一员，在发现同名设备“soc-audio”后，soc_driver中的probe函数会被调用

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


Soc-core.c        //声卡drv
    static struct platform_driver soc_driver = {
    .driver        = {
            .name        = "soc-audio",
            .owner        = THIS_MODULE,
            .pm        = &soc_pm_ops,
            },
        .probe        = soc_probe,
        .remove        = soc_remove,
        };
```

在probe函数内部会把新增的声卡snd_soc_s3c24xx_uda134x加入card链表card_list,然后对声卡链表的每个节点，进行platform（码流传输模块），dai_list（数字音频接口）比对，如果发现相同，则调用各自的probe函数（这里card没有probe函数）

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


    soc_probe(struct platform_device *pdev)
        struct snd_soc_device *socdev = platform_get_drvdata(pdev);
        struct snd_soc_card *card = socdev->card;
        ret = snd_soc_register_card(card);
            list_add(&card->list, &card_list);
            snd_soc_instantiate_cards();
                list_for_each_entry(card, &card_list, list)
                    snd_soc_instantiate_card(card);
                        list_for_each_entry(platform, &platform_list, list)
                            if (card->platform == platform) {
                            found = 1;
                            break;
                        }
                        list_for_each_entry(dai, &dai_list, list)
                            if (card->dai_link[i].cpu_dai == dai) {
                            found = 1;
                            break;
                        }
                        for (i = 0; i < card->num_links; i++) {
                            if (!card->dai_link[i].codec_dai->ops)
                                card->dai_link[i].codec_dai->ops = &null_dai_ops;
                        }
                        card->probe(pdev);
                        cpu_dai->probe(pdev, cpu_dai);        //s3c24xx_i2s_probe
                        codec_dev->probe(pdev);            //uda134x_soc_probe
                        platform->probe(pdev);
```



另外，Soc-core.c中也向外提供函数，如：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


    snd_soc_init_card(struct snd_soc_device *socdev)
        snd_card_register(codec->card);                        //位于sound\core\init.c
            device_create(sound_class, card->dev,MKDEV(0, 0), card,"card%i", card->number);        //创建设备节点
```



可以注意到上方的函数中，probe有四个，除了已经注册的一个card之外，还有另外三个

以下三个文件就是这三个probe所在的变量的本体





**Uda134x.c**

提供音量控制等的声音操作函数，并且向pcm提供声音操作函数

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


uda134x_init(void)
        snd_soc_register_dai(&uda134x_dai);        /*Soc-core.c提供该函数*/
            list_add(&dai->list, &dai_list);    //uda134x_dai加入dai_list
            snd_soc_instantiate_cards();
                list_for_each_entry(card, &card_list, list)
                    snd_soc_instantiate_card(card);
                        list_for_each_entry(platform, &platform_list, list)
                            if (card->platform == platform) {
                            found = 1;
                            break;
                        }
                        list_for_each_entry(dai, &dai_list, list)
                            if (card->dai_link[i].cpu_dai == dai) {
                            found = 1;
                            break;
                        }
                        for (i = 0; i < card->num_links; i++) {
                            if (!card->dai_link[i].codec_dai->ops)
                                card->dai_link[i].codec_dai->ops = &null_dai_ops;
                        }
                        card->probe(pdev);
                        cpu_dai->probe(pdev, cpu_dai);        //s3c24xx_i2s_probe
                        codec_dev->probe(pdev);            //uda134x_soc_probe
                        platform->probe(pdev);

    struct snd_soc_dai uda134x_dai = {
        .name = "UDA134X",
        /* playback capabilities */
        .playback = {
            .stream_name = "Playback",
            .channels_min = 1,
            .channels_max = 2,
            .rates = UDA134X_RATES,
            .formats = UDA134X_FORMATS,
        },
        /* capture capabilities */
        .capture = {
            .stream_name = "Capture",
            .channels_min = 1,
            .channels_max = 2,
            .rates = UDA134X_RATES,
            .formats = UDA134X_FORMATS,
        },
        /* pcm operations */
        .ops = &uda134x_dai_ops,
    };

    static struct snd_soc_dai_ops uda134x_dai_ops = {
        .startup    = uda134x_startup,
        .shutdown    = uda134x_shutdown,
        .hw_params    = uda134x_hw_params,
        .digital_mute    = uda134x_mute,
        .set_sysclk    = uda134x_set_dai_sysclk,
        .set_fmt    = uda134x_set_dai_fmt,
    };
```

另外就是该文件提供的probe函数是最终创建出声卡实例，使声卡能够运行于操作系统之上

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


    uda134x_soc_probe(struct platform_device *pdev)
        snd_soc_new_pcms(socdev, SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1);        //创建声卡pcm实例
            soc_new_pcm(socdev, &card->dai_link[i], i)                //S3c-core.c提供的实例创建函数
                snd_pcm_new(codec->card, new_name, codec->pcm_devs++, playback,capture, &pcm);
                snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &soc_pcm_ops);    //设置操作函数
                ret = platform->pcm_new(codec->card, codec_dai, pcm);        //platform = s3c24xx_soc_platform, pcm_new = s3c24xx_pcm_new,
                    s3c24xx_pcm_new(struct snd_card *card,struct snd_soc_dai *dai, struct snd_pcm *pcm)
                        s3c24xx_pcm_preallocate_dma_buffer(pcm,SNDRV_PCM_STREAM_PLAYBACK);    //分配DMA
        snd_soc_add_controls(codec, uda1341_snd_controls,ARRAY_SIZE(uda1341_snd_controls));    //将声卡与混频控制单元关联，控制音量
        snd_soc_init_card(socdev);            //Soc-core.c提供，提交声卡（创建设备节点）

    struct snd_soc_codec_device soc_codec_dev_uda134x = {
        .probe =        uda134x_soc_probe,
        .remove =       uda134x_soc_remove,
        .suspend =      uda134x_soc_suspend,
        .resume =       uda134x_soc_resume,
    };

    static struct snd_soc_device s3c24xx_uda134x_snd_devdata = {
        .card = &snd_soc_s3c24xx_uda134x,
        .codec_dev = &soc_codec_dev_uda134x,
        .codec_data = &s3c24xx_uda134x,
    };
    
    static const struct snd_kcontrol_new uda1340_snd_controls[] = {        //声音控制
    SOC_SINGLE("Master Playback Volume", UDA134X_DATA000, 0, 0x3F, 1),

    SOC_SINGLE("Tone Control - Bass", UDA134X_DATA001, 2, 0xF, 0),
    SOC_SINGLE("Tone Control - Treble", UDA134X_DATA001, 0, 3, 0),

    SOC_ENUM("Sound Processing Filter", uda134x_mixer_enum[0]),
    SOC_ENUM("PCM Playback De-emphasis", uda134x_mixer_enum[1]),

    SOC_SINGLE("DC Filter Enable Switch", UDA134X_STATUS0, 0, 1, 0),
    };
```





**S3c24xx-i2s.c**

进行soc的i2s端口的初始化，向pcm提供i2s操作接口，在probe函数内初始化i2s用到的GPIO

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


    s3c24xx_i2s_exit(void)
        snd_soc_register_dai(&s3c24xx_i2s_dai);        /*Soc-core.c提供该函数*/
            list_add(&dai->list, &dai_list);
            snd_soc_instantiate_cards();
                list_for_each_entry(card, &card_list, list)
                    snd_soc_instantiate_card(card);
                        list_for_each_entry(platform, &platform_list, list)
                            if (card->platform == platform) {
                            found = 1;
                            break;
                        }
                        list_for_each_entry(dai, &dai_list, list)
                            if (card->dai_link[i].cpu_dai == dai) {
                            found = 1;
                            break;
                        }
                        for (i = 0; i < card->num_links; i++) {
                            if (!card->dai_link[i].codec_dai->ops)
                                card->dai_link[i].codec_dai->ops = &null_dai_ops;
                        }
                        card->probe(pdev);
                        cpu_dai->probe(pdev, cpu_dai);        //s3c24xx_i2s_probe
                        codec_dev->probe(pdev);            //uda134x_soc_probe
                        platform->probe(pdev);

    struct snd_soc_dai s3c24xx_i2s_dai = {
        .name = "s3c24xx-i2s",
        .id = 0,
        .probe = s3c24xx_i2s_probe,
        .suspend = s3c24xx_i2s_suspend,
        .resume = s3c24xx_i2s_resume,
        .playback = {
            .channels_min = 2,
            .channels_max = 2,
            .rates = S3C24XX_I2S_RATES,
            .formats = SNDRV_PCM_FMTBIT_S8 | SNDRV_PCM_FMTBIT_S16_LE,},
        .capture = {
            .channels_min = 2,
            .channels_max = 2,
            .rates = S3C24XX_I2S_RATES,
            .formats = SNDRV_PCM_FMTBIT_S8 | SNDRV_PCM_FMTBIT_S16_LE,},
        .ops = &s3c24xx_i2s_dai_ops,
    };

    static struct snd_soc_dai_ops s3c24xx_i2s_dai_ops = {
        .trigger    = s3c24xx_i2s_trigger,
        .hw_params    = s3c24xx_i2s_hw_params,
        .set_fmt    = s3c24xx_i2s_set_fmt,
        .set_clkdiv    = s3c24xx_i2s_set_clkdiv,
        .set_sysclk    = s3c24xx_i2s_set_sysclk,
    };


    s3c24xx_i2s_probe(struct platform_device *pdev,struct snd_soc_dai *dai)
        /* Configure the I2S pins in correct mode */
        s3c2410_gpio_cfgpin(S3C2410_GPE0, S3C2410_GPE0_I2SLRCK);
        s3c2410_gpio_cfgpin(S3C2410_GPE1, S3C2410_GPE1_I2SSCLK);
        s3c2410_gpio_cfgpin(S3C2410_GPE2, S3C2410_GPE2_CDCLK);
        s3c2410_gpio_cfgpin(S3C2410_GPE3, S3C2410_GPE3_I2SSDI);
        s3c2410_gpio_cfgpin(S3C2410_GPE4, S3C2410_GPE4_I2SSDO);

        writel(S3C2410_IISCON_IISEN, s3c24xx_i2s.regs + S3C2410_IISCON);

        s3c24xx_snd_txctrl(0);
        s3c24xx_snd_rxctrl(0);
```





**S3c24xx-pcm.c**

向设备节点提供pcm操作函数ioctl等，后面会被封装成设备节点的fops，没有probe函数

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code


    struct snd_soc_platform s3c24xx_soc_platform = {
        .name        = "s3c24xx-audio",
        .pcm_ops     = &s3c24xx_pcm_ops,
        .pcm_new    = s3c24xx_pcm_new,
        .pcm_free    = s3c24xx_pcm_free_dma_buffers,
    };

    static struct snd_pcm_ops s3c24xx_pcm_ops = {
        .open        = s3c24xx_pcm_open,
        .close        = s3c24xx_pcm_close,
        .ioctl        = snd_pcm_lib_ioctl,
        .hw_params    = s3c24xx_pcm_hw_params,
        .hw_free    = s3c24xx_pcm_hw_free,
        .prepare    = s3c24xx_pcm_prepare,
        .trigger    = s3c24xx_pcm_trigger,
        .pointer    = s3c24xx_pcm_pointer,
        .mmap        = s3c24xx_pcm_mmap,
    };
```



总结：

![](https://pic002.cnblogs.com/images/2012/421096/2012120800441797.jpg)












