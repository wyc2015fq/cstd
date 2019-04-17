# 基于Kurento的WebRTC移动视频群聊技术方案 - DoubleLi - 博客园






说在前面的话：视频实时群聊天有三种[架构](http://lib.csdn.net/base/architecture)：

       Mesh架构：终端之间互相连接，没有中心服务器，产生的问题，每个终端都要连接n-1个终端，每个终端的编码和网络压力都很大。群聊人数N不可能太大。

       Router架构：终端之间引入中心服务器，学名MCU(Multi Point Control Unit),每个终端的视频流都发布到MCU服务器上，然后服务器负责编码发布多视频流的工作，减轻客户端的压力。

       Mix架构：在Router架构基础上，多个视频流在服务器端被合为一个视频流，减轻网络压力。

       下面讲我们的选择，在MCU方面有licode、kurento等解决方案。kurento在视频群聊领域有专门的kurento Room解决方案，官方还提供一个kurento room server的样例实现。

       首先可以考虑不是一个Kurento Room Demo作为搭建方案原型的MCU组件。

       Room Demo的部署可见:http://doc-kurento-room.readthedocs.io/en/stable/demo_deployment.html

      其中碰到一些Maven编译问题：





**[plain]**[view plain](http://blog.csdn.net/dipolar/article/details/51953690#)[copy](http://blog.csdn.net/dipolar/article/details/51953690#)

[print](http://blog.csdn.net/dipolar/article/details/51953690#)[?](http://blog.csdn.net/dipolar/article/details/51953690#)

- Unable to initialise extensions Component descriptor role: 'com.jcraft.jsch.UIKeyboardInteractive', implementation: 'org.apache.maven.wagon.providers.ssh.jsch.interactive.PrompterUIKeyboardInteractive', role hint: 'default' has a hint, but there are other implementations that don't  



      Maven的安装版本需要时3.0以上



      还有碰到找不到bower命令行问题。bower是[Node.js](http://lib.csdn.net/base/nodejs)下面的一个包管理工具，安装[node.js](http://lib.csdn.net/base/nodejs)以后用npm安装即可

      最后按照部署指南网页中的命令启动服务器即可。

      Demo服务器有两部分，一部分是Demo Web服务器，二是把官方的kurento room server也集成到了这个demo中。不用再架设独立的kurento room server

      说说[Android](http://lib.csdn.net/base/android)段的实施：再说一个公司：http://www.nubomedia.eu/，这家公司提供实时媒体通信开源云服务，核心组件可能是kurento media server，它的官网和kurento官网用一个模板，about里面显示两家组织有联系，kurento官方提供的[Java](http://lib.csdn.net/base/java)Client因为底层API原因在Android上不肯用，这个nubomedia组织提供了一个kurento android client的实现，同时还提供了一个kurento room client的实现以及room使用案例：https://github.com/nubomedia-vtt/nubo-test,这家公司对其开发的开源方案管理非常及时，早晨提个接口的issue，下午已经commit了代码修改。

       这个案例虽然支持room沟通，但视频沟通是基于room发布订阅机制做的双人聊天。略改一下代码应该就可以实现多人聊天不过这家组织提供的两个client实现和官方的接口高度相似。主要改的是PeerVideoActivity这个类，下面我share一个基本走通多端通信的这个类的代码，供大家参考：





**[java]**[view plain](http://blog.csdn.net/dipolar/article/details/51953690#)[copy](http://blog.csdn.net/dipolar/article/details/51953690#)

[print](http://blog.csdn.net/dipolar/article/details/51953690#)[?](http://blog.csdn.net/dipolar/article/details/51953690#)

- package fi.vtt.nubotest;  
- 
- import android.app.ListActivity;  
- import android.content.SharedPreferences;  
- import android.graphics.PixelFormat;  
- import android.opengl.GLSurfaceView;  
- import android.os.Bundle;  
- import android.os.Handler;  
- import android.util.Log;  
- import android.view.Menu;  
- import android.view.MenuItem;  
- import android.view.View;  
- import android.view.WindowManager;  
- import android.widget.TextView;  
- import android.widget.Toast;  
- 
- import org.webrtc.IceCandidate;  
- import org.webrtc.MediaStream;  
- import org.webrtc.PeerConnection;  
- import org.webrtc.RendererCommon;  
- import org.webrtc.SessionDescription;  
- import org.webrtc.VideoRenderer;  
- import org.webrtc.VideoRendererGui;  
- 
- import java.util.Map;  
- 
- import fi.vtt.nubomedia.kurentoroomclientandroid.RoomError;  
- import fi.vtt.nubomedia.kurentoroomclientandroid.RoomListener;  
- import fi.vtt.nubomedia.kurentoroomclientandroid.RoomNotification;  
- import fi.vtt.nubomedia.kurentoroomclientandroid.RoomResponse;  
- import fi.vtt.nubomedia.webrtcpeerandroid.NBMMediaConfiguration;  
- import fi.vtt.nubomedia.webrtcpeerandroid.NBMPeerConnection;  
- import fi.vtt.nubomedia.webrtcpeerandroid.NBMWebRTCPeer;  
- 
- import fi.vtt.nubotest.util.Constants;  
- 
- /** 
-  * Activity for receiving the video stream of a peer 
-  * (based on PeerVideoActivity of Pubnub's video chat tutorial example. 
-  */  
- public class PeerVideoActivity extends ListActivity implements NBMWebRTCPeer.Observer, RoomListener {  
- private static final String TAG = "PeerVideoActivity";  
- 
- private NBMMediaConfiguration peerConnectionParameters;  
- private NBMWebRTCPeer nbmWebRTCPeer;  
- 
- private SessionDescription localSdp;  
- private SessionDescription remoteSdp;  
- private String PaticipantID;  
- 
- private VideoRenderer.Callbacks localRender;  
- private VideoRenderer.Callbacks remoteRender;  
- private GLSurfaceView videoView;  
- 
- private SharedPreferences mSharedPreferences;  
- 
- private int publishVideoRequestId;  
- private int sendIceCandidateRequestId;  
- 
- private TextView mCallStatus;  
- 
- private String  username, calluser;  
- private boolean backPressed = false;  
- private Thread  backPressedThread = null;  
- 
- private static final int LOCAL_X_CONNECTED = 72;  
- private static final int LOCAL_Y_CONNECTED = 72;  
- private static final int LOCAL_WIDTH_CONNECTED = 25;  
- private static final int LOCAL_HEIGHT_CONNECTED = 25;  
- // Remote video screen position  
- private static final int REMOTE_X = 0;  
- private static  int REMOTE_Y = 0;  
- private static final int REMOTE_WIDTH = 25;  
- private static final int REMOTE_HEIGHT = 25;  
- 
- private Handler mHandler;  
- private CallState callState;  
- 
- private enum CallState{  
-         IDLE, PUBLISHING, PUBLISHED, WAITING_REMOTE_USER, RECEIVING_REMOTE_USER,PATICIPANT_JOINED,RECEIVING_PATICIPANT,  
-     }  
- 
- @Override  
- public void onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         callState = CallState.IDLE;  
- 
-         setContentView(R.layout.activity_video_chat);  
-         getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);  
-         mHandler = new Handler();  
-         Bundle extras = getIntent().getExtras();  
- if (extras == null || !extras.containsKey(Constants.USER_NAME)) {  
-             ;  
-             Toast.makeText(this, "Need to pass username to PeerVideoActivity in intent extras (Constants.USER_NAME).",  
-                     Toast.LENGTH_SHORT).show();  
-             finish();  
- return;  
-         }  
- this.username      = extras.getString(Constants.USER_NAME, "");  
-         Log.i(TAG, "username: " + username);  
- 
- if (extras.containsKey(Constants.CALL_USER)) {  
- this.calluser      = extras.getString(Constants.CALL_USER, "");  
-             Log.i(TAG, "callUser: " + calluser);  
-         }  
- 
- this.mCallStatus   = (TextView) findViewById(R.id.call_status);  
-         TextView prompt   = (TextView) findViewById(R.id.receive_prompt);  
-         prompt.setText("Receive from " + calluser);  
- 
- this.videoView = (GLSurfaceView) findViewById(R.id.gl_surface);  
- // Set up the List View for chatting  
-         RendererCommon.ScalingType scalingType = RendererCommon.ScalingType.SCALE_ASPECT_FILL;  
-         VideoRendererGui.setView(videoView, null);  
- 
- 
-         localRender = VideoRendererGui.create(  LOCAL_X_CONNECTED, LOCAL_Y_CONNECTED,  
-                 LOCAL_WIDTH_CONNECTED, LOCAL_HEIGHT_CONNECTED,  
-                 scalingType, true);  
-         NBMMediaConfiguration.NBMVideoFormat receiverVideoFormat = new NBMMediaConfiguration.NBMVideoFormat(352, 288, PixelFormat.RGB_888, 20);  
-         peerConnectionParameters = new NBMMediaConfiguration(   NBMMediaConfiguration.NBMRendererType.OPENGLES,  
-                 NBMMediaConfiguration.NBMAudioCodec.OPUS, 0,  
-                 NBMMediaConfiguration.NBMVideoCodec.VP8, 0,  
-                 receiverVideoFormat,  
-                 NBMMediaConfiguration.NBMCameraPosition.FRONT);  
-         nbmWebRTCPeer = new NBMWebRTCPeer(peerConnectionParameters, this, localRender, this);  
-         nbmWebRTCPeer.initialize();  
-         Log.i(TAG, "PeerVideoActivity initialized");  
-         mHandler.postDelayed(publishDelayed, 4000);  
- 
-         MainActivity.getKurentoRoomAPIInstance().addObserver(this);  
- 
- 
- 
-         callState = CallState.PUBLISHING;  
-         mCallStatus.setText("Publishing...");  
- 
-     }  
- 
- private Runnable publishDelayed = new Runnable() {  
- @Override  
- public void run() {  
-             nbmWebRTCPeer.generateOffer("derp", true);  
-         }  
-     };  
- 
- @Override  
- public boolean onCreateOptionsMenu(Menu menu) {  
- // Inflate the menu; this adds items to the action bar if it is present.  
-         getMenuInflater().inflate(R.menu.menu_video_chat, menu);  
- return true;  
-     }  
- 
- @Override  
- public boolean onOptionsItemSelected(MenuItem item) {  
- // Handle action bar item clicks here. The action bar will  
- // automatically handle clicks on the Home/Up button, so long  
- // as you specify a parent activity in AndroidManifest.xml.  
- int id = item.getItemId();  
- 
- //noinspection SimplifiableIfStatement  
- if (id == R.id.action_settings) {  
- return true;  
-         }  
- 
- return super.onOptionsItemSelected(item);  
-     }  
- 
- @Override  
- protected void onStart() {  
- super.onStart();  
- 
-     }  
- 
- @Override  
- protected void onPause() {  
-         nbmWebRTCPeer.stopLocalMedia();  
- super.onPause();  
-     }  
- 
- @Override  
- protected void onResume() {  
- super.onResume();  
-         nbmWebRTCPeer.startLocalMedia();  
-     }  
- 
- @Override  
- protected void onStop() {  
-         endCall();  
- super.onStop();  
-     }  
- 
- @Override  
- protected void onDestroy() {  
- super.onDestroy();  
-     }  
- 
- @Override  
- public void onBackPressed() {  
- // If back button has not been pressed in a while then trigger thread and toast notification  
- if (!this.backPressed){  
- this.backPressed = true;  
-             Toast.makeText(this,"Press back again to end.",Toast.LENGTH_SHORT).show();  
- this.backPressedThread = new Thread(new Runnable() {  
- @Override  
- public void run() {  
- try {  
-                         Thread.sleep(5000);  
-                         backPressed = false;  
-                     } catch (InterruptedException e){ Log.d("VCA-oBP","Successfully interrupted"); }  
-                 }  
-             });  
- this.backPressedThread.start();  
-         }  
- // If button pressed the second time then call super back pressed  
- // (eventually calls onDestroy)  
- else {  
- if (this.backPressedThread != null)  
- this.backPressedThread.interrupt();  
- super.onBackPressed();  
-         }  
-     }  
- 
- public void hangup(View view) {  
-         finish();  
-     }  
- 
- public void receiveFromRemote(View view){  
-         Log.e(TAG,"--->receiveFromRemote");  
- if (callState == CallState.PUBLISHED){  
-             callState = CallState.WAITING_REMOTE_USER;  
-             nbmWebRTCPeer.generateOffer("remote", false);  
-             runOnUiThread(new Runnable() {  
- @Override  
- public void run() {  
-                     mCallStatus.setText("Waiting remote stream...");  
-                 }  
-             });  
-         }  
-     }  
- 
- /** 
-      * Terminates the current call and ends activity 
-      */  
- private void endCall() {  
-         callState = CallState.IDLE;  
- try  
-         {  
- if (nbmWebRTCPeer != null) {  
-                 nbmWebRTCPeer.close();  
-                 nbmWebRTCPeer = null;  
-             }  
-         }  
- catch (Exception e){e.printStackTrace();}  
-     }  
- 
- @Override  
- public void onLocalSdpOfferGenerated(final SessionDescription sessionDescription, NBMPeerConnection nbmPeerConnection) {  
-         Log.e(TAG,"--->onLocalSdpOfferGenerated");  
- if (callState == CallState.PUBLISHING || callState == CallState.PUBLISHED) {  
-             localSdp = sessionDescription;  
-             Log.e(TAG,"--->onLocalSdpOfferGenerated:publish");  
-             runOnUiThread(new Runnable() {  
- @Override  
- public void run() {  
- if (MainActivity.getKurentoRoomAPIInstance() != null) {  
-                         Log.d(TAG, "Sending " + sessionDescription.type);  
-                         publishVideoRequestId = ++Constants.id;  
- 
- //                    String sender = calluser + "_webcam";  
- //                    MainActivity.getKurentoRoomAPIInstance().sendReceiveVideoFrom(sender, localSdp.description, publishVideoRequestId);  
- 
-                         MainActivity.getKurentoRoomAPIInstance().sendPublishVideo(localSdp.description, false, publishVideoRequestId);  
-                     }  
-                 }  
-             });  
-         } else { // Asking for remote user video  
-             Log.e(TAG,"--->onLocalSdpOfferGenerated:remote");  
-             remoteSdp = sessionDescription;  
- //            nbmWebRTCPeer.selectCameraPosition(NBMMediaConfiguration.NBMCameraPosition.BACK);  
-             runOnUiThread(new Runnable() {  
- @Override  
- public void run() {  
- if (MainActivity.getKurentoRoomAPIInstance() != null) {  
-                         Log.e(TAG, "Sending--> " +calluser+ sessionDescription.type);  
-                         publishVideoRequestId = ++Constants.id;  
- 
-                         String sender = calluser + "_webcam";  
-                         MainActivity.getKurentoRoomAPIInstance().sendReceiveVideoFrom(sender, remoteSdp.description, publishVideoRequestId);  
-                     }  
-                 }  
-             });  
-         }  
-     }  
- 
- @Override  
- public void onLocalSdpAnswerGenerated(SessionDescription sessionDescription, NBMPeerConnection nbmPeerConnection) {  
-     }  
- 
- @Override  
- public void onIceCandidate(IceCandidate iceCandidate, NBMPeerConnection nbmPeerConnection) {  
-         Log.e(TAG,"--->onIceCandidate");  
-         sendIceCandidateRequestId = ++Constants.id;  
- if (callState == CallState.PUBLISHING || callState == CallState.PUBLISHED){  
-             Log.e(TAG,"--->onIceCandidate:publish");  
-             MainActivity.getKurentoRoomAPIInstance().sendOnIceCandidate(this.username, iceCandidate.sdp,  
-                     iceCandidate.sdpMid, Integer.toString(iceCandidate.sdpMLineIndex), sendIceCandidateRequestId);  
-         } else{  
-             Log.e(TAG,"--->onIceCandidate:"+this.calluser);  
-             MainActivity.getKurentoRoomAPIInstance().sendOnIceCandidate(this.calluser, iceCandidate.sdp,  
-                     iceCandidate.sdpMid, Integer.toString(iceCandidate.sdpMLineIndex), sendIceCandidateRequestId);  
-         }  
-     }  
- 
- @Override  
- public void onIceStatusChanged(PeerConnection.IceConnectionState iceConnectionState, NBMPeerConnection nbmPeerConnection) {  
-         Log.i(TAG, "onIceStatusChanged");  
-     }  
- 
- @Override  
- public void onRemoteStreamAdded(MediaStream mediaStream, NBMPeerConnection nbmPeerConnection) {  
- if (callState == CallState.PUBLISHING || callState == CallState.PUBLISHED) {  
-             Log.e(TAG, "-->onRemoteStreamAdded-->no");  
- return;  
- 
- 
-         }  
-         Log.e(TAG, "-->onRemoteStreamAdded");  
-         RendererCommon.ScalingType scalingType = RendererCommon.ScalingType.SCALE_ASPECT_FILL;  
-         remoteRender = VideoRendererGui.create( REMOTE_X, REMOTE_Y,  
-                 REMOTE_WIDTH, REMOTE_HEIGHT,  
-                 scalingType, false);  
-         REMOTE_Y = REMOTE_Y+25;  
-         nbmWebRTCPeer.attachRendererToRemoteStream(remoteRender, mediaStream);  
-         runOnUiThread(new Runnable() {  
- @Override  
- public void run() {  
-                 mCallStatus.setText("");  
-             }  
-         });  
-     }  
- 
- @Override  
- public void onRemoteStreamRemoved(MediaStream mediaStream, NBMPeerConnection nbmPeerConnection) {  
-         Log.i(TAG, "onRemoteStreamRemoved");  
-     }  
- 
- @Override  
- public void onPeerConnectionError(String s) {  
-         Log.e(TAG, "onPeerConnectionError:" + s);  
-     }  
- 
- @Override  
- public void onRoomResponse(RoomResponse response) {  
-         Log.e(TAG, "-->OnRoomResponse:" + response);  
- if (Integer.valueOf(response.getId()) == publishVideoRequestId){  
-             SessionDescription sd = new SessionDescription(SessionDescription.Type.ANSWER,  
-                     response.getValue("sdpAnswer").get(0));  
- if (callState == CallState.PUBLISHING){  
-                 callState = CallState.PUBLISHED;  
-                 nbmWebRTCPeer.processAnswer(sd, "derp");  
-             } else if (callState == CallState.WAITING_REMOTE_USER){  
-                 callState = CallState.RECEIVING_REMOTE_USER;  
-                 nbmWebRTCPeer.processAnswer(sd, "remote");  
-             } else if (callState == CallState.PATICIPANT_JOINED){  
- 
-                 callState = CallState.RECEIVING_PATICIPANT;  
-                 nbmWebRTCPeer.processAnswer(sd, this.PaticipantID);  
- //NOP  
-             }  
-         }  
-     }  
- 
- @Override  
- public void onRoomError(RoomError error) {  
-         Log.e(TAG, "OnRoomError:" + error);  
-     }  
- 
- @Override  
- public void onRoomNotification(RoomNotification notification) {  
-         Log.e(TAG, "OnRoomNotification--> (state=" + callState.toString() + "):" + notification);  
- 
- if(notification.getMethod().equals("iceCandidate")) {  
-             Map<String, Object> map = notification.getParams();  
- 
-             String sdpMid = map.get("sdpMid").toString();  
- int sdpMLineIndex = Integer.valueOf(map.get("sdpMLineIndex").toString());  
-             String sdp = map.get("candidate").toString();  
- 
-             IceCandidate ic = new IceCandidate(sdpMid, sdpMLineIndex, sdp);  
-             Log.e(TAG, "callState-->" + callState);  
- if (callState == CallState.PUBLISHING || callState == CallState.PUBLISHED) {  
-                 nbmWebRTCPeer.addRemoteIceCandidate(ic, "derp");  
-             }else if(callState==CallState.PATICIPANT_JOINED ||  callState== CallState.RECEIVING_PATICIPANT){  
-                 nbmWebRTCPeer.addRemoteIceCandidate(ic,this.PaticipantID);  
-             }else {  
-                 nbmWebRTCPeer.addRemoteIceCandidate(ic, "remote");  
-             }  
-         }  
- if(notification.getMethod().equals("participantPublished"))  
-         {  
-             Map<String, Object> map = notification.getParams();  
- final String user = map.get("id").toString();  
- this.calluser = user;  
- this.PaticipantID = "pt_"+this.calluser;  
- 
-             PeerVideoActivity.this.runOnUiThread(new Runnable() {  
- @Override  
- public void run() {  
-                     callState = CallState.PATICIPANT_JOINED;  
-                     nbmWebRTCPeer.generateOffer(PaticipantID, false);  
- 
-                 }  
-             });  
-         }  
-     }  
- 
- @Override  
- public void onRoomConnected() {  
- 
-     }  
- 
- @Override  
- public void onRoomDisconnected() {  
- 
-     }  
- }  








       再就是android room demo中的MainActivity的添加cert的代码要去掉注释，让这段代码生效，就可以连通服务器了。

       在[iOS](http://lib.csdn.net/base/ios)的实施方面，上面这家公司也提供了一个工具包：https://github.com/nubomediaTI/Kurento-iOS ，工具包里面也有demo

       Web方面，最上面官方的哪个demo就足够参考了

       后记：很荣幸这篇博客获得了很多CSDN程序员的关注和询问，这只能证明我很荣幸有机会在去年的那个时间点(16年7月)在大家之前处理了一个后续大家都很关注的技术问题，而处理这个问题主要用到的服务器端room server项目和android端nubo test项目，官方在后续好像都做了一定的升级，反而是我自己搞完这个之后，因为产品设计的原因，后来再没有深入地去生产实施这个东西，甚至开发笔记本关于这个项目的源码项目好像都已经删除了，对于大家提出的问题，早期的我还能答一答，后面的我估计你们用到的源码和我用到的源码估计都不是一个版本了，再就是里面的代码细节也基本忘得差不多，在这儿我建议后续开发这个功能可以去深入阅读分析Kurento官方(https://github.com/Kurento）和欧洲媒体服务云服务商nubomedia官方（https://github.com/nubomedia-vtt）的代码示例和文档。我面给出的代码样例是基于nubomedia一对一视聊样例改的，官方原始代码样例在这段时间内都有了变更。在掌握大的基本WebRTC通信的原理的前提下，我觉得改新的代码估计也不会太难。









