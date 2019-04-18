# Java基础知识六——数组 - weixin_33985507的博客 - CSDN博客
2018年02月01日 21:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
# 一、数组
  存储同一种数据类型的多个元素的容器
定义格式
  A：数据类型[] 数组名;（推荐的使用方法）
  B：数据类型 数组名[];
举例：
  int [] arr;  定义了一个int类型的数组，数组名是arr
  int arr [];  定义了一个int类型的变量，变量名是arr数组
# 二、数组的初始化
  所谓的初始化，其实就是为数组开辟内存空间，并为数组中的每个元素赋予初始值。
数组的初始化有两种方式：
- 动态初始化：只给出长度，由系统输出初始化值
- 静态初始化：给出初始化值，由系统决定长度
## 1、动态初始化
  数据类型[] 数组名 = new 数据类型[数组长度]；
例：
```
int[] arr = new int[3];
//输出数组名
System.out.println("arr:"+arr);//打印的是数组的地址值
//输出数组的元素：数组名[索引]
System.in.println("arr[0]:"+arr[0]);//0
```
注意：
  1、索引是数组中元素的编号，标号从0开始，最大的编号是数组的长度-1
  2、数组的动态初始化会由系统给数组附一个初始值：
  int 类型——0
  double类型——0.0
  String类型——null
  boolean类型——false
## 2、静态初始化
  数据类型[] 数组名 = new 数据类型[]{元素1,元素2,元素3,...}
举例：
```
int [] arr= new int[]{1,2,3};
```
简化的格式：
  数据类型[] 数组名 = {元素1,元素2,元素3,...}
举例：
```
int[] arr = { 1, 2, 3 };
```
# 三、Java中的内存分配
- 栈：存储的是局部变量
- 堆：存储的是new出来的东西
- 方法区：面向对象部分说明
- 本地方法区：和系统相关
- 寄存器：给CPU使用的
## 1、栈
  栈存储的是局部变量，也就是定义在方法中的变量。
  使用完毕，立即回收。
## 2、堆
  堆中存储的是new出来的东西、实体、对象。
  A：每一个对象都有地址值
  B：每一个对象的数据都有默认值
  byte,short,int,long    0
  float,double      0.0
  char         "/u0000"
  boolean        false
  引用类型        null
  C：数据使用完毕后，会在垃圾回收器空闲的时候被回收
# 四、Java中数组的内存图解
## 1、图解1
  定义一个数组，输出数组名及元素，然后给数组中的元素赋值，再次输出数组名及元素
