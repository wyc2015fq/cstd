
# Android 文件下载三种基本方式 - 小小情意 - 博客园






# [Android 文件下载三种基本方式](https://www.cnblogs.com/xiaoxiaoqingyi/p/7003241.html)

### 一、自己封装URLConnection 连接请求类
publicvoiddownloadFile1() {try{//下载路径，如果路径无效了，可换成你的下载路径String url = "http://c.qijingonline.com/test.mkv";
        String path=Environment.getExternalStorageDirectory().getAbsolutePath();finallongstartTime =System.currentTimeMillis();
        Log.i("DOWNLOAD","startTime="+startTime);//下载函数String filename=url.substring(url.lastIndexOf("/") + 1);//获取文件名URL myURL =newURL(url);
        URLConnection conn=myURL.openConnection();
        conn.connect();
        InputStream is=conn.getInputStream();intfileSize = conn.getContentLength();//根据响应获取文件大小if(fileSize <= 0)thrownewRuntimeException("无法获知文件大小 ");if(is ==null)thrownewRuntimeException("stream is null");
        File file1=newFile(path);if(!file1.exists()){
            file1.mkdirs();
        }//把数据存入路径+文件名FileOutputStream fos =newFileOutputStream(path+"/"+filename);bytebuf[] =newbyte[1024];intdownLoadFileSize = 0;do{//循环读取intnumread =is.read(buf);if(numread == -1)
            {break;
            }
            fos.write(buf,0, numread);
            downLoadFileSize+=numread;//更新进度条}while(true);
        Log.i("DOWNLOAD","download success");
        Log.i("DOWNLOAD","totalTime="+ (System.currentTimeMillis() -startTime));
        is.close();
    }catch(Exception ex) {
        Log.e("DOWNLOAD", "error: " +ex.getMessage(), ex);
    }
}
这种方式在Android 刚兴起的时候，很少下载封装框架，就自己封装了。虽然一般的文件都能下载，但这种方式缺点很多，不稳定或者各种各样的问题会出现。

### 二、Android自定的下载管理（会在notification 显示下载的进度，同时可以暂停、重新连接等）
privatevoiddownloadFile2(){//下载路径，如果路径无效了，可换成你的下载路径String url = "http://c.qijingonline.com/test.mkv";//创建下载任务,downloadUrl就是下载链接DownloadManager.Request request =newDownloadManager.Request(Uri.parse(url));//指定下载路径和下载文件名request.setDestinationInExternalPublicDir("", url.substring(url.lastIndexOf("/") + 1));//获取下载管理器DownloadManager downloadManager=(DownloadManager) getSystemService(Context.DOWNLOAD_SERVICE);//将下载任务加入下载队列，否则不会进行下载downloadManager.enqueue(request);
}
这种方式其实就是交给了Android系统的另一个app去下载管理。这样的好处不会消耗该APP的 CPU资源。缺点是：控制起来很不灵活。

### 三、使用第三方 okhttp 网络请求框架
privatevoiddownloadFile3(){//下载路径，如果路径无效了，可换成你的下载路径finalString url = "http://c.qijingonline.com/test.mkv";finallongstartTime =System.currentTimeMillis();
    Log.i("DOWNLOAD","startTime="+startTime);
    Request request=newRequest.Builder().url(url).build();newOkHttpClient().newCall(request).enqueue(newCallback() {
        @OverridepublicvoidonFailure(Call call, IOException e) {//下载失败e.printStackTrace();
            Log.i("DOWNLOAD","download failed");
        }
        @OverridepublicvoidonResponse(Call call, Response response)throwsIOException {
            Sink sink=null;
            BufferedSink bufferedSink=null;try{
                String mSDCardPath=Environment.getExternalStorageDirectory().getAbsolutePath();
                File dest=newFile(mSDCardPath,   url.substring(url.lastIndexOf("/") + 1));
                sink=Okio.sink(dest);
                bufferedSink=Okio.buffer(sink);
                bufferedSink.writeAll(response.body().source());
                bufferedSink.close();
                Log.i("DOWNLOAD","download success");
                Log.i("DOWNLOAD","totalTime="+ (System.currentTimeMillis() -startTime));
            }catch(Exception e) {
                e.printStackTrace();
                Log.i("DOWNLOAD","download failed");
            }finally{if(bufferedSink !=null){
                    bufferedSink.close();
                }
            }
        }
    });
}
okhttp是一个很有名气的开源框架，目前已经很多大公司都直接使用它作为网络请求库（七牛云SDK， 阿里云SDK）。 且里面集成了很多优势，包括 okio (一个I/O框架，优化内存与CPU)。
在接收数据的时候使用了 okio框架 来做一些I/O处理，okio框架是弥补Java.io 上的不足，节省CPU与内存资源，但这个例子，我没还没看到其效果，也可以自己来接收字符流，然后获取下载的进度
publicvoiddownloadFile(){finalString url = "http://c.qijingonline.com/test.mkv";finallongstartTime =System.currentTimeMillis();
        Log.i("DOWNLOAD","startTime="+startTime);
        OkHttpClient okHttpClient=newOkHttpClient();
        Request request=newRequest.Builder().url(url).build();
        okHttpClient.newCall(request).enqueue(newCallback() {
            @OverridepublicvoidonFailure(Call call, IOException e) {//下载失败e.printStackTrace();
                Log.i("DOWNLOAD","download failed");
            }
            @OverridepublicvoidonResponse(Call call, Response response)throwsIOException {
                InputStream is=null;byte[] buf =newbyte[2048];intlen = 0;
                FileOutputStream fos=null;//储存下载文件的目录String savePath =Environment.getExternalStorageDirectory().getAbsolutePath();try{
                    is=response.body().byteStream();longtotal =response.body().contentLength();
                    File file=newFile(savePath, url.substring(url.lastIndexOf("/") + 1));
                    fos=newFileOutputStream(file);longsum = 0;while((len = is.read(buf)) != -1) {
                        fos.write(buf,0, len);
                        sum+=len;intprogress = (int) (sum * 1.0f / total * 100);//下载中//listener.onDownloading(progress);}
                    fos.flush();//下载完成//listener.onDownloadSuccess();Log.i("DOWNLOAD","download success");
                    Log.i("DOWNLOAD","totalTime="+ (System.currentTimeMillis() -startTime));
                }catch(Exception e) {
                    e.printStackTrace();//listener.onDownloadFailed();Log.i("DOWNLOAD","download failed");
                }finally{try{if(is !=null)
                            is.close();
                    }catch(IOException e) {
                    }try{if(fos !=null)
                            fos.close();
                    }catch(IOException e) {
                    }
                }
            }
        });
    }


综合来看，第三种方案是最佳的，是目前最流行的下载方案。






