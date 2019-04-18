# Java键盘字符乱码判断代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月04日 16:45:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2302








判断字符串中是否含有无效字符（字母、数字、汉字），对键盘上的字符一一做测试。



```java
//判断是否是乱码
	    private static boolean isMessyCode(String strName) {
	        try {
	            Pattern p = Pattern.compile("\\s*|\t*|\r*|\n*");
	            Matcher m = p.matcher(strName);
	            String after = m.replaceAll("");
	            String temp = after.replaceAll("\\p{P}", "");
	            char[] ch = temp.trim().toCharArray();

	            int length = (ch != null) ? ch.length : 0;
	            for (int i = 0; i < length; i++) {
	                char c = ch[i];
	                if (!Character.isLetterOrDigit(c)) {
	                    String str = "" + ch[i];
	                    if ( str.equals("=") || str.equals("|")|| str.equals("+"))//剔除特殊字符的检查
	                    	continue;
	                    //if (!str.matches("[\u4e00-\u9fa5]+")) {
	                    if (!str.matches("[0-9a-zA-Z\\u4e00-\\u9fa5]+")){
	                        return true;
	                    }
	                }
	            }
	        } catch (Exception e) {
	            e.printStackTrace();
	        }
	        return false;
	    }
```


```java
public static void main(String[] args) throws Exception {
String str="(";//\\!@#$%^&*";
System.out.println(isMessyCode(str));
}
```
如果含有无效字符，返回true，否则返回false，对键盘上的字符一一试验，发现+、=属于无效字符，而&又是有效，可以用这段代码来试试。




