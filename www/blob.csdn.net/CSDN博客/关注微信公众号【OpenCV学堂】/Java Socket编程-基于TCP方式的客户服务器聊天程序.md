# Java Socket编程 - 基于TCP方式的客户服务器聊天程序 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年11月18日 01:14:10[gloomyfish](https://me.csdn.net/jia20003)阅读数：13928








本文讲演示如何通过Java Socket建立C/S方式的聊天程序。实现的功能

主要包括如下几个方面：

1. 用户登录，在线用户列表刷新

2.客户端与服务器的TCP连接，实现消息的发送与接受

3.Java Swing与多线程编程技巧

一个整体的Class关系图如下：

![](https://img-my.csdn.net/uploads/201211/18/1353172677_6048.png)


程序实现的服务器端UI如下：

![](https://img-my.csdn.net/uploads/201211/18/1353171124_7766.png)




一个JList组件用来显示在线的所有用户，一个JTextArea组件用来显示所有消息

记录。所有消息必须通过服务器端转发。点击【start】按钮启动服务器端监听

默认监听端口为9999。

**启动服务器端的Action中的代码如下：**



```java
Thread startThread = new Thread(new Runnable() {
                public void run() {
                	startServer(9999);
                }
            });
	startThread.start();
	startBtn.setEnabled(false);
	shutDownBtn.setEnabled(true);
```
**startServer（）的代码如下：**




```java
private void startServer(int port) {
	try {
		serverSocket = new ServerSocket(port);
		System.out.println("Server started at port :" + port);
		while(true) {
			Socket client = serverSocket.accept(); // blocked & waiting for income socket
			System.out.println("Just connected to " + client.getRemoteSocketAddress());
			DataInputStream bufferedReader = new DataInputStream(client.getInputStream());
			byte[] cbuff = new byte[256];
			int size = bufferedReader.read(cbuff);
			char[] charBuff = convertByteToChar(cbuff, size);
			String userName = String.valueOf(charBuff);
			ChatServerClientThread clentThread = new ChatServerClientThread(userName, client, this);
			clientList.add(clentThread);
			userNameList.add(userName);
			clentThread.start();
			updateUserList();
		}

	} catch (IOException e) {
		e.printStackTrace();
	}
}
```
简单协议规则：




1.      任何消息发送完以后系统自动加上结束标志EOF

2.      接受到用户消息以后通过解析EOF来完成消息传递

3.      自动发送更新用户列表到所有客户端当有新客户登入时
为什么我要实现上述简单协议，其实任何网络通信都是基于协议实现
只有基于协议实现才可控制，可检查。协议是网络通信的最重要一环。

客户端UI设计如下：

![](https://img-my.csdn.net/uploads/201211/18/1353171835_2114.png)

一个自定义的JPanel实现背景渐进颜色填充。



- Message组件用来接受用户输入的聊天信息

- Friend List 会自动刷新用户列表，当有新用户登录时候

- History Record用来显示聊天记录
- 【Connect】点击连接到Server端，前提是必须填写设置中全部，默认
    的机器IP为127.0.0.1端口为9999

- 【send】按钮点击会发送用户输入的消息到指定的其它客户端。如果

    没有选择用户，则发送到服务器端。

-  一次发送消息的大小不得大于200个字节。

完整的客户端代码如下：



```java
package com.gloomyfish.socket.tutorial.chat;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import com.gloomyfish.custom.swing.ui.CurvedGradientPanel;

public class ChatClient extends JFrame implements ActionListener {
	public final static String CONNECT_CMD = "Connect";
	public final static String DISCONNECT_CMD = "Disconnect";
	public final static String SEND_CMD = "Send";
	public final static String END_FLAG = "EOF";

	/**
	 * 
	 */
	private static final long serialVersionUID = 5837742337463099673L;
	private String winTitle;
	private JLabel userLabel;
	private JLabel passwordLabel;
	private JLabel ipLabel;
	private JLabel portLabel;
	
	// text field
	private JTextField userField;
	private JPasswordField passwordField;
	private JTextField ipField;
	private JTextField portField;
	
	private JList friendList;
	private JTextArea historyRecordArea;
	private JTextArea chatContentArea;
	
	// buttons
	private JButton connectBtn;
	private JButton disConnectBtn;
	private JButton sendBtn;
	private JCheckBox send2AllBtn;
	
	// socket
	private Socket mSocket;
	private SocketAddress address;
	private ChatClientThread m_client;
	
	public ChatClient() {
		super("Chat Client");
		initComponents();
		setupListener();
	}
	
	private void initComponents() {
		JPanel settingsPanel = new CurvedGradientPanel();
		JPanel chatPanel = new CurvedGradientPanel();
		GridLayout gy = new GridLayout(1,2,10,2);
		getContentPane().setLayout(gy);
		getContentPane().add(settingsPanel);
		getContentPane().add(chatPanel);
		
		// set up settings info
		settingsPanel.setLayout(new BorderLayout());
		settingsPanel.setOpaque(false);
		JPanel gridPanel = new JPanel(new GridLayout(4, 2));
		gridPanel.setBorder(BorderFactory.createTitledBorder("Server Settings & User Info"));
		gridPanel.setOpaque(false);
		userLabel = new JLabel("User Name:");
		passwordLabel = new JLabel("User Password:");
		ipLabel = new JLabel("Server IP Address:");
		portLabel = new JLabel("Server Port");
		userLabel.setOpaque(false);
		passwordLabel.setOpaque(false);
		ipLabel.setOpaque(false);
		portLabel.setOpaque(false);
		userField = new JTextField();
		passwordField = new JPasswordField();
		ipField = new JTextField();
		portField = new JTextField();
		connectBtn = new JButton(CONNECT_CMD);
		disConnectBtn = new JButton(DISCONNECT_CMD);
		JPanel btnPanel = new JPanel();
		btnPanel.setOpaque(false);
		btnPanel.setLayout(new FlowLayout());
		btnPanel.add(connectBtn);
		btnPanel.add(disConnectBtn);
		
		gridPanel.add(userLabel);
		gridPanel.add(userField);
		gridPanel.add(passwordLabel);
		gridPanel.add(passwordField);
		gridPanel.add(ipLabel);
		gridPanel.add(ipField);
		gridPanel.add(portLabel);
		gridPanel.add(portField);
		friendList = new JList();
		JScrollPane friendPanel = new JScrollPane(friendList);
		friendPanel.setOpaque(false);
		friendPanel.setBorder(BorderFactory.createTitledBorder("Friend List:"));
		settingsPanel.add(btnPanel, BorderLayout.SOUTH);
		settingsPanel.add(gridPanel, BorderLayout.NORTH);
		settingsPanel.add(friendPanel,BorderLayout.CENTER);
		
		chatPanel.setLayout(new GridLayout(3,1));
		chatPanel.setOpaque(false);
		historyRecordArea = new JTextArea();
		JScrollPane histroyPanel = new JScrollPane(historyRecordArea);
		histroyPanel.setBorder(BorderFactory.createTitledBorder("Chat History Record:"));
		histroyPanel.setOpaque(false);
		chatContentArea = new JTextArea();
		JScrollPane messagePanel = new JScrollPane(chatContentArea);
		messagePanel.setBorder(BorderFactory.createTitledBorder("Message:"));
		messagePanel.setOpaque(false);
		// chatPanel.add(friendPanel);
		chatPanel.add(histroyPanel);
		chatPanel.add(messagePanel);
		sendBtn = new JButton(SEND_CMD);
		send2AllBtn = new JCheckBox("Send to All online Users");
		send2AllBtn.setOpaque(false);
		JPanel sendbtnPanel = new JPanel();
		sendbtnPanel.setOpaque(false);
		sendbtnPanel.setLayout(new FlowLayout());
		sendbtnPanel.add(sendBtn);
		sendbtnPanel.add(send2AllBtn);
		chatPanel.add(sendbtnPanel);
	}
	
	private void setupListener() {
		connectBtn.addActionListener(this);
		disConnectBtn.addActionListener(this);
		sendBtn.addActionListener(this);
		disConnectBtn.setEnabled(false);
	}
	
	/**
	 * <p></p>
	 * 
	 * @param content - byte array
	 * @param bsize - the size of bytes
	 */
	public synchronized void handleMessage(char[] content, int bsize) {
		// char[] inputMessage = convertByteToChar(content, bsize);
		String receivedContent = String.valueOf(content);
		int endFlag = receivedContent.indexOf(END_FLAG);
		receivedContent = receivedContent.substring(0, endFlag);
		System.out.println("Client " + userField.getText() + " Message:" + receivedContent);
		if(receivedContent.contains("#")) {
			String[] onlineUserList = receivedContent.split("#");
			friendList.setListData(onlineUserList);
		} else {
			// just append to chat history record...
			appendHistoryRecord(receivedContent + "\r\n");
		}
	}
	
	public synchronized void appendHistoryRecord(String record) {
		historyRecordArea.append(record);
	}
	
	private String getSelectedUser() {
		int index = friendList.getSelectedIndex();
		if(index >= 0) {
		String user = (String)friendList.getSelectedValue();
		return user;
		} else {
			return "Server";
		}
	}
	
//	private char[] convertByteToChar(byte[] cbuff, int size) {
//		char[] charBuff = new char[size];
//		for(int i=0; i<size; i++) {
//			charBuff[i] = (char)cbuff[i];
//		}
//		return charBuff;
//	}
	
	public void setTitle(String title) {
		winTitle = title;
		super.setTitle(winTitle);
	}
	
	public String getTitle() {
		return super.getTitle();
	}
	
	public static void main(String[] args) {
		ChatClient client = new ChatClient();
		client.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		client.pack();
		client.setVisible(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(SEND_CMD.equals(e.getActionCommand())) {
			String chatContent = chatContentArea.getText();
			if(checkNull(chatContent)) {
				JOptionPane.showMessageDialog(this, "Please enter the message at least 6 characters!");
				return;
			} else if(chatContent.getBytes().length > 200) {
				JOptionPane.showMessageDialog(this, "The length of the message must be less than 200 characters!");
				return;
			}
			try {
				m_client.dispatchMessage(getSelectedUser() + "#" + chatContent);
				m_client.dispatchMessage(END_FLAG);
				appendHistoryRecord("me :" + chatContent + "\r\n");
				chatContentArea.setText(""); // try to clear user enter......
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		} else if(DISCONNECT_CMD.equals(e.getActionCommand())) {
			enableSettingsUI(true);
		} else if(CONNECT_CMD.equals(e.getActionCommand())) {
			String serverHostName = ipField.getText();
			String portStr = portField.getText();
			String userName = userField.getText();
			char[] password = passwordField.getPassword();
			System.out.println("Password = " + password.length);
			if(checkNull(serverHostName) || checkNull(portStr) || checkNull(userName)) {
				JOptionPane.showMessageDialog(this, "Please enter user name, server host name, server port!");
				return;
			}
			setTitle("Chat Client-" + userName);
			address = new InetSocketAddress(serverHostName, Integer.parseInt(portStr));
			mSocket = new Socket();
			try {
				mSocket.connect(address);
				m_client = new ChatClientThread(this, mSocket);
				m_client.dispatchMessage(userName); // send user name
				// m_client.dispatchMessage(END_FLAG); // send end flag
				m_client.start();
				enableSettingsUI(false);
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
			
		}
	}
	
	private void enableSettingsUI(boolean enable) {
		ipField.setEditable(enable);
		portField.setEnabled(enable);
		userField.setEditable(enable);
		passwordField.setEnabled(enable);
		connectBtn.setEnabled(enable);
		disConnectBtn.setEnabled(!enable);
	}
	
	private boolean checkNull(String inputString) {
		if(inputString ==  null || inputString.length() == 0) {
			return true;
		} else {
			return false;
		}
	}

}
```
客户端SOCKET通信线程的代码如下：




```java
package com.gloomyfish.socket.tutorial.chat;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class ChatClientThread extends Thread {
	private ChatClient _mClient;
	private Socket _mSocket;
	private DataOutputStream dos;
	
	public ChatClientThread(ChatClient cclient, Socket socket) {
		this._mClient = cclient;
		this._mSocket = socket;
	}
	
	public void run() {
		try {
			DataInputStream bufferedReader = new DataInputStream(_mSocket.getInputStream());
			byte[] cbuff = new byte[256];
			char[] tbuff = new char[256];
			int size = 0;
			int byteCount = 0;
			int length = 0;
			while(true) {
				if((size = bufferedReader.read(cbuff))> 0) {
					char[] temp = convertByteToChar(cbuff, size);
					length = temp.length;
					if((length + byteCount) > 256) {
						length = 256 - byteCount;
					}
					System.arraycopy(temp, 0, tbuff, byteCount, length);
					byteCount += size;
					if(String.valueOf(tbuff).indexOf(ChatClient.END_FLAG) > 0) {
						_mClient.handleMessage(tbuff, byteCount);
						byteCount = 0;
						clearTempBuffer(tbuff);
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private void clearTempBuffer(char[] tbuff) {
		for(int i=0; i<tbuff.length; i++) {
			tbuff[i] = ' ';
		}
	}
	
	private char[] convertByteToChar(byte[] cbuff, int size) {
		char[] charBuff = new char[size];
		for(int i=0; i<size; i++) {
			charBuff[i] = (char)cbuff[i];
		}
		return charBuff;
	}
	
	public synchronized void dispatchMessage(String textMsg) throws IOException {
		if(dos == null) {
			dos = new DataOutputStream(_mSocket.getOutputStream());
		}
		byte[] contentBytes = textMsg.getBytes();
		dos.write(contentBytes, 0, contentBytes.length);
	}
}
```
服务器端的消息转发代码如下：





```java
public synchronized void dispatchMessage(String[] keyValue, String userName) throws IOException {
		chatArea.append(userName + " to " + keyValue[0] + " : " + keyValue[1] + "\r\n");
		for(ChatServerClientThread client : clientList) {
			if(client.getUserName().equals(keyValue[0])) {
				client.dispatchMessage(userName + " says: " + keyValue[1]);
				client.dispatchMessage(END_FLAG);
				break;
			}
		}
	}
```
服务器端的客户端线程run方法的代码如下：




```java
public void run() {
	System.out.println("start user = " + userName);
	try {
		DataInputStream bufferedReader = new DataInputStream(userSocket.getInputStream());
		byte[] cbuff = new byte[256];
		char[] tbuff = new char[256];
		int size = 0;
		int byteCount = 0;
		int length = 0;
		while(true) {
			if((size = bufferedReader.read(cbuff))> 0) {
				char[] temp = convertByteToChar(cbuff, size);
				length = temp.length;
				if((length + byteCount) > 256) {
					length = 256 - byteCount;
				}
				System.arraycopy(temp, 0, tbuff, byteCount, length);
				byteCount += size;
				if(String.valueOf(tbuff).indexOf(ChatServer.END_FLAG) > 0) {
					String receivedContent = String.valueOf(tbuff);
					int endFlag = receivedContent.indexOf(ChatServer.END_FLAG);
					receivedContent = receivedContent.substring(0, endFlag);
					String[] keyValue = receivedContent.split("#");
					if(keyValue.length > 1) {
						server.dispatchMessage(keyValue, userName);
					}
					byteCount = 0;
					clearTempBuffer(tbuff);
				}
			}
		}
	} catch (IOException e) {
		e.printStackTrace();
	}
}
```
**最终程序的运行结果截屏如下：**


![](https://img-my.csdn.net/uploads/201211/18/1353172575_7105.png)

![](https://img-my.csdn.net/uploads/201211/18/1353172606_9574.png)







