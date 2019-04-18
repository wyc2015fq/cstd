# 使用apache ant jar包 进行zip 打包操作 （解决中文问题） - z69183787的专栏 - CSDN博客
2014年08月14日 11:07:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4324
#### **使用jar包：ant-1.7.0.jar**
```java
List<String[]> fileList = new ArrayList<String[]>();
		fileList.add(new String[]{"附件1.doc","http://10.1.48.101:8080/workflow/attach/downloadFile.action?fileId=5029497"});
		fileList.add(new String[]{"附件2.doc","http://10.1.48.101:8080/workflow/attach/downloadFile.action?fileId=5029496"});
		
		String zipName = "ZipExport.rar";
		//String zipName = "ZipExport.zip";
		try {
			int len = 0;
			byte[] buffers = new byte[1024];
			
			File file = new File("D:\\"+zipName);
			ByteArrayOutputStream bos = new ByteArrayOutputStream();
			ZipOutputStream zos = new ZipOutputStream(new FileOutputStream(file));
			try {
				//zos = new ZipOutputStream(bos);
				for (String[] att : fileList) {
					URL url = new URL(att[1]);
					URLConnection con = url.openConnection();
					InputStream is = con.getInputStream();
					ZipEntry entry = new ZipEntry(att[0]);
					zos.putNextEntry(entry);
					while ((len = is.read(buffers)) != -1) {
						zos.write(buffers, 0, len);
					}
					zos.setEncoding("gbk");//解决中文
					zos.closeEntry();
					is.close();
				}
				zos.flush();
				zos.close();
			} catch (Exception localException) {
		}
		}catch(Exception e){}
```
