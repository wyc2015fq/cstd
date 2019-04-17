# opencv cmake编译错误 - zqjackking的博客 - CSDN博客





2018年06月02日 11:05:36[zqjackking](https://me.csdn.net/zqjackking)阅读数：1875








使用Cmake编译opencv源码遇到如下错误

**CMake Error: The following variables are used in this project, but they are set to NOTFOUND. 

Please set them or make sure they are set and tested correctly in the CMake files: 

CUDA_nppi_LIBRARY (ADVANCED) 

linked by target “opencv_cudev” in directory D:/Cproject/opencv/opencv/sources/modules/cudev 

linked by target “opencv_cudev” in directory D:/Cproject/opencv/opencv/sources/modules/cudev 

linked by target “opencv_test_cudev” in directory D:/Cproject/opencv/opencv/sources/modules/cudev/test 

linked by target “opencv_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_test_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_perf_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_test_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_perf_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_flann” in directory D:/Cproject/opencv/opencv/sources/modules/flann 

linked by target “opencv_flann” in directory D:/Cproject/opencv/opencv/sources/modules/flann 

linked by target “opencv_test_flann” in directory D:/Cproject/opencv/opencv/sources/modules/flann 

linked by target “opencv_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_test_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_perf_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_test_ml” in directory D:/Cproject/opencv/opencv/sources/modules/ml 

linked by target “opencv_ml” in directory D:/Cproject/opencv/opencv/sources/modules/ml 

linked by target “opencv_ml” in directory D:/Cproject/opencv/opencv/sources/modules/ml 

linked by target “opencv_test_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_perf_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_test_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_perf_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_test_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_perf_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_test_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_perf_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_perf_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_test_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_test_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_perf_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_perf_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_test_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_test_shape” in directory D:/Cproject/opencv/opencv/sources/modules/shape 

linked by target “opencv_shape” in directory D:/Cproject/opencv/opencv/sources/modules/shape 

linked by target “opencv_shape” in directory D:/Cproject/opencv/opencv/sources/modules/shape 

linked by target “opencv_test_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_perf_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_perf_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_test_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_highgui” in directory D:/Cproject/opencv/opencv/sources/modules/highgui 

linked by target “opencv_highgui” in directory D:/Cproject/opencv/opencv/sources/modules/highgui 

linked by target “opencv_test_highgui” in directory D:/Cproject/opencv/opencv/sources/modules/highgui 

linked by target “opencv_perf_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_test_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_ts” in directory D:/Cproject/opencv/opencv/sources/modules/ts 

linked by target “opencv_ts” in directory D:/Cproject/opencv/opencv/sources/modules/ts 

linked by target “opencv_test_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_perf_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_test_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_perf_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_test_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_perf_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_test_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_perf_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_perf_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_test_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_test_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_perf_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_perf_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_test_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_test_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_perf_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_test_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_perf_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_videostab” in directory D:/Cproject/opencv/opencv/sources/modules/videostab 

linked by target “opencv_videostab” in directory D:/Cproject/opencv/opencv/sources/modules/videostab 

linked by target “opencv_traincascade” in directory D:/Cproject/opencv/opencv/sources/apps/traincascade 

linked by target “opencv_createsamples” in directory D:/Cproject/opencv/opencv/sources/apps/createsamples 

linked by target “opencv_annotation” in directory D:/Cproject/opencv/opencv/sources/apps/annotation 

linked by target “opencv_visualisation” in directory D:/Cproject/opencv/opencv/sources/apps/visualisation 

linked by target “opencv_version” in directory D:/Cproject/opencv/opencv/sources/apps/version 

Please set them or make sure they are set and tested correctly in the CMake files: 

CUDA_nppi_LIBRARY (ADVANCED) 

linked by target “opencv_cudev” in directory D:/Cproject/opencv/opencv/sources/modules/cudev 

linked by target “opencv_cudev” in directory D:/Cproject/opencv/opencv/sources/modules/cudev 

linked by target “opencv_test_cudev” in directory D:/Cproject/opencv/opencv/sources/modules/cudev/test 

linked by target “opencv_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_test_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_perf_core” in directory D:/Cproject/opencv/opencv/sources/modules/core 

linked by target “opencv_test_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_perf_cudaarithm” in directory D:/Cproject/opencv/opencv/sources/modules/cudaarithm 

linked by target “opencv_flann” in directory D:/Cproject/opencv/opencv/sources/modules/flann 

linked by target “opencv_flann” in directory D:/Cproject/opencv/opencv/sources/modules/flann 

linked by target “opencv_test_flann” in directory D:/Cproject/opencv/opencv/sources/modules/flann 

linked by target “opencv_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_test_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_perf_imgproc” in directory D:/Cproject/opencv/opencv/sources/modules/imgproc 

linked by target “opencv_test_ml” in directory D:/Cproject/opencv/opencv/sources/modules/ml 

linked by target “opencv_ml” in directory D:/Cproject/opencv/opencv/sources/modules/ml 

linked by target “opencv_ml” in directory D:/Cproject/opencv/opencv/sources/modules/ml 

linked by target “opencv_test_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_perf_video” in directory D:/Cproject/opencv/opencv/sources/modules/video 

linked by target “opencv_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_test_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_perf_cudabgsegm” in directory D:/Cproject/opencv/opencv/sources/modules/cudabgsegm 

linked by target “opencv_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_test_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_perf_cudafilters” in directory D:/Cproject/opencv/opencv/sources/modules/cudafilters 

linked by target “opencv_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_test_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_perf_cudaimgproc” in directory D:/Cproject/opencv/opencv/sources/modules/cudaimgproc 

linked by target “opencv_perf_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_test_cudawarping” in directory D:/Cproject/opencv/opencv/sources/modules/cudawarping 

linked by target “opencv_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_test_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_perf_imgcodecs” in directory D:/Cproject/opencv/opencv/sources/modules/imgcodecs 

linked by target “opencv_perf_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_test_photo” in directory D:/Cproject/opencv/opencv/sources/modules/photo 

linked by target “opencv_test_shape” in directory D:/Cproject/opencv/opencv/sources/modules/shape 

linked by target “opencv_shape” in directory D:/Cproject/opencv/opencv/sources/modules/shape 

linked by target “opencv_shape” in directory D:/Cproject/opencv/opencv/sources/modules/shape 

linked by target “opencv_test_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_perf_videoio” in directory D:/Cproject/opencv/opencv/sources/modules/videoio 

linked by target “opencv_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_perf_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_test_cudacodec” in directory D:/Cproject/opencv/opencv/sources/modules/cudacodec 

linked by target “opencv_highgui” in directory D:/Cproject/opencv/opencv/sources/modules/highgui 

linked by target “opencv_highgui” in directory D:/Cproject/opencv/opencv/sources/modules/highgui 

linked by target “opencv_test_highgui” in directory D:/Cproject/opencv/opencv/sources/modules/highgui 

linked by target “opencv_perf_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_test_objdetect” in directory D:/Cproject/opencv/opencv/sources/modules/objdetect 

linked by target “opencv_ts” in directory D:/Cproject/opencv/opencv/sources/modules/ts 

linked by target “opencv_ts” in directory D:/Cproject/opencv/opencv/sources/modules/ts 

linked by target “opencv_test_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_perf_features2d” in directory D:/Cproject/opencv/opencv/sources/modules/features2d 

linked by target “opencv_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_test_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_perf_calib3d” in directory D:/Cproject/opencv/opencv/sources/modules/calib3d 

linked by target “opencv_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_test_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_perf_cudafeatures2d” in directory D:/Cproject/opencv/opencv/sources/modules/cudafeatures2d 

linked by target “opencv_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_test_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_perf_cudalegacy” in directory D:/Cproject/opencv/opencv/sources/modules/cudalegacy 

linked by target “opencv_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_perf_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_test_cudaobjdetect” in directory D:/Cproject/opencv/opencv/sources/modules/cudaobjdetect 

linked by target “opencv_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_test_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_perf_cudaoptflow” in directory D:/Cproject/opencv/opencv/sources/modules/cudaoptflow 

linked by target “opencv_perf_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_test_cudastereo” in directory D:/Cproject/opencv/opencv/sources/modules/cudastereo 

linked by target “opencv_test_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_perf_stitching” in directory D:/Cproject/opencv/opencv/sources/modules/stitching 

linked by target “opencv_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_test_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_perf_superres” in directory D:/Cproject/opencv/opencv/sources/modules/superres 

linked by target “opencv_videostab” in directory D:/Cproject/opencv/opencv/sources/modules/videostab 

linked by target “opencv_videostab” in directory D:/Cproject/opencv/opencv/sources/modules/videostab 

linked by target “opencv_traincascade” in directory D:/Cproject/opencv/opencv/sources/apps/traincascade 

linked by target “opencv_createsamples” in directory D:/Cproject/opencv/opencv/sources/apps/createsamples 

linked by target “opencv_annotation” in directory D:/Cproject/opencv/opencv/sources/apps/annotation 

linked by target “opencv_visualisation” in directory D:/Cproject/opencv/opencv/sources/apps/visualisation 

linked by target “opencv_version” in directory D:/Cproject/opencv/opencv/sources/apps/version**
原因解析：

cuda9不再支持2.0架构

解决方案，参考：[https://blog.csdn.net/u014613745/article/details/78310916](https://blog.csdn.net/u014613745/article/details/78310916)




