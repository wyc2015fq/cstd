# Java JDK 5-JDK12文档导航【持续更新】 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年04月02日 14:59:23[boonya](https://me.csdn.net/boonya)阅读数：80








最近总觉得去查询JDK各个版本之前的差异和JVM配置上的异同不是很方便，特整理贴于此处。

笔记1：这里说明了为什么在JDK6-JDK8中 JVM配置里面是以-server 或-client开头的来区分客户端和服务端，而之后的版本去掉了虚拟机的区分配置。

[https://docs.oracle.com/en/java/javase/index.html](https://docs.oracle.com/en/java/javase/index.html)

**目录**

[JDK12](#JDK12)

[Overview](#Overview)

[Tools](#Tools)

[Language and Libraries](#Language%20and%20Libraries)

[Specifications](#Specifications)

[Security](#Security)

[HotSpot Virtual Machine](#HotSpot%20Virtual%20Machine)

[Manage and Troubleshoot](#Manage%20and%20Troubleshoot)

[Client Technologies](#Client%20Technologies)

[JDK11](#JDK11)

[Overview](#Overview)

[Tools](#Tools)

[Language and Libraries](#Language%20and%20Libraries)

[Specifications](#Specifications)

[Security](#Security)

[HotSpot Virtual Machine](#HotSpot%20Virtual%20Machine)

[Manage and Troubleshoot](#Manage%20and%20Troubleshoot)

[Client Technologies](#Client%20Technologies)

[JDK10](#JDK10)

[Overview](#Overview)

[Tools](#Tools)

[Language and Libraries](#Language%20and%20Libraries)

[Specifications](#Specifications)

[Security](#Security)

[HotSpot Virtual Machine](#HotSpot%20Virtual%20Machine)

[Manage and Troubleshoot](#Manage%20and%20Troubleshoot)

[Deploy](#Deploy)

[Client Technologies](#Client%20Technologies)

[JDK9](#JDK9)

[Get Started](#Get%20Started)

[Tools](#Tools)

[Language](#Language)

[Specifications](#Specifications)

[Core Libraries](#Core%20Libraries)

[Security](#Security)

[HotSpot Virtual Machine](#HotSpot%20Virtual%20Machine)

[Manage and Troubleshoot](#Manage%20and%20Troubleshoot)

[Deploy](#Deploy)

[Client Technologies](#Client%20Technologies)

[JDK8](#JDK8)

[About Java SE 8](#About%20Java%20SE%208)

[Download and Install](#Download%20and%20Install)

[Write Your First Application](#Write%20Your%20First%20Application)

[Learn the Language](#Learn%20the%20Language)

[Monitor and Troubleshoot](#Monitor%20and%20Troubleshoot)

[HotSpot Virtual Machine](#HotSpot%20Virtual%20Machine)

[Deploy](#Deploy)

[Reference](#Reference)

[Release Notes](#Release%20Notes)

[JDK7](#JDK7)

[JDK6](#JDK6)

[JDK5](#JDK5)

## JDK12

本章介绍Java虚拟机（JVM）的实现以及Java HotSpot技术的主要功能：
-     自适应编译器：标准解释器用于启动应用程序。应用程序运行时，将分析代码以检测性能瓶颈或热点。 Java HotSpot VM编译代码的性能关键部分以提高性能，但不编译很少使用的代码（大多数应用程序）。 Java HotSpot VM使用自适应编译器来决定如何使用内联等技术优化编译代码。
-     快速内存分配和垃圾收集：Java HotSpot技术为对象和快速，高效，最先进的垃圾收集器提供快速内存分配。
-     线程同步：Java HotSpot技术提供了一种线程处理功能，旨在扩展以用于大型共享内存多处理器服务器。



在Oracle Java Runtime Environment（JRE）8及更早版本中，JVM的不同实现（客户端VM，服务器VM和最小VM）支持常用作客户端，服务器和嵌入式系统的配置。由于大多数系统现在可以利用服务器VM，因此Oracle Java运行时环境（JRE）9仅提供该VM实现。

### Overview
- [Release Notes](https://www.oracle.com/technetwork/java/javase/12-relnote-issues-5211422.html)
- [What's New](http://www.oracle.com/technetwork/java/javase/12-relnote-issues-5211422.html#NewFeature)
- [Migration Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSMIG-GUID-C25E2B1D-6C24-4403-8540-CFEA875B994A)
- [Download the JDK](https://www.oracle.com/technetwork/java/javase/downloads/index.html)
- [Install Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJIG-GUID-8677A77F-231A-40F7-98B9-1FD0B48C346A)
- [Version String](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJIG-GUID-DCA60310-6565-4BB6-8D24-6FF07C1C4B4E)

### Tools
- [Tools Reference](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSWOR-GUID-55DE52DF-5774-4AAB-B334-E026FBAE6F34)
- [JShell User's Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSHEL-GUID-630F27C8-1195-4989-9F6B-2C51D46F52C8)
- [Javadoc Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJAV-GUID-7A344353-3BBF-45C4-8B28-15025DDCC643)

### Language and Libraries
- [Language Updates](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSLAN)
- [Core Libraries](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSCOR-GUID-C6BE8117-F73E-4BE6-98AA-681A0CD4EEA9)
- [JDK HTTP Client](http://openjdk.java.net/groups/net/httpclient/)
- [Java Tutorials](https://docs.oracle.com/javase/tutorial/tutorialLearningPaths.html)
- [Modular JDK](http://openjdk.java.net/projects/jigsaw/)
- [Java Scripting Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJSG-GUID-76E11DD9-C7A4-4232-AF57-6B6A0535BB60)
- [Nashorn User's Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSNUG-GUID-89EC58B9-A26D-45FD-84B1-AD7FEF1B2928)

### Specifications
- [API Documentation](https://docs.oracle.com/en/java/javase/12/docs/api/overview-summary.html)
- [Language and VM](https://docs.oracle.com/javase/specs/index.html)
- [Java Security Standard Algorithm Names](https://docs.oracle.com/en/java/javase/12/docs/specs/security/standard-names.html)
- [JAR](https://docs.oracle.com/en/java/javase/12/docs/specs/jar/jar.html)
- [Java Native Interface (JNI)](https://docs.oracle.com/en/java/javase/12/docs/specs/jni/index.html)
- [JVM Tool Interface (JVM TI)](https://docs.oracle.com/en/java/javase/12/docs/specs/jvmti.html)
- [Serialization](https://docs.oracle.com/en/java/javase/12/docs/specs/serialization/index.html)
- [Java Debug Wire Protocol (JDWP)](https://docs.oracle.com/en/java/javase/12/docs/specs/jdwp/jdwp-spec.html)
- [Standard Doclet](https://docs.oracle.com/en/java/javase/12/docs/specs/doc-comment-spec.html)

### Security
- [Secure Coding Guidelines](https://www.oracle.com/technetwork/java/seccodeguide-139067.html)
- [Security Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSSEC-GUID-2EF91196-D468-4D0F-8FDC-DA2BEA165D10)

### HotSpot Virtual Machine
- [Java Virtual Machine Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJVM-GUID-982B244A-9B01-479A-8651-CB6475019281)
- [Garbage Collection Tuning](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSGCT-GUID-326EB4CF-8C8C-4267-8355-21AB04F0D304)
- [JRockit to HotSpot Migration Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JRHMG107)

### Manage and Troubleshoot
- [Troubleshooting Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSTGD107)
- [Java Mission Control](https://docs.oracle.com/javacomponents/index.html)
- [Management Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSMGM-GUID-EA3CFF69-F0D3-47AB-9AED-EF1CBF7F2B24)
- [JMX Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJMX-GUID-72DCB4C1-93F8-4F37-B46E-2C708139C8A5)

### Client Technologies
- [Java Accessibility Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSACC-GUID-17F9FD40-E191-41CE-BCF9-D956F1EF5111)

## JDK11

本章介绍Java虚拟机（JVM）的实现以及Java HotSpot技术的主要功能：
-     自适应编译器：标准解释器用于启动应用程序。应用程序运行时，将分析代码以检测性能瓶颈或热点。 Java HotSpot VM编译代码的性能关键部分以提高性能，但不编译很少使用的代码（大多数应用程序）。 Java HotSpot VM使用自适应编译器来决定如何使用内联等技术优化编译代码。
-     快速内存分配和垃圾收集：Java HotSpot技术为对象和快速，高效，最先进的垃圾收集器提供快速内存分配。
-     线程同步：Java HotSpot技术提供了一种线程处理功能，旨在扩展以用于大型共享内存多处理器服务器。



在Oracle Java Runtime Environment（JRE）8及更早版本中，JVM的不同实现（客户端VM，服务器VM和最小VM）支持常用作客户端，服务器和嵌入式系统的配置。由于大多数系统现在可以利用服务器VM，因此Oracle Java运行时环境（JRE）9仅提供该VM实现。

### Overview
- [Release Notes](https://www.oracle.com/technetwork/java/javase/11-relnotes-5012447.html)
- [What's New](https://www.oracle.com/technetwork/java/javase/11-relnote-issues-5012449.html#NewFeature)
- [Migration Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSMIG-GUID-561005C1-12BB-455C-AD41-00455CAD23A6)
- [Download the JDK](https://www.oracle.com/technetwork/java/javase/downloads/index.html)
- [Install Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSJIG-GUID-8677A77F-231A-40F7-98B9-1FD0B48C346A)
- [Version String](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSJIG-GUID-DCA60310-6565-4BB6-8D24-6FF07C1C4B4E)

### Tools
- [Tools Reference](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSWOR-GUID-55DE52DF-5774-4AAB-B334-E026FBAE6F34)
- [JShell User's Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSHEL-GUID-630F27C8-1195-4989-9F6B-2C51D46F52C8)
- [Javadoc Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSJAV-GUID-7A344353-3BBF-45C4-8B28-15025DDCC643)

### Language and Libraries
- [Language Updates](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSLAN)
- [Core Libraries](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSCOR-GUID-C6BE8117-F73E-4BE6-98AA-681A0CD4EEA9)
- [JDK HTTP Client](http://openjdk.java.net/groups/net/httpclient/)
- [Java Tutorials](https://docs.oracle.com/javase/tutorial/tutorialLearningPaths.html)
- [Modular JDK](http://openjdk.java.net/projects/jigsaw/)
- [Java Scripting Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSJSG-GUID-76E11DD9-C7A4-4232-AF57-6B6A0535BB60)
- [Nashorn User's Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSNUG-GUID-89EC58B9-A26D-45FD-84B1-AD7FEF1B2928)

### Specifications
- [API Documentation](https://docs.oracle.com/en/java/javase/11/docs/api/overview-summary.html)
- [Language and VM](https://docs.oracle.com/javase/specs/index.html)
- [JAR](https://docs.oracle.com/en/java/javase/11/docs/specs/jar/jar.html)
- [Java Native Interface (JNI)](https://docs.oracle.com/en/java/javase/11/docs/specs/jni/index.html)
- [JVM Tool Interface (JVM TI)](https://docs.oracle.com/en/java/javase/11/docs/specs/jvmti.html)
- [Serialization](https://docs.oracle.com/en/java/javase/11/docs/specs/serialization/index.html)
- [Java Debug Wire Protocol (JDWP)](https://docs.oracle.com/en/java/javase/11/docs/specs/jdwp/jdwp-spec.html)
- [Standard Doclet](https://docs.oracle.com/en/java/javase/11/docs/specs/doc-comment-spec.html)

### Security
- [Secure Coding Guidelines](https://www.oracle.com/technetwork/java/seccodeguide-139067.html)
- [Security Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSSEC-GUID-2EF91196-D468-4D0F-8FDC-DA2BEA165D10)

### HotSpot Virtual Machine
- [Java Virtual Machine Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSJVM-GUID-982B244A-9B01-479A-8651-CB6475019281)
- [Garbage Collection Tuning](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSGCT-GUID-326EB4CF-8C8C-4267-8355-21AB04F0D304)
- [JRockit to HotSpot Migration Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JRHMG107)

### Manage and Troubleshoot
- [Troubleshooting Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSTGD107)
- [Java Mission Control](https://docs.oracle.com/javacomponents/index.html)
- [Management Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSMGM-GUID-EA3CFF69-F0D3-47AB-9AED-EF1CBF7F2B24)
- [JMX Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSJMX-GUID-72DCB4C1-93F8-4F37-B46E-2C708139C8A5)

### Client Technologies
- [Java Accessibility Guide](https://www.oracle.com/pls/topic/lookup?ctx=javase11&id=JSACC-GUID-17F9FD40-E191-41CE-BCF9-D956F1EF5111)

## JDK10

本章介绍Java虚拟机（JVM）的实现以及Java HotSpot技术的主要功能：
-     自适应编译器：标准解释器用于启动应用程序。应用程序运行时，将分析代码以检测性能瓶颈或热点。 Java HotSpot VM编译代码的性能关键部分以提高性能，但不编译很少使用的代码（大多数应用程序）。 Java HotSpot VM使用自适应编译器来决定如何使用内联等技术优化编译代码。
-     快速内存分配和垃圾收集：Java HotSpot技术为对象和快速，高效，最先进的垃圾收集器提供快速内存分配。
-     线程同步：Java HotSpot技术提供了一种线程处理功能，旨在扩展以用于大型共享内存多处理器服务器。



在Oracle Java Runtime Environment（JRE）8及更早版本中，JVM的不同实现（客户端VM，服务器VM和最小VM）支持常用作客户端，服务器和嵌入式系统的配置。由于大多数系统现在可以利用服务器VM，因此Oracle Java运行时环境（JRE）9仅提供该VM实现。

### Overview
- [Release Notes](http://www.oracle.com/technetwork/java/javase/10-relnote-issues-4108729.html)
- [Migration Guide](https://docs.oracle.com/javase/10/migrate/toc.htm#JSMIG-GUID-7744EF96-5899-4FB2-B34E-86D49B2E89B6)
- [Download the JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
- [Install Guide](https://docs.oracle.com/javase/10/install/overview-jdk-10-and-jre-10-installation.htm#JSJIG-GUID-8677A77F-231A-40F7-98B9-1FD0B48C346A)
- [New Version String](https://docs.oracle.com/javase/10/install/version-string-format.htm#JSJIG-GUID-DCA60310-6565-4BB6-8D24-6FF07C1C4B4E)

### Tools
- [Tools Reference](https://docs.oracle.com/javase/10/tools/tools-and-command-reference.htm#JSWOR-GUID-55DE52DF-5774-4AAB-B334-E026FBAE6F34)
- [JShell User's Guide](https://docs.oracle.com/javase/10/jshell/introduction-jshell.htm#JSHEL-GUID-630F27C8-1195-4989-9F6B-2C51D46F52C8)
- [Javadoc Guide](https://docs.oracle.com/javase/10/javadoc/javadoc.htm#JSJAV-GUID-7A344353-3BBF-45C4-8B28-15025DDCC643)

### Language and Libraries
- [Language Updates](https://docs.oracle.com/javase/10/language/toc.htm)
- [Core Libraries](https://docs.oracle.com/javase/10/core/java-core-libraries1.htm#JSCOR-GUID-C6BE8117-F73E-4BE6-98AA-681A0CD4EEA9)
- [Java Tutorials](http://docs.oracle.com/javase/tutorial/tutorialLearningPaths.html)
- [Modular JDK](http://openjdk.java.net/projects/jigsaw/)
- [Java Scripting Guide](https://docs.oracle.com/javase/10/scripting/scripting-languages-and-java.htm#JSJSG-GUID-76E11DD9-C7A4-4232-AF57-6B6A0535BB60)
- [Nashorn User's Guide](https://docs.oracle.com/javase/10/nashorn/introduction.htm#JSNUG-GUID-89EC58B9-A26D-45FD-84B1-AD7FEF1B2928)

### Specifications
- [API Documentation](https://docs.oracle.com/javase/10/docs/toc.htm)
- [Language and VM](https://docs.oracle.com/javase/specs/index.html)
- [JAR](https://docs.oracle.com/javase/10/docs/specs/jar/jar.html)
- [Java Native Interface (JNI)](https://docs.oracle.com/javase/10/docs/specs/jni/index.html)
- [JVM Tool Interface (JVM TI)](https://docs.oracle.com/javase/10/docs/specs/jvmti.html)
- [Serialization](https://docs.oracle.com/javase/10/docs/specs/serialization/index.html)

### Security
- [Secure Coding Guidelines](http://www.oracle.com/technetwork/java/seccodeguide-139067.html)
- [Security Guide](https://docs.oracle.com/javase/10/security/java-security-overview1.htm#JSSEC-GUID-2EF91196-D468-4D0F-8FDC-DA2BEA165D10)

### HotSpot Virtual Machine
- [Java Virtual Machine Guide](https://docs.oracle.com/javase/10/vm/java-virtual-machine-technology-overview.htm#JSJVM-GUID-982B244A-9B01-479A-8651-CB6475019281)
- [Garbage Collection Tuning](https://docs.oracle.com/javase/10/gctuning/introduction-garbage-collection-tuning.htm#JSGCT-GUID-326EB4CF-8C8C-4267-8355-21AB04F0D304)
- [JRockit to HotSpot Migration Guide](https://docs.oracle.com/javase/10/jrockit-hotspot/introduction.htm#JRHMG107)

### Manage and Troubleshoot
- [Troubleshooting Guide](https://docs.oracle.com/javase/10/troubleshoot/general-java-troubleshooting.htm#JSTGD107)
- [Java Flight Recorder](https://docs.oracle.com/javacomponents/index.html)
- [Java Mission Control](https://docs.oracle.com/javacomponents/index.html)
- [Management Guide](https://docs.oracle.com/javase/10/management/overview-java-se-monitoring-and-management.htm#JSMGM-GUID-EA3CFF69-F0D3-47AB-9AED-EF1CBF7F2B24)
- [JMX Guide](http://www.oracle.com/pls/topic/lookup?ctx=javase10&id=JSJMX-GUID-4C3E84BA-3AD8-418A-92F1-3719C6A77F4B)

### Deploy
- [Deployment Guide](https://docs.oracle.com/javase/10/deploy/getting-started.htm#JSDPG101)
- [Java Control Panel](https://docs.oracle.com/javase/10/deploy/java-control-panel.htm#JSDPG-GUID-4CD5AAF3-864D-4EBD-B62E-BEF9892EFB6A)

### Client Technologies
- [JavaFX and Swing (JDK 8)](https://docs.oracle.com/javase/8/javase-clienttechnologies.htm)
- [Java Accessibility Guide](https://docs.oracle.com/javase/10/access/java-accessibility-overview.htm#JSACC-GUID-17F9FD40-E191-41CE-BCF9-D956F1EF5111)

## JDK9

本章介绍Java虚拟机（JVM）的实现以及Java HotSpot技术的主要功能：
-     自适应编译器：标准解释器用于启动应用程序。应用程序运行时，将分析代码以检测性能瓶颈或热点。 Java HotSpot VM编译代码的性能关键部分以提高性能，但不编译很少使用的代码（大多数应用程序）。 Java HotSpot VM使用自适应编译器来决定如何使用内联等技术优化编译代码。
-     快速内存分配和垃圾收集：Java HotSpot技术为对象和快速，高效，最先进的垃圾收集器提供快速内存分配。
-     线程同步：Java HotSpot技术提供了一种线程处理功能，旨在扩展以用于大型共享内存多处理器服务器。



在Oracle Java Runtime Environment（JRE）8及更早版本中，JVM的不同实现（客户端VM，服务器VM和最小VM）支持常用作客户端，服务器和嵌入式系统的配置。由于大多数系统现在可以利用服务器VM，因此Oracle Java运行时环境（JRE）9仅提供该VM实现。

### Get Started
- [Release Notes](http://www.oracle.com/technetwork/java/javase/9-relnotes-3622618.html)
- [What's New](https://docs.oracle.com/javase/9/whatsnew/toc.htm)
- [Migrate to JDK 9](https://docs.oracle.com/javase/9/migrate/toc.htm#JSMIG-GUID-7744EF96-5899-4FB2-B34E-86D49B2E89B6)
- [Get the Latest Release](http://www.oracle.com/pls/topic/lookup?ctx=javase9&id=download_jdk9)
- [Install the JDK and JRE](https://docs.oracle.com/javase/9/install/overview-jdk-9-and-jre-9-installation.htm#JSJIG-GUID-8677A77F-231A-40F7-98B9-1FD0B48C346A)



### Tools
- [Tools Reference](https://docs.oracle.com/javase/9/tools/tools-and-command-reference.htm#JSWOR-GUID-55DE52DF-5774-4AAB-B334-E026FBAE6F34)
- [JShell User's Guide](https://docs.oracle.com/javase/9/jshell/introduction-jshell.htm#JSHEL-GUID-630F27C8-1195-4989-9F6B-2C51D46F52C8)
- [Javadoc Guide](https://docs.oracle.com/javase/9/javadoc/javadoc.htm#JSJAV-GUID-7A344353-3BBF-45C4-8B28-15025DDCC643)

### Language
- [Modular JDK](http://openjdk.java.net/projects/jigsaw/)
- [Java Tutorials (JDK 8)](http://docs.oracle.com/javase/tutorial/tutorialLearningPaths.html)
- [Java SE 9 Language Updates](https://docs.oracle.com/javase/9/language/toc.htm#JSLAN-GUID-B06D7006-D9F4-42F8-AD21-BF861747EDCF)
- [Java Scripting Guide](https://docs.oracle.com/javase/9/scripting/scripting-languages-and-java.htm#JSJSG-GUID-76E11DD9-C7A4-4232-AF57-6B6A0535BB60)
- [Nashorn User's Guide](https://docs.oracle.com/javase/9/nashorn/introduction.htm#JSNUG-GUID-89EC58B9-A26D-45FD-84B1-AD7FEF1B2928)

### Specifications
- [API Documentation](https://docs.oracle.com/javase/9/docs/toc.htm)
- [Java Language and Virtual Machine Specifications](https://docs.oracle.com/javase/specs/index.html)

### Core Libraries
- [Core Libraries](https://docs.oracle.com/javase/9/core/enhanced-deprecation1.htm#JSCOR-GUID-23B13A9E-2727-42DC-B03A-E374B3C4CE96)
- [Internationalization](https://docs.oracle.com/javase/9/intl/internationalization-enhancements-jdk-9.htm#JSINT-GUID-5ED91AA9-B2E3-4E05-8E99-6A009D2B36AF)
- [RMI Security Recommendations](https://docs.oracle.com/javase/9/rmi/toc.htm)

### Security
- [Secure Coding Guidelines](http://www.oracle.com/technetwork/java/seccodeguide-139067.html)
- [Security Guide](https://docs.oracle.com/javase/9/security/java-security-overview1.htm#JSSEC-GUID-2EF91196-D468-4D0F-8FDC-DA2BEA165D10)

### HotSpot Virtual Machine
- [Java Virtual Machine Guide](https://docs.oracle.com/javase/9/vm/java-virtual-machine-technology-overview.htm#JSJVM-GUID-982B244A-9B01-479A-8651-CB6475019281)
- [Garbage Collection Tuning](https://docs.oracle.com/javase/9/gctuning/introduction-garbage-collection-tuning.htm#JSGCT-GUID-326EB4CF-8C8C-4267-8355-21AB04F0D304)
- [JRockit to HotSpot Migration Guide](https://docs.oracle.com/javase/9/jrockit-hotspot/introduction.htm#JRHMG107)

### Manage and Troubleshoot
- [Troubleshooting Guide](https://docs.oracle.com/javase/9/troubleshoot/general-java-troubleshooting.htm#JSTGD107)
- [Java Flight Recorder](https://docs.oracle.com/javacomponents/index.html)
- [Java Mission Control](https://docs.oracle.com/javacomponents/index.html)
- [Management Guide](https://docs.oracle.com/javase/9/management/overview-java-se-monitoring-and-management.htm#JSMGM-GUID-EA3CFF69-F0D3-47AB-9AED-EF1CBF7F2B24)
- [JMX Guide](http://www.oracle.com/pls/topic/lookup?ctx=javase9&id=JSJMX-GUID-4C3E84BA-3AD8-418A-92F1-3719C6A77F4B)

### Deploy
- [Deployment Guide](https://docs.oracle.com/javase/9/deploy/getting-started.htm#JSDPG101)
- [Java Control Panel](https://docs.oracle.com/javase/9/deploy/java-control-panel.htm#JSDPG-GUID-4CD5AAF3-864D-4EBD-B62E-BEF9892EFB6A)

### Client Technologies
- [JavaFX and Swing (JDK 8)](https://docs.oracle.com/javase/8/javase-clienttechnologies.htm)
- [Java Accessibility Guide](https://docs.oracle.com/javase/9/access/java-accessibility-overview.htm#JSACC-GUID-17F9FD40-E191-41CE-BCF9-D956F1EF5111)



## JDK8

JDK提供Java虚拟机（VM）的一个或多个实现：
-     在通常用于客户端应用程序的平台上，JDK附带了一个名为Java HotSpot Client VM（客户端VM）的VM实现。调整客户端VM以减少启动时间和内存占用。启动应用程序时，可以使用-client命令行选项调用它。
-     在所有平台上，JDK都附带了一个名为Java HotSpot Server VM（服务器VM）的Java虚拟机实现。服务器VM旨在实现最高的程序执行速度。启动应用程序时，可以使用-server命令行选项调用它。



Java HotSpot技术的一些功能，对于两种VM实现都是通用的，如下所示。
-     自适应编译器 - 使用标准解释器启动应用程序，但在运行时会对代码进行分析，以检测性能瓶颈或“热点”。 Java HotSpot VM编译代码中性能关键部分以提高性能，同时避免不必要的编译很少使用的代码（大多数程序）。 Java HotSpot VM还使用自适应编译器来动态决定如何使用内嵌等技术优化编译代码。编译器执行的运行时分析允许它消除猜测，确定哪些优化将产生最大的性能优势。
-     快速内存分配和垃圾收集 - Java HotSpot技术为对象提供快速内存分配，并提供快速，高效，最先进的垃圾收集器选择。
-     线程同步 - Java编程语言允许使用多个并发的程序执行路径（称为“线程”）。 Java HotSpot技术提供了一种线程处理功能，旨在轻松扩展以用于大型共享内存多处理器服务器。

### About Java SE 8
- [What's New (Features and Enhancements)](http://www.oracle.com/pls/topic/lookup?ctx=javase80&id=sewhatsnew)
- [Commercial Features ![icon: link opens in a new window](https://docs.oracle.com/javase/8/assets/img-common/new-window.png)](https://docs.oracle.com/javacomponents/index.html)
- [Compatibility Guide](http://www.oracle.com/pls/topic/lookup?ctx=javase80&id=secompat)
- [Known Issues](http://www.oracle.com/pls/topic/lookup?ctx=javase80&id=seissues)

### Download and Install
- [Certified System Configurations](http://www.oracle.com/pls/topic/lookup?ctx=javase80&id=secert)
- [Download and Installation Instructions](https://docs.oracle.com/javase/8/docs/technotes/guides/install/install_overview.html)

### Write Your First Application
- [Get Started with Java](https://docs.oracle.com/javase/tutorial/getStarted/index.html)
- [Get Started with JavaFX](http://www.oracle.com/pls/topic/lookup?ctx=javase80&id=JFXST)

### Learn the Language
- [Java Tutorials Learning Paths](https://docs.oracle.com/javase/tutorial/tutorialLearningPaths.html)

### Monitor and Troubleshoot
- [Java Mission Control ![icon: link opens in a new window](https://docs.oracle.com/javase/8/assets/img-common/new-window.png)](https://docs.oracle.com/javacomponents/jmc.htm)
- [Java Flight Recorder ![icon: link opens in a new window](https://docs.oracle.com/javase/8/assets/img-common/new-window.png)](https://docs.oracle.com/javacomponents/jmc.htm)
- [Troubleshooting Guide](https://docs.oracle.com/javase/8/docs/technotes/guides/troubleshoot)

### HotSpot Virtual Machine
- [HotSpot Virtual Machine Garbage Collection Tuning Guide](https://docs.oracle.com/javase/8/docs/technotes/guides/vm/gctuning)
- [JRockit to HotSpot Migration Guide](http://www.oracle.com/pls/topic/lookup?ctx=javacomponents&id=JRHMG)

### Deploy
- [Deployment Guide](https://docs.oracle.com/javase/8/docs/technotes/guides/deploy)

### Reference
- [Java SE API Documentation](https://docs.oracle.com/javase/8/docs/api/index.html)
- [JavaFX API Documentation](http://www.oracle.com/pls/topic/lookup?ctx=javase80&id=JFXAP)
- [Developer Guides](https://docs.oracle.com/javase/8/docs/index.html)
- [Java Language and Virtual Machine Specifications](https://docs.oracle.com/javase/specs/index.html)
- [Java SE Tools Reference for UNIX](https://docs.oracle.com/javase/8/docs/technotes/tools/unix/index.html)
- [Java SE Tools Reference for Windows](https://docs.oracle.com/javase/8/docs/technotes/tools/windows/index.html)

### Release Notes
- [Java SE Release Notes](http://www.oracle.com/pls/topic/lookup?ctx=javase80&id=serelnotes)



## JDK7

JDK™提供Java™虚拟机（VM）的一个或多个实现：
-     在通常用于客户端应用程序的平台上，JDK附带一个名为Java HotSpot™客户端VM（客户端VM）的VM实现。调整客户端VM以减少启动时间和内存占用。启动应用程序时，可以使用-client命令行选项调用它。
-     在所有平台上，JDK都附带了一个名为Java HotSpot Server VM（服务器VM）的Java虚拟机实现。服务器VM旨在实现最高的程序执行速度。启动应用程序时，可以使用-server命令行选项调用它。

Java HotSpot技术的一些功能，对于两种VM实现都是通用的，如下所示。
-  自适应编译器 - 使用标准解释器启动应用程序，但在运行时会对代码进行分析，以检测性能瓶颈或“热点”。 Java HotSpot VM编译代码中性能关键部分以提高性能，同时避免不必要的编译很少使用的代码（大多数程序）。 Java HotSpot VM还使用自适应编译器来动态决定如何使用内嵌等技术优化编译代码。编译器执行的运行时分析允许它消除猜测，确定哪些优化将产生最大的性能优势。
- 快速内存分配和垃圾收集 -  Java HotSpot技术为对象提供快速内存分配，并提供快速，高效，最先进的垃圾收集器选择。
- 线程同步 -  Java编程语言允许使用多个并发的程序执行路径（称为“线程”）。 Java HotSpot技术提供了一种线程处理功能，旨在轻松扩展以用于大型共享内存多处理器服务器。


|[JDK](https://docs.oracle.com/javase/7/docs/technotes/guides/index.html#jre-jdk)||[**Java Language**](https://docs.oracle.com/javase/7/docs/technotes/guides/language/index.html)||[Java Language](https://docs.oracle.com/javase/7/docs/technotes/guides/language/index.html)||----||[Java Language](https://docs.oracle.com/javase/7/docs/technotes/guides/language/index.html)|| | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|[Java Language](https://docs.oracle.com/javase/7/docs/technotes/guides/language/index.html)| | | | | | | | | | | | | | | | | | | | | |
|[**Tools &			Tool APIs**](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html)||[java](https://docs.oracle.com/javase/7/docs/technotes/tools/windows/java.html)|[javac](https://docs.oracle.com/javase/7/docs/technotes/guides/javac/index.html)|[javadoc](https://docs.oracle.com/javase/7/docs/technotes/guides/javadoc/index.html)|[jar](https://docs.oracle.com/javase/7/docs/technotes/guides/jar/index.html)|[javap](https://docs.oracle.com/javase/7/docs/technotes/tools/windows/javap.html)|[JPDA](https://docs.oracle.com/javase/7/docs/technotes/guides/jpda/index.html)| ||----|----|----|----|----|----|----||[JConsole](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#jconsole)|[Java VisualVM](https://docs.oracle.com/javase/7/docs/technotes/guides/visualvm/index.html)|[JMC](https://docs.oracle.com/javase/7/docs/technotes/guides/jmc/index.html)|[JFR](https://docs.oracle.com/javase/7/docs/technotes/guides/jfr/index.html)|[Java DB](http://docs.oracle.com/javadb/index_jdk7.html)|[Int'l](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#intl)|[JVM TI](https://docs.oracle.com/javase/7/docs/technotes/guides/jvmti/index.html)||[IDL](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#idl)|[Deploy](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#deployment)|[Security](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#security)|[Troubleshoot](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#troubleshoot)|[Scripting](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#scripting)|[Web Services](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#webservices)|[RMI](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#rmi)||[java](https://docs.oracle.com/javase/7/docs/technotes/tools/windows/java.html)|[javac](https://docs.oracle.com/javase/7/docs/technotes/guides/javac/index.html)|[javadoc](https://docs.oracle.com/javase/7/docs/technotes/guides/javadoc/index.html)|[jar](https://docs.oracle.com/javase/7/docs/technotes/guides/jar/index.html)|[javap](https://docs.oracle.com/javase/7/docs/technotes/tools/windows/javap.html)|[JPDA](https://docs.oracle.com/javase/7/docs/technotes/guides/jpda/index.html)|[JConsole](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#jconsole)|[Java VisualVM](https://docs.oracle.com/javase/7/docs/technotes/guides/visualvm/index.html)|[JMC](https://docs.oracle.com/javase/7/docs/technotes/guides/jmc/index.html)|[JFR](https://docs.oracle.com/javase/7/docs/technotes/guides/jfr/index.html)|[Java DB](http://docs.oracle.com/javadb/index_jdk7.html)|[Int'l](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#intl)|[JVM TI](https://docs.oracle.com/javase/7/docs/technotes/guides/jvmti/index.html)|[IDL](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#idl)|[Deploy](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#deployment)|[Security](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#security)|[Troubleshoot](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#troubleshoot)|[Scripting](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#scripting)|[Web Services](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#webservices)|[RMI](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#rmi)|
|[java](https://docs.oracle.com/javase/7/docs/technotes/tools/windows/java.html)|[javac](https://docs.oracle.com/javase/7/docs/technotes/guides/javac/index.html)|[javadoc](https://docs.oracle.com/javase/7/docs/technotes/guides/javadoc/index.html)|[jar](https://docs.oracle.com/javase/7/docs/technotes/guides/jar/index.html)|[javap](https://docs.oracle.com/javase/7/docs/technotes/tools/windows/javap.html)|[JPDA](https://docs.oracle.com/javase/7/docs/technotes/guides/jpda/index.html)| | | | | | | | | | | | | | | | |
|[JConsole](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#jconsole)|[Java VisualVM](https://docs.oracle.com/javase/7/docs/technotes/guides/visualvm/index.html)|[JMC](https://docs.oracle.com/javase/7/docs/technotes/guides/jmc/index.html)|[JFR](https://docs.oracle.com/javase/7/docs/technotes/guides/jfr/index.html)|[Java DB](http://docs.oracle.com/javadb/index_jdk7.html)|[Int'l](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#intl)|[JVM TI](https://docs.oracle.com/javase/7/docs/technotes/guides/jvmti/index.html)| | | | | | | | | | | | | | | |
|[IDL](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#idl)|[Deploy](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#deployment)|[Security](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#security)|[Troubleshoot](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#troubleshoot)|[Scripting](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#scripting)|[Web Services](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#webservices)|[RMI](https://docs.oracle.com/javase/7/docs/technotes/tools/index.html#rmi)| | | | | | | | | | | | | | | |
|[JRE](https://docs.oracle.com/javase/7/docs/technotes/guides/index.html#jre-jdk)|[**Deployment**](https://docs.oracle.com/javase/7/docs/technotes/guides/jweb/index.html)||[Java Web Start](https://docs.oracle.com/javase/7/docs/technotes/guides/javaws/index.html)|[Applet / Java Plug-in](https://docs.oracle.com/javase/7/docs/technotes/guides/jweb/applet/applet_dev_guide.html)||----|----||[Java Web Start](https://docs.oracle.com/javase/7/docs/technotes/guides/javaws/index.html)|[Applet / Java Plug-in](https://docs.oracle.com/javase/7/docs/technotes/guides/jweb/applet/applet_dev_guide.html)| | | | | | | | | | | | | | | | | |
|[Java Web Start](https://docs.oracle.com/javase/7/docs/technotes/guides/javaws/index.html)|[Applet / Java Plug-in](https://docs.oracle.com/javase/7/docs/technotes/guides/jweb/applet/applet_dev_guide.html)| | | | | | | | | | | | | | | | | | | | |
|[**User Interface			Toolkits**](https://docs.oracle.com/javase/7/docs/technotes/guides/index.html#userinterface)||[JavaFX](http://docs.oracle.com/javafx/2/)||----||[JavaFX](http://docs.oracle.com/javafx/2/)| | | | | | | | | | | | | | | | | | | |
|[JavaFX](http://docs.oracle.com/javafx/2/)| | | | | | | | | | | | | | | | | | | | | |
||[Swing](https://docs.oracle.com/javase/7/docs/technotes/guides/swing/index.html)|[Java 2D](https://docs.oracle.com/javase/7/docs/technotes/guides/2d/index.html)|[AWT](https://docs.oracle.com/javase/7/docs/technotes/guides/awt/index.html)|[Accessibility](https://docs.oracle.com/javase/7/docs/technotes/guides/access/index.html)| ||----|----|----|----|----||[Drag and Drop](https://docs.oracle.com/javase/7/docs/technotes/guides/dragndrop/index.html)|[Input Methods](https://docs.oracle.com/javase/7/docs/technotes/guides/imf/index.html)|[Image I/O](https://docs.oracle.com/javase/7/docs/technotes/guides/imageio/index.html)|[Print Service](https://docs.oracle.com/javase/7/docs/technotes/guides/jps/index.html)|[Sound](https://docs.oracle.com/javase/7/docs/technotes/guides/sound/index.html)||[Swing](https://docs.oracle.com/javase/7/docs/technotes/guides/swing/index.html)|[Java 2D](https://docs.oracle.com/javase/7/docs/technotes/guides/2d/index.html)|[AWT](https://docs.oracle.com/javase/7/docs/technotes/guides/awt/index.html)|[Accessibility](https://docs.oracle.com/javase/7/docs/technotes/guides/access/index.html)|[Drag and Drop](https://docs.oracle.com/javase/7/docs/technotes/guides/dragndrop/index.html)|[Input Methods](https://docs.oracle.com/javase/7/docs/technotes/guides/imf/index.html)|[Image I/O](https://docs.oracle.com/javase/7/docs/technotes/guides/imageio/index.html)|[Print Service](https://docs.oracle.com/javase/7/docs/technotes/guides/jps/index.html)|[Sound](https://docs.oracle.com/javase/7/docs/technotes/guides/sound/index.html)|[Java SE			API](https://docs.oracle.com/javase/7/docs/api/index.html)| | | | | | | | | | | |
|[Swing](https://docs.oracle.com/javase/7/docs/technotes/guides/swing/index.html)|[Java 2D](https://docs.oracle.com/javase/7/docs/technotes/guides/2d/index.html)|[AWT](https://docs.oracle.com/javase/7/docs/technotes/guides/awt/index.html)|[Accessibility](https://docs.oracle.com/javase/7/docs/technotes/guides/access/index.html)| | | | | | | | | | | | | | | | | | |
|[Drag and Drop](https://docs.oracle.com/javase/7/docs/technotes/guides/dragndrop/index.html)|[Input Methods](https://docs.oracle.com/javase/7/docs/technotes/guides/imf/index.html)|[Image I/O](https://docs.oracle.com/javase/7/docs/technotes/guides/imageio/index.html)|[Print Service](https://docs.oracle.com/javase/7/docs/technotes/guides/jps/index.html)|[Sound](https://docs.oracle.com/javase/7/docs/technotes/guides/sound/index.html)| | | | | | | | | | | | | | | | | |
|[**Integration			Libraries**](https://docs.oracle.com/javase/7/docs/technotes/guides/index.html#integration)||[IDL](https://docs.oracle.com/javase/7/docs/technotes/guides/idl/index.html)|[JDBC](https://docs.oracle.com/javase/7/docs/technotes/guides/jdbc/index.html)|[JNDI](https://docs.oracle.com/javase/7/docs/technotes/guides/jndi/index.html)|[RMI](https://docs.oracle.com/javase/7/docs/technotes/guides/rmi/index.html)|[RMI-IIOP](https://docs.oracle.com/javase/7/docs/technotes/guides/rmi-iiop/index.html)|[Scripting](https://docs.oracle.com/javase/7/docs/technotes/guides/scripting/index.html)||----|----|----|----|----|----||[IDL](https://docs.oracle.com/javase/7/docs/technotes/guides/idl/index.html)|[JDBC](https://docs.oracle.com/javase/7/docs/technotes/guides/jdbc/index.html)|[JNDI](https://docs.oracle.com/javase/7/docs/technotes/guides/jndi/index.html)|[RMI](https://docs.oracle.com/javase/7/docs/technotes/guides/rmi/index.html)|[RMI-IIOP](https://docs.oracle.com/javase/7/docs/technotes/guides/rmi-iiop/index.html)|[Scripting](https://docs.oracle.com/javase/7/docs/technotes/guides/scripting/index.html)| | | | | | | | | | | | | | |
|[IDL](https://docs.oracle.com/javase/7/docs/technotes/guides/idl/index.html)|[JDBC](https://docs.oracle.com/javase/7/docs/technotes/guides/jdbc/index.html)|[JNDI](https://docs.oracle.com/javase/7/docs/technotes/guides/jndi/index.html)|[RMI](https://docs.oracle.com/javase/7/docs/technotes/guides/rmi/index.html)|[RMI-IIOP](https://docs.oracle.com/javase/7/docs/technotes/guides/rmi-iiop/index.html)|[Scripting](https://docs.oracle.com/javase/7/docs/technotes/guides/scripting/index.html)| | | | | | | | | | | | | | | | |
|[**Other Base			Libraries**](https://docs.oracle.com/javase/7/docs/technotes/guides/index.html#otherbase)||[Beans](https://docs.oracle.com/javase/7/docs/technotes/guides/beans/index.html)|[Int'l Support](https://docs.oracle.com/javase/7/docs/technotes/guides/intl/index.html)|[Input/Output](https://docs.oracle.com/javase/7/docs/technotes/guides/io/index.html)|[JMX](https://docs.oracle.com/javase/7/docs/technotes/guides/jmx/index.html)||----|----|----|----||[JNI](https://docs.oracle.com/javase/7/docs/technotes/guides/jni/index.html)|[Math](https://docs.oracle.com/javase/7/docs/technotes/guides/math/index.html)|[Networking](https://docs.oracle.com/javase/7/docs/technotes/guides/net/index.html)|[Override Mechanism](https://docs.oracle.com/javase/7/docs/technotes/guides/standards/index.html)||[Security](https://docs.oracle.com/javase/7/docs/technotes/guides/security/index.html)|[Serialization](https://docs.oracle.com/javase/7/docs/technotes/guides/serialization/index.html)|[Extension Mechanism](https://docs.oracle.com/javase/7/docs/technotes/guides/extensions/index.html)|[XML JAXP](https://docs.oracle.com/javase/7/docs/technotes/guides/xml/index.html)||[Beans](https://docs.oracle.com/javase/7/docs/technotes/guides/beans/index.html)|[Int'l Support](https://docs.oracle.com/javase/7/docs/technotes/guides/intl/index.html)|[Input/Output](https://docs.oracle.com/javase/7/docs/technotes/guides/io/index.html)|[JMX](https://docs.oracle.com/javase/7/docs/technotes/guides/jmx/index.html)|[JNI](https://docs.oracle.com/javase/7/docs/technotes/guides/jni/index.html)|[Math](https://docs.oracle.com/javase/7/docs/technotes/guides/math/index.html)|[Networking](https://docs.oracle.com/javase/7/docs/technotes/guides/net/index.html)|[Override Mechanism](https://docs.oracle.com/javase/7/docs/technotes/guides/standards/index.html)|[Security](https://docs.oracle.com/javase/7/docs/technotes/guides/security/index.html)|[Serialization](https://docs.oracle.com/javase/7/docs/technotes/guides/serialization/index.html)|[Extension Mechanism](https://docs.oracle.com/javase/7/docs/technotes/guides/extensions/index.html)|[XML JAXP](https://docs.oracle.com/javase/7/docs/technotes/guides/xml/index.html)| | | | | | | | |
|[Beans](https://docs.oracle.com/javase/7/docs/technotes/guides/beans/index.html)|[Int'l Support](https://docs.oracle.com/javase/7/docs/technotes/guides/intl/index.html)|[Input/Output](https://docs.oracle.com/javase/7/docs/technotes/guides/io/index.html)|[JMX](https://docs.oracle.com/javase/7/docs/technotes/guides/jmx/index.html)| | | | | | | | | | | | | | | | | | |
|[JNI](https://docs.oracle.com/javase/7/docs/technotes/guides/jni/index.html)|[Math](https://docs.oracle.com/javase/7/docs/technotes/guides/math/index.html)|[Networking](https://docs.oracle.com/javase/7/docs/technotes/guides/net/index.html)|[Override Mechanism](https://docs.oracle.com/javase/7/docs/technotes/guides/standards/index.html)| | | | | | | | | | | | | | | | | | |
|[Security](https://docs.oracle.com/javase/7/docs/technotes/guides/security/index.html)|[Serialization](https://docs.oracle.com/javase/7/docs/technotes/guides/serialization/index.html)|[Extension Mechanism](https://docs.oracle.com/javase/7/docs/technotes/guides/extensions/index.html)|[XML JAXP](https://docs.oracle.com/javase/7/docs/technotes/guides/xml/index.html)| | | | | | | | | | | | | | | | | | |
|[**lang and util			Base Libraries**](https://docs.oracle.com/javase/7/docs/technotes/guides/index.html#langutil)||[lang and util](https://docs.oracle.com/javase/7/docs/technotes/guides/lang/index.html)|[Collections](https://docs.oracle.com/javase/7/docs/technotes/guides/collections/index.html)|[Concurrency Utilities](https://docs.oracle.com/javase/7/docs/technotes/guides/concurrency/index.html)|[JAR](https://docs.oracle.com/javase/7/docs/technotes/guides/jar/index.html)| ||----|----|----|----|----||[Logging](https://docs.oracle.com/javase/7/docs/technotes/guides/logging/index.html)|[Management](https://docs.oracle.com/javase/7/docs/technotes/guides/management/index.html)|[Preferences API](https://docs.oracle.com/javase/7/docs/technotes/guides/preferences/index.html)|[Ref Objects](https://docs.oracle.com/javase/7/docs/api/java/lang/ref/package-summary.html)| ||[Reflection](https://docs.oracle.com/javase/7/docs/technotes/guides/reflection/index.html)|[Regular Expressions](https://docs.oracle.com/javase/7/docs/api/java/util/regex/package-summary.html)|[Versioning](https://docs.oracle.com/javase/7/docs/technotes/guides/versioning/index.html)|[Zip](https://docs.oracle.com/javase/7/docs/api/java/util/zip/package-summary.html)|[Instrumentation](https://docs.oracle.com/javase/7/docs/technotes/guides/instrumentation/index.html)||[lang and util](https://docs.oracle.com/javase/7/docs/technotes/guides/lang/index.html)|[Collections](https://docs.oracle.com/javase/7/docs/technotes/guides/collections/index.html)|[Concurrency Utilities](https://docs.oracle.com/javase/7/docs/technotes/guides/concurrency/index.html)|[JAR](https://docs.oracle.com/javase/7/docs/technotes/guides/jar/index.html)|[Logging](https://docs.oracle.com/javase/7/docs/technotes/guides/logging/index.html)|[Management](https://docs.oracle.com/javase/7/docs/technotes/guides/management/index.html)|[Preferences API](https://docs.oracle.com/javase/7/docs/technotes/guides/preferences/index.html)|[Ref Objects](https://docs.oracle.com/javase/7/docs/api/java/lang/ref/package-summary.html)|[Reflection](https://docs.oracle.com/javase/7/docs/technotes/guides/reflection/index.html)|[Regular Expressions](https://docs.oracle.com/javase/7/docs/api/java/util/regex/package-summary.html)|[Versioning](https://docs.oracle.com/javase/7/docs/technotes/guides/versioning/index.html)|[Zip](https://docs.oracle.com/javase/7/docs/api/java/util/zip/package-summary.html)|[Instrumentation](https://docs.oracle.com/javase/7/docs/technotes/guides/instrumentation/index.html)| | | | | | | |
|[lang and util](https://docs.oracle.com/javase/7/docs/technotes/guides/lang/index.html)|[Collections](https://docs.oracle.com/javase/7/docs/technotes/guides/collections/index.html)|[Concurrency Utilities](https://docs.oracle.com/javase/7/docs/technotes/guides/concurrency/index.html)|[JAR](https://docs.oracle.com/javase/7/docs/technotes/guides/jar/index.html)| | | | | | | | | | | | | | | | | | |
|[Logging](https://docs.oracle.com/javase/7/docs/technotes/guides/logging/index.html)|[Management](https://docs.oracle.com/javase/7/docs/technotes/guides/management/index.html)|[Preferences API](https://docs.oracle.com/javase/7/docs/technotes/guides/preferences/index.html)|[Ref Objects](https://docs.oracle.com/javase/7/docs/api/java/lang/ref/package-summary.html)| | | | | | | | | | | | | | | | | | |
|[Reflection](https://docs.oracle.com/javase/7/docs/technotes/guides/reflection/index.html)|[Regular Expressions](https://docs.oracle.com/javase/7/docs/api/java/util/regex/package-summary.html)|[Versioning](https://docs.oracle.com/javase/7/docs/technotes/guides/versioning/index.html)|[Zip](https://docs.oracle.com/javase/7/docs/api/java/util/zip/package-summary.html)|[Instrumentation](https://docs.oracle.com/javase/7/docs/technotes/guides/instrumentation/index.html)| | | | | | | | | | | | | | | | | |
|[**Java Virtual Machine**](https://docs.oracle.com/javase/7/docs/technotes/guides/vm/index.html)||[Java HotSpot VM](https://docs.oracle.com/javase/7/docs/technotes/guides/vm/index.html)||----||[Java HotSpot VM](https://docs.oracle.com/javase/7/docs/technotes/guides/vm/index.html)|| | | | | | | | | | | | | | | | | | |
|[Java HotSpot VM](https://docs.oracle.com/javase/7/docs/technotes/guides/vm/index.html)| | | | | | | | | | | | | | | | | | | | | |

## JDK6

DKTM提供JavaTM虚拟机（VM）的一个或多个实现：
-         在通常用于客户端应用程序的平台上，JDK附带一个名为Java HotSpotTM客户端VM（客户端VM）的VM实现。调整客户端VM以减少启动时间和内存占用。启动应用程序时，可以使用-client命令行选项调用它。
-         在所有平台上，JDK都附带了一个名为Java HotSpot Server VM（服务器VM）的Java虚拟机实现。服务器VM旨在实现最高的程序执行速度。启动应用程序时，可以使用-server命令行选项调用它。



    Java HotSpot技术的一些功能，对于两种VM实现都是通用的，如下所示。
-         自适应编译器 - 使用标准解释器启动应用程序，但在运行时会对代码进行分析，以检测性能瓶颈或“热点”。 Java HotSpot VM编译代码中性能关键部分以提高性能，同时避免不必要的编译很少使用的代码（大多数程序）。 Java HotSpot VM还使用自适应编译器来动态决定如何使用内嵌等技术优化编译代码。编译器执行的运行时分析允许它消除猜测，确定哪些优化将产生最大的性能优势。
-         快速内存分配和垃圾收集 - Java HotSpot技术为对象提供快速内存分配，并提供快速，高效，最先进的垃圾收集器选择。
-         线程同步 - Java编程语言允许使用多个并发的程序执行路径（称为“线程”）。 Java HotSpot技术提供了一种线程处理功能，旨在轻松扩展以用于大型共享内存多处理器服务器。
|||||----||| | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|| | | | | | | | | | | | | | | | | | | | |
|[JDK](https://docs.oracle.com/javase/6/docs/technotes/guides/index.html#jre-jdk)||[**Java Language**](https://docs.oracle.com/javase/6/docs/technotes/guides/language/index.html)||[Java Language](https://docs.oracle.com/javase/6/docs/technotes/guides/language/index.html)||----||[Java Language](https://docs.oracle.com/javase/6/docs/technotes/guides/language/index.html)| | | | | | | | | | | | | | | | |
|[Java Language](https://docs.oracle.com/javase/6/docs/technotes/guides/language/index.html)| | | | | | | | | | | | | | | | | | | | |
|` [**Tools &			Tool APIs**](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html)||[java](https://docs.oracle.com/javase/6/docs/technotes/tools/windows/java.html)|[javac](https://docs.oracle.com/javase/6/docs/technotes/guides/javac/index.html)|[javadoc](https://docs.oracle.com/javase/6/docs/technotes/guides/javadoc/index.html)|[apt](https://docs.oracle.com/javase/6/docs/technotes/guides/apt/index.html)|[jar](https://docs.oracle.com/javase/6/docs/technotes/guides/jar/index.html)|[javap](https://docs.oracle.com/javase/6/docs/technotes/tools/windows/javap.html)|[JPDA](https://docs.oracle.com/javase/6/docs/technotes/guides/jpda/index.html)|[JConsole](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#jconsole)|[Java VisualVM](https://docs.oracle.com/javase/6/docs/technotes/guides/visualvm/index.html)|[Java DB](http://download.oracle.com/javadb/index_jdk6.html)||----|----|----|----|----|----|----|----|----|----||[Security](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#security)|[Int'l](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#intl)|[RMI](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#rmi)|[IDL](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#idl)|[Deploy](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#deployment)|[Monitoring](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#monitor)|[Troubleshoot](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#troubleshoot)|[Scripting](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#scripting)|[JVM TI](https://docs.oracle.com/javase/6/docs/technotes/guides/jvmti/index.html)| ||[java](https://docs.oracle.com/javase/6/docs/technotes/tools/windows/java.html)|[javac](https://docs.oracle.com/javase/6/docs/technotes/guides/javac/index.html)|[javadoc](https://docs.oracle.com/javase/6/docs/technotes/guides/javadoc/index.html)|[apt](https://docs.oracle.com/javase/6/docs/technotes/guides/apt/index.html)|[jar](https://docs.oracle.com/javase/6/docs/technotes/guides/jar/index.html)|[javap](https://docs.oracle.com/javase/6/docs/technotes/tools/windows/javap.html)|[JPDA](https://docs.oracle.com/javase/6/docs/technotes/guides/jpda/index.html)|[JConsole](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#jconsole)|[Java VisualVM](https://docs.oracle.com/javase/6/docs/technotes/guides/visualvm/index.html)|[Java DB](http://download.oracle.com/javadb/index_jdk6.html)|[Security](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#security)|[Int'l](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#intl)|[RMI](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#rmi)|[IDL](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#idl)|[Deploy](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#deployment)|[Monitoring](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#monitor)|[Troubleshoot](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#troubleshoot)|[Scripting](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#scripting)|[JVM TI](https://docs.oracle.com/javase/6/docs/technotes/guides/jvmti/index.html)|
|[java](https://docs.oracle.com/javase/6/docs/technotes/tools/windows/java.html)|[javac](https://docs.oracle.com/javase/6/docs/technotes/guides/javac/index.html)|[javadoc](https://docs.oracle.com/javase/6/docs/technotes/guides/javadoc/index.html)|[apt](https://docs.oracle.com/javase/6/docs/technotes/guides/apt/index.html)|[jar](https://docs.oracle.com/javase/6/docs/technotes/guides/jar/index.html)|[javap](https://docs.oracle.com/javase/6/docs/technotes/tools/windows/javap.html)|[JPDA](https://docs.oracle.com/javase/6/docs/technotes/guides/jpda/index.html)|[JConsole](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#jconsole)|[Java VisualVM](https://docs.oracle.com/javase/6/docs/technotes/guides/visualvm/index.html)|[Java DB](http://download.oracle.com/javadb/index_jdk6.html)| | | | | | | | | | | |
|[Security](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#security)|[Int'l](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#intl)|[RMI](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#rmi)|[IDL](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#idl)|[Deploy](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#deployment)|[Monitoring](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#monitor)|[Troubleshoot](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#troubleshoot)|[Scripting](https://docs.oracle.com/javase/6/docs/technotes/tools/index.html#scripting)|[JVM TI](https://docs.oracle.com/javase/6/docs/technotes/guides/jvmti/index.html)| | | | | | | | | | | | |
|[JRE](https://docs.oracle.com/javase/6/docs/technotes/guides/index.html#jre-jdk)|[**RIAs**](https://docs.oracle.com/javase/6/docs/technotes/guides/jweb/index.html)|||[Java Web Start](https://docs.oracle.com/javase/6/docs/technotes/guides/javaws/index.html)|[Applet / Java Plug-in](https://docs.oracle.com/javase/6/docs/technotes/guides/jweb/applet/applet_dev_guide.html)||----|----|----|||[Java Web Start](https://docs.oracle.com/javase/6/docs/technotes/guides/javaws/index.html)|[Applet / Java Plug-in](https://docs.oracle.com/javase/6/docs/technotes/guides/jweb/applet/applet_dev_guide.html)| | | | | | | | | | | | | | | |
||[Java Web Start](https://docs.oracle.com/javase/6/docs/technotes/guides/javaws/index.html)|[Applet / Java Plug-in](https://docs.oracle.com/javase/6/docs/technotes/guides/jweb/applet/applet_dev_guide.html)| | | | | | | | | | | | | | | | | | |
|[**User Interface			Toolkits**](https://docs.oracle.com/javase/6/docs/technotes/guides/index.html#userinterface)||[AWT](https://docs.oracle.com/javase/6/docs/technotes/guides/awt/index.html)|[Swing](https://docs.oracle.com/javase/6/docs/technotes/guides/swing/index.html)|[Java 2D](https://docs.oracle.com/javase/6/docs/technotes/guides/2d/index.html)| | | ||----|----|----|----|----|----||[Accessibility](https://docs.oracle.com/javase/6/docs/technotes/guides/access/index.html)|[Drag n Drop](https://docs.oracle.com/javase/6/docs/technotes/guides/dragndrop/index.html)|[Input Methods](https://docs.oracle.com/javase/6/docs/technotes/guides/imf/index.html)|[Image I/O](https://docs.oracle.com/javase/6/docs/technotes/guides/imageio/index.html)|[Print Service](https://docs.oracle.com/javase/6/docs/technotes/guides/jps/index.html)|[Sound](https://docs.oracle.com/javase/6/docs/technotes/guides/sound/index.html)||[AWT](https://docs.oracle.com/javase/6/docs/technotes/guides/awt/index.html)|[Swing](https://docs.oracle.com/javase/6/docs/technotes/guides/swing/index.html)|[Java 2D](https://docs.oracle.com/javase/6/docs/technotes/guides/2d/index.html)|[Accessibility](https://docs.oracle.com/javase/6/docs/technotes/guides/access/index.html)|[Drag n Drop](https://docs.oracle.com/javase/6/docs/technotes/guides/dragndrop/index.html)|[Input Methods](https://docs.oracle.com/javase/6/docs/technotes/guides/imf/index.html)|[Image I/O](https://docs.oracle.com/javase/6/docs/technotes/guides/imageio/index.html)|[Print Service](https://docs.oracle.com/javase/6/docs/technotes/guides/jps/index.html)|[Sound](https://docs.oracle.com/javase/6/docs/technotes/guides/sound/index.html)||[Java SE			API](https://docs.oracle.com/javase/6/docs/api/index.html)| | | | | | | | |
|[AWT](https://docs.oracle.com/javase/6/docs/technotes/guides/awt/index.html)|[Swing](https://docs.oracle.com/javase/6/docs/technotes/guides/swing/index.html)|[Java 2D](https://docs.oracle.com/javase/6/docs/technotes/guides/2d/index.html)| | | | | | | | | | | | | | | | | | |
|[Accessibility](https://docs.oracle.com/javase/6/docs/technotes/guides/access/index.html)|[Drag n Drop](https://docs.oracle.com/javase/6/docs/technotes/guides/dragndrop/index.html)|[Input Methods](https://docs.oracle.com/javase/6/docs/technotes/guides/imf/index.html)|[Image I/O](https://docs.oracle.com/javase/6/docs/technotes/guides/imageio/index.html)|[Print Service](https://docs.oracle.com/javase/6/docs/technotes/guides/jps/index.html)|[Sound](https://docs.oracle.com/javase/6/docs/technotes/guides/sound/index.html)| | | | | | | | | | | | | | | |
|[**Integration			Libraries**](https://docs.oracle.com/javase/6/docs/technotes/guides/index.html#integration)||[IDL](https://docs.oracle.com/javase/6/docs/technotes/guides/idl/index.html)|[JDBC](https://docs.oracle.com/javase/6/docs/technotes/guides/jdbc/index.html)|[JNDI](https://docs.oracle.com/javase/6/docs/technotes/guides/jndi/index.html)|[RMI](https://docs.oracle.com/javase/6/docs/technotes/guides/rmi/index.html)|[RMI-IIOP](https://docs.oracle.com/javase/6/docs/technotes/guides/rmi-iiop/index.html)|[Scripting](https://docs.oracle.com/javase/6/docs/technotes/guides/scripting/index.html)||----|----|----|----|----|----||[IDL](https://docs.oracle.com/javase/6/docs/technotes/guides/idl/index.html)|[JDBC](https://docs.oracle.com/javase/6/docs/technotes/guides/jdbc/index.html)|[JNDI](https://docs.oracle.com/javase/6/docs/technotes/guides/jndi/index.html)|[RMI](https://docs.oracle.com/javase/6/docs/technotes/guides/rmi/index.html)|[RMI-IIOP](https://docs.oracle.com/javase/6/docs/technotes/guides/rmi-iiop/index.html)|[Scripting](https://docs.oracle.com/javase/6/docs/technotes/guides/scripting/index.html)| | | | | | | | | | | | | |
|[IDL](https://docs.oracle.com/javase/6/docs/technotes/guides/idl/index.html)|[JDBC](https://docs.oracle.com/javase/6/docs/technotes/guides/jdbc/index.html)|[JNDI](https://docs.oracle.com/javase/6/docs/technotes/guides/jndi/index.html)|[RMI](https://docs.oracle.com/javase/6/docs/technotes/guides/rmi/index.html)|[RMI-IIOP](https://docs.oracle.com/javase/6/docs/technotes/guides/rmi-iiop/index.html)|[Scripting](https://docs.oracle.com/javase/6/docs/technotes/guides/scripting/index.html)| | | | | | | | | | | | | | | |
|[**Other Base			Libraries**](https://docs.oracle.com/javase/6/docs/technotes/guides/index.html#otherbase)||[Beans](https://docs.oracle.com/javase/6/docs/technotes/guides/beans/index.html)|[Intl Support](https://docs.oracle.com/javase/6/docs/technotes/guides/intl/index.html)|[Input/Output](https://docs.oracle.com/javase/6/docs/technotes/guides/io/index.html)|[JMX](https://docs.oracle.com/javase/6/docs/technotes/guides/jmx/index.html)|[JNI](https://docs.oracle.com/javase/6/docs/technotes/guides/jni/index.html)|[Math](https://docs.oracle.com/javase/6/docs/technotes/guides/math/index.html)||----|----|----|----|----|----||[Networking](https://docs.oracle.com/javase/6/docs/technotes/guides/net/index.html)|[Override Mechanism](https://docs.oracle.com/javase/6/docs/technotes/guides/standards/index.html)|[Security](https://docs.oracle.com/javase/6/docs/technotes/guides/security/index.html)|[Serialization](https://docs.oracle.com/javase/6/docs/technotes/guides/serialization/index.html)|[Extension Mechanism](https://docs.oracle.com/javase/6/docs/technotes/guides/extensions/index.html)|[XML JAXP](https://docs.oracle.com/javase/6/docs/technotes/guides/xml/index.html)||[Beans](https://docs.oracle.com/javase/6/docs/technotes/guides/beans/index.html)|[Intl Support](https://docs.oracle.com/javase/6/docs/technotes/guides/intl/index.html)|[Input/Output](https://docs.oracle.com/javase/6/docs/technotes/guides/io/index.html)|[JMX](https://docs.oracle.com/javase/6/docs/technotes/guides/jmx/index.html)|[JNI](https://docs.oracle.com/javase/6/docs/technotes/guides/jni/index.html)|[Math](https://docs.oracle.com/javase/6/docs/technotes/guides/math/index.html)|[Networking](https://docs.oracle.com/javase/6/docs/technotes/guides/net/index.html)|[Override Mechanism](https://docs.oracle.com/javase/6/docs/technotes/guides/standards/index.html)|[Security](https://docs.oracle.com/javase/6/docs/technotes/guides/security/index.html)|[Serialization](https://docs.oracle.com/javase/6/docs/technotes/guides/serialization/index.html)|[Extension Mechanism](https://docs.oracle.com/javase/6/docs/technotes/guides/extensions/index.html)|[XML JAXP](https://docs.oracle.com/javase/6/docs/technotes/guides/xml/index.html)| | | | | | | |
|[Beans](https://docs.oracle.com/javase/6/docs/technotes/guides/beans/index.html)|[Intl Support](https://docs.oracle.com/javase/6/docs/technotes/guides/intl/index.html)|[Input/Output](https://docs.oracle.com/javase/6/docs/technotes/guides/io/index.html)|[JMX](https://docs.oracle.com/javase/6/docs/technotes/guides/jmx/index.html)|[JNI](https://docs.oracle.com/javase/6/docs/technotes/guides/jni/index.html)|[Math](https://docs.oracle.com/javase/6/docs/technotes/guides/math/index.html)| | | | | | | | | | | | | | | |
|[Networking](https://docs.oracle.com/javase/6/docs/technotes/guides/net/index.html)|[Override Mechanism](https://docs.oracle.com/javase/6/docs/technotes/guides/standards/index.html)|[Security](https://docs.oracle.com/javase/6/docs/technotes/guides/security/index.html)|[Serialization](https://docs.oracle.com/javase/6/docs/technotes/guides/serialization/index.html)|[Extension Mechanism](https://docs.oracle.com/javase/6/docs/technotes/guides/extensions/index.html)|[XML JAXP](https://docs.oracle.com/javase/6/docs/technotes/guides/xml/index.html)| | | | | | | | | | | | | | | |
|[**lang and util			Base Libraries**](https://docs.oracle.com/javase/6/docs/technotes/guides/index.html#langutil)||[lang and util](https://docs.oracle.com/javase/6/docs/technotes/guides/lang/index.html)|[Collections](https://docs.oracle.com/javase/6/docs/technotes/guides/collections/index.html)|[Concurrency Utilities](https://docs.oracle.com/javase/6/docs/technotes/guides/concurrency/index.html)|[JAR](https://docs.oracle.com/javase/6/docs/technotes/guides/jar/index.html)|[Logging](https://docs.oracle.com/javase/6/docs/technotes/guides/logging/index.html)|[Management](https://docs.oracle.com/javase/6/docs/technotes/guides/management/index.html)| ||----|----|----|----|----|----|----||[Preferences API](https://docs.oracle.com/javase/6/docs/technotes/guides/preferences/index.html)|[Ref Objects](https://docs.oracle.com/javase/6/docs/api/java/lang/ref/package-summary.html)|[Reflection](https://docs.oracle.com/javase/6/docs/technotes/guides/reflection/index.html)|[Regular Expressions](https://docs.oracle.com/javase/6/docs/api/java/util/regex/package-summary.html)|[Versioning](https://docs.oracle.com/javase/6/docs/technotes/guides/versioning/index.html)|[Zip](https://docs.oracle.com/javase/6/docs/api/java/util/zip/package-summary.html)|[Instrumentation](https://docs.oracle.com/javase/6/docs/technotes/guides/instrumentation/index.html)||[lang and util](https://docs.oracle.com/javase/6/docs/technotes/guides/lang/index.html)|[Collections](https://docs.oracle.com/javase/6/docs/technotes/guides/collections/index.html)|[Concurrency Utilities](https://docs.oracle.com/javase/6/docs/technotes/guides/concurrency/index.html)|[JAR](https://docs.oracle.com/javase/6/docs/technotes/guides/jar/index.html)|[Logging](https://docs.oracle.com/javase/6/docs/technotes/guides/logging/index.html)|[Management](https://docs.oracle.com/javase/6/docs/technotes/guides/management/index.html)|[Preferences API](https://docs.oracle.com/javase/6/docs/technotes/guides/preferences/index.html)|[Ref Objects](https://docs.oracle.com/javase/6/docs/api/java/lang/ref/package-summary.html)|[Reflection](https://docs.oracle.com/javase/6/docs/technotes/guides/reflection/index.html)|[Regular Expressions](https://docs.oracle.com/javase/6/docs/api/java/util/regex/package-summary.html)|[Versioning](https://docs.oracle.com/javase/6/docs/technotes/guides/versioning/index.html)|[Zip](https://docs.oracle.com/javase/6/docs/api/java/util/zip/package-summary.html)|[Instrumentation](https://docs.oracle.com/javase/6/docs/technotes/guides/instrumentation/index.html)| | | | | | |
|[lang and util](https://docs.oracle.com/javase/6/docs/technotes/guides/lang/index.html)|[Collections](https://docs.oracle.com/javase/6/docs/technotes/guides/collections/index.html)|[Concurrency Utilities](https://docs.oracle.com/javase/6/docs/technotes/guides/concurrency/index.html)|[JAR](https://docs.oracle.com/javase/6/docs/technotes/guides/jar/index.html)|[Logging](https://docs.oracle.com/javase/6/docs/technotes/guides/logging/index.html)|[Management](https://docs.oracle.com/javase/6/docs/technotes/guides/management/index.html)| | | | | | | | | | | | | | | |
|[Preferences API](https://docs.oracle.com/javase/6/docs/technotes/guides/preferences/index.html)|[Ref Objects](https://docs.oracle.com/javase/6/docs/api/java/lang/ref/package-summary.html)|[Reflection](https://docs.oracle.com/javase/6/docs/technotes/guides/reflection/index.html)|[Regular Expressions](https://docs.oracle.com/javase/6/docs/api/java/util/regex/package-summary.html)|[Versioning](https://docs.oracle.com/javase/6/docs/technotes/guides/versioning/index.html)|[Zip](https://docs.oracle.com/javase/6/docs/api/java/util/zip/package-summary.html)|[Instrumentation](https://docs.oracle.com/javase/6/docs/technotes/guides/instrumentation/index.html)| | | | | | | | | | | | | | |
|[**Java Virtual			Machine**](https://docs.oracle.com/javase/6/docs/technotes/guides/vm/index.html)||[Java Hotspot Client and Server VM](https://docs.oracle.com/javase/6/docs/technotes/guides/vm/index.html)||----||[Java Hotspot Client and Server VM](https://docs.oracle.com/javase/6/docs/technotes/guides/vm/index.html)| | | | | | | | | | | | | | | | | | |
|[Java Hotspot Client and Server VM](https://docs.oracle.com/javase/6/docs/technotes/guides/vm/index.html)| | | | | | | | | | | | | | | | | | | | |

## JDK5



![](https://docs.oracle.com/javase/1.5.0/docs/images/j2se5.gif)


|**Search the Documentation**|[Location](https://docs.oracle.com/javase/1.5.0/docs/#location)|
|----|----|



> |[Search the online documentation](http://java.sun.com/j2se/1.5.0/search.html)|*website*|
|----|----|


**New to the Java Platform?**

> |See the [New-to-JavaTM Programming Center](http://developer.java.sun.com/developer/onlineTraining/new2java/index.html).|*website*|
|----|----|


**General Information**

> |**Readme, Overview, Changes**| |
|----|----|
|[README](http://java.sun.com/j2se/1.5.0/README.html)|*website*|
|[New Features and Enhancements](https://docs.oracle.com/javase/1.5.0/docs/relnotes/features.html)|*docs*|
|[Release Notes](http://java.sun.com/j2se/1.5.0/relnotes.html)|*website*|
|[Version 5.0 or 1.5.0?](https://docs.oracle.com/javase/1.5.0/docs/relnotes/version-5.0.html)|*docs*|
|| |
|**Installation Instructions**| |
|[Installation Notes](http://java.sun.com/j2se/1.5.0/install.html)[System Configurations](http://java.sun.com/j2se/1.5.0/system-configurations.html)|*website*|
|| |
|**Compatibility**| |
|[Version Compatibility with Previous Releases](http://java.sun.com/j2se/1.5.0/compatibility.html)|*website*|
|| |
|**Bugs**| |
|[Fixed and Known Bugs](http://java.sun.com/j2se/1.5.0/fixedbugs/index.html)|*website*|
|[Submitting a Bug Report](http://java.sun.com/webapps/bugreport)|*website*|
|| |
|**Contacts**| |
|Contacting Java Software|*docs*|
|| |
|**Releases and Downloads**| |
|[JDK Download Page](http://java.sun.com/j2se/1.5.0/download.jsp)|*website*|
|[Java Software Home Page](http://java.sun.com/)|*website*|
|| |
|**Legal Notices**| |
|[Documentation Redistribution Policy](http://java.sun.com/docs/redist.html)|*website*|
|[Copyright and License Terms for Documentation](https://docs.oracle.com/javase/1.5.0/docs/relnotes/SMICopyright.html)|*docs*|




**API & Language Documentation **

> 

|[Java 2 Platform API Specification](https://docs.oracle.com/javase/1.5.0/docs/api/index.html)[(NO FRAMES)](https://docs.oracle.com/javase/1.5.0/docs/api/overview-summary.html)|*docs*|
|----|----|
|| |
|[Note About sun.* Packages](http://java.sun.com/products/jdk/faq/faq-sun-packages.html)|*website*|
|| |
|[The Java Language Specification](http://java.sun.com/docs/books/jls/third_edition/html/j3TOC.html)[(DOWNLOAD)](http://java.sun.com/docs/books/jls/index.html)|*website*|
|| |
|[The Java Virtual Machine Specification](http://java.sun.com/docs/books/vmspec/2nd-edition/jvms-java5.html)|*website*|



**Guide to Features - Java Platform **

Design specs, functional specs, user guides, tutorials and demos.

You can [Download PDF](http://java.sun.com/j2se/1.5.0/download-pdf.html) versions of some docs.



> |[**J2SE Overview**](https://docs.oracle.com/javase/1.5.0/docs/guide/index.html)|*docs*|
|----|----|
|[**New Features and Enhancements**](https://docs.oracle.com/javase/1.5.0/docs/relnotes/features.html)|*docs*|








|**Java Language**| |
|----|----|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Programming Language](https://docs.oracle.com/javase/1.5.0/docs/guide/language/index.html)|*docs*|
|**Virtual Machine**| |
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Virtual Machine](https://docs.oracle.com/javase/1.5.0/docs/guide/vm/index.html)|*docs*|
|**Base Libraries**| |
|**java.lang, java.util Packages**| |
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Language and Utility Packages](https://docs.oracle.com/javase/1.5.0/docs/guide/lang/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Monitoring and Management](https://docs.oracle.com/javase/1.5.0/docs/guide/management/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Package Version Identification](https://docs.oracle.com/javase/1.5.0/docs/guide/versioning/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Reference Objects](https://docs.oracle.com/javase/1.5.0/docs/guide/refobs/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Reflection](https://docs.oracle.com/javase/1.5.0/docs/guide/reflection/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Collections Framework](https://docs.oracle.com/javase/1.5.0/docs/guide/collections/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Concurrency Utilities](https://docs.oracle.com/javase/1.5.0/docs/guide/concurrency/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Archive (JAR) Files](https://docs.oracle.com/javase/1.5.0/docs/guide/jar/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Zip Files](https://docs.oracle.com/javase/1.5.0/docs/api/java/util/zip/package-summary.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Logging](https://docs.oracle.com/javase/1.5.0/docs/guide/logging/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Regular Expressions](https://docs.oracle.com/javase/1.5.0/docs/api/java/util/regex/package-summary.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Preferences](https://docs.oracle.com/javase/1.5.0/docs/guide/preferences/index.html)|*docs*|
|**Other packages**| |
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Math](https://docs.oracle.com/javase/1.5.0/docs/guide/math/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[New I/O](https://docs.oracle.com/javase/1.5.0/docs/guide/nio/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[I/O](https://docs.oracle.com/javase/1.5.0/docs/guide/io/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Object Serialization](https://docs.oracle.com/javase/1.5.0/docs/guide/serialization/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Networking](https://docs.oracle.com/javase/1.5.0/docs/guide/net/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Security](https://docs.oracle.com/javase/1.5.0/docs/guide/security/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Internationalization](https://docs.oracle.com/javase/1.5.0/docs/guide/intl/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[JavaBeansTM Component API](https://docs.oracle.com/javase/1.5.0/docs/guide/beans/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Management Extensions (JMX)](https://docs.oracle.com/javase/1.5.0/docs/guide/jmx/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[XML (JAXP)](https://docs.oracle.com/javase/1.5.0/docs/guide/xml/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Native Interface (JNI)](https://docs.oracle.com/javase/1.5.0/docs/guide/jni/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Extension Mechanism](https://docs.oracle.com/javase/1.5.0/docs/guide/extensions/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Endorsed Standards Override Mechanism](https://docs.oracle.com/javase/1.5.0/docs/guide/standards/index.html)|*docs*|
|**Integration Libraries**| |
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Database Connectivity (JDBCTM)](https://docs.oracle.com/javase/1.5.0/docs/guide/jdbc/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java IDL](https://docs.oracle.com/javase/1.5.0/docs/guide/idl/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Naming and Directory InterfaceTM (JNDI)](https://docs.oracle.com/javase/1.5.0/docs/guide/jndi/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Remote Method Invocation (RMI)](https://docs.oracle.com/javase/1.5.0/docs/guide/rmi/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[RMI-IIOP](https://docs.oracle.com/javase/1.5.0/docs/guide/rmi-iiop/index.html)|*docs*|
|**User Interface Libraries**| |
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[2D Graphics and Imaging](https://docs.oracle.com/javase/1.5.0/docs/guide/2d/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Accessibility](https://docs.oracle.com/javase/1.5.0/docs/guide/access/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Abstract Window Toolkit](https://docs.oracle.com/javase/1.5.0/docs/guide/awt/index.html) (AWT)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Drag-and-Drop Data Transfer](https://docs.oracle.com/javase/1.5.0/docs/guide/dragndrop/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Image I/O](https://docs.oracle.com/javase/1.5.0/docs/guide/imageio/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Input Method Framework](https://docs.oracle.com/javase/1.5.0/docs/guide/imf/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Print Service](https://docs.oracle.com/javase/1.5.0/docs/guide/jps/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Sound](https://docs.oracle.com/javase/1.5.0/docs/guide/sound/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Project Swing Components](https://docs.oracle.com/javase/1.5.0/docs/guide/swing/index.html)|*docs*|
|**Deployment**| |
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[General Deployment](https://docs.oracle.com/javase/1.5.0/docs/guide/deployment/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Plug-in](https://docs.oracle.com/javase/1.5.0/docs/guide/plugin/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Web Start](https://docs.oracle.com/javase/1.5.0/docs/guide/javaws/index.html)|*docs*|
|**Tool Specifications**| |
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Annotation Processing Tool](https://docs.oracle.com/javase/1.5.0/docs/guide/apt/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Javadoc Tool](https://docs.oracle.com/javase/1.5.0/docs/guide/javadoc/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Platform Debugger Architecture (JPDA)](https://docs.oracle.com/javase/1.5.0/docs/guide/jpda/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Debug Interface (JDI)](https://docs.oracle.com/javase/1.5.0/docs/guide/jpda/jdi/index.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[Java Debug Wire Protocol (JDWP)](https://docs.oracle.com/javase/1.5.0/docs/guide/jpda/jdwp-spec.html)|*docs*|
|![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[JVMTM Tool Interface (JVM TI)](https://docs.oracle.com/javase/1.5.0/docs/guide/jvmti/index.html) (replaces [JVMPI](https://docs.oracle.com/javase/1.5.0/docs/guide/jvmpi/index.html) and [JVMDI](https://docs.oracle.com/javase/1.5.0/docs/guide/jpda/jvmdi-spec.html))|*docs*|



**JDK Tools and Utilities**

Reference documentation for the JDK tools and utilities.



> |[JDK Tool and Utility Documentation](https://docs.oracle.com/javase/1.5.0/docs/tooldocs/index.html)			       Including Troubleshooting and Diagnostic Information|*docs*|
|----|----|
|[J2SE 5.0 Trouble-Shooting and Diagnostic Guide](http://java.sun.com/j2se/1.5/pdf/jdk50_ts_guide.pdf)			        Tools, options, and other items for use in analyzing problems|*web*|



**Demos, Samples, Tutorials, Training, and Reference**

Sample code for applets and applications, training, courses, and tutorials for the JDK.



> |[Demonstration Applets and Applications](https://docs.oracle.com/javase/1.5.0/docs/relnotes/demos.html)			        Included in JDK and on java.sun.com|*docs*|
|----|----|
|| |
|[Java Web Start Samples](https://docs.oracle.com/javase/1.5.0/docs/relnotes/samples.html)			        Included in JDK|*docs*|
|| |
|[Java Examples](http://javaalmanac.com)			        Java Developers Almanac|*website*|
|| |
|[The Java Tutorial](http://java.sun.com/docs/books/tutorial/index.html)			        An example-filled guide to the Java programming language and core APIs.|*website*|
|| |
|[Effective Java](http://java.sun.com/docs/books/effective/index.html)			        Best selling guide about best programming practices.|*website*|
|| |
|[Training for the Java programming language](http://java.sun.com/learning/index.html)			        Directory of various training resources.|*website*|
|| |
|[On-Line Courses for the Java Programming Language](http://developer.java.sun.com/developer/onlineTraining)			        Courses from the Java Developer ConnectionSM.|*website*|
|| |
|[The Java Class Libraries](http://java.sun.com/docs/books/chanlee)			        Code examples for the Java 2 Platform API:![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[1.2 Supplement](http://java.sun.com/docs/books/chanlee/supplement/examples.html)![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[1.1 Packages: java.lang, java.net, java.text, java.util, java.math](http://java.sun.com/docs/books/chanlee/second_edition/vol1/examples.html)![](https://docs.oracle.com/javase/1.5.0/docs/images/bullet-round-indented.gif)[1.1 Packages: java.applet, java.awt, java.beans](http://java.sun.com/docs/books/chanlee/second_edition/examples.html)|*website*|
|| |
|[The Java FAQ](http://java.sun.com/products/jdk/faq.html)			        Frequently asked questions about Java and the JDK.|*website*|
|| |
|[Code Conventions for the Java Programming Language](http://java.sun.com/docs/codeconv/index.html)			        Standards and styles for coding Java programs.|*website*|
|| |
|[Java Documentation in HTMLHelp and WinHelp Formats](http://java.sun.com/docs/windows_format.html)			        For Windows users.|*website*|




