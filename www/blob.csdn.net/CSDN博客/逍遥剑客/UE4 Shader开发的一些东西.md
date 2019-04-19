# UE4 Shader开发的一些东西 - 逍遥剑客 - CSDN博客
2019年01月12日 16:37:58[xoyojank](https://me.csdn.net/xoyojank)阅读数：426
[http://blog.kiteandlightning.la/ue4-hlsl-shader-development-guide-notes-tips/](http://blog.kiteandlightning.la/ue4-hlsl-shader-development-guide-notes-tips/)
[https://zhuanlan.zhihu.com/p/36696626](https://zhuanlan.zhihu.com/p/36696626)
- Ctrl+Shift+.可以编译引擎Shader并重新载入, 但是对Plugins下的无效
- 如果要在RenderDoc里看到Shader代码, 需要打开r.ShaderDevelopmentMode=1, r.Shaders.Optimize=0, r.Shaders.KeepDebugInfo=1
- UE4 Material Editor里的CustomNode是可以写Shader代码的, 如果要想换行就先在记事本里写好再粘贴过去
- 自己写VertexShader时, SEMANTIC是不支持POSITION0, TEXCOORD0这种标记的, 只能使用ATTRIBUTE0, ATTRIBUTE1这种, 因为UE4在声明VertexDeclaration时是没有指定SEMANTIC的
- 还遇到一个坑, 会导致VS的out输出值为0
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112163446856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112163505606.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
把SV_POSITION放到VertexShader最后一个参数就好了, 坑了我好久…
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112163549126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112163556327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
