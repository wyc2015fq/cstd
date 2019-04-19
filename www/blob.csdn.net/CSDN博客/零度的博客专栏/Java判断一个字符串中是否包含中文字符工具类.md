# Java判断一个字符串中是否包含中文字符工具类 - 零度的博客专栏 - CSDN博客
2016年11月24日 14:30:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：3664
                
Java判断一个字符串是否有中文一般情况是利用Unicode编码(CJK统一汉字的编码区间：0x4e00–0x9fbb)的正则来做判断，但是其实这个区间来判断中文不是非常精确，因为有些中文的标点符号比如：，。等等是不能识别的。
以下是比较完善的判断方法：CharUtil.java
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/53162015#)[copy](http://blog.csdn.net/z69183787/article/details/53162015#)[print](http://blog.csdn.net/z69183787/article/details/53162015#)[?](http://blog.csdn.net/z69183787/article/details/53162015#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1984383/fork)
- package com.micmiu.utils;  
- 
- import java.util.regex.Pattern;  
- 
- /**
-  * 字符通用工具类
-  * 
-  * @author <a href="http://www.micmiu.com">Michael Sun</a>
-  */
- publicclass CharUtil {  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         String[] strArr = new String[] { "www.micmiu.com",  
- "!@#$%^&*()_+{}[]|\"'?/:;<>,.", "！￥……（）——：；“”‘’《》，。？、", "不要啊",  
- "やめて", "韩佳人", "한가인" };  
- for (String str : strArr) {  
-             System.out.println("===========> 测试字符串：" + str);  
-             System.out.println("正则判断：" + isChineseByREG(str) + " -- "
-                     + isChineseByName(str));  
-             System.out.println("Unicode判断结果 ：" + isChinese(str));  
-             System.out.println("详细判断列表：");  
- char[] ch = str.toCharArray();  
- for (int i = 0; i < ch.length; i++) {  
- char c = ch[i];  
-                 System.out.println(c + " --> " + (isChinese(c) ? "是" : "否"));  
-             }  
-         }  
- 
-     }  
- 
- // 根据Unicode编码完美的判断中文汉字和符号
- privatestaticboolean isChinese(char c) {  
-         Character.UnicodeBlock ub = Character.UnicodeBlock.of(c);  
- if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS  
-                 || ub == Character.UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS  
-                 || ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A  
-                 || ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B  
-                 || ub == Character.UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION  
-                 || ub == Character.UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS  
-                 || ub == Character.UnicodeBlock.GENERAL_PUNCTUATION) {  
- returntrue;  
-         }  
- returnfalse;  
-     }  
- 
- // 完整的判断中文汉字和符号
- publicstaticboolean isChinese(String strName) {  
- char[] ch = strName.toCharArray();  
- for (int i = 0; i < ch.length; i++) {  
- char c = ch[i];  
- if (isChinese(c)) {  
- returntrue;  
-             }  
-         }  
- returnfalse;  
-     }  
- 
- // 只能判断部分CJK字符（CJK统一汉字）
- publicstaticboolean isChineseByREG(String str) {  
- if (str == null) {  
- returnfalse;  
-         }  
-         Pattern pattern = Pattern.compile("[\\u4E00-\\u9FBF]+");  
- return pattern.matcher(str.trim()).find();  
-     }  
- 
- // 只能判断部分CJK字符（CJK统一汉字）
- publicstaticboolean isChineseByName(String str) {  
- if (str == null) {  
- returnfalse;  
-         }  
- // 大小写不同：\\p 表示包含，\\P 表示不包含 
- // \\p{Cn} 的意思为 Unicode 中未被定义字符的编码，\\P{Cn} 就表示 Unicode中已经被定义字符的编码
-         String reg = "\\p{InCJK Unified Ideographs}&&\\P{Cn}";  
-         Pattern pattern = Pattern.compile(reg);  
- return pattern.matcher(str.trim()).find();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.micmiu.utils;
import java.util.regex.Pattern;
/**
 * 字符通用工具类
 * 
 * @author <a href="http://www.micmiu.com">Michael Sun</a>
 */
public class CharUtil {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String[] strArr = new String[] { "www.micmiu.com",
				"!@#$%^&*()_+{}[]|\"'?/:;<>,.", "！￥……（）——：；“”‘’《》，。？、", "不要啊",
				"やめて", "韩佳人", "한가인" };
		for (String str : strArr) {
			System.out.println("===========> 测试字符串：" + str);
			System.out.println("正则判断：" + isChineseByREG(str) + " -- "
					+ isChineseByName(str));
			System.out.println("Unicode判断结果 ：" + isChinese(str));
			System.out.println("详细判断列表：");
			char[] ch = str.toCharArray();
			for (int i = 0; i < ch.length; i++) {
				char c = ch[i];
				System.out.println(c + " --> " + (isChinese(c) ? "是" : "否"));
			}
		}
	}
	// 根据Unicode编码完美的判断中文汉字和符号
	private static boolean isChinese(char c) {
		Character.UnicodeBlock ub = Character.UnicodeBlock.of(c);
		if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS
				|| ub == Character.UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS
				|| ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A
				|| ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B
				|| ub == Character.UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION
				|| ub == Character.UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS
				|| ub == Character.UnicodeBlock.GENERAL_PUNCTUATION) {
			return true;
		}
		return false;
	}
	// 完整的判断中文汉字和符号
	public static boolean isChinese(String strName) {
		char[] ch = strName.toCharArray();
		for (int i = 0; i < ch.length; i++) {
			char c = ch[i];
			if (isChinese(c)) {
				return true;
			}
		}
		return false;
	}
	// 只能判断部分CJK字符（CJK统一汉字）
	public static boolean isChineseByREG(String str) {
		if (str == null) {
			return false;
		}
		Pattern pattern = Pattern.compile("[\\u4E00-\\u9FBF]+");
		return pattern.matcher(str.trim()).find();
	}
	// 只能判断部分CJK字符（CJK统一汉字）
	public static boolean isChineseByName(String str) {
		if (str == null) {
			return false;
		}
		// 大小写不同：\\p 表示包含，\\P 表示不包含 
		// \\p{Cn} 的意思为 Unicode 中未被定义字符的编码，\\P{Cn} 就表示 Unicode中已经被定义字符的编码
		String reg = "\\p{InCJK Unified Ideographs}&&\\P{Cn}";
		Pattern pattern = Pattern.compile(reg);
		return pattern.matcher(str.trim()).find();
	}
}
```
**[plain]**[view plain](http://blog.csdn.net/z69183787/article/details/53162015#)[copy](http://blog.csdn.net/z69183787/article/details/53162015#)[print](http://blog.csdn.net/z69183787/article/details/53162015#)[?](http://blog.csdn.net/z69183787/article/details/53162015#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1984383/fork)
- ===========> 测试字符串：www.micmiu.com  
- 正则判断：false -- false  
- Unicode判断结果 ：false  
- 详细判断列表：  
- w --> 否  
- w --> 否  
- w --> 否  
- . --> 否  
- m --> 否  
- i --> 否  
- c --> 否  
- m --> 否  
- i --> 否  
- u --> 否  
- . --> 否  
- c --> 否  
- o --> 否  
- m --> 否  
- ===========> 测试字符串：!@#$%^&*()_+{}[]|"'?/:;<>,.  
- 正则判断：false -- false  
- Unicode判断结果 ：false  
- 详细判断列表：  
- ! --> 否  
- @ --> 否  
- # --> 否  
- $ --> 否  
- % --> 否  
- ^ --> 否  
- & --> 否  
- * --> 否  
- ( --> 否  
- ) --> 否  
- _ --> 否  
- + --> 否  
- { --> 否  
- } --> 否  
- [ --> 否  
- ] --> 否  
- | --> 否  
- " --> 否  
- ' --> 否  
- ? --> 否  
- / --> 否  
- : --> 否  
- ; --> 否  
- < --> 否  
- > --> 否  
- , --> 否  
- . --> 否  
- ===========> 测试字符串：！￥……（）——：；“”‘’《》，。？、  
- 正则判断：false -- false  
- Unicode判断结果 ：true  
- 详细判断列表：  
- ！ --> 是  
- ￥ --> 是  
- … --> 是  
- … --> 是  
- （ --> 是  
- ） --> 是  
- — --> 是  
- — --> 是  
- ： --> 是  
- ； --> 是  
- “ --> 是  
- ” --> 是  
- ‘ --> 是  
- ’ --> 是  
- 《 --> 是  
- 》 --> 是  
- ， --> 是  
- 。 --> 是  
- ？ --> 是  
- 、 --> 是  
- ===========> 测试字符串：不要啊  
- 正则判断：true -- false  
- Unicode判断结果 ：true  
- 详细判断列表：  
- 不 --> 是  
- 要 --> 是  
- 啊 --> 是  
- ===========> 测试字符串：やめて  
- 正则判断：false -- false  
- Unicode判断结果 ：false  
- 详细判断列表：  
- や --> 否  
- め --> 否  
- て --> 否  
- ===========> 测试字符串：韩佳人  
- 正则判断：true -- false  
- Unicode判断结果 ：true  
- 详细判断列表：  
- 韩 --> 是  
- 佳 --> 是  
- 人 --> 是  
- ===========> 测试字符串：한가인  
- 正则判断：false -- false  
- Unicode判断结果 ：false  
- 详细判断列表：  
- 한 --> 否  
- 가 --> 否  
- 인 --> 否  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
===========> 测试字符串：www.micmiu.com
正则判断：false -- false
Unicode判断结果 ：false
详细判断列表：
w --> 否
w --> 否
w --> 否
. --> 否
m --> 否
i --> 否
c --> 否
m --> 否
i --> 否
u --> 否
. --> 否
c --> 否
o --> 否
m --> 否
===========> 测试字符串：!@#$%^&*()_+{}[]|"'?/:;<>,.
正则判断：false -- false
Unicode判断结果 ：false
详细判断列表：
! --> 否
@ --> 否
# --> 否
$ --> 否
% --> 否
^ --> 否
& --> 否
* --> 否
( --> 否
) --> 否
_ --> 否
+ --> 否
{ --> 否
} --> 否
[ --> 否
] --> 否
| --> 否
" --> 否
' --> 否
? --> 否
/ --> 否
: --> 否
; --> 否
< --> 否
> --> 否
, --> 否
. --> 否
===========> 测试字符串：！￥……（）——：；“”‘’《》，。？、
正则判断：false -- false
Unicode判断结果 ：true
详细判断列表：
！ --> 是
￥ --> 是
… --> 是
… --> 是
（ --> 是
） --> 是
— --> 是
— --> 是
： --> 是
； --> 是
“ --> 是
” --> 是
‘ --> 是
’ --> 是
《 --> 是
》 --> 是
， --> 是
。 --> 是
？ --> 是
、 --> 是
===========> 测试字符串：不要啊
正则判断：true -- false
Unicode判断结果 ：true
详细判断列表：
不 --> 是
要 --> 是
啊 --> 是
===========> 测试字符串：やめて
正则判断：false -- false
Unicode判断结果 ：false
详细判断列表：
や --> 否
め --> 否
て --> 否
===========> 测试字符串：韩佳人
正则判断：true -- false
Unicode判断结果 ：true
详细判断列表：
韩 --> 是
佳 --> 是
人 --> 是
===========> 测试字符串：한가인
正则判断：false -- false
Unicode判断结果 ：false
详细判断列表：
한 --> 否
가 --> 否
인 --> 否
```
