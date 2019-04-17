# .Net Framework Initialization Error – Unable to find a version of the runtime to run this applicatio - V__KING__的专栏 - CSDN博客





2013年12月17日 10:37:16[v__king__](https://me.csdn.net/V__KING__)阅读数：685








# 错误：.Net Framework Initialization Error – Unable to find a version of the runtime to run this application

查了一下,有人提供了这样的解决方法,试了一下,成功:

修改Exe的Config文件,一般是与exe同名,例如test.exe对应test.exe.config,在其中的<configuration>标签内添加如下内容:
<startup>
  <supportedRuntime version="v4.0.30319" />
</startup>
当然,具体的版本号根据实际情况而修改




如果文件夹下没有Config文件,自己创建一个也有效果,内容如下:
<configuration>
  <startup>
    <supportedRuntime version="v4.0.30319" />
  </startup>
</configuration>




