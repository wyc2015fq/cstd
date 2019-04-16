# Android端通过HttpURLConnection上传文件到服务器 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年01月22日 20:28:13[gloomyfish](https://me.csdn.net/jia20003)阅读数：11458








Android端通过HttpURLConnection上传文件到服务器

一：实现原理

最近在做Android客户端的应用开发，涉及到要把图片上传到后台服务器中，自己选择了做Spring3 MVC HTTP API作为后台上传接口，android客户端我选择用HttpURLConnection来通过form提交文件数据实现上传功能，本来想网上搜搜拷贝一下改改代码就好啦，发现根本没有现成的例子，多数的例子都是基于HttpClient的或者是基于Base64编码以后作为字符串来传输图像数据，于是我不得不自己动手，参考了网上一些资料，最终实现基于HttpURLConnection上传文件的android客户端代码，废话少说，其实基于HttpURLConnection实现文件上传最关键的在于要熟悉Http协议相关知识，知道MIME文件块在Http协议中的格式表示，基本的传输数据格式如下：

![](https://img-blog.csdn.net/20160122202047690)




其中boundary表示form的边界，只要按照格式把内容字节数写到HttpURLConnection的对象输出流中，服务器端的Spring Controller 就会自动响应接受，跟从浏览器页面上上传文件是一样的。
服务器端HTTP API， 我是基于Spring3 MVC实现的Controller，代码如下：



```java
@RequestMapping(value = "/uploadMyImage/{token}", method = RequestMethod.POST)
public @ResponseBody String getUploadFile(HttpServletRequest request, HttpServletResponse response,
		@PathVariable String token) {
	logger.info("spring3 MVC upload file with Multipart form");
	logger.info("servlet context path : " + request.getSession().getServletContext().getRealPath("/"));
	UserDto profileDto = userService.getUserByToken(token);
	String imgUUID = "";
	try {
		if (request instanceof MultipartHttpServletRequest && profileDto.getToken() != null) {
			MultipartHttpServletRequest multipartRequest = (MultipartHttpServletRequest) request;
			logger.info("spring3 MVC upload file with Multipart form");
			// does not work, oh my god!!
			MultipartFile file = multipartRequest.getFiles("myfile").get(0);
			InputStream input = file.getInputStream();
			long fileSize = file.getSize();
			BufferedImage image = ImageIO.read(input);
			// create data transfer object
			ImageDto dto = new ImageDto();
			dto.setCreateDate(new Date());
			dto.setFileName(file.getOriginalFilename());
			dto.setImage(image);
			dto.setCreator(profileDto.getUserName());
			dto.setFileSize(fileSize);
			dto.setType(ImageAttachmentType.CLIENT_TYPE.getTitle());
			dto.setUuid(UUID.randomUUID().toString());

			/// save to DB
			imgUUID = imageService.createImage(dto);
			input.close();
		}
	} catch (Exception e) {
		e.printStackTrace();
		logger.error("upload image error", e);
	}

	return imgUUID;
}
```



Android客户端基于HttpURLConnection实现上传的代码，我把它封装成一个单独的类文件，这样大家可以直接使用，只要传入上传的URL等参数即可。代码如下：

```java
package com.demo.http;

import java.io.BufferedInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Random;

import android.os.Handler;
import android.util.Base64;
import android.util.Log;

public class UploadImageTask implements APIURLConstants {
	private String requestURL = DOMAIN_ADDRESS + UPLOAD_DESIGN_IMAGE_URL; // default
	private final String CRLF = "\r\n";
	private Handler handler;
	private String token;
	public UploadImageTask(String token, Handler handler) {
		this.handler = handler;
		this.token = token;
	}

	public String execute(File...files) {
		InputStream inputStream = null;
		HttpURLConnection urlConnection = null;
		FileInputStream fileInput = null;
		DataOutputStream requestStream = null;
		handler.sendEmptyMessage(50);
		try {
			// open connection
			URL url = new URL(requestURL.replace("{token}", this.token));
			urlConnection = (HttpURLConnection) url.openConnection();
			// create random boundary
			Random random = new Random();
			byte[] randomBytes = new byte[16];
			random.nextBytes(randomBytes);
			String boundary = Base64.encodeToString(randomBytes, Base64.NO_WRAP);

			/* for POST request */
			urlConnection.setDoOutput(true);
			urlConnection.setDoInput(true);
			urlConnection.setUseCaches(false);
			urlConnection.setRequestMethod("POST");
			long size = (files[0].length() / 1024);
			if(size >= 1000) {
				handler.sendEmptyMessage(-150);
				return "error";
			}
			// 构建Entity form
			urlConnection.setRequestProperty("Connection", "Keep-Alive");
			urlConnection.setRequestProperty("Content-Type", "multipart/form-data;boundary=" + boundary);
			urlConnection.setRequestProperty("Cache-Control", "no-cache");
			
			// never try to chunked mode, you need to set a lot of things
			//	if(size > 400) {
			//		urlConnection.setChunkedStreamingMode(0);
			//	}
			//	else {
			//		urlConnection.setFixedLengthStreamingMode((int)files[0].length());
			//	}
			// end comment by zhigang on 2016-01-19
			
			/* upload file stream */
			fileInput = new FileInputStream(files[0]);
			requestStream = new DataOutputStream(urlConnection.getOutputStream());
			String nikeName = "myfile";
			requestStream = new DataOutputStream(urlConnection.getOutputStream());
			requestStream.writeBytes("--" + boundary + CRLF);
			requestStream.writeBytes("Content-Disposition: form-data; name=\"" + nikeName + "\"; filename=\"" + files[0].getName() + "\""+ CRLF);
			requestStream.writeBytes("Content-Type: " + getMIMEType(files[0]) + CRLF);
			requestStream.writeBytes(CRLF);
			// 写图像字节内容
			int bytesRead;
			byte[] buffer = new byte[8192];
			handler.sendEmptyMessage(50);
			while((bytesRead = fileInput.read(buffer)) != -1) {
				requestStream.write(buffer, 0, bytesRead);
			}
			requestStream.flush();
			requestStream.writeBytes(CRLF);
			requestStream.flush();
			requestStream.writeBytes("--" + boundary + "--" + CRLF);
			requestStream.flush();
			fileInput.close();

			// try to get response
			int statusCode = urlConnection.getResponseCode();
			if (statusCode == 200) {
				inputStream = new BufferedInputStream(urlConnection.getInputStream());
				String imageuuId = HttpUtil.convertInputStreamToString(inputStream);
				Log.i("image-uuid", "uploaded image uuid : " + imageuuId);
				handler.sendEmptyMessage(50);
				return imageuuId;
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (inputStream != null) {
				try {
					inputStream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if(requestStream != null) {
				try {
					requestStream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if(fileInput != null) {
				try {
					fileInput.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (urlConnection != null) {
				urlConnection.disconnect();
			}
		}
		handler.sendEmptyMessage(50);
		return null;
	}

	private String getMIMEType(File file) {
		String fileName = file.getName();
		if(fileName.endsWith("png") || fileName.endsWith("PNG")) {
			return "image/png";
		}
		else {
			return "image/jpg";
		}
	}

}
```
经过本人测试，效果杠杠的！！所以请忘记HttpClient这个东西，android开发再也不需要它了。





