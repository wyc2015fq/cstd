# 安装并配置工具以使用iOS进行构建 - DoubleLi - 博客园







**Visual Studio 2015**






Visual Studio文档的新家是docs.microsoft.com上的[Visual Studio 2017文档](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://docs.microsoft.com/visualstudio&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhhrwI-iIF6vxTF1vUSUr2FMoTWimg) 。

有关Visual Studio 2017的最新文档，请参阅在docs.microsoft.com上[使用iOS安装和配置要构建的工具](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://docs.microsoft.com/visualstudio/cross-platform/install-and-configure-tools-to-build-using-ios&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiOu6vdZS3cX4zevW2WB6XTy4de7A) 。 您可以使用Visual C ++进行跨平台移动开发来编辑，调试和将iOS代码部署到iOS模拟器或iOS设备，但由于许可限制，代码必须在Mac上远程构建和运行。 要使用Visual Studio构建和运行iOS应用程序，您需要在Mac上设置和配置远程代理程序[vcremote](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3FLinkId%3D534988&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiXlTEAOvmblwcdbNEAdwnNgof5ow) 。 远程代理处理来自Visual Studio的构建请求，并在连接到Mac的iOS设备上或Mac上的iOS模拟器上运行应用程序。

|![System_CAPS_ICON_note.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC101471.jpeg) 注意|
|----|
|有关使用云托管的Mac服务而不是Mac的信息，请参阅[在云中构建和模拟iOS](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://taco.visualstudio.com/en-us/docs/build_ios_cloud/&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhhoahXiyQm-R8TiRgO4ghP11RJHPg) 。 这些说明适用于使用适用于Apache Cordova的Visual Studio工具进行构建。 要使用说明构建使用Visual C ++进行跨平台移动开发，请将vcremote替换为vs-mda-remote。|


一旦安装了使用iOS构建的工具，请参阅本主题了解如何在Visual Studio和Mac上快速配置和更新iOS开发的远程代理。

[先决条件](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#Prerequisites)

[安装适用于iOS的远程代理](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#Install)

[启动远程代理](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#Start)

[在Visual Studio中配置远程代理](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#ConfigureVS)

[生成一个新的安全PIN码](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#GeneratePIN)

[生成一个新的服务器证书](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#GenerateCert)

[在Mac上配置远程代理](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#ConfigureMac)


## 先决条件






要安装和使用远程代理为iOS开发代码，必须先具备以下先决条件：
- 
运行OS X Mavericks或更高版本的Mac电脑

- 
[Apple ID](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://appleid.apple.com/&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjzondpi0fdWkkV5UMBdYALAibIFQ)

- 
Apple的一个活跃的[iOS开发人员计划](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://developer.apple.com/programs/ios/&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiY0fc9PJ6PC8M1CrKW5NZ32b9Omg)帐户

- 
[Xcode 6](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://developer.apple.com/xcode/downloads/&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiaPrKPplSRWnlFmTJakIc0nR6FUg)

Xcode 6可以从App Store下载。

- 
Xcode命令行工具

要安装Xcode命令行工具，请在Mac上打开终端应用程序并输入以下命令：

`xcode-select --install`

- 
在Xcode中配置的iOS签名身份

有关获取iOS签名标识的详细信息，请参阅iOS开发者库中的[维护签名标识和证书](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://developer.apple.com/library/ios/documentation/IDEs/Conceptual/AppDistributionGuide/MaintainingCertificates/MaintainingCertificates.html&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhizhs4hDGKe9tP_QaSr4AoNyuMiyw) 。 要在Xcode中查看或设置您的签名身份，请打开**Xcode**菜单并选择**首选项** 。 选择**帐户**并选择您的Apple ID，然后选择**查看详细信息**按钮。

- 
如果您正在使用iOS设备进行开发，则会在Xcode中为您的设备配置供应配置文件

有关创建配置配置文件的详细信息，请参阅在iOS开发人员库中[使用成员中心创建配置配置文件](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://developer.apple.com/library/ios/documentation/IDEs/Conceptual/AppDistributionGuide/MaintainingProfiles/MaintainingProfiles.html&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjoWvggt2xufJ1mHQk3GxY93CdPKA#//apple_ref/doc/uid/TP40012582-CH30-SW24) 。

- 
[Node.js的](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://nodejs.org/&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjg4HVI2D00o7am_g_HW9s1boFwRQ)

- 
npm的更新版本

Node.js附带的npm版本可能不足以安装vcremote。 要更新npm，请在Mac上打开终端应用程序并输入以下命令：

`sudo npm install -g npm@latest`



## 安装适用于iOS的远程代理






当您为跨平台移动开发安装Visual C ++时，Visual Studio可以与[vcremote](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3FLinkId%3D534988&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiXlTEAOvmblwcdbNEAdwnNgof5ow)通信，在Mac上运行的远程代理可以传输文件，构建和运行iOS应用程序并发送调试命令。

在安装远程代理之前，请确保您已满足[先决条件](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#Prerequisites)并安装了[用于跨平台移动开发的Visual C ++](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/dn707598.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjGW_Cq6AhYfaXeH9HdXMVuZGLhBQ#InstallTheTools) 。

### 下载并安装远程代理
- 
从Mac上的终端应用程序中，输入：

`sudo npm install -g --unsafe-perm vcremote`

建议使用全局安装（ **-g** ）开关，但不是必需的。

在安装过程中，vcremote已安装，并且在您的Mac上激活了开发人员模式。 [Homebrew](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://brew.sh/&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhj1eh6q9qvdg_2C37gLV62xJc7aOw)和两个npm软件包，vcremote-lib和vcremote-utils也被安装。

|![System_CAPS_ICON_note.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC101471.jpeg) 注意|
|----|
|要安装Homebrew，您必须具有sudo（管理员）访问权限。 如果您需要在不使用sudo的情况下安装vcremote，则可以手动将Homebrew安装在usr / local位置，并将其bin文件夹添加到您的路径中。 有关更多信息，请参阅[Homebrew文档](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://github.com/Homebrew/homebrew/wiki/Installation&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhhMSXSQuFOsaOje-G7Od8C6ab6jBg) 。 要手动启用开发人员模式，请在终端应用程序中输入以下命令： `DevToolsSecurity –enable`|



如果更新到Visual Studio的新版本，则还必须更新到当前版本的远程代理。 要更新远程代理，请重复这些步骤以下载并安装远程代理。


## 启动远程代理






远程代理必须运行Visual Studio才能生成并运行您的iOS代码。 Visual Studio必须先与远程代理配对，然后才能进行通信。 默认情况下，远程代理以安全连接模式运行，这需要PIN与Visual Studio配对。

### 启动远程代理
- 
从Mac上的终端应用程序中，输入：

`vcremote`

这将启动具有〜/ vcremote默认构建目录的远程代理。 有关其他配置选项，请参阅[在Mac上配置远程代理](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#ConfigureMac) 。


首次启动代理程序时，以及每次创建新客户端证书时，都会向您提供在Visual Studio中配置代理程序所需的信息，包括主机名，端口和PIN。

![Use vcremote to generate a secure PIN](https://i-msdn.sec.s-msft.com/dynimg/IC829286.jpeg)

如果您打算使用主机名在Visual Studio中配置远程代理，请使用主机名从Windows ping Mac，以验证它是否可访问。 否则，您可能需要使用IP地址。

生成的PIN码一次性使用，仅在有限的时间内有效。 如果您在时间到期之前未将Visual Studio与远程代理配对，则需要生成新的PIN。 有关更多信息，请参阅[生成新的安全PIN](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#GeneratePIN) 。

您可以在不安全模式下使用远程代理。 在不安全模式下，远程代理可以与无需PIN的Visual Studio配对。

#### 禁用安全连接模式
- 
要在vcremote中禁用安全连接模式，请在Mac上的终端应用程序中输入以下命令：

`vcremote --secure false`


#### 启用安全连接模式
- 
要启用安全连接模式，请输入以下命令：

`vcremote --secure true`


一旦你启动了远程代理，你可以在Visual Studio中使用它，直到你停止它为止。

#### 停止远程代理
- 在运行终端窗口vcremote中，输入`Control+C`


## 在Visual Studio中配置远程代理






要从Visual Studio连接到远程代理，必须在Visual Studio选项中指定远程配置。

#### 从Visual Studio配置远程代理
- 
如果代理程序尚未在Mac上运行，请按照[启动远程代理程序中](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#Start)的步骤进行操作。 您的Mac必须运行Visual Studio的vcremote才能成功配对，连接并构建您的项目。

- 
在您的Mac上，获取Mac的主机名或IP地址。

您可以通过在终端窗口中使用**ifconfig**命令来获取IP地址。 使用活动网络接口下列出的inet地址。

- 
在Visual Studio菜单栏上，选择“ **工具”** ，“ **选项”** 。

- 
在“ **选项”**对话框中，展开“ **跨平台”** ， **C ++** ， **iOS** 。

- 
在“ **主机名”**和“ **端口”**字段中，输入启动时由远程代理指定的值。 主机名称可以是Mac的DNS名称或IP地址。 默认端口是3030。

|![System_CAPS_ICON_note.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC101471.jpeg) 注意|
|----|
|如果您无法使用主机名称ping Mac，则可能需要使用IP地址。|


- 
如果您在默认安全连接模式下使用远程代理，请选中**安全**复选框，然后在**Pin**字段中输入由远程代理指定的PIN值。 如果您在不安全的连接模式下使用远程代理，请清除**安全**复选框并将**引脚**字段留空。

- 
选择**配对**以启用配对。

![Configure vcremote connection for iOS builds](https://i-msdn.sec.s-msft.com/dynimg/IC829287.jpeg)

配对一直存在，直到您更改主机名或端口。 如果在“ **选项”**对话框中更改主机名或端口，要撤消更改，请选择“ **恢复”**按钮以恢复到先前的配对。

如果配对不成功，请按照[启动远程代理中](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#Start)的步骤验证远程代理是否正在运行。 如果自生成远程代理程序PIN码以来已过去太多时间，请按照Mac上[生成新安全PIN码中](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#GeneratePIN)的步骤操作，然后重试。 如果您使用的是Mac的主机名，请尝试在**主机名字**段中使用IP地址。

- 
更新**远程根目录**字段中的文件夹名称，以指定Mac上的主目录（〜）中的远程代理所使用的文件夹。 默认情况下，远程代理使用/ Users / `username` / vcremote作为远程根目录。

- 
选择**确定**保存远程配对连接设置。


每次使用时，Visual Studio都使用相同的信息连接到Mac上的远程代理。 除非您在Mac上生成新的安全证书或其主机名或IP地址更改，否则不需要再次将Visual Studio与远程代理配对。


## 生成一个新的安全PIN码






第一次启动远程代理时，生成的PIN在有限的时间内有效 - 默认情况下为10分钟。 如果您在时间到期之前未将Visual Studio与远程代理配对，则需要生成新的PIN。

#### 生成新的PIN码
- 
停止代理，或在Mac上打开第二个终端应用程序窗口，然后使用它输入命令。

- 
在终端应用程序中输入此命令：

`vcremote generateClientCert`

远程代理生成新的临时PIN。 要使用新的PIN配对Visual Studio，请重复[在Visual Studio](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#ConfigureVS)中[配置远程代理中](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#ConfigureVS)的步骤。



## 生成一个新的服务器证书






出于安全考虑，将Visual Studio与远程代理配对的服务器证书绑定到Mac的IP地址或主机名。 如果这些值更改，则必须生成新的服务器证书，然后使用新值重新配置Visual Studio。

#### 生成新的服务器证书
- 
停止vcremote代理。

- 
在终端应用程序中输入此命令：

`vcremote resetServerCert`

- 
当提示确认时，输入`Y`

- 
在终端应用程序中输入此命令：

`vcremote generateClientCert`

这会生成一个新的临时PIN码。

- 
要使用新的PIN配对Visual Studio，请重复[在Visual Studio](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#ConfigureVS)中[配置远程代理中](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ#ConfigureVS)的步骤。



## 在Mac上配置远程代理






您可以使用各种命令行选项配置远程代理。 例如，您可以指定端口来侦听构建请求，并指定要在文件系统上维护的最大构建数量。 默认情况下，限制为10个构建。 远程代理将删除在关闭时超过最大值的构建。

#### 配置远程代理
- 
要查看远程代理命令的完整列表，请在终端应用程序中输入：

`vcremote --help`

- 
要禁用安全模式并启用简单的基于HTTP的连接，请输入：

`vcremote --secure false`

使用此选项时，请清除**安全**复选框，并在Visual Studio中配置代理时将**引脚**字段留空。

- 
要为远程代理文件指定位置，请输入：

`vcremote --serverDir directory_path`

其中*directory_path*是Mac上放置日志文件，构建版本和服务器证书的位置。 默认情况下，此位置是/ Users / *username* / vcremote。 构建按此位置中的构建编号进行组织。

- 
要使用后台进程将`stdout`和`stderr`捕获到名为server.log的文件，请输入：

`vcremote > server.log 2>&1 &`

server.log文件可以帮助解决构建问题。

- 
要使用配置文件而不是命令行参数运行代理，请输入：

`vcremote --config config_file_path`

其中*config_file_path*是JSON格式的配置文件的路径。 启动选项及其值不能包含破折号。



## 也可以看看






[安装Visual C ++进行跨平台移动开发](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/dn707598.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjGW_Cq6AhYfaXeH9HdXMVuZGLhBQ)











