# 剑指offer系列-T40找出数组中只出现一次的2个数 - 右上角同学的专栏 - CSDN博客
2016年09月04日 11:53:29[右上角徐](https://me.csdn.net/u011032983)阅读数：256
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
import java.util.ArrayList;
/**
 * 
 */
/**
 * @author xhl
 *题目:一个整型数组里除了两个数字之外,其他的数字都出现了两次。请写程序找出这两个只出现一次的数字。要求时间复杂度是O(n),空间复杂度是O(1)。
 *思路：1. 首先数组中所有元素依次异或，因为相同的元素异或得到0，所以最终的答案就等于那2个唯一的元素a^b<span style="white-space:pre">	</span>的值。
	2. 因为a,b不同，所以异或得到的答案肯定是不等于0的，那么我们就找到a^b的二进制表示中最后一个为1的位，假如是第k位。而a,b两个数在第k位上是不同的，一个为0，一个为1
	3. 接下来我们将第k位是1的分成一组，第k位是0的分成一组，如果2个元素相同，那么他们第k位肯定是一<span style="white-space:pre">	</span>样的，所以肯定被分到同一组中。而a,b则被分到2组中去了。
<span style="white-space:pre">	</span>然后我们就可以在每个分组中异或每一个元素，最终就可以得到那2个唯一的元素。
 */
public class offerT40 {
	/**
	 * @param args
	 */
	ArrayList<ArrayList> sort2part(int[] a,int index){
		int p=1<<index;
		ArrayList<ArrayList> aa=new ArrayList<ArrayList>();
		ArrayList<Integer> a1=new ArrayList<Integer>();
		ArrayList<Integer> a2=new ArrayList<Integer>();
		for(int i=0;i<a.length;i++){
			if((a[i]&p)==0){
				a1.add(a[i]);
			}
			else
				a2.add(a[i]);
		}
		aa.add(a1);
		aa.add(a2);
		return aa;
		
	}
	
	int findIndex(int[] a){
		int index=0;
		int res=a[0];
		for(int i=1;i<a.length;i++){
			res=res^a[i];
		}
		while((res&1)==0){
			res=res>>1;
			index++;
		}
		return index;
		
	}
	
	int[] find2num(int[] a){
		int index=findIndex(a);
		ArrayList<ArrayList> aa=sort2part(a,index);
		ArrayList<Integer> a1=aa.get(0);
		ArrayList<Integer> a2=aa.get(1);
		int[] res=new int[2];
		int num1=a1.get(0);
		int num2=a2.get(0);
		for(int i=0;i<a1.size();i++){
			num1=num1^a1.get(i);
		}
		for(int i=0;i<a2.size();i++){
			num2=num2^a2.get(i);
		}
		res[0]=num1;
		res[1]=num2;
		return res;
		
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT40 o=new offerT40();
		int[] a={2,4,3,6,3,2,5,5};
		int[] res=o.find2num(a);
		for(int i=0;i<res.length;i++){
			System.out.println(res[i]);
		}
	}
}
```
