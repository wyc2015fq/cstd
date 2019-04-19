# OkHttp上传文件，服务器端请求解析找不到文件信息的问题 - =朝晖= - 博客园
# [OkHttp上传文件，服务器端请求解析找不到文件信息的问题](https://www.cnblogs.com/dhcn/p/7121330.html)
长话短说，不深入解释了，官方给的上传案例代码：
- private static final String IMGUR_CLIENT_ID = "...";  
- private static final MediaType MEDIA_TYPE_PNG = MediaType.parse("image/png");  
- 
- private final OkHttpClient client = new OkHttpClient();  
- 
- public void run() throws Exception {  
- // Use the imgur image upload API as documented at https://api.imgur.com/endpoints/image  
-     RequestBody requestBody = new MultipartBuilder()  
-         .type(MultipartBuilder.FORM)  
-         .addPart(  
-             Headers.of("Content-Disposition", "form-data; name=\"title\""),  
-             RequestBody.create(null, "Square Logo"))  
-         .addPart(  
-             Headers.of("Content-Disposition", "form-data; name=\"image\""),  
-             RequestBody.create(MEDIA_TYPE_PNG, new File("website/static/logo-square.png")))  
-         .build();  
- 
-     Request request = new Request.Builder()  
-         .header("Authorization", "Client-ID " + IMGUR_CLIENT_ID)  
-         .url("https://api.imgur.com/3/image")  
-         .post(requestBody)  
-         .build();  
- 
-     Response response = client.newCall(request).execute();  
- if (!response.isSuccessful()) throw new IOException("Unexpected code " + response);  
- 
-     System.out.println(response.body().string());  
-   }  
上面这个代码上传文件在[Java](http://lib.csdn.net/base/java)服务器端用[spring](http://lib.csdn.net/base/javaee) boot的 MultiPart解析不到上传的文件，用[Python](http://lib.csdn.net/base/python) Django，问题亦然。
找到了一个泡网同学的代码：
做了改动1：
- RequestBody requestBody = new MultipartBuilder()  
-                 .type(MultipartBuilder.FORM)  
-                 .addPart(  
-                         Headers.of("Content-Disposition", "form-data; name=\"UserId\""),  
-                         RequestBody.create(null, UserId))  
-                 .addPart(  
-                         Headers.of("Content-Disposition", "form-data; name=\"SecurityCode\""),  
-                         RequestBody.create(null, SecurityCode))  
-                 .addPart(  
-                         Headers.of("Content-Disposition", "form-data; name=\"File\"; filename =\"avatar_upload.jpg\""),  
-                         RequestBody.create(MediaType.parse("image/jpeg"), file))  
- 
- 
-                 .build();  
改为以上代码后，Django请求解析可以找到文件了，Spring boot的multipart任然找不到文件信息。
改动1的改动点是：文件的Header信息里面必须有上传文件的名字，当然这个名字其实是随便起的，一般后缀保持正确即可。
这个时候不得不再参考泡网同学的代码，做了修改2：
- RequestBody requestBody = new MultipartBuilder()  
-                 .type(MultipartBuilder.FORM)  
-                 .addPart(  
-                         Headers.of("Content-Disposition", "form-data; name=\"UserId\""),  
-                         RequestBody.create(null, UserId))  
-                 .addPart(  
-                         Headers.of("Content-Disposition", "form-data; name=\"SecurityCode\""),  
-                         RequestBody.create(null, SecurityCode))  
-                 .addPart(  
-                         Headers.of("Content-Disposition", "form-data; name=\"File\"; filename =\"avatar_upload.jpg\""),  
-                         RequestBody.create(MediaType.parse("application/octet-stream"), file))  
- 
-                 .build();  
 这回Spring Boot multipart 依旧找不到文件信息了，这回的改动点是把mime类型由图片类型"image/jpeg"改成了附件类型：
- "application/octet-stream"  
 最后借鉴StackOverFlow的某贴的代码：http://stackoverflow.com/questions/30707681/multipart-file-upload-with-okhttp-spring，
- RequestBody body = new MultipartBuilder()  
-     .addFormDataPart("qqfile", filename, RequestBody.create(MediaType.parse("media/type"), new File(filename)))  
-     .type(MultipartBuilder.FORM)  
-     .build();  
- 
- Request request = new Request.Builder()  
-     .url("/path/to/your/upload")  
-     .post(body)  
-     .build();  
- 
- OkHttpClient client = new OkHttpClient();  
- Response response = client.newCall(request).execute();  
因为这个帖子的topic就是解决Spring MultiPart的问题，最后终于可以上传了，修改点在于彻底修改了添加文件使用的函数接口，我在build gradle里面已经准备用Apache HttpClient了。
OkHttp有点坑爹啊。
参考：
https://github.com/square/okhttp/wiki/Recipes
http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2015/0824/3355.html

