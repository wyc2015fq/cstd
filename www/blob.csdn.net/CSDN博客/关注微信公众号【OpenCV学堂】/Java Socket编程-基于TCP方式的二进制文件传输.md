# Java Socket编程 - 基于TCP方式的二进制文件传输 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年12月02日 13:02:26[gloomyfish](https://me.csdn.net/jia20003)阅读数：17430








一个基于Java Socket协议之上文件传输的完整示例，基于TCP通信完成。

除了基于TCP的二进制文件传输，还演示了JAVA Swing的一些编程技巧，Demo程序

实现主要功能有以下几点：

1.      基于Java Socket的二进制文件传输(包括图片，二进制文件，各种文档work，

         PDF)

2.      SwingWorker集合JProgressBar显示实时传输/接受完成的百分比

3.      其它一些Swing多线程编程技巧

首先来看一下整个Dome的Class之间的关系图：

![](https://img-my.csdn.net/uploads/201212/02/1354422899_5567.png)


下面按照上图来详细解释各个类的功能与代码实现：

服务器端：

FileTransferServer类的功能首先是在端口9999创建一个服务器套接字并

开始监听连接。相关代码如下：



```java
private void startServer(int port) {
	try {
		serverSocket = new ServerSocket(port);
		System.out.println("Server started at port :" + port);
		while(true) {
			Socket client = serverSocket.accept(); // blocked & waiting for income socket
			System.out.println("Just connected to " + client.getRemoteSocketAddress());
			FileReceiveTask task = new FileReceiveTask(client);
			bar.setValue(0); // reset it now
			task.addPropertyChangeListener(new PropertyChangeListener() {
	            public void propertyChange(PropertyChangeEvent evt) {
	                if ("progress".equals(evt.getPropertyName())) {
	                	bar.setValue((Integer) evt.getNewValue());
	                }
	            }
	        });
			
			task.execute();
		}

	} catch (IOException e) {
		e.printStackTrace();
	}
}
```
关于PropertyChangeListener, Java提供了一个非常有力的工具类来


监控任意Bean Model的数据改变，程序通过添加该监听器实现对

SwingWorker的progress属性值改变的事件捕获，然后更新JProgressBar

实例对象，实现了UI的刷新。FileTransferServer类的完整源代码如下：



```java
package com.gloomyfish.socket.tutorial.filetransfer;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;

public class FileTransferServer extends JFrame implements ActionListener {
	/**
	 * 
	 */
	public final static String START_SVR = "Start";
	public final static String SHUT_DOWN_SVR = "Shut Down";
	public final static String END_FLAG = "EOF";
	private static final long serialVersionUID = 1L;
	private ServerSocket serverSocket;
	private JButton startBtn;
	private JProgressBar bar;
	public FileTransferServer() {
		super("File Server");
		initComponent();
		setupListener();
	}

	private void setupListener() {
		startBtn.addActionListener(this);
	}

	private void initComponent() {
		startBtn = new JButton(START_SVR);
		JPanel progressPanel = new JPanel();
		progressPanel.setLayout(new BoxLayout(progressPanel, BoxLayout.Y_AXIS));
		bar = new JProgressBar();
		bar.setMinimum(0);
		bar.setMaximum(100);
		progressPanel.add(bar);
		getContentPane().setLayout(new BorderLayout());
		JPanel btnPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		btnPanel.add(startBtn);
		getContentPane().add(btnPanel, BorderLayout.SOUTH);
		getContentPane().add(progressPanel, BorderLayout.CENTER);
	}
	
	private void startServer(int port) {
		try {
			serverSocket = new ServerSocket(port);
			System.out.println("Server started at port :" + port);
			while(true) {
				Socket client = serverSocket.accept(); // blocked & waiting for income socket
				System.out.println("Just connected to " + client.getRemoteSocketAddress());
				FileReceiveTask task = new FileReceiveTask(client);
				bar.setValue(0); // reset it now
				task.addPropertyChangeListener(new PropertyChangeListener() {
		            public void propertyChange(PropertyChangeEvent evt) {
		                if ("progress".equals(evt.getPropertyName())) {
		                	bar.setValue((Integer) evt.getNewValue());
		                }
		            }
		        });
				
				task.execute();
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void showSuccess() {
		bar.setValue(100);
		JOptionPane.showMessageDialog(this, "file received successfully!");
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(START_SVR.equals(e.getActionCommand())) {
			Thread startThread = new Thread(new Runnable() {
                public void run() {
                	startServer(9999);
                }
            });
			startThread.start();
			startBtn.setEnabled(false);
		} else if(SHUT_DOWN_SVR.equals(e.getActionCommand())) {

		} else {
			// do nothing...
		}
	}
	
	public static void main(String[] args) {
		FileTransferServer server = new FileTransferServer();
		server.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		server.setSize(400, 400);
		server.setResizable(false);
		server.setVisible(true);
	}
}
```
**FileReceiveTask是服务器端的文件接受类：**


首先从建立的TCP流中得到文件名与文件大小，然后开始接受文件内容字节

并写入创建的文件对象流中，最后验证文件大小与写入的字节流是否相等

最后发送一条消息到文件发送方，告诉对方文件传输完成，可以关闭TCP流。

该类的完整源代码如下：



```java
package com.gloomyfish.socket.tutorial.filetransfer;

import java.io.BufferedOutputStream;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;

import javax.swing.SwingWorker;

public class FileReceiveTask extends SwingWorker<Integer, Object> {
	private Socket _mSocket;
	public FileReceiveTask(Socket client) {
		this._mSocket = client;
	}

	@Override
	protected Integer doInBackground() throws Exception {
		// get file meta information
		DataInputStream input = new DataInputStream(_mSocket.getInputStream());
		String fileName = input.readUTF();
		int fileLength = (int)input.readLong(); // number of total bytes
		File file = new File("C:\\Users\\fish\\Downloads" + File.separator + fileName);
		BufferedOutputStream output = new BufferedOutputStream(new FileOutputStream(file));
		System.out.println("Received File Name = " + fileName);
		System.out.println("Received File size = " + fileLength/1024 + "KB");
		
		// start to receive the content of the file and write them
		byte[] content = new byte[2048];
		int offset = 0;
		int numReadBytes = 0;
		while(offset < fileLength && (numReadBytes = input.read(content)) > 0) {
			output.write(content, 0, numReadBytes);
			float precent = 100.0f * ((float)offset)/((float)fileLength);
			setProgress((int)precent);
			offset += numReadBytes;
		}
		System.out.println("numReadBytes = " + numReadBytes);
		if(offset < fileLength) {
			numReadBytes = input.read(content);
			System.out.println("numReadBytes = " + numReadBytes);
			System.out.println("File content error at server side");
		} else {
			System.out.println("File Receive Task has done correctly");
		}
		setProgress(100);
		
		// tell client to close the socket now, we already receive the file successfully!!
	    BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(_mSocket.getOutputStream()));
	    bufferedWriter.write("DONE\r\n");
	    bufferedWriter.flush();
	    
	    // close the file and socket
		output.close();
		_mSocket.close();
		return 100;
	}

}
```
**客户端：**


FileTransferClient是客户端UI类，用来实现到服务端的连接，然后选择

要传输的文件(图片，PDF，Word文档等各种二进制文件)。如果没有

输入服务器信息，会弹出提示要求输入。端口已经指定为：9999

【send File】按钮会打开文件选择框，用户选择要传输文件以后，创建

FileTransferTask线程，并开始执行文件传送。客户端UI代码如下：



```java
package com.gloomyfish.socket.tutorial.filetransfer;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.net.InetSocketAddress;
import java.net.SocketAddress;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JTextField;
/**
 * 我一般写英文注释，偶尔我也会写中文注释，只是觉得写英文
 * 注释跟代码比较统一，无他。
 * 
 * @Date 2012-11-30
 * @author fish
 *
 */
public class FileTransferClient extends JFrame implements ActionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public final static String SEND_CMD = "Send File";
	public final static int MINIMUM = 0;
	public final static int MAXIMUM = 100;
	// public final static String CONNECT_CMD = "Connect";
	private JButton sendFileBtn;
	private JTextField serverField;
	private JTextField portField;
	private JProgressBar bar;
	
	public FileTransferClient() {
		super("File Transfer Client");
		initComponents();
	}

	private void initComponents() {
		getContentPane().setLayout(new BorderLayout());
		JPanel progressPanel = new JPanel();
		progressPanel.setLayout(new BoxLayout(progressPanel, BoxLayout.Y_AXIS));
		bar = new JProgressBar();
		progressPanel.add(bar);
		bar.setMinimum(MINIMUM);
		bar.setMaximum(MAXIMUM);
		JPanel serverSettingPanel = new JPanel();
		serverSettingPanel.setLayout(new GridLayout(2,2,5,5));
		serverSettingPanel.setBorder(BorderFactory.createTitledBorder("Server Setting"));
		serverField = new JTextField();
		portField = new JTextField();
		serverSettingPanel.add(new JLabel("Server IP/Host:"));
		serverSettingPanel.add(serverField);
		serverSettingPanel.add(new JLabel("Server Port:"));
		serverSettingPanel.add(portField);
		
		sendFileBtn = new JButton(SEND_CMD);
		JPanel btnPanel = new JPanel();
		btnPanel.setLayout(new FlowLayout(FlowLayout.RIGHT));
		btnPanel.add(sendFileBtn);
		getContentPane().add(serverSettingPanel, BorderLayout.NORTH);
		getContentPane().add(btnPanel, BorderLayout.SOUTH);
		getContentPane().add(progressPanel, BorderLayout.CENTER);
		sendFileBtn.addActionListener(this);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		String command = e.getActionCommand();
		if(command.equals(SEND_CMD)) {
			if(checkNull()) {
				JOptionPane.showMessageDialog(this, "Please enter server host and port in order to set up the connection!");
				return;
			}
			JFileChooser chooser = new JFileChooser();
			int status = chooser.showOpenDialog(null);
			if (status == JFileChooser.APPROVE_OPTION) {
				File f = chooser.getSelectedFile();
				SocketAddress address = new InetSocketAddress(getServer(), getPort());
				FileTransferTask task = new FileTransferTask(f, address, this);
				bar.setValue(0);
				task.addPropertyChangeListener(new PropertyChangeListener() {
		            public void propertyChange(PropertyChangeEvent evt) {
		                if ("progress".equals(evt.getPropertyName())) {
		                	bar.setValue((Integer) evt.getNewValue());
		                }
		            }
		        });
				task.execute(); // 异步task执行
			}
		} else {
			// do nothing
		}
	}
	
	public void showSuccess() {
		bar.setValue(100);
		JOptionPane.showMessageDialog(this, "file send successfully!");
	}
	
	public String getServer() {
		return serverField.getText().trim();
	}
	
	public int getPort() {
		return Integer.parseInt(portField.getText().trim());
	}
	/**
	 * make sure the UI already have some correct input information here!!!
	 * @return
	 */
	private boolean checkNull() {
		String serverName = serverField.getText();
		String port = portField.getText();
		if(serverName == null || serverName.length() == 0 || port == null || port.length() == 0) {
			return true;
		}
		
		try {
			Integer.parseInt(port); // try to parse it as server port number , validation code.
		} catch(NumberFormatException ne) {
			ne.printStackTrace();
			return true;
		}
		return false;
	}
	
	public static void main(String[] args) {
		FileTransferClient client = new FileTransferClient();
		client.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		client.setSize(400, 400);
		client.setResizable(false);
		// client.pack();
		client.setVisible(true);
	}

}
```
FileTransferTask实现的功能主要有：


1. 发送文件meta信息到接受方(文件名与文件大小)

2. 读取文件内容字节写入Socket字节流中，发送到接受方

3. 从Socket字节流中读取对方接受完成通知信息，调用弹出文件传输成功信息

该类完全源代码如下：



```java
package com.gloomyfish.socket.tutorial.filetransfer;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.SocketAddress;

import javax.swing.SwingWorker;

public class FileTransferTask extends SwingWorker<Integer, Object> {
	private File selectedFile;
	private Socket mSocket;
	private SocketAddress address;
	private FileTransferClient parent;
	
	public FileTransferTask(File file, SocketAddress address, FileTransferClient owner /*, JProgressBar progress*/) {
		this.address = address;
		this.selectedFile = file;
		mSocket = new Socket();
		this.parent = owner;
	}
	
	@Override
	protected Integer doInBackground() throws Exception {
		// Get the size of the file
	    long length = selectedFile.length();
	    if (length > Integer.MAX_VALUE) {
	        throw new IOException("Could not completely read file " + selectedFile.getName() + " as it is too long (" + length + " bytes, max supported " + Integer.MAX_VALUE + ")");
	    }
	    
	    mSocket.connect(address);
		
	    // Create the byte array to hold the file data
	    mSocket.setSoLinger(true, 60);
	    DataOutputStream dout = new DataOutputStream(mSocket.getOutputStream());
	    // now we start to send the file meta info.
	    dout.writeUTF(selectedFile.getName());
	    dout.writeLong(length);
	    dout.flush();
	    // end comment
	    FileDataPackage pData = new FileDataPackage();
	    DataInputStream is = new DataInputStream(new FileInputStream(selectedFile));
	    byte[] bytes = new byte[2048];

		// Read in the bytes
		int offset = 0;
		int numRead = 0;
		int fsize = (int)length;
		while (offset < fsize && (numRead=is.read(bytes, 0, bytes.length)) >= 0) {
			pData.setData(bytes, numRead);
			dout.write(pData.getPackageData(), 0, pData.getPackageData().length);
			dout.flush();
			offset += numRead;
			float precent = 100.0f * ((float)offset)/((float)fsize);
			setProgress((int)precent);
		}
		System.out.println("total send bytes = " + offset);
		// Ensure all the bytes have been read in
		if (offset < fsize) {
			throw new IOException("Could not completely transfer file " + selectedFile.getName());
		}
		mSocket.shutdownOutput();
		
		// receive the file transfer successfully message from connection
		
		BufferedInputStream streamReader = new BufferedInputStream(mSocket.getInputStream());
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(streamReader));
		String doneMsg = bufferedReader.readLine();
		if("DONE".equals(doneMsg)) {
			parent.showSuccess();
		}
		// Close the file input stream 
		setProgress(100);
		// dout.close();
		mSocket.close();
		is.close();
		System.out.println("close it now......");
		return 100;
	}
}
```
数据包类如下，不解释！




```java
package com.gloomyfish.socket.tutorial.filetransfer;
/**
 * this is very simple file transfer protocol over TCP socket
 * @date 2012-12-01
 * @author zhigang jia
 *
 */
public class FileDataPackage {

	private int dataLength; // 数据包中数据长度，两个字节
	private byte[] databuff; // 数据包中数据，meici最大不超过2048字节
	
	public final static byte[] EOF = new byte[]{'E', 'O','F'};
	
	public FileDataPackage() {
		dataLength = 0;
		databuff = new byte[2048];
	}
	
	public byte[] getPackageData() {
		byte[] pData = new byte[dataLength];
		// end comment
		System.arraycopy(databuff, 0, pData, 0, dataLength);
		return pData;
	}
	
	public void setData(byte[] data, int bsize) {
		dataLength = bsize;
		for(int i=0; i<databuff.length; i++) {
			if(i<bsize) {
				databuff[i] = data[i];
			} else {
				databuff[i] = ' ';
			}
		}
	}
}
```
每次发送的最大字节数为2048个字节。程序最终运行效果如下(win7 + JDK6u30)：


![](https://img-my.csdn.net/uploads/201212/02/1354424751_1818.png)


![](https://img-my.csdn.net/uploads/201212/02/1354424765_5301.png)


**觉得不错，请顶一下啊，谢谢！**

**转载务必注明**



