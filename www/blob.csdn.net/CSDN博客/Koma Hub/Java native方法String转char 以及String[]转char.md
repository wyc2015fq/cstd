# Java native方法String转char*以及String[]转char** - Koma Hub - CSDN博客
2017年09月08日 11:57:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：224
```java
JNIEXPORT void JNICALL Java_swap_swap_1float
  (JNIEnv *env, jclass cla, jstring in, jstring out)
        char *FN1;
        char *FN2;
       FN1  =  (char*)(*env)->GetStringUTFChars(env,in,NULL);
       FN2  =  (char*)(*env)->GetStringUTFChars(env,out,NULL);
```
反向转换：
```java
out = env->NewStringUTF(FN1);
```
String[]转char**
```cpp
/ *
 * Class:     com_xmu_engine_HTK
 * Method:    HVite
 * Signature: ()V
http://blog.chinaunix.net/uid-29421714-id-4152891.html
 * /
JNIEXPORT void JNICALL Java_com_xmu_engine_HTK_HVite
    (JNIEnv * env, jclass jc,jobjectArray strArray ){
    jstring jstr;
    jsize len = (*env)->GetArrayLength(env, strArray);
    char **pstr = (char **) malloc(len*sizeof(char *));
             
    int i=0;
            
    for (i=0 ; i<len;i++){
        jstr = (*env)->GetObjectArrayElement(env, strArray, i);
        pstr[i] = (char *)(*env)->GetStringUTFChars(env, jstr, 0);           
               
        }    
            
    HVite(len,pstr);
            
}
```
