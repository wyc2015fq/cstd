# API Guide：绪论-系统权限 - 一世豁然的专栏 - CSDN博客





2017年04月18日 11:41:52[一世豁然](https://me.csdn.net/Explorer_day)阅读数：877








注意：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。





Android是一种特权分离的操作系统，每个应用程序都以不同的系统标识（Linux用户标识和组ID）运行。 系统的一部分也被分成不同的身份。 因此，Linux将应用程序从系统中隔离开来。




通过“权限”机制提供了更多细化的安全功能，该机制强制对特定进程可以执行的特定操作的限制，以及授予对特定数据片段进行临时访问的per-URI权限。





本文档介绍了应用程序开发人员如何使用Android提供的安全功能。 Android开放源代码项目中提供了更多的Android安全性概述。








一、安全架构

Android安全架构的中心设计点是，默认情况下，没有应用程序有权执行任何会对其他应用程序，操作系统或用户造成不利影响的操作。 这包括读取或写入用户的私人数据（如联系人或电子邮件），读取或写入其他应用程序的文件，执行网络访问，保持设备唤醒等等。





由于每个Android应用程序都在一个进程沙箱中运行，应用程序必须显式共享资源和数据。 他们通过声明对基本沙箱不提供的附加功能所需的权限来执行此操作。 应用程序静态声明所需的权限，Android系统会提示用户同意。





应用程序沙箱不依赖于用于构建应用程序的技术。 特别是Dalvik VM不是安全边界，任何应用程序都可以运行本机代码（请参阅Android NDK）。 所有类型的应用程序 - Java，本机和混合 - 都以相同的方式进行沙盒化，并且具有相同的安全程度。








二、申请签名

所有APK（.apk文件）必须使用其开发人员持有的私钥的证书进行签名。 此证书标识应用程序的作者。 证书不需要由证书机构签署; 一般情况下，Android应用程序使用自签名证书是绝对允许的。 Android中的证书的目的是区分应用程序作者。 这允许系统授予或拒绝应用程序访问签名级权限，并授予或拒绝应用程序与其他应用程序相同的Linux身份的请求。








三、用户ID和文件访问

在安装时，Android会为每个软件包提供不同的Linux用户ID。 在该设备的包装寿命期间，身份保持不变。 在不同的设备上，相同的包可能具有不同的UID; 重要的是每个包在给定设备上具有不同的UID。





由于安全执行在流程级别发生，所以任何两个软件包的代码通常不能在同一进程中运行，因为它们需要作为不同的Linux用户运行。 您可以使用每个包的AndroidManifest.xml的清单标签中的sharedUserId属性来分配相同的用户ID。 这样做的目的是为了安全起见——这两个包被视为相同的应用程序，具有相同的用户ID和文件权限。 请注意，为了保留安全性，只有两个具有相同签名（并请求相同的sharedUserId）的应用程序将被赋予相同的用户ID。





应用程序存储的任何数据将被分配该应用程序的用户ID，而其他程序包通常不可访问。 当使用getSharedPreferences（String，int），openFileOutput（String，int）或openOrCreateDatabase（String，int，SQLiteDatabase.CursorFactory）创建新文件时，可以使用MODE_WORLD_READABLE和/或MODE_WORLD_WRITEABLE标志来允许任何其他包读取/ 写文件。设置这些标志时，该文件仍然由您的应用程序拥有，但是其全局读取和/或写入权限已经被正确设置，以便任何其他应用程序可以看到它。








四、使用权限

默认情况下，基本的Android应用程序没有与其关联的权限，这意味着它不会对对用户体验或设备上的任何数据造成不利影响。 要使用设备的受保护功能，您必须在应用程序清单中包含一个或多个<uses-permission>标签。




例如，需要监视传入SMS消息的应用程序将指定：




```
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.android.app.myapp" >
    <uses-permission android:name="android.permission.RECEIVE_SMS" />
    ...
</manifest>
```





如果您的应用程序在其清单中列出了正常的权限（即，不会对用户的隐私或设备的操作造成很大风险的权限），系统将自动授予这些权限。 如果您的应用列出其清单中的危险权限（即可能影响用户隐私或设备正常运行的权限），则系统会要求用户明确授予这些权限。 Android提出请求的方式取决于系统版本以及您的应用程序定位的系统版本：


I、如果设备正在运行Android 6.0（API级别23）或更高版本，并且应用的targetSdkVersion为23或更高版本，则应用程序将在运行时请求用户的权限。 用户可以随时撤销权限，因此应用程序需要检查每次运行时是否具有权限。 有关在应用程序中请求权限的更多信息，请参阅“使用系统权限指南”。

II、如果设备正在运行Android 5.1（API级别22）或更低版本，或者应用程序的targetSdkVersion为22或更低版本，系统将要求用户在用户安装应用程序时授予权限。 如果您向应用程序的更新版本添加新权限，系统会在用户更新应用程序时要求用户授予该权限。 用户安装应用程序后，他们可以撤销权限的唯一方法是卸载该应用程序。




通常，权限失败将导致将SecurityException反回到应用程序。 但是，这并不能保证在任何地方都能发生。 例如，在方法调用返回后，sendBroadcast（Intent）方法检查数据正在传递给每个接收方的权限，因此如果有权限失败，则不会收到异常。 然而，在几乎所有情况下，系统日志将记录权限失败的消息。





Android系统提供的权限可以在Manifest.permission中找到。 任何应用程序也可以定义和执行自己的权限，因此这不是所有可能权限的全面列表。





在程序运行期间，可能会在许多地方执行特定许可：


I、在进入系统时，防止应用程序执行某些功能。

II、启动活动时，防止应用程序启动其他应用程序的活动。

III、发送和接收广播，以控制哪些人可以接收您的广播或谁可以向您发送广播。

IV、访问和操作内容提供时。

V、绑定或启动服务。




一）、自动权限调整


随着时间的推移，新的限制可能会添加到平台中，以便为了使用某些API，您的应用程序必须请求以前不需要的权限。 由于现有的应用程序可以免费获得对这些API的访问权限，Android可能会将新的许可请求应用于应用程序的清单，以避免在新平台版本上破坏该应用程序。Android是否应用程序可能需要权限是基于为targetSdkVersion属性提供的值来决定的。如果该值低于添加权限的版本，则Android会添加权限。





例如，在API级别4中添加了WRITE_EXTERNAL_STORAGE权限，以限制对共享存储空间的访问。 如果您的targetSdkVersion为3或更低版本，则此更新版本的Android会将此权限添加到您的应用中。





注意：如果权限自动添加到您的应用程序，您的Google Play上的应用程序列表会列出这些附加权限，即使您的应用程序可能不需要它们。





为了避免这种情况并删除您不需要的默认权限，请始终将targetSdkVersion更新为尽可能高。 您可以在Build.VERSION_CODES文档中看到每个发行版添加了哪些权限。








五、常规和危险的权限

系统权限分为几个保护级别。 要知道的两个最重要的保护级别是正常和危险的权限：


I、正常的权限涵盖了您的应用程序需要访问应用程序沙箱外的数据或资源的区域，但用户隐私风险甚至其他应用程序的运行风险很小。 例如，设置时区的权限是一个正常的权限。 如果一个应用程序声明它需要一个正常的权限，系统将自动授予该应用的权限。 有关当前正常权限的完整列表，请参阅常规权限。


II、危险的权限涵盖了应用程序想要涉及用户私人信息的数据或资源的区域，或者可能影响用户存储的数据或其他应用程序的操作。 例如，读取用户联系人的能力是一个危险的许可。 如果一个应用程序声明它需要一个危险的权限，用户必须明确地授予该应用的权限。




一）、权限组

所有危险的Android系统权限属于权限组。 如果设备正在运行Android 6.0（API级别23），并且应用的targetSdkVersion是23或更高版本，则当您的应用程序请求危险的权限时，会适用以下系统行为：


I、如果应用程序请求其清单中列出的危险许可，并且应用程序当前没有权限组中的任何权限，系统将向用户显示描述应用程序要访问的权限组的对话框。 对话框不会描述该组中的特定权限。 例如，如果应用程序请求READ_CONTACTS权限，则系统对话框只是说应用程序需要访问设备的联系人。 如果用户获得批准，系统会向应用程序提供其请求的许可。

II、如果应用程序请求其清单中列出的危险许可，并且应用程序在同一权限组中已经具有另一个危险的权限，则系统将立即授予权限，而无需与用户进行任何交互。 例如，如果应用程序先前已请求并被授予READ_CONTACTS权限，然后它请求WRITE_CONTACTS，系统立即授予该权限。




任何权限都可以属于权限组，包括您的应用程序定义的常规权限和权限。 但是，如果许可是危险的，权限组只会影响用户体验。 您可以忽略权限组以获取正常权限。





如果设备正在运行Android 5.1（API级别22）或更低版本，或者该应用的targetSdkVersion为22或更低版本，系统将要求用户在安装时授予权限。 系统再次告诉用户应用程序需要哪些权限组，而不是单独的权限。





危险权限以及权限组：


|Permission Group|Permissions|
|----|----|
|`CALENDAR`|- `READ_CALENDAR`- `WRITE_CALENDAR`|
|`CAMERA`|- `CAMERA`|
|`CONTACTS`|- `READ_CONTACTS`- `WRITE_CONTACTS`- `GET_ACCOUNTS`|
|`LOCATION`|- `ACCESS_FINE_LOCATION`- `ACCESS_COARSE_LOCATION`|
|`MICROPHONE`|- `RECORD_AUDIO`|
|`PHONE`|- `READ_PHONE_STATE`- `CALL_PHONE`- `READ_CALL_LOG`- `WRITE_CALL_LOG`- `ADD_VOICEMAIL`- `USE_SIP`- `PROCESS_OUTGOING_CALLS`|
|`SENSORS`|- `BODY_SENSORS`|
|`SMS`|- `SEND_SMS`- `RECEIVE_SMS`- `READ_SMS`- `RECEIVE_WAP_PUSH`- `RECEIVE_MMS`|
|`STORAGE`|- `READ_EXTERNAL_STORAGE`- `WRITE_EXTERNAL_STORAGE`|





六、定义和执行权限

要强制执行您自己的权限，您必须首先在AndroidManifest.xml中使用一个或多个<permission>元素声明它们。





例如，一个想要控制谁可以开始其中一个活动的应用程序可以声明此操作的权限如下：




```
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.myapp" >
    <permission android:name="com.example.myapp.permission.DEADLY_ACTIVITY"
        android:label="@string/permlab_deadlyActivity"
        android:description="@string/permdesc_deadlyActivity"
        android:permissionGroup="android.permission-group.COST_MONEY"
        android:protectionLevel="dangerous" />
    ...
</manifest>
```


注意：系统不允许多个包声明具有相同名称的权限，除非所有包都使用相同的证书签名。 如果软件包声明权限，则系统不允许用户安装具有相同权限名称的其他软件包，除非这些软件包使用与第一个软件包相同的证书进行签名。 为避免命名冲突，我们建议对自定义权限使用反向域式命名，例如com.example.myapp.ENGAGE_HYPERSPACE。






protectLevel属性是必需的，告诉系统如何向用户通知需要许可的应用程序，或被允许持有该许可的用户，如链接文档中所述。





android：permissionGroup属性是可选的，仅用于帮助系统向用户显示权限。 在大多数情况下，您将要将其设置为标准系统组（在android.Manifest.permission_group中列出），尽管您可以自己定义组。 优选使用现有组，因为这简化了向用户显示的许可UI。





您需要为权限提供标签和描述。 这些是用户在查看权限列表（android：label）或单个权限（android：description）的详细信息时可以看到的字符串资源。使用几句话来描述正在保护的关键功能的许可。 描述应该是一些句子，描述持有人允许的权限。 我们的惯例是一个两句话的描述：第一句描述了权限，第二句话告诉用户如果应用被授予权限，可能会出错的事情类型





以下是CALL_PHONE权限的标签和说明示例：




```
<string name="permlab_callPhone">directly call phone numbers</string>
<string name="permdesc_callPhone">Allows the application to call
    phone numbers without your intervention. Malicious applications may
    cause unexpected calls on your phone bill. Note that this does not
    allow the application to call emergency numbers.</string>
```


您可以使用“设置”应用程序和shell命令adb shell pm列表权限查看当前系统中定义的权限。 要使用“设置”应用程序，请转到“设置”>“应用程序”。 选择一个应用程序并向下滚动以查看应用程序使用的权限。 对于开发人员，adb'-s'选项以与用户将如何看到它们相似的形式显示权限：





```
$ adb shell pm list permissions -s
All Permissions:

Network communication: view Wi-Fi state, create Bluetooth connections, full
Internet access, view network state

Your location: access extra location provider commands, fine (GPS) location,
mock location sources for testing, coarse (network-based) location

Services that cost you money: send SMS messages, directly call phone numbers

...
```





一）、自定义权限建议

应用程序可以通过定义<uses-permission>元素来定义自己的自定义权限并从其他应用程序请求自定义权限。 但是，您应该仔细评估应用程序是否需要这样做。


I、如果您正在设计一套将功能相互揭露的应用程序，请尝试设计应用程序，以便每个权限仅定义一次。 如果应用程序没有使用相同的证书签名，则必须执行此操作。 即使应用程序都使用相同的证书进行签名，所以最佳做法是仅定义一次权限。

II、如果功能仅适用于与提供应用程序签名相同的签名的应用程序，则可能会通过使用签名检查来避免定义自定义权限。 当您的一个应用程序发出其他应用程序的请求时，第二个应用程序可以在遵守该请求之前验证这两个应用程序是否使用相同的证书进行签名。

III、如果您正在开发一套仅在自己的设备上运行的应用程序，那么您应该开发并安装一个管理套件中所有应用程序的权限的软件包。 此软件包不需要提供任何服务本身。 它只是声明所有权限，套件中的其他应用程序使用<uses-permission>元素请求这些权限。







二）、在AndroidManifest.xml中执行权限

您可以通过AndroidManifest.xml应用高级权限，限制对系统或应用程序的整个组件的访问。 为此，请在所需组件上包含一个android：permission属性，命名控制对其的访问权限。





活动权限（适用于<activity>标记）限制谁可以启动关联的活动。 在Context.startActivity（）和Activity.startActivityForResult（）中检查权限。 如果调用者没有所需的权限，则从调用中抛出SecurityException异常。





服务权限（应用于<service>标记）限制谁可以启动或绑定到关联的服务。 在Context.startService（），Context.stopService（）和Context.bindService（）期间检查权限; 如果调用者没有所需的权限，则从调用中抛出SecurityException异常。





BroadcastReceiver权限（应用于<receiver>标签）限制谁可以向相关联的接收方发送广播。 当Context.sendBroadcast（）返回后，系统会尝试将提交的广播传送给给定的接收者，从而检查该权限。 因此，权限失败不会导致将异常抛回给调用者; 它只是没有实现意图。 以相同的方式，可以向Context.registerReceiver（）提供一个权限，以控制谁可以向编程注册的接收方广播。 另一方面，当调用Context.sendBroadcast（）来限制允许BroadcastReceiver对象接收广播时，可以提供一个权限（见下文）。





ContentProvider权限（应用于<provider>标记）限制谁可以访问ContentProvider中的数据。 （内容提供商有一个重要的额外的安全设施，他们称之为URI权限，稍后会介绍）。与其他组件不同，您可以设置两个独立的权限属性：android：readPermission限制谁可以从提供程序读取，而android： writePermission限制谁可以写入它。请注意，如果提供程序受到读取和写入权限的保护，则只保留写入权限并不意味着您可以从提供程序读取。当您首次检索提供程序（如果您没有任何权限，将抛出SecurityException）以及对提供程序执行操作时，将检查权限。使用ContentResolver.query（）需要持有读权限;使用ContentResolver.insert（），ContentResolver.update（），ContentResolver.delete（）需要写权限。在所有这些情况下，不保留所需权限导致从调用中抛出SecurityException。





三）、发送广播时强制执行权限

除了执行可以向注册的BroadcastReceiver发送Intent的权限（如上所述）之外，您还可以在发送广播时指定所需的权限。 通过调用具有权限字符串的Context.sendBroadcast（），您需要接收方的应用程序必须拥有该权限才能接收广播。





请注意，接收方和广播机构都可能需要许可。 当这种情况发生时，两个权限检查都必须传递给要传递到关联目标的意图。





四）、其他许可执行

任何调用服务都可以执行任意细粒度的权限。 这是通过Context.checkCallingPermission（）方法完成的。 使用所需的权限字符串调用，它将返回一个整数，表示该权限是否已被授予当前调用进程。 请注意，这只能在您执行来自另一个进程的呼叫时使用，通常通过从服务发布的IDL接口或以其他方式给予另一个进程。





还有一些其他有用的方法来检查权限。 如果您有另一个进程的pid，可以使用Context方法Context.checkPermission（String，int，int）来检查该pid的权限。 如果您有其他应用程序的包名称，则可以使用PackageManager.checkPermission（String，String）的直接PackageManager方法来确定该特定包是否已被授予特定权限。








七、URI权限

到目前为止描述的标准权限系统与内容提供商一起使用时通常是不够的。 内容提供商可能希望通过读取和写入权限来保护自身，而其直接客户端也需要将特定的URI转交给其他应用程序以进行操作。 一个典型的例子是邮件应用程序中的附件。 访问邮件应受到权限的保护，因为这是敏感的用户数据。 但是，如果向图像查看器提供图像附件的URI，则该图像查看器将无权打开附件，因为它没有理由持有访问所有电子邮件的权限。





该问题的解决方案是per-URI权限：当启动活动或将结果返回到活动时，调用者可以设置Intent.FLAG_GRANT_READ_URI_PERMISSION和/或Intent.FLAG_GRANT_WRITE_URI_PERMISSION。 这允许接收活动权限访问Intent中的特定数据URI，而不管其是否具有访问与Intent相对应的内容提供商中的数据的任何权限。





这种样式允许用户常用功能样式模型，此模型是用户交互驱动了对细粒度权限的特别授予。这可以是将应用程序所需的权限降低到与其行为直接相关的权限的关键工具。




然而，授予细粒度的URI权限需要与持有这些URI的内容提供商进行一些合作。 强烈建议内容提供商实施此设施，并声明它们通过android：grantUriPermissions属性或<grant-uri-permissions>标记来支持它。





更多信息可以在Context.grantUriPermission（），Context.revokeUriPermission（）和Context.checkUriPermission（）方法中找到。




