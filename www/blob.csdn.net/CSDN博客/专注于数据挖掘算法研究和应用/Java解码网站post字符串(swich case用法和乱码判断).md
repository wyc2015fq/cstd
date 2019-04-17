# Java解码网站post字符串(swich case用法和乱码判断) - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月03日 16:09:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2567
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









对网站post字符串进行解码，用常见字符集试验，用了switch case并对乱码做判断，如果是js加密且ContentType文件类型是text/octet纯二进制就无法解码。具体代码如下：



```java
public class DPIPost {
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
	@SuppressWarnings("finally")
	public static String decodePostData(String postdata) throws UnsupportedEncodingException{
		String charSet="Base64";//字符集
		String retPostdata=postdata;//返回值
		try{
			switch(charSet){
			case "Base64"://base64解码
				byte[] bEncode=postdata.getBytes();//获取post串的字节
				byte[] bDecode= it.sauronsoftware.base64.Base64.decode(bEncode);//用base64解码
				String strBase64=new String(bDecode);	
				if(strBase64.contains("&") && strBase64.contains("=") && !strBase64.contains("%")){//解码成功的标志
					String str1=strBase64.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strBase64;
						break;		
					}
				}else if(strBase64.contains("%")) {//需URL解码
					postdata=strBase64;	
				}
				charSet="UTF-8";
			case "UTF-8": 
				String strUTF8=URLDecoder.decode(postdata,charSet);
				if(strUTF8.contains("&") && strUTF8.contains("=")){//解码成功的标志
					String str1=strUTF8.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strUTF8;
						break;		
					}
				}
				charSet="UTF-16";
			case "UTF-16": 
				String strUTF16=URLDecoder.decode(postdata,charSet);
				if(strUTF16.contains("&") && strUTF16.contains("=")){//解码成功的标志
					String str1=strUTF16.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strUTF16;
						break;		
					}
				}
				charSet="GBK";
			case "GBK": 
				String strGBK=URLDecoder.decode(postdata,charSet);
				if(strGBK.contains("&") && strGBK.contains("=")){//解码成功的标志
					String str1=strGBK.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strGBK;
						break;		
					}
				}
				charSet="GB2312";
			case "GB2312": 
				String strGB2312=URLDecoder.decode(postdata,charSet);
				if(strGB2312.contains("&") && strGB2312.contains("=")){//解码成功的标志
					String str1=strGB2312.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strGB2312;
						break;		
					}
				}
				charSet="GB18030";
			case "GB18030": 
				String strGB18030=URLDecoder.decode(postdata,charSet);
				if(strGB18030.contains("&") && strGB18030.contains("=")){//解码成功的标志
					String str1=strGB18030.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strGB18030;
						break;		
					}
				}
				charSet="UNICODE";
			case "UNICODE": 
				String strUnicode=URLDecoder.decode(postdata,charSet);
				if(strUnicode.contains("&") && strUnicode.contains("=")){//解码成功的标志
					String str1=strUnicode.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strUnicode;
						break;		
					}
				}
				charSet="BIG5";
			case "BIG5": 
				String strBig5=URLDecoder.decode(postdata,charSet);
				if(strBig5.contains("&") && strBig5.contains("=")){//解码成功的标志
					String str1=strBig5.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strBig5;
						break;		
					}
				}
				charSet="ISO-8859-1";
			case "ISO-8859-1":
				String strISO88591=URLDecoder.decode(postdata,charSet);
				if(strISO88591.contains("&") && strISO88591.contains("=")){//解码成功的标志
					String str1=strISO88591.replaceAll("&", "a");
					String str2=str1.replaceAll("=", "b");
					if(!DPIPost.isMessyCode(str2)) {//不含有乱码
						retPostdata= strISO88591;
						break;		
					}
				}
			default:
				retPostdata=postdata;
				break;
			}		
		}catch(IOException e){e.printStackTrace();}
		finally {
			return retPostdata;
		}
		
	}
	
	public static void main(String[] args) throws Exception {
                //String postdata="FPLVSUxLBLWTSXU42VSTphDvrt+4N/Dgg98AGwTGreaSx+02QKL7UaP9g25SBcW3wVJbIWyFmdg1TWijkOClg7VLbC9RNDKecYrtY9FMasrBjRfaOsPSnotLc56WZnW5mRMgZWnCz9uO2oIXAPCTvFydjXwUpkDSxsl0KHQQ6cLOlzNgh5yzZesB6XiSdXYKeaydFZtaScf0HgWZwweiqajmWZVnot4cGKJzCmDChtp6CgcZ5/TSE1Of34d9mJWTwYU0bfjb/ah1BqYMB/Wh4ZPqIoWoCrzSN/+IKg9Phnh9foRBAlljaTNxQwCtZcdHyOwfkcePHtXp4hueE1BRfylrAjYC5fzZb9A75vR9lkNFT73XLWeLh1ar83QTDQiSwHy4nLbqfymmvtIx5NlFrsWcKQrO9o0vfJp7fF2ECemUU0u8afKxrQlqEnleke6Teytg3tBeef8xc0qduKXga36vcqf6C2gAgHlO/RZ/L0XjZWk9EKG3du188oU6rolhKbsCOjEb4wXtoPGxBLu/LDHqbS+HZtR6SvBW0wNp2HjWbPuVkauu8ZSuATlJJk+fSwA9X76ndDLdTbuulciAqqsW6RvqqNI/4YcHwvSDOwo=";
                //String postdata="YXBwa2V5PThjMDAyYzVjMzBkNSZkZXZpY2U9NDBjZmIyMjg2OTY5ODExZmYyZTQ1NGI5OWRlOGUyYmZkN2Y2YmZmYiZwbGF0PTEmYXBwcGtnPWNvbS5kZXdtb2JpbGUua3VhaXlhJmFwcHZlcj0xNDQmc2RrdmVyPTYwMDU2Jm5ldHdvcmt0eXBlPXdpZmk=";
		String postdata="dnY9MCUwOTIwMTYtMDctMjYrMDAlM0EwNCUzQTAwJTA5MzUyNzQ2MDY2Njc0MjUyJTA5R1Bob25lJTA5NC40LjIlMDkxOTIwKjEwODAlMDlTTS1OOTAwJTA5MSUwOTExODk4ODAwMzElMDk2OTg0MjY0MjQ4M2FkZDBhNjM1MDMzMDZkNjNmMDQ0MyUwOTcuMyUwOTUxMzE0MDkwMCUwOTEzJTA5MSUwOTEwJTA5JTA5MCUwOTAlMDkxJTA5MSUwOTIxNTMlMDkxJTA5NzI3NDglMDkxNiUwOTAlMDkxJTA5MjAwJTA5MCUwOTExMy4wNzQ0MyUyQzIyLjg2ODM4NCUwOTIwMCUwOTUxMzE0MDkwMCUwOTAlMDkxMSUwOSUwOWE0MzQ0YWJlNmU1NmE5MjgwYmViMGVkNWFiNTc0ZTc2JTA5ZTc3Yjg3OTkyNzQzNTJjZiUwOSUwOTElMDkwJTA5MSUwOTc0MjE3JTA5MCUwOTElMDk2MTNBMDFFNjYzNkY1MTc2NUNDN0M4QTYzOTM3RjVGQyUwOSUyNnVwZGVyaWQlM0QlMDklMDkwJTA5JTA5JTA5JTA5cXlfaG9tZSUyQ1IlM0EyMDQ4MzUxMTIlM0E2JTJDMTAwJTJDKyUyQzElMDklMDklMDkwJTA5MCUwOTAlMDkwJTA5LTElMDklN0IlMjJydHlwZSUyMiUzQSUyMjElMjIlMkMlMjJwb3AlMjIlM0ElMjIxJTIyJTJDJTIycG9zJTIyJTNBJTIyMCUyMiU3RCUwOSUwOXZiN3JuaGExY2s1a3pvdGQlMDkwJTA5Y25fcw==";
		String retPostData=DPIPost.decodePostData(postdata);
		if(retPostData != postdata) System.out.println(retPostData);
	}
}
```





