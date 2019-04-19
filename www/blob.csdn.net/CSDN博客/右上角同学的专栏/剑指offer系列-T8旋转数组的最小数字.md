# 剑指offer系列-T8旋转数组的最小数字 - 右上角同学的专栏 - CSDN博客
2016年06月30日 21:52:00[右上角徐](https://me.csdn.net/u011032983)阅读数：180
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
创作需要灵感，编程是一种创作，即使不能保证你的思想前无古人，也凝结着敲代码的人当下的思维结晶。说不定以后自己会看起来也会感叹，哦，那时的我在思考。
寻找旋转数组的最小数字，体现了二分查找的应用思想，不过要想考虑全面还是比较繁琐，既需要实现{3,4,5,1,2}这样的常规旋转数组，又要想到特殊情况如{1,2,3,4,5,6}，代码如下，略繁琐。
/**
 * @author xhl 旋转数组的最小数字 把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。
 *         输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。
 *         例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。
 *         NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。
 * 
 */
public class offerT8 {
/**
* @param args
*/
int Getmin(int[] array) {
if (array.length == 0)
return 0;
else {
int minnumber = Getmin(array, 0, array.length - 1);
return minnumber;
}
}
int Getmin(int[] array, int l, int r) {
int minnumber = 0;
if (array[(l + r) / 2] >= array[l] && array[(l + r) / 2] >= array[r]
&& r - l > 1)
        //只有在旋转数组如 { 3, 4, 5, 1, 2 }，中间指针落在前部分时，
//最小值才有可能在中间指针的后面,(array[(l + r) / 2] >= array[l] 
//&& array[(l + r) / 2] >= array[r]是为了辨别是这种情况，而不是落
                                                     //在后部分或者1,2,3,4,5这种特殊的旋转数组
minnumber = Getmin(array, (l + r) / 2, r);//继续在中间指针后面寻找
else if (r - l > 1)//没有到只有两个元素的结束状态
minnumber = Getmin(array, l, (l + r) / 2);//继续在中间指针前面寻找
else if (array[r] <= array[l])//在两个元素时递归结束，判断左右两个数大小
minnumber = array[r];
else
minnumber = array[l];
return minnumber;
}
public static void main(String[] args) {
// TODO Auto-generated method stub
int[] list = { 3, 4, 5, 1, 2 };
offerT8 o = new offerT8();
int minone = o.Getmin(list);
System.out.println(minone);
}
}
