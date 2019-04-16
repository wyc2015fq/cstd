# 使用 JNA 模拟C语言结构体的完整源代码 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月20日 19:11:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：29









## C语言源代码：

### 头文件


```cpp
#define MYLIBAPI  extern   "C"     __declspec( dllexport ) 

MYLIBAPI void say(wchar_t* pValue);

 struct UserStruct{
   long id;
   wchar_t*  name;
   int age;
};

MYLIBAPI void sayUser(UserStruct* pUserStruct);

struct CompanyStruct{
    long id;
   wchar_t*  name;
//  UserStruct*  users[100];
   UserStruct   users[100];
   int count;
};

struct CompanyStruct2{
    long id;
   wchar_t*  name;
  UserStruct*  users[100];
  // UserStruct   users[100];
   int count;
};

MYLIBAPI void sayCompany(CompanyStruct* pCompanyStruct);

MYLIBAPI void sayCompany2(CompanyStruct2* pCompanyStruct);
```


### 源文件

```cpp
#include "stdafx.h"
#include <locale.h>
#include <iostream>
#include "Out.h"

void  say(wchar_t* pValue){
     std::wcout.imbue(std::locale("chs"));
     std::wcout<<L"上帝说："<<pValue<<std::endl;
}

void sayUser(UserStruct* pUserStruct){
   std::wcout.imbue(std::locale("chs"));
   std::wcout<<L"ID:"<<pUserStruct->id<<std::endl;
   std::wcout<<L"姓名："<<pUserStruct->name<<std::endl;
   std::wcout<<L"年龄："<<pUserStruct->age<<std::endl;
}

void sayCompany(CompanyStruct* pCompanyStruct){
	 std::wcout.imbue(std::locale("chs"));
	 std::wcout<<L"ID:"<<pCompanyStruct->id<<std::endl;
	 std::wcout<<L"公司名称："<<pCompanyStruct->name<<std::endl;
	 std::wcout<<L"员工总数："<<pCompanyStruct->count<<std::endl;
     for(int i=0;i<pCompanyStruct->count;i++){
        sayUser(&pCompanyStruct->users[i]);
     }
}

void sayCompany2(CompanyStruct2* pCompanyStruct){
	std::wcout.imbue(std::locale("chs"));
	std::wcout<<L"ID:"<<pCompanyStruct->id<<std::endl;
	std::wcout<<L"公司名称："<<pCompanyStruct->name<<std::endl;
	std::wcout<<L"员工总数："<<pCompanyStruct->count<<std::endl;
     for(int i=0;i<pCompanyStruct->count;i++){
        sayUser(pCompanyStruct->users[i]);
     }
}
```



## Java源文件：

请先把JNA.jar导入classpath。



