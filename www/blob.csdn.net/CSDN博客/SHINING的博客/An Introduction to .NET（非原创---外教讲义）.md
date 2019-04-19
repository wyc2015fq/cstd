# An Introduction to .NET（非原创---外教讲义） - SHINING的博客 - CSDN博客
2005年09月03日 10:49:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1806
**An Introduction to .NET**
**Basic concept of .Net**
**Programming**
Set of instructions provided to computer is called programming.
**Program development cycle**
Program development cycle is divided into six steps.
**1- Analyze the problem**
Analyzing the problem means identifying specification and defining each program’s input, output and processing components.
**2-****Design programs.**
In this step, the solution to the problem is planned. The sequence of each module of is carefully prepared. Such a sequence of steps is called algorithm.
**3-****Code programs **
coding is the word used for writing the program lines. The programming codes are written in programming languages.
**4- Test and debug program**
Testing is the process of finding the errors in a program, and debugging is the process of correcting errors found during the testing process.
**5- Complete the documentation **
Documentation is the process of explaining the various steps of the program code into simple understandable language so that the codes of the program can be understood by other person **.**
**6- Manintain the program**
Maintaining the program means modifying and enhancing the program with different rfeatures according to the needs of the company for which the program is made
What is event and event driven programming?
Event 
In terms of programming event means occurring or happening of a user-action. Event driven programming responds to the many types of events. For example mouse click, mouse release and dragging of mouse are some examples of events.
What do you understand by GUI?
The full form of GUI is graphical user interface. It is a type of programming environment in which the commandsare represented in the form of pictures. For example save command, ok command and cancel command buttons of windows.
What do you understand by a message?
A message is the information/request sent to the application
What is the similarityand difference between option button and check boxes？
**The Evolution of Microsoft .NET**
As is the case with most of the technologies, .NET too was not developed overnight. Instead, .NET evolved over a period of years during which many technologies came and were found lacking somewhere that lead to the development of the other technologies. And there is no doubt that .NET is the latest in the league of these technologies. And there is not doubt that .NET is the latest in the league of these technologies and strategies. So let us have a glimpse at the road map that led to the development of .NET.
Microsoft’s MS-DOS operating system was initially developed for a very limited computer (in terms of processing capabilities) and therefore was designed to run only one application at a time. But with the passage of time and advent of new techniques, Intel Corporation developed more and more powerful CPUs, such as the 80286 and 80386. At that time Microsoft began envisioning the limitless possibilities of running multiple applications concurrently and this vision led to the development of the cooperatively multitasking Microsoft Windows environment. Further, it lead to the development of various technologies like OLE (Object Linking and Embedding). ODBC (Open Database Connectivity) and ADO (ActiveX Data Objects) for data exchange, which ultimately led to one of the most successful component architectures named COM (Component Object Model). Having achieved tremendous success in the desktop arena, Microsoft had the vision of making inroads into the Internet world. And this strategy had to be developed with web-centric environments in mind. The .NET strategy is nothing but Microsoft’s vision of future’s web-centric environments where the focus shifts from the desktop to the Internet.

MS-DOS
Microsoft Windows Environment
Microsoft Data Access Technologies
(OLE, ODBC, ADO, ActiveX)
Microsoft COM
(Component Object Model)
Microsoft .NET strategy







Fig. 1.1 The Road to .NET strategy
.NET is the latest vision about how people will arrange and display data from different sources distributed over the Internet. In a way this makes sense because the emphasis is on a user-centric approach. Simply put, .NET is Microsoft’s strategy for delivering software as a service. The .NET will enable developers to create programs that transcend device boundaries and fully harness the connectivity of the Internet. The .NET strategy will bring fundamental changes in the way companies interact with their customers and partners over the Internet.
**Microsoft .NET**
Microsoft.NET (formerly known as Next Generation Windows Services, NGWS) is the umbrella term for the Microsoft’s strategy to move from a client-centric model to a network-centric model. It can be best described as the initiative that will allow the Internet to be the basis of a new operating system. It frees us from the constraints of hardware by making user data available from the Internet. The .NET is important to users as it makes their information accessible across all devices. It is also important to developers because it will change the way they develop applications by allowing them to hook onto Web Services. Microsoft held the Professional Developers Conference (PDC) in Orlando, Florida where they revealed details about their next generation platform for Windows and Internet software development - .NET, people all across the globe have considered .NET as a broad but ambitious initiative that revolves around the .NET Framework. The .NET framework is the actual language and execution platform besides providing an extensive set of class libraries. Though there is no doubt that it seems broad and ambitious but it is certainly a revolutionary idea that introduces a completely new model of programming and deployment of applications.
The vision of .NET is globally distributed systems that use XML as the universal glue to allow functions running on different computers across an organization or across the world to come together in a single application. In this vision, systems from servers to wireless palmtops, will share the same general platform, with versions of .NET available for all of them, and with each of them able to collaborate transparently with others. The .NET also aims to make traditional business applications easier to develop and deploy. Figure 1.2 shows how applications and services can be interlinked in an Internet-based environment.
Devices
Browsers
App Code
Internet
Vertical and Portal Services
Application Specific Web Services
Building Block Web Services
OS and Local Services

