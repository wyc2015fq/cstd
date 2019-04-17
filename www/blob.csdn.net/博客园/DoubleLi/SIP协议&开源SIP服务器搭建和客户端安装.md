# SIP协议&开源SIP服务器搭建和客户端安装 - DoubleLi - 博客园






# 1. SIP


SIP 是一个应用层的控制协议，可以用来建立，修改，和终止多媒体会话，例如Internet电话

SIP在建立和维持终止多媒体会话协议上，支持五个方面：

1)   用户定位： 检查终端用户的位置，用于通讯。

2)   用户有效性：检查用户参与会话的意愿程度。

3)   用户能力：检查媒体和媒体的参数。

4)   建立会话： “ringing”,建立会话参数在呼叫方和被叫方。

5)       会话管理：包括发送和终止会话，修改会话参数，激活服务等等。

## 1.1 SIP基本组成要素


用户代理：SIP用户代理是一个SIP逻辑网络端点，用于创建、发送、接收SIP消息并管理一个SIP会话

代理服务器：SIP代理服务器（PROXY）在网络上位于SIP UAC和UAS之间，用于帮助UAC和UAS间的消息路由

注册服务器：SIP注册服务器用于接收SIP注册请求，并保存发送注册请求的UA的位置信息

重定向服务器：SIP 重定向服务器允许 SIP 代理服务器将 SIP 会话邀请信息定向到外部域


## 1.2 SIP 基本呼叫流程

### 1.2.1 注册流程




1.       用户首次试呼时，终端代理A向代理服务器发送REGISTER 注册请求。

2.       代理服务器通过后端认证/计费中心获知用户信息不在数据库中，便向终端代理回送401Unauthorized 质询信息，其中包含安全认证所需的令牌。

3.       终端代理提示用户输入其标识和密码后，根据安全认证令牌将其加密后，再次用REGISTER 消息报告给代理服务器。

4.       代理服务器将REGISTER消息中的用户信息解密，通过认证/计费中心验证其合法后，     将该用户信息登记到数据库中，并向终端代理A 返回成功响应消息200 OK。

### 1.2.2 注销流程




1.       终端向代理服务器送Register消息注销，其头中expire 字段置0。

2.       代理服务器收到后回送200OK 响应，并将数据库中的用户有关信息注销。

### 1.2.3 基本呼叫建立过程





1.       用户摘机发起一路呼叫，终端代理A 向该区域的代理服务器发起Invite 请求。

2.       代理服务器通过认证/计费中心确认用户认证已通过后，检查请求消息中的Via 头域中是否已包含其地址。若已包含，说明发生环回，返回指示错误的应答。如果没有问题，代理服务器在请求消息的Via 头域插入自身地址，并向Invite 消息的To 域所指示的被叫终端代理B 转送Invite 请求。

3.       代理服务器向终端代理A 送呼叫处理中的应答消息，100 Trying。

4.       终端代理B 向代理服务器送呼叫处理中的应答消息，100 Trying;

5.       终端代理B 指示被叫用户振铃，用户振铃后，向代理服务器发送180 Ringing 振铃信息。

6.       代理服务器向终端代理A 转发被叫用户振铃信息。

7.       被叫用户摘机，终端代理B 向代理服务器返回表示连接成功的应答（200 OK）。

8.       代理服务器向终端代理A 转发该成功指示（200 OK）。

9.       终端代理A 收到消息后，向代理服务器发ACK 消息进行确认。

10.   代理服务器将ACK 确认消息转发给终端代理B。

11.   主被叫用户之间建立通信连接，开始通话。

### 1.2.4 正常呼叫释放过程





1.       用户通话结束后，被叫用户挂机，终端代理B 向代理服务器发送Bye 消息。

2.       代理服务器转发Bye 消息至终端代理A，同时向认证/计费中心送用户通话的详细信息，请求计费。

3.       主叫用户挂机后，终端代理A向代理服务器发送确认挂断响应消息200 OK。

4.       代理服务器转发响应消息200OK。

### 1.2.5 被叫无应答流程一





1.       用户A 发起一路呼叫，终端代理A 向代理服务器发Invite 请求消息。

2.       代理服务器向被叫用户的终端代理B 转发该Invite 请求。

3.       代理服务器向终端代理A 回送100 Trying 响应，表示呼叫已在处理中。

4.       终端代理B向代理服务器回送100 Trying，告知代理服务器呼叫正在处理。

