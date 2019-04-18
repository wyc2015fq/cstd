# Linux下安装配置EasyPR中文车牌识别系统 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年09月25日 17:27:06[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7504








                
1)EasyPR开源项目地址：https://github.com/liuruoze/EasyPR

     参考：

     https://github.com/liuruoze/EasyPR/blob/master/Usage.md

     http://www.cnblogs.com/justinzhang/p/5523649.html

     http://www.cnblogs.com/subconscious/p/3979988.html




2)Ubuntu环境：

     root@Slave1:~# uname -a

     Linux Slave1 3.16.0-77-generic #99~14.04.1-Ubuntu SMP Tue Jun 28 19:17:10 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux

     GCC/G++安装：#gcc --versiongcc

                  (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4

                  #apt-get install g++

                  #g++ --version 

                  g++ (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4 

     CMake安装：Ubunut默认cmake version 2.8.12.2

                EasyPR要求cmake版本3.1.0+，需要升级

               #add-apt-repository ppa:george-edison55/cmake-3.x

               #apt-get update

               #apt-get install cmake

               #cmake --version  

                 cmake version 3.2.2 

   3)Opencv安装：

     http://opencv.org/downloads.html 下载opencv-3.1.0.zip放在/home目录下

     #unzip opencv-3.1.0.zip

     解压到/home目录下

     #cd opencv-3.1.0

     #mkdir release

     #cd release

     #cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..

     #make 

     #make install

     测试：

     #mkdir /tmp/opencvdemo

     #cd /tmp/opencvdemo

     #vi DisplayImage.cpp

     编辑： 



```cpp
#include <stdio.h>
            #include <opencv2/opencv.hpp>
            using namespace cv;
            int main(int argc, char** argv )
            {
               if ( argc != 2 )
               {
                printf("usage: DisplayImage.out <Image_Path>\n");
                return -1;
             }
            Mat image;
           image = imread( argv[1], 1 );
          if ( !image.data )
         {
            printf("No image data \n");
            return -1;
          }
          namedWindow("Display Image", WINDOW_AUTOSIZE );
          imshow("Display Image", image);
          waitKey(0);
         return 0;
         }
```
#vi CMakeLists.txt

      编辑：





```
cmake_minimum_required(VERSION 2.8)
            project( DisplayImage )
            find_package( OpenCV REQUIRED )
            add_executable( DisplayImage DisplayImage.cpp )
            target_link_libraries( DisplayImage ${OpenCV_LIBS} )
```


 #cmake .

      #make

      #./DisplayImage uk722.jpg  //要显示图片需要安装GTK+ 2.x版本






4)EasyPR安装：

     https://github.com/liuruoze/EasyPR 下载EasyPR-master.zip

     #unzip EasyPR-master.zip 

     解压到/home目录下

     #cd EasyPR-master

     #vi CMakeLists.txt

      set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/usr/local/share/OpenCV")

      修改为opencv的CMake配置文件路径

     #cmake CMakeLists.txt

     #make -j 12

     生成demo_linux_amd64可执行程序，用于测试。



   5)测试：

     #cd EasyPR-master

     #./demo_linux_amd64 ?

       有5个命令

     ann    ann operation

     judge    determine whether an image block is the license plate

     locate    locate plates in an image

     recognize    plate recognition

     svm    svm operations

     #./demo_linux_amd64 recognize -p resources/image/plate_recognize.jpg --svm resources/model/svm.xml --ann resources/model/ann.xml

     可以训练自己的样本来提升识别。

  参考：

![](https://img-blog.csdn.net/20160925172922199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160925172942293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