Access
Application
Call Services


Fig. 1.2 The .NET Vision
**The .NET Experience**
Microsoft .NET can be thought of as an experience, which provides the end user a means to interact with devices, based on anytime, anywhere fundamental that users have been wishing for sometime now. From the users perspective, the .NET experience is a whole new world of something that allows him anytime anywhere access of devices. The Microsoft .NET experience is a dramatically more personal, integrated computing experience using Web services delivered through smart devices. The .NET experience will have features like:
lPersonal and integrated experience. In contrast to the large amount of information provided by provider, .NET experiences are centered on the user, integrating their data and preferences into a single application.
lInteractions delivered through smart devices. Users experience .NET through their interactions with smart devices. Smart devices are Web enabled appliances, such as personal computers, handheld computers, and smart phones. The software that makes the smart devices more intelligent in their interactions with users will enrich the .NET experience. Though the user may or may not get to know all this, but this will work for him behind the scenes and he will have experiences similar to the following situations:
Imagine you wanted to adjust the air conditioning in your house while you were away from home. You could use a .NET experience that controls your household utilities through a smart device. The device you use could be your desktop computer while you were at office, your smart phone while you were driving to the grocery store, or a kiosk while you were shopping for dinner ingredients.
As per the .NET vision, users will have the .NET experiences unique in the following aspects:
lEach of these devices generates a different interface for the .NET experience. The desktop computer might present a graphical map and accept mouse input. The smart phone could give you verbal options and ask you to push a button to specify your choice. The kiosk may have a touch screen and a small set of temperature options. You are using the same .NET experience to perform the same task, but you see a distinct interface, which is appropriate to the device.
l.NET experiences are location-independent. Taking advantage of the strengths of both locally installed and Web-based applications, .NET experiences can be accessed both online and offline.
l.NET experiences are user-centric. .NET experiences read a user’s preferences and profile information, stored in building block services, and automatically incorporate the user’s information and preferences into what is being presented. Based on user login ID on the Website, the .NET experience already knows where you live. User need not enter an address.
**The .NET Platform**
The .NET platform is the developer’s perspective of .NET wherein he views .NET as an amalgam of a set of services, specifications, guidelines and tools for incorporating the .NET vision. It includes the .NET infrastructure and tools to build and operate a new generation of smart Internet devices. As of the developer, .NET platform is something that helps him put the .NET vision into a reality and hence help the developer in providing the user with the .NET experience. It can be further categorized as composition of the following constituents:
l**.NET Framework:** The .NET Framework is an environment for building, deploying, and running Web Services and other applications. It consists of three main parts:
1.Common Language Runtime – The execution is an environment for .NET applications. The Common Language Runtime is a requirement for any application developed with the framework.
2.Framework classes – These APIs are provided as part of .NET. The .NET Framework includes classes, interfaces, and value types that help expedite and optimize the development process and give you access to system functionality.
3.ASP.NET – The web technology for the generation of dynamic web content under the .NET. Web applications developed using ASP.NET can connect to databases, interact with users and provide asynchronous services.
The .NET Framework provides two mechanisms for protecting resources and code from unauthorized code and users:
1.Code access security uses permission to control the access code that has to protect resources and operations. It helps protect computer systems from malicious mobile code and provides a way to allow mobile code to run safely.
2.Role-based security provides information needed to make decisions about what a user is allowed to do. These decisions can be based on either the user’s identity or role membership or both.
l**.NET Products:** The .NET products will include a whole range of tools and servers that rely on XML as a language to describe data and SOAP (Simple Object Access Protocol) as protocol for transmission of data between products. This includes Microsoft Windows.NET (with a core integrated set of building block services). MSN.NET, personal subscription services, Microsoft Office.NET, Microsoft Visual Studio.NET, and Microsoft bCentral for .NET.
l**.NET Services:** The .NET services will include all the web services and other corporate services provided by the third party vendors. A vast range of partners and developers will have the opportunity to produce corporate and vertical services built on the .NET platform.
The development tools provided as part of .NET platform will utilize the .NET Framework to allow developers to quickly build and deploy robust applications that take advantage of the new .NET Common Language Runtime (CLR) environment. Using the tools with the .NET Framework SDK provides:
1.A fully managed, protected, and feature rich application execution environment
2.Application integration with Active Server Pages (ASP)
3.Improved isolation of application components
4.Simplified application deployment
**Advantages of .NET**
There is no doubt that the .NET strategy offers new ways as regards how the applications will behave in future. In fact not only how they behave but how they are built will also change and these changes are going to make a move from the desktop to web environments. The .NET will offer the following advantages:
l.NET is multi-lingual – With the .NET platform we can use several languages, such as C++, Jscript.NET, VB.NET (alias VB 7) and C# (pronounced as Csharp), a new language that has emerged with .NET. ALL these languages are compiled via an intermediate binary code, which is independent of hardware and operating systems. This intermediate language called Microsoft Intermediate Language (MSIL) is then executed in the Common Language Runtime (CLR), the execution environment for .NET applications.
l.NET Applications are portable – Applications compiled as intermediate code are presented as Portable Executable (PEs). These (PEs) can be implemented over a vast range of hardware and software architectures: Intel PCs with Windows 9x, Windows NT4, Windows 2000 or future 64 bit Windows versions, microcontroller-based PDAs with PocketPC (e.g. Windows CE), and other operating systems.
lAll languages must comply with a common agreement – For a language to be eligible for the range of languages supported by the .NET platform, it must provide a set of possibilities and constructions listed in an agreement called the Common Language Specification (CLS). To add a language to .NET, all that is required is for it to meet the requirements of the CLS, and a compiler to convert that language into MSIL.
lManaged codes – All code in .NET is managed code because the runtime provides services that include automatic memory management, debugging support, enhanced security, and interoperability with unmanaged code, such as COM components. Earlier, the developers had to explicitly manage the memory in languages like C++.
lInteroperability – All languages supported by the .NET framework support a common set of programming classes as their base class library. This means that the developers will not have trouble while interoperating between languages since all of them support the same set of APIs and also have same data types as specified by the Common Type Specification (CTS). Interoperability is being achieved because of the fact that an assembly manifest containing metadata information is being generated at compile time.
Note: **Metadata** is the information about data that describes your code. An assembly is the functional unit for sharing and reuse that contains metadata information of a .NET component.
# The Working of .NET
With .NET, Microsoft is sending us a version of an Internet made up of an infinite number of interoperable Web applications, which together form a global service exchange network. These Web Services are based on the **Simple Object Protocol (SOAP)** and XML. Not only are these Web Services likely to develop on the Internet, but may also change the way we plan information systems in enterprises, by making SOAP systematically used as application integration middleware, playing the role of a simple but efficient standard. An enterprise information system could then also become a network of front and back-office applications, which interoperate through SOAP, and reciprocally, use the Web Services that they implement.
The .NET is a three-layered architecture. On the server side, it has operating systems such as Windows DataCenter. In the middle, it has XML, combined with the Simple Object Access Protocol (SOAP) to expose information in sources such as databases and spreadsheets so that developers can call them with XML. On the client side, it has operating systems that support XML parsing to display the information based on the tags assigned to it. Figure below shows the representation of how the .NET works and how SOAP and XML act as glue between different applications.






