# Qt添加库文件和头文件目录（QCreator） - DoubleLi - 博客园






在使用QtCreator开发图像处理程序的时候想加入Opencv库来处理图形，添加头文件，需要编辑工程文件夹下的.pro文件在文件中添加以下内容，即可包含头文件的文件夹：



INCLUDEPATH +=D:\OpenCV2.0\vc2008\include\opencv



（D:\OpenCV2.0\vc2008\include\opencv就是我的opencv头文件所在的文件夹，如果还增加其他的头文件，就再添加相应的内容到.pro文件中）



光添加头文件还是会报错的，需要添加相应的库文件到相应的工程中，这样我们就需要添加库文件到工程中：



LIBS += D:\OpenCV2.0\vc2008\lib\cv200.lib

LIBS += D:\OpenCV2.0\vc2008\lib\cv200d.lib

LIBS +=D:\OpenCV2.0\vc2008\lib\cxcore200.lib

LIBS += D:\OpenCV2.0\vc2008\lib\cxcore200d.lib

LIBS += D:\OpenCV2.0\vc2008\lib\ml200.lib

LIBS += D:\OpenCV2.0\vc2008\lib\ml200d.lib

LIBS +=D:\OpenCV2.0\vc2008\lib\cvaux200.lib

LIBS +=D:\OpenCV2.0\vc2008\lib\cvaux200d.lib

LIBS += D:\OpenCV2.0\vc2008\lib\highgui200.lib

LIBS +=D:\OpenCV2.0\vc2008\lib\highgui200d.lib

LIBS += D:\OpenCV2.0\vc2008\lib\cxts200.lib

LIBS +=D:\OpenCV2.0\vc2008\lib\cxts200d.lib



相信会编程的朋友都知道该怎么做了，我就不赘述了。









