# ACE Service Configurator框架 - 深之JohnChen的专栏 - CSDN博客

2011年12月05日 20:46:08[byxdaz](https://me.csdn.net/byxdaz)阅读数：2124标签：[service																[框架																[object																[class																[stream																[delete](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[ACE网络编程](https://blog.csdn.net/byxdaz/article/category/628882)

http://docs.huihoo.com/ace_tao/ACE-2002-12/Part-Two/Chapter-8.htm
**概述**

ACE Service Configurator框架是Component Configurator模式的一种实现。使用它可以在运行时动态地配置服务和流，无论它们是动态地链接进你的程序的，还是其对象是从共享库中动态加载的。你既可以配置服务（对象），也可以配置流（基于配置文件而非在编译时装配模块）。

## 配置静态服务

静态服务是代码已经链接进可执行的服务。ACE Service Configurator使我们能把服务构建成静态链接的、可配置的服务：将所有代码都链接进可执行程序中，但是在框架显式地激活服务之前，服务不会实例化或者激活。

配置静态服务牺牲了一定的灵活性换取更好的安全性。对于某些服务来说――它们必须只含有受信任的、静态链接的服务（代码）――这是非常有用的。

激活静态服务可以从代码中设置激活方法，也可以在配置文件中进行配置。

服务要求被设计成为一个ACE_Service_Object的子类。我们将会实现一个最简单地服务：它什么都不做，除了在激活、挂起、恢复、移除时打印出一行消息。

### class MyStaticObj : public ACE_Service_Object 

{

public:

  MyStaticObj(ACE_Reactor *r = ACE_Reactor::instance())

  :ACE_Service_Object(r){;}

  virtual ~MyStaticObj(){;}

  //初始化。服务激活时调用。若返回－1表示出错，此时服务会被移除。

  virtual int init (int argc, ACE_TCHAR *argv[])

  {

  printf("---MyStaticObj::init\n");

  return 0;

  }

  //服务结束。移除时

  virtual int fini()

  {

  printf("---MyStaticObj::fini\n");

  return 0;

  }

  virtual int suspend(){

  printf("---MyStaticObj::suspend\n");

  return 0;

  }

  virtual int resume(){

  printf("---MyStaticObj::resume\n");

  return 0;

  }

};

**ACE_FACTORY_DEFINE (ACE_Local_Service, MyStaticObj)**

**ACE_STATIC_SVC_DEFINE (**

**  MyStaticObj-Discriptor,**

**  ACE_TEXT ("MyStaticObj"),**

**  ACE_SVC_OBJ_T,**

**  &ACE_SVC_NAME (MyStaticObj),**

**  ACE_Service_Type::DELETE_THIS**

**  | ACE_Service_Type::DELETE_OBJ,**

**  0 // This object is not initially active.**

**)**

**ACE_STATIC_SVC_REQUIRE (MyStaticObj-Discriptor)**

**说明**

后面三行是ACE Service Configurator设计的一些宏。ACE把已知的、可配置的静态服务放在一个内部仓库中。每个静态服务都必须用ACE提供的一些宏把一些薄记信息插入这个仓库。这样框架就能实例化一个MyStaticObj对象，以在服务激活时执行服务。

**ACE_FACTORY_DEFINE (CLS,SERVICE_CLASS)**

它生成不依赖于编译器的工厂方法，并且一致地管理动态内存

CLS是我们的程序/库用来导入/导出声明的标识符，如果服务工厂不需要导出到DLL外部，可以使用特殊符号ACE_Local_Service。

SERVICE_CLASS是从ACE_Service_Object派生的类的名称。

**ACE_STATIC_SVC_DEFINE (SERVICE_VAR,NAME,TYPE, FACTORY,FLAGS,ACTIVE)**

这个宏利用上一个宏展开的工厂方法创建一个类的实例，并且向ACE Service Configurator仓库登记服务的相关消息，这样我们可以在需要的时候对服务进行管理。

SERVICE_VAR是将被创建的对象的名称，即实例名。

NAME。一个不包含空格的文本串，在服务配置文件中将用该名称来标识这个服务。

TYPE.正在登记的对象类型。构建服务使用ACE_SVC_OBJ_T。

FACTORY。一个指针，指向用于创建服务对象的工厂函数。

FLAGS。指定在服务关闭时，怎样处置与服务相关的对象。

ACTIVE。如果为1，则服务会在程序启动时激活，但不能传给它任何参数。如果是0，服务就会根据服务配置文件中的指令进行初始化。

**ACE_STATIC_SVC_REQUIRE (SERVICE_VAR)**

确保在程序启动时，创建服务对象的一个实例。

**启动程序**

我们在main()函数中开始。

main.cpp

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])