5.       被叫用户振铃，终端代理B 向代理服务器送180 Ring 响应。

6.       代理服务器向终端代理A 转发该响应消息。

7.       被叫久振铃无应答，终端代理A判断超时后，向代理服务器送Cancel 消息放弃该呼叫。

8.       代理服务器收到Cancel消息后，向终端代理A 回送200 OK 响应。

9.       代理服务器将Cancel 消息转发给终端代理B。

10.   终端代理B 向代理服务器回送200 OK 响应。

11.   终端代理B 向代理服务器送487 请求已撤销的响应消息。

12.   代理服务器收到后回送ACK确认。

13.   代理服务器向终端代理A 送487 请求已撤销消息。

14.   终端代理A 向代理服务器回送ACK 确认。

### 1.2.6 被叫无应答流程二






1.       用户A 发起一路呼叫，终端代理A 向代理服务器发Invite 请求消息。

2.       代理服务器向被叫用户的终端代理B 转发该Invite 请求。

3.       代理服务器向终端代理A 回送100 Trying 响应，表示呼叫已在处理中。

4.       终端代理B向代理服务器回送100 Trying，告知代理服务器呼叫正在处理。

5.       被叫用户振铃，终端代理B 向代理服务器送180 Ring 响应。

6.       代理服务器向终端代理A 转发该响应消息。

7.       被叫久振铃无应答，终端代理B判断超时后，向代理服务器送408 Request timeout 消息放弃该呼叫。

8.       代理服务器收到408Request timeout 消息后，转发该消息给终端代理A。

9.       代理服务器收到后回送ACK确认给终端代理B。

10.   终端代理A 向代理服务器回送ACK 确认。

## 1.3 代理服务器的路由


### 1.3.1 路由记录的一般过程


1)       proxy会检查Request-URI。如果它指向的是本proxy所负责的区域，那么proxy会用位置服务的结果来替换这个URI。否则，proxy不改变这个URI。

2)       proxy会检查Route头域的最上URI。如果这个URI指向这个proxy，这个proxy从Route头域中移除（这个路由节点已经到达）。

3)       proxy会转发请求到最上的Route头域值所标志的URI，或者Request-URI(如果没有Route头域)。

### 1.3.2 基本SIP四边形



本例子是一个基本的SIP四边传送，U1->P1->P2->U2，使用proxy来传送。下边是过程。



U1 发送：

INVITE sip:callee@domain.com SIP/2.0

Contact: sip:caller@u1.example.com

发给P1,P1是一个外发的proxy。P1并不管辖domain.com，所以它查找DNS并且发送请求到那里。它也增加一个Record-Route头域值：

INVITE sip:callee@domain.com SIP/2.0

Contact: sip:caller@u1.example.com

Record-Route: <sip:p1.example.com;lr>



P2收到这个请求。这是domain.com所以它查找位置服务器并且重写Request-URI。它也增加一个Record-Route头域值。请求中没有Route头域，所以它解析一个新的Request-URI来决定把请求发送到哪里。

INVITE sip:callee@u2.domain.com SIP/2.0

Contact: sip:caller@u1.example.com

Record-Route: <sip:p2.domain.com; lr>

Record-Route: <sip:p1.example.com;lr>



在u2.domain.com的被叫方接收到这个请求并且返回一个200OK应答：

SIP/2.0 200 OK

Contact: sip: callee@u2.domain.com

Record-Route: <sip:p2.domain.com;lr>

Record-Route: <sip:p1.example.com;lr>



u2的被叫方并且设置对话的状态的remote target URI为：

sip: caller@u1.example.com并且它的路由集合是：

（<sip:p2.domain.com;lr>,<sip:p1.example.com;lr>）



这个转发通过P2到P1到U1。现在U1设置它自己的对话状态的remote target URI为：sip:calle@u2.domain.com并且它的路由集合是：

（<sip:p1.example.com;lr>,<sip:p2.domain.com;lr>）



由于所有的路由集合元素都包含了lr参数，那么U1构造最后的BYE请求：

BYE sip:callee@u2.domain.com SIP/2.0

Route:<sip:p1.example.com;lr>,<sip:p2.domain.com;lr>



就像其他所有的节点（包括proxy）会做的那样，它会使用DNS来解析最上的Route头域的URI值，这样来决定往哪里发送这个请求。这就发到了P1。P1发现Request-URI中标记的URI不是它负责的域，于是它就不改变这个Request-URI。然后看到它是Route头域的第一个值，于是就从Route头域中移去，并且转发这个请求到P2：

