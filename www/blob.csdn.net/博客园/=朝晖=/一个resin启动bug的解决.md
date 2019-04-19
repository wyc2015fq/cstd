# 一个resin启动bug的解决 - =朝晖= - 博客园
# [一个resin启动bug的解决](https://www.cnblogs.com/dhcn/p/7116117.html)
这个bug的问题后来被确认为Resin所在目录层有中文目录名。
--------------------------------------------------------------------------------------------------------------------------
java.lang.NullPointerException 
at com.caucho.vfs.Jar.openReadImpl(Jar.java:504) 
at com.caucho.vfs.Jar.openReadImpl(Jar.java:470) 
at com.caucho.vfs.JarPath.openReadImpl(JarPath.java:211) 
at com.caucho.vfs.Path.openRead(Path.java:1140) 
at com.caucho.relaxng.CompactVerifierFactoryImpl.compileSchema(CompactVerifierFactoryImpl.java:89) 
at com.caucho.config.Config.findCompactSchema(Config.java:513) 
at com.caucho.config.Config.configure(Config.java:250) 
at com.caucho.server.resin.BootConfig.configureFile(BootConfig.java:131) 
at com.caucho.server.resin.Resin.configureFile(Resin.java:850) 
at com.caucho.server.resin.Resin.<init>(Resin.java:215) 
at com.caucho.server.resin.Resin.<init>(Resin.java:173) 
at com.caucho.server.resin.Resin.main(Resin.java:1233)

