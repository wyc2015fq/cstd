# /usr/lib/x86_64-linux-gnu/libboost_system.so.1.54.0: error adding symbols: D - 家家的专栏 - CSDN博客





2015年03月14日 19:34:48[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4431








把命令：

g++ -c readImageToMat.cpp

        nvcc runCifar10Kaggle.cu readImageToMat.o  -lopencv_core -lopencv_highgui -lopencv_imgproc -lrt -lcublas -lboost_thread -ljpeg -arch sm_20 -O2





改为：

g++ -c readImageToMat.cpp

        nvcc runCifar10Kaggle.cu readImageToMat.o  -lopencv_core -lopencv_highgui -lopencv_imgproc -lrt -lcublas
**-lboost_system** -lboost_thread -ljpeg -arch sm_20 -O2





即可。。。







参考：




http://stackoverflow.com/questions/20057127/freeling-error-with-python-and-java-api-undefined-symbol-zn5boost6system15sys




