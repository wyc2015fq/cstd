# 链接：NFC：基于主机的卡模拟 - 一世豁然的专栏 - CSDN博客





2017年05月17日 15:02:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1636








许多提供NFC功能的Android设备已经支持NFC卡仿真。 在大多数情况下，卡被设备中的单独芯片仿真，称为安全元件。 由无线运营商提供的许多SIM卡还包含一个安全元件。




Android 4.4引入了一种额外的卡片仿真方法，它不涉及称为基于主机的卡片仿真的安全元件。 这允许任何Android应用程序模拟卡并直接与NFC阅读器通话。 本文档介绍了基于主机的卡仿真（HCE）在Android上的工作原理，以及如何开发使用此技术来模拟NFC卡的应用程序。








一、拥有安全元素的卡仿真

当使用安全元件提供NFC卡仿真时，要仿真的卡通过Android应用程序提供给设备上的安全元件。 然后，当用户通过NFC终端保持设备时，设备中的NFC控制器将来自读取器的所有数据直接传递到安全元件。 图1说明了这个概念。


![](https://img-blog.csdn.net/20170517141305259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.具有安全元件的NFC卡仿真。





安全元件本身执行与NFC终端的通信，并且完全不涉及Android应用程序。 交易完成后，Android应用程序可以直接查询安全元件的交易状态并通知用户。








二、基于主机的卡模式

当使用基于主机的卡仿真模拟NFC卡时，将数据路由到直接运行Android应用程序的主机CPU，而不是将NFC协议帧路由到安全元件。 图2说明了基于主机的卡仿真的工作原理。


![](https://img-blog.csdn.net/20170517141642008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图2.没有安全元件的NFC卡仿真。








三、支持的NFC卡和协议

NFC标准提供对许多不同协议的支持，并且存在可以被仿真的不同类型的卡。





Android 4.4支持当今市场上常见的几种协议。 许多现有的非接触式卡已经基于这些协议，例如非接触式支付卡。 目前市场上许多NFC读取器也支持这些协议，包括Android NFC设备作为读者本身（参见IsoDep类）。 这允许您仅使用Android驱动的设备构建和部署HCE端到端的NFC解决方案。





具体来说，Android 4.4支持模拟基于NFC-Forum ISO-DEP规范（基于ISO / IEC 14443-4）和ISO / IEC 7816-4规范中定义的过程应用协议数据单元（APDU）的卡。 只有在Nfc-A（ISO / IEC 14443-3 A型）技术的基础上，Android才需要模拟ISO-DEP。 支持Nfc-B（ISO / IEC 14443-4 B型）技术是可选的。 所有这些规格的分层如图3所示。


![](https://img-blog.csdn.net/20170517142009181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








四、HCE服务

Android中的HCE架构基于Android Service组件（称为“HCE服务”）。 服务的主要优点之一是它可以在后台运行，无需任何用户界面。 这适合许多HCE应用程序（如忠诚度或过境卡），用户不需要启动应用程序即可使用它。 相反，轻击NFC读取器上的设备会启动正确的服务（如果尚未运行），并在后台执行该事务。 当然，如果有意义的话，您可以自由地从您的服务中启动其他用户界面（如用户通知）。





一）、服务选择

当用户将设备点击到NFC阅读器时，Android系统需要知道NFC读卡器实际想要与之通信的HCE服务。 这是ISO / IEC 7816-4规范所在的地方：它定义了以应用程序ID（AID）为中心选择应用程序的方式。 AID由最多16个字节组成。 如果您正在为现有的NFC阅读器基础设施模拟卡片，那些读者所寻找的AID通常是众所周知的并公开注册的（例如，支付网络的AID，如Visa和MasterCard）。





如果要为自己的应用程序部署新的读卡器基础架构，则需要注册自己的AID。 AID的注册程序在ISO / IEC 7816-5规范中定义。 如果您正在为Android部署HCE应用程序，Google建议按照7816-5注册AID，因为它将避免与其他应用程序的冲突。





二）、AID组

在某些情况下，HCE服务可能需要注册多个AID来实现某个应用程序，并且需要确保它是所有这些AID的默认处理程序（与组中的某些AID相对于另一个服务）。





AID组是应被视为归属于操作系统的AID的列表。 对于AID组中的所有AID，Android保证以下之一：


1、组中的所有AID都将路由到此HCE服务。

2、组中没有AID被路由到该HCE服务（例如，因为用户优选在组中请求一个或多个AID的另一服务）




换句话说，没有中间状态，组中的一些AID可以路由到一个HCE服务，另一些则被路由到另一个。





三）、AID组和类别

每个AID组可以与一个类别相关联。 这允许Android按类别将HCE服务组合在一起，这反过来又允许用户在类别级别而不是AID级别设置默认值。 一般来说，避免在应用程序的任何面向用户的部分中提及AID：对于普通用户来说，这并不意味着什么。





Android 4.4支持两类：CATEGORY_PAYMENT（涵盖行业标准支付应用）和CATEGORY_OTHER（适用于所有其他HCE应用）。





注意：在任何给定时间，系统中可能只能启用CATEGORY_PAYMENT类别中的一个AID组。 通常，这将是一个了解主要信用卡支付协议并可在任何商家工作的应用程序。


对于仅在一个商家（例如储值卡）工作的闭环付款应用程序，您应该使用CATEGORY_OTHER。 此类别中的AID组可以始终处于活动状态，并且如果需要，可以在AID选择期间由NFC读取器优先考虑。








五、实施HCE服务

要使用基于主机的卡仿真来模拟NFC卡，您需要创建一个处理NFC交易的服务组件。





一）、检查HCE支持

您的应用程序可以通过检查FEATURE_NFC_HOST_CARD_EMULATION功能来检查设备是否支持HCE。 您应该使用应用程序清单中的<uses-feature>标记来声明您的应用程序使用HCE功能，以及该应用程序是否需要运行。





二）、服务实施

Android 4.4提供了一个方便的服务类，可以作为实现HCE服务的基础：HostApduService类。





因此，第一步是扩展HostApduService。




```java
public class MyHostApduService extends HostApduService {
    @Override
    public byte[] processCommandApdu(byte[] apdu, Bundle extras) {
       ...
    }
    @Override
    public void onDeactivated(int reason) {
       ...
    }
}
```


HostApduService声明了需要重写和实现的两个抽象方法。




当NFC读取器向您的服务发送应用协议数据单元（APDU）时，将调用processCommandApdu（）。 APDU也在ISO / IEC 7816-4规范中定义。 APDU是NFC读卡器和HCE服务之间交换的应用级数据包。 该应用级协议是半双工的：NFC读卡器将向您发送一个命令APDU，它将等待您发送响应APDU作为回报。





注意：ISO / IEC 7816-4规范还定义了多个逻辑通道的概念，您可以在单独的逻辑通道上进行多个并行APDU交换。 然而，Android的HCE实现仅支持单个逻辑通道，因此只有一个单线程的APDU交换。





如前所述，Android使用AID来确定读者想要与之通信的HCE服务。 通常，NFC读取器发送到您的设备的第一APDU是“选择AID”APDU; 该APDU包含读者想要与之对话的AID。 Android从APDU中提取AID，将其解析为HCE服务，然后将该APDU转发到已解析的服务。





您可以通过从processCommandApdu（）返回响应APDU的字节来发送响应APDU。 请注意，此方法将在您的应用程序的主线程中调用，不应该被阻止。 因此，如果您无法立即计算并返回响应APDU，则返回null。 然后，您可以在另一个线程上执行必要的工作，并使用HostApduService类中定义的sendResponseApdu（）方法在完成后发送响应。





Android将继续将APDU从阅读器转发到您的服务，直到以下任一方式：


1、NFC读取器发送另一个“SELECT AID”APDU，其中OS解析为不同的服务;

2、NFC阅读器和您的设备之间的NFC链接断开。




在这两种情况下，您的Deactivated（）实现类都被调用，其中一个参数指出了两个发生的哪一个。





如果您正在使用现有的读卡器基础架构，则需要实现读者期望在HCE服务中使用的现有应用程序级协议。





如果您正在部署您所控制的新的读写器基础架构，则可以定义自己的协议和APDU序列。 一般来说，尝试限制APDU的数量和需要交换的数据的大小：这样可以确保您的用户只需要在短时间内将其设备保存在NFC阅读器上。 一个合理的上限是大约1KB的数据，通常可以在300ms内交换。





三）、服务清单申报和AID注册

您的服务必须像往常一样在清单中声明，但是一些额外的部分也必须添加到服务声明中。





首先，要告诉平台它是一个执行HostApduService接口的HCE服务，您的服务声明必须包含SERVICE_INTERFACE操作的意图过滤器。





另外，为了告诉平台哪个AID组被该服务请求，在服务的声明中必须包含一个SERVICE_META_DATA <meta-data>标签，指向一个XML资源，其中包含有关HCE服务的附加信息。





最后，您必须将android：exported属性设置为true，并在服务声明中要求“android.permission.BIND_NFC_SERVICE”权限。 前者确保服务可以通过外部应用程序绑定。 然后，后者强制只有持有“android.permission.BIND_NFC_SERVICE”权限的外部应用程序才能绑定到您的服务。 由于“android.permission.BIND_NFC_SERVICE”是一个系统权限，所以这有效地强制只有Android操作系统可以绑定到您的服务。





以下是HostApduService清单声明的示例：




```
<service android:name=".MyHostApduService" android:exported="true"
         android:permission="android.permission.BIND_NFC_SERVICE">
    <intent-filter>
        <action android:name="android.nfc.cardemulation.action.HOST_APDU_SERVICE"/>
    </intent-filter>
    <meta-data android:name="android.nfc.cardemulation.host_apdu_service"
               android:resource="@xml/apduservice"/>
</service>
```


该元数据标签指向一个apduservice.xml文件。 具有包含两个专有AID的单个AID组声明的文件的示例如下所示：



```
<host-apdu-service xmlns:android="http://schemas.android.com/apk/res/android"
           android:description="@string/servicedesc"
           android:requireDeviceUnlock="false">
    <aid-group android:description="@string/aiddescription"
               android:category="other">
        <aid-filter android:name="F0010203040506"/>
        <aid-filter android:name="F0394148148100"/>
    </aid-group>
</host-apdu-service>
```


需要使用<host-apdu-service>标签来包含一个<android：description>属性，其中包含用户界面中可能显示的服务描述。 requireDeviceUnlock属性可用于指定设备必须解锁，才能调用该服务来处理APDU。




<host-apdu-service>必须包含一个或多个<aid-group>标签。 每个<aid-group>标签都需要：


1、包含一个android：description属性，其中包含适合在UI中显示的AID组的用户友好描述。

2、将其android：category属性设置为指示AID组所属的类别，例如 由CATEGORY_PAYMENT或CATEGORY_OTHER定义的字符串常量。

3、每个<aid-group>必须包含一个或多个<aid-filter>标签，每个标签都包含一个AID。 AID必须以十六进制格式指定，并且包含偶数个字符。




最后，您的应用程序还需要持有NFC许可才能注册为HCE服务。








六、AID冲突解决

多个HostApduService组件可能安装在单个设备上，同一个AID可由多个服务注册。 Android平台根据AID属于哪个类别来解决AID冲突。 每个类别可能有不同的冲突解决政策。





例如，对于某些类别（如付款），用户可能可以在Android设置UI中选择默认服务。 对于其他类别，策略可能总是要求用户在发生冲突时调用哪个服务。 要查询某个类别的冲突解决策略，请参阅getSelectionModeForCategory（）。





一）、检查您的服务是否为默认值

应用程序可以使用isDefaultServiceForCategory（ComponentName，String）API来检查他们的HCE服务是否是特定类别的默认服务。





如果您的服务不是默认值，则可以将其设为默认值。 请参阅ACTION_CHANGE_DEFAULT。








七、付款申请

Android认为已将“付款”类别声明为AID组的HCE服务作为付款应用程序。 Android 4.4发行版包含一个名为“tap＆pay”的顶级设置菜单条目，其中列举了所有此类支付应用程序。 在此设置菜单中，用户可以选择在点击支付终端时将调用的默认付款应用程序。





一）、付款申请所需资产

为了提供更具视觉吸引力的用户体验，HCE支付应用程序需要为其服务提供额外的资产：所谓的服务横幅。





此资产的大小应为260x96 dp，并且可以通过将android：apduServiceBanner属性添加到指向可绘制资源的<host-apdu-service>标记来指定您的元数据XML文件。 一个例子如下所示：




```
<host-apdu-service xmlns:android="http://schemas.android.com/apk/res/android"
        android:description="@string/servicedesc"
        android:requireDeviceUnlock="false"
        android:apduServiceBanner="@drawable/my_banner">
    <aid-group android:description="@string/aiddescription"
               android:category="payment">
        <aid-filter android:name="F0010203040506"/>
        <aid-filter android:name="F0394148148100"/>
    </aid-group>
</host-apdu-service>
```



八、屏幕关闭和锁屏行为

当设备的屏幕关闭时，当前的Android实现会使NFC控制器和应用程序处理器完全关闭。 因此，当屏幕关闭时，HCE服务将无法正常工作。





HCE服务可以从锁定屏幕起作用：这由HCE服务的<host-apdu-service>标签中的android：requireDeviceUnlock属性控制。 默认情况下，不需要设备解锁，即使设备被锁定，您的服务也将被调用。





如果您为您的HCE服务将android：requireDeviceUnlock属性设置为“true”，则当您点击选择要解析为您的服务的AID的NFC阅读器时，Android将提示用户解锁设备。 解锁后，Android会显示一个对话框，提示用户再次点击完成交易。 这是必要的，因为用户可能已经将设备移动离开NFC读取器以将其解锁。








九、与安全元素卡共存

部分开发人员感兴趣的部分是依赖安全元素进行卡仿真的应用程序。 Android的HCE实现旨在与其他实现卡仿真的方法并行工作，包括使用安全元素。





注意：Android不提供用于直接与安全元素通信的API。





这种共存基于称为“AID路由”的原理：NFC控制器保留由（有限）路由规则列表组成的路由表。 每个路由规则包含AID和目的地。 目的地可以是主机CPU（Android应用程序正在运行）或连接的安全元素。





当NFC读取器发送具有“选择AID”的APDU时，NFC控制器解析它并检查AID是否与其路由表中的任何AID匹配。 如果匹配，该APDU及其后面的所有APDU将被发送到与AID相关联的目的地，直到接收到另一个“选择AID”APDU或NFC链路断开。





注意：虽然ISO / IEC 7816-4也定义了“部分匹配”的概念，但目前Android HCE设备不支持此功能。





这种架构如图4所示。


![](https://img-blog.csdn.net/20170517144845059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图4. Android操作与安全元素和主机卡仿真。





NFC控制器通常还包含用于APDU的默认路由。 当路由表中找不到AID时，使用默认路由。 虽然此设置可能因设备而异，但Android设备需要确保您的应用注册的AID正确路由到主机。





实现HCE服务的Android应用程序或使用安全元素的Android应用程序不必担心配置路由表 - 即由Android自动处理。 Android只需要知道HCE服务可以处理哪些AID，哪些安全元素可以处理哪些AID。 根据安装的服务和用户配置为首选，路由表被自动配置。





我们已经描述了如何为HCE服务声明AID。 以下部分说明如何为使用安全元素进行卡仿真的应用程序声明AID。





一）、安全元素AID注册

使用安全元素进行卡仿真的应用程序可以在其清单中声明所谓的“关闭主机服务”。 这种服务的声明与宣布HCE服务几乎相同。 例外情况是：


1、意图过滤器中使用的操作必须设置为SERVICE_INTERFACE。

2、元数据名称属性必须设置为SERVICE_META_DATA。

3、元数据XML文件必须使用<offhost-apdu-service>根标记。



```
<service android:name=".MyOffHostApduService" android:exported="true"
         android:permission="android.permission.BIND_NFC_SERVICE">
    <intent-filter>
        <action android:name="android.nfc.cardemulation.action.OFF_HOST_APDU_SERVICE"/>
    </intent-filter>
    <meta-data android:name="android.nfc.cardemulation.off_host_apdu_service"
               android:resource="@xml/apduservice"/>
</service>
```


注册两个AID的相应apduservice.xml文件的示例：



```
<offhost-apdu-service xmlns:android="http://schemas.android.com/apk/res/android"
           android:description="@string/servicedesc">
    <aid-group android:description="@string/subscription" android:category="other">
        <aid-filter android:name="F0010203040506"/>
        <aid-filter android:name="F0394148148100"/>
    </aid-group>
</offhost-apdu-service>
```


android：requireDeviceUnlock属性不适用于主机服务，因为主机CPU不涉及事务，因此在设备被锁定时不能阻止安全元素执行事务。




必须使用android：apduServiceBanner属性作为支付应用程序的主机服务，以便作为默认付款应用程序可选。





二）、关闭主机服务调用

Android本身永远不会启动或绑定到声明为“脱离主机”的服务。 这是因为实际交易由安全元素执行，而不是由Android服务本身执行。 服务声明仅允许应用程序注册安全元件上存在的AID。








十、HCE和安全

HCE架构本身提供了一个核心安全性：因为您的服务受到BIND_NFC_SERVICE系统权限的保护，因此只有操作系统可以绑定到您的服务并与之通信。 这确保您接收的任何APDU实际上是由NFC控制器由操作系统接收到的APDU，并且您发回的任何APDU只会进入操作系统，而后者又将APDU直接转发到NFC控制器。





核心的剩余部分是您的应用程序发送到NFC读取器的数据。 这在HCE设计中有意解耦：它不在乎数据来自哪里，它只是确保它被安全地传输到NFC控制器并输出到NFC读卡器。





为了安全地存储和检索您希望从HCE服务发送的数据，您可以例如依靠Android应用程序沙箱，将其应用程序的数据与其他应用程序隔离开来。 有关Android安全性的更多详细信息，请参阅安全提示。








十一、协议参数和详细信息

开发人员感兴趣的是想要了解什么协议参数HCE设备在NFC协议的防冲突和激活阶段使用。 这样可以构建与Android HCE设备兼容的读卡器基础架构。





一）、Nfc-A（ISO / IEC 14443 A型）协议防冲突和激活

作为Nfc-A协议激活的一部分，交换多个帧。





在交换的第一部分，HCE设备将呈现其UID; 应该假设HCE设备有一个随机的UID。 这意味着在每一个水龙头上，呈现给读者的UID将是一个随机生成的UID。 因此，NFC读取器不应该依赖于HCE设备的UID作为身份验证或身份验证的一种形式。





NFC读取器随后可以通过发送SEL_REQ命令来选择HCE设备。 HCE设备的SEL_RES响应将至少具有第6位（0x20），表示设备支持ISO-DEP。 请注意，也可以设置SEL_RES中的其他位，例如支持NFC-DEP（p2p）协议。 由于可以设置其他位，因此希望与HCE设备交互的读者应该仅显式地检查第6位，而不是将完整的SEL_RES与值0x20进行比较。





二）、ISO-DEP激活

在Nfc-A协议被激活之后，由NFC读取器启动ISO-DEP协议激活。 它发送“RATS”（请求回答选择）命令。 RATS响应（ATS）由NFC控制器完全生成，并且不能由HCE服务配置。 然而，HCE实现需要满足NFC论坛对ATS响应的要求，因此NFC读取器可以依赖于根据任何HCE设备的NFC论坛要求设置的这些参数。





以下部分详细介绍了NFC控制器在HCE设备上提供的ATS响应的各个字节：


1、TL：ATS响应的长度。 不得超过20个字节。




2、T0：在所有HCE设备上必须设置第5,6和7位，指示TA（1），TB（1）和TC（1）都包含在ATS响应中。 位1至4表示FSCI，编码最大帧大小。 在HCE设备上，FSCI的值必须在0h和8h之间。




3、T（A）1：定义读写器与仿真器之间的比特率，以及它们是否可以是不对称的。 HCE设备没有比特率要求或保证。




4、T（B）1：位1到4表示启动帧保护时间整数（SFGI）。 在HCE设备上，SFGI必须<= 8h。 位5到8表示帧等待时间整数（FWI），并对帧等待时间（FWT）进行编码。 在HCE设备上，FWI必须<= 8h。




5、T（C）1：位5表示支持“高级协议功能”。 HCE设备可能支持或可能不支持“高级协议功能”。 位2表示支持DID。 HCE设备可能支持或不支持DID。 位1表示支持NAD。 HCE设备不能支持NAD并将位1设置为零。




6、历史字节：HCE设备最多可以返回15个历史字节。 愿意与HCE服务互动的NFC读者不应该对历史字节或其存在的内容进行假设。




请注意，许多HCE设备可能符合EMVCo中联合支付网络在其“非接触式通信协议”规范中规定的协议要求。 尤其是：


1、T0中的FSCI必须在2h和8h之间。




2、T（A）1必须设置为0x80，表示仅支持106 kbit / s的比特率，不支持读卡器和仿真器之间的不对称比特率。




3、T（B）1中的FWI必须<= 7h。




三）、APDU数据交换

如前所述，HCE实现只支持单个逻辑通道。 尝试在不同逻辑通道上选择应用程序将无法在HCE设备上运行。




