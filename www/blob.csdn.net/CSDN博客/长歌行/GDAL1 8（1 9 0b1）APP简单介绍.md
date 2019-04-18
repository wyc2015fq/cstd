# GDAL1.8（1.9.0b1）APP简单介绍 - 长歌行 - CSDN博客





2012年12月31日 14:51:42[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2055








本来想看看GDAL能否支持矢量数据栅格化的，就在这边找找，后来索性把每个APP都看看，可以了解GDAL的API的用法。
一下为列表，顺序讲解





![](https://img-my.csdn.net/uploads/201212/31/1356936932_3349.png)






[http://wenku.baidu.com/view/c3d449691eb91a37f1115c9e.html](http://wenku.baidu.com/view/c3d449691eb91a37f1115c9e.html)

page 44

我上传的文档 :)

![](https://img-my.csdn.net/uploads/201212/31/1356936959_9427.png)




![](https://img-my.csdn.net/uploads/201212/31/1356936974_4046.png)



1.dumpoverviews


Purpose:  Dump overviews to external files.




所有的APP都是控制台程序，都有很全面的输入解析，以后写控制台程序可以借鉴

APP里面的Usage（）都是告诉输入和输出的格式





GDALGeneralCmdLineProcessor里面是对系统命令进行解析，为共有的输入，如版本号等




本APP的输入参数如下：


[-masks] <filename> [overview]

三组参数，是否有掩码，图像的文件名，需要输出的缩略图的具体层号，比如 1 2 3 ，就是需要1,2,3层的缩略图输出。



本例中使用1000个int的容量的数组来存储这个信息。

intanReqOverviews[1000];




之后的处理会先获取数据的缩略图的总数，然后遍历每个缩略图，将所有的缩略图的层号和输入的进行比较。




如果号码相符，则使用以下函数进行输出，如果需要输出掩码的overview则修改一下输入的参数，第二个参数由hSrcOver修改为GDALGetMaskBand（hSrcOver）





DumpBand(hSrcDS,hSrcOver,osFilename);

改函数会根据原波段的信息创建一个输出波段





/* --------------------------------------------------------------------
 */

/*      Get base ds info.                                              
 */

/* --------------------------------------------------------------------
 */

bHaveGT=GDALGetGeoTransform(hBaseDS,adfGeoTransform)
 ==CE_None;

nOrigXSize=GDALGetRasterXSize(hBaseDS);

nOrigYSize=GDALGetRasterYSize(hBaseDS);


GDALDatasetHhDstDS;

intnXSize=GDALGetRasterBandXSize(hSrcOver);

intnYSize=GDALGetRasterBandYSize(hSrcOver);

GDALDataTypeeDT=GDALGetRasterDataType(hSrcOver);

GDALDriverHhDriver=GDALGetDriverByName("GTiff");




hDstDS=GDALCreate(hDriver,pszName,nXSize,nYSize,

                        
 1,eDT,NULL);




GDAL里面创建一个DataSource的函数为





GDALDatasetHCPL_DLLCPL_STDCALLGDALCreate(GDALDriverHhDriver,

constchar*,int,int,int,GDALDataType,

char**
 );


hDriver：    使用哪种格式的数据的驱动

pszName:     输出的文件名

nXSize:      波段长

nYSize:      波段宽

1：          波段数目？？？

eDT：         数据存储的单元类型（int float等）




这些只给出了图像的基本信息，但是作为地理数据，需要分辨率，即像素区域代表的实际区域的范围，而且上面只创建了一个dataset的框架，里面没有实际的数据，需要进行设置。





if(bHaveGT)

    {

doubleadfOvGeoTransform[6];



memcpy(adfOvGeoTransform,adfGeoTransform,

sizeof(double)
 * 6 );



adfOvGeoTransform[1]
 *= (nOrigXSize/ (double)nXSize);

adfOvGeoTransform[2]
 *= (nOrigXSize/ (double)nXSize);

adfOvGeoTransform[4]
 *= (nOrigYSize/ (double)nYSize);

adfOvGeoTransform[5]
 *= (nOrigYSize/ (double)nYSize);



GDALSetGeoTransform(hDstDS,adfOvGeoTransform);



GDALSetProjection(hDstDS,GDALGetProjectionRef(hBaseDS)
 );

    }








缩略图的仿射变换信息和原始数据的信息是线性变化的。投影信息是用之前的。




intiLine;




for(iLine=
 0;iLine<nYSize;iLine++
 )

    {

GDALRasterIO(hSrcOver,GF_Read,
 0,iLine,nXSize,
 1,

pData,nXSize,
 1,eDT, 0, 0 );

GDALRasterIO(GDALGetRasterBand(hDstDS,
 1 ),GF_Write,

                      0,iLine,nXSize,
 1,

pData,nXSize,
 1,eDT, 0, 0 );

    }

CPLFree(pData);




GDALClose(hDstDS);





以上代码将数据每个波段的指定的层的缩略图输出到指定文件。




GDALRasterIO应该是使用频率最高的函数了吧


GDALRasterIO(GDALRasterBandHhRBand,GDALRWFlageRWFlag,

intnDSXOff,intnDSYOff,intnDSXSize,intnDSYSize,

void*pBuffer,intnBXSize,intnBYSize,GDALDataTypeeBDataType,

intnPixelSpace,intnLineSpace);








hRBand：     波段的句柄

eRWFlag：    读或写

nDSXOff      目标数据x方向的偏移

nDSYOff      目标数据y方向的偏移 


nDSXSize     目标数据x方向上要写入或者读出的大小

nDSYSize     ·······

pBuffer      读出或者写入的数据组的指针

nBXSize      buffer的x方向大小

nBYSize      ·······

eBDataType   数据格式

nPixelSpace  ？？？

nLineSpace 
  ？？？




2.gdal2ogr


Purpose:  Create an OGR datasource
 from the values of a GDAL dataset

          May be useful to test gdal_grid and generate its input
 OGR file




输出输出：





printf("Usage:
 gdal2ogr [--help-general] [-f format_name]\n"

"               
 [-b band_number] [-l dest_layer_name]\n"

"               
 [-t type]\n"

"               
 gdal_datasource_src_name ogr_datasource_dst_name\n"

"\n"

"
 -f format_name: output file format name, possible values are:\n");




-f 格式名 默认为 Esri shapefile

-b 波段数

-l 转换为矢量数据的图层的名称

-t 转换的数据类型 polygon point等

然后就是两个数据的名称路径

未完待续





3.gdal_contour


Contour
 Generator mainline.


未完待续







4.gdal_grid



 * Purpose:  GDAL scattered data gridding (interpolation)
 tool

由分散的点生成栅格，有插值的意思






intmain(intargc,char**argv)


 
        OGRGeometryFactory::createFromWkt(&argv[i+
 1],NULL, &poClipSrc);




poClipSrc=LoadGeometry(pszClipSrcDS,pszClipSrcSQL,

pszClipSrcLayer,pszClipSrcWhere);


/*     
 Find the output driver.                                         */


/*     
 Open input datasource.                                          */


/*     
 Create target raster file.                                      */


hDstDS=GDALCreate(hDriver,pszDest,nXSize,nYSize,nBands,

eOutputType,papszCreateOptions);


/*
 -------------------------------------------------------------------- */

/*     
 If algorithm was not specified assigh default one.              */

/*
 -------------------------------------------------------------------- */


/*
 -------------------------------------------------------------------- */

/*     
 Process SQL request.                                            */

/*
 -------------------------------------------------------------------- */


ProcessLayer(hLayer,hDstDS,poSpatialFilter,nXSize,nYSize,
 1,

bIsXExtentSet,bIsYExtentSet,

dfXMin,dfXMax,dfYMin,dfYMax,pszBurnAttribute,

eOutputType,eAlgorithm,pOptions,

bQuiet,pfnProgress);



/*
 -------------------------------------------------------------------- */

/*     
 Process each layer.                                             */

/*
 -------------------------------------------------------------------- */


OGRLayer内部有


intm_bFilterIsEnvelope;

OGRGeometry*m_poFilterGeom;

OGREnvelopem_sFilterEnvelope;

这几个过滤器，通过


intInstallFilter(OGRGeometry*
 );

来进行设置，表现为










if(poSpatialFilter!=NULL)











OGR_L_SetSpatialFilter(hLayer,
 (OGRGeometryH)poSpatialFilter);

属性过滤器表现为：




if(pszWHERE)

       
 {

if(OGR_L_SetAttributeFilter(hLayer,pszWHERE)
 !=OGRERR_NONE)

break;

       
 }

使用以下创建一个属性查询


OGRFeatureQuery     *m_poAttrQuery;







使用以下处理


ProcessLayer(hLayer,hDstDS,poSpatialFilter,nXSize,nYSize,

i+
 1 +nBands-nLayerCount,

bIsXExtentSet,bIsYExtentSet,

dfXMin,dfXMax,dfYMin,dfYMax,pszBurnAttribute,

eOutputType,eAlgorithm,pOptions,

bQuiet,pfnProgress);




5.gdal_rasterize.cpp





 * Purpose:  Rasterize OGR shapes into a
 GDAL raster.

矢量数据栅格化


printf(

"Usage:
 gdal_rasterize [-b band]* [-i] [-at]\n"

"      
 [-burn value]* | [-a attribute_name] [-3d]\n"

"      
 [-l layername]* [-where expression] [-sql select_statement]\n"

"      
 [-of format] [-a_srs srs_def] [-co \"NAME=VALUE\"]*\n"

"      
 [-a_nodata value] [-init value]*\n"

"      
 [-te xmin ymin xmax ymax] [-tr xres yres] [-tap] [-ts width height]\n"

"      
 [-ot {Byte/Int16/UInt16/UInt32/Int32/Float32/Float64/\n"

"            
 CInt16/CInt32/CFloat32/CFloat64}] [-q]\n"

"      
 <src_datasource> <dst_filename>\n");





intmain(intargc,char**argv)


/*
 -------------------------------------------------------------------- */

/*     
 Parse arguments.                                                */

/*
 -------------------------------------------------------------------- */


hSrcDS=OGROpen(pszSrcFilename,FALSE,NULL);


OGRLayerHhLayer=OGR_DS_GetLayerByName(hSrcDS,papszLayers[i]
 );





hDstDS=CreateOutputDataset(ahLayers,hSRS,

bGotBounds,sEnvelop,

hDriver,pszDstFilename,

nXSize,nYSize,dfXRes,dfYRes,

bTargetAlignedPixels,

anBandList.size(),eOutputType,

papszCreateOptions,adfInitVals,

bNoDataSet,dfNoData);

不要激动，上面这个函数只是设置了输出栅格的范围以及仿射，投影信息，以下才是栅格化


staticvoidProcessLayer(

OGRLayerHhSrcLayer,intbSRSIsSet,

GDALDatasetHhDstDS,std::vector<int>anBandList,

std::vector<double>
 &adfBurnValues,intb3D,intbInverse,

constchar*pszBurnAttribute,char**papszRasterizeOptions,

GDALProgressFuncpfnProgress,void*pProgressData)




三部曲




入口


CPLErrGDALRasterizeGeometries(GDALDatasetHhDS,

intnBandCount,int*panBandList,

intnGeomCount,OGRGeometryH*pahGeometries,

GDALTransformerFuncpfnTransformer,

void*pTransformArg,

double*padfGeomBurnValue,

char**papszOptions,

GDALProgressFuncpfnProgress,

void*pProgressArg)

栅格化


gv_rasterize_one_shape(pabyChunkBuf,iY,

poDS->GetRasterXSize(),nThisYChunkSize,

nBandCount,eType,bAllTouched,

                                   
 (OGRGeometry*)pahGeometries[iShape],

padfGeomBurnValue+iShape*nBandCount,

eBurnValueSource,

pfnTransformer,pTransformArg);

出口


eErr=

poDS->RasterIO(GF_Write,
 0,iY,

poDS->GetRasterXSize(),nThisYChunkSize,

pabyChunkBuf,

poDS->GetRasterXSize(),nThisYChunkSize,

eType,nBandCount,panBandList,
 0, 0, 0 );




6.gdal_translate.cpp


 *
 Purpose:  GDAL Image Translator Program

投影转换




7.gdaladdo.cpp


 *
 Purpose:  Commandline application to build overviews. 

使用命令行来创建金字塔索引




8.gdalbuildvrt.cpp


 *
 Purpose:  Commandline application to build VRT datasets from raster products or content of SHP tile index


fprintf(stdout,"%s",

"Usage:
 gdalbuildvrt [-tileindex field_name] [-resolution {highest|lowest|average|user}]\n"

"                   
 [-tr xres yres] [-tap] [-separate] [-allow_projection_difference] [-q]\n"

"                   
 [-te xmin ymin xmax ymax] [-addalpha] [-hidenodata] \n"

"                   
 [-srcnodata \"value [value...]\"] [-vrtnodata \"value [value...]\"] \n"

"                   
 [-input_file_list my_liste.txt] [-overwrite] output.vrt [gdalfile]*\n"

"\n"

"eg.\n"

" 
 % gdalbuildvrt doq_index.vrt doq/*.tif\n"

" 
 % gdalbuildvrt -input_file_list my_liste.txt doq_index.vrt\n"

"\n"

"NOTES:\n"

" 
 o With -separate, each files goes into a separate band in the VRT band. Otherwise,\n"

"   
 the files are considered as tiles of a larger mosaic.\n"

" 
 o The default tile index field is 'location' unless otherwise specified by -tileindex.\n"

" 
 o In case the resolution of all input files is not the same, the -resolution flag.\n"

"   
 enable the user to control the way the output resolution is computed. average is the default.\n"

" 
 o Input files may be any valid GDAL dataset or a GDAL raster tile index.\n"

" 
 o For a GDAL raster tile index, all entries will be added to the VRT.\n"

" 
 o If one GDAL dataset is made of several subdatasets and has 0 raster bands, its\n"

"   
 datasets will be added to the VRT rather than the dataset itself.\n"

" 
 o By default, only datasets of same projection and band characteristics may be added to the VRT.\n"

           
 );

exit(
 1 );




9.gdaldem.cpp


 *
 Purpose: 

dem的各种处理

printf("
 Usage: \n"

"
 - To generate a shaded relief map from any GDAL-supported elevation raster : \n\n"

"    
 gdaldem hillshade input_dem output_hillshade \n"

"                
 [-z ZFactor (default=1)] [-s scale* (default=1)] \n"

"                
 [-az Azimuth (default=315)] [-alt Altitude (default=45)]\n"

"                
 [-alg ZevenbergenThorne]\n"

"                
 [-compute_edges] [-b Band (default=1)] [-of format] [-co \"NAME=VALUE\"]* [-q]\n"

"\n"

"
 - To generates a slope map from any GDAL-supported elevation raster :\n\n"

"    
 gdaldem slope input_dem output_slope_map \n"

"                
 [-p use percent slope (default=degrees)] [-s scale* (default=1)]\n"

"                
 [-alg ZevenbergenThorne]\n"

"                
 [-compute_edges] [-b Band (default=1)] [-of format] [-co \"NAME=VALUE\"]* [-q]\n"

"\n"

"
 - To generate an aspect map from any GDAL-supported elevation raster\n"

"  
 Outputs a 32-bit float tiff with pixel values from 0-360 indicating azimuth :\n\n"

"    
 gdaldem aspect input_dem output_aspect_map \n"

"                
 [-trigonometric] [-zero_for_flat]\n"

"                
 [-alg ZevenbergenThorne]\n"

"                
 [-compute_edges] [-b Band (default=1)] [-of format] [-co \"NAME=VALUE\"]* [-q]\n"

"\n"

"
 - To generate a color relief map from any GDAL-supported elevation raster\n"

"    
 gdaldem color-relief input_dem color_text_file output_color_relief_map\n"

"                
 [-alpha] [-exact_color_entry | -nearest_color_entry]\n"

"                
 [-b Band (default=1)] [-of format] [-co \"NAME=VALUE\"]* [-q]\n"

"    
 where color_text_file contains lines of the format \"elevation_value red green blue\"\n"

"\n"

"
 - To generate a Terrain Ruggedness Index (TRI) map from any GDAL-supported elevation raster\n"

"    
 gdaldem TRI input_dem output_TRI_map\n"

"                
 [-compute_edges] [-b Band (default=1)] [-of format] [-co \"NAME=VALUE\"]* [-q]\n"

"\n"

"
 - To generate a Topographic Position Index (TPI) map from any GDAL-supported elevation raster\n"

"    
 gdaldem TPI input_dem output_TPI_map\n"

"                
 [-compute_edges] [-b Band (default=1)] [-of format] [-co \"NAME=VALUE\"]* [-q]\n"

"\n"

"
 - To generate a roughness map from any GDAL-supported elevation raster\n"

"    
 gdaldem roughness input_dem output_roughness_map\n"

"                
 [-compute_edges] [-b Band (default=1)] [-of format] [-co \"NAME=VALUE\"]* [-q]\n"

"\n"

"
 Notes : \n"

"  
 Scale is the ratio of vertical units to horizontal\n"

"   
 for Feet:Latlong use scale=370400, for Meters:LatLong use scale=111120 \n\n");

exit(
 1 );







10.gdalenhance.cpp


 *
 Purpose:  Commandline application to do image enhancement. 

图像增强？？应该是图像重采样吧


printf("Usage:
 gdalenhance [--help-general]\n"

"      
 [-of format] [-co \"NAME=VALUE\"]*\n"

"      
 [-ot {Byte/Int16/UInt16/UInt32/Int32/Float32/Float64/\n"

"            
 CInt16/CInt32/CFloat32/CFloat64}]\n"

"      
 [-src_scale[_n] src_min src_max]\n"

"      
 [-dst_scale[_n] dst_min dst_max]\n"

"      
 [-lutbins count]\n"

"      
 [-s_nodata[_n] value]\n"

"      
 [-stddev multiplier]\n"

"      
 [-equalize]\n"

"      
 [-config filename]\n"

"      
 src_dataset dst_dataset\n\n");

printf("%s\n\n",GDALVersionInfo("--version")
 );


/************************************************************************/

/*                         
 EnhancerCallback()                          */

/*                                                                     
 */

/*     
 This is the VRT callback that actually does the image rescaling.*/

/************************************************************************/




staticCPLErrEnhancerCallback(void*hCBData,

intnXOff,intnYOff,intnXSize,intnYSize,

void*pData)





intiBin=
 (int) ((pafSrcImage[iPixel]
 -psEInfo->dfScaleMin)*dfScale);

应该是最近邻元法




11.gdalflattenmask.c


 *
 Purpose:  GDAL mask flattening utility


printf("Usage:
 gdalflattenmask [--help-general] [-of output_format] \n"

"                      
 [-co \"NAME=VALUE\"]* [-set_alpha] [-a_nodata val] \n"

"                      
 srcdatasetname dstdatasetname\n"

"\n"

"This
 utility is intended to produce a new file that merges regular data\n"

"bands
 with the mask bands, for applications not being able to use the mask band concept.\n"

"*
 If -set_alpha is not specified, this utility will use the mask band(s)\n"

" 
 to create a new dataset with empty values where the mask has null values.\n"

"*
 If -set_alpha is specified, a new alpha band is added to the destination\n"

" 
 dataset with the content of the global dataset mask band.\n");


intmain(intargc,char*argv[])

创建，则是有无alpha通道


if(nBands>
 1 && (GDALGetMaskFlags(hSrcBand)
 &GMF_PER_DATASET) == 0)


if(GDALGetRasterColorInterpretation(hSrcBand)
 ==GCI_AlphaBand)


hDstDS=GDALCreate(hDriver,

pszDstFilename,

nXSize,

nYSize,

nBands+
 ((bSetAlpha) ? 1 : 0),

GDT_Byte,

papszCreateOptions);


if(bSetAlpha)

   
 {

GDALRasterBandHhDstAlphaBand=GDALGetRasterBand(hDstDS,nBands+1);

GDALSetRasterColorInterpretation(hDstAlphaBand,GCI_AlphaBand);

   
 }


/*
 -------------------------------------------------------------------- */

/*     
 Write the data values now                                       */

/*
 -------------------------------------------------------------------- */




因为是要做掩码波段，故无效值的取值比较重要


dfNoDataValue=GDALGetRasterNoDataValue(hSrcBand,
 &bHasNoData);





GDALRasterIO(hSrcBand,GF_Read,
 0,iLine,nXSize,
 1,

pabyBuffer,nXSize,
 1,eDataType, 0, 0);

if(!bSetAlpha)

           
 {

GDALRasterIO(hMaskBand,GF_Read,
 0,iLine,nXSize,
 1,

pabyMaskBuffer,nXSize,
 1,GDT_Byte, 0, 0);


if(pabyMaskBuffer[iCol]
 == 0 ||

                               
 ((nMaskFlag&GMF_ALPHA)
 != 0 &&pabyMaskBuffer[iCol]
 < 128))

pabyBuffer[iCol]
 = (GByte)dfNoDataValue;





输出


GDALRasterIO(hDstBand,GF_Write,
 0,iLine,nXSize,
 1,

pabyBuffer,nXSize,
 1,eDataType, 0, 0);












输出掩码波段





/*
 -------------------------------------------------------------------- */

/*     
 Create the alpha band if -set_alpha is specified                */

/*
 -------------------------------------------------------------------- */





GDALRasterIO(hMaskBand,GF_Read,
 0,iLine,nXSize,
 1,

pabyMaskBuffer,nXSize,
 1,GDT_Byte, 0, 0);

for(iCol=
 0;iCol<nXSize;iCol++)

           
 {

/*
 If the mask is 1-bit, expand 1 to 255 */

if(pabyMaskBuffer[iCol]
 == 1 && (nMaskFlag&GMF_ALPHA)
 == 0)

pabyMaskBuffer[iCol]
 = 255;

           
 }

GDALRasterIO(hDstAlphaBand,GF_Write,
 0,iLine,nXSize,
 1,

pabyMaskBuffer,nXSize,
 1,GDT_Byte, 0, 0);




12.gdalinfo.c


 *
 Purpose:  Commandline application to list info about a file.


printf("Usage:
 gdalinfo [--help-general] [-mm] [-stats] [-hist] [-nogcp] [-nomd]\n"

"               
 [-norat] [-noct] [-nofl] [-checksum] [-proj4] [-mdd domain]*\n"

"               
 [-sd subdataset] datasetname\n");




13.gdalmanage.cpp


 *
 Purpose:  Commandline utility for GDAL identify, delete, rename and copy

 *          
 (by file) operations.

printf("Usage:
 gdalmanage identify [-r] [-u] files*\n"

"   
 or gdalmanage copy [-f driver] oldname newname\n"

"   
 or gdalmanage rename [-f driver] oldname newname\n"

"   
 or gdalmanage delete [-f driver] datasetname\n");




14.gdaltindex.c


 *
 Purpose:  Commandline App to build tile index for raster files.

创建栅格索引的shp文件，它能为每个栅格数据建立一个记录，一个包含栅格名称的属性，以及这个栅格的外边界所组成的多边形。


fprintf(stdout,"%s",

"\n"

"Usage:
 gdaltindex [-tileindex field_name] [-write_absolute_path] \n"

"                 
 [-skip_different_projection] index_file [gdal_file]*\n"

"\n"

"eg.\n"

" 
 % gdaltindex doq_index.shp doq/*.tif\n"

"\n"

"NOTES:\n"

" 
 o The shapefile (index_file) will be created if it doesn't already exist.\n"

" 
 o The default tile index field is 'location'.\n"

" 
 o Raster filenames will be put in the file exactly as they are specified\n"

"   
 on the commandline unless the option -write_absolute_path is used.\n"

" 
 o If -skip_different_projection is specified, only files with same projection ref\n"

"   
 as files already inserted in the tileindex will be inserted.\n"

" 
 o Simple rectangular polygons are generated in the same\n"

"   
 coordinate system as the rasters.\n");




15.gdaltorture.cpp


 *
 Purpose:  Commandline utility to torture GDAL API on datasets


printf("Usage:
 gdaltorture [-r] [-u] [-rw] files*\n");

exit(
 1 );




16.gdaltransform.cpp


 *
 Purpose:  Commandline point transformer.




17.gdalwarp.cpp


 *
 Purpose:  Test program for high performance warper API.

投影转换盒投影绑定，同时可以进行图像向前，这个程序可以重新投影所支持的投影




18.gdalwarpsimple.c


 *
 Purpose:  Commandline program for doing a variety of image warps, including

 *          
 image reprojection.




19.multireadtest.cpp



 *
 Purpose:  Multithreading test application.

GDAL自己封装了多线程啊



pGlobalMutex=CPLCreateMutex();

CPLReleaseMutex(pGlobalMutex);




nPendingThreads=nThreadCount;




for(iThread=
 0;iThread<nThreadCount;iThread++
 )

   
 {

if(CPLCreateThread(WorkerFunc,NULL)
 == -1 )

       
 {

printf("CPLCreateThread()
 failed.\n");

exit(
 1 );

       
 }

   
 }




while(nPendingThreads>
 0 )

CPLSleep(
 0.5 );




CPLReleaseMutex(pGlobalMutex);




20.nearblack.cpp



 *
 Purpose:  Convert nearly black or nearly white border to exact black/white.

将栅格中接近黑或者白色的像元转换为黑色或者白色，可以用于修补压缩丢失信息而造成的不准确，以便于镶嵌时将其视为透明。






printf("nearblack
 [-of format] [-white | [-color c1,c2,c3...cn]*] [-near dist] [-nb non_black_pixels]\n"

"         
 [-setalpha] [-setmask] [-o outfile] [-q] [-co \"NAME=VALUE\"]* infile\n");

exit(
 1 );



intmain(intargc,char**argv)



staticvoidProcessLine(GByte*pabyLine,GByte*pabyMask,intiStart,

intiEnd,intnSrcBands,intnDstBands,intnNearDist,

intnMaxNonBlack,intbNearWhite,Colors*poColors,

int*panLastLineCounts,intbDoHorizontalCheck,

intbDoVerticalCheck,intbBottomUp)




21.ogr2ogr.cpp



 *
 Purpose:  Simple client for translating between formats.

进行矢量格式的转换




22.ogrdissolve.cpp



 *
 Purpose:  Allow a user to dissolve geometries based on an attribute.




23.ogrinfo.cpp



 *
 Purpose:  Simple client for viewing OGR driver data.




24.ogrtindex.cpp



 *
 Purpose:  Program to generate a UMN MapServer compatible tile index for a

 *          
 set of OGR data sources. 




25.test_ogrsf.cpp



 *
 Purpose:  Formal test harnass for OGRLayer implementations.




26.testepsg.cpp



 *
 Purpose:  Test mainline for translating EPSG definitions into WKT.










































































