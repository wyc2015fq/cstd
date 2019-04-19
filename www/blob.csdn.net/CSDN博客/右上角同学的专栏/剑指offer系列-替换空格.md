# 剑指offer系列-替换空格 - 右上角同学的专栏 - CSDN博客
2016年06月28日 21:45:58[右上角徐](https://me.csdn.net/u011032983)阅读数：216
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * 替换空格 题目描述：请实现一个函数，将一个字符串中的空格替换成“%20”。 例如，当字符串为We Are
 * Happy.则经过替换之后的字符串为We%20Are%20Happy。
 */
public class offert42 {
	public String replace(StringBuffer oldstr) {
		int count = 0;
		int newlength = 0;
		char[] newarray = null;// 新字符数组
		// 统计空格个数
		for (int i = 0; i < oldstr.length(); i++) {
			if (oldstr.charAt(i) == ' ')
				count++;
		}
		newlength = oldstr.length() + ("%20".length() - 1) * count;
		newarray = new char[newlength];
		// 基本思路是：从后往前，不是空格就把old里的元素给new,是空格就把“%20”里的元素
		// 给new,简单来说就是，遇到空格后“%20”相当于old，new还是new。
		for (int i = oldstr.length() - 1, j = newlength - 1; i >= 0 && j >= 0; i--, j--) {
			if (oldstr.charAt(i) != ' ')
				newarray[j] = oldstr.charAt(i);
			else {
				for (int h = "%20".length() - 1; h >= 0 && j >= 0; h--, j--)
					newarray[j] = "%20".charAt(h);
				j++;// 因为在最后h<0循环结束时，j也多减了1，这里要补回来
			}
		}
		return String.valueOf(newarray);
	}
	public static void main(String[] args) {
		StringBuffer str = new StringBuffer("we are happy");
		// String str2=null;
		// str=s.nextLine();
		offert42 o = new offert42();
		String str2 = o.replace(str);
		System.out.print(str2);
	}
}
```
