# 核磁数据处理之: FSL数据快视 - happyhorizon的算法天空 - CSDN博客
2018年04月08日 18:09:43[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：725
数据质量检查的时候, 惯用的做法是用fsleyes逐个打开检查, 当数据量变大的时候, 这个做法非常消耗时间和精力. 考虑到NifTI数据本质上就是好多成像切片三维重建的, 如果可以将其中一些切片拿出来在html中批量显示, 一个NifTI文件对应一排切片图像, 这样就可以到达快速检查的目的. 好在FSL已经提供了这样的功能, 只需要组合两个FSL命令: slicesdir 和imglob. 
`slicesdir `imglob *``
如果只查看当前文件夹下以特定名称开头的NifTI文件. 以pat开头为例:
`slicesdir `imglob pat*``
imglog选择切片, slicesdir生成数据快视html. 
![这里写图片描述](https://img-blog.csdn.net/20180408180749235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
