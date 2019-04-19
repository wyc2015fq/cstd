# Returning a Mat from native JNI to Java - 三少GG - CSDN博客
2013年06月10日 16:27:11[三少GG](https://me.csdn.net/scut1135)阅读数：3579
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
# [Returning a Mat from native JNI to Java](http://answers.opencv.org/question/12090/returning-a-mat-from-native-jni-to-java/)
Q:
I have a question about mixing OpenCV's C++ and Java API. I'm using the new desktop Java API to do some processing on images, but due to Java limitations I am forced to capture the images
 in C++. The images are in an OS-dependant format, so I thought it would be best to convert them to the OpenCV format before handing it to the OS-independant Java code. In the android tutorials
 (Tutorial 2 Advanced) I saw that they instantiate a Mat on the Java side, and use getNativeObjAddr() to pass a pointer to C++, where it is used as a native C++ object. However, since I can
 not make any assumptions on the dimensions or channels of the images, I can not take this approach. I want Java to be able to receive the Mat object without knowing anything about it beforehand.
 I have nu clue how to do this correctly. I have two ideas:
1) I create a Java Mat object from the C++ side ((*env)->NewObject), use JNI to call getNativeObjAddr and work with the
 returned pointer. While this might work, it seems so backwards... Maybe there's a better way?
2) I create a C++ Mat, and return the pointer to Java. I'm hoping there's some kind of functionality that allows me to wrap it in a Java Mat object.
 The documentation mentions the Mat(long addr) constructor, but there's no further explanation, and I have no idea what it's for. Can it be used to create a Mat object
 from a pointer to a native Mat object?
TL;DR: How do I get a Mat created in C++ to Java ?
Thank you for reading!
You can create an empty Mat in Java part and then call create method from C++:
```
// Java
Mat m = new Mat();
jni_func(m.**getNativeObjAddr****()**);
// C++
void jni_func(jlong matPtr)
{
    Mat* mat = (Mat*) matPtr;
    mat->create(rows, cols, type);
    memcpy(mat->data, data, mat->step * mat->rows);
}
```
[link](http://answers.opencv.org/question/12090/returning-a-mat-from-native-jni-to-java/?answer=12097#post-id-12097)deleteflag
 offensive[edit](http://answers.opencv.org/answers/12097/edit/)
## Comments

Hah, I feel stupid now... I like this approach, as it doesn't require the bridge to be crossed once we're in the function. Thanks for that!
[peanutman](http://answers.opencv.org/users/1773/peanutman/) (Apr
 23 '13)edit

# [Returning Mat object from native code to java in OpenCV](http://stackoverflow.com/questions/9935618/returning-mat-object-from-native-code-to-java-in-opencv)
[http://stackoverflow.com/questions/9935618/returning-mat-object-from-native-code-to-java-in-opencv](http://stackoverflow.com/questions/9935618/returning-mat-object-from-native-code-to-java-in-opencv)
I have an OpenCV Android app. Most of its code is in Java but I have one function that is in C. The function gets a Mat object and returns a new one.
My question is how do I return a Mat from the native code to Java? Couldn't find any example of that.
Thanks.
Answer:
Today I had to return a Mat from native code. I started with "Tutorial 2 Advanced - 2. Mix Java+Native OpenCV" it already passes two Mat (Images captured from camera) objects to the native code. But I wanted to return extracted feature, thus I added 
```
jlong
 addrDescriptor
```
 to the signature:
```
extern "C" {
JNIEXPORT void JNICALL Java_org_opencv_samples_tutorial4_Sample4View_FindFeatures(JNIEnv* env, jobject thiz, jlong addrGray, jlong addrRgba, jlong addrDescriptor)
{
    Mat* pMatGr=(Mat*)addrGray;
    Mat* pMatRgb=(Mat*)addrRgba;
    Mat* pMatDesc=(Mat*)addrDescriptor;
    vector<KeyPoint> v;
    //OrbFeatureDetector detector(50);
    OrbFeatureDetector detector;
    OrbDescriptorExtractor  extractor;
    detector.detect(*pMatGr, v);
    extractor.compute( *pMatGr, v, *pMatDesc );
    circle(*pMatRgb, Point(100,100), 10, Scalar(5,128,255,255));
    for( size_t i = 0; i < v.size(); i++ ) {
        circle(*pMatRgb, Point(v[i].pt.x, v[i].pt.y), 10, Scalar(255,128,0,255));
    }
    }
}
```
In the java part I added the Mat
```
private Mat descriptor;
descriptor = new Mat();
```
The method `getNativeObjAddr()` does
 the trick. The Mat is allocated in java and its address is passed to the native code, thus there isn't any explicit returning.
```
FindFeatures(mGraySubmat.getNativeObjAddr(), mRgba.getNativeObjAddr(), descriptor.getNativeObjAddr());
Log.i("desc:"  , descriptor.dump());
```
The Mat was filled with the required data and is directly accessible in the java code after the JNI invokation returns.
Somwhere else in the code the Mat is released:
```
if ( descriptor != null) 
  descriptor.release();
descriptor = null;
```
**Rect没有****getNativeObjAddr****函数，不能类似**。
尽量使用** jintArray.**
eg: [http://lveyo.iteye.com/blog/360622](http://lveyo.iteye.com/blog/360622)
jintArray faceArray = env-> NewIntArray(4*total);
jint faceBuf[4];
  
env->SetIntArrayRegion(faceArray,0,4,faceBuf);
  
return faceArray;
  
