
# Java方法-System.arraycopy方法和Arrays.copyOf() - wangzhiguo9261的博客 - CSDN博客


2018年02月24日 14:02:55[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：85


System.arraycopy方法：如果是数组比较大，那么使用System.arraycopy会比较有优势，因为其使用的是内存复制，省去了大量的数组寻址访问等时间
**[java]**[view plain](http://blog.csdn.net/ochangwen/article/details/50587937#)[copy](http://blog.csdn.net/ochangwen/article/details/50587937#)
publicstaticnativevoidarraycopy(Object src,intsrcPos,
Object dest,intdestPos,
intlength);

复制指定源数组src到目标数组dest。**复制从src的srcPos索引开始**，复制的个数是length，复制到dest的索引从destPos开始。
### 1、基本使用
**[java]**[view plain](http://blog.csdn.net/ochangwen/article/details/50587937#)[copy](http://blog.csdn.net/ochangwen/article/details/50587937#)
@Test
publicvoidtestCopy() {
int[] ids = {1,2,3,4,5};
// 1、测试复制到别的数组上
// 将ids数组的索引从0开始其后5个数，复制到ids2数组的索引从0开始
int[] ids2 =newint[5];
System.arraycopy(ids,0, ids2,0,5);
System.out.println(Arrays.toString(ids2));// [1, 2, 3, 4, 5]

// 2、测试自我复制
System.arraycopy(ids,0, ids,3,2);
System.out.println(Arrays.toString(ids));// [1, 2, 3, 1, 2]

// 3、如果是类型转换问题
Object[] o1 = {1,2,3,4.5,6.7};
Integer[] o2 =newInteger[5];
try{
System.arraycopy(o1,0, o2,0, o1.length);
}catch(ArrayStoreException ex) {
// 发生存储转换，部分成功的数据会被复制过去
System.out.println("拷贝发生异常：数据转换错误，无法存储。");
}
// 从结果看，前面3个可以复制的数据已经被存储了。剩下的则没有
System.out.println(Arrays.toString(o2));// [1, 2, 3, null, null]
}

### 2、复制后改变复制后的数组
如果是复制一个一维数组，那么改变复制后的数组并不影响原数组。但是如果复制一个二维数组，那么改变其中任何一个数组，那么另一个的值也发生了变化。开始不是很明白，后来上网查了查资料，理解了其中奥妙。
java其实没有二维数组的概念，平常实现的二维数组只是元素是一维数组的一维数组，而数组也是引用类型，继承自Object类。数组是new出来的。这些性质也就导致arraycopy()二维数组时出现的问题。
如果是一维数组，那么元素都是基础类型（如int，double等），使用arraycopy()方法后，是把原数组的值传给了新数组，属于值传递。而如果是二维数组，数组的第一维装的是一个一维数组的引用，第二维里是元素数值。对二维数组应用arraycopy()方法后，第一维的引用被复制给新数组的第一维，也就是两个数组的第一维都指向相同的“那些数组”。而这时改变其中任何一个数组的元素的值，其实都修改了“那些数组”的元素的值，所以原数组和新数组的元素值都一样了。
**[java]**[view plain](http://blog.csdn.net/ochangwen/article/details/50587937#)[copy](http://blog.csdn.net/ochangwen/article/details/50587937#)
@Test
publicvoidtestCopy2(){
int[] s1 = {1,2,3,4,5};
int[] s2 =newint[5];
System.arraycopy(s1,0, s2,0,5);
System.out.println("This is s1");
for(intaS1 : s1) {
System.out.print(aS1 +" , ");
}
s2[2] =111;
System.out.println("\nThis is s2");
for(intaS2 : s2) {
System.out.print(aS2 +" , ");
}
System.out.println("\nThis is s1");
for(intaS1 : s1) {
System.out.print(aS1 +" , ");
}
System.out.println("\n-----------------------");
//二维数组
int[][] s3 = {{1,2,3,4,5}, {6,7,8,9,10}};
int[][] s4 =newint[s3.length][s3[0].length];
System.out.println("This is s3");
System.arraycopy(s3,0, s4,0, s3.length);
for(int[] aS3 : s3) {
for(intj =0; j < s4[0].length; j++) {
System.out.print(aS3[j] +",");
}
}
s4[1][3] =111;
System.out.println("\nThis is s4");
for(int[] aS4 : s4) {
for(intj =0; j < s4[0].length; j++) {
System.out.print(aS4[j] +",");
}
}
System.out.println("\nThis is s3");
for(int[] aS3 : s3) {
for(intj =0; j < s4[0].length; j++) {
System.out.print(aS3[j] +",");
}
}
}

结果：This is s1 ：1 , 2 , 3 , 4 , 5 ,
This is s2 ： 1 , 2 , 111 , 4 , 5 ,
This is s1 ： 1 , 2 , 3 , 4 , 5 ,
-----------------------
This is s3 ： 1,2,3,4,5,6,7,8,9,10,
This is s4 ： 1,2,3,4,5,6,7,8,111,10,
This is s3 ： 1,2,3,4,5,6,7,8,111,10,
### 3、Arrays.copyOf()
该方法对于不同的数据类型都有相应的方法重载。
**[java]**[view plain](http://blog.csdn.net/ochangwen/article/details/50587937#)[copy](http://blog.csdn.net/ochangwen/article/details/50587937#)
//复杂数据类型
publicstatic<T,U> T[] copyOf(U[] original,intnewLength, Class<?extendsT[]> newType) {
T[] copy = ((Object)newType == (Object)Object[].class)
? (T[])newObject[newLength]
: (T[]) Array.newInstance(newType.getComponentType(), newLength);
System.arraycopy(original,0, copy,0,
Math.min(original.length, newLength));
returncopy;
}
publicstatic<T> T[] copyOf(T[] original,intnewLength) {
return(T[]) copyOf(original, newLength, original.getClass());
}
由U类型复制为T类型？
original - 要复制的数组
newLength - 要返回的副本的长度
newType - 要返回的副本的类型**[java]**[view plain](http://blog.csdn.net/ochangwen/article/details/50587937#)[copy](http://blog.csdn.net/ochangwen/article/details/50587937#)
//基本数据类型（其他类似byte，short···）
publicstaticint[] copyOf(int[] original,intnewLength) {
int[] copy =newint[newLength];
System.arraycopy(original,0, copy,0,
Math.min(original.length, newLength));
returncopy;
}
观察其源代码发现copyOf()，在其内部创建了一个新的数组，然后调用arrayCopy()向其复制内容，返回出去。
总结：
1.copyOf()的实现是用的是arrayCopy();
2.arrayCopy()需要目标数组，对两个数组的内容进行可能不完全的合并操作。
3.copyOf()在内部新建一个数组，调用arrayCopy()将original内容复制到copy中去，并且长度为newLength。返回copy;
4.arraycopy 方法会因为新数组大小比久数组大小小而报IndexOutOfBoundsException;
copyOf 则不会因此报错，因为copyOf 的返回值是在内部new 好的copy 数组，而该copy 数组new 的大小就等于newLength ，
故即使在客户端指定好了新数组newArray 的大小，接收到返回值后也是指向底层new 出来的数组copy 。换句话说( 也可以因此推出其他的区别) ，在客户端代码中即使不给新数组new 对象，如：String[] newStr = null;那么对于arraycopy 是会报NullPointerException 的错误的，而对于java.util.Arrays 中的copyOf 方法则由于jdk 底层已经new 出了对象而不会报该错误！不过需要特别注意的是：copyOf 方法最后也是调用System.arraycopy 的方法，不过由于前面的准备，异常情况就不会出现了。

