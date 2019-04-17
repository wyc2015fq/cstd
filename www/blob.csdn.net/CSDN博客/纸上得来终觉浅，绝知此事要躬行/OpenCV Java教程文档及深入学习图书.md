# OpenCV Java教程文档及深入学习图书 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年11月27日 17:01:36[boonya](https://me.csdn.net/boonya)阅读数：2795













OpenCV是一个基于BSD许可（开源）发行的跨平台计算机视觉库，可以运行在Linux、Windows、Android和Mac OS操作系统上。它轻量级而且高效——由一系列 C 函数和少量 C++ 类[构成](https://baike.baidu.com/item/%E6%9E%84%E6%88%90/103686)，同时提供了Python、Ruby、MATLAB等语言的接口，实现了[图像处理](https://baike.baidu.com/item/%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86)和计算机视觉方面的很多通用算法。

OpenCV用C++语言编写，它的主要接口也是C++语言，但是依然保留了大量的C语言[接口](https://baike.baidu.com/item/%E6%8E%A5%E5%8F%A3/2886384)。该库也有大量的Python, Java and MATLAB/OCTAVE (版本2.5)的接口。这些语言的API接口函数可以通过在线[文档](https://baike.baidu.com/item/%E6%96%87%E6%A1%A3)获得。如今也提供对于C#,Ch, Ruby的支持。




其所有新的开发和算法都是用C++接口。一个使用CUDA的GPU接口也于2010年9月开始实现。



OpenCV官方Java教程地址：[http://opencv-java-tutorials.readthedocs.io/en/latest/](http://opencv-java-tutorials.readthedocs.io/en/latest/)


# Welcome to OpenCV Java Tutorials documentation!



- [Installing OpenCV for Java](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html)- [Introduction to OpenCV for Java](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#introduction-to-opencv-for-java)
- [Install the latest Java version](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#install-the-latest-java-version)
- [Install the latest Eclipse version](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#install-the-latest-eclipse-version)
- [Install OpenCV 3.x under Windows](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#install-opencv-3-x-under-windows)
- [Install OpenCV 3.x under macOS](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#install-opencv-3-x-under-macos)
- [Install OpenCV 3.x under Linux](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#install-opencv-3-x-under-linux)
- [Set up OpenCV for Java in Eclipse](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#set-up-opencv-for-java-in-eclipse)
- [Set up OpenCV for Java in other IDEs (experimental)](http://opencv-java-tutorials.readthedocs.io/en/latest/01-installing-opencv-for-java.html#set-up-opencv-for-java-in-other-ides-experimental)

- [Your First Java Application with OpenCV](http://opencv-java-tutorials.readthedocs.io/en/latest/02-first-java-application-with-opencv.html)- [A Java application with OpenCV](http://opencv-java-tutorials.readthedocs.io/en/latest/02-first-java-application-with-opencv.html#a-java-application-with-opencv)
- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/02-first-java-application-with-opencv.html#what-we-will-do-in-this-tutorial)
- [Create a New Project](http://opencv-java-tutorials.readthedocs.io/en/latest/02-first-java-application-with-opencv.html#create-a-new-project)
- [Add a User Library](http://opencv-java-tutorials.readthedocs.io/en/latest/02-first-java-application-with-opencv.html#add-a-user-library)
- [Create a simple application](http://opencv-java-tutorials.readthedocs.io/en/latest/02-first-java-application-with-opencv.html#create-a-simple-application)

- [Your First JavaFX Application with OpenCV](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html)- [A JavaFX application with OpenCV](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#a-javafx-application-with-opencv)
- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#what-we-will-do-in-this-tutorial)
- [Your First Application in JavaFX](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#your-first-application-in-javafx)
- [Installing e(fx)clipse plugin and Scene Builder](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#installing-e-fx-clipse-plugin-and-scene-builder)
- [Working with Scene Builder](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#working-with-scene-builder)
- [Key Concepts in JavaFX](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#key-concepts-in-javafx)
- [Managing GUI Interactions With the Controller Class](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#managing-gui-interactions-with-the-controller-class)
- [Video Capturing](http://opencv-java-tutorials.readthedocs.io/en/latest/03-first-javafx-application-with-opencv.html#video-capturing)

- [OpenCV Basics](http://opencv-java-tutorials.readthedocs.io/en/latest/04-opencv-basics.html)- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/04-opencv-basics.html#what-we-will-do-in-this-tutorial)
- [Getting started](http://opencv-java-tutorials.readthedocs.io/en/latest/04-opencv-basics.html#getting-started)
- [Color channel checkbox](http://opencv-java-tutorials.readthedocs.io/en/latest/04-opencv-basics.html#color-channel-checkbox)
- [Load an Image and Add it to the Stream](http://opencv-java-tutorials.readthedocs.io/en/latest/04-opencv-basics.html#load-an-image-and-add-it-to-the-stream)
- [Calculate a Histogram](http://opencv-java-tutorials.readthedocs.io/en/latest/04-opencv-basics.html#calculate-a-histogram)
- [Draw the Histogram](http://opencv-java-tutorials.readthedocs.io/en/latest/04-opencv-basics.html#draw-the-histogram)

- [Fourier Transform](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html)- [Goal](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#goal)
- [What is the Fourier Transform?](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#what-is-the-fourier-transform)
- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#what-we-will-do-in-this-tutorial)
- [Getting Started](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#getting-started)
- [Load the file](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#load-the-file)
- [Applying the DFT](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#applying-the-dft)
- [Applying the inverse DFT](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#applying-the-inverse-dft)
- [Analyzing the results](http://opencv-java-tutorials.readthedocs.io/en/latest/05-fourier-transform.html#analyzing-the-results)

- [Face Detection and Tracking](http://opencv-java-tutorials.readthedocs.io/en/latest/06-face-detection-and-tracking.html)- [Goal](http://opencv-java-tutorials.readthedocs.io/en/latest/06-face-detection-and-tracking.html#goal)
- [Cascade Classifiers](http://opencv-java-tutorials.readthedocs.io/en/latest/06-face-detection-and-tracking.html#cascade-classifiers)
- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/06-face-detection-and-tracking.html#what-we-will-do-in-this-tutorial)
- [Getting Started](http://opencv-java-tutorials.readthedocs.io/en/latest/06-face-detection-and-tracking.html#getting-started)
- [Loading the Classifiers](http://opencv-java-tutorials.readthedocs.io/en/latest/06-face-detection-and-tracking.html#loading-the-classifiers)
- [Detection and Tracking](http://opencv-java-tutorials.readthedocs.io/en/latest/06-face-detection-and-tracking.html#detection-and-tracking)

- [Image Segmentation](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html)- [Goal](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#goal)
- [Canny edge detector](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#canny-edge-detector)
- [Dilatation and Erosion](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#dilatation-and-erosion)
- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#what-we-will-do-in-this-tutorial)
- [Getting Started](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#getting-started)
- [Using the Canny edge detection](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#using-the-canny-edge-detection)
- [Canny Result](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#canny-result)
- [Using the Background Removal](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#using-the-background-removal)
- [Background Removal Result](http://opencv-java-tutorials.readthedocs.io/en/latest/07-image-segmentation.html#background-removal-result)

- [Object Detection](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html)- [Goal](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html#goal)
- [Morphological Image Processing](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html#morphological-image-processing)
- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html#what-we-will-do-in-this-tutorial)
- [Getting Started](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html#getting-started)
- [Image processing](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html#image-processing)
- [Morphological Operators](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html#morphological-operators)
- [Object tracking](http://opencv-java-tutorials.readthedocs.io/en/latest/08-object-detection.html#object-tracking)

- [Camera Calibration](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html)- [Goal](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#goal)
- [What is the camera calibration?](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#what-is-the-camera-calibration)
- [Calibration Pattern](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#calibration-pattern)
- [What we will do in this tutorial](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#what-we-will-do-in-this-tutorial)
- [Getting Started](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#getting-started)
- [Pattern Recognition](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#pattern-recognition)
- [Saving Data](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#saving-data)
- [Camera Calibration](http://opencv-java-tutorials.readthedocs.io/en/latest/09-camera-calibration.html#id1)




# Indices and tables
- [Index](http://opencv-java-tutorials.readthedocs.io/en/latest/genindex.html)
- [Module Index](http://opencv-java-tutorials.readthedocs.io/en/latest/py-modindex.html)
- [Search Page](http://opencv-java-tutorials.readthedocs.io/en/latest/search.html)

中文学习图书：
![](https://img-blog.csdn.net/20171127165953984)







