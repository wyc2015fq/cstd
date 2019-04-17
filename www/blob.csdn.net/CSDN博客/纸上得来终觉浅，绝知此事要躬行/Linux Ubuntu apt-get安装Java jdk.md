# Linux Ubuntu apt-get安装Java jdk - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月29日 13:39:58[boonya](https://me.csdn.net/boonya)阅读数：6320








通过SSH shell窗口连接Linux系统，在shell窗口完成后续操作。

## 判断是否支持Java

直接输入：java 进行验证.................


#-----------------表示存在---------------------输出相关的信息如：

![](https://img-blog.csdn.net/20161229133831283)


#-----------------表示不存在-------------------Ubuntu会提示相关的apt-get 的Java安装版本。

![](https://img-blog.csdn.net/20161229131115679)


```
root@ubuntu:~# java
The program 'java' can be found in the following packages:
 * default-jre
 * gcj-4.9-jre-headless
 * openjdk-7-jre-headless
 * gcj-4.8-jre-headless
 * openjdk-6-jre-headless
 * openjdk-8-jre-headless
Try: apt-get install <selected package>
```

## apt-get安装Java

apt-get支持的jdk版本：

 * default-jre

 * gcj-4.9-jre-headless

 * openjdk-7-jre-headless

 * gcj-4.8-jre-headless

 * openjdk-6-jre-headless

 * openjdk-8-jre-headless

输入命令：apt-get install openjdk-7-jre-headless #自己所需的JDK版本


![](https://img-blog.csdn.net/20161229131354633)

整个安装流程的日志输出：



```
root@ubuntu:~# apt-get install openjdk-7-jre-headless
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following extra packages will be installed:
  ca-certificates-java java-common libasyncns0 libavahi-client3 libavahi-common-data libavahi-common3 libcups2 libflac8 liblcms2-2 libogg0 libpcsclite1 libpulse0 libsctp1 libsndfile1 libvorbis0a libvorbisenc2 lksctp-tools tzdata tzdata-java
Suggested packages:
  default-jre equivs cups-common liblcms2-utils pcscd pulseaudio icedtea-7-jre-jamvm libnss-mdns sun-java6-fonts fonts-dejavu-extra fonts-ipafont-gothic fonts-ipafont-mincho ttf-wqy-microhei ttf-wqy-zenhei fonts-indic
The following NEW packages will be installed:
  ca-certificates-java java-common libasyncns0 libavahi-client3 libavahi-common-data libavahi-common3 libcups2 libflac8 liblcms2-2 libogg0 libpcsclite1 libpulse0 libsctp1 libsndfile1 libvorbis0a libvorbisenc2 lksctp-tools openjdk-7-jre-headless tzdata-java
The following packages will be upgraded:
  tzdata
1 upgraded, 19 newly installed, 0 to remove and 89 not upgraded.
Need to get 40.9 MB of archives.
After this operation, 63.6 MB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://us.archive.ubuntu.com/ubuntu/ vivid/main libasyncns0 amd64 0.8-5build1 [12.3 kB]
Get:2 http://us.archive.ubuntu.com/ubuntu/ vivid/main libavahi-common-data amd64 0.6.31-4ubuntu4 [21.1 kB]
Get:3 http://us.archive.ubuntu.com/ubuntu/ vivid/main libavahi-common3 amd64 0.6.31-4ubuntu4 [22.0 kB]                                                                                                                                                                       
Get:4 http://us.archive.ubuntu.com/ubuntu/ vivid/main libavahi-client3 amd64 0.6.31-4ubuntu4 [25.7 kB]                                                                                                                                                                       
Get:5 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main libcups2 amd64 2.0.2-1ubuntu3.2 [195 kB]                                                                                                                                                                       
Get:6 http://us.archive.ubuntu.com/ubuntu/ vivid/main libogg0 amd64 1.3.2-1 [17.2 kB]                                                                                                                                                                                        
Get:7 http://us.archive.ubuntu.com/ubuntu/ vivid/main libflac8 amd64 1.3.1-1ubuntu1 [208 kB]                                                                                                                                                                                 
Get:8 http://us.archive.ubuntu.com/ubuntu/ vivid/main liblcms2-2 amd64 2.6-3ubuntu2 [137 kB]                                                                                                                                                                                 
Get:9 http://us.archive.ubuntu.com/ubuntu/ vivid/main libpcsclite1 amd64 1.8.11-3ubuntu1 [21.3 kB]                                                                                                                                                                           
Get:10 http://us.archive.ubuntu.com/ubuntu/ vivid/main libvorbis0a amd64 1.3.4-2 [87.3 kB]                                                                                                                                                                                   
Get:11 http://us.archive.ubuntu.com/ubuntu/ vivid/main libvorbisenc2 amd64 1.3.4-2 [70.4 kB]                                                                                                                                                                                 
Get:12 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main libsndfile1 amd64 1.0.25-9.1ubuntu0.15.04.1 [138 kB]                                                                                                                                                          
Get:13 http://us.archive.ubuntu.com/ubuntu/ vivid/main libpulse0 amd64 1:6.0-0ubuntu6 [246 kB]                                                                                                                                                                               
Get:14 http://us.archive.ubuntu.com/ubuntu/ vivid/main libsctp1 amd64 1.0.16+dfsg-2 [8,414 B]                                                                                                                                                                                
Get:15 http://us.archive.ubuntu.com/ubuntu/ vivid/main ca-certificates-java all 20140324 [12.3 kB]                                                                                                                                                                           
Get:16 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main tzdata all 2015g-0ubuntu0.15.04 [170 kB]                                                                                                                                                                      
Get:17 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main tzdata-java all 2015g-0ubuntu0.15.04 [70.6 kB]                                                                                                                                                                
Get:18 http://us.archive.ubuntu.com/ubuntu/ vivid/main java-common all 0.52 [131 kB]                                                                                                                                                                                         
Get:19 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main openjdk-7-jre-headless amd64 7u95-2.6.4-0ubuntu0.15.04.1 [39.3 MB]                                                                                                                                            
22% [19 openjdk-7-jre-headless 7,312 kB/39.3 MB 19%]                                                                                                                                                                                                       60.0 kB/s 8min 53s^C
root@ubuntu:~# apt-get install openjdk-7-jre-headless
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following extra packages will be installed:
  ca-certificates-java java-common libasyncns0 libavahi-client3 libavahi-common-data libavahi-common3 libcups2 libflac8 liblcms2-2 libogg0 libpcsclite1 libpulse0 libsctp1 libsndfile1 libvorbis0a libvorbisenc2 lksctp-tools tzdata tzdata-java
Suggested packages:
  default-jre equivs cups-common liblcms2-utils pcscd pulseaudio icedtea-7-jre-jamvm libnss-mdns sun-java6-fonts fonts-dejavu-extra fonts-ipafont-gothic fonts-ipafont-mincho ttf-wqy-microhei ttf-wqy-zenhei fonts-indic
The following NEW packages will be installed:
  ca-certificates-java java-common libasyncns0 libavahi-client3 libavahi-common-data libavahi-common3 libcups2 libflac8 liblcms2-2 libogg0 libpcsclite1 libpulse0 libsctp1 libsndfile1 libvorbis0a libvorbisenc2 lksctp-tools openjdk-7-jre-headless tzdata-java
The following packages will be upgraded:
  tzdata
1 upgraded, 19 newly installed, 0 to remove and 89 not upgraded.
Need to get 39.3 MB/40.9 MB of archives.
After this operation, 63.6 MB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main openjdk-7-jre-headless amd64 7u95-2.6.4-0ubuntu0.15.04.1 [39.3 MB]
Get:2 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main openjdk-7-jre-headless amd64 7u95-2.6.4-0ubuntu0.15.04.1 [39.3 MB]                                                                                                                                             
Get:3 http://us.archive.ubuntu.com/ubuntu/ vivid/main lksctp-tools amd64 1.0.16+dfsg-2 [41.9 kB]                                                                                                                                                                             
Fetched 7,362 kB in 17min 27s (7,027 B/s)                                                                                                                                                                                                                                    
Preconfiguring packages ...
Selecting previously unselected package libasyncns0:amd64.
(Reading database ... 58255 files and directories currently installed.)
Preparing to unpack .../libasyncns0_0.8-5build1_amd64.deb ...
Unpacking libasyncns0:amd64 (0.8-5build1) ...
Selecting previously unselected package libavahi-common-data:amd64.
Preparing to unpack .../libavahi-common-data_0.6.31-4ubuntu4_amd64.deb ...
Unpacking libavahi-common-data:amd64 (0.6.31-4ubuntu4) ...
Selecting previously unselected package libavahi-common3:amd64.
Preparing to unpack .../libavahi-common3_0.6.31-4ubuntu4_amd64.deb ...
Unpacking libavahi-common3:amd64 (0.6.31-4ubuntu4) ...
Selecting previously unselected package libavahi-client3:amd64.
Preparing to unpack .../libavahi-client3_0.6.31-4ubuntu4_amd64.deb ...
Unpacking libavahi-client3:amd64 (0.6.31-4ubuntu4) ...
Selecting previously unselected package libcups2:amd64.
Preparing to unpack .../libcups2_2.0.2-1ubuntu3.2_amd64.deb ...
Unpacking libcups2:amd64 (2.0.2-1ubuntu3.2) ...
Selecting previously unselected package libogg0:amd64.
Preparing to unpack .../libogg0_1.3.2-1_amd64.deb ...
Unpacking libogg0:amd64 (1.3.2-1) ...
Selecting previously unselected package libflac8:amd64.
Preparing to unpack .../libflac8_1.3.1-1ubuntu1_amd64.deb ...
Unpacking libflac8:amd64 (1.3.1-1ubuntu1) ...
Selecting previously unselected package liblcms2-2:amd64.
Preparing to unpack .../liblcms2-2_2.6-3ubuntu2_amd64.deb ...
Unpacking liblcms2-2:amd64 (2.6-3ubuntu2) ...
Selecting previously unselected package libpcsclite1:amd64.
Preparing to unpack .../libpcsclite1_1.8.11-3ubuntu1_amd64.deb ...
Unpacking libpcsclite1:amd64 (1.8.11-3ubuntu1) ...
Selecting previously unselected package libvorbis0a:amd64.
Preparing to unpack .../libvorbis0a_1.3.4-2_amd64.deb ...
Unpacking libvorbis0a:amd64 (1.3.4-2) ...
Selecting previously unselected package libvorbisenc2:amd64.
Preparing to unpack .../libvorbisenc2_1.3.4-2_amd64.deb ...
Unpacking libvorbisenc2:amd64 (1.3.4-2) ...
Selecting previously unselected package libsndfile1:amd64.
Preparing to unpack .../libsndfile1_1.0.25-9.1ubuntu0.15.04.1_amd64.deb ...
Unpacking libsndfile1:amd64 (1.0.25-9.1ubuntu0.15.04.1) ...
Selecting previously unselected package libpulse0:amd64.
Preparing to unpack .../libpulse0_1%3a6.0-0ubuntu6_amd64.deb ...
Unpacking libpulse0:amd64 (1:6.0-0ubuntu6) ...
Selecting previously unselected package libsctp1:amd64.
Preparing to unpack .../libsctp1_1.0.16+dfsg-2_amd64.deb ...
Unpacking libsctp1:amd64 (1.0.16+dfsg-2) ...
Selecting previously unselected package ca-certificates-java.
Preparing to unpack .../ca-certificates-java_20140324_all.deb ...
Unpacking ca-certificates-java (20140324) ...
Preparing to unpack .../tzdata_2015g-0ubuntu0.15.04_all.deb ...
Unpacking tzdata (2015g-0ubuntu0.15.04) over (2015c-1) ...
Processing triggers for ca-certificates (20141019) ...
Updating certificates in /etc/ssl/certs... 0 added, 0 removed; done.
Running hooks in /etc/ca-certificates/update.d....done.
Setting up tzdata (2015g-0ubuntu0.15.04) ...

Current default time zone: 'America/Los_Angeles'
Local time is now:      Wed Dec 28 21:32:54 PST 2016.
Universal Time is now:  Thu Dec 29 05:32:54 UTC 2016.
Run 'dpkg-reconfigure tzdata' if you wish to change it.

Selecting previously unselected package tzdata-java.
(Reading database ... 58357 files and directories currently installed.)
Preparing to unpack .../tzdata-java_2015g-0ubuntu0.15.04_all.deb ...
Unpacking tzdata-java (2015g-0ubuntu0.15.04) ...
Selecting previously unselected package java-common.
Preparing to unpack .../java-common_0.52_all.deb ...
Unpacking java-common (0.52) ...
Selecting previously unselected package openjdk-7-jre-headless:amd64.
Preparing to unpack .../openjdk-7-jre-headless_7u95-2.6.4-0ubuntu0.15.04.1_amd64.deb ...
Unpacking openjdk-7-jre-headless:amd64 (7u95-2.6.4-0ubuntu0.15.04.1) ...
Selecting previously unselected package lksctp-tools.
Preparing to unpack .../lksctp-tools_1.0.16+dfsg-2_amd64.deb ...
Unpacking lksctp-tools (1.0.16+dfsg-2) ...
Processing triggers for man-db (2.7.0.2-5) ...
Setting up libasyncns0:amd64 (0.8-5build1) ...
Setting up libavahi-common-data:amd64 (0.6.31-4ubuntu4) ...
Setting up libavahi-common3:amd64 (0.6.31-4ubuntu4) ...
Setting up libavahi-client3:amd64 (0.6.31-4ubuntu4) ...
Setting up libcups2:amd64 (2.0.2-1ubuntu3.2) ...
Setting up libogg0:amd64 (1.3.2-1) ...
Setting up libflac8:amd64 (1.3.1-1ubuntu1) ...
Setting up liblcms2-2:amd64 (2.6-3ubuntu2) ...
Setting up libpcsclite1:amd64 (1.8.11-3ubuntu1) ...
Setting up libvorbis0a:amd64 (1.3.4-2) ...
Setting up libvorbisenc2:amd64 (1.3.4-2) ...
Setting up libsndfile1:amd64 (1.0.25-9.1ubuntu0.15.04.1) ...
Setting up libpulse0:amd64 (1:6.0-0ubuntu6) ...
Setting up libsctp1:amd64 (1.0.16+dfsg-2) ...
Setting up tzdata-java (2015g-0ubuntu0.15.04) ...
Setting up java-common (0.52) ...
Setting up lksctp-tools (1.0.16+dfsg-2) ...
Setting up openjdk-7-jre-headless:amd64 (7u95-2.6.4-0ubuntu0.15.04.1) ...
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/rmid to provide /usr/bin/rmid (rmid) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/java to provide /usr/bin/java (java) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/keytool to provide /usr/bin/keytool (keytool) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/pack200 to provide /usr/bin/pack200 (pack200) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/rmiregistry to provide /usr/bin/rmiregistry (rmiregistry) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/unpack200 to provide /usr/bin/unpack200 (unpack200) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/orbd to provide /usr/bin/orbd (orbd) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/servertool to provide /usr/bin/servertool (servertool) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/tnameserv to provide /usr/bin/tnameserv (tnameserv) in auto mode
update-alternatives: using /usr/lib/jvm/java-7-openjdk-amd64/jre/lib/jexec to provide /usr/bin/jexec (jexec) in auto mode
Setting up ca-certificates-java (20140324) ...
Adding debian:Digital_Signature_Trust_Co._Global_CA_1.pem
Adding debian:TURKTRUST_Certificate_Services_Provider_Root_2007.pem
Adding debian:TeliaSonera_Root_CA_v1.pem
Adding debian:China_Internet_Network_Information_Center_EV_Certificates_Root.pem
Adding debian:GeoTrust_Global_CA.pem
Adding debian:GeoTrust_Universal_CA_2.pem
Adding debian:Staat_der_Nederlanden_Root_CA.pem
Adding debian:NetLock_Business_=Class_B=_Root.pem
Adding debian:Starfield_Root_Certificate_Authority_-_G2.pem
Adding debian:XRamp_Global_CA_Root.pem
Adding debian:Chambers_of_Commerce_Root_-_2008.pem
Adding debian:DST_Root_CA_X3.pem
Adding debian:Certum_Trusted_Network_CA.pem
Adding debian:Secure_Global_CA.pem
Adding debian:TC_TrustCenter_Universal_CA_I.pem
Adding debian:D-TRUST_Root_Class_3_CA_2_EV_2009.pem
Adding debian:DigiCert_Assured_ID_Root_G2.pem
Adding debian:SecureSign_RootCA11.pem
Adding debian:America_Online_Root_Certification_Authority_2.pem
Adding debian:Verisign_Class_1_Public_Primary_Certification_Authority.pem
Adding debian:TC_TrustCenter_Class_2_CA_II.pem
Adding debian:NetLock_Arany_=Class_Gold=_F?tan煤s铆tv谩ny.pem
Adding debian:NetLock_Qualified_=Class_QA=_Root.pem
Adding debian:QuoVadis_Root_CA_1_G3.pem
Adding debian:America_Online_Root_Certification_Authority_1.pem
Adding debian:ACCVRAIZ1.pem
Adding debian:WellsSecure_Public_Root_Certificate_Authority.pem
Adding debian:ACEDICOM_Root.pem
Adding debian:DigiCert_Global_Root_G3.pem
Adding debian:thawte_Primary_Root_CA_-_G2.pem
Adding debian:Root_CA_Generalitat_Valenciana.pem
Adding debian:Verisign_Class_3_Public_Primary_Certification_Authority_2.pem
Adding debian:Visa_eCommerce_Root.pem
Adding debian:DigiCert_High_Assurance_EV_Root_CA.pem
Adding debian:StartCom_Certification_Authority.pem
Adding debian:Buypass_Class_2_CA_1.pem
Adding debian:Swisscom_Root_EV_CA_2.pem
Adding debian:SwissSign_Platinum_CA_-_G2.pem
Adding debian:GTE_CyberTrust_Global_Root.pem
Adding debian:Starfield_Services_Root_Certificate_Authority_-_G2.pem
Adding debian:certSIGN_ROOT_CA.pem
Adding debian:thawte_Primary_Root_CA_-_G3.pem
Adding debian:AddTrust_Public_Services_Root.pem
Adding debian:Equifax_Secure_CA.pem
Adding debian:RSA_Security_2048_v3.pem
Adding debian:Go_Daddy_Class_2_CA.pem
Adding debian:T?B陌TAK_UEKAE_K枚k_Sertifika_Hizmet_Sa?lay谋c谋s谋_-_S眉r眉m_3.pem
Adding debian:Juur-SK.pem
Adding debian:Verisign_Class_4_Public_Primary_Certification_Authority_-_G3.pem
Adding debian:VeriSign_Universal_Root_Certification_Authority.pem
Adding debian:COMODO_Certification_Authority.pem
Adding debian:SwissSign_Gold_CA_-_G2.pem
Adding debian:Thawte_Premium_Server_CA.pem
Adding debian:Security_Communication_EV_RootCA1.pem
Adding debian:COMODO_ECC_Certification_Authority.pem
Adding debian:UTN_DATACorp_SGC_Root_CA.pem
Adding debian:OISTE_WISeKey_Global_Root_GA_CA.pem
Adding debian:Trustis_FPS_Root_CA.pem
Adding debian:Entrust.net_Premium_2048_Secure_Server_CA.pem
Adding debian:AffirmTrust_Commercial.pem
Adding debian:Verisign_Class_3_Public_Primary_Certification_Authority_-_G2.pem
Adding debian:Deutsche_Telekom_Root_CA_2.pem
Adding debian:DigiCert_Trusted_Root_G4.pem
Adding debian:ComSign_Secured_CA.pem
Adding debian:Certinomis_-_Autorit茅_Racine.pem
Adding debian:GlobalSign_Root_CA.pem
Adding debian:Verisign_Class_2_Public_Primary_Certification_Authority_-_G2.pem
Adding debian:TC_TrustCenter_Class_3_CA_II.pem
Adding debian:QuoVadis_Root_CA_2_G3.pem
Adding debian:Comodo_Secure_Services_root.pem
Adding debian:AddTrust_Low-Value_Services_Root.pem
Adding debian:Staat_der_Nederlanden_Root_CA_-_G2.pem
Adding debian:Thawte_Server_CA.pem
Adding debian:A-Trust-nQual-03.pem
Adding debian:Baltimore_CyberTrust_Root.pem
Adding debian:AddTrust_External_Root.pem
Adding debian:thawte_Primary_Root_CA.pem
Adding debian:Digital_Signature_Trust_Co._Global_CA_3.pem
Adding debian:Autoridad_de_Certificacion_Firmaprofesional_CIF_A62634068.pem
Adding debian:Equifax_Secure_Global_eBusiness_CA.pem
Adding debian:SecureTrust_CA.pem
Adding debian:Equifax_Secure_eBusiness_CA_1.pem
Adding debian:PSCProcert.pem
Adding debian:NetLock_Notary_=Class_A=_Root.pem
Adding debian:DigiCert_Assured_ID_Root_G3.pem
Adding debian:Security_Communication_RootCA2.pem
Adding debian:Go_Daddy_Root_Certificate_Authority_-_G2.pem
Adding debian:AC_Ra铆z_Certic谩mara_S.A..pem
Adding debian:AffirmTrust_Premium_ECC.pem
Adding debian:Certigna.pem
Adding debian:SG_TRUST_SERVICES_RACINE.pem
Adding debian:CA_Disig_Root_R1.pem
Adding debian:QuoVadis_Root_CA_2.pem
Adding debian:Verisign_Class_2_Public_Primary_Certification_Authority_-_G3.pem
Adding debian:AffirmTrust_Premium.pem
Adding debian:Verisign_Class_3_Public_Primary_Certification_Authority.pem
Adding debian:IGC_A.pem
Adding debian:DST_ACES_CA_X6.pem
Adding debian:Camerfirma_Chambers_of_Commerce_Root.pem
Adding debian:spi-cacert-2008.pem
Adding debian:ComSign_CA.pem
Adding debian:AffirmTrust_Networking.pem
Adding debian:VeriSign_Class_3_Public_Primary_Certification_Authority_-_G4.pem
Adding debian:Starfield_Class_2_CA.pem
Adding debian:E-Guven_Kok_Elektronik_Sertifika_Hizmet_Saglayicisi.pem
Adding debian:TWCA_Global_Root_CA.pem
Adding debian:Camerfirma_Global_Chambersign_Root.pem
Adding debian:CA_Disig_Root_R2.pem
Adding debian:Swisscom_Root_CA_1.pem
Adding debian:UTN_USERFirst_Email_Root_CA.pem
Adding debian:E-Tugra_Certification_Authority.pem
Adding debian:DigiCert_Assured_ID_Root_CA.pem
Adding debian:EBG_Elektronik_Sertifika_Hizmet_Sa?lay谋c谋s谋.pem
Adding debian:AddTrust_Qualified_Certificates_Root.pem
Adding debian:Izenpe.com.pem
Adding debian:Hellenic_Academic_and_Research_Institutions_RootCA_2011.pem
Adding debian:GeoTrust_Primary_Certification_Authority.pem
Adding debian:Buypass_Class_2_Root_CA.pem
Adding debian:Verisign_Class_3_Public_Primary_Certification_Authority_-_G3.pem
Adding debian:TWCA_Root_Certification_Authority.pem
Adding debian:GlobalSign_Root_CA_-_R2.pem
Adding debian:Taiwan_GRCA.pem
Adding debian:T-TeleSec_GlobalRoot_Class_2.pem
Adding debian:Actalis_Authentication_Root_CA.pem
Adding debian:Swisscom_Root_CA_2.pem
Adding debian:GlobalSign_Root_CA_-_R3.pem
Adding debian:SwissSign_Silver_CA_-_G2.pem
Adding debian:DigiCert_Global_Root_G2.pem
Adding debian:Verisign_Class_1_Public_Primary_Certification_Authority_-_G3.pem
Adding debian:Buypass_Class_3_Root_CA.pem
Adding debian:QuoVadis_Root_CA.pem
Adding debian:Verisign_Class_1_Public_Primary_Certification_Authority_-_G2.pem
Adding debian:Entrust_Root_Certification_Authority.pem
Adding debian:GeoTrust_Universal_CA.pem
Adding debian:T-TeleSec_GlobalRoot_Class_3.pem
Adding debian:S-TRUST_Authentication_and_Encryption_Root_CA_2005_PN.pem
Adding debian:ePKI_Root_Certification_Authority.pem
Adding debian:Sonera_Class_1_Root_CA.pem
Adding debian:CA_Disig.pem
Adding debian:D-TRUST_Root_Class_3_CA_2_2009.pem
Adding debian:Microsec_e-Szigno_Root_CA.pem
Adding debian:Hongkong_Post_Root_CA_1.pem
Adding debian:ApplicationCA_-_Japanese_Government.pem
Adding debian:TURKTRUST_Certificate_Services_Provider_Root_2.pem
Adding debian:StartCom_Certification_Authority_2.pem
Adding debian:DigiCert_Global_Root_CA.pem
Adding debian:Certplus_Class_2_Primary_CA.pem
Adding debian:QuoVadis_Root_CA_3_G3.pem
Adding debian:EC-ACC.pem
Adding debian:Certum_Root_CA.pem
Adding debian:Comodo_Trusted_Services_root.pem
Adding debian:Buypass_Class_3_CA_1.pem
Adding debian:Atos_TrustedRoot_2011.pem
Adding debian:GeoTrust_Primary_Certification_Authority_-_G2.pem
Adding debian:Microsec_e-Szigno_Root_CA_2009.pem
Adding debian:Comodo_AAA_Services_root.pem
Adding debian:CNNIC_ROOT.pem
Adding debian:WoSign_China.pem
Adding debian:VeriSign_Class_3_Public_Primary_Certification_Authority_-_G5.pem
Adding debian:TURKTRUST_Certificate_Services_Provider_Root_1.pem
Adding debian:Cybertrust_Global_Root.pem
Adding debian:GeoTrust_Primary_Certification_Authority_-_G3.pem
Adding debian:WoSign.pem
Adding debian:Network_Solutions_Certificate_Authority.pem
Adding debian:StartCom_Certification_Authority_G2.pem
Adding debian:Global_Chambersign_Root_-_2008.pem
Adding debian:GeoTrust_Global_CA_2.pem
Adding debian:QuoVadis_Root_CA_3.pem
Adding debian:Sonera_Class_2_Root_CA.pem
Adding debian:UTN_USERFirst_Hardware_Root_CA.pem
Adding debian:EE_Certification_Centre_Root_CA.pem
Adding debian:Security_Communication_Root_CA.pem
Adding debian:NetLock_Express_=Class_C=_Root.pem
done.
Processing triggers for libc-bin (2.21-0ubuntu4) ...
Processing triggers for ca-certificates (20141019) ...
Updating certificates in /etc/ssl/certs... 0 added, 0 removed; done.
Running hooks in /etc/ca-certificates/update.d....
done.
done.
root@ubuntu:~#
```



## 验证是否安装成功

直接输入：java 进行验证.................

![](https://img-blog.csdn.net/20161229133831283)

![](https://img-blog.csdn.net/20161229134130739)


安装成功！！！！

如果你要使用Tomcat服务器，有了Java依赖，那么直接就可以启动了。




