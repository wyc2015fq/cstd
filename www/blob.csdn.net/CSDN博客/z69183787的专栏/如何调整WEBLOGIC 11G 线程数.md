# 如何调整WEBLOGIC 11G 线程数 - z69183787的专栏 - CSDN博客
2013年02月20日 14:29:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16554
在网上查到两到方法:
第一方法:直接在启动命令里加上.
-Dweblogic.threadpool.MinPoolSize=800 -Dweblogic.threadpool.MaxPoolSize=800
我是在setDomainEnv.sh 里的JAVA_OPTIONS中加的我公司机子上测试可以通过
第二方法:直接在配置文件中加上
1,在CONFIG.XML中的
   <server>
          <name>Adminserver</name>
          <self-tuning-thread-pool-size-min>1000</self-tuning-thread-pool-size-min>
          <self-tuning-thread-pool-size-max>1000</self-tuning-thread-pool-size-max> 
          ...
     </server>
2、在startweblogic.cmd脚本中加入一下java启动参数：
     -Dweblogic.configuration.schemaValidationEnabled=false (注我在11G上加不加这个上面配置的线程数都可以用)
我在我机子上测试通过.
