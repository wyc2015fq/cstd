# jstring 和char* 之间的转换方法 - xqhrs232的专栏 - CSDN博客
2011年05月13日 09:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：791标签：[encoding																[jni																[null																[command																[sun																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=sun&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.th7.cn/Article/bc/ja/200808/328118.html](http://www.th7.cn/Article/bc/ja/200808/328118.html)
这两天一直在研究jstring 和char* 之间是如何转换的。现把自己成功运行例子贴出来。 
//jstring to char*
char* jstringTostring(JNIEnv* env, jstring jstr)
{
       char* rtn = NULL;
       jclass clsstring = env->FindClass("java/lang/String");
       jstring strencode = env->NewStringUTF("utf-8");
       jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
       jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
       jsize alen = env->GetArrayLength(barr);
       jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
       if (alen > 0)
       {
                 rtn = (char*)malloc(alen + 1); 软件开发网 www.mscto.com 
                 memcpy(rtn, ba, alen);
                 rtn[alen] = 0;
       }
       env->ReleaseByteArrayElements(barr, ba, 0);
       return rtn;
} 
//char* to jstring
jstring stoJstring(JNIEnv* env, const char* pat)
{
       jclass strClass = env->FindClass("Ljava/lang/String;");
       jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
       jbyteArray bytes = env->NewByteArray(strlen(pat));
       env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
       jstring encoding = env->NewStringUTF("utf-8");
       return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
} 
//invoke function
JNIEXPORT jstring JNICALL Java_test_cs_web_SWIFTAlianceCASmfTest_strcal
  (JNIEnv *env, jclass obj, jstring jstr1, jstring jstr2)
{
    jbyteArray bytes = 0;
    jthrowable exc;
    char *pszResult = NULL;    
    char *pszSTR1 = NULL;
    char *pszSTR2 = NULL;
    pszSTR1 = jstringTostring(env, jstr1);
    pszSTR2 = jstringTostring(env, jstr2); 
    int nlen = sizeof(char)*(strlen(pszSTR1)+strlen(pszSTR2));
    pszResult = (char*)malloc(nlen);
    strcpy(pszResult, pszSTR1);
    strcat(pszResult, pszSTR2); 
    jstring jstrRe = stoJstring(env, pszResult);
    free(pszSTR1);
    free(pszSTR2);
    free(pszResult);
    return(jstrRe);
} 
Note: 函数中间有一些签名的定义，不是很好理解。细节请参考sun的jni文档介绍or command javap 
//==============================================================
要注意的一点就是C下跟C++对env的引用方式是不一样的，C下为---(*env),C++下为----env。
