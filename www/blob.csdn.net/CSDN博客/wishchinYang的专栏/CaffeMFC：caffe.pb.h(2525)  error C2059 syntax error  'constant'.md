# CaffeMFC：caffe.pb.h(2525):  error C2059: syntax error : 'constant' - wishchinYang的专栏 - CSDN博客
2018年03月02日 15:18:24[wishchin](https://me.csdn.net/wishchin)阅读数：1132

下边的语句会报 syntax error : 'constant'。
staticconstDimCheckModeSTRICT =V1LayerParameter_DimCheckMode_STRICT;
staticconstDimCheckMode PERMISSIVE =V1LayerParameter_DimCheckMode_PERMISSIVE
排错方法：在STRICT和PERMISSIVE前加个_，也就是改个名字，则可以集成到MFC中。