BYE sip:callee@u2.domain.com SIP/2.0

Route: <sip:p2.domain.com;lr>

P2也发现它自己并非负责这个Request-URI的域（P2负责的是domain.com并非u2.domain.com）,于是P2并不改变它。它看到自己在Route的第一个值，于是移去这个，并且向u2.domain.com转发（根据在Request-URI上查找DNS）：

BYE sip:callee@u2.domain.com SIP/2.0

### 1.3.3 重写Record-Route头域值



在这里例子中,U1和U2是在不同的私有域空间中，并且他们通过proxy P1开始一个对话，这个P1作为不同私有namespace的一个网关存在。

U1->P1->U2

U1发送：

INVITE sip:callee@gateway.leftprivatespace.comSIP/2.0

Contact:<sip:caller@u1.leftprivatespace.com>



P1使用自己的定位服务并且发送下边的信息到U2:

INVITE sip:callee@rightprivatespace.comSIP/2.0

Contact:<sip:caller@u1.leftprivatespace.com>

Record-Route:<sip:gateway.rightprivatespace.com;lr>



U2发送200 OK应答回给P1：

SIP/2.0 200 OK

Contact:<sip:callee@u2.rightprivatespace.com>

Record-Route:<sip:gateway.rightprivatespace.com;lr>



P1重写它的Record-Route头域参数，提供成为U1能够使用的参数，并且发送给P1：

SIP/2.0 200 OK

Contact:<sip:callee@u2.rightprivatespace.com>

Record-Route: <sip:gateway.leftprivatespace.com;lr>



稍后，U1发送接下来的BYE到P1:

BYE sip:callee@u2.rightprivatespace.comSIP/2.0

Route:<sip:gateway.leftprivatespace.com;lr>



P1转发到U2：

BYE sip:callee@u2.rightpriatespace.comSIP/2.0

# 2 开源SIP服务器和客户端


## 2.1 开源SIP 服务器


1)       Asterisk

2)       Cipango

3)       FreeSWITCH

4)       GNU SIP Witch

5)       Mobicents

6)       Mysipswitch

7)       OpenSER

8)       OpenSIPS

9)       SSailFin

10)   SIP Express Router sipX

11)   Yate

12)   YXA

## 2.2 开源SIP 客户端



1)       Blink

2)       Ekiga

3)       Empathy

4)       Jitsi

5)       KPhone

6)       Linphone

7)       MicroSIP

8)       PhoneGaim

9)       QuteCom

10)   SFLphone

11)   Telephone

12)   Twinkle

13)   Yate client

## 2.3 开源SIP服务器比较


|名称|编程语言|操作系统|许可协议|功能|
|----|----|----|----|----|
|Asterisk|C|跨平台|GNU GPL/Proprietary|Voice mailConference callingInteractive voice responseautomatic call distribution|
|Cipango|Java||Apache 2.0||
|Free Switch|C|跨平台|MPL|会议，使用XML-RPC 控制呼叫，Interactive voice response (IVR), TTS/ASR (语音合成和语音识别), Public switched telephone network (PSTN) 接口，可连接模拟和数字中继，VoIP 协议包括 SIP，SCCP, H.323, XMPP, GoogleTalk, t.38 等等|
|GNU SIP Witch|C++|跨平台|GPL|Call forwarding, call distribution, call hold, presence information and (text) messages, supports encrypted calls and also enables NAT traversal to establish the peer-to-peer connections.|
|Mobicents|Java|跨平台|LGPL||
|Mysipswitch|C#||BSD|SIP account creationSetting up a customized dial planSetting up 3rd party SIP RegistrationsSIP traffic forwardingSIP Accounts activity monitoring via the websiteSIP traffic monitoring via telnetOnline switchboard: call hold/resume, call transfer/forward, call hangupUsual security featuresClick to Call (Beta)Possibility to run it on a local computerMultiple call forwardingRUBY Dial plansENUM Lookup|
|Opensip|C|Linux, FreeBSD, Solaris|GNU GPL|SIP registrar serverSIP router / proxy (lcr, dynamic routing, dialplan features)SIP redirect serverSIP presence agentSIP back-to-back User AgentSIP IM server (chat and end-2-end IM)SIP to SMS gateway (bidirectional)SIP to XMPP gateway for presence and IM (bidirectional)SIP load-balancer or dispatcherSIP front end for gateways/asteriskSIP NAT traversal unitSIP application server|
|SailFin|Java|Cross-platform|||
|SIP Express Router|C|Linux, BSD, Solaris|GPL|RFC 3261 functionality, a variety of database backends (mysql, oracle, postgres, radius, text-db), management features (remote management via XML-RPC, load-balancing), NATi traversal, telephony features (LCR, speeddial), multidomain hosting, ENUM, presence, and even more|
|sipX||Fedora CentOS RHEL|Affero General Public License|traditional private branch exchange (PBX) like voice mail, interactive voice response systems, auto attendants and the like. Furthermore it integrates with Exchange 2007and Active Directory Environments.|
|Yate|C++|Cross-platform|GNU General Public License with linking exception|VoIP serverSS7 switchVoIP clientJabber serverJabber clientConference server - with up to 200 voice channels in a single conferenceVoIP to PSTN gatewayPC2Phone and Phone2PC gatewayIP Telephony server and/or clientH.323 gatekeeperH.323 multiple endpoint serverH.323<->SIP ProxySIP session border controllerSIP routerSIP registration serverIAX server and/or clientJingle client or serverMGCP server (Call Agent)ISDN passive and active recorderISDN, RBS, analog passive recorderCall center serverIVR enginePrepaid and/or postpaid cards system|
|YXA|Erlang|Cross-platform|New BSD license||

