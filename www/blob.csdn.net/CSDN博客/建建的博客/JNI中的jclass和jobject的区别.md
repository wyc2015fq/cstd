# JNI中的jclass和jobject的区别 - 建建的博客 - CSDN博客
2017年06月01日 09:38:52[纪建](https://me.csdn.net/u013898698)阅读数：2471
jclass和jobject的迷惑
第一次使用JNI，实例引用（jobject）和类引用(jclass)让人觉得很困惑。
实例引用与一个数组和java.lang.Object类或它的子类的实例对应。类引用与java.lang.Class实例对应，它代表着类的类型。
一个操作如GetFieldID,需要参数jclass，是一个类操作，因为它从一个类中获得field的描述。与此相反，GetIntField需要参数jobject，这是一个实例操作，因为它从这个实例中获得这个field的值。在所有的JNI方法中jobject和实例操作的结合和jclass和类操作的结合保持一致。所以是很容易记住类操作与实例操作的不同的。
随后，普及下静态方法和实例方法的区别：
静态方法与静态变量一样，属于类本身，而不属于那个类的一个对象。调用一个被定义为static的方法，可以通过在它前面加上这个类的名称，也可以像调用非静态方法一样通过类对象调用。
实例方法必须通过类的实例来使用。实例方法可以使用类的非静态成员，也可以使用类的静态成员。
类的静态方法，静态变量是在类装载的时候装载的。但是要特别注意，类的静态变量是该类的对象所共有的，即是所有对象共享变量。所以建议尽量少用静态变量。尽量在静态方法中使用内部变量。
其中static关键字即表示静态的。声明静态方法的语法如下：
<访问修饰符>static返回类型 方法名(参数列表)
{//方法体}
静态方法与实例方法唯一不同的，就是静态方法在返回类型前加static关键字。静态方法的调用有两种途径：
（1）通过类的实例对象去调用
调用格式为： 对象名.方法名
(2) 通过类名直接调用
调用格式为： 类名::方法名
对于JNI的头文件的区别：
```
/*
 * Class:     com_nedu_jni_helloword_HeaderFile
 * Method:    doInt
 * Signature: (DD)I
 */
JNIEXPORT jint JNICALL Java_com_nedu_jni_helloword_HeaderFile_doInt__DD
  (JNIEnv *, jobject, jdouble, jdouble);
/*
 * Class:     com_nedu_jni_helloword_HeaderFile
 * Method:    doInt
 * Signature: (DDD)I
 */
JNIEXPORT jint JNICALL Java_com_nedu_jni_helloword_HeaderFile_doInt__DDD
  (JNIEnv *, jclass, jdouble, jdouble, jdouble);
```
这里可以看到参数列表的第二个参数的类型不一样，第一个是jobject，表明这个函数是一个实例方法，指向这个实例；而下面的是jclass，表明是个静态方法，指向这个类的class