**The Microsoft .NET**
**Windows**
**Data**
**Center**

**Server**
**XML**
**Based**
**.NET**
**services**
**on the**
**.NET**
**enterprise**
**Windows**
**With .NET**
**Support**

**Client**
**S**
**O**
**A**
**P**
**SOAP**







**Microsoft .NET – A working scenario**
The .NET makes intensive use of XML, and places a lot ofemphasis on the SOAP protocol. Microsoft plans to introduce a new system of programming which will be based on the reuse of services. SOAP and Web Services are the cornerstones of the .NET platform.
According to .NET framework, all source code is first converted into an intermediate code ( known as IL Code) at compilation time. Before the IL Code is executed, it has to be converted to CPU-specific executable code. Once the CPU-specific exe is generated, it can be executed as a normal application. Figure below gives the details.
## Source code in any .NET language
## Machine independent IL code
## Machine dependent executable code




**.NET Executable flow**
**Summary:**
lMicrosoft .NET is Microsoft’s strategy for delivering software as a service.
lMicrosoft’s .NET is an initiative that revolves around the .NET Framework, which encompasses the actual language and executable platform besides providing an extensive set of class libraries.
lMicrosoft’s .net strategy is an approachtowards making Internet a true distributed computing platform by providing a framework that enables computers, devices, and services to collaborate.
lThe .NET was earlier named as Next Generation Windows Services (NGWS).
lThe primary objective of .NET is to provide developers with a means to create interoperable applications.
lThe Common Language Runtime (CLR) is the execution environment for .NET Framework applications.
lWith the .NET platform, one can use several languages, such as C++, Jscript, Visual Basic.NET (VB.NET), and C#.
lApplications designed using .NET are compiled into an intermediate code called IL code.
lFor a language to be eligible for the range of languages supported by the .NET platform, it must comply the Common Language Specification (CLS).
lApplications compiled as intermediate code are presented as Portable Executables (PEs).
lWeb Services provided by the .NET platform are based on the Simple Object Access Protocol (SOAP) and XML.
