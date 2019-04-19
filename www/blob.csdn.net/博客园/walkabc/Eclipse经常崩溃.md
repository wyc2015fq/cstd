# Eclipse经常崩溃 - walkabc - 博客园
# [Eclipse经常崩溃](https://www.cnblogs.com/hitfire/articles/4542632.html)
　　最近Eclipse很容易崩溃掉，查看一下崩溃的原因就是
```csharp;gutter
#
# A fatal error has been detected by the Java Runtime Environment:
#
#  SIGSEGV (0xb) at pc=0x00007fbfbd6752a1, pid=12955, tid=140463811180288
#
# JRE version: Java(TM) SE Runtime Environment (8.0_45-b14) (build 1.8.0_45-b14)
# Java VM: Java HotSpot(TM) 64-Bit Server VM (25.45-b02 mixed mode linux-amd64 compressed oops)
# Problematic frame:
# C  [libsoup-2.4.so.1+0x6c2a1]  soup_session_feature_detach+0x11
#
# Failed to write core dump. Core dumps have been disabled. To enable core dumping, try "ulimit -c unlimited" before starting Java again
#
# If you would like to submit a bug report, please visit:
#   http://bugreport.java.com/bugreport/crash.jsp
# The crash happened outside the Java Virtual Machine in native code.
# See problematic frame for where to report the bug.
#
---------------  T H R E A D  ---------------
Current thread (0x00007fc04000a000):  JavaThread "main" [_thread_in_native, id=12956, stack(0x00007fc04781b000,0x00007fc04791c000)]
siginfo: si_signo: 11 (SIGSEGV), si_code: 1 (SEGV_MAPERR), si_addr: 0x0000000000000000
Registers:
RAX=0x00007fc040334fc0, RBX=0x0000000000000000, RCX=0x00007fc040b6bc40, RDX=0x0000000000000000
RSP=0x00007fc047919270, RBP=0x00007fc040b6bc40, RSI=0x00007fc040b6bc40, RDI=0x0000000000000000
R8 =0x0000000000000000, R9 =0x0000000000000009, R10=0x00007fc047919050, R11=0x00007fbfbd675290
R12=0x0000000000000000, R13=0x00007fbfbdb05628, R14=0x00007fc047919338, R15=0x00007fc04000a000
RIP=0x00007fbfbd6752a1, EFLAGS=0x0000000000010206, CSGSFS=0x0000000000000033, ERR=0x0000000000000004
  TRAPNO=0x000000000000000e
Top of Stack: (sp=0x00007fc047919270)
0x00007fc047919270:   0000000000000009 00007fbfbdb05628
0x00007fc047919280:   00007fc047919300 00007fbfbd8ceea5
0x00007fc047919290:   00007fbfbdb05628 00007fc031015994
0x00007fc0479192a0:   00007fc047919300 00007fc031015719
0x00007fc0479192b0:   00007fc04000a000 00007fc0310156e2
0x00007fc0479192c0:   00007fc0479192c0 00007fbfbdb05628
0x00007fc0479192d0:   00007fc047919338 00007fbfbdb09c30
0x00007fc0479192e0:   0000000000000000 00007fbfbdb05628
0x00007fc0479192f0:   0000000000000000 00007fc047919320
0x00007fc047919300:   00007fc047919380 00007fc031007c4d
0x00007fc047919310:   00000000fc1e0f50 00007fc03100f578
0x00007fc047919320:   00007fc040b6bc40 00007fc031007690
0x00007fc047919330:   0000000000000000 0000000000000001
0x00007fc047919340:   00007fc047919340 00007fbfbdb056c9
0x00007fc047919350:   00007fc0479193b0 00007fbfbdb09c30
0x00007fc047919360:   0000000000000000 00007fbfbdb05718
0x00007fc047919370:   00007fc047919320 00007fc047919398
0x00007fc047919380:   00007fc0479193f8 00007fc031007c4d
0x00007fc047919390:   0000000000000000 00007fc040b6bc40
0x00007fc0479193a0:   0000000000000000 0000000000000000
0x00007fc0479193b0:   0000000000000000 00007fc0479193b8
0x00007fc0479193c0:   00007fbfbdaf375e 00007fc047919480
0x00007fc0479193d0:   00007fbfbdaf9dc0 0000000000000000
0x00007fc0479193e0:   00007fbfbdaf3988 00007fc047919398
0x00007fc0479193f0:   00007fc047919470 00007fc0479194c8
0x00007fc047919400:   00007fc031007c4d 0000000000000000
0x00007fc047919410:   0000000000000000 0000000000000000
0x00007fc047919420:   0000000000000000 0000000000000000
0x00007fc047919430:   0000000000000000 0000000000000000
0x00007fc047919440:   00007fc040b6bc40 0000000000000000
0x00007fc047919450:   00000000fc239930 00000000fc237f38
0x00007fc047919460:   00007fc041a76120 00000000fc237868 
Instructions: (pc=0x00007fbfbd6752a1)
0x00007fbfbd675281:   83 c4 08 48 89 df 5b 48 89 ee 5d ff e0 66 90 55
0x00007fbfbd675291:   48 89 f5 53 48 89 fb 48 83 ec 08 e8 2f fd ff ff
0x00007fbfbd6752a1:   48 8b 3b 48 89 c6 e8 84 b5 fc ff 48 8b 40 18 48
0x00007fbfbd6752b1:   83 c4 08 48 89 df 5b 48 89 ee 5d ff e0 66 90 55 
Register to memory mapping:
RAX=0x00007fc040334fc0 is an unknown value
RBX=0x0000000000000000 is an unknown value
RCX=0x00007fc040b6bc40 is an unknown value
RDX=0x0000000000000000 is an unknown value
RSP=0x00007fc047919270 is pointing into the stack for thread: 0x00007fc04000a000
RBP=0x00007fc040b6bc40 is an unknown value
RSI=0x00007fc040b6bc40 is an unknown value
RDI=0x0000000000000000 is an unknown value
R8 =0x0000000000000000 is an unknown value
R9 =0x0000000000000009 is an unknown value
R10=0x00007fc047919050 is pointing into the stack for thread: 0x00007fc04000a000
R11=0x00007fbfbd675290: soup_session_feature_detach+0 in /usr/lib/x86_64-linux-gnu/libsoup-2.4.so.1 at 0x00007fbfbd609000
R12=0x0000000000000000 is an unknown value
R13={method} {0x00007fbfbdb05628} '_soup_session_feature_detach' '(JJ)V' in 'org/eclipse/swt/internal/webkit/WebKitGTK'
R14=0x00007fc047919338 is pointing into the stack for thread: 0x00007fc04000a000
R15=0x00007fc04000a000 is a thread
Stack: [0x00007fc04781b000,0x00007fc04791c000],  sp=0x00007fc047919270,  free space=1016k
Native frames: (J=compiled Java code, j=interpreted, Vv=VM code, C=native code)
C  [libsoup-2.4.so.1+0x6c2a1]  soup_session_feature_detach+0x11
Java frames: (J=compiled Java code, j=interpreted, Vv=VM code)
j  org.eclipse.swt.internal.webkit.WebKitGTK._soup_session_feature_detach(JJ)V+0
j  org.eclipse.swt.internal.webkit.WebKitGTK.soup_session_feature_detach(JJ)V+9
j  org.eclipse.swt.browser.WebKit.create(Lorg/eclipse/swt/widgets/Composite;I)V+830
j  org.eclipse.swt.browser.Browser.<init>(Lorg/eclipse/swt/widgets/Composite;I)V+81
j  com.adobe.flexbuilder.editors.derived.informationviewer.AbstractFlexBrowserControl.isAvailable(Lorg/eclipse/swt/widgets/Composite;)Z+12
j  com.adobe.flexide.as.core.asdoc.ASDocControlCreator.doCreateInformationControl(Lorg/eclipse/swt/widgets/Shell;)Lorg/eclipse/jface/text/IInformationControl;+1
j  org.eclipse.jface.text.AbstractReusableInformationControlCreator.createInformationControl(Lorg/eclipse/swt/widgets/Shell;)Lorg/eclipse/jface/text/IInformationControl;+20
j  org.eclipse.jface.text.AbstractInformationControlManager.getInformationControl()Lorg/eclipse/jface/text/IInformationControl;+176
j  org.eclipse.jface.text.AbstractInformationControlManager.internalShowInformationControl(Lorg/eclipse/swt/graphics/Rectangle;Ljava/lang/Object;)V+18
j  org.eclipse.jface.text.AbstractInformationControlManager.presentInformation()V+70
j  org.eclipse.jface.text.AbstractHoverInformationControlManager.presentInformation()V+64
j  org.eclipse.jface.text.TextViewerHoverManager.doPresentInformation()V+1
j  org.eclipse.jface.text.TextViewerHoverManager$5.run()V+4
J 7019 C2 org.eclipse.swt.widgets.Display.readAndDispatch()Z (71 bytes) @ 0x00007fc03230d96c [0x00007fc03230d420+0x54c]
J 8074% C1 org.eclipse.ui.internal.Workbench.runEventLoop(Lorg/eclipse/jface/window/Window$IExceptionHandler;Lorg/eclipse/swt/widgets/Display;)V (54 bytes) @ 0x00007fc0322788d4 [0x00007fc0322787a0+0x134]
```
　　原来是因为WebkitGtk造成了，在configuration/config.ini中添加下面一行即可解决问题。
```csharp;gutter
org.eclipse.swt.browser.DefaultType=mozilla
```
