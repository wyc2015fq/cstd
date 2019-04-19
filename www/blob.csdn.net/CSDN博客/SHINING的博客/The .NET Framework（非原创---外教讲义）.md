# The .NET Framework（非原创---外教讲义） - SHINING的博客 - CSDN博客
2005年09月03日 10:54:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1977标签：[.net																[application																[web																[microsoft																[service																[asp](https://so.csdn.net/so/search/s.do?q=asp&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=.net&t=blog)
个人分类：[技术综合区](https://blog.csdn.net/snsn1984/article/category/144823)

**    The .Net Framework**
**Topics**
Understanding .NET Framework
Common Language Runtime (CLR)
Unified Programming Classes
Active Server Pages+ (ASP+ or ASP.NET)
Basic Architecture of .NET Framework
Common Language Specification
Understanding ASP.NET in detail
Understanding Unified Programming Classes in detail
Security in .NET
**Objectives**------------───────────────────────────────────
After studying this chapter, the students should be able to:
Understand the .NET Framework
Understand the constituents of .NET Framework such as CLR, Unified 
Programming Classes and ASP.NET
Understand the basic architecture of .NET framework
Understand security implementation in .NET framework
───────────────────────────────────┘**Common Language Runtime(CLR)**
It consist of three parts
lThe Framework classes or Unified Programming Classes and
lASP.NET.
lThe Common Language Runtime(CLR)
The .NET Framework forms the infrastructure for the overall .NET Platform. The common language runtime and class libraries (including Windows Forms, ADO.NET, and ASP.NET) combine together to provide services and solutions that can be easily integrated within and across a variety of system. The .NET Framework provides a fully managed, protected, and feature-rich application execution environment, simplified development and deployment, and seamless integration with a wide variety of languages.
Common Language Runtime (CLR)
The .NET Framework provides a run-time environment called the Common Language Runtime, which manages the execution of code and provides services that make the development process easier.
Compilers and tools expose the runtime's functionality and enable you to write code that benefits from this managed execution environment.
The Common Language Runtime makes it easy to design components and applications whose objects interact across languages. Objects written in different languages can communicate with each other and their behaviors can be tightly integrated. For example, you can define a class, then, using a different language, derive a class from your original class or call a method on it. You can also pass an instance of a class to a method on a class written in a different language. This increases-language integration because language compilers and tools that target the runtime use a common type system defined by the runt[me, and they follow the runtime's roles for defining new types, as well as creating, using, persisting, and binding to types.
Code that you develop with a language compiler that targets the runtime is called managed code; it
Benefits from features such as cross-language integration, cross-language exception handling,
Enhanced security, versioning and deployment support, a simplified model for component interaction debugging and profiling services.
**Unified Programming Classes**
The .NEI' Framework provides a unified, object-oriented, hierarchical, extensible set of class libraries APIs) for developers to use. Developers of C++ use the Microsoft Foundation Classes, Java developers of Java use the Windows Foundation Classes, and the developers of Visual Basic use Visual Basic APIs. The .NET Framework unifies the disparate frameworks Microsoft has today. As a result, developers no longer have to learn multiple frameworks to do their work. But more than that, by creating a common set of APIs across all programming languages, the .NET Framework enables cross-language inheritance, error handling, and debugging, in effect, all programming languages, from J Script to C++, become equal and developers are free to choose the language that they want to use.
**Active Server Pages+ (ASP+ or ASP.NET)**
ASP+ builds on the programming classes of the .NET Framework, providing a Web application model in the form of a set of controls and infrastructure that make it simple to build Web applications. Developers are exposed to a set of ASP+ controls that encapsulate common Hypertext Markup Language (HTML) user interface widgets such as text boxes, drop-down menus, and so on. Thesecontrols actually run on the Web Server and project their user interface as HTML to a browser. On the'server, the controls expose an object-oriented programming model that brings the richness of object oriented programming to the Web developer. ASP+ also provides infrastructure services such as session state management and process recycling that further reduce the amount of code a developer must write and increase application reliability. ASP+ also uses these concepts to enable developers todeliver software as a service. Using ASP+ Web Services features, ASP+ developers can simply write .net programs.
The upper-most layer includes all the languages supported by the .NET framework.. At present ,NET framework supports almost 20 languages, At the first glance it seem as if all the technologies participating in the .NET framework are more or less Microsoft products, But it is not true. Some third party vendor can provide the web services supported by the ASP.NET layer. Also since .NET framework is more of a specification, technology vendors can use these specifications to develop their own products such as SDKs, IDEs, Complilers etc. for windows or other platforms. All the five layers are fully supported in the Visual Studio,NET IDE provided
by Microsoft with the .NET framework, Although you can write your code in any text editor such as notepad, Visual Studio gives you an environment in which you can develop applications easily and faster than in the conventional programming tools. Now let us have a detailed look at the components involved. Now let us discuss some of these components in detail,
**Win Forms Application Model**
On top of the services framework sit two application models: the Windows (Win Forms) application model and the Web application model. The Win Forms application model is quite similar to existing Windows-based forms packages. It allows developers to take advantage of the rich user interface features of Windows. Win Forms also takes advantage of the Microsoft .NET Framework runtime to reduce the total cost of ownership for Windows-based client applications. Since most of the common services are provided by the runtime, the client application has to only provide application-specific logic. Moreover. Client applications developed using Win Forms can avail of the built-in security model of the runtime.
Application deployment and versioning is quite simple, as has already been seen. Applications do not have to contend with version conflicts while using components.
**Web Application Model**
The Web application model also sits on top of the services framework. It basically comprises of Web services and Web applications.
**1. Web Services**
A Web Service is simply an application that is delivered as a service using Internet standards. It has its own URL and can be requested directly by the client. The application is executed and the
requested information is returned to the client. A Web service can be integrated with other Web
services to provide composite functionality to clients. It thus integrates component-based technologies and the Web to provide services to clients. The client only needs to know which Web service to access and the URL of that Web service t6 access it. The client is not concerned with the actual implementation of the Web service. A Web Service thus represents black-box functionality. The black-box provides well-defined interfaces, called contracts that describe the services provided. These services can be used either locally or remotely along with application-specific code to develop custom Web applications. For example, an online store can use Login services, authentication services, personalization focalization services, cataloging services, payment processing services and shipment services to do its business. It may package all these services together to give a distinct look and feel to its site.
The black-box functionality of Web services is akin to that of component technologies. But. Component technologies use specific protocols such as DCOM. RMI. or IIOP These protocols require, specific and homogeneous infrastructures on both the client and service machines. But, on the Intermit. it is very difficult to control or even know the client-side environment. Hence, Web services cannot use the protocols used by component technologies to communicate with clients. Instead, they use Web protocols and data formats such as HTTP and XML.
Web services are also able to cater to clients independently of language, platform, and object model with the help of Web Service contracts. A contract only describes the messages the Web Service accepts and generates and the protocols that it supports. It does not specify the implementation of the service. This implies that different vendors can implement a Web service differently as long as they honor the contract. Likewise. vendors of Web service consumers (clients) can implement the client, as they want to as long as they honor the contract.
While Web Services and Web service consumers are independent of each other and interface only
through a contract, using standard Web protocols, they also require some standard language to
Communicate. Currently, XML is used as such a standard (yet extensible) (meta)-Language to describe data and commands. The Simple Object Access Protocol (SOAP) is an industry standard XML-based protocol that can be used for representing data and commands in an extensible way. It can be used to exchange structured and typed information on the Web service contracts are described using the XML-based Service Contract Language (SCL) (previously known as Service Description Language, SDL). The message formats within the contracts can be specified using SOAP. Web service contracts can be published and discovered in a
standard way using the Disco specification.
These standards ensure that different Web services can be accessed in a uniform manner by both
developers and development tools.
**2.Web Applications**
Microsoft .NET Framework provides a common application model for Web applications irrespective of the language and technology used to build the applications. A Web application is a combination of Web pages on Web services. Each Web page and Web service can have its own URL. All these individual URLs are clubbed together and connected to a base URL, which is the URL of the starting point of the application. The starting point could be a Web page or a Web service.
The .NET Framework provides programming languages such as Visual Basic and C# as well as a
server-side technology, Active Server Pages+ (ASP+).
**Common Language Specification**
Since there are many languages, there are problems related to the basic architecture of these
languages as the data type might be different or even if they employ same kind of data types their size might vary. An int uses 2 bytes in C++ while it uses 4 bytes in Java. This may lead to serious problems in multiple language domains. CLS has been designed as solution for this problem 
The Microsoft .NET Framework is built on the type system that the runtime defines. It supplies built-in primitive types as well as other types that you can use or derive from when building your component or application. The primary design challenge in creating a common language specification is choosing the right size subset large enough that it is properly expressive and small enough that all languages can reasonably, accommodate it, Because the CLS is about language interoperability, its rules apply only to "externally visible" items. The CLS assumes that language interoperability is important only across the assembly boundary that is, within a single assembly there are no restrictions as to the programming techniques that are used.
A type is CLS-compliant if all its publicly accessible parts (those classes, interfaces, methods, fields, properties, and events that are available to code executing in another assembly) either have
signatures composed only of CLS-compliant types, or are specifically marked as not CLS-compliant
A tool is a CLS-compliant consumer tool if it can completely use any CLS-compliant type. That is, call any CLS-compliant method; create an instance of any type CLS-compliant, read and modify any CLS compliant field, etc. A tool is a CLS-compliant extender tool if it is a consumer tool and, in addition, can extend any (non-sealed) CLS-compliant base class, can implement any CLS-complaint interface, and can place any CLS-compliant custom attribute on all appropriate elements of metadata. It must also be able to define new CLS-complaint interfaces.
While many things contribute to exclude a CTS concept from the CLS, one should be explicitly called out: any construct that would make it impossible to rapidly verify code is excluded from
the CLS. This allows all CLS-compliant languages to produce verifiable code if they so choose.
Language compilers and tools expose the runtime's functionality in ways that are intended to be useful and intuitive to their developers. This means that some features of the runtime might be more noticeable in one environment than in the other. Therefore, how you experience the runtime depends on which language compilers or tools you use. Let us now discuss and understand Unified programming classes here.
**Understanding ASP.NET in Detail**
ASP.NET (or ASP+) takes advantage of the common language runtime and services framework to
provide a reliable, robust, scalable environment for Web applications. It provides services, such as
state management services and higher-level programming models, such as ASP+ Web Forms and
ASP+ Web Services, to simplify application development. The CLR assembly model simplifies
application deployment.
The core of ASP+ is composed of the HTTP runtime, which processes HTTP requests. It is managed code that runs within an unmanaged host process, such as LINUX server machines or Microsoft internet Explorer on client machines. It processes incoming HTTP requests by resolving the URL of the requested application and dispatching the request to the specified application. It is multithreaded and processes requests asynchronously. Hence, it cannot be blocked by bad application code. It is also capable of automatically recovering from access violations, memory leaks, deadlocks, etc,
Once an HTTP request reaches the requested application, it is routed through a pipeline of HTTP
modules to a request handler. HTTP modules and request handlers are managed classes that
implement specific interfaces defined by ASP+. HTTP modules generally implement low-level services such as security, state management, and tracing. Request-handlers generally implement higher-level programming models such as Web Services and Web Forms. The pipeline architecture enables services to be added and removed easily and from applications with the help of HTTP modules.
An application can have multiple request handlers, provided it has specific URLs for each request handler. But, all requests to the application are routed through the same pipeline. The Web is basically states and a Web server does not remember an HTTP request once it hasbeen processed. ASP+ enables applications to retain state at three levels: application, session, anduser. User state is useful for strong user preferences and other personalization information.
ASP+ provides caching services to improve performance. An output cache saves completely
rendered pages, and a fragment cache stores partial pages. Applications. HTTP modules, andrequest handlers can also store arbitrary objects in the cache as needed. ASP+ supports live updating of applications. An administrator does not need to shut down the Web server or even the application to update application files. Application files are never locked, so they can simply be overwritten. When the application files are updated, the system detects the file changes and gracefully switches over to the new version. The system launches a new instance of the updated application, and begins routing incoming requests to that application. When all outstanding requests being processed by the existing application instance have been handled, that instance is shut down.
**ASP+ Web Forms**
Web forms bring the ease of Windows application development to the Web. They are similar to Visual Basic-based Windows forms. They support traditional ASP syntax but enable separation of application code from user interface content. Web Forms support graphical controls that encapsulate common user interface elements, such as buttons and text boxes. These controls generate the user interface typically in the form of HTML. A graphical WYSIWYG development environment can hence be used for designing the layout of Web pages. ASP+ also supports more complex Web controls such as calendars and ad rotators. These controls can detect the type of client and generate appropriate content. Web Forms have to be capable of responding to user interaction. For this, they take advantage of the ASP+ services, which provide a component-based, event-driven programming model. ASP+ also provides state management and data access services.
The separation of code and content enables ASP+ pages to be dynamically compiled into managed
classes for fast performance. ASP+ also provides thread safety by routing each incoming HTTP
request to a new page instance.
**ASP+ Web Services**
The ASP+ Web Services programming model can be used for building Web Services with ASP+. It simplifies development and is easy to use for developers who have worked with ASP or Visual Basic. Developers using this programming model do not need to understand HTTP, SOAP, SCL, or any other specifications for Web Services.
ASP+ Web Service files have the extension, .asmx and can be deployed as part of a Web application. They basically contain a definition of the class implementing the service or a reference to the class if it is defined elsewhere. These classes are derived from the ASP+ Web Service class and are managed by the CLR. They expose public class methods as Web Service methods by marking them with the Web Method attribute. The Web Service methods can be invoked by calling the ASMX file using either SOAP or HTT'P protocols. Developers do not have to define contracts for the Web Service as the contract files are automatically generated by ASP+ when reused by the caller. ASP+ Web Services are stateless. They can use the ASP+ State Management services if required. They run in the Web application model and get all the benefits of that model. ASP+ Web Services also enable generation of proxies for Web Services. These proxies hide the networking and marshaling code from the application code, so that the Web Service can be used just like any other managed code. Understanding Unified Programming Classes in Detail To facilitate interoperability between languages, the .NET Framework are CLS-compliant and can be used from any language compiler that targets the Common Language Runtime.
The .NET Framework includes types the encapsulate data structures, perform I/O, give you access to information about a loaded class, and provide a way to invoke .NET Framework security checks, as well as classes that encapsulate exceptions, and other helpful; functionality such as disk access, server controls and rich GUI generation. These types are designed to be the foundation which .NET applications, .components, and controls are built.
The .NET Framework provides both abstract base classes and class implementations derived from
those base classes. You can use these derived classes 'as is" or derive your own classes from them.
It also provides interfaces and default implementations of those interfaces. To get the interface's
functionality, you can either implement the interface yourself or use or derive a class from one of the runtime-based classes that implements the interface.
The framework classes include standard base class libraries, such as collections, input/output, string, and numerical classes . It also includes base class, libraries that provide access to operating system services, such as graphics, networking, threading, globalization, and cryptography, and a data access class library. Apart from these, it provides classes that can be used by development tools, such as debugging and profiling services..NET Framework types are named using a dot-syntax naming scheme that connotes a naming hierarchy. This technique is used to logically group related classes together so that they can be searched and referenced more easily. For example, the System. Reflection.x Field no class is related to other classes that also use a System. Refiection.x naming pattern: all of the classes named with a System. Reflection prefix can be used to discover information about types at runtime. The part of the name up to the last dot (e.g., System. Reflection) is often referred to as the namespace name and the
last part (e.g., Field no) as the class name. The use of naming patterns to group related classes into
namespaces is a very useful way to build and document. class libraries. This naming syntax has no
effect on visibility, member access, inheritance, or binding. A namespace may be partitioned across
multiple assemblies and a single assembly may contain classes from multiple namespaces. It is the
assembly that provides the formal structure for visibility and versioning in the runtime.
The root namespace for the types in the .NET Framework is the System namespace. This namespace
includes classes that represent the base data types used by all applications: Object (the root of the
inheritance hierarchy), Byte, Char,-ATray,'lfit32, String, etc. Many of these types correspond to the
primitive data types that your language compiler uses. When you write a code using .NET Framework
types, you can, if you wish, use your language's corresponding type when a runtime-based type is
expected. Along with the base data types, there are almost 100 classes directly in the System namespace ranging from classes for handling exceptions and forming delegates to dealing with core runtime concepts such as application domains and the automatic memory manager,
The System namespace also contains 24 second-level namespaces. 
**Security in .NET**
The .NET Framework provides two mechanisms for protecting resources and code from unauthorized code and users: 
**Code access security**
It uses permissions to control the access code that has to protect resources and operations. It helps
protect computer systems from malicious mobile code and provides a way to allow mobile code to run safely. Code access security allows code to be trusted to varying degrees, depending on where the code comes from and on other aspects of the code's identity. Code access security also enforces these varying levels of trust on code, which minimizes the amount of code that must be fully trusted in order to run. Using code access security can reduce the likelihood that your code can be misused by malicious code.
**Role-based security**
It provides information needed to make decisions about what a user is allowed to do. These decisions can be based on either the user's identity or role membership or both. Roles are often used in financial business applications as a means of policy enforcement..NET Framework role-based security provides support for authorization by making information about the principal available to the current thread..NET applications can make authorization decisions based on either the principal's identity or role membership or both. A role is a named set of users that have the same privileges with respect to security,
**Flashback**
lThe .NET Framework forms the infrastructure for the overall .NET Platform.
lThe .NET Framework is an environment for building, deploying, and running Web Services andother applications.
lCommon Language Runtime manages the execution of code and provides services that make
The development process easier.
The .NET Framework provides a unified, object-oriented, hierarchical, extensible set of classLibraries(APIs)for developers to use.
lASP.NET builds on the programming classes of the .NET Framework, providing a Webapplication model in the form of a set of controls and infrastructure that make it simple to buildWeb applications.
The Common Language Runtime is the execution engine for .NET Framework applications.A third party vendor can provide the web services.
lThe CLS assumes that language interoperability is important only across the assemblyboundary.
lThe System namespace also contains 24 second-level namespaces.











