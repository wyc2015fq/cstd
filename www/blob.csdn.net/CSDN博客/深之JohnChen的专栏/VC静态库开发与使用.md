# VC静态库开发与使用 - 深之JohnChen的专栏 - CSDN博客

2016年09月02日 15:58:19[byxdaz](https://me.csdn.net/byxdaz)阅读数：915


开发一个静态库。

1、 打开VC，新建工程->win32项目（LIB-TEST），在“应用程序设置”中选择“静态库”。

2、 导出接口使用extern "C" 标记，声明为C编译、连接方式的外部函数

比如：

/*

功能说明：初始化

参数说明：pLicenseFileName，表示License文件名称;pProgramName,表示程序名称

返回值：int类型 0，表示成功，非0，表示错误码

*/

extern "C" int Register_Init(char*pLicenseFileName="License.lic",char *pProgramName=NULL);

/*

功能说明：反初始化

参数说明:无

返回值：int类型 0，表示成功，非0，表示错误码

*/

extern"C" int Register_UnInit();

/*

功能说明：是否合法注册

参数说明:无

返回值：int类型 0，表示合法注册，非0，表示不合法注册

*/

extern"C" int Register_IsValidRegister();

3、在项目中山实现接口（或者新建一个.cpp文件）。

int Register_Init(char*pLicenseFileName,char *pProgramName)

{

//实现处理…

}

4、生成debug和release静态库，分别不同命名。

![](https://img-blog.csdn.net/20160902155658869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5、如果想在静态库中使用对话框的话，需要一个接口，设置外面的主资源句柄，以便静态库使用。

void Lib_Init(HINSTANCE hMainResource)

{

    HINSTANCE hOldResrouce = (HINSTANCE)AfxGetResourceHandle();

    AfxSetResourceHandle((hMainResource);

    m_pDlgTip = new CAuthenticationTipDialog();//对话框

    m_pDlgTip->Create(CAuthenticationTipDialog::IDD,NULL);

    ...

    AfxSetResourceHandle(hOldResrouce);

}

工程引用静态库

1、 定义接口，即静态库对外的函数。

2、 debug和release分别引用不同的debug和release静态库。

3、 为了保证编译不出错误，确保本工程和静态库的运行时库一致。

![](https://img-blog.csdn.net/20160902155730198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

