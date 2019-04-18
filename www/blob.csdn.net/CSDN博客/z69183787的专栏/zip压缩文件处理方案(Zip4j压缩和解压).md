# zip压缩文件处理方案(Zip4j压缩和解压) - z69183787的专栏 - CSDN博客
2019年01月25日 11:34:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：91
个人分类：[工具-Zip解压压缩](https://blog.csdn.net/z69183787/article/category/2485649)
[https://blog.csdn.net/ditto_zhou/article/details/82977395](https://blog.csdn.net/ditto_zhou/article/details/82977395)
**主要特性**
- Create, Add, Extract, Update, Remove files from a Zip file针对ZIP压缩文件创建、添加、抽出、更新和移除文件
- Read/Write password protected Zip files(读写有密码保护的Zip文件)
- Supports AES 128/256 Encryption(支持AES 128/256算法加密)
- Supports Standard Zip Encryption(支持标准Zip算法加密)
- Supports Zip64 format(支持zip64格式)
- Supports Store (No Compression) and Deflate compression method(支持Store(非压缩)和Deflate压缩方法---不太明白)
- Create or extract files from Split Zip files (Ex: z01, z02,...zip)(针对分块zip文件创建和抽出文件)
- Supports Unicode file names(支持Unicode编码文件名)Progress Monitor(进度监控)
**使用**
zip4j默认采用utf-8编码，支持中文，也支持密码和多种压缩算法，可以说是功能强大，只是简单的解压文件，按照下面几步
```
public static void upzip(File zipFile,String dest,String password) {
		try {
			ZipFile zFile = new ZipFile(zipFile);//指向压缩文件.zip
			//设置文件名编码,在gbk系统中需要设置,文件名称是中文就不会乱码
                        zFile.setFileNameCharset("gbk");
			if(!zFile.isValidZipFile()) {// 检查文件是否合法
				throw new Exception("文件名不合法");
			}
			File destFile = new File(dest);//解压目录
			if(destFile.isDirectory()&&!destFile.exists()) {
				destFile.mkdirs();
			}
			if(zFile.isEncrypted()) {//如果设置了密码
				zFile.setPassword(password.toCharArray());
			}
			zFile.extractAll(dest);//将文件输出到目标目录
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
```
注意，如果解压的文件中文件名包含中文，一定要设置文件名字符集编码
`zFile.setFileNameCharset("gbk");`
对文件名编码字符集方法的调用一定要靠前，必须在isValidZipFile方法前调用，因为isValidZipFile方法的调用，会去设置文件名的编码
`zFile.isValidZipFile()``zipModel = headerReader.readAllHeaders(this.fileNameCharset);`
**压缩和解压**
```
public class CompressUtil {
	private static Logger logger = Logger.getLogger(CompressUtil.class);
	/**
	 * 解压文件
	 * 
	 * @param zipFile  压缩文件
	 * @param dest     解压文件的存放目录
	 * @param password 压缩文件的密码
	 */
	public static void unzip(File zipFile, String dest, String password) {
		try {
			ZipFile zFile = new ZipFile(zipFile);// 指向压缩文件.zip
			zFile.setFileNameCharset("gbk");// 设置文件名编码,在gbk系统中需要设置
			if (!zFile.isValidZipFile()) {// 检查文件是否合法
				throw new Exception("文件名不合法");
			}
			File destFile = new File(dest);// 解压目录
			if (destFile.isDirectory() && !destFile.exists()) {
				destFile.mkdirs();
			}
			if (zFile.isEncrypted()) {// 如果设置了密码
				zFile.setPassword(password.toCharArray());
			}
			zFile.extractAll(dest);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/**
	 * 返回的是解压后的文件数组
	 * 
	 * @param zipFile 压缩文件路径
	 * @param desc    解压后文件的存放路径
	 * @param passwd  密码
	 * @param charset 文件名称编码
	 * @return
	 */
	public static File[] getFilesByUnzip(File zipFile, String dest, String passwd, String charset) {
		try {
			ZipFile zFile = new ZipFile(zipFile);
			zFile.setFileNameCharset(charset);
			if (!zFile.isValidZipFile()) {
				throw new Exception("文件名称不合法");
			}
			File destFile = new File(dest);
			if (destFile.isDirectory() && !destFile.exists()) {
				destFile.mkdirs();
			}
			if (zFile.isEncrypted()) {
				zFile.setPassword(passwd);
			}
			zFile.extractAll(dest);
			// 获取文件列表
			List fileHeaders = zFile.getFileHeaders();
			List<File> extractFiles = new ArrayList<>();
			if (fileHeaders != null && fileHeaders.size() > 0) {
				for (int i = 0; i < fileHeaders.size(); i++) {
					FileHeader header = (FileHeader) fileHeaders.get(i);
					if (header != null) {
						extractFiles.add(new File(destFile, header.getFileName()));
					}
				}
			}
			File[] arr = new File[extractFiles.size()];
			return extractFiles.toArray(arr);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	/**
	 * 使用给定密码压缩指定文件或文件夹到指定位置. dest可传最终压缩文件存放的绝对路径,也可以传存放目录,也可以传null或者"".
	 * 如果传null或者""则将压缩文件存放在当前目录,即跟源文件同目录,压缩文件名取源文件名,以.zip为后缀;
	 * 如果以路径分隔符(File.separator)结尾,则视为目录,压缩文件名取源文件名,以.zip为后缀,否则视为文件名.
	 * @param src         要压缩的文件或文件夹路径
	 * @param dest        压缩文件存放路径
	 * @param isCreateDir 是否在压缩文件里创建目录,仅在压缩文件为目录时有效. 如果为false,将直接压缩目录下文件到压缩文件.
	 * @param passwd      压缩使用的密码
	 * @return 最终的压缩文件存放的绝对路径,如果为null则说明压缩失败.
	 */
	public static String zip(String src, String dest, boolean isCreateDir, String passwd) {
		File srcFile = new File(src);
		dest = buildDestinationZipFilePath(srcFile, dest);//获取压缩文件的存放路径 
		ZipParameters zipParameters = new ZipParameters();
		zipParameters.setCompressionMethod(Zip4jConstants.COMP_DEFLATE);//设置压缩方法是deflate
		zipParameters.setCompressionLevel(Zip4jConstants.DEFLATE_LEVEL_NORMAL);//设置压缩级别
		if(passwd!=null&&passwd.trim().length()>0) {
			// 如果密码不为空，压缩包进行加密,需要设置压缩算法
			zipParameters.setEncryptFiles(true);
			zipParameters.setEncryptionMethod(Zip4jConstants.ENC_METHOD_STANDARD);//加密算法设置为standard
			zipParameters.setPassword(passwd.toCharArray());
		}
		try {
			ZipFile zipFile = new ZipFile(dest);
			if(srcFile.isDirectory()) {
				if(!isCreateDir) {//如果false，表示不按照目录结构进行压缩
					ArrayList<File> list = new ArrayList<>();
					Collections.addAll(list, srcFile.listFiles());
					zipFile.addFiles(list, zipParameters);
				}else {
					//按照目录结构压缩
					zipFile.addFolder(srcFile, zipParameters);
				}
			}else {
				zipFile.addFile(srcFile, zipParameters);
			}
		} catch (ZipException e) {
			logger.error("-----文件压缩失败-----");
			logger.error(e.getMessage());
			dest = null;
		}
		return dest;
	}
	
	/**
	 * 构建压缩文件存放路径,如果不存在将会创建 * 传入的可能是文件名或者目录,也可能不传,此方法用以转换最终压缩文件的存放路径 * 
	 * @param srcFile 源文件 
	 * @param destParam 压缩目标路径 * 
	 * @return 正确的压缩文件存放路径
	 */
	private static String buildDestinationZipFilePath(File srcFile, String destParam) {
		if (destParam == null || destParam.trim() == "") {
			if (srcFile.isDirectory()) {
				destParam = srcFile.getParent() + File.separator + srcFile.getName() + ".zip";
			} else {
				String fileName = srcFile.getName().substring(0, srcFile.getName().lastIndexOf("."));
				destParam = srcFile.getParent() + File.separator + fileName + ".zip";
			}
		} else {
			createDestDirectoryIfNecessary(destParam); // 在指定路径不存在的情况下将其创建出来
			if (destParam.endsWith(File.separator)) {
				String fileName = "";
				if (srcFile.isDirectory()) {
					fileName = srcFile.getName();
				} else {
					fileName = srcFile.getName().substring(0, srcFile.getName().lastIndexOf("."));
				}
				destParam += fileName + ".zip";
			}
		}
		return destParam;
	}
    /**
     * 创建压缩文件存放目录
     * @param destParam
     */
	private static void createDestDirectoryIfNecessary(String destParam) {
		File file = null;
		if(destParam.endsWith(File.separator)) {
			file = new File(destParam);
		}else {
			file = new File(destParam.substring(0, destParam.lastIndexOf(File.separator)));
		}
		if(file.exists()) {
			file.mkdirs();
		}
	}
}
```
**删除**
进行删除操作前， 也要设置文件名编码为gbk，因为如果是中文名称的文件，就会抛出could not find file header for file的错误
删除文件
根据文件名称或者FileHeader删除
```
public RemoveFileFromZipFile() {
		
		try {
			// Initiate ZipFile object with the path/name of the zip file.
			ZipFile zipFile = new ZipFile("d:\\1.zip");
			zipFile.setFileNameCharset("gbk");
			// Note: If this zip file is a split file then this method throws an exception as
			// Zip Format Specification does not allow updating split zip files
			
			// Please make sure that this zip file has more than one file to completely test
			// this example
			
			// Removal of a file from a zip file can be done in two ways:
			// 1. 直接根据文件名称删除
			zipFile.removeFile("新建文本文档.txt");
			
			// 2. 创建FileHeader，根据FileHeader删除
			if (zipFile.getFileHeaders() != null && zipFile.getFileHeaders().size() > 0) {
				zipFile.removeFile((FileHeader)zipFile.getFileHeaders().get(0));
			} else {
				System.out.println("This cannot be demonstrated as zip file does not have any files left");
			}
			
		} catch (ZipException e) {
			e.printStackTrace();
		}
```
删除目录
将目录下面的文件删除，在删除目录
```
/**
	 * 删除压缩文件中的文件或者目录
	 * @param file 压缩文件的路径
	 * @param removeFile 文件名或者目录
	 */
	public static void removeDirFromZip(String file,String removeFile) {
		try {
			ZipFile zipFile = new ZipFile(file);
			zipFile.setFileNameCharset("gbk");//设置文件名的编码格式
			if(!removeFile.endsWith("/")) {
				//删除文件的方式，第一种是根据文件的名称，第二种是根据文件的fileHeader进行删除
				//zipFile.removeFile(removeFile);
				FileHeader header = new FileHeader();
				header.setFileName(removeFile);
				zipFile.removeFile(header);
			}else {
				//如果是目录的话，需要先将目录下面的文件删除掉，在删除目录
				if(!removeFile.endsWith("/")) removeFile +="/";
				List headers = zipFile.getFileHeaders();
				List<FileHeader> removeFiles = new ArrayList<>();
				for (int i = 0; i < headers.size(); i++) {
					FileHeader subHeader =(FileHeader) headers.get(i);
					if(subHeader.getFileName().contains(removeFile)
							&&!subHeader.getFileName().equals(removeFile)) {
						//不能直接删除，会抛出java.lang.IndexOutOfBoundsException
//						zipFile.removeFile(subHeader);
						// 这里我们使用一个list记录下删除的subheader，在进行删除
						removeFiles.add(subHeader);
					}
				}
				for (int i = 0; i < removeFiles.size(); i++) {
					zipFile.removeFile(removeFiles.get(i));
				}
				//最后将目录删除
				if(zipFile.getFileHeader(removeFile)!=null) {
					zipFile.removeFile(removeFile);
				}
			}
		} catch (ZipException e) {
			e.printStackTrace();
		}
	}
```
参考博客：[https://blog.csdn.net/zhyh1986/article/details/7921376](https://blog.csdn.net/zhyh1986/article/details/7921376)
