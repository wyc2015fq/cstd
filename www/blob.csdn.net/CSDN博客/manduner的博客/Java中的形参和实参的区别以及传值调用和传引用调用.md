
# Java中的形参和实参的区别以及传值调用和传引用调用 - manduner的博客 - CSDN博客


2019年04月02日 16:10:22[Manduner_TJU](https://me.csdn.net/manduner)阅读数：20


# 1，名词解析
（1）形参：形参出现函数定义中，在整个函数体内都可以使用，离开函数则不能使用。
（2）实参：实参出现在主函数中，进入被调函数后，实参变量也不能使用。
（3）形参和实参的功能是做数据传送。发生函数调用时，主调函数把实参的值传送给被调函数的形参从而向被调函数传送数据。
# 2，形参和实参的区别
（1）形参变量只有在被调用时才分配内存单元，在调用结束后，立即释放所分配的内存单元。因此，形参只有在函数内部有效。函数调用结束返回主调函数后则不能在使用该形参变量。
（2）实参可以变量、函数、数组等。无论实参是何种类型的，在进行函数调用时，他们都必须具有确定的值，一便把这些值传送给形参。
（3）形参和实参在类型、数量、顺序上应该保持严格的一致。
（4）函数在调用中发生的数据传递是单向的。即只能把实参的值传送给形参，而不能把形参的值反向传送给实参。因此，在函数调用过程中，形参的值发生改变，而实参中的值不会改变。
# 3，值传递和引用传递
（1）Java方法基本数据类型是传值，对象类型传引用，这是千真万确的。
（2）当参数是对象时，无论方法体内进行了何种操作，都不会改变实参对象的引用。
（3）当参数是对象时，只有在方法内部改变了对象的内容时，才会改变实参对象内容。
（4）按值传递意味着当将一个参数传递给一个函数时，函数接收的是原始值的一个副本。
（5）按引用传递意味着当将一个参数传递给一个函数时，函数接收的是原始值的内存地址，而不是值的副本。
（6）“在Java里面参数传递都是按值传递”这句话的意思是：按值传递是传递的值的拷贝，按引用传递其实传递的是引用的地址值，所以统称按值传递。
（7）在Java里面只有基本类型和按照下面这种定义方式的String是按值传递，其它的都是按引用传递。就是直接使用双引号定义字符串方式：String str = “Java”;
# 4，实例
## （1）引用传递
```python
public class Ex2 {
 
    public static void main(String[] args) {
 
        int[] a = {1,2,3,4,5};
 
        expand(a);
    
        printArray(a);//输出1 2 3 4 5
 
        changeArray(a);
 
        printArray(a);//输出10 2 3 4 5
 
    }
 
    public static void expand(int[] a){
 
        int[] newArray = new int[a.length * 2];
 
        System.arraycopy(a, 0, newArray, 0, a.length);
 
        a = newArray; //一定要记住，形参如论做何种操作，一定不会改变实参的引用
                  //所以，newArray赋值给了a, 只是形参引用上的改变，不会导致实参引用变化
 
    }
 
    public static void changeArray(int[] a){
 
        a[0] = 10;//形参的操作，导致了实参的内容变化，但是实参引用一定没有变
 
    }
 
    public static void printArray(int[] a){
 
        for(int i = 0; i<a.length; i++){
 
        System.out.print(a[i] + "\t");
 
        }
 
        System.out.println();
 
    }
 
}
```
## （2）值传递
```python
package listIt;
/**
* @author Manduner_TJU
* @version 创建时间：2019年4月2日下午3:38:12
*/
public class CanShuChuanDi {
	public static void main(String[] args) {
		 //值传递
		 int b = 3;
		 System.out.println("实参值为："+b);//实参值为：3
		 testVal(b);//形参值为：10
		 System.out.println("实参值为："+b);//实参值为：3
		}
		 
		public static void testVal(int b) {
			b = 10;
			System.out.println("形参值为："+b);
		}
}
```
5，参考资料
1，[Java实参和形参与传值和传引用](https://blog.csdn.net/wanliguodu/article/details/55653876)
2，[Java中的形参和实参的区别以及传值调用和传引用调用](https://blog.csdn.net/miniminiyu/article/details/52061401#commentBox)

