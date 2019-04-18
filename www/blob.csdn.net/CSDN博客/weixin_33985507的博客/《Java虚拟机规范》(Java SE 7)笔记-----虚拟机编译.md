# 《Java虚拟机规范》(Java SE 7)笔记-----虚拟机编译 - weixin_33985507的博客 - CSDN博客
2016年08月06日 14:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
这里的编译是指Java语言编译成Java虚拟机指令集的编译器。指令格式:
```
<index> <opcode> [<operand1> [operand1...]] [<comment>]
```
index为指令操作码的索引，也可以认为相对于方法开始处的偏移量。在表示运行时常量池索引的操作数前会以"#"开头。
```
10 ldc #1 //Push float constant 100.0
```
#### 1.常量、局部变量的使用和控制结构
一个简单的for循环例子，成对应字节码的执行过程，这是一个理解字节码执行比较好的例子。
```
void spin() {
    int i = 0;
    for(i = 0; i < 100;i ++ ) { 
    }
}
//对应的字节码(编译器实现可能不同)
Method void spin()
0 iconst_0
1 istore_1
2 goto 8
5 iinc 1 1
8 iload_1
9 bipush 100
11 if_icmplt 5
```
![1002628-bd8e0c375bbdc1b2.png](https://upload-images.jianshu.io/upload_images/1002628-bd8e0c375bbdc1b2.png)
jvm_for_loop1
iconst_0操作码隐式包含了int类型0操作数，表示将int型0值压入操作数栈，这样不需要专门为入栈操作保存一个立即操作数的值，避免读取解析操作数，简单高效。如果使用bitpush 0将会增加一个字节的长度。类似指令还有 iconst_m1 iconst_1 ...iconst_5。
![1002628-1d23f0c6d52cc944.png](https://upload-images.jianshu.io/upload_images/1002628-1d23f0c6d52cc944.png)
jvm_for_loop2
istore_1同样隐式包含了int类型1操作数，表示从操作数栈栈顶弹出一个int类型的值，存放到第一个局部变量里面。(为什么是第一个呢，因为这是一个实例方法第0个局部变量永远是当前对象this)。
![1002628-9d4f4d9ae38aac21.png](https://upload-images.jianshu.io/upload_images/1002628-9d4f4d9ae38aac21.png)
jvm_for_loop3
无条件跳转到偏移量为8的指令段执行，第一次循环量i的值是不加的
![1002628-59c9d478d9740312.png](https://upload-images.jianshu.io/upload_images/1002628-59c9d478d9740312.png)
jvm_for_loop4
iload_1同样包含隐式操作数，表示将第1个局部变量的值加载到操作数栈，bitpush 100表示将int类型100值压入栈。
![1002628-82c473ce5697eef7.png](https://upload-images.jianshu.io/upload_images/1002628-82c473ce5697eef7.png)
jvm_for_loop5
if_icmplt 5表示将操作数栈弹出，比较两位的值如果i小于100则跳转到偏移量为5的地方执行，否则执行return操作。
![1002628-bd5b879031a0fd59.png](https://upload-images.jianshu.io/upload_images/1002628-bd5b879031a0fd59.png)
jvm_for_loop6
某些局部变量需要频繁操作，Java虚拟机也做了相应的支持，iinc 1 1的作用是对第1个局部变量加1操作。
![1002628-1964aa80ec4fe3f7.png](https://upload-images.jianshu.io/upload_images/1002628-1964aa80ec4fe3f7.png)
jvm_for_loop7
然后继续重复上面的操作，直到局部变量的值操作100。
Java虚拟机是基于栈架构设计的，大多数操作从当前栈幀的操作数栈取出1个或者多个操作数，如果指令有计算结果压入操作数栈。没调用一个方法都会创建一个新的栈幀，并创建对应方法所需的操作数栈和局部变量表。
将上面的i的数据类型换成short，double有不同的编译代码，由于Java虚拟机指令不能超过256条，不可能对每种数据类型操作都尽善，例如缺乏对byte char short数据类型直接操作的支持，需要转换为int类型，代价就是将他们长度扩展为4字节。对于浮点型，缺少条件转移指令。
#### 2.算数运算
Java虚拟机基于操作数栈来进行算术运算，运算指令的操作数从操作数栈弹出，如果有运算结果会被放回操作数栈(除了前面提到的iinc指令直接对局部变量表进行操作)。
#### 3.访问运行时常量池
很多数值常量，对象，字段，方法都是通过当前类的运行时常量进行访问。ldc指令用于访问运行时常量池中的对象，当使用的运行时常量多于256个时，用ldc_w来代替访问。特别地，当访问类型为double和long的运行时常量池项使用ldc2_w。
#### 4.更多的控制结构示例
while语句编译:
```
void whileInt(){
    int i = 0;
    while(i < 100) {
        i++;
    }
}
//编译后的代码
 0: iconst_0
 1: istore_1
 2: iload_1
 3: bipush        100
 5: if_icmpge     14
 8: iinc          1, 1
11: goto          2
14: return
```
虚拟机对各种数据结构的控制结构采用了相似的方式编译，只是根据不同数据类型使用不同的指令访问。这么做多少会使编译代码效率降低，需要更多的指令来实现相应的数据类型判断。浮点型数据的比较指令:float-----fcmpl和fcmpg,double-----dcmpl和dcmpg，语义相似，对待NaN(Not A Number)变量时有所区别，具体参考指令描述。
#### 5.接收参数
如果传递了n个参数给某个实例方法，则当前的栈幀做将它他们保存到第1个到第n个局部变量中，因为第0个局部变量时当前对象的引用this。如果是静态方法则从第0个局部变量开始。
实例方法:
```
int add(int i,int j) {
        return i + j;
}
//-------------------
 0: iload_1
 1: iload_2
 2: iadd
 3: ireturn
```
静态方法:
```
static int sub(int i,int j) {
    return i - j;
}
//-------------------
0: iload_0
1: iload_1
2: isub
3: ireturn
```
#### 6.方法调用
invokevirtual 普通方法实例调用根据运行时对象类型进行分派，也就是多态：
```
int add2(int i,int j) {
    return add(i,j);
}
//-----------------------
 0: aload_0
 1: iload_1
 2: iload_2
 3: invokevirtual #8                  // Method add:(II)I
 6: ireturn
```
aload_0加载局部变量this到操作数栈，iload_1 iload_2加载第1个局部变量和第二个局部变量到操作数栈，invokevirtual调用实例方法，ireturn会把当前操作数栈的栈顶值（就是add(int,int)方法的返回值）压入调用add2()方法的操作数栈，然后切换栈幀。
invokestatic指令用于调用类的静态方法，比较类似，操作数栈里面不需要this变量:
```
int add3(int i,int j){
    return addStatic(i,j);
}
int addStatic(int i,int j) {
    return i + j;
}
//-------------------------
// int add3(int, int);
 0: iload_1
 1: iload_2
 2: invokestatic  #9                  // Method addStatic:(II)I
 5: ireturn
```
invokespecial用来调用父类方法和私有方法。
#### 7.类实例
Java虚拟机创建类实例通过 new 关键字创建，并且会调用虚拟机层面上的初始化方法:
```
Object createInstance() {
    return new Object();
}
//-------------------
 0: new           #3                  // class java/lang/Object
 3: dup
 4: invokespecial #1                  // Method java/lang/Object."<init>":()V
 7: areturn
```
如果有类实例有字段则通过putfield来赋值，并且在构造方法里面会显式调用父类的的构成方法：
```
public Hello(int age) {
    this.age = age;
}
//---------------------
 0: aload_0
 1: invokespecial #1                  // Method java/lang/Object."<init>":()V
 4: aload_0
 5: iload_1
 6: putfield      #2                  // Field age:I
 9: return
```
#### 8.数组
使用newarray指令创建基本类型数组:
```
public int[] createIntArray() {
    int[] a = new int[10];
    a[0] = 1;
    return a;
}
//---------------------
 0: bipush        10
 2: newarray       int
 4: astore_1
 5: aload_1
 6: iconst_0
 7: iconst_1
 8: iastore
 9: aload_1
10: areturn
```
iastore赋值之后会将三个元素都出栈，引用类型的aastore也是类似。
使用anewarray创建引用类型数组:
```
public Object[] createObjArray() {
    Object[] objects= new Object[10];
    objects[0] = this;
    return objects;
}
//-------------------------
 0: bipush        10
 2: anewarray     #3                  // class java/lang/Object
 5: astore_1
 6: aload_1
 7: iconst_0
 8: aload_0
 9: aastore
10: aload_1
11: areturn
```
使用multianewarray指令创建多维数组:
```
public int[][] createMultiIntArray() {
    int[][] a = new int[5][5];
    a[0][0] = 1;
    return a;
}
//--------------------
0: iconst_5
 1: iconst_5
 2: multianewarray #12,  2            // class "[[I"
 6: astore_1
 7: aload_1
 8: iconst_0
 9: aaload
10: iconst_0
11: iconst_1
12: iastore
13: aload_1
14: areturn
```
#### 9.编译switch语句
编译器会通过分支条件判断采用tableswitch或lookupswitch指令,当分支条件密集采用tableswitch来编译，类似数组索引的方式，只需要一次查找:
```
public int choose(int index){
    switch(index) {
        case 0:
            return 0;
        case 1:
            return 2;
        case 2:
            return 3;
        default:
            return -1;
    }
}
//------------------------------------------
 0: iload_1
 1: tableswitch   { // 0 to 2
               0: 28
               1: 30
               2: 32
         default: 34
    }
28: iconst_0
29: ireturn
30: iconst_2
31: ireturn
32: iconst_3
33: ireturn
34: iconst_m1
35: ireturn
```
当分支条件值比较稀疏时，tableswitch指令空间使用率比较低。这种情况就会使用lookupswitch指令来代替，Java虚拟机会指令的索引表进行排序，采用非线性的搜索来(如二分法)确定key值:
```
public int choose(int index){
    switch(index) {
        case 0:
            return 0;
        case 100:
            return 2;
        case 200:
            return 3;
        default:
            return -1;
    }
}
//---------------------------
 0: iload_1
 1: lookupswitch  { // 3
               0: 36
             100: 38
             200: 40
         default: 42
    }
36: iconst_0
37: ireturn
38: iconst_2
39: ireturn
40: iconst_3
41: ireturn
42: iconst_m1
43: ireturn
```
注意，虚拟机中指令tableswitch和lookupswitch分支条件的值类型只能为int，Java语言中支持其他类型值的switch都会通过某些方式的转换变成int类型。
例如，在Java SE7 中，可以使用String来作为switch的分支条件，其实是转换为hashcode来实现的，所以这里就会有一个坑，如果字符串的hashcode的值一样就会出现分支判断错误的情况，所以在比比较String的时候，使用if-else结合equals来判断比较好。
#### 使用操作数栈
Java虚拟机是基于栈架构的，很多指令操作的对象都是操作数栈，比如前面的算数运行，方法的调用，都是在操作数栈进行的，下面一个是成员变量自增的操作。
```
int index = -1;
public void setIndex() {
    index++;
}
//-----------------
 0: aload_0
 1: dup
 2: getfield      #2                  // Field index:I
 5: iconst_1
 6: iadd
 7: putfield      #2                  // Field index:I
10: return
```
> 
题外话，从这里编译的字节码可以看得出i++并不是原子性的操作，类似的还有new关键字
注意操作指令不允许操作那些不可拆分的操作数，例如double,long类型的操作数。
#### 抛异常与异常处理
虚拟机抛异常通过指令athrow来实现，先初始化异常类，然后athrow指令抛出。athrow操作数栈顶值必须是一个指向Throwable或者其类型实例的引用。
```
public int testThrowEx(int i) {
    if(i == 0) {
        throw new RuntimeException();
    }
    return i;
}
//----------------------
 0: iload_1
 1: ifne          12
 4: new           #14                 // class java/lang/RuntimeException
 7: dup
 8: invokespecial #15                 // Method java/lang/RuntimeException."<init>":()V
11: athrow
12: iload_1
13: ireturn
```
对于异常处理，每个try-catch语句块都会对应一个异常处理器，由异常处理器组成一个异常表:
```
public int testCatchEx(int i) {
    try {
        testThrowEx(i);
    }catch(RuntimeException e){
        i++;
    }catch(Exception e){
        i--;
    }
    return i;
}
//---------------
 0: aload_0
 1: iload_1
 2: invokevirtual #16                 // Method testThrowEx:(I)I
 5: pop
 6: goto          20
 9: astore_2
10: iinc          1, 1
13: goto          20
16: astore_2
17: iinc          1, -1
20: iload_1
21: ireturn
  Exception table:
 from    to  target type
     0     6     9   Class java/lang/RuntimeException
     0     6    16   Class java/lang/Exception
```
注意，这里异常表的偏移量包含了from，并不包含to，也就是try-catch语句块偏移量 index >= from ,index < to。
#### 编译finally语句块
早期的虚拟机版本finally语句块的实现是使用jsr,jsr_w,ret来实现，Java 7开始就禁止出现jsr,jsr_w指令。所以异常处理器就使用了新的方式实现，一个带finally语句块的try语句，在编译的时候会生成一个特殊的异常处理器any，这个异常处理器会捕获所有异常，
```
public int testCatchEx(int i) {
    try {
        testThrowEx(i);
    }catch(RuntimeException e){
        i++;
    }catch(Exception e){
        i--;
    }finally {
        i++;
    }
    return i;
}
//-------------------------
 0: aload_0
 1: iload_1
 2: invokevirtual #16                 // Method testThrowEx:(I)I
 5: pop
 6: iinc          1, 1
 9: goto          38
12: astore_2
13: iinc          1, 1
16: iinc          1, 1
19: goto          38
22: astore_2
23: iinc          1, -1
26: iinc          1, 1
29: goto          38
32: astore_3
33: iinc          1, 1
36: aload_3
37: athrow
38: iload_1
39: ireturn
  Exception table:
 from    to  target type
     0     6    12   Class java/lang/RuntimeException
     0     6    22   Class java/lang/Exception
     0     6    32   any
    12    16    32   any
    22    26    32   any
```
try语句块，catch语句块，执行结束后都会执行finally里面的指令。
#### 同步
方法级的同步的通过设置方法的ACCESS_FLAGS来隐式指定，对应的flag为ACC_SYNCHRONIZED:
```
public synchronized void setIndex() {
    index++;
}
//-----------------
public synchronized void setIndex();
    descriptor: ()V
    flags: ACC_PUBLIC, ACC_SYNCHRONIZED
```
同步语句块通过monitorenter，monitorexit来实现，两指令的操作数都必须为对象的引用类型，任何一个对象都有一个monitor与之关联，当线程执行到monitorenter指令时，指令会尝试获取对象对应的monitor的所有权，如果monitor的计数器为0，那么线程将持有对象的monitor，如果其他线程已经持有monitor,那么当前线程将会被阻塞，直到其他线程释放对象的monitor。 如果当前线程已经持有，那么monitor的计数器加1。
monitorexit则是当monitor的计数器为0时，则释放monitor对象。
```
public void setIndex() {
    synchronized(this) {
        index++;
    }
}
//------------------
 0: aload_0
 1: dup
 2: astore_1
 3: monitorenter
 4: aload_0
 5: dup
 6: getfield      #2                  // Field index:I
 9: iconst_1
10: iadd
11: putfield      #2                  // Field index:I
14: aload_1
15: monitorexit
16: goto          24
19: astore_2
20: aload_1
21: monitorexit
22: aload_2
23: athrow
24: return
  Exception table:
 from    to  target type
     4    16    19   any
    19    22    19   any
```
无论方法是正常调用结束，还是异常调用结束，monitorexit都必须配对执行。
#### 注解
注解保存在Class文件RuntimeVisibleAnnotations和RuntimeInvisibleAnnotations属性中，例如:
```
@Deprecated
public void setIndex() {
}
//----------------------
public void setIndex();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=0, locals=1, args_size=1
         0: return
      LineNumberTable:
        line 104: 0
    Deprecated: true
    RuntimeVisibleAnnotations:
      0: #57()
```