## 2.4 开源SIP客户端比较


|名称|编程语言|操作系统|许可协议|功能|
|----|----|----|----|----|
|Blink|Python|Mac OS X, Windows and Linux|GNU GPL|OSX Integration (iCloud, iTunes, Address Book, Keychain, Voice Over)iCloud synchronization for accountsHistory menu for outgoing and incoming callsHistory browserSystem Address Book external plugin (can dial with Blink from Address Book)Answering machineCall transferCall recordingLDAP directoryLaunch external application on incoming callsPhone number translations|
|Ekiga|C C++|Unix-like, Windows|GNU General Public License|Call forwarding on busy, no answer, always (SIP and H.323)Call transfer (SIP and H.323)Call hold (SIP and H.323)DTMF support (SIP and H.323)Basic instant messaging (SIP)Text chat (SIP and H.323)Register with several registrars (SIP) and gatekeepers (H.323) simultaneouslyAbility to use an outbound proxy (SIP) or a gateway (H.323)Message waiting indications (SIP)Audio and video (SIP and H.323)STUN support (SIP and H.323)LDAP supportAudio codec algorithms: iLBC, GSM 06.10, MS-GSM, G.711 A-law, G.711 µ-law, G.726, G.721, Speex, G.722, CELT (also G.723.1, G.728, G.729, GSM 06.10, GSM-AMR, G.722.2 [GSM‑AMR-WB] using Intel IPP)Video codec algorithms: H.261, H.263+, H.264, Theora, MPEG-4|
|Empathy|C|BSD, Linux, Other Unix-like|GNU GPL|ulti-protocol: Google Talk (Jabber/XMPP), MSN, IRC, Salut, AIM, Facebook, Yahoo!, Gadu Gadu, Groupwise, ICQ and QQ. (Supported protocols depend on installed Telepathy Connection Manager components.) Supports all protocols supported by Pidgin.File transfer for XMPP, and local networks.Voice and video call using SIP, XMPP and Google Talk.Some IRC support.For detailed list of supported protocol features see hereConversation theming (see list of supported Adium themes).Sharing and viewing location information.Private and group chat (with smileys and spell checking).Conversation logging.Automatic away and extended away presence.Automatic reconnection using Network Manager.Python bindings for libempathy and libempathy-gtkSupport for collaborative applications (“tubes”).|
|Jitsi|Java|Linux, Mac OS X, Windows (all Java supported)|LGPL|Attended and blind call transferAuto awayAuto re-connectAuto answer and Auto ForwardCall recordingCall encryption with SRTP and ZRTPConference callsDirect media connection establishment with the ICE protocolDesktop StreamingEncrypted password storage using a master passwordFile transfer for XMPP, AIM/ICQ, Windows Live Messenger, YIMInstant messaging encryption with OTRIPv6 support for SIP and XMPPMedia relaying with the TURN protocolMessage Waiting Indication (RFC 3842)Voice and video calls for SIP and XMPP using H.264 and H.263 or VP8 for video encodingWideband audio with SILK, G.722, Speex and OpusDTMF support with SIP INFO, RTP (RFC 2833/RFC 4733), In-bandZeroconf via mDNS/DNS-SD (à la Apple's Bonjour)DNSSECGroup video support (Jitsi Videobridge)Packet loss concealment with the SILK and Opus codecs|
|KPhone|C++|Linux|GPL|Multiple parallel sessions (in the case of audio, one may be active, the others are held).Own ring tones or "ring music"NAT-traversal and STUN supportSupported sound systems: ALSA and OSSSRTP encryption for voicePresence informationCall HoldCall transferCall forwardingAuto Answer|
|Linphone|C|Cross-platform|GNU GPL version 2|SIP user agent compliant with RFC 3261SIP/UDP, SIP/TCP, SIP/TLSSupports IPv6Digest authenticationSupports multiple calls simultaneously with call management features: hold on with music, resume, transfer...Multiple SIP proxy support: registrar, proxies, outbound proxiesText instant messaging with delivery notificationPresence using the SIMPLE standard in peer to peer modeDTMF (telephone tones) support using SIP INFO or RFC 2833|
|MicroSIP|C/C++|Windows|GNU General Public License|Profile of a lightweight background application[2]Small memory footprint (<20 mb RAM usage)Strong adherence to the SIP standardSupport for a number of codecs: Speex (narrow band and wideband), G.711 (u-law, a-law), GSM, iLBC, SILK (narrow band, wideband and ultra wideband), G.722No Support for VP8 codec as of nowSTUN and ICE NAT traversalSIP SIMPLE presence and messaging|
|QuteCom|C/C++|Cross-platform|GNU General Public License|SIP complianceProvider agnosticAllows users to send SMS to FranceNAT traversalCross-platformAudio smileysQt-based GUIChatting with MSN, AIM, ICQ, Yahoo and XMPP usersEncryption via SRTP, but key exchange over Everbee key that is not a StandardUses standard Session Initiation Protocol|
|SFLphone|C / C++|Linux|GNU General Public License 3|SIP and IAX compatibleUnlimited number of callsCall recordingAttended call transferCall holdMultiple audio conferencing (from 0.9.7 version)TLS and ZRTP support (from 0.9.7 version)Audio codecs supported: G711u, G711a, GSM, Speex (8, 16, 32 kHz), CELT, G.722Multiple SIP accounts supportSTUN support per account (0.9.7)DTMF support (SIP INFO)Instant messagingCall history + search featureSilence detection with Speex audio codecAccount assistant wizardCentral server providing free SIP/IAX accountSIP presence subscriptionVideo multiparty conferencing (EXPERIMENTAL)Multichannel audio support [EXPERIMENTAL]Flac and OGG/Vorbis ringtone supportDesktop notification: voicemail number, incoming call, information messagesMinimize on start-upMinimize to traynot Direct IP-to-IP SIP call - P2P is not supported by IAx2 according to the documentationSIP Re-inviteAddress book support: Evolution Data Server integration (for the GNOME client), KABC integration for the KDE clientPulseAudio supportNative ALSA interface, DMix supportLocale settings: French, English, Russian, German, Chinese, Spanish, Italian, VietnameseAutomatic opening of incoming URL|
|Telephone|Objective-C|Mac OS X|BSD License||
|Twinkle|C++|GNU/Linux|GNU General Public License|2 call appearances (lines)Multiple active call identitiesCustom ring tonesCall WaitingCall Hold3-way conference callingMuteCall redirection on demandCall redirection unconditionalCall redirection when busyCall redirection no answerReject call redirection requestBlind call transferCall transfer with consultation (attended call transfer)Reject call transfer requestCall rejectRepeat last callDo not disturbAuto answerMessage Waiting InidicationVoice mail speed dialUser defineable scripts triggered on call eventsE.g. to implement selective call reject or distinctive ringingRFC 2833 DTMF eventsInband DTMFOut-of-band DTMF (SIP INFO)STUN support for NAT traversalSend NAT keep alive packets when using STUNNAT traversal through static provisioningPersistent TCP connections for NAT traversalMissed call indicationHistory of call detail records for incoming, outgoing, successful and missed callsDNS SRV supportAutomatic failover to an alternate server if a server is unavailableOther programs can originate a SIP call via Twinkle, e.g. call from address bookSystem tray iconSystem tray menu to quickly originate and answer calls while Twinkle stays hiddenUser defineable number conversion rulesSimple address bookSupport for UDP and TCP as transport for SIPPresenceInstant messagingSimple file transfer with instant messageInstant message composition indicationCommand line interface (CLI)|
|Yate|C++|Cross-platform|GNU General Public License with linking exception|VoIP serverSS7 switchVoIP clientJabber serverJabber clientConference server - with up to 200 voice channels in a single conferenceVoIP to PSTN gatewayPC2Phone and Phone2PC gatewayIP Telephony server and/or clientH.323 gatekeeperH.323 multiple endpoint serverH.323<->SIP ProxySIP session border controllerSIP routerSIP registration serverIAX server and/or clientJingle client or serverMGCP server (Call Agent)ISDN passive and active recorderISDN, RBS, analog passive recorderCall center serverIVR enginePrepaid and/or postpaid cards system|


## 2.5 SIP 服务器和客户端选择


### 2.5.1 SIP服务器选择：OpenSIPS


1)       高通过量

