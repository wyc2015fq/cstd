# java读取文件读取工具类-将文件到byte数组的几种方式 - 零度的博客专栏 - CSDN博客
2016年04月14日 15:38:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：2266
` `
```
package zmx.util;
import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileChannel.MapMode;
import java.util.Arrays;
import java.util.List;
/**
 * java实现读取文件到Byte数组
 * @author zhangwenchao
 *
 */
public class FileToByteArrayUtils {
	public static byte[] toByteArray0(String filePath) throws IOException {
		File file = new File(filePath);
		long fileSize = file.length();
		if (fileSize > Integer.MAX_VALUE) {
			System.out.println("file too big...");
			return null;
		}
		FileInputStream fi = new FileInputStream(file);
		byte[] buffer = new byte[(int) fileSize];
		int offset = 0;
		int numRead = 0;
		while (offset < buffer.length && (numRead = fi.read(buffer, offset, buffer.length - offset)) >= 0) {
			offset += numRead;
		}
		// 确保所有数据均被读取
		if (offset != buffer.length) {
		throw new IOException("Could not completely read file "
					+ file.getName());
		}
		fi.close();
		return buffer;
	}
	/**
	 * the traditional io way
	 * @param filename
	 * @return
	 * @throws IOException
	 */
	public static byte[] toByteArray1(String filename) throws IOException {
		File f = new File(filename);
		if (!f.exists()) {
			throw new FileNotFoundException(filename);
		}
		ByteArrayOutputStream bos = new ByteArrayOutputStream((int) f.length());
		BufferedInputStream in = null;
		try {
			in = new BufferedInputStream(new FileInputStream(f));
			int buf_size = 1024;
			byte[] buffer = new byte[buf_size];
			int len = 0;
			while (-1 != (len = in.read(buffer, 0, buf_size))) {
				bos.write(buffer, 0, len);
			}
			return bos.toByteArray();
		} catch (IOException e) {
			e.printStackTrace();
			throw e;
		} finally {
			try {
				in.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			bos.close();
		}
	}
	/**
	 * NIO way
	 * @param filename
	 * @return
	 * @throws IOException
	 */
	public static byte[] toByteArray2(String filename) throws IOException {
		File f = new File(filename);
		if (!f.exists()) {
			throw new FileNotFoundException(filename);
		}
		FileChannel channel = null;
		FileInputStream fs = null;
		try {
			fs = new FileInputStream(f);
			channel = fs.getChannel();
			ByteBuffer byteBuffer = ByteBuffer.allocate((int) channel.size());
			while ((channel.read(byteBuffer)) > 0) {
				// do nothing
				// System.out.println("reading");
			}
			return byteBuffer.array();
		} catch (IOException e) {
			e.printStackTrace();
			throw e;
		} finally {
			try {
				channel.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			try {
				fs.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	/**
	 * Mapped File way MappedByteBuffer 可以在处理大文件时，提升性能
	 * @param filename
	 * @return
	 * @throws IOException
	 */
	public static byte[] toByteArray3(String filename) throws IOException {
		FileChannel fc = null;
		try {
			fc = new RandomAccessFile(filename, "r").getChannel();
			MappedByteBuffer byteBuffer = fc.map(MapMode.READ_ONLY, 0,fc.size()).load();
			//System.out.println(byteBuffer.isLoaded());
			byte[] result = new byte[(int) fc.size()];
			if (byteBuffer.remaining() > 0) {
				// System.out.println("remain");
				byteBuffer.get(result, 0, byteBuffer.remaining());
			}
			return result;
		} catch (IOException e) {
			e.printStackTrace();
			throw e;
		} finally {
			try {
				fc.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	
	
	public static void main(String[] args) {
		
		try {
			byte[] dataArray0 = FileToByteArrayUtils.toByteArray0("D:\\projects\\testWeb\\src\\zmx\\util\\test.txt");
			
			for(int i=0;i<dataArray0.length;i++){
				System.out.print(dataArray0[i]);
			}
			System.out.println();
			
			byte[] dataArray1 = FileToByteArrayUtils.toByteArray1("D:\\projects\\testWeb\\src\\zmx\\util\\test.txt");
			for(int i=0;i<dataArray1.length;i++){
				System.out.print(dataArray1[i]);
			}
			System.out.println();
			
			byte[] dataArray2 = FileToByteArrayUtils.toByteArray2("D:\\projects\\testWeb\\src\\zmx\\util\\test.txt");
			for(int i=0;i<dataArray2.length;i++){
				System.out.print(dataArray2[i]);
			}
			System.out.println();
			
			byte[] dataArray3 = FileToByteArrayUtils.toByteArray3("D:\\projects\\testWeb\\src\\zmx\\util\\test.txt");
			for(int i=0;i<dataArray3.length;i++){
				System.out.print(dataArray3[i]);
			}
			System.out.println();
			
			
		
		    
		} catch (IOException e) {
			
			e.printStackTrace();
		}
		
	}
	
	
	
	
	
	
	
	
}
```
