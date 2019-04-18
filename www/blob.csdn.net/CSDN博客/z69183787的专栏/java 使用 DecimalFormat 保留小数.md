# java 使用 DecimalFormat 保留小数 - z69183787的专栏 - CSDN博客
2014年02月27日 16:07:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1686
```java
java.text.DecimalFormat df = new java.text.DecimalFormat("0.00");
		
java.text.DecimalFormat df2 = new java.text.DecimalFormat("0.0000");
```
```java
public String convertDoubleToDouble(java.text.DecimalFormat df,Double s){
		String result = "";
		if(s==null){
			result = " ";
		}else{
			result = df.format(s*100);
		}
		return result;
	}
	
	public String convertLongToDouble(java.text.DecimalFormat df,Long s){
		String result = "";
		if(s==null){
			result = " ";
		}else{
			result = df.format((double)s/10000);
		}
		return result;
	}
	
	public String convertDoubleToDouble2(java.text.DecimalFormat df,Double s){
		String result = "";
		if(s==null){
			result = " ";
		}else{
			result = df.format((double)s/10000);
		}
		return result;
	}
```
