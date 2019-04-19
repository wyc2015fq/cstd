# 剑指offer系列—T28字符串的排列 - 右上角同学的专栏 - CSDN博客
2016年07月16日 11:50:04[右上角徐](https://me.csdn.net/u011032983)阅读数：228
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
import java.util.ArrayList;
/**
 * @author xhl
 * 字符串的排列
 * 题目描述
 * 输入一个字符串,按字典序打印出该字符串中字符的所有排列。例如输入字符串abc,
 * 则打印出由字符a,b,c所能排列出来的所有字符串abc,acb,bac,bca,cab和cba。 结
 * 果请按字母顺序输出。 
	输入描述:
	输入一个字符串,长度不超过9(可能有字符重复),字符只包括大小写字母。
	思路：考察全排列，在博客《剑指offer系列-T12_1全排列》已经描述，不过此题
	需要考虑有重复元素的情况。
 * 
 */
public class offerT28 {
	/**
	 * @param args
	 */
	ArrayList<String> list = new ArrayList<String>();
	
	public void Permutation1(char[] chararray, int index) {
		if (index == chararray.length - 1) {
			list.add(String.valueOf(chararray));
		} else {
			for (int i = index; i < chararray.length; i++) {
				if (i != index && chararray[index] == chararray[i])//判断是否有相等的元素，相等则不用再交换顺序，因为效果和不交换一样
					continue;
				else {
					swap(chararray, i, index);
					Permutation1(chararray, index + 1);
					swap(chararray, i, index);
				}
			}
		}
	}
	public ArrayList<String> Permutation(String str) {
		Permutation1(str.toCharArray(), 0);
		return list;
	}
	void swap(char[] a, int i, int j) {
		char temp;
		temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT28 o = new offerT28();
		String str = "abc";
		ArrayList<String> list = o.Permutation(str);
		System.out.print(list);
	}
}
```
