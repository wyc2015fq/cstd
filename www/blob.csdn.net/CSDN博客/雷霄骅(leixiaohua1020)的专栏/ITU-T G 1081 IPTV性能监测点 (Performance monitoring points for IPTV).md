# ITU-T G.1081 IPTV性能监测点 (Performance monitoring points for IPTV) - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月04日 16:39:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：45









ITU-T建议书 G.1081

IPTV性能监测点 Performance monitoring points for IPTV

**Summary**

Successful deployment of IPTV servicesrequires performance parameters to be monitored at a number of different pointsin the complete end to end chain, including the customer premises, keyaggregation points and at interconnect points between disparate and serviceprovider network domains. This Recommendation defines five monitoring pointswhere such performance measurements can be made.

**Keywords**

IPTV,performance, QoS, QoE, monitoring points



# 1.范围 (Scope)

ThisRecommendation defines performance monitoring points which will allow theservice provider/network operator to monitor the performance of the completeIPTV service delivery to the end-user.

Successful deployment of IPTV servicesrequires performance monitoring at the customer premise (e.g. set-top-box), keyaggregation points (e.g DSL Access Multiplexer DSLAM) or cable modemtermination system (CMTS) and at interconnect points between disparate networkdomains and service provider domain boundaries. Performance monitoring can helpin:

·Finding errors in an end-to-endsystem (system debugging)

·Checking the resourceutilization and the work load of system components

·Comparing values (metrics)regarding performance of different system deployments

·Providing a base for modellingthe system

·Identifying system bottlenecks

·Optimizing IPTV networkdeployment

·Ensuring that systemperformance does not degrade with time.

The goal of performance monitoring is toprovide end-users of IPTV services with a high user experience by identifying,localizing and quantifying service and network issues.

# 2.参考 (References)

Thefollowing ITU-T Recommendations and other references contain provisions, which,through reference in this text, constitute provisions of this Recommendation.At the time of publication, the editions indicated were valid. AllRecommendations and other references are subject to revision; users of this Recommendationare therefore encouraged to investigate the possibility of applying the mostrecent edition of the Recommendations and other references listed below. A listof the currently valid ITU-T Recommendations is regularly published.

Thereference to a document within this Recommendation does not give it, as astand-alone document, the status of a Recommendation.



[ITU-T Y.1540] ITU-TRecommendation Y.1540 (2007), “Internet protocol data communication service –IP packet transfer and availability performance parameters,” Nov. 2007.

[ITU-T Y.1543] ITU-T Recommendation Y.1543 (2007), “Measurements in IPnetworks for inter-domain performance assessment,” Nov. 2007.

[ITU-T Y.1544] ITU-T Recommendation Y.1544 (2008), “Multicast IPperformance parameters,” .

# 3.定义 (Definitions)

Platform Ahardware and/or software architecture that serves as a foundation or base forrealising a certain functionality.

# 4.缩写和首字母缩略词 (Abbreviations and acronyms)

本建议使用下列缩略语：

CMTS Cable Modem Termination System 电缆调制解调器终端系统

DSLAM DSL Access Multiplexer DSL接入复用器

NMS NetworkManagement System 网络管理系统

OSS OperationsSupport System

QoE Qualityof Experience 体验质量

QoS Qualityof Service 服务质量

VoD Videoon Demand 视频点播

# 5.约定 (Conventions)

In this document:

The keyword “**is recommended” **indicates a requirement which isrecommended but which is not absolutely required. Thus this requirement need not be present toclaim conformance.

The keyword “**may**” indicates a requirement which is optional.

# 6.IPTV性能监测点 (IPTV performance monitoring points)

The entire IPTV content delivery chain can be divided into multiple *domains.* Operators at domain bordershave the option to perform monitoring which, when taken together, forms anend-to-end monitoring topology.

This domain approach is independent of any specific monitoringmethod.

Monitored performance characteristics, across a single domain ormultiple domains, can be integrated with existing or new operations supportsystems (OSS)and/or network management system (NMS) systems.



