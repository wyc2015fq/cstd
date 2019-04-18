# Java判断是否是整数，小数或实数的正则表达式 - z69183787的专栏 - CSDN博客
2013年10月18日 09:35:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：42824
   经常会遇到这样的情况，需要判断一个字符串是否是一个合法的数，包括整数，小数或者实数。
    网上查到很多文章大多是判断这个字符串是否全为数字，比如下面这段来自StringUtils的代码，可以看到，13.2这样的数字实际上会返回false，可是，他的确是一个数字。
**[java]**[view
 plain](http://blog.csdn.net/liweisnake/article/details/8672421#)[copy](http://blog.csdn.net/liweisnake/article/details/8672421#)
- publicstaticboolean isNumeric(String str) {  
- if (str == null) {  
- returnfalse;  
-         }  
- int sz = str.length();  
- for (int i = 0; i < sz; i++) {  
- if (Character.isDigit(str.charAt(i)) == false) {  
- returnfalse;  
-             }  
-         }  
- returntrue;  
-     }  
   当然，网上还能查到很多其他方式，诸如用正则表达式判断是否0-9，用字符ascii码判断是否是数字以及用Double.parseDouble()是否抛出异常来判断是否为数字。
   事实上，除了最后一种方式能达到我们的要求，其他的都很难真正做到类似的判断。但是最后一种方式也很难区别出到底是正整数，负整数，正小数还是负小数，而且，捕获异常的方式实在是有些难看。
   基于此原因，我自己写了一个工具类，专门用作数的检测，目前能够检测正整数，负整数，整数，正小数，负小数，小数以及实数，采用的仍然是正则表达式的方式，当然，如果有遗漏或者错误，欢迎联系我以便更正，同时也欢迎修改或使用这些代码以便符合你的应用场景。
   可以简单讲下正则的思想以便修改，
   1. 对于正整数而言，可以带+号，第一个数字不能为0
   2. 对于负整数而言，必须带负号，第一个数字也不能为0
   3. 对于整数而言，实际是由0，正整数和负整数组成的，所以偷个懒用前两个方法一起判断
   4. 对于正小数而言，可以考带+号，并考虑两种情况，第一个数字为0和第一个数字不为0，第一个数字为0时，则小数点后面应该不为0，第一个数字不为0时，小数点后可以为任意数字
   5. 对于负小数而言，必须带负号，其余都同上
   6. 对于小数，可以带正负号，并且带小数点就行了，但是至少保证小数点有一边不为空，所以这里还是分左边不为空和右边不为空的情况
   7. 实数比较简单，，要么是整数，要么是小数
**[java]**[view
 plain](http://blog.csdn.net/liweisnake/article/details/8672421#)[copy](http://blog.csdn.net/liweisnake/article/details/8672421#)
- package com.sap.cesp.creditinsight.web.app.util;  
- 
- import java.util.regex.Matcher;  
- import java.util.regex.Pattern;  
- 
- publicclass NumberValidationUtils {  
- 
- privatestaticboolean isMatch(String regex, String orginal){  
- if (orginal == null || orginal.trim().equals("")) {  
- returnfalse;  
-         }  
-         Pattern pattern = Pattern.compile(regex);  
-         Matcher isNum = pattern.matcher(orginal);  
- return isNum.matches();  
-     }  
- 
- publicstaticboolean isPositiveInteger(String orginal) {  
- return isMatch("^\\+{0,1}[1-9]\\d*", orginal);  
-     }  
- 
- publicstaticboolean isNegativeInteger(String orginal) {  
- return isMatch("^-[1-9]\\d*", orginal);  
-     }  
- 
- publicstaticboolean isWholeNumber(String orginal) {  
- return isMatch("[+-]{0,1}0", orginal) || isPositiveInteger(orginal) || isNegativeInteger(orginal);  
-     }  
- 
- publicstaticboolean isPositiveDecimal(String orginal){  
- return isMatch("\\+{0,1}[0]\\.[1-9]*|\\+{0,1}[1-9]\\d*\\.\\d*", orginal);  
-     }  
- 
- publicstaticboolean isNegativeDecimal(String orginal){  
- return isMatch("^-[0]\\.[1-9]*|^-[1-9]\\d*\\.\\d*", orginal);  
-     }  
- 
- publicstaticboolean isDecimal(String orginal){  
- return isMatch("[-+]{0,1}\\d+\\.\\d*|[-+]{0,1}\\d*\\.\\d+", orginal);  
-     }  
- 
- publicstaticboolean isRealNumber(String orginal){  
- return isWholeNumber(orginal) || isDecimal(orginal);  
-     }  
- 
- }  
测试用例如下：
**[java]**[view
 plain](http://blog.csdn.net/liweisnake/article/details/8672421#)[copy](http://blog.csdn.net/liweisnake/article/details/8672421#)
- package com.sap.cesp.creditinsight.web.app.util;  
- 
- import junit.framework.Assert;  
- 
- import org.junit.Test;  
- 
- publicclass NumberValidationUtilsTest {  
- 
- /**
-      * Test method for {@link com.sap.cesp.creditinsight.web.app.util.NumberValidationUtils#isPositiveInteger(java.lang.String)}
-      */
- //correct test case: 1, 87653521123567
- //wrong test case: 0.1, 0, 0123, -1, -0.1, ab
- @Test
- publicvoid testIsPositiveInteger() {  
-         Assert.assertTrue(NumberValidationUtils.isPositiveInteger("1"));  
-         Assert.assertTrue(NumberValidationUtils.isPositiveInteger("+12"));  
-         Assert.assertTrue(NumberValidationUtils.isPositiveInteger("87653521123567"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveInteger("0.1"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveInteger("0"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveInteger("0123"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveInteger("-1"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveInteger("-0.1"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveInteger("ab"));  
-     }  
- 
- /**
-      * Test method for {@link com.sap.cesp.creditinsight.web.app.util.NumberValidationUtils#isNegativeInteger(java.lang.String)}
-      */
- //correct test case: -1, -87653521123567
- //wrong test case: 0.1, 0, 0123, 1, -0.1, -ab
- @Test
- publicvoid testIsNegativeInteger() {  
-         Assert.assertTrue(NumberValidationUtils.isNegativeInteger("-1"));  
-         Assert.assertTrue(NumberValidationUtils.isNegativeInteger("-87653521123567"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeInteger("0.1"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeInteger("0"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeInteger("0123"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeInteger("1"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeInteger("-0.1"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeInteger("ab"));  
-     }  
- 
- /**
-      * Test method for {@link com.sap.cesp.creditinsight.web.app.util.NumberValidationUtils#isWholeNumber(java.lang.String)}.
-      */
- //correct test case: -1, 0, 1, 8673434231, -282464334
- //wrong test case: 0.1, 0123, -0.1, ab
- @Test
- publicvoid testIsWholeNumber() {  
-         Assert.assertTrue(NumberValidationUtils.isWholeNumber("-1"));  
-         Assert.assertTrue(NumberValidationUtils.isWholeNumber("0"));  
-         Assert.assertTrue(NumberValidationUtils.isWholeNumber("1"));  
-         Assert.assertTrue(NumberValidationUtils.isWholeNumber("+12"));  
-         Assert.assertTrue(NumberValidationUtils.isWholeNumber("8673434231"));  
-         Assert.assertTrue(NumberValidationUtils.isWholeNumber("-282464334"));  
-         Assert.assertFalse(NumberValidationUtils.isWholeNumber("0123"));  
-         Assert.assertFalse(NumberValidationUtils.isWholeNumber("0.1"));  
-         Assert.assertFalse(NumberValidationUtils.isWholeNumber("-0.1"));  
-         Assert.assertFalse(NumberValidationUtils.isWholeNumber("ab"));  
-     }  
- 
- /**
-      * Test method for {@link com.sap.cesp.creditinsight.web.app.util.NumberValidationUtils#isPositiveDecimal(java.lang.String)}
-      */
- //correct test case: 0.1, 0.132213, 1.0
- //wrong test case: 1, 0.0, 0123, -1, -0.1
- @Test
- publicvoid testIsPositiveDecimal() {  
-         Assert.assertTrue(NumberValidationUtils.isPositiveDecimal("0.1"));  
-         Assert.assertTrue(NumberValidationUtils.isPositiveDecimal("0.132213"));  
-         Assert.assertTrue(NumberValidationUtils.isPositiveDecimal("30.00"));  
-         Assert.assertTrue(NumberValidationUtils.isDecimal("0."));  
-         Assert.assertTrue(NumberValidationUtils.isPositiveDecimal("+12.0"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveDecimal("0123"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveDecimal("1"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveDecimal("0.0"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveDecimal("ab"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveDecimal("-1"));  
-         Assert.assertFalse(NumberValidationUtils.isPositiveDecimal("-0.1"));  
-     }  
- 
- /**
-      * Test method for {@link com.sap.cesp.creditinsight.web.app.util.NumberValidationUtils#isNegativeDecimal(java.lang.String)}
-      */
- //correct test case: -0.132213, -1.0
- //wrong test case: 1, 0, 0123, -1, 0.1
- @Test
- publicvoid testIsNegativeDecimal() {  
-         Assert.assertTrue(NumberValidationUtils.isNegativeDecimal("-0.132213"));  
-         Assert.assertTrue(NumberValidationUtils.isNegativeDecimal("-1.0"));  
-         Assert.assertTrue(NumberValidationUtils.isDecimal("-0."));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeDecimal("1"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeDecimal("0"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeDecimal("0123"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeDecimal("0.0"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeDecimal("ab"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeDecimal("-1"));  
-         Assert.assertFalse(NumberValidationUtils.isNegativeDecimal("0.1"));  
-     }  
- 
- /**
-      * Test method for {@link com.sap.cesp.creditinsight.web.app.util.NumberValidationUtils#isDecimal(java.lang.String)}.
-      */
- //correct test case: 0.1, 0.00, -0.132213
- //wrong test case: 1, 0, 0123, -1,  0., ba
- @Test
- publicvoid testIsDecimal() {  
-         Assert.assertTrue(NumberValidationUtils.isDecimal("0.1"));  
-         Assert.assertTrue(NumberValidationUtils.isDecimal("0.00"));  
-         Assert.assertTrue(NumberValidationUtils.isDecimal("+0.0"));  
-         Assert.assertTrue(NumberValidationUtils.isDecimal("-0.132213"));  
-         Assert.assertTrue(NumberValidationUtils.isDecimal("0."));  
-         Assert.assertFalse(NumberValidationUtils.isDecimal("1"));  
-         Assert.assertFalse(NumberValidationUtils.isDecimal("0123"));  
-         Assert.assertFalse(NumberValidationUtils.isDecimal("0"));  
-         Assert.assertFalse(NumberValidationUtils.isDecimal("ab"));  
-         Assert.assertFalse(NumberValidationUtils.isDecimal("-1"));  
- 
-     }  
- 
- /**
-      * Test method for {@link com.sap.cesp.creditinsight.web.app.util.NumberValidationUtils#isRealNumber(java.lang.String)}.
-      */
- //correct test case: 0.032213, -0.234, 0.0, 1, -1, 0
- //wrong test case: 00.13, ab, +0.14
- @Test
- publicvoid testIsRealNumber() {  
-         Assert.assertTrue(NumberValidationUtils.isRealNumber("0.032213"));  
-         Assert.assertTrue(NumberValidationUtils.isRealNumber("-0.234"));  
-         Assert.assertTrue(NumberValidationUtils.isRealNumber("0.0"));  
-         Assert.assertTrue(NumberValidationUtils.isRealNumber("1"));  
-         Assert.assertTrue(NumberValidationUtils.isRealNumber("+0.14"));  
-         Assert.assertTrue(NumberValidationUtils.isRealNumber("-1"));  
-         Assert.assertTrue(NumberValidationUtils.isRealNumber("0.0"));  
-         Assert.assertFalse(NumberValidationUtils.isRealNumber("00.13"));  
-         Assert.assertFalse(NumberValidationUtils.isRealNumber("ab"));  
- 
-     }  
- 
- }  
    float和double在内存中的表示方式： [http://www.360doc.com/content/12/0328/22/8265633_198759107.shtml](http://www.360doc.com/content/12/0328/22/8265633_198759107.shtml)
    浮点数在计算机中怎么表示：[http://www.ruanyifeng.com/blog/2010/06/ieee_floating-point_representation.html](http://www.ruanyifeng.com/blog/2010/06/ieee_floating-point_representation.html)
