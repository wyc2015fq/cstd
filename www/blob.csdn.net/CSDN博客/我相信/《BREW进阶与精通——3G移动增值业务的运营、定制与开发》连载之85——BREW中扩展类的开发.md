# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之85——BREW中扩展类的开发 - 我相信...... - CSDN博客





2010年09月29日 14:22:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1142








扩展类不作为独立的应用程序使用，手机用户不能直接特别购买、下载或删除它们，而是在使用此扩展类的应用程序的内部环境中处理这些功能。扩展类是一种特殊的应用，相当于应用开发商自己开发的库，在PC机上表现为DLL库，在终端上同样是mod执行文件。

扩展类应用从所有者的角度可以分为两种：公有扩展类和私有扩展类。公有扩展类由开发商开发和拥有，提供给其他开发使用并收费。私有扩展类可以由开发商开发和拥有，供给自己的一个或多个应用使用，与其他应用共享收益；也可以由运营商开发和拥有，由运营商发布，提供给开发商或合作伙伴使用；还可以由手机厂商开发和拥有，由手机厂商发布，给合作伙伴使用。



在扩展类创建时要注意实现的次序，首先要分配内存，然后是初始化虚表，最后是初始化成员变量和所需的其他代码，示例如下：

if(nSize < sizeof(MyExtension)) nSize += sizeof(MyExtension);

If ((pMe = (MyExtension *)MALLOC(nSize + sizeof(VTBL(IMyExtension)))) == NULL)

return ENOMEMORY;

// initialize the data members

pMe->m_nRefs = 1;

pMe->m_MyData1 = 0;

pMe->m_MyData2 = 0;

扩展类中其他函数的实现与一般应用中的方法实现相同。

在使用扩展类时相当于一个接口的调用，只需包含扩展类的头文件，然后创建扩展类的接口实例，就可以正常使用扩展类中的函数了。例如：

If(ISHELL_CreateInstance(pMe->m_pIShell,AEECLSID_MYEXTENSION,(void **)&pExtCls))

return FALSE;

IEXTCLS_MyFunction1(pExtCls, 0);

IEXTCLS_Release(pExtCls);

一个应用和扩展类间的通信与前面讨论的应用间的通信方法是类似的。





