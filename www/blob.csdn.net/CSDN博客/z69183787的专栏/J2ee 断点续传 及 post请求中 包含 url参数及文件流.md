# J2ee 断点续传 及 post请求中 包含 url参数及文件流 - z69183787的专栏 - CSDN博客
2013年05月03日 10:50:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3535
个人分类：[Http请求-HttpURLConnection																[文件处理-断点续传](https://blog.csdn.net/z69183787/article/category/2186153)](https://blog.csdn.net/z69183787/article/category/2175775)
首先
client端：
```java
HttpURLConnection conn = null;
		BufferedInputStream fin = null;
		BufferedOutputStream out = null;
		URL reqUrl;
		try {
			reqUrl = new URL("http://<ip>:<port>/Emergency/phone/attachmentUpload");
			conn = (HttpURLConnection) reqUrl.openConnection();
			conn.setConnectTimeout(3000);
			conn.setRequestMethod("PUT");
			if(isCommit){
				conn.setRequestProperty("isCommit", "true");
			}else{
				conn.setRequestProperty("isCommit", "false");
			}
			conn.setRequestMethod("PUT");
			conn.setRequestProperty("Content-Type", "binary/octet-stream");
			conn.setRequestProperty("offset", StringUtil.toString(offset));
			conn.setDoOutput(true);
			conn.setDoInput(true);
			// 1M的chunk缓冲
			conn.setChunkedStreamingMode(1024*1024);
			out = new BufferedOutputStream(conn.getOutputStream());
			fin = new BufferedInputStream(new FileInputStream(file));
			byte[] buf = new byte[bufferSizeUpload];
			int len = -1;
			long currentUploadSize = offset;
			fin.skip(offset);
			while ((len = fin.read(buf)) != -1¤tUploadSize<offset+uploadSize) {
				if(offset+uploadSize-currentUploadSize<bufferSizeUpload){
					len = Integer.parseInt(StringUtil.toString(offset+uploadSize-currentUploadSize));
				}
				if(len>0){
					if(out!=null){
						out.write(buf, 0, len);
						out.flush();
					}
				}
				currentUploadSize += len;
			}
		} catch (SocketTimeoutException e) {
			e.printStackTrace();
		} catch(IOException e){
			e.printStackTrace();
		} finally {
			try {
				if (fin != null) {
					fin.close();
					fin = null;
				}
				if (out != null) {
					out.close();
					out = null;
				}
				if (conn != null) {
					conn.disconnect();
					conn = null;
				}
			} catch (IOException ioe) {
				ioe.printStackTrace();
				throw new RuntimeException("Release resource failed.");
			}
		}
```
如上所示：url参数通过setRequestProperty方法放在请求头中 ，而文件流放在 body 中。
同时需要注意 HttpURLConnection.setChunkedStreamingMode 此方法保证每次文件流达到指定大小就发送一次，避免了放在缓存并一次性传输中可能遇到的数据缺失。
[http://blog.csdn.net/z69183787/article/details/8186918](http://blog.csdn.net/z69183787/article/details/8186918) 可参考
服务端：
```java
public String underUpload() throws IOException{
		String data = request.getHeader("data"); 
		String userInfo = request.getHeader("userInfo"); 
		userInfo = new String(userInfo.getBytes("iso-8859-1"),"GBK");
		//System.out.println(data);
		//System.out.println(userInfo);
		Map<String,String> data_map = gson.fromJson(data,attachVo.data_map.getClass());
		if(data_map!=null) attachVo.data_map=data_map;
		attachVo.user = gson.fromJson(userInfo,attachVo.user.getClass());
		apiService.attachUpload(attachVo,new BufferedInputStream(request.getInputStream()));
		//response.setStatus(200);
		return null;
	}
```
使用的是ssh架构 ，通过getHeader 得到 url参数，通过getInputStream得到文件流，同时还要控制好编码，以防中文乱码。
至于断点续传，基本思想是通过信息头的部分 传递每次传输的文件大小，与服务器端的文件大小匹配。并通过
```java
RandomAccessFile raFile = new RandomAccessFile(uploadFile.getAbsolutePath(), "rw");
		raFile.seek(uploadFile.length());
```
方法进行根据传输大小实时进行文件读写。
```java
public static void saveFile(String destFilePathStr,String destFileName){
		try {
			File destFilePath = new File(destFilePathStr);
			if(!destFilePath.exists()){
				destFilePath.mkdirs();
				destFilePath = null;
			}
			File destFile = new File(destFilePathStr+"//"+destFileName);
			if(!destFile.exists()){
				destFile.createNewFile();
			}
	
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public static long uploadFile(String offset,String destFileName,BufferedInputStream bis) 
	throws IOException{
		File uploadFile = new File(destFileName);
		int len = 0;
		byte[] bt = new byte[1024];
		RandomAccessFile raFile = new RandomAccessFile(uploadFile.getAbsolutePath(), "rw");
		raFile.seek(uploadFile.length());
	    while ((len = bis.read(bt)) > 0){
	    	raFile.write(bt, 0, len);
    	}
	    long l = raFile.length();
	    try {
		     if(bis != null)
		    	 bis.close();
		     if (raFile != null)
		    	 raFile.close();
		     
		} catch (IOException e) {
			l = 0;
		    e.printStackTrace();
		}
		return l ;
	}
```
首先创建一个空文件，接下来根据每次接收的文件流及文件长度 写入文件。
