# Android 音频口PCM通信 - 简书


# Android 音频口PCM通信



### 相关

[demo下载地址](https://link.jianshu.com?t=http://download.csdn.net/detail/dangxw_/5985175)
[相关链接](https://link.jianshu.com?t=http://www.robots-everywhere.com/re_wiki/index.php?title=Serial_on_Android_using_the_audio_port)

### demo介绍：

本例只实现了发送字符串的功能（用的是方波标记），而且界面布局很简单，贴图：






![](https://upload-images.jianshu.io/upload_images/1802307-82015436df768d9d.jpg)




点击【send】将会发送出一段噪音，噪音的内容是用你所输入的字符串编码过来的，用的是Ascall码转换成高低电频，当然在手机的耳机插口需要插入一个自己开发的板子，接收端需要安装配套的信号接受器。

绑定button。
`listener = new Listener(); play = (Button)findViewById(R.id.play); text = (EditText)findViewById(R.id.text); play.setOnClickListener(listener);`

listener的内容：
```
private class Listener implements OnClickListener  
{  
    @Override  
    public void onClick(View arg0) {  
        // TODO Auto-generated method stub  
        sendme = text.getText().toString();  
        Log.d("!!!!!!","listener");  
        if(sendme != ""&& sendme != null)  
        {     
            if(thread == null)  
            {  
                thread = new AudioThread();  
                thread.start();  
            }  
            else  
                thread.letRun();  
        }  
          
    }  
      
}
```

thread的内容：
`class AudioThread extends Thread{ @Override public void run() { // TODO Auto-generated method stub while(true) { if(isrun) { Log.d("!!!!!!","runner"); audioplayer = new AudioSend(); audioplayer.play(sendme); try { sleep(1000); } catch (InterruptedException e) { // TODO Auto-generated catch block e.printStackTrace(); } isrun =false; } } } public void letRun() { Log.d("!!!!!!","letrunner"); isrun = true; } }`

已经发现重点就在audioplayer这个对象，这个是自己用audiosend类封装的，运用的是AudioTrack，查看api就可以发现这个类有个write（）方法，方法是重载的：write（byte[]，int，int）或者是write（short[]，int，int），数组内即为pcm消息。

### PCM内容解析

先来了解下耳机的构造：

我们知道，耳机是用来听音乐，打电话的，既然是和声音相关的，那么耳机线上传输的就是音频信号，常见的音频信号一般都是在100Hz——10KHz左右的范围内，那么手机里面的音频输出系统（DA和音频功放）的幅频特性（也既带宽）一定也是在这个范围（这是本人的猜想，由于设备和仪器有限，没有进行系统的测试，有兴趣的朋友可以用相关的测试仪器测测），那么，既然有带宽，好家伙，我们就可以通过努力在这个频带内实现我们的通信信道了！另外值得提的一点是，耳机线上传输的音频信号是交流的！

下面我们来看看市面上常见的耳机座（公头）的引脚定义，android手机上用的耳机大多都是3.5mm的四芯座，在这四个芯中，分别是：地、左声道、右声道和线控开关（MIC）





![](https://upload-images.jianshu.io/upload_images/1802307-63b71b573d95af59.jpg)




发现其实耳机接受的就是电流，那么那个数组中包含的应该就是电流的强度。那么我们只需要将所要发送的内容转换为电流的强度来标记，并且在接受端按照相同的方式来解码即可。

### 通信协议

如图所示通信协议中每十个位标记位发送的一个字母，开始位定义成低电流终止位定义成高电流（我们没有定奇偶校检位），中间的八位来定义字母的内容（也就是字母转换成Acall码之后的内容）但是每个电流如果用1和0表示的话差别太小了，单片机很可能检测不到，或者检测失误，所以我将高电流定义为-128，低电流定义为16，代码先将每个字母转化成对应的ascll码，然后再将它转换成对应的高低电流的波峰和波谷。





![](https://upload-images.jianshu.io/upload_images/1802307-f7a03c81b2edff00.jpg)




### 发送消息

`public class AudioSend { static int baudRate = 4800; static int maxRate = 48000; static int delayBit = 0; private static byte ihigh = (byte) (-128); private static byte ilow = (byte) (16); AudioTrack audioplayer; static int minSize; static byte[] getBuffer(String str) { int bytesinframe = delayBit + 10;//delay + 8bit + 一个标识开始的位 + 一个标识结束的位 byte[] sendme = str.getBytes(); int n = maxRate/baudRate; boolean[] bits = new boolean[sendme.length \* bytesinframe];// byte[] waveform = new byte[(sendme.length\*bytesinframe* n)]; //防止失真，延长每个波的变化的播放时间 Arrays.fill(bits, true); //当其不断传出电流的时候标志着无信息传送，一旦有低压电流标志开始传送数据 int i,m,k,j = 0; for (i=0;i<sendme.length;++i) { m=i\*bytesinframe; bits[m]=false; bits[++m]=((sendme[i]&1)==1);//位操作，也可以先转换成数字再用 Integer.toBinaryString bits[++m]=((sendme[i]&2)==2); bits[++m]=((sendme[i]&4)==4); bits[++m]=((sendme[i]&8)==8); bits[++m]=((sendme[i]&16)==16); bits[++m]=((sendme[i]&32)==32); bits[++m]=((sendme[i]&64)==64); bits[++m]=((sendme[i]&128)==128); //加上延时的位 for(k=0;k<bytesinframe-9;k++) bits[++m]=true; } //转换成需要的byte数组 for (i=0;i<bits.length;i++) { for (k=0;k<n;k++) { waveform[j++]=(bits[i])?((byte) (ihigh)):((byte) (ilow)); } } bits=null; return waveform; } public void play (String str) { byte[] send = getBuffer(str); minSize = AudioTrack.getMinBufferSize(48000,AudioFormat.CHANNEL_OUT_MONO ,AudioFormat.ENCODING_PCM_16BIT); audioplayer = new AudioTrack(AudioManager.STREAM_MUSIC, 48000, AudioFormat.CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT, minSize, AudioTrack.MODE_STREAM); audioplayer.play(); audioplayer.write(send, 0, send.length); audioplayer.stop(); audioplayer.release(); } }`

在构造audiotrack的时候
`minSize = AudioTrack.getMinBufferSize(48000,AudioFormat.CHANNEL_OUT_MONO ,AudioFormat.ENCODING_PCM_16BIT); audioplayer = new AudioTrack(AudioManager.STREAM_MUSIC, 48000, AudioFormat.CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT, minSize, AudioTrack.MODE_STREAM);`

第一行其实是获取最小缓冲区大小，阐述按顺序依次是采样率，声道，和采样精度.采样率就是每秒钟要发送多少个点过去（其实android上只能是4800,44100,48000），也就是告诉audiotrack对象每秒需要从我所write的数组中提取多少个点，当然在播放音乐的时候采样率越高就会音质效果越好。声道就不提了，只是音频中实现声道的方式并不是若干条不相干的音频，而是混在一次，比如双通道：123456的采样，播放结果是左声道：135，右声道246.单通道的话就是123456，时间延长一倍。采样精度就是电流的强度的上限和下限，当然也是越大音质越好，目前似乎只能是8BIT和16BIT，应该也就是这个原因所以write接受的是byte和short数组吧。

对于代码段：
`//转换成需要的byte数组 for (i=0;i<bits.length;i++) { for (k=0;k<n;k++) { waveform[j++]=(bits[i])?((byte) (ihigh)):((byte) (ilow)); } }`

目的是为了达到我们想要的采样率而延时，也就是将每个波的变化延长k个单位时间，k呢是48000/4800得来的，也就是我机器每秒发送48000个电流变化值出去，但是现在我想只让它发送4800个，那也就需要把每个点复制十份然后发送。当然，如果你是想把采样率定为4800的话可以直接在初始化的时候写，但是在硬件设置中采样率（其实硬件中叫波特率）就有很多选择了可以是600,1200,2400,4800，44100等等，为了可以尽可能的满足不同硬件需求，建议这样写。