![](https://img-blog.csdn.net/20131104152930406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



Figure1/G.1081 – IPTV模型 (IPTV Domains)

The exact topology and domains will varyfrom one operator to another; however, monitoring can be applied at each domainboundary. An example topology with generalized domain boundaries is shown inFigure 1. These domains are further divided into specific monitoring domains inFigure 2. Within each domain, different aspects can be monitored at each domainboundary as outlined below.

![](https://img-blog.csdn.net/20131104152935921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Figure2/G.1081 – IPTV性能监测点 (IPTV Performance Monitoring Points)

A complete performance monitoring system is recommended to include aperformance monitoring management platform to translate the measured data fromthe monitoring points into actionable knowledge. Functions of the managementplatform could include data storage, analysis, correlation, faultidentification, root cause analysis, report generation and visualization. Therange of the management platform(s) may cover all domains as shown in Figure 2.

The interfaces between the performance monitoring managementplatform(s) and the monitoring points will be further defined in the future.

# 7. 性能监测点定义 (Performance monitoringpoint definitions)

This section provides a basic description of each monitoring point shown inthe previous section. The capabilities and parameters listed below are not anexhaustive list. Some parameters may be difficult to obtain in some scenarios.Conversely, there may be some other parameters that are essential. These arestill under study.

#### 7.1 监测点1 Monitoring point 1 – PT1

This point demarcates the domain borderbetween content provision and IPTV control. PT1is located at the domain border between content providers (contentowners and aggregators) and IPTV Service provider.

PT1 enablesthe measurement of source audio-visual quality and metadata verification and theanalysis of information to be exchanged between content provider and IPTV serviceprovider.

PT1 may capture thefollowing information:

·Coding parameters of video, audioand text content;

·Quality parameters of video,audio and text content;

·Acknowledgement for the report ofsource content transfer receipt;

PT1may support thefollowing capabilities:

·Real-time request/responsetransactions by the protocols involved;

·Real-time notifications ofasynchronous events;

·Reliable and secure deliveryof any messages involved;

·Clock synchronization withother monitoring points;

·Identification of variouscontent providers;

·Report testing information for quality parameters ofvideo audio and text content fromvarious content providers to the service quality monitoring platform.

#### 7.2监测点2 Monitoring point 2 – PT2

This point is located at the domain borderbetween the service provider and network provider. It should aim for originalstreaming quality monitoring. PT2 monitors the service provider streamquality at the head-end egress.

PT2 may capture thefollowing information:

·Quality parameters of streammedia from the core node of the service provider;

·Performance parameters of EPGservers at the core node of the service provider;

·Performance parameters of VODservers at the core node of the service provider;

·Performance parameters of service related servers at the core node of service provider;

PT2 may support thefollowing capabilities:

·Real-time request/responsetransactions by the protocols involved;

·Real-time notifications ofasynchronous events;

·Reliable and secure delivery ofany messages involved;

·Clock synchronization withothermonitoring points;

·Identification of variousservers;

·Report information for quality parameters ofvideo, audio and text content and performance parameters of servers to the service quality monitoring platform.

#### 7.3 监测点3 Monitoring point 3 – PT3

This point is located at the boundarybetween the IP core and IP edge networks for monitoring of IP-relatedperformance parameters.This point can be placed on any type of interfacebetween the IPTV core network and the edge network.

PT3 may capture the following information:

·Source network segment anddestination network segment to be measured;

·IP network performance as definedin (ITU-T Y.1540]. Recommended methods of measurement are found in [ITU-T Y.1543].

o Mean one-way delay

o One-way packet delay variation

o Packet loss ratio

o Packet loss profile

o Path unavailability

·Multicast IP networkperformance parameters defined in [ITU-T Y.1544].

o Successful join time

o Successful leave time

o Group mean one-way delay

o Group IP service availability

o Mean group loss ratio

PT3 may support the following capabilities:

·Real-time request/response transactions;

·Real-time notifications ofasynchronous events;

·Reliable and secure delivery ofany messages involved;

·Clock synchronization withother monitoring points;

·Submission of r information forquality parameters of IP network to service quality monitoring platform

·Access to raw IP information(e.g., packet headers, type of codec) of the audio-visual or text stream;

·Able to capture information onquality originating from end-user devices.

#### 7.4 监测点4 Monitoring Point 4 – PT4

This point is closest to the user wheremonitoring the quality of streaming, audio-visual quality, and IPTV serviceattributes areimportant. Monitoring at this point can be implemented byintroducing the performance monitoring function in Home Gateways and STBs, forexample.

PT4 may capture the following information:

·Source network segment anddestination network segment to be measured;

·Access the service qualityinformation and stream media quality information of the IPTV service platformfrom edge point of IPTV;

·Access to raw IP information(e.g., packet headers, type of codec) of the audiovisual or text stream;

·IP network performance parametersdefined in [ITU-T Y.1540]. Recommended methods of measurement are found in [ITU-TY.1543].

o Mean one-way delay

o One-way packet delay variation

o Packet loss ratio

o Path unavailability

·Multicast IP network performanceparameters defined in [ITU-T Y.1544].

o Successful join time

o Successful leave time

o Group mean one-way delay

o Group IP service availability

o Mean group loss ratio

PT4 may be able to support the following capabilities:

·Real-time request/response transactions;

·Real-time notifications ofasynchronous events;

·Reliable and secure delivery ofthe messages involved;

·Clock synchronization withothermonitoring points;

·Simulate the integrated serviceflow of IPTV terminal;

·Report information for parameters of IP network performance, servicequality that access IPTV service platform from edge point, and stream mediaquality to service quality monitoring platform.

#### 7.5 监测点5 Monitoring point 5 – PT5

This point is at the final end-point anddirectly relates to end-user QoE. Monitoring audio-visual quality, textaccuracy and IPTV service attributes as perceived by the end-user areimportant.

PT5 may capture the following information:

·Source network segment anddestination network segment to be monitored;

·Access the quality informationof service and stream media of the IPTV service platform from the client;

PT5 may support the following capabilities:

·Real-time request/response transactions;

·Real-time notifications ofasynchronous events;

·Reliable and secure delivery ofany messages involved;

·Clock synchronization withother monitoring points;

·Integrated service informationof IPTV terminal;

·Report information for parameters of IP network performance, servicequality that access IPTV service platform from client, and stream media qualityto service quality monitoring platform.

# 8.安全性考虑 (Security considerations)

Security aspects have not been addressed inthis Recommendation.