```
public class ArrayDemo {
    public static void main(String[] args) {
        int[] arr = new int[3];
        System.out.println(arr);// 地址值：001
        System.out.println(arr[0]);// 值：0
        System.out.println(arr[1]);// 值：0
        System.out.println(arr[2]);// 值：0
    }
}
```
![10374225-c541663ff1d531ae.png](https://upload-images.jianshu.io/upload_images/10374225-c541663ff1d531ae.png)
## 2、图解2
  定义两个数组，分别输出数组名及元素，然后分别给数组中的元素赋值，分别再次输出数组名及元素
```
public class ArrayDemo2 {
    public static void main(String[] args) {
        // 定义两个数组
        int[] arr = new int[2];
        int[] arr2 = new int[3];
        // 分别输出数组名及元素
        System.out.println("arr:" + arr);// 地址值：001
        System.out.println("arr[0]" + arr[0]);// 值：0
        System.out.println("arr[1]" + arr[1]);// 值：0
        System.out.println("---------------------");
        System.out.println("arr2:" + arr2);// 地址值：002
        System.out.println("arr2[0]" + arr2[0]);// 值：0
        System.out.println("arr2[1]" + arr2[1]);// 值：0
        System.out.println("arr2[2]" + arr2[2]);// 值：0
        System.out.println("-----------------------");
        // 然后分别给数组中的元素赋值
        arr[1] = 100;
        arr2[0] = 200;
        arr2[1] = 300;
        // 分别再次输出数组名及元素
        System.out.println("arr:" + arr);// 地址值：001
        System.out.println("arr[0]" + arr[0]);// 值：0
        System.out.println("arr[1]" + arr[1]);// 值：100
        System.out.println("---------------------");
        System.out.println("arr2:" + arr2);// 地址值：002
        System.out.println("arr2[0]" + arr2[0]);// 值：200
        System.out.println("arr2[1]" + arr2[1]);// 值：300
        System.out.println("arr2[2]" + arr2[2]);// 值：0
        System.out.println("-----------------------");
    }
}
```
![10374225-afd8053d54712392.png](https://upload-images.jianshu.io/upload_images/10374225-afd8053d54712392.png)
## 3、图解3
  定义两个数组，先定义一个数组，赋值输出。然后定义第二个数组的时候把第一个数组的地址赋值给第二个数组，然后给第二个数组赋值，再次输出两个数组名及元素。
```
public class ArrayDemo3 {
    public static void main(String[] args) {
        // 先定义一个数组
        int[] arr = new int[3];
        // 赋值
        arr[0] = 100;
        arr[1] = 200;
        arr[2] = 300;
        // 输出
        System.out.println("arr:" + arr);// 地址值：001
        System.out.println("arr[0]" + arr[0]);// 值：100
        System.out.println("arr[1]" + arr[1]);// 值：200
        System.out.println("arr[2]" + arr[2]);// 值：300
        System.out.println("-----------------------");
        // 然后定义第二个数组的时候把第一个数组的地址赋值给第二个数组
        int[] arr2 = arr;
        // 然后给第二个数组赋值
        arr2[0] = 111;
        arr2[1] = 222;
        arr2[2] = 333;
        // 再次输出两个数组名及元素
        System.out.println("arr:" + arr);// 地址值：001
        System.out.println("arr[0]" + arr[0]);// 值：111
        System.out.println("arr[1]" + arr[1]);// 值：222
        System.out.println("arr[2]" + arr[2]);// 值：333
        System.out.println("---------------------");
        System.out.println("arr2:" + arr2);// 地址值：001
        System.out.println("arr2[0]" + arr2[0]);// 值：111
        System.out.println("arr2[1]" + arr2[1]);// 值：222
        System.out.println("arr2[2]" + arr2[2]);// 值：333
        System.out.println("-----------------------");
    }
}
```
![10374225-8c03618829d9e7fd.png](https://upload-images.jianshu.io/upload_images/10374225-8c03618829d9e7fd.png)
# 五、数组的遍历
标准格式：
```
for(int x=0; x<arr.length; x++) {
            System.out.println(arr[x]);
}
```
举例：
```
public class ArrayDemo4 {
    public static void main(String[] args) {
        //定义一个数组
        int[] arr = {11,22,33,44,55};
        
        //原始做法
        System.out.println(arr[0]);
        System.out.println(arr[1]);
        System.out.println(arr[2]);
        System.out.println(arr[3]);
        System.out.println(arr[4]);
        System.out.println("------------------");
        
        //利用循环遍历数组
        for(int x=0; x<5; x++) {
            System.out.println(arr[x]);
        }
        System.out.println("------------------");
        System.out.println("arr数组共有"+arr.length+"个元素");
```
应用：求数组中的最大值
```
public class ArrayDemo5 {
    public static void main(String[] args) {
        //定义数组
        int[] arr = {12,98,45,73,60};
        
        //定义参照物
        int max = arr[0];
        
        //从第二个数开始遍历数组进行比较
        for(int x=1; x<arr.length; x++) {
            if(arr[x] > max) {
            //如果元素比max大，那么就继续留下来当参照物
                max = arr[x];
            }
        }
        System.out.println("数组中的最大值是："+max);
    }
}
```
# 六：数组操作的两个小问题
## 1、数组索引越界——ArrayIndexOutOfBoundsException
  访问到了数组中的不存在的索引时发生。
## 2、空指针异常——NullPointerException
  数组引用没有指向对象，却在操作对象中的元素时发生。
# 七、二维数组
一维数组：用来存储统一数据类型元素的容器
二维数组：用来装一维数组的
## 1、动态初始化
格式：
```
数据类型[][] 数组名 = new 数据类型[二维数组长度][一维数组长度];
```
举例：
```
public class ArrayDemo6 {
    public static void main(String[] args) {
        // 定义一个数组
        int [][] arr = new int[3][2];
        
        System.out.println(arr[0]);//返回的是数组的地址值
        System.out.println("arr[0][0]="+arr[0][0]);//值：0
        System.out.println("arr[0][1]="+arr[0][1]);//值：0
        System.out.println("----------------------");
        
        //修改二维数组内元素的值
        arr[0][0] = 100;
        arr[0][1] = 200;
        
        //再次输出
        System.out.println("arr[0][0]="+arr[0][0]);//值：100
        System.out.println("arr[0][1]="+arr[0][1]);//值：200
        System.out.println("----------------------");
    }
}
```
## 2、二维数组的静态初始化
格式1：
```
数据类型[][] 数组名 ={{},{},{},...};//每一个大括号都是一个一维数组
```
格式2：
```
//创建一个长度为3的二维数组，里面的每一个元素为null
int[][] arr = new int[3][];
//这时需要我们给他填装一维数组
arr[0] = new int[3];
arr[1] = new int[6];
arr[2] = new int[100];
```
## 3、二维数组的遍历
```
for(int i = 0; i < arr.length; i++){
    for(int j = 0; j < arr.length; j++){
        System.out.println(arr[i][j]);
    }
}
```
