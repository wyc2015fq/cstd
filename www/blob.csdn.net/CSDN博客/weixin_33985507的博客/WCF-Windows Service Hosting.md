# WCF - Windows Service Hosting - weixin_33985507的博客 - CSDN博客
2015年07月08日 16:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
[WCF - Windows Service Hosting](http://www.tutorialspoint.com/wcf/wcf_windows_service_hosting.htm)
The operation of Windows service hosting is a simple one. Given below are the steps with requisite coding and screenshots that explain the process in an easy way.
在windows服务上托管wcf是一个简单的操作。
**Step-1**: Now let’s create a WCF service. Open Visual Studio 2008 and click New → Project and select Class Library from the template.
**Step-2**: Add reference System.ServiceModel to the project. This is the core assembly used for creating the WCF service.
**Step-3**: Next, we can create the ISimpleCalculator interface. Add the Service and Operation Contract attribute as shown below:
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ServiceModel;
namespace WCFHostedWindowsService
{
    [ServiceContract]
    interface ISimpleCalculator
    {
        [OperationContract]
        int Sum(int number1, int number2);
        [OperationContract]
        int Subtract(int number1, int number2);
        [OperationContract]
        int Multiply(int number1, int number2);
        [OperationContract]
        double Divide(int number1, int number2);
    }
}
```
**Step-4**: Implement the ISimpleCalculator interface as shown below:
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace WCFHostedWindowsService
{
    class SimpleCalculator : ISimpleCalculator
    {
        public int Sum(int number1, int number2)
        {
            return number1 + number2;
        }
        public int Subtract(int number1, int number2)
        {
            return number1 - number2;
        }
        public int Multiply(int number1, int number2)
        {
            return number1 * number2;
        }
        public double Divide(int number1, int number2)
        {
            if (number2 != 0)
            {
                return number1 / number2;
            }
            else
            {
                return 0;
            }
        }
    }
}
```
**Step-5**: Build the Project and get the dll. Now, we are ready with the WCF service. We are going to see how to host the WCF service in Windows service.
**Note**: In this project, it is mentioned that we are creating both Contract and Service (implementation) in the same project. However it is always a good practice if you have both in different projects.
**Step-6**: Open Visual Studio 2008 and Click New → Project and select Windows Service.
**Step-7**: Add 'WindowsServiceHostedService.dll' as reference to the project. This assembly is going to act as service.
**Step-8**: The OnStart method of the service can be used to write the hosting code for WCF. We have to make sure that we are using only one service host object. OnStop method is used to close the Service Host. The following code shows how to host the WCF service in Windows service.
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ServiceModel;
using System.ServiceModel.Description;
using System.ServiceProcess;
using WindowsServiceHostedService;
namespace WCFHostedWindowsService
{
    class WCFHostedWindowsService : ServiceBase
    {
        ServiceHost serviceHost;
        public WCFHostedWindowsService()
        {
            InitializeComponent();
        }
        protected override void OnStart(string[] args)
        {
            if (serviceHost != null)
            {
                serviceHost.Close();
            }
            //Create a URI to serve as the base address
            Uri httpUrl = new Uri("http://localhost:8090/WindowsServiceHostedService/SimpleCalculator");
            //Create ServiceHost
            serviceHost = new ServiceHost(typeof(SimpleCalculator), httpUrl);
            //Add a service endpoint
            serviceHost.AddServiceEndpoint(typeof(ISimpleCalculator), new WSHttpBinding(), "");//ISimpleCalulator  //ISimpleCalulator
            //Enable metadata exchange
            ServiceMetadataBehavior smb = new ServiceMetadataBehavior();
            smb.HttpGetEnabled = true;
            serviceHost.Description.Behaviors.Add(smb);
            //Start the Service
            serviceHost.Open();
        }
        protected override void OnStop()
        {
            if (serviceHost != null)
            {
                serviceHost.Close();
                serviceHost = null;
            }
        }
        /// <summary> 
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }
        #region 组件设计器生成的代码
        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            this.ServiceName = "Service1";
        }
        #endregion
    }
}
```
****Step-9**: **In order to install the service, we need to have the Installer class for the Windows service. So add a new Installer class to the project, which is inherited from the Installer class. Given below is the code that shows the Service name, StartUp type, etc. of the service.
**Step-10**: Build the project to get the executable file WCFHostedWindowsService.exe. Next, we need to install the service using the Visual Studio Command Prompt.
So open the command prompt by clicking Start→All Programs→Microsoft Visual Studio 2008→Visual Studio Tools→ Visual Studio Command Prompt. Using the install util utility application, you can install the service as shown below.
