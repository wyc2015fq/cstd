
# 剑指offer——数组中出现次数超过一半的数字 - manduner的博客 - CSDN博客


2019年04月08日 16:53:06[Manduner_TJU](https://me.csdn.net/manduner)阅读数：10标签：[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
数组中有一个数字出现的次数超过数组长度的一半，请找出这个数字。例如输入一个长度为9的数组{1,2,3,2,2,2,5,4,2}。由于数字2在数组中出现了5次，超过数组长度的一半，因此输出2。如果不存在则输出0。
# 2，解题思路
## （1）思路一：
遍历数组，统计每个元素出现的次数。思路简单，但是**时间复杂度是O(n^2).**
## （2）思路二：阵地攻守。
阵地攻守思想：先找出可能的目标元素，再统计该元素出现的次数。时间复杂度是O(n).
第一个数字作为第一个士兵，守阵地；count = 1；
遇到相同元素，count++;
遇到不相同元素，即为敌人，同归于尽,count--；
当遇到count为0的情况，又以新的i值作为守阵地的士兵.
继续下去，到最后还留在阵地上的士兵，有可能是目标元素。
验证该元素出现的次数是否满足要求(出现[1,1,2,2]或者[1,2,3,4]的情况不满足要求，所以需要验证一下).
# 3，源码
```python
/**
* @author Manduner_TJU
* @version 创建时间：2019年4月8日下午3:24:13
*/
public class MoreThanHalfNum {
	public static void main(String[] args) {
		int[] a = {1,2,3,2,2,2,5,4,2};
		//int result = MoreThanHalfNum_Solution(a);
		int result = MoreThanHalfNum_Solution2(a);
		System.out.println(result);
	}
	
    //1，遍历数组，统计每个元素出现的次数。思路简单，但是时间复杂度是O(n^2).
	public static int MoreThanHalfNum_Solution(int [] array) {
    	int len = array.length;
    	if(len == 0) return 0;
    	int result=0;
        count=0;
    	for(int i=0;i<len;i++) {
    		if(count(array[i],array)*2 > len) {
    			result = array[i];
    			break;
    		}
    	}
		return result;
    }
	public static int count(int x, int[] array) {
		int num=0;
		for(int i=0;i<array.length;i++) {
			if(x==array[i]) {
				num++;
			}
		}
		
		return num;
	}
	
	/**2,阵地攻守思想：先找出可能的目标元素，再统计该元素出现的次数。时间复杂度是O(n).
	 * 第一个数字作为第一个士兵，守阵地；count = 1；
	 * 遇到相同元素，count++;
	 * 遇到不相同元素，即为敌人，同归于尽,count--；
	 * 当遇到count为0的情况，又以新的i值作为守阵地的士兵.
	 * 继续下去，到最后还留在阵地上的士兵，有可能是目标元素。
	 * 验证该元素出现的次数是否满足要求(出现[1,1,2,2]或者[1,2,3,4]的情况不满足要求).
	 * @param a
	 * @return
	 */
    public static int MoreThanHalfNum_Solution2(int[] a) {
    	int num = a[0];
    	int count = 1;
    	for(int i=1;i<a.length;i++) {
    		if(a[i]==num) count++;
    		else count--;
    		if(count==0) {
    			num = a[i];
    			count = 1;
    		}
    	}
    	
    	int result=0;
        count=0;
    	for(int i=0;i<a.length;i++) {
    		if(a[i]==num) count++;
    	}
    	
    	if(count*2>a.length) result = num;
		return result;
	}
	
}
```


