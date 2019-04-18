# GDAL实现遥感影像数据压缩 - weixin_33985507的博客 - CSDN博客
2018年05月25日 22:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：65
## 一.目的
压缩，其实一开始我是拒绝的，因为我觉得意义有限,尤其是在数据量比较小的MODIS上，特别是经过格网切分之后。。。
![8947186-ae667e8e51b35d56.png](https://upload-images.jianshu.io/upload_images/8947186-ae667e8e51b35d56.png)
压缩后反而变大了有木有
但是放在同样大小的高分影像上，压缩就很有意义了
![8947186-9310a999ccdb2d5f.png](https://upload-images.jianshu.io/upload_images/8947186-9310a999ccdb2d5f.png)
数据量明显下降
其实最主要的原因是师兄的压迫。。。。
## 二.过程
大家都知道，作为一个开发者，我们很多时候是拒绝使用AE做二次开发的（显得自己超没有逼格）。一般情况下我们都使用GDAL（666）。可我懵逼的看了半天API，悲哀的发现居然特么的没有相应的API接口，***。然后我就去二笔呵呵看了LZW算法原理，气势汹汹的准备自己写一波算法。但这时我发现一个问题，那就是根据LZW算法，压缩时要有一个对应的码表，然后解压时去读对应的码表，嗯，这就是问题，我特么自己弄得码表好像只有我自己能明白吧，我还能给GDAL塞进去不成。。。
没错，我又回去看GDAL API了，我不相信这么强大的工具会没有压缩功能。哈哈，果然被我发现了。
官方API：
> 
dst_ds = driver.CreateCopy(dst_filename, src_ds, strict=0,options=["TILED=YES", "***COMPRESS=PACKBITS***"])
666
## 三.代码实现
废话不多说，就是利用CreateCopy（）这个函数去做，直接上代码
> 
/**
*    BaseProcesses是我自己封装的一个函数，大家使用正常GDAL API即可，功能都是一样的
**/
        Gdal.AllRegister();
            BaseProcesses.GdalRegister();
            Dataset ds ;
            Dataset dd;
            int strict = 0;
            int xsize;
            int ysize;
            int iBandNumRef;
            string sBandDataType;
            double[] dGeoTransform;
            string sGeoProjectionRef;
            string [] options=new string[]{"TILED=YES", "COMPRESS=LZW"};
            string path = "D:\\hxl\\yasuotest\\cptest3.tif";
            BaseProcesses.OpenImage(out ds,"D:\\hxl\\yasuotest\\610325201610190000006256250160001.tif");
            BaseProcesses.GetBaseInforOfImage(ds, out xsize, out ysize, out iBandNumRef, out sBandDataType, out dGeoTransform, out sGeoProjectionRef);
            Driver di = ds.GetDriver();
            di.CreateCopy(path, ds, strict, options, null, null);
            BaseProcesses.OpenImage(out dd, path);
            for (int i = 1; i <= ds.RasterCount;i++)
            {
                int[] dataarry = new int[xsize*ysize];
                ds.GetRasterBand(i).ReadRaster(0, 0, xsize, ysize, dataarry, xsize, ysize,0,0);
                dd.GetRasterBand(i).WriteRaster(0, 0, xsize, ysize, dataarry, xsize, ysize, 0, 0);
            }
            dd.FlushCache();
            dd.Dispose();
哈哈，大功告成
## 四.实验结果
![8947186-971e2e09cde436f6.png](https://upload-images.jianshu.io/upload_images/8947186-971e2e09cde436f6.png)
效果还不错吧！由于数据保密，我就不放图啦
