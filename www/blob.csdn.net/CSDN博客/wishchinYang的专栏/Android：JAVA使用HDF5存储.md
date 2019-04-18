# Android：JAVA使用HDF5存储 - wishchinYang的专栏 - CSDN博客
2017年11月01日 10:03:05[wishchin](https://me.csdn.net/wishchin)阅读数：925
        Hierarchical Data Format，可以存储不同类型的[图像](http://baike.baidu.com/view/42116.htm)和数码数据的文件格式，并且可以在不同类型的机器上传输，同时还有统一处理这种文件格式的函数库。大多数普通计算机都支持这种[文件格式](http://baike.baidu.com/view/1066926.htm)。
 HDF是用于存储和分发科学数据的一种自我描述、多对象文件格式。HDF是由美国国家超级计算应用中心（NCSA）创建的，以满足不同群体的科学家在不同工程项目领域之需要。
        关于HDF5的介绍，请查看HDF5的官方页面。和参考此文：[科学存储数据格式-HDF5](http://blog.csdn.net/wishchin/article/details/11826035)
        此外，HDF5提供了JAVA接口，下面是java使用HDF5的一些注意事项和相关代码。
1.Eclipse配置
        到hdf5的安装文件夹下面，把jarhdf5.jar, slf4j-api.jar，slf4j-simple.jar 添加到功能lib目录，同时把hdf5_java.dll文件添加到一个目录X。注意，在此过程中，添加且只添加这三个文件，第四个slf4j-nop-1.7.5.jar暂时不使用。
    HDF5为C++ native的第三方库，按照引入库的一般步骤加入Eclipse工程。windows->preference->java->userLIbraries->new。自行添加库的名字。我的名字是HDF5。
        然后在新建的用户库里面，Add JARS，添加jarhdf5.jar, slf4j-api.jar，slf4j-simple.jar三个文件，并在jarhdf5.jar下面配置Native library location，链接到X目录，用以调用Native动态库hdf5_java.dll。
        然后在工程的源码文件中，引入hdf5的包。
 例如：
```java
import hdf.hdf5lib.H5;
import hdf.hdf5lib.HDF5Constants;
```
2.读取HDF5文件
        初次使用，使用了C++写入的文件，测试读取，可运行代码如下：
```java
public static void main(String[] args) {
                String fname = "D:/dataSet/RecoSys/JavaTest/HDF5DatasetRead.h5";
		// private static
		String dsname = e = "training_data";
		try {
			// create the HDF5 file and add groups and dataset into the file
			//save_to_file(fname, dsname, dims2D);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		// 测试读出
		try {
			load_from_file(fname, dsname);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
}
```
load函数：
```java
public static int load_from_file(String fname, String dsname) throws Exception {
                //读取先后使用了fileID，SpaceID,DatasetID
		long file_id = -1;
		long space_id = -1;
		long dataset_id = -1;
		// Open file using the default properties.
		try {
			file_id = H5.H5Fopen(fname, HDF5Constants.H5F_ACC_RDWR, HDF5Constants.H5P_DEFAULT);
		} catch (Exception e) {
			e.printStackTrace();
		}
		// Open dataset using the default properties.
		try {
			if (file_id >= 0)
				dataset_id = H5.H5Dopen(file_id, dsname, HDF5Constants.H5P_DEFAULT);
		} catch (Exception e) {
			e.printStackTrace();
		}
		// Allocate array of pointers to two-dimensional arrays (theelements of the dataset.
		int l = 20;
		int w =20;
		long[] dims = {0,0};
		long[] dims2 = {0,0};
		//获取
		space_id = H5.H5Dget_space(dataset_id);
		H5.H5Sget_simple_extent_dims(space_id,dims,dims2);
		l= (int)dims[0];
		w= (int)dims[1];
		
		int[][] dataRead = new int[l][w];
		try {
			if (dataset_id >= 0)
				H5.H5Dread(dataset_id, HDF5Constants.H5T_NATIVE_INT, HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
						HDF5Constants.H5P_DEFAULT, dataRead);
		} catch (Exception e) {
			e.printStackTrace();
		}
		// print out the data values
		System.out.println("\n\nOriginal Data Values");
		for (int i = 0; i < l; i++) {
			System.out.print("\n" + dataRead[i][0]);
			for (int j = 1; j < w; j++) {
				System.out.print(", " + dataRead[i][j]);
			}
		}
}
```
实现把数据读取到dataRead中。
3.保存HDF5文件
..............待完成..................

