# WindowsCE摩托罗拉MC3100 PDA扫描开启和HEX解码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年08月26日 13:56:39[boonya](https://me.csdn.net/boonya)阅读数：3961








﻿﻿PDA扫描设置参考：[http://blog.csdn.net/wang_cel/article/details/46845731](http://blog.csdn.net/wang_cel/article/details/46845731)


### 1.开启设备扫描功能

#### 1.1定位扫描应用开启的位置

打开控制面板

![](https://img-blog.csdn.net/20150711230202247?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击“DataWedge”图标

![](https://img-blog.csdn.net/20150711230227692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

双击图标打开；

#### 1.2设置扫描格式并开启回车功能

点击“Status：”，条码扫描开启

![](https://img-blog.csdn.net/20150711230245646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点“Basic format...“

![](https://img-blog.csdn.net/20150711230302679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点”Send data Enabled“

![](https://img-blog.csdn.net/20150711230319999?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点”7. Send ENTER key“，条码扫描后自动加回车键（根据需要也可选TAB键）

![](https://img-blog.csdn.net/20150711230342364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 2.扫描二维码后获取到的HEX串解码

![](https://img-blog.csdn.net/20150826135346015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如上界面通过扫描输入来查询，若不解码显示出来的是类似“53534e303031313530383234303031”的HEX码输入条件，此种情况自然就查不出相关的记录了；

#### 2.1解码工具类



```java
package com.wlyd.wms.util.api;
/**
 * 
 * @packge com.wlyd.wms.util.api.PDAHexDecoder
 * @date   2015年8月26日  上午10:47:23
 * @author wlyd
 * @comment   PDA扫描二维码HEX解码工具
 * @update
 */
public class PDAHexDecoder {

	/**
	 * 
	* @MethodName: hexStringToBytes 
	* @Description: hexString ->Bytes
	* @param hexString
	* @return
	* @throws
	 */
	public static byte[] hexStringToBytes(String hexString) {
		if (hexString == null || hexString.equals("")) {
			return null;
		}
		hexString = hexString.toUpperCase();
		int length = hexString.length() / 2;
		char[] hexChars = hexString.toCharArray();
		byte[] d = new byte[length];
		for (int i = 0; i < length; i++) {
			int pos = i * 2;
			d[i] = (byte) (charToByte(hexChars[pos]) << 4 | charToByte(hexChars[pos + 1]));

		}
		return d;
	}

	/**
	 * 
	* @MethodName: charToByte 
	* @Description: char ->Byte
	* @param c
	* @return
	* @throws
	 */
	private static byte charToByte(char c) {
		return (byte) "0123456789ABCDEF".indexOf(c);
	}

	/**
	 * 
	 * @MethodName: bytesToHexString
	 * @Description: byte——>String
	 * @param src
	 * @return
	 * @throws
	 */
	public static String bytesToHexString(byte[] src) {
		StringBuilder stringBuilder = new StringBuilder("");
		if (src == null || src.length <= 0) {
			return null;
		}
		for (int i = 0; i < src.length; i++) {
			int v = src[i] & 0xFF;
			String hv = Integer.toHexString(v);
			if (hv.length() < 2) {
				stringBuilder.append(0);
			}
			stringBuilder.append(hv);
		}
		return stringBuilder.toString();
	}

	/**
	 * 
	 * @MethodName: printHexString
	 * @Description: byte——>hexString
	 * @param b
	 * @return
	 * @throws
	 */
	public String printHexString(byte[] b) {
		String a = "";
		for (int i = 0; i < b.length; i++) {
			String hex = Integer.toHexString(b[i] & 0xFF);
			if (hex.length() == 1) {
				hex = '0' + hex;
			}

			a = a + hex;
		}

		return a;
	}

}
```






#### 2.2解码测试用例



```java
public class StringTest extends TestCase {

	@Test
	public void testHex() throws UnsupportedEncodingException{
		String hexString="53534e303031313530383234303031";
		byte[] bytes=PDAHexDecoder.hexStringToBytes(hexString);
		String result=new String(bytes, "UTF-8");
		System.out.println(result);
	}
	
	@Test
	public void testStringToHex(){
		String str="SSN001150824001";
		String hexString=PDAHexDecoder.bytesToHexString(str.getBytes());
		System.out.println(hexString);
	}
}
```






#### 2.3JavaScript处理解码



```java
// 扫描二维码HEX解码
	function decode()
	{
		var batchNo = $.trim($("#batchNo").val());
		if (!batchNo || batchNo == "") {
			return;
		}
		var param={"hexCode":batchNo};
		$.ajax({
			type : 'post',
			async : false,
			url : "${pageContext.request.contextPath}/pda/hex/decode.html?" + "&res=" + Math.random(),
			contentType : "application/json; charset=utf-8",
			dataType : 'json',
			timeout:5000,
			data : JSON.stringify(param),
			success : function(result) {
				if (result.statusCode == 101) {
					$("#batchNo").val(result.data);
				}else{
					alert(result.reason + "!");
				}
			},
			error : function() {
				alert("解码提示：网络访问异常!");
			}
		});
	}
```








