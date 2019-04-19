# Java正则Matcher类的matches()、lookAt()和find()的区别 - 零度的博客专栏 - CSDN博客
2017年01月05日 16:43:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：869
        在Matcher类中有matches、lookingAt和find都是匹配目标的方法，但容易混淆，整理它们的区别如下：
- **matches**: 整个匹配，只有整个字符序列完全匹配成功，才返回True，否则返回False。但如果前部分匹配成功，将移动下次匹配的位置。
- **lookingAt**: 部分匹配，总是从第一个字符进行匹配,匹配成功了不再继续匹配，匹配失败了,也不继续匹配。
- **find**: 部分匹配，从当前位置开始匹配，找到一个匹配的子串，将移动下次匹配的位置。
- **reset: **给当前的Matcher对象配上个新的目标，目标是就该方法的参数；如果不给参数，reset会把Matcher设到当前字符串的开始处。
        使用示例代码来展示他们的区别更清晰明了：java代码：
```
package zmx.regex.test;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
public class MatcherTest {
	
	public static void main(String[] args){
				Pattern pattern = Pattern.compile("\\d{3,5}");
				String charSequence = "123-34345-234-00";
				Matcher matcher = pattern.matcher(charSequence);
		 
				//虽然匹配失败，但由于charSequence里面的"123"和pattern是匹配的,所以下次的匹配从位置4开始
				print(matcher.matches());
				//测试匹配位置
				matcher.find();
				print(matcher.start());
		 
				//使用reset方法重置匹配位置
				matcher.reset();
		 
				//第一次find匹配以及匹配的目标和匹配的起始位置
				print(matcher.find());
				print(matcher.group()+" - "+matcher.start());
				//第二次find匹配以及匹配的目标和匹配的起始位置
				print(matcher.find());
				print(matcher.group()+" - "+matcher.start());
		 
				//第一次lookingAt匹配以及匹配的目标和匹配的起始位置
				print(matcher.lookingAt());
				print(matcher.group()+" - "+matcher.start());
		 
				//第二次lookingAt匹配以及匹配的目标和匹配的起始位置
				print(matcher.lookingAt());
				print(matcher.group()+" - "+matcher.start());
				
				//使用reset方法重置匹配位置
				matcher.reset();
				//遍历所有匹配的子串
				while(matcher.find()){
					print(matcher.group()+" - "+matcher.start());
				}
				
			}
			public static void print(Object o){
				System.out.println(o);
			}
}
```
执行结果：
false
4
true
123 - 0
true
34345 - 4
true
123 - 0
true
123 - 0
123 - 0
34345 - 4
