# JNI系列入门之C语言与Java的双向通信（一） - weixin_33985507的博客 - CSDN博客
2018年07月27日 13:06:03[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
> 
**JNI系列文章：**
- [JNI系列之入门Hello JNI C（一）](https://juejin.im/post/5b5b16535188251afe7b99e5)
- [JNI系列之入门Hello JNI C（二）](https://juejin.im/post/5b5b17d5f265da0f9e58aa0d)
- [JNI系列入门之C语言与Java的双向通信（一）](https://juejin.im/post/5b5b1884f265da0f6f1aae83)
- [JNI系列入门之C语言与Java的双向通信（二）](https://juejin.im/post/5b5b18d0518825597f6b8226)
- [JNI系列入门之C语言中文字符串乱码问题](https://juejin.im/post/5b5b1ad3f265da0f8145f194)
# 一、概述
- Java层向C层通信
- 通过调用静态无参数、有参数的native方法
- 通过调用非静态无参数、有参数的native方法
- C层向Java层通信
- C层访问和修改Java层的属性
- C层访问和修改Java层的静态属性
- C层访问Java层的方法
- C层访问Java层的静态方法
- C层访问Java层的构造方法，并创建Java对象返回
- java中传入数组
- C中生成一个数组返回给java
# 二、实现
> 
Java层向C层通信
- 通过调用静态无参数、有参数的native方法
```java
JniTest.java
// native的静态方法，生成的JNI函数参数是(JNIEnv *env, jclass jcls)
public native static String getStringFromC();
// 生成的JNI函数参数是(JNIEnv *env, jclass jcls, jstring jstr_input)
public native static String getNewString(String input);
复制代码
```
在java中声明两个native方法，然后通过javah命令生成头文件，具体的头文件生成步骤，可以看[JNI系列之入门Hello JNI C（一）](https://link.juejin.im?target=http%3A%2F%2Fwww.jianshu.com%2Fp%2F9c237512230c)：
```java
com_jerry_jnitest_JniTest.h
JNIEXPORT jstring JNICALL Java_com_jerry_jnitest_JniTest_getStringFromC
  (JNIEnv *, jclass);
JNIEXPORT jstring JNICALL Java_com_jerry_jnitest_JniTest_getNewString
(JNIEnv *, jclass, jstring);
复制代码
```
具体实现：
```java
// 静态无参函数实现，返回一个C的字符串
JNIEXPORT jstring JNICALL Java_com_jerry_jnitest_JniTest_getStringFromC
(JNIEnv *env, jclass jcls) {
	char *text = "Hi, Jerry! 动态链接库，调用起来了！";
	return (*env)->NewStringUTF(env, text);
}
// 静态有参函数实现，新建一个C字符串与java输入的字符串拼接后返回给java
JNIEXPORT jstring JNICALL Java_com_jerry_jnitest_JniTest_getNewString
(JNIEnv *env, jclass jcls, jstring jstr_input) {
        // 将java的输入参数jstring -> c字符串
	char *input = (*env)->GetStringUTFChars(env, jstr_input, NULL);
	char text[30] = "Jerry";
	// 拼接字符串
	char *new_text = strcat(text, input);
	printf("newText = %s\n", new_text);
	// 创建一个jstring返回给java
	jstring jstr_new = (*env)->NewStringUTF(env, new_text);
	return jstr_new;
}
复制代码
```
getStringFromC函数中，我们使用(*env)来调用NewStringUTF函数通过传入C字符指针也就是字符串来创建一个jstring类型的字符串（对应java数据类型的String），很简单。也许你会问，为什么是(*env)来调用函数，因为JNIEnv本身就是一个结构体指针了，所以env就是结构体二级指针变量，而NewStringUTF函数又是定义在JNIEnv这个结构体指针的结构体里的函数（这是一个函数指针），所以需要用(*env)来取到结构体一级指针变量的地址，来获取其地址所指向的内容。具体可以看上一篇文章[JNI系列之入门Hello JNI C（二）](https://link.juejin.im?target=http%3A%2F%2Fwww.jianshu.com%2Fp%2F06e6ab5a4c2e)
getNewString函数中多了一个参数，jstr_input表示java中native方法的参数String input，在函数实现里，先将jstring转换成c字符串，然后引入c的函数库string.h头文件，使用strcat拼接出一个新的c字符串，然后再用NewStringUTF函数把新的C字符串->jstring，返回给java。
```java
public static void main(String[] args) {
		// 在main方法中调用native方法
		System.out.println(getStringFromC());
		System.out.println(getNewString("真是太帅了..."));
}
复制代码
```
输出结果：
这是java中的native方法调用，看的出来，C的中文字符串乱码，而java传入的中文字符串不会乱码。关于C的中文字符串乱码问题，我将会在下一篇文章里说明解决方案。
- 通过调用非静态无参数、有参数的native方法
```java
// native的对象方法，生成的NI函数参数是(JNIEnv *env, jobject jobj)
	public native String getNameFromC();
复制代码
```
C中的头文件实现:
```java
// 非静态无参函数
JNIEXPORT jstring JNICALL Java_com_jerry_jnitest_JniTest_getNameFromC
(JNIEnv *env, jobject jobj) {
	
	return (*env)->NewStringUTF(env, "call getNameFromC");
}
复制代码
```
对于非静态native方法，就是函数的参数变成了jobject，这个jobj就是在java中调用非静态native方法的那个对象，比如：
```java
JniTest  jt = new JniTest();
jt.getNameFromC();
复制代码
```
jobj就表示对象jt。而静态的jclass就表示native方法声明的那个类类型比如JniTest.class。
> 
C层向Java层通信
- C层访问和修改Java层的属性
我们在java中定义一个属性name:
```java
public String name = "Jerry";
复制代码
```
为了方便编写使用C层来调用Java层的内容，接下来都会先在Java层创建一个native方法，然后在C层native函数实现里来调用访问Java层的内容。
```java
/**
  * @return 修改后的属性内容
  */
	public native String accessField();
复制代码
```
C中的实现：
```java
// 1. 访问java非属性
// 修改java中属性name的值并返回
JNIEXPORT jstring JNICALL Java_com_jerry_jnitest_JniTest_accessField
(JNIEnv *env, jobject jobj) {
	// 获取java中name属性所在对象的class类类型
	jclass jcls = (*env)->GetObjectClass(env, jobj);
	// 获取name属性的fieldID
	// (参数：name表示Java中属性的名字，最后一个参数表示属性的类型签名)
	jfieldID fid = (*env)->GetFieldID(env, jcls, "name", "Ljava/lang/String;");
	if (fid == NULL) {
		printf("fid is NULL!");
	}
	
	// 通过属性的fieldId获取属性的值
	jstring jstr_fvalue = (*env)->GetObjectField(env, jobj, fid);
	// 要想操作修改属性值，得先把jstring -> 转换成 c的字符串
	// 最后一个参数：JNI_TRUE是一个宏定义值是1，表示true需要拷贝（拷贝过一份内存地址），
    // JNI_FALSE表示不需要拷贝（和java使用同一份字符串内存地址），官方建议使用NULL
	char *ch_fvalue = (*env)->GetStringUTFChars(env, jstr_fvalue, NULL);
	// 使用string.h的字符串操作库修改属性值
	char text[20] = "hello ";
	char *new_fvalue = strcat(text, ch_fvalue);
	// c -> java
	jstring new_jstr = (*env)->NewStringUTF(env, new_fvalue);
	// 修改属性的值
	(*env)->SetObjectField(env, jobj, fid, new_jstr);
	return new_jstr;
}
复制代码
```
代码中注释写的很清楚了，这里还需要注意的是关于属性和方法的签名规则：
|数据类型|签名|
|----|----|
|boolean|Z|
|byte|B|
|char|C|
|short|S|
|int|I|
|long|J|
|float|F|
|double|D|
|void|V|
|Object|L开头，然后以/分隔包的完整类型名，后面再加";"(分号)，比如说 String 签名就是Ljava/lang/String;|
|||
|int[]Object[]|以[开头，在加上数组元素类型的签名，比如int[] 签名就是[I   , 再比如 int[][]  签名就是 [[I  ,object数组签名就是  [Ljava/lang/Object;|
- C层访问和修改Java层的静态属性
在java中定义一个静态属性size:
```java
private static int size = 26;
复制代码
```
在C中实现一个函数用来修改java的这个静态属性：
```java
// 2. 访问java静态属性
// 修改java中静态属性size的值并返回
JNIEXPORT void JNICALL Java_com_jerry_jnitest_JniTest_accessStaticField
(JNIEnv *env, jobject jobj) {
	// 获取jclass
	jclass cls = (*env)->GetObjectClass(env, jobj);
	// 获取jfieldID
	jfieldID fID = (*env)->GetStaticFieldID(env, cls, "size", "I");
	// 获取属性值
	jint size= (*env)->GetStaticIntField(env, cls, fID);
	size += 12;
	// 修改属性值
	(*env)->SetStaticIntField(env, cls, fID, size);
}
复制代码
```
经过上面非静态属性的访问，静态的属性就很简单了，一样的套路：获取jclass -> 获取属性的jfieldID -> 获取属性值 ->设置属性值。还有一个套路：获取属性值 一般都是GetStaticField，设置属性值一般都是SetStaticField。
> 
**JNI系列文章：**
- [JNI系列之入门Hello JNI C（一）](https://juejin.im/post/5b5b16535188251afe7b99e5)
- [JNI系列之入门Hello JNI C（二）](https://juejin.im/post/5b5b17d5f265da0f9e58aa0d)
- [JNI系列入门之C语言与Java的双向通信（一）](https://juejin.im/post/5b5b1884f265da0f6f1aae83)
- [JNI系列入门之C语言与Java的双向通信（二）](https://juejin.im/post/5b5b18d0518825597f6b8226)
- [JNI系列入门之C语言中文字符串乱码问题](https://juejin.im/post/5b5b1ad3f265da0f8145f194)
