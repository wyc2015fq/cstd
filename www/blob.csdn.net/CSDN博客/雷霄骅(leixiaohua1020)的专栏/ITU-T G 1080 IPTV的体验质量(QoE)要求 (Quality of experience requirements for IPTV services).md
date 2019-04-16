# ITU-T G.1080 IPTV的体验质量(QoE)要求 (Quality of experience requirements for IPTV services) - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月04日 16:46:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：54
个人分类：[IPTV](https://blog.csdn.net/leixiaohua1020/article/category/1638143)










IPTV的服务质量(QoE)要求 Quality of experience requirements for IPTV services

**Summary**

This Recommendation defines user requirements for Quality ofExperience for IPTV services. The QoE requirements are defined from an end userperspective and are agnostic to network deployment architectures and transportprotocols. The QoE requirements are specified as end-to-end and information isprovided on how they influence network transport and application layerbehaviour. QoE requirements for video, audio, text, graphics, control functionsand meta-data are provided. Compression coding schemes addressed in this Recommendation are examples,and detailed numeric values as performance targets, e.g. bit rate, packet lossrate, are also examples. The readers may appropriately choose or replace these parameters values in order to be consistent with therequirements of each IPTV service context to which they are intended to be applied.

**Keywords**

IPTV, QoE, QoS





# 1范围 (Scope)

This Recommendation defines userrequirements for Quality of Experience (QoE) for IPTV services. The QoErequirements are defined from an end-user perspective and are agnostic tonetwork deployment architectures and transport protocols. The QoE requirementsare specified for the end-to-end service and information is provided on howthey influence network transport and application layer behaviour. Compression coding schemesaddressed in this Recommendation are examples, and detailed numeric values givenas performance targets, e.g. bit rate, packet loss rate, are also examples. Thereaders mayappropriately choose or replace these parameters values in order to be consistent with therequirements of each IPTV service context to which they are intended to be applied.

# 2参考(References)

The following ITU-T Recommendations and other references containprovisions which, through reference in this text, constitute provisions of thisRecommendation. At the time of publication, the editions indicated were valid.All Recommendations and other references are subject to revision; users of thisRecommendation are therefore encouraged to investigate the possibility ofapplying the most recent edition of the Recommendations and other referenceslisted below. A list of the currently valid ITU-T Recommendations is publishedregularly. The reference to a document within this Recommendation does not giveit, as a stand-alone document, the status of a Recommendation.



[ITU-TE.800] ITU-TRecommendation E.800 (1994), *Terms and Definitions Related to Quality ofService and Network Performance Including Dependability*

[ITU-T F.700] ITU-TF.700 (2000), *Framework recommendation for multimedia services*,

[ITU-T H.262] ITU-TRecommendation H.262 (2000), *Informationtechnology - Generic coding of moving pictures and associated audioinformation: Video*

[ITU-TH.264] ITU-TRecommendation H.264 (2005), *Advanced video coding for generic audiovisualservices*

[ITU-T P.10/G.100] ITU-T Recommendation P.10/G.100, Amd 1 (2007), New*AppendixI, Definition of Quality of Experience (QoE)*

[ITU-T P.800] ITU-TRecommendation P.800 (1996), *Methods for Subjective Determination of TransmissionQuality*

[ITU-TT.140] ITU-TRecommendation T.140 (1998), *Protocol for Multimedia Application TextConversatio*n

[ITU-TY.1541] ITU-TRecommendation Y.1541 (2006), *Network Performance Objectives for IP-based Services*

[ITU-R BT.500-11] Recommendation ITU-R BT.500-11 (2002), *Methodology for the subjective assessment of the quality of televisionpictures*

[ITU-R BT.601-6] Recommendation ITU-R BT.601-6(2007), *Studio encoding parameters of digital television for standard 4:3and wide-screen 16:9 aspect ratios*

[ITU-RBT.1359-1] RecommendationITU-R BT.1359-1 (1998), * Relativetiming of sound and vision for broadcasting*

# 3定义(Definitions)

## 3.1 Termsdefined elsewhere

This Recommendation uses the followingterms defined elsewhere:

**3.1.1 Quality of experience (QoE) [ITU-TP.10/G.100]:** The overall acceptability of an application or service, as perceivedsubjectively by the end-user.

NOTE 1 – Quality ofExperience includes the complete end-to-end system effects (client, terminal,network, services infrastructure, etc.).

NOTE 2 – Overallacceptability may be influenced by user expectations and context.

## 3.2 Terms defined in this recommendation

This recommendation defines the followingterms:

**3.2.1 Channel zapping:** The act of quickly changingfrom one channel to another

**3.2.2 Group of Pictures:** The Group ofPictures (GOP) is a group of successive pictures within a MPEG-coded filmand/or video stream. Each MPEG-coded film and/or video stream consists ofsuccessive GOPs. From the MPEG pictures contained in it, the visible frames aregenerated.

**3.2.3 VoD Trick Modes:** Download and streamingvideo on demand (VoD) systems provide the user with a large subset of[VCR](http://en.wikipedia.org/wiki/Video_cassette_recorder)functionality including [pause](http://en.wikipedia.org/wiki/Pause), [fast forward](http://en.wikipedia.org/wiki/Fast_forward), fast [rewind](http://en.wikipedia.org/wiki/Rewind), slow forward, slow rewind, jump toprevious/future frame etc. These functions are usually referred to as"trick modes".

**3.2.4 Triple Play Services:** Services that include IPTV, VoIP, andInternet Access

**3.2.5 Clean Audio: **Audiotrack of an IPTV Service with background sounds removed.

# 4缩写和首字母缩略词(Abbreviationsand acronyms)

This Recommendation uses the following abbreviations and acronyms:

AAC Advanced Audio Coding 高级音频编码

AC-3 Dolby Digital Audio (Advanced Codec 3)AC-3杜比数字音频

ARQ Automatic Repeat reQuest 自动重发请求

ATIS Alliancefor Telecommunications Industry Solutions

ATSC Advanced Television Systems Committee 先进电视系统委员会

A/V Audio Video 音频视频

AVC Advanced Video Codec 高级视频编解码器

AVS Audio and Video coding Standard (Chinese) AVS音视频编码标准（中国）

BER Bit Error Rate 误比特率

BML Broadcast Markup Language

CBR Constant Bit Rate

CPU Central Processing Unit

DCT Discrete Cosine Transform

DSCQS Double Stimulus Continuous Quality Scale

DSL Digital Subscriber Line

DVB Digital Video Broadcast

DVD Digital Video Disk

ECG Electronic Content Guide

EPG Electronic Program Guide

FEC ForwardError Correction

FFW FastForWard

fps framesper second

GOP Groupof Pictures

GWR GateWayRouter

HDTV HighDefinition TeleVision

HG HomeGateway

HTML HyperTextMarkup Language

IETF TheInternet Engineering Task Force

IGMP InternetGroup Management Protocol

IGP InteractiveGateway Protocol

IPG InteractiveProgram Guide

ITU-R [InternationalTelecommunication Union](http://rds.yahoo.com/_ylt=A0oGklmxx6lGiyMAb11XNyoA;_ylu=X3oDMTE4NDNrazgxBHNlYwNzcgRwb3MDMgRjb2xvA3NrMQR2dGlkA0Y5NzRfODYEbANXUzE-/SIG=117sc7iep/EXP=1185618225/**http%3A/www.itu.int/) RadiocommunicationSector

ITU-T [InternationalTelecommunication Union](http://rds.yahoo.com/_ylt=A0oGklmxx6lGiyMAb11XNyoA;_ylu=X3oDMTE4NDNrazgxBHNlYwNzcgRwb3MDMgRjb2xvA3NrMQR2dGlkA0Y5NzRfODYEbANXUzE-/SIG=117sc7iep/EXP=1185618225/**http%3A/www.itu.int/) TelecommunicationStandardization Sector

IP InternetProtocol

IPTV InternetProtocol TeleVision

MOS MeanOpinion Score

MP MeasuredPoint

MP-3 MPEG-1Audio Layer 3

MPEG MovingPictures Expert Group

MPLS MultiProtocolLabel Switching

NICAM NearInstantaneous Companded Audio Multiplex

NTSC NationalTelevision Standards Committee

PAL PhaseAlternating Line

PC PersonalComputer

PDV PacketDelay Variation

PHB Per-Hop Behaviour

PLR Packet Loss Ratio

PTD PacketTransfer Delay

QoE Qualityof Experience

QoS Qualityof Service

RFC RequestFor Comments

SDH SynchronousDigital Hierarchy

SDTV StandardDefinition TeleVision

SECAM SEquentielCouleur A Mémoire, French for "Sequential Color with Memory"

SMPTE Societyof Motion Picture and Television Engineers

SONET SynchronousOptical NETwork

STB Set-TopBox

TS TransportStream

VBR VariableBit Rate

VoD Video on Demand

VoIP Voice over IP

VCR Video Cassette Recorder

# 5介绍QoE (Introduction to QoE)

QoE is defined in [ITU-T P.10/G.100] in Appendix I as the overallacceptability of an application or service, as perceived subjectively by theend-user. It includes the complete end-to-end system effects (client, terminal,network, services infrastructure, etc) and may be influenced by userexpectations and context. Hence in principle,QoE is measured subjectively by the end-user and may differ from oneuser to the other. However, it is often estimated using objective measurements.

Differencesin perceptual acuity and preference mean that QoE judgments obtained fromdifferent people may vary. Therefore,measurements of QoE are generally made using group data. Where the necessary studies have been done tocalibrate the relationship with QoE, it may also be estimated using objectivemeasurements.

A numberof system performance characteristics contribute to QoE of the mediastream. For example, the codec and theencoding bit rate used, media resolution in the source and at the display,corruption or loss of information, and delay. Interactions among video content,the codec and bit rate used, and the specific bits corrupted and/or packetslost contribute to a high variability in the perceived quality of the videooutput.

There are additional factors that can influence the viewer's response. Some of these affect the perception ofquality, such as the context of the judgment (a particular image will be ratedone way in the context of standard definition TV (SDTV), another in the contextof high definition TV (HDTV), and still differently in the context of a videoclip on Internet), cultural background, motivation, attention related factors,emotional state, and so on. (Directevaluations of QoE are designed to exclude these factors, since they are notgenerally under the control of a network operator and so do not contribute toequipment requirements.)

Other factors influence the viewer's judgment of acceptability. These include things like previous experiencewith the specific communication mode or related modes (for instance, experiencewith DVD quality will influence how acceptable one finds IPTV or VoD), how muchone is paying for the service, and what special benefits the service provides(mobility, time independence, exceptionally large program library), and soon.

Acceptability is not equivalent toQoE. A low resolution video image willhave a lower QoE than a high resolution image, but it may be completelyacceptable for certain applications and services, depending on the end device,the physical size of the display, and the purpose for which it is being used.

Figure 5-1 shows factors contributing to QoE. These factors areorganised as those related to quality of service and those that can beclassified as human components.

QoE for video is often measured via carefully controlled subjectivetests [b ITU-R BT.500-11], [ITU-T P.800], where video samples are played toviewers, who are asked to rate them on a scale, typically consisting of fivepoints. The rating assigned to each case are averaged together to yield a meanrating or mean opinion score (MOS).

Quality of service (QoS, see ITU-T E.800) involves the totality ofcharacteristics of a telecommunications service that bear on its ability tosatisfy stated and implied needs of the user of the service. In general,network performance is a major component of QoS, and so network mechanisms forQoS are an important consideration. QoS mechanisms include any mechanism thatcontributes to improvement of the overall performance of the system and henceto improving end-user experience. QoS mechanisms can be implemented atdifferent levels. For example at the network level it includes trafficmanagement mechanisms such as buffering and scheduling employed todifferentiate between traffic belonging to different applications. QoSmechanisms at levels other than the transport include loss concealment,application forward error correction (FEC), etc.

Related to QoS are the QoS performance parameters. Similar to theQoS mechanisms QoS parameters can be defined for different layers. At thenetwork layer those parameters usually include packet loss rate, delay anddelay variation.

![](https://img-blog.csdn.net/20131104153707843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Figure 5-1/G.1080 - QoE Dimension

Note that in this Figure, service billingrelates to the “value for money” perceived by the user for the particularservice.

Typicallythere will be multiple service level performance (QoS) metrics that impactoverall QoE. Thereare a number of service level performance characteristics (that is, objectiveparameters of service performance such as encoding bit rate, packet loss, delay,availability, etc.) that affect QoE. Ingeneral, these are correlated with QoE as measured by the MOS. The relation between QoE andservice performance (QoS) metrics is typically estimated empirically. Having identified the QoE/QoS relationship,it can be used in two ways:
- Given a QoS measurement, one could in principle predict the expected QoE for a user, with appropriate assumptions.
- Given a target QoE for a user, one could in principle deduce the net required service layer performance, with appropriate assumptions.

Toensure that the appropriate service quality is delivered, QoE targets should beestablished for each service and be included early on in system design andengineering processes where they are translated into objective service levelperformance metrics. Quality of Experience will be an important factor in themarketplace success of triple-play services and is expected to be a keydifferentiator with respect to competing service offerings. Subscribers to network services do not carehow service quality is achieved. Whatmatters to them is how well a service meets their expectations foreffectiveness, operability, availability, and ease of use.

# 6视频和音频的QoE (QoE for video and audio)

QoE requirements for video and audiomay be based on QoE scales such as the Mean Opinion Score (MOS) and DoubleStimulus Continuous Quality Scale (DSCQS) [b ITU-RBT.500-11]. However,conducting subjective tests is difficult because they are time consuming andexpensive. Moreover, reliable objective quality assessment methods have notbeen established for transmitted video and audio. Therefore, this clauseprovides provisional QoE requirements on the basis of the objective parametersthat are correlated to the subjective QoE.

This Recommendation addresses QoEtargets and shows how to express QoE requirements in the context of numericalparameters such as bit rate or packet loss rate. The process of determining QoEperformance targets must consider anumber of issues for example: the purpose of the IPTV service, QoE level of thecurrent broadcasting systems (which sets user expectation), compression codingscheme to be used for the service, content characteristics, content provider requirements,customer satisfactions. While the requirement values shown in the tables in thisclause are NOT generically applicable to any specific or all IPTV services,they are to be understood as provisional values which are subject to change. Readersof this Recommendation are invited to replace the numerical values shown in the tables in this clause withappropriate ones that conform to the requirements required by a specific IPTV service context.

## 6.1 媒体压缩和同步要求 Requirements for media compression and synchronization

One of the main components of QoE for video and audio is digitization andcompression of video and audio source materials and the various settings andparameters selected. Since video compression schemes such as those defined bythe Moving Picture Expert Group (MPEG) are not lossless and an identical copyof the original source material cannot be recovered, there are potentiallynegative impacts on video picture quality and therefore on viewer QoE. The mainfactors influencing video QoE at the application layer due to compression are:



• Quality of source material

– the quality of the delivered mediadepends on the quality of the source material.

• The baseline quality (no network impairments) of the codec.

Note: A partial list of video codecs is provided in Appendix II.



• Resolution

– Some systems reduce the horizontal resolution to achieve the target bitrates, for example in standard definition television (SDTV) the resolutionmaybe reduced to ‘Half’ or “Three Quarters” which produces a less sharp picturethan ‘Full’ resolution



• Bit rate

– During periods of high complexity (entropy) compression may leave visibleartefacts if the bit rate is not sufficient



• Application layer video encoding - constant bit rate (CBR) vs. variable bitrate (VBR) at the encoder output

– Video encoding is naturally variable bit rate but to simplify networkengineering for Telco delivery systems, the video encoders are set to provide aconstant bit rate (as averaged over some specified time period in the order ofseconds).

– VBR streams such as those used in DVD encoding have constant qualitysince the bit rate is allowed to vary to accommodate varying complexity of thesource material

– CBR streams have variable quality since there may be times when the bitrate is insufficient to accommodate the video complexity but CBR steams allowstraightforward traffic engineering and system design



• Group of Pictures (GOP) structure

–Shorter GOPs improve quality in terms of performance of random accessibilityand error recovery, but reduce the maximum compression ratio.

–Longer GOPs improve maximum compression ratio, but increase channel change timeand the amount of damage a lost packet will cause.

–Dynamic GOPs can be used to better handle scene changes and other effects butare not always implemented on STBs. In addition, dynamic GOPs can causevariability in zapping latency and may complicate mechanisms intended toincrease zapping speed.

• Motion Vector Search Range

- Wider searches provideimproved quality but at increased complexity and encoder delay

- Large search ranges arerequired for high motion content such as sports

• Rate Control

- Mode decisions greatly affectthe bit rate

- Proprietary schemes arecommonly used to gain competitive advantage

• Pre-processing (such as noise reduction)

– Usually proprietary and non-standard butcan improve bit rate / quality trade-off

• Tandem encoding and rate shaping (e.g. digital turnaround)



*Video Compression Artefact Examples*

Figure 6-１ illustratesseveral kinds of compression artefacts that are largely due to insufficientbits allocated resulting in too coarse quantization of DCT coefficients ormotion vectors and/or otherwise poor motion estimation. Additionional detailsof compression artefacts may be found in [b NTIA264].

![](https://img-blog.csdn.net/20131104153712250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



Figure 6-1/G.1080 - Compression Artefacts[1]]



Similarly, there are similar parameter implications on the audio side.

Note: A partial list of audio codecs is provided in Appendix II



In additionto the separate audio and video application layer impairments, thesynchronization between audio and video components must be maintained to ensuresatisfactory QoE. There has been a great deal of research on A/Vsynchronization requirements in video conferencing and analogue broadcastsystems and specifications in such bodies as ITU-R [b ITU-RBT.1359-1].

Because audio that appears before video is very unnatural (sound takeslonger to propagate than light so sound lagging visual is normal) some bodiesspecifying television specific A/V synchronization have recommended tightertolerances than typically used for video conferencing applications.



Recommended provisional minimum engineering objectives for applicationlayer, data plane parameters are presented in the following sub- clauses forvarious video services.



Note: Appendix III provides supplementary information related to thefollowing sub-clauses.



### 6.1.1 标清电视：一般最低要求 Standard definition TV (SDTV):General minimum objectives



Table 6-1 provides provisional video application layer performanceobjectives at the MPEG elementary stream level, prior to IP encapsulation forbroadcast SDTV (480i / 576i). The objectives for audio elementary stream bitrates are additionally specified below.



**Table 6-1/G.1080 - Provisional Application LayerPerformance requirements for Standard Definition Broadcast Program Sources**
|**Video Codec standard****(non-inclusive list)**|**Minimum Bit Rate (video elementary stream level)**|**Pre-processing Enabled**|
|----|----|----|
|H.262 - Main profile at Main level (MP@ML)|2.5 Mbit/s CBR|Yes (if available)|
|H.264 (Main profile at Level 3.0)|1.75 Mbit/s CBR|Yes (if available)|
|SMPTE 421M|1.75 Mbit/s CBR|Yes (if available)|
|AVS|1.75 Mbit/s CBR|Yes (if available)|



Table 6-2 provides provisional audio application layer performance requirementsfor standard definition audio sources.



**Table 6-2/G.1080 - Provisional Audio Application LayerPerformance requirements for Standard Definition Sources**
|**Audio Codec Standard****(Non-inclusive list)**|**Number of Channels**|**Minimum Bit Rate (audio elementary stream level, in kbit/s)**|
|----|----|----|
|MPEG-1 Audio Layer II|Mono or stereo|128 for stereo|
|Dolby Digital (AC-3)|5.1 if available, elseleft/right stereo pair|384 for 5.1ch / 128 for stereo|
|AAC|Stereo|96 for stereo|
|MPEG-1 Audio Layer III (MP3)|Stereo|128|
|MPEG-2 Audio Layer III (MP3)|Stereo|For further study|



In general, audio codecs chosen should align with industry standards in thegeography of deployment to ensure maximum compatibility with consumerreceivers. Bit rates should be aligned with original source material qualityand transcoding between formats should be avoided if possible.



Table 6-3 provides provisional audio-video synchronization requirements.



**Table 6-3/G.1080 - Provisional SDTV Audio – VideoSynchronization Requirement**

|Audio – VideoSynchronization|Audio Lead Video|Audio Lag Video|
|----|----|----|
|15 ms maximum|45 ms maximum| |




Inconsistent loudness levels between channels can negatively impact QoE. Itis recommended that equipment be used in the service provider head-end toensure similar loudness levels across the range of channels provided to theuser.



Another audio quality issue beyond the scope of this document is thedynamic range compression for links between the STB and TV.

### 6.1.2 标清电视：点播和优质内容的要求 Standard definition (SD) TV: VoD and PremiumContent Objectives

Video on demand (VoD) and other premium content such as pay per view instandard definition format will have similar application layer performance factorsas regular broadcast materials. However, subscriber expectation may be higherbecause of additional fees paid to access the content and comparison toalternative delivery options. In the case of VoD, users may compare to VoDmaterials delivered over digital cable systems or even contained in DVDs.



Table 6-4 provides recommended video encoding bit rates for standarddefinition, VoD and other premium content.





**Table 6-4/G.1080 - Provisional ApplicationLayer Performance requirements for H.262 Standard Definition VoD and PremiumProgram Sources**
|**Video Codec standard****(Non-inclusive list)**|**Minimum Bit Rate (video elementary stream level)**|**Pre-processing Enabled**|
|----|----|----|
|H.262 - Main profile at Main level (MP@ML)|3.18 Mbit/s CBR|Yes (if available)|
|H.264 (Main profile at Level 3)|2.1 Mbit/s CBR|Yes (if available)|
|SMPTE 421M|2.1 Mbit/s CBR|Yes (if available)|
|AVS|2.1 Mbits/s CBR|Yes (if available)|



Table 6-5 provides provisional recommended audio codec bit rates for VoDand premium content.



**Table 6-5/G.1080 - Provisional Audio Application LayerPerformance for VoD and Premium Standard Definition sources**
|**Audio Codec Standard****(non-inclusive list)**|**Number of Channels**|**Minimum Bit Rate (audio elementary stream level, in kbit/s)**|
|----|----|----|
|Dolby Digital (AC-3)|5.1 if available, elseleft/right stereo pair|384 for 5.1ch / 192 for stereo|
|AAC|5.1 if available, elseleft/right stereo pair|384 for 5.1ch / 192 for stereo|



### 6.1.3 高清电视 High definition TV (HDTV):



Table 6-6 provides provisional video application layer performanceobjectives for broadcast HDTV (720p / 1080i).



**Table 6-6/G.1080 - Provisional Application LayerPerformance requirements for High Definition (HD) Broadcast Program Sources**
|**Video Codec standard****(non-inclusive list)**|**Minimum Bit Rate (video elementary stream level)**|**Pre-processing Enabled**|
|----|----|----|
|H.262 - Main profile at Main level (MP@ML)|15 Mbit/s CBR|Yes (if available)|
|H.264 (Main profile at Level 4)|10 Mbit/s CBR|Yes (if available)|
|SMPTE 421M|10 Mbit/s CBR|Yes (if available)|
|AVS|10 Mbits/s CBR|Yes (if available)|



Table 6-7 provides provisional audio application layer performance requirementsfor high definition audio sources.



**Table 6-7/G.1080 - Provisional MinimumAudio Application Layer Performance requirements for High Definition Sources**
|**Audio Codec standard****(non-inclusive list)**|**Number of Channels**|**Minimum Bit Rate****(Audio elementary stream level, in kbit/s)**|
|----|----|----|
|MPEG-1 Audio Layer II|Mono or stereo|128 for stereo|
|Dolby Digital (AC-3)|5.1 if available, elseleft/right stereo pair|384 for 5.1ch /128 for stereo|
|AAC|5.1 if available, else left/right stereo pair|384 for 5.1ch /128 for stereo|
|MPEG-1/2 Audio Layer III(MP3)|Stereo|128|



## 6.2 网络传输性能的影响 Impactof network transmission on performance

Key criteria for network transmission include loss, latency and jitter (seeAppendix I). In general, reasonable end-to-end delay and jitter values are notproblematic due to STB de-jitter buffers, provided the de-jitter buffer size isprovisioned to match network and video element performance. Video streamshowever are highly sensitive to information loss and the QoE impact is in turncorrelated to a number of variables including:
- Highly dependent on type of data lost

-System information and header lossesproduce different impairments

-Lost data from I and P frames producedifferent impairments than B frame packet losses due to temporal errorpropagation
- Dependent on codec used
- Dependent on MPEG transport stream packetization used
- Loss distance and loss profile
- With high encoding bit rates, the stream is more vulnerable to packet loss impairments

-For the same packet loss ratio,impairments due to loss on a higher rate video stream occur more frequently(i.e. there are more visible errors per unit time) simply because there aremore packets per second transmitted and each one has the same probability to beaffected.
- Decoder concealment algorithms can mitigate perceptual impact of some losses.



An error or sequence of errors in a video bit stream can cause effectsranging from no noticeable audio or video impact to the user to complete lossof the video or audio signal depending on what was lost and the robustness ofthe implementation.



Appendix IV provides additional information as to how transmissionimpairments can affect quality.





The video application should be able to operate normally in the presence ofnormal operational defects. One such normal operational consideration is theoperation of protection switching mechanisms in the network. SONET/SDHprotection switching mechanisms may result in a potential packet loss duration inthe order of, for example, 50ms. For some other protection mechanisms (e.g.MPLS fast reroute, fast IGP convergence) the potential packet loss duration canbe longer, for example, on the order of 250ms. Service providers are encouragedto add mechanisms to minimize or eliminate the visible effect of suchprotection mechanisms as these events cascade to a large number of subscribers.



Considering some other protection mechanisms the potential packet lossduration can be longer. For example, a complete reconvergence of the IP (IGP) routingtable would imply potential packet loss bursts in the order of 30 sec. An IPTVsystem would not be expected to maintain normal service through such an event.Such events can be considered a service outage rather than an in servicequality defect.



The goal is to minimize visible artefacts to as few as possible using acombination of network performance requirements, loss recovery mechanisms (e.g.FEC, interleaver) and loss mitigation mechanisms (e.g. decoder lossconcealment).



# 7文字和显示的QoE (QoE for text and graphics)

Informationin this clause is taken from [ITU-T F.700].

## 7.1 Media component text

### 7.1.1 定义 Definition

The media component text allows for the capture and representation of information,its transfer from originating user(s) to destination user(s), its presentation to human user(s), processing, filing and retrieval.

### 7.1.2 描述 Description

#### 7.1.2.1 General description

Textis a representation medium consisting of formatted characters. It is stored andtransmitted as a sequence of codes. Although it may be displayed on the samescreen as video and still pictures, it requires decoding into specific fontsfor presentation to the user, whether on the screen or on paper. The input isthrough a keyboard. The output may be presented by a printer or on a screen.

Thefollowing levels of quality are defined:

T0: minimum quality, basic alphabet andpunctuation, no formatting or choice of font;

T0*bis*: videotex quality, basic alphabet andpunctuation, basic graphic character set, no formatting or choice of font;

T1: Usable text conversation qualitycharacterized by:

·Font support for [b_ISO-10646] Languagearea Latin-1 plus the target language area for the implementation.

·No more than 1 corrupted,dropped or marked missing character per 100.

·Delay from character input inthe transmitter to display in the receiver shorter than 2 s.

T2: Good text conversation qualitycharacterized by:

·Font support for all charactersin [b_ISO-10646].

·No more than 1 corrupted,dropped or marked missing character per 500.

·Delay from character input inthe transmitter to display in the receiver shorter than 1 s.

#### 7.1.2.2 Additional facilities

Theuser may be given control over text through editing and presentation functions.He may also be able to insert graphics, still pictures or animated pictures withinthe text.

#### 7.1.2.3 Requirements forvarious audiovisual services

Whentext is for the support of conversational services, the timing aspects of textentry and display are critical. Text may be transmitted and displayed in nearreal time, as text is entered. It may also be transmitted only after specificend-of-sentence action or on a specific send request. In a conversation betweentwo users, the near real-time conversation is important for optimized benefitof the conversation. For multi-user conferences, a sentence based transmissionmay be more relevant in an open discussion, while for a subtitled speech, thereal-time text transmission is preferred.

Forretrieval services, it may be accepted to transmit and display a whole page oftext in one operation.

Forconversation, editing may be reduced to "new line", "erase lastcharacter", while the editing for information retrieval should contain apossibility to replace text anywhere on the page and add various formattingeffects to any part of text. Annotations that stand out distinctly are alsodesirable.

The levels of text qualityrequired for various services are the following (marked by X):

**Table 7-1/G.1080 - Levels of TextRequirements**

|Service|Quality level| | | |
|----|----|----|----|----|
|T0|T0 *bis*|T1|T2| |
|Telex|X||||
|Videotex||X|||
|Text telephony|||X|X|
|Total Conversation||||X|
|Messaging services|||X|X|
|Retrieval services|||X|X|




### 7.1.3 质量方面 Quality Aspects

Thequality of text depends mainly upon the capabilities for formatting and usingdifferent types of fonts and special characters. When no error correction ismade, for instance in conversation, text quality is also measured in terms ofcorrupted characters, dropped characters and characters replaced by the missingtext marker [ITU-T T.140].

### 7.1.4 Intercommunication

Thecharacters with their formatting may be decoded and assembled into bit mapswhich can then be handled as still pictures, e.g. as facsimile pages.

## 7.2 Media component graphics

### 7.2.1 定义 Definition

Themedia component graphics allows for the capture and representation of information,its transfer from originating user(s) to destination user(s), its presentation to human user(s), processing, filing and retrieval. This media component allowsgraphic pictures to be captured and transmitted as geometrical objects whosepositions, shapes and colours are coded so that they can be reproduced in adistant terminal.

### 7.2.2 描述 Description

#### 7.2.2.1 General description

Graphicsis a representation medium consisting of geometrical objects featured by theirpositions, shapes and colours. It is stored and transmitted as a set of codesand parameters. Although it may be displayed on the same screen as video andstill pictures, it requires decoding into specific geometrical figures forpresentation to the user, whether on a screen or on paper.

Theinput may be through a graphics tablet, an electronic pencil, some other two‑dimensionaltransducer or dedicated graphic software on a microcomputer or workstation. Theoutput device may be a printer or a screen.

### 7.2.3 Quality aspects

Theintrinsic quality of the graphic depends on the number and the complexity ofthe objects that can be generated, the precision of their dimensions andpositions, the number of possible colours. The overall quality perceived by theuser depends also on the resolution of the input and output systems.

### 7.2.4 Intercommunication

Thegraphic objects may be decoded and assembled into bit maps which can then behandled as still pictures.

# 8控制功能的QoE (QoE for control functions)

## 8.1 频道切换时间的QoE要求 QoE requirements for channel zapping time

Channelzapping time (channelswitching time) hasstrong relationship with end user experience of service quality. Generally, itis primarily determined by the time required to have a proper frame at the STBto start decode processing for the new channel. Channel zapping requests canoccur when

-Meta datarequest in EPG or IPG

-Random selecting channel by entering channel numberusing remote control

-ChannelUp/Down button using remote control

-Channel Up/Down button using STB front panel

-Selecting a channel on IPG application menu

-Powering on STB/TV and tuning to initial channelassigned by IPG

Asa QoEparameter, channel zapping time can be described by three components: IGMPdelay, buffering delay, and decoding delay, as shown in the Figure below (notethat the timings are not necessarily to scale).

![](https://img-blog.csdn.net/20131104153715953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**Figure 8-1/G.1080 - Components that contributeto Channel Zapping Time**



### 8.1.1 频道切换时间的分类 Classification of channel zapping time

**8.1.1.1 **IGMP延时 **IGMP delay**

A channel zaprequest is triggered by a channel change which is mapped by the STB to amulticast group address carried in the IGMP message. The IGMP message, whichincludes join message, is sent to the homegate (HG). The HG, playing an IGMPproxy role, will process the IGMP message and send an IGMP request to theGateway Router (GWR). After the IGMP message is sent towards the rendezvouspoint by the GWR, the corresponding channel data should be delivered to the endpoint at some point. The time to get the content data after sending the firstIGMP message is called the IGMP delay.

**8.1.1.2 缓冲延时 Buffering delay**

While the STB receives IPTV multicasttraffic, it stacks the packets in a buffer. Buffering delay is the time betweenthe arrival of the first multicast traffic in the buffer and when the STB hassufficient data for playing to the screen.

**8.1.1.3 **解码延时 **Decoding delay**

After STB starts to receive andbuffers multicast stream, decoding delay processes buffered data and renderthem to TV screen. This type of delay includes both codec decoding delay, whichintends to program-specific information frames in order to decide the targetchannel and I-frame acquisition delay, which is for reduction of bandwidthrequired for digital video transmission.

### 8.1.2 频道切换时间的要求 Requirements for Channel Zapping Time

One of the key elements involved in validating Qualityof Experience (QoE) in IPTV service is how quickly users can change TVchannels, which is often referred to as Channel Zapping Time. However, theexplicit relation between Channel Zapping Time and the user perceived quality as expressed asa Mean Opinion Score (MOS) is still under study [b_CZT_TNO].

## 8.2 视频点播控制功能的QoE要求 QoE requirements VoD trick mode

Video on Demand (VoD) trick mode provides VCR-likefeatures in VoD services. When a subscriber desires a video content through STB, the subscriber accesses the video content from the EPG which supports thecontents-search engine to help access of content information. To guarantee VCR-like flexibility, this mode provides the trick ability to handle pause,play, rewind, fast forward, and stop entries for these control features.



### 8.2.1 控制的延迟 Trick Latency

Correspondently,each control function (video selection, play, pause, rewind, FFW, stop) has itsown delay. QoE metrics for VoD transaction quality are expressed by thefollowing indicators:

-Videoselection process delay: Timing period from the time when the subject isselected to the time when content is displayed.

-Play Delay:Timing period from the time when the Play entry was selected to the time thecontent is displayed.

-Stop Delay:Timing period from the time when the Stop play video entry was selected to thetime the content is stopped playing as indicated by video content display.

-Rewind Delay:Timing period from the time when the Rewind video entry was selected to thetime the rewind action is executed as indicated on display device.

-Pause Delay: Timingperiod from the time when the Pause video entry was selected to the time thepause action is executed as indicated on display device.

-FFW Delay:Timing period from the time when the Fast Forward video entry was selected tothe time the FFW action is executed as indicated on display device.



### 8.2.2 控制的QoE要求 Requirements for VoD Trick Features

From a QoE perspective, trick feature latency is oneof the most important issues to guarantee satisfaction of subscribers. As eachtrick feature latency directly affects QoE, the latency is required to besufficiently low to meet user’s requirement for QoE relating to VoD trickfeatures.

# 9其他IPTV服务的QoE (QoE for other IPTV services)

## 9.1 元数据的QoE要求 QoE requirements for Metadata

Figure 9-1 shows the componentsof Metadata.



![](https://img-blog.csdn.net/20131104153720640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





**Figure 9-1/G.1080 - Components of Metadata**



(1) Availability

High availability is recommended to be ensured in transmitting themetadata on network.

(2) Data size

Metadata is recommended to be transported in such a way that the size ofthe transported data would be sufficiently small, relative to such factors asthe number of the total services, the number of the contents, and networkbandwidth.

(3) Correctness

The service provider should ensure themetadata tagged to a particular content is correct.

An example to illustrate the importance ofmetadata is the correctness of “rating” of content. The correct rating oncontent is directly related to what the customer expects. An incorrect parentalrating e.g. a “family” rating for an adult movie can have serious implicationsfor the customer experience and business of the service provider.

### 9.1.1 EPG的QoE要求 QoE requirements for EPG

The following items are recommended to be considered aspart of the definition of QoE for IPTV services.

(1) User-friendliness

EPG user interface is recommended to be designed for ease of use.

(2) Response time to display EPG page　

The response time - the interval from pushing the EPG button of remotecontrol to the display of the EPG page - is recommended to sufficiently short.

## 9.2 浏览器的QoE要求 QoE requirements for Browser

If abrowser, such as those for BML or HTML, is used to provide the user aninteractive content from the service provider, the following points arerecommended to be taken into account.

(1) Characteristics of a television set

The IPTV QoErequirements on browsers are recommended to take intoaccount that the behavioural patterns and expectations of television userstypically differ from those of PC users.

Moreover the differences in the capacities oftypical TVs (and STBs), on one hand, and PCs, on the other, should be takeninto account. For example, as the CPUperformance of a television set is usually inferior to that of PC, the contentsdesigned for PC-use does not necessarily work in the TV environment, making itnecessary to set up QoE measures taking into account the difference in CPUperformance between PC and television. It should be stressed that the browseron an IPTV service may not have the same capacity as the browser on a PC has.

(2) TV-like display

Some features of TV-like display is recommended to be considered necessary for browser QoE, for such are commonly imposedby content providers. Examples are:

- Overlay function,

-Consistency of displayed pictures across terminals.

(3) Character size

The character size is recommended to be sufficientlylarge.

(4) Navigation

The navigation function is recommended to be considered forincreasing the level of convenience and operability.

(5) Cookie

The use of Cookies is recommended to be done with carebecause of the possible limitation on the nonvolatile memory capacity of theterminal. The number, the size and the expiration date of cookies may need tobe clearly specified.

## 9.3 内容导航的QoE要求 QoE requirements for Content Navigation

Contentnavigation is defined as functions for contents discovery and selection. So,content navigation is provided byvarious methods such as direct channel selection, EPG, recommendation. QoErequirements according to navigation methods are described in the followingsections.

### 9.3.1 内容导航通过直接的选择，通过向上/向下按钮 Contents navigation by directchannel selection, by up/down button

Ease ofcontents selection, especially under numerous contents condition, isrecommended to be considered. For that sake, the elapsed time for selectingcontents and subjective evaluations of ease of use (e.g. MOS) are recommendedto be considered.

### 9.3.2 内容导航通过EPG/ECG Contents navigation by EPG/ECG

EPG/ECG isone of the most useful ways of contents navigation. The elapsed time fordiscovering and selecting contents, and subjective evaluations of ease of use(e.g. MOS) are recommended to be considered.

### 9.3.3 内容导航通过内容推荐 Contents navigation by contents recommendation

Effectivecontents recommendation is useful for users. For example, IPTV service providerrecommends contents to the user according to his/her preferences. And, contentsrecommendation from his/her friends is also effective.

Forcontents recommendation, recommendation accuracy and security of personalinformation are recommended to be considered. If the recommended contentsinclude many preferring contents, the QoE of recommendation would be high. Obviously,the security of personal information affects QoE. And communication functionsuch as obtaining the 3rd parties’ metadata is recommended to be considered.

# 10可达性的要求 (Accessibility requirements)

The intentof thisclause is to capture specific performance requirements for IPTV servicesrelated to accessibility. This is for further study, but will encompass thefollowing areas:

·Audio quality (including the provision of clean audio)

·Video quality (including sufficient frame rate and resolution for signlanguage, lip reading etc [b_ITU-T HSup1])

·Audio/video synchronisation



# 11安全性考虑 (Security considerations)

Securityaspects have not been addressed in this Recommendation.








