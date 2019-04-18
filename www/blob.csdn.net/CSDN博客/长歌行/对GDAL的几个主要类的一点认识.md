# 对GDAL的几个主要类的一点认识 - 长歌行 - CSDN博客





2012年02月01日 16:10:02[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：849标签：[存储																[delete																[date																[xml](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
个人分类：[图形图像](https://blog.csdn.net/iuhsihsow/article/category/1065735)





## GDALMajorObject


所有核心类的父类是GDALMajorObject，它定义了一些操作元数据的属性和方法供子类继承。

元数据：data about date 描述数据的数据，majorobject使用GDALMultiDomainMetadata对象存储元数据。该对象存在一个域名列表和一个元数据内容列表。用户传入域名以及元数据内容已进行数据设置。可序列化为xml。






## GDALDriverManager



管理GDALDriver的类，存储了GDALDriver的数目以及列表。

可以通过序号以及名称来获取处理对象数据对应的driver。

使用一个全局变量staticvolatile GDALDrvierManager poDM管理，在register数据驱动时，通过GetGDALDriverManage这个全局函数，新建或获取，然后通过RegisterDriver添加对应驱动。






## GDALDriver


数据驱动类，只定义了成员方法，没有成员变量，通过函数指针来实现对于不同的驱动类型采用不同的方式。

有Create方法创建数据集，Delete删除数据集，Rename重命名数据集，CopyFile，CreateCopy

另外定义了其他函数指针，算是接口了。
在注册的时候，根据对象类型创建driver，并设置driver的描述，元数据等，并设置函数指针为对应数据集的静态函数。如poDriver->pfnOpen= JDEMDataset::Open。其中Open方法为JDEMDataset中的静态函数。



## GDALDataSet


一套关联栅格波段，通常来自一个文件。
数据成员包括GDALDriver指针，波段的数目大小，波段列表，引用计数，是否被共享。等信息。以及一个GDALDefaultOverViews对象。提供一个IRasterIO接口给派生类使用，作为读取数据的接口。






## GDALRasterBand


一个单一的栅格数据波段

存在一个GDALRasterBlock友元

存在一个GDALDataset对象（GDAL貌似很喜欢把外层对象的指针存在分类的对象里面。如在学生的信息里面存储班级的信息，班级的信息里面要有一个年级的指针，当然，班级类是学生类的友元，年级类是班级类的友元。难道是学生犯错了，然后由班级处置的意思？）

Rasterband中定义了比rasterdataset更细致的数据类型。



最后来一个很熟悉的图：






![](http://hi.csdn.net/attachment/201202/1/0_1328084219b46y.gif)](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)




