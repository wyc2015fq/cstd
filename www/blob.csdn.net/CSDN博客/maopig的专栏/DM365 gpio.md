# DM365 gpio - maopig的专栏 - CSDN博客
2012年07月19日 00:13:35[maopig](https://me.csdn.net/maopig)阅读数：1720
davinci_cfg_reg（DM365_AEMIF_CLK_GIO50）;
 davinci_cfg_reg（DM365_AEMIF_ADV_GIO51）;
 err = gpio_request（LED4_ON_GIO50，"led4"）;
 if（err<0）
 {
  printk（"Cannot request gpio: ％d
"，LED4_ON_GIO50）;
  goto fail;
 }
 err = gpio_request（LED5_ON_GIO51，"led5"）;
 if（err<0）
 {
  printk（"Cannot request gpio: ％d
"，LED5_ON_GIO51）;
  goto fail;
 }
 gpio_direction_output（GPIO_TX， 0）;
 gpio_set_value（GPIO_TX， 0）;
