# Gtk：利用alignment控制button大小 - Koma Hub - CSDN博客
2017年09月06日 22:00:45[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：443
 将button写入一个alignment里面，在对alignment进行操作 
```cpp
button=gtk_button_new_with_label("...");
align = gtk_alignment_new(0,0.5,0,0);  
gtk_container_add(GTK_CONTAINER(align),button);  
g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(select_file),_vel);
gtk_table_attach_defaults(GTK_TABLE(table),align,5,6,0,1);
```
结果一目了然：
处理前结果：
![](https://img-blog.csdn.net/20170906220139652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
处理后：
![](https://img-blog.csdn.net/20170906220147984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
