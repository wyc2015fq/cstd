# dlib face detector 时间测试 - weixin_33985507的博客 - CSDN博客
2018年09月27日 20:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：38
首先记住一句话，让严谨成为工作习惯。
最近在做人脸特征点检测，考虑到dlib有人脸检测的API，所以测试一下性能。
最初用Python API测试人脸检测API dlib.get_frontal_face_detector()的速度 每张图片1300ms（CentOS服务器，图片大小960x960,等比例resize为 宽度800）。
然后在PC上测试C++ API get_frontal_face_detector()， 速度为每张图片800ms。
这看起来很正常。
然后看到基于dlib的face_recognition API 库：[https://github.com/ageitgey/face_recognition](https://github.com/ageitgey/face_recognition)
发现 的Python接口非常快，查看API封装发现，同样调用了dlib的Python API dlib.get_frontal_face_detector()，但是速度非常快。这里有一个相关的issue:[https://github.com/ageitgey/face_recognition/issues/256](https://github.com/ageitgey/face_recognition/issues/256)
于是在CentOS服务器上重新测试dlib的Python接口，发现每张图片大约20ms,图片size:450x344, 等比例resize到 宽度为120。
猜想：因为在PC上测试dlib C++ API是自己编译的程序，使用单CPU，而服务器是多CPU环境，而且服务器上dlib是通过pip install 命令安装编译的。猜测是在这个编译过程中配置了多CPU选项，所以导致Python API会比C++API快很多，给人造成dlib  face detect API速度很快的假象。
为保证严谨，基于同一张图片分别做Python API 和 C++ API速度测试
python 代码：
```
# USAGE
# python align_faces.py --shape-predictor shape_predictor_68_face_landmarks.dat --image images/example_01.jpg
# import the necessary packages
from imutils.face_utils import FaceAligner
from imutils.face_utils import rect_to_bb
import argparse
import imutils
import dlib
import cv2
import time
t1 = time.time()*1000
# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-p", "--shape-predictor", required=True,
    help="path to facial landmark predictor")
ap.add_argument("-i", "--image", required=True,
    help="path to input image")
args = vars(ap.parse_args())
# initialize dlib's face detector (HOG-based) and then create
# the facial landmark predictor and the face aligner
detector = dlib.get_frontal_face_detector()
#predictor = dlib.shape_predictor(args["shape_predictor"])
#predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')
#fa = FaceAligner(predictor, desiredFaceWidth=256)
# load the input image, resize it, and convert it to grayscale
image = cv2.imread(args["image"])
#image = imutils.resize(image, width=120)
#gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
gray=image
# show the original input image and detect faces in the grayscale
# image
#cv2.imshow("Input", image)
t2 = time.time() * 1000
print("use time: {}ms".format(t2-t1))
t1=t2
rects = detector(gray, 2)
t2 = time.time() * 1000
print("use time: {}ms".format(t2-t1))
t1=t2
# loop over the face detections
for rect in rects:
    # extract the ROI of the *original* face, then align the face
    # using facial landmarks
        '''
    (x, y, w, h) = rect_to_bb(rect)
    faceOrig = imutils.resize(image[y:y + h, x:x + w], width=256)
    faceAligned = fa.align(image, gray, rect)
        '''
        '''
    import uuid
    f = str(uuid.uuid4())
    cv2.imwrite("foo/" + f + ".png", faceAligned)
    # display the output images
    cv2.imshow("Original", faceOrig)
    cv2.imshow("Aligned", faceAligned)
        #if cv2.waitKey(1) & 0xFF == ord('q'):
        #    break
        '''
t2 = time.time() * 1000
print("use time: {}ms".format(t2-t1))
```
C++测试代码：
```
// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This example program shows how to find frontal human faces in an image.  In
    particular, this program shows how you can take a list of images from the
    command line and display each on the screen with red boxes overlaid on each
    human face.
    The examples/faces folder contains some jpg images of people.  You can run
    this program on them and see the detections by executing the following command:
        ./face_detection_ex faces/*.jpg
    This face detector is made using the now classic Histogram of Oriented
    Gradients (HOG) feature combined with a linear classifier, an image pyramid,
    and sliding window detection scheme.  This type of object detector is fairly
    general and capable of detecting many types of semi-rigid objects in
    addition to human faces.  Therefore, if you are interested in making your
    own object detectors then read the fhog_object_detector_ex.cpp example
    program.  It shows how to use the machine learning tools which were used to
    create dlib's face detector.
    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.
*/
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include <time.h>
using namespace dlib;
using namespace std;
// ----------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    try
    {
        if (argc == 1)
        {
            cout << "Give some image files as arguments to this program." << endl;
            return 0;
        }
        frontal_face_detector detector = get_frontal_face_detector();
        image_window win;
        // Loop over all the images provided on the command line.
        for (int i = 1; i < argc; ++i)
        {
            cout << "processing image " << argv[i] << endl;
            array2d<unsigned char> img;
            load_image(img, argv[i]);
            // Make the image bigger by a factor of two.  This is useful since
            // the face detector looks for faces that are about 80 by 80 pixels
            // or larger.  Therefore, if you want to find faces that are smaller
            // than that then you need to upsample the image as we do here by
            // calling pyramid_up().  So this will allow it to detect faces that
            // are at least 40 by 40 pixels in size.  We could call pyramid_up()
            // again to find even smaller faces, but note that every time we
            // upsample the image we make the detector run slower since it must
            // process a larger image.
            pyramid_up(img);
            // Now tell the face detector to give us a list of bounding boxes
            // around all the faces it can find in the image.
            clock_t start, ends;
            start = clock();
            std::vector<rectangle> dets = detector(img);
            ends = clock();
            cout << ends - start << endl;
            cout << "Number of faces detected: " << dets.size() << endl;
            // Now we show the image on the screen and the face detections as
            // red overlay boxes.
            win.clear_overlay();
            win.set_image(img);
            win.add_overlay(dets, rgb_pixel(255, 0, 0));
            cout << "Hit enter to process the next image..." << endl;
            //cin.get();
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
    
}
// --------------------------------------------------------------------------------
```
Python API运行时间为：大约130ms
```
(env) [hongjie.li@test face-alignment]$ python align_faces.py -p True -i test2_0.5_0.5.png 
use time: 402.801025391ms
use time: 130.208007812ms
use time: 0.198974609375ms
(env) [hongjie.li@test face-alignment]$ python align_faces.py -p True -i test2_0.5_0.5.png 
use time: 310.430908203ms
use time: 130.185302734ms
use time: 0.200927734375ms
```
C++ API 运行时间约为：50ms
```
PS C:\files\face_detection_dlib\build\Release> .\face_detect.exe ..\..\test2_0.5_0.5.png
processing image ..\..\test2_0.5_0.5.png
56
Number of faces detected: 1
Hit enter to process the next image...
PS C:\files\face_detection_dlib\build\Release> .\face_detect.exe ..\..\test2_0.5_0.5.png
processing image ..\..\test2_0.5_0.5.png
48
Number of faces detected: 1
Hit enter to process the next image...
```
测试图片,图片大小为 225x216
![12495273-b4a2fb05e8489308.png](https://upload-images.jianshu.io/upload_images/12495273-b4a2fb05e8489308.png)
test2_0.5_0.5.png
总结：由于之前用Python API 和 C++ API分别测试时忽略了图片的大小（resize操作），甚至C++ API测试时不是严格对detector测试，含包括了其他函数比如
```
win.clear_overlay();
            win.set_image(img);
            win.add_overlay(dets, rgb_pixel(255, 0, 0));
```
后来发现这些操作也占用了大量的时间，导致得到dlib C++ API比Python API要慢的错误结论。
虽然在dlib的setup.py中确实有多CPU的编译配置，可能dlib在pip install安装编译的时候确实使用了多CPU，但是 dlib 的 face detector  C++ API速度并不比Python API慢。
关于dlib face detector API是否使用多CPU,影响到dlib是否适合在移动端使用，所以才会有这个测试过程。
整个总结重测过程花了不少时间，吃了不严谨的亏。
