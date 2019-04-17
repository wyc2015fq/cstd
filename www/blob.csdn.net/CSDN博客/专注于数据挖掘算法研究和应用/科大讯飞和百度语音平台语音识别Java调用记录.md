# 科大讯飞和百度语音平台语音识别Java调用记录 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年09月28日 15:59:23[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9121








                
1、科大讯飞平台：http://www.xfyun.cn/doccenter/java

     注册并创建应用，下载SDK，选择语音听写+Java+创建的应用(APPID)；

     未提交审核的应用，服务量会有500次/日的限制，开发完成后提交审核解除限制；

     SDK里面的Sample功能直接导入eclipse，编译执行，支持粤语；




2、百度http://yuyin.baidu.com/

     注册并 创建应用，下载REST API的压缩文档，提供Http接口支持整段录音文件的识别；

     原始语音的录音格式目前支持评测 8k/16k 采样率 16bit 位深的单声道语音；

     压缩格式支持：pcm（不压缩）、wav、opus、speex、amr、x-flac；

     语音识别服务开通成功后即可获得 50000次/日 的在线识别调用配额；

     http://yuyin.baidu.com/docs/asr/57

     Sample代码



```java
package org.asr.baidu.speech.serviceapi;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import javax.xml.bind.DatatypeConverter;

import org.asr.json.JSONObject;

public class Sample {

    private static final String serverURL = "http://vop.baidu.com/server_api";
    private static String token = "";
    //private static final String testFileName = "test.pcm";
    private static final String testFileName = "D:\\tmp\\1.wav";
    //put your own params here
    private static final String apiKey = "";
    private static final String secretKey = "";
    private static final String cuid = "";

    public static void main(String[] args) throws Exception {
        getToken();
        //method1();
        method2();
    }

    private static void getToken() throws Exception {
        String getTokenURL = "https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials" + 
            "&client_id=" + apiKey + "&client_secret=" + secretKey;
        HttpURLConnection conn = (HttpURLConnection) new URL(getTokenURL).openConnection();
        token = new JSONObject(printResponse(conn)).getString("access_token");
    }

    private static void method1() throws Exception {
        File pcmFile = new File(testFileName);
        HttpURLConnection conn = (HttpURLConnection) new URL(serverURL).openConnection();

        // construct params
        JSONObject params = new JSONObject();
        params.put("format", "pcm");
        params.put("rate", 8000);
        params.put("channel", "1");
        params.put("token", token);
        params.put("cuid", cuid);
        params.put("len", pcmFile.length());
        params.put("speech", DatatypeConverter.printBase64Binary(loadFile(pcmFile)));

        // add request header
        conn.setRequestMethod("POST");
        conn.setRequestProperty("Content-Type", "application/json; charset=utf-8");

        conn.setDoInput(true);
        conn.setDoOutput(true);

        // send request
        DataOutputStream wr = new DataOutputStream(conn.getOutputStream());
        wr.writeBytes(params.toString());
        wr.flush();
        wr.close();

        printResponse(conn);
    }

    private static void method2() throws Exception {
        File pcmFile = new File(testFileName);
        HttpURLConnection conn = (HttpURLConnection) new URL(serverURL
                + "?cuid=" + cuid + "&token=" + token).openConnection();

        // add request header
        conn.setRequestMethod("POST");
        conn.setRequestProperty("Content-Type", "audio/pcm; rate=8000");

        conn.setDoInput(true);
        conn.setDoOutput(true);

        // send request
        DataOutputStream wr = new DataOutputStream(conn.getOutputStream());
        wr.write(loadFile(pcmFile));
        wr.flush();
        wr.close();

        printResponse(conn);
    }

    private static String printResponse(HttpURLConnection conn) throws Exception {
        if (conn.getResponseCode() != 200) {
            // request error
            return "";
        }
        InputStream is = conn.getInputStream();
        BufferedReader rd = new BufferedReader(new InputStreamReader(is));
        String line;
        StringBuffer response = new StringBuffer();
        while ((line = rd.readLine()) != null) {
            response.append(line);
            response.append('\r');
        }
        rd.close();
        System.out.println(new JSONObject(response.toString()).toString(4));
        return response.toString();
    }

    private static byte[] loadFile(File file) throws IOException {
        InputStream is = new FileInputStream(file);

        long length = file.length();
        byte[] bytes = new byte[(int) length];

        int offset = 0;
        int numRead = 0;
        while (offset < bytes.length
                && (numRead = is.read(bytes, offset, bytes.length - offset)) >= 0) {
            offset += numRead;
        }

        if (offset < bytes.length) {
            is.close();
            throw new IOException("Could not completely read file " + file.getName());
        }

        is.close();
        return bytes;
    }
}
```




3）两个平台都提供相对详细的开发者教程，Demo也可以直接使用，相对来说科大讯飞平台支持方言和语音训练，百度暂时没发现有语音训练功能。



