
# Java实用方法整理（二）——正则表达式的应用 - manduner的博客 - CSDN博客


2018年03月15日 22:49:54[Manduner_TJU](https://me.csdn.net/manduner)阅读数：129标签：[Java																](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[正则表达式																](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



### 1，正则表达式的应用
#### （1）正则表达式应用——分割作用案例
```python
import java.util.Arrays;
/* 正则表达式的应用——分割功能
 * 需求：有如下字符串“
```
```python
88, 66; 99:       55, 22, 77;      33
```
```python
”，用代码实现输出“22 33 55 66 77 88 99”
 * 解决思路：
 * （1）分割字符串
 * （2）将字符串数组转换为整型数组
 * （3）排序
 * （4）输出要求的字符串
*/
public class RegexTest2 {
	public static void main(String[] args) {
		String s = "88, 66; 99:       55, 22, 77;      33";
		//1.分割字符串
		String[] str = s.split("\\p{Punct} +");
		
		//2.将字符串数组转换为整型数组
		int[] arry = new int[str.length];
		for(int i = 0; i < str.length; i++) {
			arry[i] = Integer.parseInt(str[i]);
		}
		
		//3.排序
		Arrays.sort(arry);
		
		//4.输出要求的字符串
		StringBuilder strb = new StringBuilder();
		for(int i = 0; i < arry.length; i++) {
			strb.append(arry[i] + " ");
		}
		
		System.out.println(strb.toString().trim());
	}
}
```
#### （2）正则表达式应用——判断功能案例
```python
/* 正则表达式应用——判断功能案例
 * 需求：判断给定邮箱是否为邮箱
 * 邮箱格式要求：#####@>>>.com,其中###代表不少于3位的数字,>>>代表不少于两位的数字或字母
 * 思路：
 * 1，创建输入对象，用于接收输入邮箱
 * 2，根据规则写出判断规则regex
 * 3，判断邮箱是否满足规则
 * 
*/
import java.util.Scanner;
public class RegexTest3 {
	public static void main(String[] args) {
		//1，创建输入对象
		Scanner sc = new Scanner(System.in);
		System.out.println("请输入邮箱:");
		String str = sc.nextLine();
		//2，定义判断规则
		String regex = "\\d{3,}@\\w{2,}\\.com";
		//3，判断
		boolean flag = str.matches(regex);
		
		System.out.println(flag);
	}
}
```
#### （3）正则表达式的应用——替换功能
```python
/*正则表达式应用———替换功能
 * 需求：用*替换字符串中出现的数字
 * 思路：
 * 用函数String类下的replaceAll(String regex,String replacement)
 * 即，用给定的replacement替换该字符串中所有匹配给定的regex的子字符串
 */
public class RegexTest4 {
	public static void main(String[] args) {
		String str = "I love you,我的银行卡号是：15543767，请记1314住啊";
		String regex = "\\d";
		String rep = "*";
		String result = str.replaceAll(regex, rep);
		System.out.println(result);
				
	}
}
```
#### （4）正则表达式的应用——获取功能
```python
import java.util.regex.Matcher;
import java.util.regex.Pattern;
/*正则表达式功能——获取功能
 * 需求：获取下面字符串中由4个字符组成的单词
 * I love you,but do you belong to me forever? I can't konw about it,but I will keep my will.
 */
public class RegrexTest5 {
	public static void main(String[] args) {
		//定义字符串
		String str = "I love you,but do you belong to me forever? "
				+ "I can't konw about it,but I will keep my will.";
		//定义正则表达式
		String regex = "\\b\\w{4}\\b"; // \\b表示单词边界
		
		//把规则编译成模式对象
		Pattern p = Pattern.compile(regex);
		//通过模式对象得到匹配器对象
		Matcher m = p.matcher(str);
		
		//调用匹配器对象的功能
		//通过find方法就是查找有没有满足条件的子串
		while(m.find()) {
			//获取匹配正则表达式的子串
			String result = m.group();
			System.out.println(result);
		}
	}
}
```


