# springboot使用JSch远程读取sshd服务器上的文件 - 零度的博客专栏 - CSDN博客
2018年08月23日 09:57:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：664
JSch 是SSH2的一个纯Java实现。它允许你连接到一个sshd 服务器，使用端口转发，X11转发，文件传输等等。你可以将它的功能集成到你自己的 程序中。同时该项目也提供一个J2ME版本用来在手机上直连SSHD服务器。实现一个java工具类。
引用：
  <dependency>
      <groupId>com.jcraft</groupId>
      <artifactId>jsch</artifactId>
  </dependency>
java工具类：
```
package com.citipub_zxsy.count.utils;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import org.apache.commons.lang3.StringUtils;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.jcraft.jsch.Channel;
import com.jcraft.jsch.ChannelExec;
import com.jcraft.jsch.JSch;
import com.jcraft.jsch.JSchException;
import com.jcraft.jsch.Session; 
/**
 * @decription 执行远程shell命令，并获取结果--实现分析日志
 * @author zhangwenchao
 * @date  2018/6/8
 *
 */
public class ShellUtils {
	
	/**配置连接
	 * @param user
	 * @param passwd
	 * @param host
	 * @param post
	 * @throws Exception
	 */
	public static Session connect(String user, String passwd, String host,int post) throws Exception {
		JSch  jsch = new JSch();
		Session session = jsch.getSession(user, host, post);
	    if (session == null) {
	        throw new Exception("session is null");
	    }
	    session.setPassword(passwd);
	    java.util.Properties config = new java.util.Properties();
	    //第一次登陆
	    config.put("StrictHostKeyChecking", "no");
	    session.setConfig(config);
	    try {
	        session.connect(30000);
	    } catch (Exception e) {
	        throw new Exception("连接远程端口无效或用户名密码错误");
	    }
        return session;
	}
	/**
	 * @description 执行shell命令
	 * @param command shell 命令
	 * @param user 用户名
	 * @param passwd 密码 
	 * @param host ip地址
	 * @param post 端口号
	 * @throws Exception
	 */
	public static void execCmd(String command, String user, String passwd, String host, int port) throws Exception {
	    System.out.println(command);
	    Session session= connect(user, passwd,host,port);
	    BufferedReader reader = null;
	    Channel channel = null;
	    try {
	            channel = session.openChannel("exec");
	            ((ChannelExec) channel).setCommand(command);
	            channel.setInputStream(null);
	            ((ChannelExec) channel).setErrStream(System.err);
	            channel.connect();
	            InputStream in = channel.getInputStream();
	            reader = new BufferedReader(new InputStreamReader(in));
	            String buf = null;	           
	            //返回数据
	            while ((buf = reader.readLine()) != null) {
	            	System.out.println(buf);
	            }
	    } catch (IOException e) {
	        e.printStackTrace();
	    } catch (JSchException e) {
	        e.printStackTrace();
	    } finally {
	        try {
	            reader.close();
	        } catch (IOException e) {
	            e.printStackTrace();
	        }
	        channel.disconnect();
	        session.disconnect();
	    }
	}
	
	
	
	public static List<String> execCmdAndOutput(String command, String user, String passwd, String host, int port) throws Exception {
	   
	    Session session= connect(user, passwd,host,port);
	    BufferedReader reader = null;
	    Channel channel = null;
	    try {
	            channel = session.openChannel("exec");
	            ((ChannelExec) channel).setCommand(command);
	            channel.setInputStream(null);
	            ((ChannelExec) channel).setErrStream(System.err);
	            channel.connect();
	            InputStream in = channel.getInputStream();
	            reader = new BufferedReader(new InputStreamReader(in));
	            List<String> output = new ArrayList<String>();
	            String buf = null;	           
	            //返回数据
	            while ((buf = reader.readLine()) != null) {
	            	output.add(buf);	            	
	            }
	            return output;
	    } catch (IOException e) {
	        e.printStackTrace();
	    } catch (JSchException e) {
	        e.printStackTrace();
	    } finally {
	        try {
	            reader.close();
	        } catch (IOException e) {
	            e.printStackTrace();
	        }
	        channel.disconnect();
	        session.disconnect();
	    }
	    return null;
	}
	
	
	/**
	 * 
	 * @param logPath
	 * @param fileList
	 * @param user
	 * @param passwd
	 * @param host
	 * @param port
	 * @return
	 * @throws Exception
	 */
	public static Set<String> execCmdAndgetOlderUserIDs(Set<String> userIDSet, String logPath, List<String> fileNames, String user, String passwd, String host, int port) throws Exception {
		Session session= connect(user, passwd,host,port);	  
	    try {
			for (String fileName : fileNames) {
				String command = "cat "+fileName+"|grep '/login'";				
				Channel channel = null;
				BufferedReader reader = null;
				try {
					channel = session.openChannel("exec");
					((ChannelExec) channel).setCommand(command);
					channel.setInputStream(null);
					((ChannelExec) channel).setErrStream(System.err);
					channel.connect();
					InputStream in = channel.getInputStream();
					reader = new BufferedReader(new InputStreamReader(in,"UTF-8"));
					String buf = null;
					//返回数据
					while ((buf = reader.readLine()) != null) {
						analyseLogOneLine(userIDSet, buf);
					}
				} finally {
					reader.close();
					channel.disconnect();
				}
			} 
		} finally {
			session.disconnect();
		}
	    return userIDSet;
	}
	
	
	/**
     * 解析一行日志，获取老用户uid存放到Set中并返回
     * @param map
     * @param str
     * @return
     */
	private static Set<String> analyseLogOneLine(Set<String> userIDSet, String str) {
		String[] logLine  = str.split("\\s+\\|\\s+");
		String userToken = null;
		String cacheResult=null;
		if(logLine!=null && logLine.length==12){
			userToken= logLine[6].trim();
			cacheResult = logLine[10].trim();
		}		        
		if(StringUtils.isNotEmpty(cacheResult)){
			
			JSONObject jsonObject = JSON.parseObject(cacheResult);
			
			JSONObject body = (JSONObject)jsonObject.get("body");
			
			if(body!=null){
				String userID = (String)body.get("userID");
				if(StringUtils.isNotEmpty(userID)&&StringUtils.isNotEmpty(userToken)&&userToken.contains("anony")){
					userToken = userToken.substring(0, userToken.length()-6);
					if(!userToken.equals(userID)){
						userIDSet.add(userID);
					}
				}
			}
			
			
			
		        	
		}
		return userIDSet;
	}
	
	
	
	
	
	public static void main(String[] args) throws Exception {
		
		execCmd("cd /web/logs/sc_zxsy; cat api-2018-06-07.0.log","yunpub","XXXXXXX","xxx.xxx.xx.xxx",22);
	}
}
```
> 
