# vim故障及必会的处理方法（详细图解流程）-老男孩之永不止步-51CTO博客
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/34fe106ec1f906ba1b688ee63eac22a0.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/10937fe7e01093b2a54fdfeb47b64db3.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/6d6139f39d4271d0f01cf9c6d5105832.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/b733b7fc247f35ac929a4ec4ace1bd06.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
1、vim编辑文件时候，修改了内容，没有正常保存退出导致，重新打开会出现下面的故障提示
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/34e1f907e165e8df3f9b320e0af4de63.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
rm -rf 文件名，之前未保存的信息丢失
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/4349078b35a44dc249d1b93e213ed260.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2、恢复修改的内容
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/0b8be575b8355b5ad583acebfde9cbee.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/b44cd56e5975500f16b623d1c1b3a8ac.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/cb280bf676e6c07dbdc2e66b3e3fa033.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
按Enter------:q离开
vim -r  /tmp/xiyuxingxia.txt,恢复内容后:wq保存退出
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/18c914d6f5e0bb5a572050bea32d2d65.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/837e6c7f571bda034c8d983c42dfaf64.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/3a4c49e2a758ff7133f86434b9bf486a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
ls  -la /tmp
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/b7e2cccfc85b52d0d5ad5a626ea27a93.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
将.xiyuxingxia.txt.swp删除
![vim故障及必会的处理方法（详细图解流程）](https://s1.51cto.com/images/blog/201904/08/cbc97c7ceb5d9deaaffb6e0b9a62aee0.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
