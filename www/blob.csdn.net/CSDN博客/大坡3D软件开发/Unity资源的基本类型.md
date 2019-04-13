
# Unity资源的基本类型 - 大坡3D软件开发 - CSDN博客

2019年02月16日 15:47:23[caimouse](https://me.csdn.net/caimouse)阅读数：122


资源的基本类型
图片文件
Unity支持大多数图片文件类型，比如BMP、TIF、TGA、JPG和PSD。如果你直接保存分层的Photoshop的PSD文件到Assets目录下面，unity会把这些文件当作简单的文件处理，并不会处理透明通道的属性，如果要使用alpha特性，需要使用别的方法进行导入。
FBX和模型文件
由于unity支持FBX的文件格式，可以从任何3D建模软件导出FBX的格式，这样模型就可以在unity里使用里了。当然，也可以使用原始的sketchup文件，unity也是支持导入这些的文件格式，比如使用3D建模软件保存为.max， .blend，.mb，.ma等原始格式，其实unity当在assets目录下发现这些文件时，也是调用相应的FBX转换插件来转换为FBX的格式。
网格和动画（Mesh和animation)
无论使用那一款建模软件，unity都支持从文件里导入网格和动画。可以只导入网格文件，也可以导入网格和动画一起的文件。
声音文件
如果放置一些未压缩的声音文件到assets目录，unity可以根据需要压缩率来设置压缩声音文件，并且进行导入进来。
[https://blog.csdn.net/caimouse/article/details/51749579](https://blog.csdn.net/caimouse/article/details/51749579)

