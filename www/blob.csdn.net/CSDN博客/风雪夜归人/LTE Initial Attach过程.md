# LTE Initial Attach过程 - 风雪夜归人 - CSDN博客
2017年08月28日 16:00:58[cdjccio](https://me.csdn.net/qq_34624951)阅读数：840
# **LTEInitialAttach过程02-1**
UE开机，通过小区选择和小区重选，驻留到合适的小区；
## 步骤1:rrcConnectionRequest
messagec1:rrcConnectionR；criticalExtensionsrrcCon；ue-IdentityrandomValue:'；rrcConnectionReqest是在SRB；在此消息中，UE-Identity的目的是为了底；NAS层通过
LTE Initial Attach 过程UE开机，通过小区选择和小区重选，驻留到合适的小区后，就进行”Initial EPS Attach”的过程。通过初始EPS附着，UE可以在EPS网络进行分组域服务的注册，同时，建立缺省的EPS承载，进行用户数据的传输。（3GPP 23.401和29.274）。
**rrcConnectionRequest message c1 : rrcConnectionRequest : {**criticalExtensions rrcConnectionRequest-r8 :
{
ue-Identity randomValue : '11110111 01001110 00000010 10000110 100 ...'B, establishmentCause mo-Signalling, spare '0'B 
}
rrcConnectionReqest是在SRB0上传输的， SRB0一直存在， 用来传输映射到CCCH 的RRC信令。
在此消息中， UE-Identity 的目的是为了底层随机接入的竞争消除。它可以是S－TMSI，也可以是UE生成的随机数。
在InitialEPSAttach的过程中，UE还没有获得S－TMSI，因此包含了一个40位的随机值。
NAS层通过establishmentCause指明连接的原因。 
## 步骤2：RRCConnectionSetup
RRCConnectionSetup message c1 : rrcConnectionSetup : {
rrc-TransactionIdentifier 0,
criticalExtensions c1 : rrcConnectionSetup-r8 : {
radioResourceConfigDedicated {
srb-ToAddModifyList { {
srb-Identity 1,
rlc-Configuration defaultValue : NULL, logicalChannelConfig defaultValue : NULL } },
mac_MainConfig { ... }
physicalConfigDedicated { 。。。 } } } }
通过底层的竞争接入冲突解决机制，UE接收到NodeB的rrcConnectionSetup信令，建立了UE与ENodeB之间的SRB1，NodeB为SRB1配置RLC层和逻辑层信道的属性。ENodeB还可以在此信令中对MAC层和物理层进行配置，如果NodeB没有对此进行配置， 36.331中定义了MAC 层和物理层的缺省值。
UE收到NodeB的rrcConnectionSetup信令后，UE和NodeB之间的SRB1就建立起来了。 
## 步骤3：RRCConnectionSetupComplete
UL-DCCH-Message =
message = c1 = rrcConnectionSetupComplete = rrc-TransactionIdentifier = 0
criticalExtensions = c1 = rrcConnectionSetupComplete-r8 = selectedPLMN-Identity = 1 dedicatedInfoNAS = ()。 在UE接收到RRCConnectionSetup消息后，向NodeB发送一个RRCConnectionSetupComplete消息。其中， selectedPLMN-Identity表示UE选中的PLMN在SIB1中广播的PLMN
 List中的序号值。
RRCConnectionSetupComplete消息中的dedicatedInfoNAS包含了NAS层的信令，在EUTRAN中UE的初始接入过程中，NAS层信令通常是EMM层的AttachRequest消息和ESM层的PDNConnectivityRequest消息。 
Msg：aTTACH_REQUEST
securityHeaderType = '0000'B protocolDiscriminator = '0111'B messageType = '01000001'B nasKeySetId iei = Omit tsc = '0'B
nasKeySetId = '111'B epsAttachType spare = '0'B
typeValue = '001'B oldGutiOrImsi iei = Omit iel = '0B'O
idDigit1 = '1111'B oddEvenInd = '0'B typeOfId = '110'B
otherDigits = '00F11000010112345678'O ueNetworkCapability iei = Omit iel = '02'O
networkCap = 'C0C0'O esmMessage iei = Omit iel = '0005'O
esmPdu = '0201D031D1'O oldPtmsiSignature = Omit additionalGuti = Omit lastVisitedRegisteredTai
iei = '52'O
plmnId = '00F110'O tac = '0001'O
drxParameter = Omit
msNetworkCapability = Omit oldLai = Omit tmsiStatus = Omit msClassmark2 = Omit msClassmark3 = Omit
supportedCodecList = Omit PiggybackedPduList NAS_UL_Pdu_Type Msg
pDN_CONNECTIVITY_REQUEST epsBearerId = '0'H
protocolDiscriminator = '0010'B
procedureTransactionIdentifier = '01'O messageType = '11010000'B pdnType
spare = '0'B
typeValue = '011'B requestType spare = '0'B
typeValue = '001'B esmInfoTransferFlag iei = 'D'H spare = '000'B eitValue = '1'B
accessPointName = Omit
protocolConfigurationOptions = Omit PiggybackedPduList = Omit
在AttachRequest中，oldGutiOrImsi会被MME用来在HSS中查询用户的签约信息。其中 = 。M-TMSI是32位的在MME内UE的ID。
MMGI＝ MME Group ID， MMEC ＝ MME Code。
由于RRCConnectionSetupComplete消息是明文传送的，为了保护IMSI的私密性，应尽量减少IMSI在空口传播，GUTI的使用就是为了这个目的。当然，手机初始附着，由于不存在OldGUTI，还是会发送一次IMSI。如果eNodeB给UE找到的MME不是之前detach那个MME，新的MME将通过old GUTI找到旧的MME（即上次detach时的MME），再发送Identification Request(GTP-C)消息给旧的MME以获得手机的IMSI。这个Identification
 Request消息包含old GUTI和完整的Attach请求消息。如果(新的)MME仍然无法得到UE的IMSI， MME会发送IdentityRequest消息给UE, 要求UE上报自己的IMSI。
drxParameter指明UE特定的DRX相关参数。UE通过此参数通知EUTRAN自己特定的寻呼周期, 系统广播消息SIB2中的PCCH Config参数中也定义了缺省的寻呼周期,在这种情况下,UE使用两者中的最小值。
lastVisitedRegisteredTai用来帮助MME生成有效的TAI列表，MME将在Attach Accept消息中返回给UE。
ueNetworkCapability包含NAS和AS的安全参数 pdnType 表示PDN连接的IP类型。（IPv4、IPv6或者IPv4/IPv6）
## 步骤4：Initial UE Message
eNodeB接收到RRCConnectionComplete消息后，根据里面的信息，选定相应的MME，然后通过eNodeB和MME之间的S1-C接口发送Initial UE Message给MME。在此消息中， eNodeB将UE发送的NAS消息转发给MME（36。413），除此之外，在此消息中，还包括如下项： IE/Group Name Message Type eNB UE
 S1AP ID NAS-PDU TAI E-UTRAN CGI S-TMSI CSG Id GUMMEI Cell Access Mode Presence M M M M M O O O O RRC Establishment cause M 其中eNB UE S1AP ID 值表示在此eNodeB中UE的S1接口。MME侧将利用此标识来确定UE所对应的S1－C逻辑连接。
TAI值由PLMN IDentitity和TAC组成， 唯一表示了UE的Tracking Area。
MME接收到Initial UE Message后，进行网络和UE之间NAS层的安全认证过程。（参见另外的文章）。
NAS层的安全认证成功后，MME会向HSS发送Update Location Request消息，向HSS更新自己的位置信息。同时，MME向HSS请求用户的APN签约信息。包括缺省的APN设置，每个APN对应的PDN类型，缺省的EPS Bearer 的QOS设置等。
此后，MME就可以与SGW，PGW进行信令交互，为缺省的EPS Bearer建立用户面和控制面的GTP Tunnel 了（GTP隧道）。
对于每个PDN的连接，需要建立一个控制面的GTP Tunnel（GTP－C），包括MME和SGW之间的S11接口和SGW和PGW之间的S5接口。对于每个EPS Bearer，需要建立一个用户面的GTP Tunnel（GTP－U），包括eNodeB 和SGW之间的S1－U接口和SGW和PGW之间的S5接口。
在LTE中， GTP－U使用的版本号为1， 注册的UDP端口号为2152。GTP－C使用的版本号为2，注册的UDP端口号为2123。
# LTE Initial Attach 过程02-2
在GTP的头部中，有一个重要的字段，叫做隧道端点；MME分配相应的缺省EPSBearerID（即E；（1）用户的身份标识，如IMSI，MSISDN,；（2）用户接入网的一些信息，（E－UTRAN，U；（3）服务网络的信息，包括MCC,MNC等；（4）GTP－CTunnel的信息，包括MMEF；（5）S5/S8Interface的信息，包括协；（6）PDN的类
在GTP的头部中，有一个重要的字段，叫做隧道端点标识符（TEID），标识了对端的GTP-U或GTP-C协议中的隧道端点。由GTP隧道的接收端分配本地TEID值，供GTP隧道的发起方使用。通过GTP-C消息在隧道的两个端点间交换TEID（包含在FTEID内）值。通过IP地址，端口号，以及TEID值就可以唯一确定一个GTP的隧道。
MME分配相应的缺省EPS Bearer ID（即EBI），构造S11接口（控制面）上GTP－C Tunnel的MME端标识MME F－TEID （注意，此信令中只有S11上的控制面TEID，而不包含S1-U的用户面FTEID，S1－U的控制面终结在eNodeB和SGW之间，eNodeB的FTEID-U 在后面的Modify Bearer Request消息中发送）， 向GW发送Create Session Request消息。 在Create
 Session Request中，主要包含如下一些主要内容
（1） 用户的身份标识，如IMSI， MSISDN, MEI, ULI (User Location Information)等
（2） 用户接入网的一些信息，（E－UTRAN， UTRAN等）
（3） 服务网络的信息，包括MCC, MNC等。
（4） GTP－C Tunnel的信息， 包括MME F－TEID
（5） S5/S8 Interface 的信息，包括协议类型（GTP－C），PDN 的地址（包含在PDN F－TEID内）。（另外一个可能的协议类型就是PMIPV6）。
（6） PDN的类型（IPV4，IPV6或者IPV4、IPV6），APN，
（7） 将要建立的Default EPS Bearer的相关信息，包括EBI （EPS Bearer ID）, QoS， APN－AMBR等，以及用于切换时的Indication Header等。
Create Session Request
Flags: 72
010. .... = Version: 2
.... 1... = T: 1
Message Type: Create Session Request (32)
Message Length: 201
Tunnel Endpoint Identifier: 0 （SGW的TEID值，由于此时并没有建立GTP－C，因而取值为零）
Sequence Number: 7660
Spare: 45056
International Mobile Subscriber Identity (IMSI)。。
RAT Type :
IE Type: RAT Type (82)
IE Length: 1
000. .... = CR flag: 0
.... 0000 = Instance: 0
RAT Type: EUTRAN (6)
Fully Qualified Tunnel Endpoint Identifier (F-TEID) :
IE Type: Fully Qualified Tunnel Endpoint Identifier (F-TEID) (87)
IE Length: 9
000. .... = CR flag: 0
.... 0000 = Instance: 0
1... .... = V4 (True-IPV4 address field Exists,False-Doesn't Exist in F-TEID): True
.0.. .... = V6 (True-IPV6 address field Exists,False-Doesn't Exist in F-TEID): False ...0 1010 = Interface Type: S11 MME GTP-C interface (10)
TEID/GRE Key: 3300033 （TEID值是由接收端分配而由发送端使用）
F-TEID IPv4: 30.0.1.1 (30.0.1.1)
Fully Qualified Tunnel Endpoint Identifier (F-TEID) :
IE Type: Fully Qualified Tunnel Endpoint Identifier (F-TEID) (87)
IE Length: 9
000. .... = CR flag: 0
.... 0001 = Instance: 1
1... .... = V4 (True-IPV4 address field Exists,False-Doesn't Exist in F-TEID): True .0.. .... = V6 (True-IPV6 address field Exists,False-Doesn't Exist in F-TEID): False ...0 0111 = Interface Type: S5/S8 PGW GTP-C interface (7)
TEID/GRE Key: 0
F-TEID IPv4: 20.0.0.1 (20.0.0.1)
PDN Type ：
IE Type: PDN Type (99)
IE Length: 1
000. .... = CR flag: 0
.... 0000 = Instance: 0
.... .001 = PDN Type: IPv4 (1)
Selection Mode :
IE Type: Selection Mode (128)
IE Length: 1
000. .... = CR flag: 0
.... 0000 = Instance: 0
.... ..00 = Selection Mode: MS or network provided APN, subscribed verified (0)
PDN Address Allocation (PAA) :
IE Type: PDN Address Allocation (PAA) (79)
IE Length: 5
000. .... = CR flag: 0
.... 0000 = Instance: 0
.... .001 = PDN Type: IPv4 (1)
PDN IPv4: 0.0.0.0 (0.0.0.0)表示需要PGW分配IPV4 Address
Indication :
IE Type: Indication (77)
IE Length: 2
000. .... = CR flag: 0
.... 0000 = Instance: 0
0... .... = DAF (Dual Address Bearer Flag): False
.0.. .... = DTF (Direct Tunnel Flag): False
..0. .... = HI (Handover Indication): False
...0 .... = DFI (Direct Forwarding Indication): False
.... 0... = OI (Operation Indication): False
.... .0.. = ISRSI (Idle mode Signalling Reduction Supported Indication): False
.... ..0. = ISRAI (Idle mode Signalling Reduction Activation Indication): False
.... ...0 = SGWCI (SGW Change Indication): False
.... 0... = PT (Protocol Type): False
.... .0.. = TDI (Teardown Indication): False
.... ..0. = SI (Scope Indication): False
.... ...0 = MSV (MS Validated): False
Access Point Name (APN) :
IE Type: Access Point Name (APN) (71)
IE Length: 18
000. .... = CR flag: 0
.... 0000 = Instance: 0
APN (Access Point Name): apn-1.example.com
APN Restriction :
IE Type: APN Restriction (127)
IE Length: 1
000. .... = CR flag: 0
.... 0000 = Instance: 0
APN Restriction: 0
Aggregate Maximum Bit Rate (AMBR) :
IE Type: Aggregate Maximum Bit Rate (AMBR) (72)
IE Length: 8
000. .... = CR flag: 0
.... 0000 = Instance: 0
AMBR Uplink (Aggregate Maximum Bit Rate for Uplink): 655360000
AMBR Downlink(Aggregate Maximum Bit Rate for Downlink): 655360000
Bearer Context : [Grouped IE]
IE Type: Bearer Context (93)
IE Length: 31
000. .... = CR flag: 0
.... 0000 = Instance: 0
EPS Bearer ID (EBI) :
IE Type: EPS Bearer ID (EBI) (73)
IE Length: 1
000. .... = CR flag: 0
.... 0000 = Instance: 0
.... 0101 = EPS Bearer ID (EBI): 5
Bearer Level Quality of Service (Bearer QoS) :
IE Type: Bearer Level Quality of Service (Bearer QoS) (80)
IE Length: 22
000. .... = CR flag: 0
.... 0000 = Instance: 0
.... ...1 = PVI (Pre-emption Vulnerability): True
..00 00.. = PL (Priority Level): 0
.0.. .... = PCI (Pre-emption Capability): False
Label (QCI): 9
Maximum Bit Rate For Uplink: 65535000
Maximum Bit Rate For Downlink: 65535000
Guaranteed Bit Rate For Uplink: 0
Guaranteed Bit Rate For Downlink: 0
接收到MME发送的Create Session Request消息后，SGW会为S5接口上的GTP Tunnel创建SGW侧的标识，以供PGW侧发来的下行GTP Tunnel使用。由于S5接口上既包含有UE用户面的数据，也包含有控制面的数据。因此，需要建立GTP－C和GTP－U的Tunnel，SGW需要创建SGW GTP－C的FTEID 和SGW GTP－U的FTEID。
SGW向PGW发送Create Session Request消息， 包含上述 TEID信息和接收到MME的Create Session Request中的部分信息。
PGW为UE分配相应的IP地址，建立UE到PDN之间的路由。返回Create Session Response 给SGW。Create Session Response的内容包括有：分配的PDN Address，以及PGW TEID－C和PGW TEID－U等。 这样SGW和PGW之间的EPS Bearer就建立起来了。SGW分配SGW TEID-C 和SGW TEID－U，并将他们包含在返回给MME的Create Session Response中。Create
 Session Response中的GTP Header的TEID值取为SGW在Create Session Request中报上来的SGW FTEID－C。
GPRS Tunneling Protocol V2
Create Session Response
Flags: 72
010. .... = Version: 2
.... 1... = T: 1
Message Type: Create Session Response (33)
Message Length: 126
Tunnel Endpoint Identifier: 3300033
Sequence Number: 7660
Spare: 45056
Cause :
IE Type: Cause (2)
IE Length: 2
000. .... = CR flag: 0
.... 0000 = Instance: 0
Cause: Request accepted (16)
.... ...0 = Cause Source (CS: True-Error originated by remote node, False-Error originated by Node sending the Message): False
PDN Address Allocation (PAA) :
IE Type: PDN Address Allocation (PAA) (79)
IE Length: 5
000. .... = CR flag: 0
.... 0000 = Instance: 0
.... .001 = PDN Type: IPv4 (1)
PDN IPv4: 40.0.0.1 (40.0.0.1)为UE分配的IPV4地址
Fully Qualified Tunnel Endpoint Identifier (F-TEID) :
IE Type: Fully Qualified Tunnel Endpoint Identifier (F-TEID) (87)
IE Length: 9
000. .... = CR flag: 0
.... 0000 = Instance: 0
1... .... = V4 (True-IPV4 address field Exists,False-Doesn't Exist in F-TEID): True .0.. .... = V6 (True-IPV6 address field Exists,False-Doesn't Exist in F-TEID): False ...0 1011 = Interface Type: S11/S4 SGW GTP-C interface (11)
TEID/GRE Key: 1
F-TEID IPv4: 30.0.2.1 (30.0.2.1)
Fully Qualified Tunnel Endpoint Identifier (F-TEID) :
IE Type: Fully Qualified Tunnel Endpoint Identifier (F-TEID) (87)
IE Length: 9
000. .... = CR flag: 0
.... 0001 = Instance: 1
1... .... = V4 (True-IPV4 address field Exists,False-Doesn't Exist in F-TEID): True .0.. .... = V6 (True-IPV6 address field Exists,False-Doesn't Exist in F-TEID): False ...0 0111 = Interface Type: S5/S8 PGW GTP-C interface (7)
TEID/GRE Key: 1
F-TEID IPv4: 20.0.0.1 (20.0.0.1)
APN Restriction : （具体内容请参见29.274）
