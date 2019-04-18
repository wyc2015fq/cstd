# Java使用smb访问远程共享目录 - z69183787的专栏 - CSDN博客
2013年11月05日 09:32:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12491
```java
private static String smburl = "smb://administrator:123@10.1.44.193/data";
	
	//zhoushun ----------------------------
	public static void createDir(String dir) throws Exception{
		SmbFile fp = new SmbFile(smburl+"//"+dir);
		System.out.println("fieldir+++++++++++++++++++++="+smburl+"//"+dir);
		//File fp = new File("Z://"+dir);  
        // 目录已存在创建文件夹  
        if (fp.exists() && fp.isDirectory()) {  
            
        } else{
        	// 目录不存在的情况下，会抛出异常  
            fp.mkdir();  
        }
	}
	
	public static void copyDir(String fileName, String target) throws Exception{
		InputStream in = null; 
		OutputStream out = null; 
		try{
			File fp = new File(fileName);  
	
			SmbFile remoteFile = new SmbFile(smburl+"//"+fp.getName()); 
			System.out.println("remoteFile+++++++++++++++++++++="+remoteFile);
			in = new BufferedInputStream(new FileInputStream(fp)); 
			out = new BufferedOutputStream(new SmbFileOutputStream(remoteFile)); 
			byte[] buffer = new byte[1024]; 
			int len;  
            while ((len = in.read(buffer)) != -1) {  
            	out.write(buffer, 0, len);  
            }  
            // 刷新此缓冲的输出流  
			out.flush();
		}catch (Exception e) { 
			e.printStackTrace(); 
			} finally { 
			try { 
			out.close(); 
			in.close(); 
			} catch (IOException e) { 
			e.printStackTrace(); 
			} 
			} 
	}
	
	 public static void fileUpload(String fileName, String newFileName, String filePath)
	  {
		 InputStream in = null; 
		OutputStream out = null; 
		 try{
	        String newname = "";
	        fileName = StringUtils.replace(fileName, "\\", "/");
	        if (fileName.indexOf("/") > -1)
	          newname = fileName.substring(fileName.lastIndexOf("/") + 1);
	        else {
	          newname = fileName;
	        }
	        SmbFileOutputStream file_out = null;
	        if ((newFileName != null) && (!(newFileName.equals("")))){
	        	file_out = new SmbFileOutputStream(smburl+"//"+filePath+"//"+newFileName);
	        	System.out.println("filename+++++++++++++++++++++="+smburl+"//"+filePath+"//"+newFileName);
	        }else {
	        	file_out = new SmbFileOutputStream(smburl+"//"+filePath+"//"+newname);
	        	System.out.println("filename+++++++++++++++++++++="+smburl+"//"+filePath+"//"+newFileName);
	        }
	        
	        File file_in = new File(fileName);
		       
			
			
				in = new BufferedInputStream(new FileInputStream(file_in)); 
				out = new BufferedOutputStream(file_out); 
				byte[] buffer = new byte[1024]; 
				int len;  
	            while ((len = in.read(buffer)) != -1) {  
	            	out.write(buffer, 0, len);  
	            }  
	            // 刷新此缓冲的输出流  
				out.flush();
				out.flush();
		}catch (Exception e) { 
			e.printStackTrace(); 
			} finally { 
			try { 
			out.close(); 
			in.close(); 
			} catch (IOException e) { 
			e.printStackTrace(); 
			} 
			} 
			
	  }
```
