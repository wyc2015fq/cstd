# 正则表达式学习（Java） - 囧囧有神的专栏 - CSDN博客





2010年11月05日 01:46:00[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：961








       花了一个晚上的时间，把Java正则表达式的视频又重新看了一遍。然后写了个小程序测试了一下。还有一些比较特殊的语法没有测试，大概了解了一下。需要用到时再去看吧。正则表达式理解不难，但要用到熟练无比，出神入化，还是有难度的。


public class Test1 {
	/*
	 * 测试 正则表达式语法 
	 */
	public static void main(String[] args){
		Test1 t = new Test1();
		t.testXieGang();//测试 斜杠 /
		t.testReplaceAll();//测试 String 类的 replace 方法
		t.testZhongKuoHao();//测试 []
		t.testPreDefChar();  // 测试 /s  /w /d  /S  /D  /W  大写的表示相反
		t.testBoundMatch();   // 测试边界匹配
		t.testLogic();     // 测试逻辑操作符
	}
	private void testXieGang(){
		/*
		 *  字符串是表示一个连续的字符序列，在程序中我们用类String来描述一个字符串，
		 *  这个用String描述的字符串与实际的字符串是不一定完全一样的。比如在字符串中双引号怎么表示？某些空白字符怎么表示，所以我们必须在
		 *  String类中引入 转义字符的概念，也就是/。 比如说 String str = "ab/t/////"b" 实际上表示的是  ab	//"b,这是一个7个字符的序列。
		 *  正则表达式是一个特殊的表达式，但也要遵守这些规定。比如 String str="////" 实际上是两个普通字符 //  ，然后用正则表达式来编译它，
		 *  就会认为只是一个/
		 */
		System.out.println("//".matches("////")); //  true   两个/ 必须用正则表达式中的//// 表示
		System.out.println("//n".matches("////n"));//true
		String str = "ni/t//hao/"an";
		System.out.println(str);     // 输出  ni	/hao"an    这个才是实际字符串
		System.out.println(str.matches(".{10}"));  //  TRUE  str中是由10个字符组成的
	}
	private void testReplaceAll(){
		String str = "niha+********oma?/t";
		str = str.replaceAll("[?]", "");//  单独的 ?(+*可以单独用) 会出问题因为有特殊意义  用[] 括起来或者//?即可
		System.out.println(str);// niha+********oma	
		str = str.replace("+", "");
		System.out.println(str);//niha********oma
		str = str.replace("*", "");
		System.out.println(str);//nihaoma
		System.out.println("92".matches("(//d+)"));//  true  小括号也是特殊字符
		System.out.println("(92)".matches("//(//d+[)]"));// true 小括号也是特殊字符，在正则表达式中 用[] 或者 //
	}
	private void testZhongKuoHao(){
		String str = "da jia hao cai shi zhen de hao ";
		String str1 = "aBDbcA";
		System.out.println("a^c".matches("[a-z^A-Z]{3}"));  //true  ^只有放在[]里面第一个位置 才是表示否定，否则就是普通字符 
		System.out.println("ABC".matches("[^a-zA-Z]{3}"));  //false ^表示否定，It is false ;
		System.out.println("abc".matches("[a-n&&g-z]{3}")+" "+"hin".matches("[a-n&&g-z]{3}"));// false  true
		System.out.println("efg".matches("[^a-d&&b-g]+"));  //true a,b,c,d 都是FALSE e f g 是TRUE 
		System.out.println("bcd".matches("[a-d&&^b-g]+"));  //true   ^ 在中间只是一个普通字符
		System.out.println("aaa".matches("[a-d&&[^b-g]]+"));//true  只有a 才是匹配的
	}
	private void testPreDefChar(){  // 测试预定字符类
		System.out.println("a/n/ta/ta".replaceAll("//s", "")); //aaa     等价于[ /t/n/x0B/f/r]
		System.out.println("(&((&^*^*^*Liujiyong814329735".replaceAll("//w",""));  //(&((&^*^*^*   等价于 [a-zA-Z_0-9]
		System.out.println("liu8143 ji29735 yong".replaceAll("//D", "")); //814329735
	}
	private void testBoundMatch(){  //测试边界匹配
		System.out.println("java (java)java/tjavajava,java".replaceAll("//bjava//b", ""));// ()	javajava,     /b表示单词边界
		System.out.println("java (java)java/tjavajava,java".replaceAll("^java", "")); // (java)java	javajava,java    ^表示字符串开头
		System.out.println("java (java)java/tjavajava,java".replaceAll("java$", "")); //java (java)java	javajava,   $表示字符串开头
	}
	private void testLogic(){//测试逻辑符 |
		System.out.println("256".matches("(//d{1,2})|(1//d{2})|(2[0-4]//d)|(25[0-5])"));//false
		System.out.println("0".matches("(//d{1,2})|(1//d{2})|(2[0-4]//d)|(25[0-5])"));//true
		System.out.println("56".matches("(//d{1,2})|(1//d{2})|(2[0-4]//d)|(25[0-5])"));//true
		System.out.println("156".matches("(//d{1,2})|(1//d{2})|(2[0-4]//d)|(25[0-5])"));//true
		System.out.println("255".matches("(//d{1,2})|(1//d{2})|(2[0-4]//d)|(25[0-5])"));//true
		System.out.println("99".matches("//d{1,2}|1//d{2}|2[0-4]//d|25[0-5]"));  // true 
		System.out.println("255.192.0.1".matches("((//d{1,2}|1//d{2}|2[0-4]//d|25[0-5])//.){3}(//d{1,2}|1//d{2}|2[0-4]//d|25[0-5])"));  // true  ip正则表达式
		
		System.out.println("b".matches("[a-d&&b-g]"));  //TRUE  b c d 会匹配这个正则表达式
		System.out.println("a&&b".matches("a&&b"));  //TRUE  在[]外面 && 只是普通字符
		//System.out.println("a".matches("a|bc|c"));  //  true
		//System.out.println("bcd".matches("a|bc|c"));//   false
		//System.out.println("c".matches("a|bc|c"));//    true
		//System.out.println("92".matches("//d{1,2}"));//   true
	}
}