```java
package com.google.p.shendl.jna.net;
import com.google.p.shendl.jna.net.TestDll1Service.TestDll1.CompanyStruct;
import com.google.p.shendl.jna.net.TestDll1Service.TestDll1.CompanyStruct2;
import com.google.p.shendl.jna.net.TestDll1Service.TestDll1.UserStruct;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Structure;
import com.sun.jna.WString;


/**
 * @author 沈东良 Edward Shen shendl_s@hotmail.com
 * 2008-11-23 下午05:07:14
 * TestDll1.dll
 */

public class TestDll1Service {
    public interface TestDll1 extends Library {
       /**
        * 当前路径是在项目下，而不是bin输出目录下。
        */
       TestDll1 INSTANCE = (TestDll1)Native.loadLibrary("TestDll1", TestDll1.class);

       public void say(WString value);

       /*
        * 定义一个类，模拟C语言的结构
        * */

       public static class UserStruct extends Structure{

            public static class ByReference extends UserStruct implements Structure.ByReference { }
            public static class ByValue extends UserStruct implements Structure.ByValue { }

           public NativeLong id;
           public WString name;
           public int age;
       }

       public void sayUser(UserStruct.ByReference struct);

       public static class CompanyStruct extends Structure{
            public static class ByReference extends CompanyStruct implements Structure.ByReference { }
           public NativeLong id;
           public WString  name;
           //public UserStruct[] users;
           public UserStruct.ByValue[] users;
           //public UserStruct.ByValue[] users=new UserStruct.ByValue[100];
           public int count;

       }

      public   void sayCompany(CompanyStruct.ByReference pCompanyStruct);
      public static class CompanyStruct2 extends Structure{
         public static class ByReference extends CompanyStruct2 implements Structure.ByReference { }
           public NativeLong id;
           public WString  name;

           public UserStruct.ByReference[] users=new UserStruct.ByReference[100];
           public int count;

       }
      public void sayCompany2(CompanyStruct2.ByReference  pCompanyStruct);
    }
    /**
     *
     */
    public TestDll1Service() {

       // TODO Auto-generated constructor stub

    }

    /**
     * @param args
     */
    public static void main(String[] args) {
       // TODO Auto-generated method stub
       TestDll1.INSTANCE.say(new WString("Hello World!"));
       System.out.println("HHEEH我我们无法万恶");
       UserStruct.ByReference userStruct=new UserStruct.ByReference();
       userStruct.id=new NativeLong(100);
       userStruct.age=30;
       userStruct.name=new WString("沈东良");
       TestDll1.INSTANCE.sayUser(userStruct);
       System.out.println("AAAAAAAAAAAAAAAAAAAAAAAAAAA");
       CompanyStruct.ByReference companyStruct=new CompanyStruct.ByReference();
       companyStruct.id=new NativeLong(1);
       companyStruct.name=new WString("Google");
       companyStruct.count=9;
        companyStruct.users=new UserStruct.ByValue[100];
       //companyStruct.users=(UserStruct[])new UserStruct().toArray(100);
       UserStruct.ByValue userStructValue=new UserStruct.ByValue();
       userStructValue.id=new NativeLong(100);
       userStructValue.age=30;
       userStructValue.name=new WString("沈东良");
       for(int i=0;i<companyStruct.count;i++){
           companyStruct.users[i]=userStructValue;
           //companyStruct.users[i].id=new NativeLong(companyStruct.users[i].id.intValue()+i);
       }

       TestDll1.INSTANCE.sayCompany(companyStruct);
       /*
       System.out.println("++++++++++++++");
       UserStruct.ByReference pUserStruct=new UserStruct.ByReference();
       pUserStruct.id=new NativeLong(90);
       pUserStruct.age=99;
       pUserStruct.name=new WString("良少");
       TestDll1.INSTANCE.sayUser(pUserStruct);
       System.out.println("---------------------");
       UserStruct.ByValue userStructValue=new UserStruct.ByValue();
       userStructValue.age=10;
       userStructValue.id=new NativeLong(1);
       userStructValue.name=new WString("良子");
       TestDll1.INSTANCE.sayUser(userStructValue);
       */
       System.out.println("BBBBBBBBBBBBBBBBBBBBBBBB");

       CompanyStruct2.ByReference companyStruct2=new CompanyStruct2.ByReference();
       companyStruct2.id=new NativeLong(2);
       companyStruct2.name=new WString("Yahoo");
       companyStruct2.count=10;
      
       UserStruct.ByReference pUserStruct=new UserStruct.ByReference();
       pUserStruct.id=new NativeLong(90);
       pUserStruct.age=99;
       pUserStruct.name=new WString("良少");
        pUserStruct.write();

    //  TestDll1.INSTANCE.sayUser(pUserStruct);

       for(int i=0;i<companyStruct2.count;i++){
           companyStruct2.users[i]=pUserStruct;
           //companyStruct.users[i].id=new NativeLong(companyStruct.users[i].id.intValue()+i);
       }

       TestDll1.INSTANCE.sayCompany2(companyStruct2);

    }

}
```







 原文地址：[http://www.cnblogs.com/armlinux/archive/2008/12/25/2390983.html](http://www.cnblogs.com/armlinux/archive/2008/12/25/2390983.html)






