# arcgis破解的时候，不能启动license manager的问题 - 毛小亮 - 博客园
# [arcgis破解的时候，不能启动license manager的问题](https://www.cnblogs.com/xianerwonder/p/4436319.html)
1.防火墙没关；（非常重要）
2.win+R，调出控制台，输入services.msc.然后手动开启ArcGIS license manager服务，关闭其余类似erdas,matlab影响该服务的开启服务进程;
3.service.txt中keygen生成的this_host一定要改为localhost。