2)       灵活的路由功能和整合

3)       有效的应用的建立

4)       支持C/C++

5)       运行平台：Linux

### 2.5.2 SIP客户端选择：Twinkle, Linphone, Kphone



1)       较为广泛应用

2)       Twinkle不支持视频通话

3)       Linphone和Kphone支持视频通话

4)       支持C/C++

5)       运行平台： Linux

## 2.6 OpenSIPS的搭建环境



1)       OpenSIPS源代码下载:

用svn down下代码 svn cohttps://opensips.svn.sourceforge.net/svnroot/opensips/branches/1.9 opensips_1_9

2)       安装MySQL

3)       OpenSIPS安装

root@ubuntu:cd /home/amaryllis/work/project/opensips/

root@ubuntu:make menuconfig

4)       OpenSIPS文件配置

a)       修改配置文件opensipsctlrc:

root@ubuntu:gedit /usr/local/opensips_proxy/etc/opensips/opensipsctlrc

b)       安装数据库:

root@ubuntu:cd /usr/local/opensips_proxy/sbin/

root@ubuntu:./opensipsdbctl create

c)       检查M4是否安装:

apt-get install m4

d)      生成opensips_residential_2013-3-10_22:52:46.cfg文件:

root@ubuntu:cd /usr/local/opensips_proxy/sbin/

