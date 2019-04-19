# AudioRecord||AudioTrack - 三少GG - CSDN博客
2012年04月19日 17:22:26[三少GG](https://me.csdn.net/scut1135)阅读数：4610标签：[header																[exception																[buffer																[byte																[button																[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
### android中AudioRecord采集音频的参数说明
|audioSource|sampleRateInHz|channelConfig|audioFormat|bufferSizeInBytes|
|----|----|----|----|----|
|the recording source. See MediaRecorder.AudioSource for recording source definitions.音频源：指的是从哪里采集音频。这里我们当然是从麦克风采集音频，所以此参数的值为MIC| | | | |
|the sample rate expressed in Hertz. Examples of rates are (but not limited to) 44100, 22050 and 11025.采样率：音频的采样频率，每秒钟能够采样的次数，采样率越高，音质越高。给出的实例是44100、22050、11025但不限于这几个参数。例如要采集低质量的音频就可以使用4000、8000等低采样率。| | | | |
|describes the configuration of the audio channels. SeeCHANNEL_IN_MONO and CHANNEL_IN_STEREO声道设置：android支持双声道立体声和单声道。MONO单声道，STEREO立体声| | | | |
|the format in which the audio data is represented. SeeENCODING_PCM_16BIT and ENCODING_PCM_8BIT编码制式和采样大小：采集来的数据当然使用PCM编码(脉冲代码调制编码，即PCM编码。PCM通过抽样、量化、编码三个步骤将连续变化的模拟信号转换为数字编码。) android支持的采样大小16bit 或者8bit。当然采样大小越大，那么信息量越多，音质也越高，现在主流的采样大小都是16bit，在低质量的语音传输的时候8bit 足够了。| | | | |
|the total size (in bytes) of the buffer where audio data is written to during the recording. New audio data can be read from this buffer in smaller chunks than this size. See getMinBufferSize(int, int, int) to determine the minimum required buffer size for the successful creation of an AudioRecord instance. Using values smaller than getMinBufferSize() will result in an initialization failure.采集数据需要的缓冲区的大小，如果不知道最小需要的大小可以在getMinBufferSize()查看。| | | | |
采集到的数据保存在一个byteBuffer中，可以使用流将其读出。亦可保存成为文件的形式
### [Android 使用AudioRecord录音相关和音频文件的封装](http://blog.csdn.net/peijiangping1989/article/details/7042610)
分类： [Android流媒体学习](http://blog.csdn.net/peijiangping1989/article/category/939391)
在Android中录音可以用MediaRecord录音，操作比较简单。但是不够专业，就是不能对音频进行处理。如果要进行音频的实时的处理或者音频的一些封装
就可以用AudioRecord来进行录音了。
这里给出一段代码。实现了AudioRecord的录音和WAV格式音频的封装。
用AudioTrack和AudioTrack类可以进行边录边播，可以参考：[http://blog.sina.com.cn/s/blog_6309e1ed0100j1rw.html](http://blog.sina.com.cn/s/blog_6309e1ed0100j1rw.html)
我们这里的代码没有播放。但是有封装和详解，如下：
**[java]**[view
 plain](http://blog.csdn.net/peijiangping1989/article/details/7042610#)[copy](http://blog.csdn.net/peijiangping1989/article/details/7042610#)[print](http://blog.csdn.net/peijiangping1989/article/details/7042610#)[?](http://blog.csdn.net/peijiangping1989/article/details/7042610#)
- package com.ppmeet;  
- 
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.FileNotFoundException;  
- import java.io.FileOutputStream;  
- import java.io.IOException;  
- import android.app.Activity;  
- import android.graphics.PixelFormat;  
- import android.media.AudioFormat;  
- import android.media.AudioRecord;  
- import android.media.MediaRecorder;  
- import android.os.Bundle;  
- import android.view.View;  
- import android.view.View.OnClickListener;  
- import android.view.Window;  
- import android.view.WindowManager;  
- import android.widget.Button;  
- 
- /**
-  * class name：TestAudioRecord<BR>
-  * class description：用AudioRecord来进行录音<BR>
-  * PS： <BR>
-  * 
-  * @version 1.00 2011/09/21
-  * @author CODYY)peijiangping
-  */
- publicclass TestAudioRecord extends Activity {  
- // 音频获取源
- privateint audioSource = MediaRecorder.AudioSource.MIC;  
- // 设置音频采样率，44100是目前的标准，但是某些设备仍然支持22050，16000，11025
- privatestaticint sampleRateInHz = 44100;  
- // 设置音频的录制的声道CHANNEL_IN_STEREO为双声道，CHANNEL_CONFIGURATION_MONO为单声道
- privatestaticint channelConfig = AudioFormat.CHANNEL_IN_STEREO;  
- // 音频数据格式:PCM 16位每个样本。保证设备支持。PCM 8位每个样本。不一定能得到设备支持。
- privatestaticint audioFormat = AudioFormat.ENCODING_PCM_16BIT;  
- // 缓冲区字节大小
- privateint bufferSizeInBytes = 0;  
- private Button Start;  
- private Button Stop;  
- private AudioRecord audioRecord;  
- privateboolean isRecord = false;// 设置正在录制的状态
- //AudioName裸音频数据文件
- privatestaticfinal String AudioName = "/sdcard/love.raw";  
- //NewAudioName可播放的音频文件
- privatestaticfinal String NewAudioName = "/sdcard/new.wav";  
- 
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         getWindow().setFormat(PixelFormat.TRANSLUCENT);// 让界面横屏
-         requestWindowFeature(Window.FEATURE_NO_TITLE);// 去掉界面标题
-         getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,  
-                 WindowManager.LayoutParams.FLAG_FULLSCREEN);  
- // 重新设置界面大小
-         setContentView(R.layout.main);  
-         init();  
-     }  
- 
- privatevoid init() {  
-         Start = (Button) this.findViewById(R.id.start);  
-         Stop = (Button) this.findViewById(R.id.stop);  
-         Start.setOnClickListener(new TestAudioListener());  
-         Stop.setOnClickListener(new TestAudioListener());  
-         creatAudioRecord();  
-     }  
- 
- privatevoid creatAudioRecord() {  
- // 获得缓冲区字节大小
-         bufferSizeInBytes = AudioRecord.getMinBufferSize(sampleRateInHz,  
-                 channelConfig, audioFormat);  
- // 创建AudioRecord对象
-         audioRecord = new AudioRecord(audioSource, sampleRateInHz,  
-                 channelConfig, audioFormat, bufferSizeInBytes);  
-     }  
- 
- class TestAudioListener implements OnClickListener {  
- 
- @Override
- publicvoid onClick(View v) {  
- if (v == Start) {  
-                 startRecord();  
-             }  
- if (v == Stop) {  
-                 stopRecord();  
-             }  
- 
-         }  
- 
-     }  
- 
- privatevoid startRecord() {  
-         audioRecord.startRecording();  
- // 让录制状态为true
-         isRecord = true;  
- // 开启音频文件写入线程
- new Thread(new AudioRecordThread()).start();  
-     }  
- 
- privatevoid stopRecord() {  
-         close();  
-     }  
- 
- privatevoid close() {  
- if (audioRecord != null) {  
-             System.out.println("stopRecord");  
-             isRecord = false;//停止文件写入
-             audioRecord.stop();  
-             audioRecord.release();//释放资源
-             audioRecord = null;  
-         }  
-     }  
- 
- class AudioRecordThread implements Runnable {  
- @Override
- publicvoid run() {  
-             writeDateTOFile();//往文件中写入裸数据
-             copyWaveFile(AudioName, NewAudioName);//给裸数据加上头文件
-         }  
-     }  
- 
- /**
-      * 这里将数据写入文件，但是并不能播放，因为AudioRecord获得的音频是原始的裸音频，
-      * 如果需要播放就必须加入一些格式或者编码的头信息。但是这样的好处就是你可以对音频的 裸数据进行处理，比如你要做一个爱说话的TOM
-      * 猫在这里就进行音频的处理，然后重新封装 所以说这样得到的音频比较容易做一些音频的处理。
-      */
- privatevoid writeDateTOFile() {  
- // new一个byte数组用来存一些字节数据，大小为缓冲区大小
- byte[] audiodata = newbyte[bufferSizeInBytes];  
-         FileOutputStream fos = null;  
- int readsize = 0;  
- try {  
-             File file = new File(AudioName);  
- if (file.exists()) {  
-                 file.delete();  
-             }  
-             fos = new FileOutputStream(file);// 建立一个可存取字节的文件
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
- while (isRecord == true) {  
-             readsize = audioRecord.read(audiodata, 0, bufferSizeInBytes);  
- if (AudioRecord.ERROR_INVALID_OPERATION != readsize) {  
- try {  
-                     fos.write(audiodata);  
-                 } catch (IOException e) {  
-                     e.printStackTrace();  
-                 }  
-             }  
-         }  
- try {  
-             fos.close();// 关闭写入流
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- // 这里得到可播放的音频文件
- privatevoid copyWaveFile(String inFilename, String outFilename) {  
-         FileInputStream in = null;  
-         FileOutputStream out = null;  
- long totalAudioLen = 0;  
- long totalDataLen = totalAudioLen + 36;  
- long longSampleRate = sampleRateInHz;  
- int channels = 2;  
- long byteRate = 16 * sampleRateInHz * channels / 8;  
- byte[] data = newbyte[bufferSizeInBytes];  
- try {  
-             in = new FileInputStream(inFilename);  
-             out = new FileOutputStream(outFilename);  
-             totalAudioLen = in.getChannel().size();  
-             totalDataLen = totalAudioLen + 36;  
-             WriteWaveFileHeader(out, totalAudioLen, totalDataLen,  
-                     longSampleRate, channels, byteRate);  
- while (in.read(data) != -1) {  
-                 out.write(data);  
-             }  
-             in.close();  
-             out.close();  
-         } catch (FileNotFoundException e) {  
-             e.printStackTrace();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- /**
-      * 这里提供一个头信息。插入这些信息就可以得到可以播放的文件。
-      * 为我为啥插入这44个字节，这个还真没深入研究，不过你随便打开一个wav
-      * 音频的文件，可以发现前面的头文件可以说基本一样哦。每种格式的文件都有
-      * 自己特有的头文件。
-      */
- privatevoid WriteWaveFileHeader(FileOutputStream out, long totalAudioLen,  
- long totalDataLen, long longSampleRate, int channels, long byteRate)  
- throws IOException {  
- byte[] header = newbyte[44];  
-         header[0] = 'R'; // RIFF/WAVE header
-         header[1] = 'I';  
-         header[2] = 'F';  
-         header[3] = 'F';  
-         header[4] = (byte) (totalDataLen & 0xff);  
-         header[5] = (byte) ((totalDataLen >> 8) & 0xff);  
-         header[6] = (byte) ((totalDataLen >> 16) & 0xff);  
-         header[7] = (byte) ((totalDataLen >> 24) & 0xff);  
-         header[8] = 'W';  
-         header[9] = 'A';  
-         header[10] = 'V';  
-         header[11] = 'E';  
-         header[12] = 'f'; // 'fmt ' chunk
-         header[13] = 'm';  
-         header[14] = 't';  
-         header[15] = ' ';  
-         header[16] = 16; // 4 bytes: size of 'fmt ' chunk
-         header[17] = 0;  
-         header[18] = 0;  
-         header[19] = 0;  
-         header[20] = 1; // format = 1
-         header[21] = 0;  
-         header[22] = (byte) channels;  
-         header[23] = 0;  
-         header[24] = (byte) (longSampleRate & 0xff);  
-         header[25] = (byte) ((longSampleRate >> 8) & 0xff);  
-         header[26] = (byte) ((longSampleRate >> 16) & 0xff);  
-         header[27] = (byte) ((longSampleRate >> 24) & 0xff);  
-         header[28] = (byte) (byteRate & 0xff);  
-         header[29] = (byte) ((byteRate >> 8) & 0xff);  
-         header[30] = (byte) ((byteRate >> 16) & 0xff);  
-         header[31] = (byte) ((byteRate >> 24) & 0xff);  
-         header[32] = (byte) (2 * 16 / 8); // block align
-         header[33] = 0;  
-         header[34] = 16; // bits per sample
-         header[35] = 0;  
-         header[36] = 'd';  
-         header[37] = 'a';  
-         header[38] = 't';  
-         header[39] = 'a';  
-         header[40] = (byte) (totalAudioLen & 0xff);  
-         header[41] = (byte) ((totalAudioLen >> 8) & 0xff);  
-         header[42] = (byte) ((totalAudioLen >> 16) & 0xff);  
-         header[43] = (byte) ((totalAudioLen >> 24) & 0xff);  
-         out.write(header, 0, 44);  
-     }  
- 
- @Override
- protectedvoid onDestroy() {  
-         close();  
- super.onDestroy();  
-     }  
- }
  
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Android入门（9）AudioRecord和AudioTrack类的使用
(2010-05-07 09:07:05
 AudioRecord和AudioTrack类是Android获取和播放音频流的重要类，放置在android.media包中。与该包中的MediaRecorder和MediaPlayer类不同，AudioRecord和AudioTrack类在获取和播放音频数据流时无需通过文件保存和文件读取，可以动态地直接获取和播放音频流，在实时处理音频数据流时非常有用。
 当然，如果用户只想录音后写入文件或从文件中取得音频流进行播放，那么直接使用MediaRecorder和MediaPlayer类是首选方案，因为这两个类使用非常方便，而且成功率很高。而AudioRecord和AudioTrack类的使用却比较复杂，我们发现很多人都不能成功地使用这两个类，甚至认为Android的这两个类是不能工作的。
 其实，AudioRecord和AudioTrack类的使用虽然比较复杂，但是可以工作，我们不仅可以很好地使用了这两个类，而且还通过套接字（Socket）实现了音频数据的网络传输，做到了一端使用AudioRecord获取音频流然后通过套接字传输出去，而另一端通过套接字接收后使用AudioTrack类播放。
 下面是我们对AudioRecord和AudioTrack类在使用方面的经验总结：
 （1）创建AudioRecord和AudioTrack类对象：创建这两个类的对象比较复杂，通过对文档的反复和仔细理解，并通过多次失败的尝试，并在北理工的某个Android大牛的网上的文章启发下，我们也最终成功地创建了这两个类的对象。创建AudioRecord和AudioTrack类对象的代码如下：
AudioRecord类：
 m_in_buf_size =AudioRecord.getMinBufferSize(8000,
 AudioFormat.CHANNEL_CONFIGURATION_MONO,
 AudioFormat.ENCODING_PCM_16BIT);
 m_in_rec = new AudioRecord(MediaRecorder.AudioSource.MIC,
 8000,
 AudioFormat.CHANNEL_CONFIGURATION_MONO,
 AudioFormat.ENCODING_PCM_16BIT,
 m_in_buf_size) ;
AudioTrack类：
 m_out_buf_size = android.media.AudioTrack.getMinBufferSize(8000,
AudioFormat.CHANNEL_CONFIGURATION_MONO,
AudioFormat.ENCODING_PCM_16BIT);
 m_out_trk = new AudioTrack(AudioManager.STREAM_MUSIC, 8000,
 AudioFormat.CHANNEL_CONFIGURATION_MONO,
 AudioFormat.ENCODING_PCM_16BIT,
 m_out_buf_size,
 AudioTrack.MODE_STREAM);
（2）关于AudioRecord和AudioTrack类的监听函数，不用也行。
 （3）调试方面，包括初始化后看logcat信息，以确定类的工作状态，初始化是否成功等。
编写好代码，没有语法错误，调用模拟器运行、调试代码时，logcat发挥了很好的功用。刚调试时，经常会出现模拟器显示出现异常，这时我们可以在代码的一些关键语句后添加如Log.d("test1","OK")；这样的语句进行标识，出现异常时我们就可以在logcat窗口观察代码执行到哪里出现异常，然后进行相应的修改、调试。模拟器不会出现异常时，又遇到了录放音的问题。录音方面，刚开始选择将语音编码数据存放在多个固定大小的文件中进行传送，但是这种情况下会出现声音断续的现象，而且要反复的建立文件，比较麻烦，后来想到要进行网上传输，直接将语音编码数据以数据流的形式传送，经过验证，这种方法可行并且使代码更加简洁。放音方面，将接收到的数据流存放在一个数组中，然后将数组中数据写到AudioTrack中。刚开始只是“嘟”几声，经过检查发现只是把数据写一次，加入循环，让数据反复写到AudioTrack中，就可以听到正常的语音了。接下来的工作主要是改善话音质量与话音延迟，在进行通话的过程中，观察logcat窗口，发现向数组中写数据时会出现Bufferflow的情况，于是把重心转移到数组大小的影响上，经过试验，发现
 AudioRecord一次会读640个数据，然后就对录音和放音中有数组的地方进行实验修改。AudioRecord和AudioTrack进行实例化时，参数中各有一个数组大小，经过试验这个数组大小和AudioRecord和AudioTrack能正常实例化所需的最小Buffer大小（即上面实例化时的m_in_buf_size和m_out_buf_size参数）相等且服务器方进行缓存数据的数组尺寸是上述数值的2倍时，语音质量最好。由于录音和放音的速度不一致，受到北理工大牛的启发，在录音方面，将存放录音数据的数组放到LinkedList中,当LinkedList中数组个数达到2（这个也是经过试验验证话音质量最好时的数据）时，将先录好的数组中数据传送出去。经过上述反复试验和修改，最终使双方通话质量较好，且延时较短（大概有2秒钟）。
 （4）通过套接字传输和接收数据
 数据传送部分，使用的是套接字。通信双方，通过不同的端口向服务器发送请求，与服务器连接上后，开始通话向服务器发送数据，服务器通过一个套接字接收到一方的数据后，先存在一个数组中，然后将该数组中数据以数据流的形式再通过另一个套接字传送到另一方。这样就实现了双方数据的传送。
 （5）代码架构
 为避免反复录入和读取数据占用较多资源，使程序在进行录放音时不能执行其他命令，故将录音和放音各写成一个线程类，然后在主程序中，通过MENU控制通话的开始、停止、结束。
最后说明，AudioRecord和AudioTrack类可以用，只是稍微复杂些。以下贴出双方通信的源码，希望对大家有所帮助：
主程序Daudioclient：
package cn.Daudioclient;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
public class Daudioclient extends Activity {
public static final int MENU_START_ID = Menu.FIRST ;
 public static final int MENU_STOP_ID = Menu.FIRST + 1 ;
 public static final int MENU_EXIT_ID = Menu.FIRST + 2 ;
 protected Saudioserver  m_player ;
 protected Saudioclient  m_recorder ;
 @Override
 public void onCreate(Bundle savedInstanceState) {
 super.onCreate(savedInstanceState);
 setContentView(R.layout.main);
 }
 public boolean onCreateOptionsMenu(Menu aMenu)
 {
 boolean res = super.onCreateOptionsMenu(aMenu) ;
 aMenu.add(0, MENU_START_ID, 0, "START") ;
 aMenu.add(0, MENU_STOP_ID, 0, "STOP") ;
 aMenu.add(0, MENU_EXIT_ID, 0, "EXIT") ;
 return res ;
 }
 public boolean onOptionsItemSelected(MenuItem aMenuItem)
 {
 switch (aMenuItem.getItemId()) {
 case MENU_START_ID:
 {
m_player = new Saudioserver() ;
 m_recorder = new Saudioclient() ;
 m_player.init() ;
 m_recorder.init() ;
 m_recorder.start() ;
 m_player.start() ;
 }
 break ;
 case MENU_STOP_ID:
 { 
m_recorder.free() ;
 m_player.free() ;
 m_player = null ;
 m_recorder = null ;
 }
 break ;
 case MENU_EXIT_ID:
 {
 int pid = android.os.Process.myPid() ;
 android.os.Process.killProcess(pid) ;
 }
 break ;
 default:
 break ;
 }
 return super.onOptionsItemSelected(aMenuItem);
 }
}
录音程序Saudioclient：
package cn.Daudioclient;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.LinkedList;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;
public class Saudioclient extends Thread
{
 protected AudioRecord m_in_rec ; 
 protected int  m_in_buf_size ;
 protected byte []  m_in_bytes ;
 protected boolean  m_keep_running ;
 protected Socket  s;
 protected DataOutputStream dout;
 protected LinkedList<byte[]>  m_in_q ;
 public void run()
{
try
{
byte [] bytes_pkg ;
 m_in_rec.startRecording() ;
 while(m_keep_running)
 {
 m_in_rec.read(m_in_bytes, 0, m_in_buf_size) ;
 bytes_pkg = m_in_bytes.clone() ;
 if(m_in_q.size() >= 2)
 {
 dout.write(m_in_q.removeFirst() , 0, m_in_q.removeFirst() .length);
 }
 m_in_q.add(bytes_pkg) ;
 }
 m_in_rec.stop() ;
 m_in_rec = null ;
 m_in_bytes = null ;
dout.close();
}
catch(Exception e)
{
e.printStackTrace();
}
 }
 public void init()
 {
m_in_buf_size = AudioRecord.getMinBufferSize(8000,
 AudioFormat.CHANNEL_CONFIGURATION_MONO,
 AudioFormat.ENCODING_PCM_16BIT);
m_in_rec = new AudioRecord(MediaRecorder.AudioSource.MIC,
8000,
AudioFormat.CHANNEL_CONFIGURATION_MONO,
AudioFormat.ENCODING_PCM_16BIT,
m_in_buf_size) ;
m_in_bytes = new byte [m_in_buf_size] ;
m_keep_running = true ;
m_in_q=new LinkedList<byte[]>();
try
{
s=new Socket("192.168.1.100",4332);
dout=new DataOutputStream(s.getOutputStream());
//new Thread(R1).start();
}
catch (UnknownHostException e)
{
// TODO Auto-generated catch block
e.printStackTrace();
}
catch (IOException e)
{
// TODO Auto-generated catch block
e.printStackTrace();
}
 }
 public void free()
{
m_keep_running = false ;
 try {
 Thread.sleep(1000) ;
 } catch(Exception e) {
 Log.d("sleep exceptions...\n","") ;
 }
}
}
放音程序Saudioserver：
package cn.Daudioclient;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;
public class Saudioserver extends Thread
{ 
 protected AudioTrack m_out_trk ;
 protected int  m_out_buf_size ;
 protected byte []  m_out_bytes ;
 protected boolean  m_keep_running ;
private Socket s;
private DataInputStream din;
public void init()
{
try
{
 s=new Socket("192.168.1.100",4331);
 din=new DataInputStream(s.getInputStream());
 m_keep_running = true ;
 m_out_buf_size = AudioTrack.getMinBufferSize(8000,
AudioFormat.CHANNEL_CONFIGURATION_MONO,
 AudioFormat.ENCODING_PCM_16BIT);
 m_out_trk = new AudioTrack(AudioManager.STREAM_MUSIC, 8000,
 AudioFormat.CHANNEL_CONFIGURATION_MONO,
 AudioFormat.ENCODING_PCM_16BIT,
 m_out_buf_size,
 AudioTrack.MODE_STREAM);
 m_out_bytes=new byte[m_out_buf_size];
 // new Thread(R1).start();
}
catch(Exception e)
{
e.printStackTrace();
}
}
public void free()
{
m_keep_running = false ;
 try {
 Thread.sleep(1000) ;
 } catch(Exception e) {
 Log.d("sleep exceptions...\n","") ;
 }
}
 public void run()
 {
 byte [] bytes_pkg = null ;
 m_out_trk.play() ;
 while(m_keep_running) {
 try
 {
din.read(m_out_bytes);
 bytes_pkg = m_out_bytes.clone() ;
 m_out_trk.write(bytes_pkg, 0, bytes_pkg.length) ;
 }
 catch(Exception e)
 {
e.printStackTrace();
 }
 }
 m_out_trk.stop() ;
 m_out_trk = null ;
 try {
din.close();
} catch (IOException e) {
// TODO Auto-generated catch block
e.printStackTrace();
}
 }
}
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**[Android AudioRecord 实现音频录制](http://hi.baidu.com/%C1%F5%D0%A1%C0%F61984519/blog/item/7cba4c39b1a753dfd46225ce.html)**
//由于程序可能用到对手机SD卡的读取，所以在manifest.xml里需要配置对sd卡存取的权限：
// <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
// <uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS"/>
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import android.app.Activity;
import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import com.telenav.funnl.android.R;
/** 
* 该实例中，我们使用AudioRecord类来完成我们的音频录制程序 
* AudioRecord类，我们可以使用三种不同的read方法来完成录制工作， 
* 每种方法都有其实用的场合 
* 一、实例化一个AudioRecord类我们需要传入几种参数 
* 1、AudioSource：这里可以是MediaRecorder.AudioSource.MIC 
* 2、SampleRateInHz:录制频率，可以为8000hz或者11025hz等，不同的硬件设备这个值不同 
* 3、ChannelConfig:录制通道，可以为udioFormat.CHANNEL_CONFIGURATION_MONO和udioFormat.CHANNEL_CONFIGURATION_STEREO
* 4、AudioFormat:录制编码格式，可以为AudioFormat.ENCODING_16BIT和BIT,其中16BIT的仿真性比8BIT好，但是需要消耗更多的电量和存储空间
* 5、BufferSize:录制缓冲大小：可以通过getMinBufferSize来获取 
* 这样我们就可以实例化一个AudioRecord对象了 
* 二、创建一个文件，用于保存录制的内容 
* 同上篇 
* 三、打开一个输出流，指向创建的文件 
* DataOutputStream dos = new DataOutputStream(new BufferedOutputSream(new FileOutputStream(file)))
* 四、现在就可以开始录制了，我们需要创建一个字节数组来存储从udioRecorder中返回的音频数据，但是 
* 注意，我们定义的数组要小于定义AudioRecord时指定的那个ufferSize 
* short[]buffer = new short[BufferSize/4]; 
* startRecording(); 
* 然后一个循环，调用AudioRecord的read方法实现读取 
* 另外使用MediaPlayer是无法播放使用AudioRecord录制的音频的，为了实现播放，我们需要 
* 使用AudioTrack类来实现 
* AudioTrack类允许我们播放原始的音频数据 
* 
* 
* 一、实例化一个AudioTrack同样要传入几个参数 
* 1、StreamType:在AudioManager中有几个常量，其中一个是TREAM_MUSIC; 
* 2、SampleRateInHz：最好和AudioRecord使用的是同一个值 
* 3、ChannelConfig：同上 
* 4、AudioFormat：同上 
* 5、BufferSize：通过AudioTrack的静态方法getMinBufferSize来获取 
* 6、Mode：可以是AudioTrack.MODE_STREAM和MODE_STATIC，关于这两种不同之处，可以查阅文档 
* 二、打开一个输入流，指向刚刚录制内容保存的文件，然后开始播放，边读取边播放 
* 
* 实现时，音频的录制和播放分别使用两个AsyncTask来完成 
*/ 
public class PCMRecordActivity extends Activity implements OnClickListener{ 
private TextView stateView; 
private Button btnStart,btnStop,btnPlay,btnFinish; 
private RecordTask recorder; 
private PlayTask player; 
private File fpath; 
private File audioFile;
private Context context;
private boolean isRecording=true, isPlaying=false; //标记 
private int frequence = 16000;//8000; //录制频率，单位hz.这里的值注意了，写的不好，可能实例化AudioRecord对象的时候，会出错。我开始写成11025就不行。这取决于硬件设备
private int channelConfig = AudioFormat.CHANNEL_CONFIGURATION_MONO; 
private int audioEncoding = AudioFormat.ENCODING_PCM_16BIT; 
public void onCreate(Bundle savedInstanceState){ 
super.onCreate(savedInstanceState); 
setContentView(R.layout.my_audio_record); 
stateView = (TextView)this.findViewById(R.id.view_state); 
stateView.setText("start recording.."); 
btnStart = (Button)this.findViewById(R.id.btn_start); 
btnStart.setText("start");
btnStart.setOnClickListener(this);
btnStop = (Button)this.findViewById(R.id.btn_stop); 
btnStop.setText("stop");
btnStop.setOnClickListener(this);
btnPlay = (Button)this.findViewById(R.id.btn_play);
btnPlay.setText("play");
btnPlay.setOnClickListener(this);
btnFinish = (Button)this.findViewById(R.id.btn_finish); 
btnFinish.setText("finish");
btnFinish.setOnClickListener(this);
btnStop.setEnabled(false); 
btnPlay.setEnabled(false); 
btnFinish.setEnabled(false); 
if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))//手机有SD卡的情况
{
//在这里我们创建一个文件，用于保存录制内容 
fpath = new File(Environment.getExternalStorageDirectory().getAbsolutePath()+"/data/files/");
fpath.mkdirs();//创建文件夹 
}else//手机无SD卡的情况
{
fpath = this.getCacheDir();
}
try { 
//创建临时文件,注意这里的格式为.pcm 
audioFile = File.createTempFile("recording", ".pcm", fpath); 
} catch (IOException e) { 
// TODO Auto-generated catch block 
e.printStackTrace(); 
} 
} 
public void onClick(View v){ 
int id = v.getId(); 
switch(id){ 
case R.id.btn_start: 
//开始录制 
//这里启动录制任务 
recorder = new RecordTask(); 
recorder.execute(); 
break; 
case R.id.btn_stop: 
//停止录制 
this.isRecording = false; 
//更新状态 
//在录制完成时设置，在RecordTask的onPostExecute中完成 
break; 
case R.id.btn_play: 
player = new PlayTask(); 
player.execute(); 
break; 
case R.id.btn_finish: 
//完成播放 
this.isPlaying = false; 
break; 
} 
} 
class RecordTask extends AsyncTask<Void, Integer, Void>{ 
@Override 
protected Void doInBackground(Void... arg0) { 
isRecording = true; 
try { 
//开通输出流到指定的文件 
DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(audioFile)));
//根据定义好的几个配置，来获取合适的缓冲大小 
int bufferSize = AudioRecord.getMinBufferSize(frequence, channelConfig, audioEncoding);
//实例化AudioRecord 
AudioRecord record = new AudioRecord(MediaRecorder.AudioSource.MIC, frequence, channelConfig, audioEncoding, bufferSize);
//定义缓冲 
short[] buffer = new short[bufferSize]; 
//开始录制 
record.startRecording(); 
int r = 0; //存储录制进度 
//定义循环，根据isRecording的值来判断是否继续录制 
while(isRecording){ 
//从bufferSize中读取字节，返回读取的short个数 
//这里老是出现buffer overflow，不知道是什么原因，试了好几个值，都没用，TODO：待解决 
int bufferReadResult = record.read(buffer, 0, buffer.length); 
//循环将buffer中的音频数据写入到OutputStream中 
for(int i=0; i<bufferReadResult; i++){ 
dos.writeShort(buffer[i]); 
} 
publishProgress(new Integer(r)); //向UI线程报告当前进度 
r++; //自增进度值 
} 
//录制结束 
record.stop(); 
Log.v("The DOS available:", "::"+audioFile.length()); 
dos.close(); 
} catch (Exception e) { 
// TODO: handle exception 
} 
return null; 
} 
//当在上面方法中调用publishProgress时，该方法触发,该方法在I线程中被执行 
protected void onProgressUpdate(Integer...progress){ 
stateView.setText(progress[0].toString()); 
} 
protected void onPostExecute(Void result){ 
btnStop.setEnabled(false); 
btnStart.setEnabled(true); 
btnPlay.setEnabled(true); 
btnFinish.setEnabled(false); 
} 
protected void onPreExecute(){ 
//stateView.setText("正在录制"); 
btnStart.setEnabled(false); 
btnPlay.setEnabled(false); 
btnFinish.setEnabled(false); 
btnStop.setEnabled(true); 
} 
} 
class PlayTask extends AsyncTask<Void, Integer, Void>{ 
@Override 
protected Void doInBackground(Void... arg0) { 
isPlaying = true; 
int bufferSize = AudioTrack.getMinBufferSize(frequence, channelConfig, audioEncoding);
short[] buffer = new short[bufferSize/4]; 
try { 
//定义输入流，将音频写入到AudioTrack类中，实现播放 
DataInputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(audioFile)));
//实例AudioTrack 
AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, frequence, channelConfig, audioEncoding, bufferSize, AudioTrack.MODE_STREAM);
//开始播放 
track.play(); 
//由于AudioTrack播放的是流，所以，我们需要一边播放一边读取 
while(isPlaying && dis.available()>0){ 
int i = 0; 
while(dis.available()>0 && i<buffer.length){ 
buffer[i] = dis.readShort(); 
i++; 
} 
//然后将数据写入到AudioTrack中 
track.write(buffer, 0, buffer.length); 
} 
//播放结束 
track.stop(); 
dis.close(); 
} catch (Exception e) { 
// TODO: handle exception 
} 
return null; 
} 
protected void onPostExecute(Void result){ 
btnPlay.setEnabled(true); 
btnFinish.setEnabled(false); 
btnStart.setEnabled(true); 
btnStop.setEnabled(false); 
} 
protected void onPreExecute(){ 
btnStart.setEnabled(false); 
btnStop.setEnabled(false); 
btnPlay.setEnabled(false); 
btnFinish.setEnabled(true); 
} 
} 
} 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [Android多媒体学：利用AudioRecord类实现自己的音频录制程序 .](http://jiahua8859-163-com.iteye.com/blog/1147948)
**博客分类：**- [android](http://jiahua8859-163-com.iteye.com/category/132680)
AudioRecord类相对于MediaRecorder来说，更加接近底层，为我们封装的方法也更少。然而实现一个AudioRecord的音频录制程序也很
简单。本实例代码如下：
可惜，本实例测试时有个问题，在录制的时候，会出现buffer over。缓存泄露，待解决。
```java
package demo.camera;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.hardware.Camera.AutoFocusCallback;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaPlayer;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
/**
 * 该实例中，我们使用AudioRecord类来完成我们的音频录制程序
 * AudioRecord类，我们可以使用三种不同的read方法来完成录制工作，
 * 每种方法都有其实用的场合
 * 一、实例化一个AudioRecord类我们需要传入几种参数
 * 1、AudioSource：这里可以是MediaRecorder.AudioSource.MIC
 * 2、SampleRateInHz:录制频率，可以为8000hz或者11025hz等，不同的硬件设备这个值不同
 * 3、ChannelConfig:录制通道，可以为AudioFormat.CHANNEL_CONFIGURATION_MONO和AudioFormat.CHANNEL_CONFIGURATION_STEREO
 * 4、AudioFormat:录制编码格式，可以为AudioFormat.ENCODING_16BIT和8BIT,其中16BIT的仿真性比8BIT好，但是需要消耗更多的电量和存储空间
 * 5、BufferSize:录制缓冲大小：可以通过getMinBufferSize来获取
 * 这样我们就可以实例化一个AudioRecord对象了
 * 二、创建一个文件，用于保存录制的内容
 * 同上篇
 * 三、打开一个输出流，指向创建的文件
 * DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(file)))
 * 四、现在就可以开始录制了，我们需要创建一个字节数组来存储从AudioRecorder中返回的音频数据，但是
 * 注意，我们定义的数组要小于定义AudioRecord时指定的那个BufferSize
 * short[]buffer = new short[BufferSize/4];
 * startRecording();
 * 然后一个循环，调用AudioRecord的read方法实现读取
 * 另外使用MediaPlayer是无法播放使用AudioRecord录制的音频的，为了实现播放，我们需要
 * 使用AudioTrack类来实现
 * AudioTrack类允许我们播放原始的音频数据
 * 
 * 
 * 一、实例化一个AudioTrack同样要传入几个参数
 * 1、StreamType:在AudioManager中有几个常量，其中一个是STREAM_MUSIC;
 * 2、SampleRateInHz：最好和AudioRecord使用的是同一个值
 * 3、ChannelConfig：同上
 * 4、AudioFormat：同上
 * 5、BufferSize：通过AudioTrack的静态方法getMinBufferSize来获取
 * 6、Mode：可以是AudioTrack.MODE_STREAM和MODE_STATIC，关于这两种不同之处，可以查阅文档
 * 二、打开一个输入流，指向刚刚录制内容保存的文件，然后开始播放，边读取边播放
 * 
 * 实现时，音频的录制和播放分别使用两个AsyncTask来完成 
 */
public class MyAudioRecord2 extends Activity{
	
	private TextView stateView;
	
	private Button btnStart,btnStop,btnPlay,btnFinish;
	
	private RecordTask recorder;
	private PlayTask player;
	
	private File audioFile;
	
	private boolean isRecording=true, isPlaying=false; //标记
	
	private int frequence = 8000; //录制频率，单位hz.这里的值注意了，写的不好，可能实例化AudioRecord对象的时候，会出错。我开始写成11025就不行。这取决于硬件设备
	private int channelConfig = AudioFormat.CHANNEL_CONFIGURATION_MONO;
	private int audioEncoding = AudioFormat.ENCODING_PCM_16BIT;
	
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.my_audio_record);
		
		stateView = (TextView)this.findViewById(R.id.view_state);
		stateView.setText("准备开始");
		btnStart = (Button)this.findViewById(R.id.btn_start);
		btnStop = (Button)this.findViewById(R.id.btn_stop);
		btnPlay = (Button)this.findViewById(R.id.btn_play);
		btnFinish = (Button)this.findViewById(R.id.btn_finish);
		btnFinish.setText("停止播放");
		btnStop.setEnabled(false);
		btnPlay.setEnabled(false);
		btnFinish.setEnabled(false);
		
		//在这里我们创建一个文件，用于保存录制内容
		File fpath = new File(Environment.getExternalStorageDirectory().getAbsolutePath()+"/data/files/");
		fpath.mkdirs();//创建文件夹
		try {
			//创建临时文件,注意这里的格式为.pcm
			audioFile = File.createTempFile("recording", ".pcm", fpath);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
	
	
	public void onClick(View v){
		int id = v.getId();
		switch(id){
		case R.id.btn_start:
			//开始录制
			
			//这里启动录制任务
			recorder = new RecordTask();
			recorder.execute();
			
			break;
		case R.id.btn_stop:
			//停止录制
			this.isRecording = false;
			//更新状态
			//在录制完成时设置，在RecordTask的onPostExecute中完成
			break;
		case R.id.btn_play:
			
			player = new PlayTask();
			player.execute();
			break;
		case R.id.btn_finish:
			//完成播放
			this.isPlaying = false;
			break;
			
		}
	}
	
	class RecordTask extends AsyncTask<Void, Integer, Void>{
		@Override
		protected Void doInBackground(Void... arg0) {
			isRecording = true;
			try {
				//开通输出流到指定的文件
				DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(audioFile)));
				//根据定义好的几个配置，来获取合适的缓冲大小
				int bufferSize = AudioRecord.getMinBufferSize(frequence, channelConfig, audioEncoding);
				//实例化AudioRecord
				AudioRecord record = new AudioRecord(MediaRecorder.AudioSource.MIC, frequence, channelConfig, audioEncoding, bufferSize);
				//定义缓冲
				short[] buffer = new short[bufferSize];
				
				//开始录制
				record.startRecording();
				
				int r = 0; //存储录制进度
				//定义循环，根据isRecording的值来判断是否继续录制
				while(isRecording){
					//从bufferSize中读取字节，返回读取的short个数
					//这里老是出现buffer overflow，不知道是什么原因，试了好几个值，都没用，TODO：待解决
					int bufferReadResult = record.read(buffer, 0, buffer.length);
					//循环将buffer中的音频数据写入到OutputStream中
					for(int i=0; i<bufferReadResult; i++){
						dos.writeShort(buffer[i]);
					}
					publishProgress(new Integer(r)); //向UI线程报告当前进度
					r++; //自增进度值
				}
				//录制结束
				record.stop();
				Log.v("The DOS available:", "::"+audioFile.length());
				dos.close();
			} catch (Exception e) {
				// TODO: handle exception
			}
			return null;
		}
		
		//当在上面方法中调用publishProgress时，该方法触发,该方法在UI线程中被执行
		protected void onProgressUpdate(Integer...progress){
			stateView.setText(progress[0].toString());
		}
		
		protected void onPostExecute(Void result){
			btnStop.setEnabled(false);
			btnStart.setEnabled(true);
			btnPlay.setEnabled(true);
			btnFinish.setEnabled(false);
		}
		
		protected void onPreExecute(){
			//stateView.setText("正在录制");
			btnStart.setEnabled(false);
			btnPlay.setEnabled(false);
			btnFinish.setEnabled(false);
			btnStop.setEnabled(true);		
		}
		
	}
	
	class PlayTask extends AsyncTask<Void, Integer, Void>{
		@Override
		protected Void doInBackground(Void... arg0) {
			isPlaying = true;
			int bufferSize = AudioTrack.getMinBufferSize(frequence, channelConfig, audioEncoding);
			short[] buffer = new short[bufferSize/4];
			try {
				//定义输入流，将音频写入到AudioTrack类中，实现播放
				DataInputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(audioFile)));
				//实例AudioTrack
				AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, frequence, channelConfig, audioEncoding, bufferSize, AudioTrack.MODE_STREAM);
				//开始播放
				track.play();
				//由于AudioTrack播放的是流，所以，我们需要一边播放一边读取
				while(isPlaying && dis.available()>0){
					int i = 0;
					while(dis.available()>0 && i<buffer.length){
						buffer[i] = dis.readShort();
						i++;
					}
					//然后将数据写入到AudioTrack中
					track.write(buffer, 0, buffer.length);
					
				}
				
				//播放结束
				track.stop();
				dis.close();
			} catch (Exception e) {
				// TODO: handle exception
			}
			return null;
		}
		
		protected void onPostExecute(Void result){
			btnPlay.setEnabled(true);
			btnFinish.setEnabled(false);
			btnStart.setEnabled(true);
			btnStop.setEnabled(false);
		}
		
		protected void onPreExecute(){	
			
			//stateView.setText("正在播放");
			btnStart.setEnabled(false);
			btnStop.setEnabled(false);
			btnPlay.setEnabled(false);
			btnFinish.setEnabled(true);			
		}
		
	}
}
```