{

  ACE_STATIC_SVC_REGISTER (MyStaticObj-Discriptor);

  ACE_Service_Config::open

  (argc, argv, ACE_DEFAULT_LOGGER_KEY, 0);

  ACE_Reactor::instance ()->run_reactor_event_loop ();

  return 0;

}

  ACE_STATIC_SVC_REGISTER (MyStaticObj-Discriptor);

这个宏是为了获得最好的移植性而调用的，在有的平台上不需要。但是建议使用它。ACE会视情况把它处理成一些有效操作或者空操作。

ACE_Service_Config类

open()：配置服务必须调用。在默认情况下，open()会尝试处理当前目录下的名为svc.conf的文件，读取其中的指令进行操作。第四个参数ignore_static_svcs默认定义为1。如果配置了静态服务，则需要把0传递过去。

**配置文件svc.conf**

如果现在编译运行这个程序，由于没有配置文件，程序将不会激活我们已经定义的静态服务。要激活这个服务，需要编辑配置文件。默认的文件是svc.conf。编辑内容（如下）之后，再运行就可以发现服务已经激活、挂起、恢复、移除了。

static MyStaticObj ""

suspend MyStaticObj

resume MyStaticObj

remove MyStaticObj

可以写入以下格式的命令对程序进行控制：

初始化静态服务：static svc-name [ “argc/argv options” ]：

svc-name为我们为静态服务配置的服务名称，后面跟随着argc/argv风格的参数。

动态链接和初始化服务：dynamic svc-name svc-type DLL-name:factory_func() [ “argc/argv options”]

svc-name为服务名称

svc-type指定服务的类型，可以是Service_Object*、Module*或者是Stream*。

DLL-name是含有factory_fun()符号的动态链接库的名称。

factory_func()是一个创建svc-type的对象的工厂方法。

完全移除服务：remove svc-name

挂起服务：suspend svc-name

恢复先前挂起的服务：resume svc-name

初始化一系列有序的层次相关的模块：stream svc-name ‘{‘module-list’};

**配置动态服务**

ACE Service Configurator框架更强大的功能表现在配置动态服务上。如果在运行时收到指示，动态服务器可以从共享库（DLL）中动态加载。这种能力允许你在运行时替换服务，从而提供极大的灵活性。

ACE Service Configurator框架简化了全部的冗余工作。我们只需要按照设计规格实现我们需要的服务，把它载入到一个动态链接库中，编辑配置文件即可。换句话说：我们只需要创建一个合乎规格的动态链接库，然后在上例的svc.conf里面添加一两行指令，再次运行该程序即可加载此动态服务，我们甚至不需要对该程序进行重新编译。

动态链接库Mydll

确定我们将要设计的动态链接库名字为Mydll

运行$ACE_ROOT/bin/generate_export_file.pl Mydll,将输出写入Mydll_Export.h中去。

设计我们的服务的类，在类的源文件中包含该文件，并将关键字

＃i nclude “Mydll_Export.h”

class  Mydll_Export MyDynamicObj : public ACE_Service_Object 

{

public:

  MyDynamicObj();

  virtual ~MyDynamicObj();

  virtual int init (int argc, ACE_TCHAR *argv[])

  {

  printf("MyDynamicObj::init------\n");

  return 0;

  }

  virtual int fini()

  {

  printf("MyDynamicObj::fini-----\n");

  return 0;

  }

};

ACE_FACTORY_DEFINE(Mydll,MyDynamicObj)

编译，并在svc.conf里面加入如下指令，再运行，发现动态服务已被加载、移除。

dynamic MyDynamicObj Service_Object* Mydll:_make_MyDynamicObj() ""

remove MyDynamicObj

实例参考：ACE_wrappers\examples\APG\Svc_Config 项目