root@ubuntu:./osipconfig

5)       设置启动项:

root@ubuntu:cd /home/amaryllis/work/project/opensips/packaging/debian

root@ubuntu:cpopensips.init /etc/init.d/opensips

root@ubuntu:chmod+x /etc/init.d/opensips

root@ubuntu:gedit/etc/init.d/opensips

6)       设置默认项opensips.default:

root@ubuntu:cd /home/amaryllis/work/project/opensips/packaging/debian 

root@ubuntu:cp opensips.default /etc/default/

root@ubuntu:cd  /etc/default/

root@ubuntu:mv opensips.default opensips

root@ubuntu:gedit opensips

7)       启动OpenSIPS:

root@ubuntu:/etc/init.d/opensips restart（重启）

或者

root@ubuntu:/etc/init.d/opensips start（启动）

详情请参考[该网页](http://blog.csdn.net/dahuaishu2010_/article/details/9095503)

## 2.7 客服端编译安装


### 2.7.1 源代码的编译安装


1)       下载源代码：源代码一般以file.tar.gzfile.tar.bz2或file.src.rpm ，用tar jxvf file.tar.bz2 或者tar zxvffile.tar.gz来解压安装包

2)       设置编译环境：安装gcc;perl; python; glibc; gtk; make; auto make 等开发工具或基础包。如果您在编 译软件时,有时提示缺少什么东西之类的,大多少的是这些开发工具和开发库等;从光盘中找出安 装就是了;有时光盘没有提供,请用 google 搜索相应的软件包,有时可能也会用到源码包编译安 装所依赖的包; 有时本来系统中已经安装了所依赖的包,但系统提示找不到应该怎么办?这时需 要我们设置一下PKG_CONFIG_PATH的环境变量就行了; #export PKG_CONFIG_PATH=/usr/lib/pkgconfig

3)       ./cofigure –prefix=/usr/你想要安装的目录

4)       Make

5)       Make install

详情请参考[该网页](http://blog.csdn.net/fang5034012/article/details/37725507)

### 2.7.2 直接安装rpm安装包



rpm -i 需要安装的包文件名










