# JAVA中的一维数组 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:44:43[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：744标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)









- Java中也有数组，新建一个数组的格式为：double[] numbers = new double [NUMBER_OF_ELEMENTS]; 这种形式下是把申明一个数组变量、创建数组、然后将数组引用赋值给变量这3个部分都融合在一起。而使用数组中的元素的格式为：numbers [i];
- 数组变量实际上存储的是指向数组的引用，不过多数情况下它们的差别是可以忽略的； 
- 数组的初始化中，最好将声明、创建和赋值融合在一条语句中执行，这样就不会出错。也可以使用先new，再赋值的做法。注意：一定都要使用new 元素类型 [元素个数] 的形式来确定数据的元素个数，不能够直接赋值；
- 在打印输出一个数组的时候，必须使用for循环来输出。不过，当数组元素为char类型的时候，我们可以使用System.out.println (数组名); 的形式来直接输出整个数组；
- 在Java中，对于数组的遍历输出还有一个for-each循环， 格式如下：for (double u: myList) {System.out.println (u); } 。其中，u是与数组元素类型相同的变量，即“对myList中每个元素u进行一下操作”；
- 数组的复制。直接使用list2=list1; 的语句只是复制了基本数据类型的变量，不能复制数组，这是因为list1, list2都只是指向数组的引用，是一个数组变量。而对于复制数组有3种方式：使用循环语句逐个复制元素； 使用System类中的静态方法arraycopy; 使用clone方法复制数组；
- 可以在数组变量名后面使用：arrayName.length来计算数组的长度；
- 对于arraycopy方法，使用方式为：System.arrarcopy (sourceArray, src_pos, targetArray, tar_pos, length);   src_pos和tar_pos都是起始位置，length为复制的长度。注意：arraycopy方法只负责复制，不能初始化一个数组，所以之前需要语句：int [] targetArray
 = new int [sourceArray.length]; 这样的声明和新建目标数组的语句；一定要是System.arraycopy；
- 匿名数组： new int [] {1,2,3,4,5}; 该数组没有使用显式地引用变量，一般出现在给方法传递数组的场合；
- 给方法传递数组的时候，如果改变方法中的数组，则方法外的数组也会被修改；
- Java对于方法，允许可变长参数列表。例如：public static void printMax (double ... numbers) {...}  其中参数表是(类型名 ... 参数名)。需要注意的是，此处的参数个数可变，但是参数一定要相同。 
- 二分查找法的前提条件是：该数组一定要已经排序好的。如果查找到，则返回下标；如果查找不到该元素，那么方法返回的最好的-1-low，因为此时的-1-low正好是该元素应该在数组中的位置下标的相反数（已排序）；
- 介绍了选择排序法和插入排序法；
- Java语言中有Arrays类，其中集成了许多实现数组的排序、查找、比较、填充方法。java.util.Arrays.sort()是排序；java.util.Arrays.binarySearch()是二分法查找；java.util.Arrays.equals()是判断两个数组是否相等；java.util.Arrays.fill()是填充数组；




