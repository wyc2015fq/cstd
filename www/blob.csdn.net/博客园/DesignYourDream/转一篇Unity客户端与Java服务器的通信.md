# 转一篇Unity客户端与Java服务器的通信 - DesignYourDream - 博客园
# [转一篇Unity客户端与Java服务器的通信](https://www.cnblogs.com/designyourdream/p/4268114.html)
转自：http://www.programering.com/a/MTNxYDMwATQ.html
A few days ago a friend asked me about Unity3D inside Protobuf, was something to write this off until now, feel shy.
In this paper, the test environment:
System: WINDOWS 7 (third, 6), OS (fourth) X 10.9
Software: VS 2012 (third, 6), Eclipse (fifth, 6)
Hardware: iPad 2 (fourth), Macbook Pro Mid 2012 (step fourth)
**Article directory:**
1, About Protobuf C#
2, Why some Protobuf released to the iOS is not, even some in the PC are used?
3, Protobuf C# version of the manual processing
3.1, Create a C# project, the first manual create each through the Protobuf serialization or deserialization of data model classes, and then export the DLL
3.2, Create a serialized C# project, and then run the generated DLL
3.3, The above two projects generated DLL onto unity
4, In Unity deserialization Protobuf
5, The server Java with Protobuf
6, Too much bother?! The client will automatically handle Protobuf
1, About Protobuf C#
First of all, U3D Protobuf is using C# to achieve, so there are several optional C# implementation:
C#: http://code.google.com/p/protobuf-csharp-port
C#: http://code.google.com/p/protosharp/
C#: 
C#/.NET/WCF/VB: http://code.google.com/p/protobuf-net/
Here I used is the http://code.google.com/p/protobuf-net/ (you can download his code and tools in https://code.google.com/p/protobuf-net/downloads/list here), It is a relatively good, Provide a variety of platform support, After decompression in the "Full" directory can be seen in every platform support
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1JWFowNVdaRDlTZTBsbWRoSjNadkF6TnZVbWRzOTJjemxHWnYwVFBCMTBRR3RXVUNwRU1KOUNic2xtWnZBRE0wOFNaNmwyYzA1MmJtOUNWeXdVTk1aVFkxOENkdTltWnYwVFJFNUVOblJWWXY1RVdaSm5SeUVtZFJobFcxVlRhaXRtVHprVmRqSmpZenBrTU1aM2JFTkdNU2hVWXZRSGVsUjNMeTh5YXlGV2J5VkdkaGQzUHlZVE4wRVRPeFFUTTJFak14TVRNd0l6TDBWbWJ1NEdaek5tTG45R2JpNXladGwyTHZvRGMwUkhh.jpg)
See the inside of the unity, it will be ready to use our protobuf-net.dll.
**2, Why some Protobuf released to the iOS is not, even some in the PC are used?**
a, Protobuf uses JIT, namely in the run-time dynamic compilation, and the characteristics in the Unity release to the iOS is not supported. Therefore, cause you can run on PC, released to the iOS have a problem.
b, Protobuf is based on the.Net 2 framework, while Unity only supports.Net 2, or some characteristics more 2, and you release set a subset of.Net 2 in Unity. You only need to modify the latter can be set up in Player setting.
The above two can also be applied to other third party libraries, if you download a PC or C# inside the normal use of the library, in which U3D can not be used, so please check whether it is the result of the above two reasons.
**3, Protobuf C# version of the manual processing**
Know the above problems, as long as we choose a.Net2.0 Protobuf, then it is not a JIT, it can be used as normal.
Here is the idea of:
**3.1, Create a C# project, the first manual create each through the Protobuf serialization or deserialization of data model classes, and then export the DLL**
In the case of VS, first, create a class library project: "file" > " > " new " " > " " (remember to choose.Net framework 2)
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1JWFowNVdaRDlTZTBsbWRoSjNadkF6TnZVbWRzOTJjemxHWnYwVFBCMTBRR3RXVUNwRU1KOUNic2xtWnZBRE0wOFNaNmwyYzA1MmJtOUNWeXdVTk1aVFkxOENkdTltWnYwVFJFNUVOblJWWXY1RVdaSm5SeUVtZFJobFcxVlRhaXRtVHprVmRqSmpZenBrTU1aM2JFTkdNU2hVWXZRSGVsUjNMeTh5YXlGV2J5VkdkaGQzUHlZVE41UXpOME1UTTJFak14TVRNd0l6TDBWbWJ1NEdaek5tTG45R2JpNXladGwyTHZvRGMwUkhh.jpg)
The unity protobuf DLL add to project references
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1JWFowNVdaRDlTZTBsbWRoSjNadkF6TnZVbWRzOTJjemxHWnYwVFBCMTBRR3RXVUNwRU1KOUNic2xtWnZBRE0wOFNaNmwyYzA1MmJtOUNWeXdVTk1aVFkxOENkdTltWnYwVFJFNUVOblJWWXY1RVdaSm5SeUVtZFJobFcxVlRhaXRtVHprVmRqSmpZenBrTU1aM2JFTkdNU2hVWXZRSGVsUjNMeTh5YXlGV2J5VkdkaGQzUDJVVE01RVRNMU1UTTJFak14TVRNd0l6TDBWbWJ1NEdaek5tTG45R2JpNXladGwyTHZvRGMwUkhh.jpg)
Then assume that you have a class WorkerInfo is needed by Protobuf serialization and deserialization, then create a WorkerInfo class, as follows:
using System;
using System.Collections.Generic;
using System.Text;
using ProtoBuf;
namespace Com.YourCompany.Project.Proto.Module{
    [ProtoContract]
    public class WorkerInfo { 
        [ProtoMember(1)]
        public int workerId;
        [ProtoMember(2)]
        public int leftClosingTimeSec;
        [ProtoMember(3)]
        public int buildingId;
    }
}
Press Shift+ F6 DLL, the project's bin\Debug directory can be found ProtoModelDLL.dll. 
**3.2, Create a serialized C# project, and then run the generated DLL**
Also in the case of VS, first create a console application: "file" > " > " new " " > " " console application; (remember to choose.Net framework 2)
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1JWFowNVdaRDlTZTBsbWRoSjNadkF6TnZVbWRzOTJjemxHWnYwVFBCMTBRR3RXVUNwRU1KOUNic2xtWnZBRE0wOFNaNmwyYzA1MmJtOUNWeXdVTk1aVFkxOENkdTltWnYwVFJFNUVOblJWWXY1RVdaSm5SeUVtZFJobFcxVlRhaXRtVHprVmRqSmpZenBrTU1aM2JFTkdNU2hVWXZRSGVsUjNMeTh5YXlGV2J5VkdkaGQzUDNnVE00VWpOd1FUTTJFak14TVRNd0l6TDBWbWJ1NEdaek5tTG45R2JpNXladGwyTHZvRGMwUkhh.jpg)
Add the Protobuf and the 3.1 generation DLL to reference
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1JWFowNVdaRDlTZTBsbWRoSjNadkF6TnZVbWRzOTJjemxHWnYwVFBCMTBRR3RXVUNwRU1KOUNic2xtWnZBRE0wOFNaNmwyYzA1MmJtOUNWeXdVTk1aVFkxOENkdTltWnYwVFJFNUVOblJWWXY1RVdaSm5SeUVtZFJobFcxVlRhaXRtVHprVmRqSmpZenBrTU1aM2JFTkdNU2hVWXZRSGVsUjNMeTh5YXlGV2J5VkdkaGQzUDRFak0xUXpOd1FUTTJFak14TVRNd0l6TDBWbWJ1NEdaek5tTG45R2JpNXladGwyTHZvRGMwUkhh.jpg)
Written in the project generated in Program.cs:
using System;
using System.Collections.Generic;
using System.Text;
using ProtoBuf.Meta;
using ProtoBuf;
using ProtoBuf.Compiler;
using Com.YourCompany.Project.Proto.Module;
namespace ProtoModelSerializerCreator
{
    class Program
    {
        static void Main(string[] args)
        {
            var model = TypeModel.Create();
            model.Add(typeof(object), true);
            model.Add(typeof(WorkerInfo), true);
            model.AllowParseableTypes = true;
            model.AutoAddMissingTypes = true;
            model.Compile("ProtoModelSerializer", "ProtoModelSerializer.dll");
        }
    }
}
Then ctrl+ F5 operation, you can see the ProtoModelSerializer.dll in bin\Debug. 
**3.3, The above two projects generated DLL (ProtoModelDLL.dll and ProtoModelSerializer.dll) and protobuf-net.dll onto unity**
How to use the fourth step?
4, In Unity deserialization Protobuf
Because of the general game client requests data volume is simple, less, therefore we request is not directly in front of the binary request. The front end is received and returned by Protobuf, and so. This discussion Protobuf deserialization.
The following code is very simple, write a test code:
using UnityEngine;
using System.Collections;
using ProtoBuf.Meta;
using Com.YourCompany.Project.Proto.Module;
using System.IO;
using Com.Duoyu001.Proto.Building;
using Com.Duoyu001.Proto.Worker;
public class TestProto : MonoBehaviour
{
	// init
	void Start ()
	{
		byte[] dataFromServ = new byte[]{8, 233, 7, 16, 100, 24, 1};	//these bytes are generated by server
		
		RuntimeTypeModel serializer = BitchSerializer.Create ();
		System.IO.MemoryStream memStream = new System.IO.MemoryStream ();
		WorkerInfo w = new WorkerInfo ();
		serializer.Deserialize (memStream, w, w.GetType ());	//asign value to proto model
		
		Debug.Log (w.workerId + ", " + w.buildingId + ", " + w.leftClosingTimeSec);
	}
}
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1RM2NoVkVhMFYzYlQ5U2UwbG1kaEozWnZBek52VW1kczkyY3psR1p2MFRQQjEwUUd0V1VDcEVNSjlDYnNsbVp2QURNMDhTWjZsMmMwNTJibTlDVnl3VU5NWlRZMThDZHU5bVp2MFRSRTVFTm5SVll2NUVXWkpuUnlFbWRSaGxXMVZUYWl0bVR6a1ZkakpqWXpwa01NWjNiRU5HTVNoVVl2UUhlbFIzTHk4eWF5RldieVZHZGhkM1Aya2pNMFVqTTFVVE0yRWpNeE1UTXdJekwwVm1idTRHWnpObUxuOUdiaTV5WnRsMkx2b0RjMFJIYQ.jpg)
After running the Unity console output worker information. The dataFromServ byte array code in the content of the actual communication when the rear end should be returned. This test is not related to Socket communication of knowledge. 
**5, The server Java with Protobuf**
To see a client using Protobuf so much trouble, how will it end? In fact, the back-end is relatively simple, with the official Google support.
Download: https://code.google.com/p/protobuf/downloads/list
After decompression is such(2.5.0):
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1RM2NoVkVhMFYzYlQ5U2UwbG1kaEozWnZBek52VW1kczkyY3psR1p2MFRQQjEwUUd0V1VDcEVNSjlDYnNsbVp2QURNMDhTWjZsMmMwNTJibTlDVnl3VU5NWlRZMThDZHU5bVp2MFRSRTVFTm5SVll2NUVXWkpuUnlFbWRSaGxXMVZUYWl0bVR6a1ZkakpqWXpwa01NWjNiRU5HTVNoVVl2UUhlbFIzTHk4eWF5RldieVZHZGhkM1B4Y2pOeEFqTXhZVE0yRWpNeE1UTXdJekwwVm1idTRHWnpObUxuOUdiaTV5WnRsMkx2b0RjMFJIYQ.jpg)
Into the "protobuf-2.5.0\java" folder, There is a maven project, You create a protobuf-java-2.5.0.jar in the target directory will use the Maven clean install jar package., No Maven download here., I used to generate maven. This time to import your Java project. You can.
And then you write a proto file, called "protobuf-2.5.0\src" inside the protoc.exe is generated, it will help you to generate a java file. (see https://developers.google.com/protocol-buffers/docs/javatutorial), here I have to provide a bat, used to call the protoc to generate the java file, manual input words too much trouble. Save it to.Bat and then double-click can be run under Windows.
@echo off
echo ** setting runtime variable
REM _protoSrc is your proto file directory location
set _protoSrc=F:\project_proto_src\trunk\xgame-controllers\protos
REM protoExe is used for the proto generated from the Java protoc.exe program position
set protoExe=C:\Users\john\Desktop\protobuf-2.5.0\src\protoc.exe
REM java_out_file to store the generated Java file directory location
set java_out_file=F:\project_proto_src\trunk\xgame-controllers\src\main\java\
for /R "%_protoSrc%" %%i in (*) do ( 
	set filename=%%~nxi 
	if "%%~xi"  == ".proto" (
		%protoExe% --proto_path=%_protoSrc% --java_out=%java_out_file% %%i
	)
)
OK, you just put the generated Java copied to or directly into a Java project your source directory, then you can use the. For example: 
As said before WorkerInfo as an example 
package com.duoyu001.xgame;
import java.util.Arrays;
import com.duoyu001.xgame.worker.proto.WorkerInfoBuilder.WorkerInfo;
public class TestProto {
	public static void main(String[] args) {
		WorkerInfo w = WorkerInfo.newBuilder().setBuildingId(1)
				.setLeftClosingTimeSec(100).setWorkerId(1001).build();
		byte[] byteArray = w.toByteArray();
		System.out.println(Arrays.toString(byteArray));
	}
}
The console will output: 
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1RM2NoVkVhMFYzYlQ5U2UwbG1kaEozWnZBek52VW1kczkyY3psR1p2MFRQQjEwUUd0V1VDcEVNSjlDYnNsbVp2QURNMDhTWjZsMmMwNTJibTlDVnl3VU5NWlRZMThDZHU5bVp2MFRSRTVFTm5SVll2NUVXWkpuUnlFbWRSaGxXMVZUYWl0bVR6a1ZkakpqWXpwa01NWjNiRU5HTVNoVVl2UUhlbFIzTHk4eWF5RldieVZHZGhkM1B5WVROMElET3pZVE0yRWpNeE1UTXdJekwwVm1idTRHWnpObUxuOUdiaTV5WnRsMkx2b0RjMFJIYQ.jpg)
The careful students will find the byte and above the "8, 233, 7, 16, 100, 24, 1" is not quite the same. Second numbers are -23 and 233
In fact, this is only byte has no symbolic difference.
Their binary representation is: 11101001
**6, Too much bother?! The client will automatically handle Protobuf**
We see the client is not a class, will need to increase the code in two project in VS, and the rear end is directly based on the proto generated code file. So, it seems a bit unfair, and that before and after the end of may produce different. In fact, protobuf I personally feel that the biggest benefit:
a, A small amount of data
b, The proto generated code template, reduce the front end alignment
But now only reduces the working front end, did not decrease, so the second advantage is not obvious.
That would be fine. I'm not so satisfied. Therefore, I decided, front end to generate the CS file based on proto.
Therefore, I use Java to compile a Proto file analysis tools, and according to the proto to generate the CS file, and then used to construct the executive vs program bat invokes the vs command line, the last generation of two DLL.
I put the above command are integrated into a bat, so the bat task is:
The execution of a java program, proto files to generate CS files.
The use of vs interface to construct two vs projects, generating two DLL.
Through the SVN to the two DLL submitted to the client on the trunk.
Call it according to the proto Java generated bat fragments, java code generation.
Through the SVN to generate java code is submitted to the server on the trunk.
Therefore, in this way, now as long as the compiled proto file, and then double-click the bat, before and after the end of available Protobuf data object through the latest update SVN.
Bat operation
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1RM2NoVkVhMFYzYlQ5U2UwbG1kaEozWnZBek52VW1kczkyY3psR1p2MFRQQjEwUUd0V1VDcEVNSjlDYnNsbVp2QURNMDhTWjZsMmMwNTJibTlDVnl3VU5NWlRZMThDZHU5bVp2MFRSRTVFTm5SVll2NUVXWkpuUnlFbWRSaGxXMVZUYWl0bVR6a1ZkakpqWXpwa01NWjNiRU5HTVNoVVl2UUhlbFIzTHk4eWF5RldieVZHZGhkM1AyUVROd1VqTTFZVE0yRWpNeE1UTXdJekwwVm1idTRHWnpObUxuOUdiaTV5WnRsMkx2b0RjMFJIYQ.jpg)
According to the proto CS file is generated and compiled two vs projects, then the generated DLL submitted to SVN
![](http://www.programering.com/images/remote/ZnJvbT1jc2RuJnVybD1RM2NoVkVhMFYzYlQ5U2UwbG1kaEozWnZBek52VW1kczkyY3psR1p2MFRQQjEwUUd0V1VDcEVNSjlDYnNsbVp2QURNMDhTWjZsMmMwNTJibTlDVnl3VU5NWlRZMThDZHU5bVp2MFRSRTVFTm5SVll2NUVXWkpuUnlFbWRSaGxXMVZUYWl0bVR6a1ZkakpqWXpwa01NWjNiRU5HTVNoVVl2UUhlbFIzTHk4eWF5RldieVZHZGhkM1AzZ1RNeFVETjFZVE0yRWpNeE1UTXdJekwwVm1idTRHWnpObUxuOUdiaTV5WnRsMkx2b0RjMFJIYQ.jpg)
Java code is generated and submitted to SVN
**This step operation we are interested can try, there are problems discussed in this blog welcome.**

