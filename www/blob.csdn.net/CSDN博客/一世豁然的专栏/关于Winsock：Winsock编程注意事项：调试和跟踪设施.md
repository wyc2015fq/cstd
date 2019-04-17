# 关于Winsock：Winsock编程注意事项：调试和跟踪设施 - 一世豁然的专栏 - CSDN博客





2018年10月11日 10:52:21[一世豁然](https://me.csdn.net/Explorer_day)阅读数：42








Windows Sockets 2应用程序开发人员需要隔离以下错误：
- 应用程序。
- Ws2_32.dll或其中一个兼容性填充程序DLL。
- 服务提供商。

Windows Sockets 2通过以下几个组件和功能满足了这一需求：
- 在Windows Vista及更高版本上集成支持Winsock跟踪。
- Windows Vista上Ws2_32.dll的专门设计的调试版本。
- 用于Windows Server 2003和Windows XP的独立原始调试和跟踪工具。



**使用Windows事件跟踪进行Winsock跟踪**

Windows Vista及更高版本中包含使用Windows事件跟踪（ETW）对Winsock跟踪的集成支持。 这是在Windows Vista及更高版本上跟踪Winsock调用的首选方法。 使用ETW的Winsock跟踪是轻量级的，适用于Windows的零售版本。 无需其他软件或组件。 只需在Windows Vista及更高版本上启用此功能。 有关更多详细信息，请参阅Winsock跟踪主题。



**使用Ws2_32.dll的调试版本**

Windows Vista上Ws2_32.dll的调试版本和Winsock跟踪的组合允许监视跨越Windows套接字2 API或SPI的所有过程调用，并在某种程度上进行控制。

如果将用于Windows Vista的Microsoft Windows软件开发工具包（SDK）版本安装到默认位置，则针对各种体系结构的Ws2_32.dll的调试版本位于以下文件夹下：

**C:\Program Files\Microsoft SDKs\Windows\v6.0\NoRedist**

应使用与Windows版本和要测试的Service Pack匹配的Ws2_32.dll的已检查版本。 请注意，可能已应用了更新测试系统上的Ws2_32.dll的安全修补程序。 适用于Windows Vista的Windows SDK和早期的平台软件开发工具包（SDK）DVD / CD订阅包括各种版本Windows的已检查版本。 您应该使用与Ws2_32.dll相同的选中版本作为正在测试的系统上使用的零售版本。 另请注意，在已检查的构建下运行的行为与使用零售构建运行时的行为不同。

注意 Windows Server 2008及更高版本的Windows SDK不再包含Ws2_32.dll的特殊调试版本。 开发人员应该使用ETW代替Winsock跟踪，因为此功能不需要调试版本。



**Windows Server 2003和Windows XP上的Winsock调试和跟踪工具**

Windows 8和Windows Server 2012之前的旧版Windows支持单独的原始调试和跟踪工具，该工具包含在Windows SDK和较旧的Platform SDK中作为示例。 调试/跟踪工具应仅用于不支持Winsock跟踪的Windows Server 2003和Windows XP。

如果Windows SDK for Windows 7安装到默认位置，则此原始Winsock跟踪功能将安装在以下文件夹中：

**C:\Program Files\Microsoft SDKs\Windows\v7.0\Samples\NetDs\winsock\dt_dll**

此文件夹中的DbgSpec.doc文件提供有关此原始跟踪工具的文档。 需要编译dt_dll文件夹中的示例代码以使用此工具。 开发人员可以自由地使用源代码来开发满足其特定需求的调试/跟踪DLL版本。

请注意，此原始Winsock跟踪功能仅适用于安装的Ws2_32.dll的调试版本。 因此，您需要获取与您正在测试的Windows版本和Service Pack相匹配的Ws2_32.dll的选中版本。

此原始dt_dll跟踪工具的限制是示例代码对每个Winsock函数调用使用全局锁（临界区）。 所以这个设施在处理竞争条件方面没有用。 需要对示例代码进行大量重写，以使此跟踪工具对处理大多数真正的Winsock问题（替换全局锁定）很有用。 此示例代码允许开发人员跟踪过程调用，过程返回，参数值和返回值。

开发人员可以使用此原始机制来跟踪过程调用，过程返回，参数值和返回值。 可以在过程调用或过程返回时更改参数值和返回值。 如果需要，可以防止或重定向过程调用。 通过访问此级别的信息和控制，开发人员可以更好地隔离应用程序，Ws2_32.dll或服务提供商中的问题。





