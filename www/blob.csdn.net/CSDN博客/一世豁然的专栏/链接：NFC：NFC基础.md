# 链接：NFC：NFC基础 - 一世豁然的专栏 - CSDN博客





2017年05月17日 11:30:15[一世豁然](https://me.csdn.net/Explorer_day)阅读数：510








本文档介绍了在Android中执行的基本NFC任务。 它解释了如何以NDEF消息的形式发送和接收NFC数据，并描述了支持这些功能的Android框架API。 有关更高级的主题，包括讨论使用非NDEF数据的讨论，请参阅高级NFC。




使用NDEF数据和Android时，有两个主要的用例：


1、从NFC标签读取NDEF数据。




2、使用Android Beam™将NDEF消息从一个设备发送到另一个设备




从NFC标签读取NDEF数据由标签调度系统处理，标签调度系统分析发现的NFC标签，对数据进行适当分类，并启动对分类数据感兴趣的应用程序。 想要处理扫描的NFC标签的应用程序可以声明一个意图过滤器并请求处理数据。





Android Beam™功能允许设备将NDEF消息推送到另一个设备上，同时通过物理轻敲设备。 这种交互提供了一种比其他无线技术（如蓝牙）更容易发送数据的方式，因为使用NFC，无需手动设备发现或配对。 当两个设备进入范围时，连接自动启动。 Android Beam可通过一组NFC API获得，因此任何应用程序都可以在设备之间传输信息。 例如，联系人，浏览器和YouTube应用程序使用Android Beam与其他设备共享联系人，网页和视频。








一、标签调度系统

除非在设备的“设置”菜单中禁用NFC，否则Android设备通常会在屏幕解锁时查找NFC标签。 当Android设备发现NFC标签时，所需的行为是让最合适的活动处理意图，而不要求用户使用什么应用程序。 由于设备会在非常短的范围内扫描NFC标签，所以很可能使用户手动选择一个活动会强制他们将设备从标签移开，并断开连接。 您应该开发您的活动，只处理您的活动关心的NFC标签，以防止活动选择器出现。





为了帮助您实现这一目标，Android提供了一个特殊的标签调度系统，可分析扫描的NFC标签，解析它们，并尝试查找对扫描数据感兴趣的应用程序。 它做到这一点：


1、解析NFC标签，找出MIME类型或标识标签中数据有效内容的URI。




2、将MIME类型或URI和有效内容封装成意图。 NFC标签如何映射到MIME类型和URI中描述了前两个步骤。




3、根据意图启动活动。 这在NFC标签如何分派到应用程序中有所描述。




一）、NFC标签如何映射到MIME类型和URI

在开始编写NFC应用程序之前，了解NFC标签的不同类型，标签分发系统如何解析NFC标签以及标签分发系统在检测到NDEF消息时所做的特殊工作非常重要。 NFC标签具有广泛的技术，并且还可以以许多不同的方式将数据写入它们。 Android最受支持的是NDEF标准，由NFC论坛定义。





NDEF数据封装在包含一个或多个记录（NdefRecord）的消息（NdefMessage）内。 每个NDEF记录必须根据您要创建的记录类型的规范形成。 Android还支持不包含NDEF数据的其他类型的标签，您可以使用android.nfc.tech包中的类来处理。 要了解有关这些技术的更多信息，请参阅Advanced NFC主题。 使用这些其他类型的标签涉及编写自己的协议栈以与标签进行通信，因此我们建议您尽可能使用NDEF进行开发，并最大限度地支持Android设备。





注意：要下载完整的NDEF规范，请访问NFC论坛规范下载站点，并参阅创建NDEF记录的常见类型，以获取如何构建NDEF记录的示例。





现在您在NFC标签中有一些背景，以下部分将更详细地介绍Android如何处理NDEF格式的标签。 当Android驱动的设备扫描包含NDEF格式化数据的NFC标签时，它将解析该消息，并尝试找出数据的MIME类型或标识URI。 为此，系统读取NdefMessage内的第一个NdefRecord，以确定如何解释整个NDEF消息（NDEF消息可以有多个NDEF记录）。 在一个格式良好的NDEF消息中，第一个NdefRecord包含以下字段：


1、3位TNF（类型名称格式）

指示如何解释可变长度类型字段。 有效值如表1所述。





2、可变长度类型

描述记录的类型。 如果使用TNF_WELL_KNOWN，请使用此字段指定记录类型定义（RTD）。 有效的RTD值在表2中描述。





3、可变长度ID

记录的唯一标识符。 此字段不经常使用，但如果您需要唯一标识标签，您可以为其创建一个ID。





4、可变长度有效载荷

您要读取或写入的实际数据有效负载。 NDEF消息可以包含多个NDEF记录，因此不要将全部有效载荷置于NDEF消息的第一个NDEF记录中。





标签分发系统使用TNF和类型字段来尝试将MIME类型或URI映射到NDEF消息。 如果成功，它会将该信息与实际有效载荷一起封装在ACTION_NDEF_DISCOVERED意图内。 但是，有时标签分派系统无法根据第一个NDEF记录确定数据的类型。 当NDEF数据无法映射到MIME类型或URI时，或者当NFC标签不包含NDEF数据时，会发生这种情况。 在这种情况下，具有关于标签技术和有效载荷的信息的Tag对象被封装在ACTION_TECH_DISCOVERED意图内。





表1描述了标签分发系统如何将TNF和类型字段映射到MIME类型或URI。 它还描述了哪些TNF不能映射到MIME类型或URI。 在这种情况下，标签调度系统将恢复为ACTION_TECH_DISCOVERED。





例如，如果标签分发系统遇到类型为TNF_ABSOLUTE_URI的记录，则会将该记录的可变长度类型字段映射为URI。 标签调度系统将该URI封装在ACTION_NDEF_DISCOVERED意图的数据字段中以及关于该标签的其他信息，例如有效载荷。 另一方面，如果遇到类型为TNF_UNKNOWN的记录，则会创建一种封装标签技术的意图。





表1.支持的TNF及其映射



|Type Name Format (TNF)|Mapping|
|----|----|
|`[TNF_ABSOLUTE_URI](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#TNF_ABSOLUTE_URI)`|基于类型字段的URI。|
|`[TNF_EMPTY](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#TNF_EMPTY)`|Falls back to `[ACTION_TECH_DISCOVERED](https://developer.android.google.cn/reference/android/nfc/NfcAdapter.html#ACTION_TECH_DISCOVERED)`.|
|`[TNF_EXTERNAL_TYPE](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#TNF_EXTERNAL_TYPE)`|URI based on the URN in the type field. The URN is encoded into the NDEF type field in a shortened form: `<domain_name>:<service_name>`. Android maps this to a URI in the form: `vnd.android.nfc://ext/<domain_name>:<service_name>`.|
|`[TNF_MIME_MEDIA](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#TNF_MIME_MEDIA)`|MIME type based on the type field.|
|`[TNF_UNCHANGED](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#TNF_UNCHANGED)`|Invalid in the first record, so falls back to `[ACTION_TECH_DISCOVERED](https://developer.android.google.cn/reference/android/nfc/NfcAdapter.html#ACTION_TECH_DISCOVERED)`.|
|`[TNF_UNKNOWN](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#TNF_UNKNOWN)`|Falls back to `[ACTION_TECH_DISCOVERED](https://developer.android.google.cn/reference/android/nfc/NfcAdapter.html#ACTION_TECH_DISCOVERED)`.|
|`[TNF_WELL_KNOWN](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#TNF_WELL_KNOWN)`|MIME type or URI depending on the Record Type Definition (RTD), which you set in the type field. See [Table 2](https://developer.android.google.cn/guide/topics/connectivity/nfc/nfc.html#well_known) for more information on available RTDs and their mappings.|


表2.支持的TNF_WELL_KNOWN的RTD及其映射



|Record Type Definition (RTD)|Mapping|
|----|----|
|`[RTD_ALTERNATIVE_CARRIER](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#RTD_ALTERNATIVE_CARRIER)`|Falls back to `[ACTION_TECH_DISCOVERED](https://developer.android.google.cn/reference/android/nfc/NfcAdapter.html#ACTION_TECH_DISCOVERED)`.|
|`[RTD_HANDOVER_CARRIER](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#RTD_HANDOVER_CARRIER)`|Falls back to `[ACTION_TECH_DISCOVERED](https://developer.android.google.cn/reference/android/nfc/NfcAdapter.html#ACTION_TECH_DISCOVERED)`.|
|`[RTD_HANDOVER_REQUEST](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#RTD_HANDOVER_REQUEST)`|Falls back to `[ACTION_TECH_DISCOVERED](https://developer.android.google.cn/reference/android/nfc/NfcAdapter.html#ACTION_TECH_DISCOVERED)`.|
|`[RTD_HANDOVER_SELECT](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#RTD_HANDOVER_SELECT)`|Falls back to `[ACTION_TECH_DISCOVERED](https://developer.android.google.cn/reference/android/nfc/NfcAdapter.html#ACTION_TECH_DISCOVERED)`.|
|`[RTD_SMART_POSTER](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#RTD_SMART_POSTER)`|URI based on parsing the payload.|
|`[RTD_TEXT](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#RTD_TEXT)`|MIME type of `text/plain`.|
|`[RTD_URI](https://developer.android.google.cn/reference/android/nfc/NdefRecord.html#RTD_URI)`|URI based on payload.|


二）、NFC标签如何分派到应用程序

当标签调度系统完成创建封装NFC标签及其识别信息的意图时，它将意图发送给过滤意图的感兴趣的应用程序。 如果多个应用程序可以处理意图，则会显示活动选择器，以便用户可以选择活动。 标签调度系统定义了三个意图，按照从最高到最低优先级的顺序列出：


1、ACTION_NDEF_DISCOVERED：当扫描包含NDEF有效负载的标签并且是公认类型时，此意图用于启动“活动”。 这是最高优先级的意图，标签发送系统尽可能尝试在任何其他意图之前以此意图启动活动。




2、ACTION_TECH_DISCOVERED：如果没有活动注册以处理ACTION_NDEF_DISCOVERED意图，则标记分发系统尝试以此意图启动应用程序。 如果扫描的标签包含无法映射到MIME类型或URI的NDEF数据，或者标签不包含NDEF数据但是已知标签技术，则此意图也将直接启动（首先不启动ACTION_NDEF_DISCOVERED）。




3、ACTION_TAG_DISCOVERED：如果没有活动处理ACTION_NDEF_DISCOVERED或ACTION_TECH_DISCOVERED意图，则此意图启动。




标签发送系统的基本工作方式如下：


1、尝试在解析NFC标签（ACTION_NDEF_DISCOVERED或ACTION_TECH_DISCOVERED）时由标记分派系统创建的意图启动活动。




2、如果没有为此意图过滤活动，请尝试启动具有下一个最低优先级意图（ACTION_TECH_DISCOVERED或ACTION_TAG_DISCOVERED）的活动，直到应用程序过滤意图或直到标签分派系统尝试所有可能的意图。




3、如果没有应用程序过滤任何意图，不做任何事情。

![](https://img-blog.csdn.net/20170517095835178?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.标签调度系统





只要有可能，使用NDEF消息和ACTION_NDEF_DISCOVERED意图，因为它是三分之一中最具体的。 这个意图允许您在比其他两个意图更合适的时间启动应用程序，为用户提供更好的体验。








二、在Android清单中请求NFC访问

在您可以访问设备的NFC硬件并正确处理NFC意图之前，请在AndroidManifest.xml文件中声明这些项目：


1、NFC <uses-permission>元素访问NFC硬件：


`<uses-permission android:name="android.permission.NFC" />`

2、应用程序可以支持的最低SDK版本。 API级别9仅通过ACTION_TAG_DISCOVERED支持有限的标签分发，并且只能通过EXTRA_NDEF_MESSAGES额外的方式访问NDEF消息。 没有其他标签属性或I / O操作可访问。 API 10级包括全面的读写器支持以及前景NDEF推送，API 14级提供了一种更轻松的方法，可以将NDEF消息推送到其他具有Android Beam的设备，并提供了额外的便利方法来创建NDEF记录。


`<uses-sdk android:minSdkVersion="10"/>`

3、使用要素元素，使您的应用程序仅在具有NFC硬件的设备上显示在Google Play中：


`<uses-feature android:name="android.hardware.nfc" android:required="true" />`

如果您的应用程序使用NFC功能，但该功能对您的应用程序并不重要，则可以省略use-feature元素，并在运行时检查NFC avalailbility，以查看getDefaultAdapter（）是否为空。







三、过滤NFC意图

要扫描您要处理的NFC标签时启动应用程序，您的应用程序可以过滤Android清单中的一个，两个或所有三个NFC意图。 但是，您通常希望过滤ACTION_NDEF_DISCOVERED意图，以便您的应用程序启动时的最多控制。 ACTION_TECH_DISCOVERED意图是针对ACTION_NDEF_DISCOVERED的ACTION_NDEF_DISCOVERED的回退，当ACTION_NDEF_DISCOVERED没有应用程序过滤时，或者当有效负载不是NDEF时。 ACTION_TAG_DISCOVERED的过滤通常是过滤的类别。
 许多应用程序将在ACTION_TAG_DISCOVERED之前过滤ACTION_NDEF_DISCOVERED或ACTION_TECH_DISCOVERED，因此您的应用程序的启动概率很低。 ACTION_TAG_DISCOVERED仅适用于在没有其他应用程序安装以处理ACTION_NDEF_DISCOVERED或ACTION_TECH_DISCOVERED意图的情况下过滤的应用程序。





由于NFC标签的部署不同，因此许多时间不在您的控制之下，这并不总是可能的，这就是为什么在必要时可以追溯到其他两个意图。 当您控制所写入的标签和数据类型时，建议您使用NDEF格式化标签。 以下部分介绍如何对每种类型的意图进行过滤。





一）、ACTION_NDEF_DISCOVERED

要过滤ACTION_NDEF_DISCOVERED意图，请声明intent过滤器以及要过滤的数据类型。 以下示例使用MIME类型为text / plain过滤ACTION_NDEF_DISCOVERED意图：




```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED"/>
    <category android:name="android.intent.category.DEFAULT"/>
    <data android:mimeType="text/plain" />
</intent-filter>
```


以下示例以http://developer.android.com/index.html的形式过滤URI。



```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED"/>
    <category android:name="android.intent.category.DEFAULT"/>
   <data android:scheme="http"
              android:host="developer.android.com"
              android:pathPrefix="/index.html" />
</intent-filter>
```



二）、ACTION_TECH_DISCOVERED

如果您的活动过滤ACTION_TECH_DISCOVERED意图，则必须创建一个XML资源文件，该文件指定您的活动在技术列表集中支持的技术。 如果技术列表集是标记支持的技术的一部分，则您的活动被认为是匹配的，您可以通过调用getTechList（）获取。





例如，如果扫描的标签支持MifareClassic，NdefFormatable和NfcA，则您的技术列表集必须指定所有三种，两种或一种技术（而不是其他技术），以使您的活动得以匹配。





以下示例定义了所有技术。 您可以删除不需要的那些。 在<project-root> / res / xml文件夹中保存此文件（您可以将其命名为任何内容）。




```
<resources xmlns:xliff="urn:oasis:names:tc:xliff:document:1.2">
    <tech-list>
        <tech>android.nfc.tech.IsoDep</tech>
        <tech>android.nfc.tech.NfcA</tech>
        <tech>android.nfc.tech.NfcB</tech>
        <tech>android.nfc.tech.NfcF</tech>
        <tech>android.nfc.tech.NfcV</tech>
        <tech>android.nfc.tech.Ndef</tech>
        <tech>android.nfc.tech.NdefFormatable</tech>
        <tech>android.nfc.tech.MifareClassic</tech>
        <tech>android.nfc.tech.MifareUltralight</tech>
    </tech-list>
</resources>
```


您还可以指定多个技术列表集。 每个技术列表集都被视为独立的，如果任何单个技术列表集合是getTechList（）返回的技术的一部分，您的活动将被视为匹配项。 这提供了用于匹配技术的AND和OR语义。 以下示例匹配可支持NfcA和Ndef技术的标签，或者可以支持NfcB和Ndef技术：



```
<resources xmlns:xliff="urn:oasis:names:tc:xliff:document:1.2">
    <tech-list>
        <tech>android.nfc.tech.NfcA</tech>
        <tech>android.nfc.tech.Ndef</tech>
    </tech-list>
</resources>

<resources xmlns:xliff="urn:oasis:names:tc:xliff:document:1.2">
    <tech-list>
        <tech>android.nfc.tech.NfcB</tech>
        <tech>android.nfc.tech.Ndef</tech>
    </tech-list>
</resources>
```


在您的AndroidManifest.xml文件中，指定刚刚在<activity>元素<element>元素中创建的资源文件，如下例所示：



```
<activity>
...
<intent-filter>
    <action android:name="android.nfc.action.TECH_DISCOVERED"/>
</intent-filter>

<meta-data android:name="android.nfc.action.TECH_DISCOVERED"
    android:resource="@xml/nfc_tech_filter" />
...
</activity>
```


有关使用标签技术和ACTION_TECH_DISCOVERED意图的更多信息，请参阅高级NFC文档中的使用支持的标签技术。




三）、ACTION_TAG_DISCOVERED

要过滤ACTION_TAG_DISCOVERED，请使用以下意图过滤器：




```
<intent-filter>
    <action android:name="android.nfc.action.TAG_DISCOVERED"/>
</intent-filter>
```


1、从意图获取信息

如果由于NFC意图开始活动，则可以从意图获取有关扫描的NFC标签的信息。 意图可以包含以下附件，具体取决于扫描的标签：


 1）、EXTRA_TAG（必需）：表示扫描标签的标签对象。

 2）、EXTRA_NDEF_MESSAGES（可选）：从标签解析的NDEF消息数组。 ACTION_NDEF_DISCOVERED意图是强制性的。

 3）、EXTRA_ID（可选）：标签的低级ID。




要获得这些附加功能，请检查您的活动是否与其中一个NFC意图一起启动，以确保标记已被扫描，然后从意图中获取附加组件。 以下示例检查ACTION_NDEF_DISCOVERED意图，并从意图额外获取NDEF消息。




```java
@Override
protected void onNewIntent(Intent intent) {
    super.onNewIntent(intent);
    ...
    if (intent != null && NfcAdapter.ACTION_NDEF_DISCOVERED.equals(intent.getAction())) {
        Parcelable[] rawMessages =
            intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
        if (rawMessages != null) {
            NdefMessage[] messages = new NdefMessage[rawMessages.length];
            for (int i = 0; i < rawMessages.length; i++) {
                messages[i] = (NdefMessage) rawMessages[i];
            }
            // Process the messages array.
            ...
        }
    }
}
```


或者，您可以从意图获取Tag对象，该对象将包含有效负载，并允许您枚举标记的技术：



```java
Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
```






四、创建NDEF记录的常见类型

本节介绍如何创建常见类型的NDEF记录，以帮助您在写入NFC标签或使用Android Beam发送数据时。 从Android 4.0（API级别14）开始，createUri（）方法可用于帮助您自动创建URI记录。 从Android 4.1（API级别16）开始，createExternal（）和createMime（）可用于帮助您创建MIME和外部类型的NDEF记录。 尽可能使用这些帮助方法，以便在手动创建NDEF记录时避免错误。





本节还介绍如何为记录创建相应的意图过滤器。 所有这些NDEF记录示例都应该在您写入标签或发送的NDEF消息的第一个NDEF记录中。





一）、TNF_ABSOLUTE_URI

注意：我们建议您使用RTD_URI类型而不是TNF_ABSOLUTE_URI，因为它更有效率。





您可以通过以下方式创建一个TNF_ABSOLUTE_URI NDEF记录：




```java
NdefRecord uriRecord = new NdefRecord(
    NdefRecord.TNF_ABSOLUTE_URI ,
    "http://developer.android.com/index.html".getBytes(Charset.forName("US-ASCII")),
    new byte[0], new byte[0]);
```


以前的NDEF记录的意图过滤器将如下所示：



```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED" />
    <category android:name="android.intent.category.DEFAULT" />
    <data android:scheme="http"
        android:host="developer.android.com"
        android:pathPrefix="/index.html" />
</intent-filter>
```



二）、TNF_MIME_MEDIA

您可以通过以下方式创建一个TNF_MIME_MEDIA NDEF记录：





使用createMime（）方法：




```java
NdefRecord mimeRecord = NdefRecord.createMime("application/vnd.com.example.android.beam",
    "Beam me up, Android".getBytes(Charset.forName("US-ASCII")));
```


手动创建NdefRecord：



```java
NdefRecord mimeRecord = new NdefRecord(
    NdefRecord.TNF_MIME_MEDIA ,
    "application/vnd.com.example.android.beam".getBytes(Charset.forName("US-ASCII")),
    new byte[0], "Beam me up, Android!".getBytes(Charset.forName("US-ASCII")));
```


以前的NDEF记录的意图过滤器将如下所示：



```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED" />
    <category android:name="android.intent.category.DEFAULT" />
    <data android:mimeType="application/vnd.com.example.android.beam" />
</intent-filter>
```



三）、使用RTD_TEXT的TNF_WELL_KNOWN

您可以通过以下方式创建一个TNF_WELL_KNOWN NDEF记录：




```java
public NdefRecord createTextRecord(String payload, Locale locale, boolean encodeInUtf8) {
    byte[] langBytes = locale.getLanguage().getBytes(Charset.forName("US-ASCII"));
    Charset utfEncoding = encodeInUtf8 ? Charset.forName("UTF-8") : Charset.forName("UTF-16");
    byte[] textBytes = payload.getBytes(utfEncoding);
    int utfBit = encodeInUtf8 ? 0 : (1 << 7);
    char status = (char) (utfBit + langBytes.length);
    byte[] data = new byte[1 + langBytes.length + textBytes.length];
    data[0] = (byte) status;
    System.arraycopy(langBytes, 0, data, 1, langBytes.length);
    System.arraycopy(textBytes, 0, data, 1 + langBytes.length, textBytes.length);
    NdefRecord record = new NdefRecord(NdefRecord.TNF_WELL_KNOWN,
    NdefRecord.RTD_TEXT, new byte[0], data);
    return record;
}
```


以前的NDEF记录的意图过滤器将如下所示：



```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED" />
    <category android:name="android.intent.category.DEFAULT" />
    <data android:mimeType="text/plain" />
</intent-filter>
```



四）、TNF_WELL_KNOWN与RTD_URI

您可以通过以下方式创建一个TNF_WELL_KNOWN NDEF记录：





使用createUri（String）方法：




```java
NdefRecord rtdUriRecord1 = NdefRecord.createUri("http://example.com");
```


使用createUri（Uri）方法：



```java
Uri uri = new Uri("http://example.com");
NdefRecord rtdUriRecord2 = NdefRecord.createUri(uri);
```


手动创建NdefRecord：



```java
byte[] uriField = "example.com".getBytes(Charset.forName("US-ASCII"));
byte[] payload = new byte[uriField.length + 1];              //add 1 for the URI Prefix
byte payload[0] = 0x01;                                      //prefixes http://www. to the URI
System.arraycopy(uriField, 0, payload, 1, uriField.length);  //appends URI to payload
NdefRecord rtdUriRecord = new NdefRecord(
    NdefRecord.TNF_WELL_KNOWN, NdefRecord.RTD_URI, new byte[0], payload);
```


以前的NDEF记录的意图过滤器将如下所示：



```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED" />
    <category android:name="android.intent.category.DEFAULT" />
    <data android:scheme="http"
        android:host="example.com"
```

```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED" />
    <category android:name="android.intent.category.DEFAULT" />
    <data android:scheme="vnd.android.nfc"
        android:host="ext"
        android:pathPrefix="/com.example:externalType"/>
</intent-filter>
```


android:pathPrefix="" /></intent-filter>





五）、TNF_EXTERNAL_TYPE

您可以通过以下方式创建一个TNF_EXTERNAL_TYPE NDEF记录：





使用createExternal（）方法：



```java
byte[] payload; //assign to your data
String domain = "com.example"; //usually your app's package name
String type = "externalType";
NdefRecord extRecord = NdefRecord.createExternal(domain, type, payload);
```


手动创建NdefRecord：



```java
byte[] payload;
...
NdefRecord extRecord = new NdefRecord(
    NdefRecord.TNF_EXTERNAL_TYPE, "com.example:externalType", new byte[0], payload);
```


以前的NDEF记录的意图过滤器将如下所示：



```
<intent-filter>
    <action android:name="android.nfc.action.NDEF_DISCOVERED" />
    <category android:name="android.intent.category.DEFAULT" />
    <data android:scheme="vnd.android.nfc"
        android:host="ext"
        android:pathPrefix="/com.example:externalType"/>
</intent-filter>
```


使用TNF_EXTERNAL_TYPE进行更通用的NFC标签部署，以更好地支持Android驱动和非Android设备。





注意：TNF_EXTERNAL_TYPE的URN具有规范格式：urn：nfc：ext：example.com：externalType，但是NFC论坛RTD规范声明必须从NDEF记录中忽略URN的urn：nfc：ext：部分。 因此，您需要提供的是域（example.com中的示例），并以冒号分隔并输入（示例中的externalType）。 当调度TNF_EXTERNAL_TYPE时，Android将urn：nfc：ext：example.com：externalType URN转换为vnd.android.nfc：//ext/example.com：externalType
 URI，这是示例中的intent过滤器所声明的。








六）、Android应用程序记录

Android应用程序记录（AAR）在Android 4.0（API级别14）中引入，可以更加确定您的应用程序在扫描NFC标签时的启动。 AAR具有嵌入NDEF记录内的应用程序的包名称。 您可以向NDEF消息的任何NDEF记录添加AAR，因为Android会搜索整个NDEF消息中的AAR消息。 如果找到AAR，它会根据AAR中的包名启动应用程序。 如果设备上没有应用程序，则会启动Google Play下载应用程序。





如果您希望防止其他应用程序对同一意图进行过滤，并且可能会处理已部署的特定标签，则AAR非常有用。 仅在应用程序级别支持AAR，因为包名称约束，而不是在意向过滤中的活动级别。 如果要在“活动”级别处理意图，请使用意图过滤器。





如果标签包含AAR，则标签调度系统将按以下方式进行分派：


1、正常情况下尝试使用意图过滤器启动活动。 如果与意图匹配的活动也符合AAR，请启动活动。




2、如果意图过滤的活动与AAR不匹配，如果多个活动可以处理意图，或者如果没有Activity处理意图，则启动由AAR指定的应用程序。




3、如果没有应用程序可以从AAR开始，请转到Google Play，以基于AAR下载应用程序。




注意：您可以使用前台调度系统覆盖AAR和意图调度系统，这样可以在发现NFC标签时使前台活动具有优先权。 使用这种方法，活动必须在前台来覆盖AAR和意图调度系统。





如果您仍然想过滤不包含AAR的扫描标签，则可以正常声明意图过滤器。 如果您的应用程序对不包含AAR的其他标签感兴趣，这将非常有用。 例如，也许您想要保证您的应用程序处理部署的专有标签以及第三方部署的常规标签。 请记住，AAR特定于Android 4.0设备或更高版本，因此在部署标签时，最有可能要使用AAR和MIME类型/ URI的组合来支持最广泛的设备。 此外，当您部署NFC标签时，请考虑如何编写NFC标签，以支持大多数设备（Android设备和其他设备）。 您可以通过定义相对唯一的MIME类型或URI来使应用程序更容易区分。





Android提供了一个简单的API来创建AAR，createApplicationRecord（）。 所有您需要做的是将AAR嵌入到您的NdefMessage中。 您不想使用NdefMessage的第一条记录，除非AAR是NdefMessage中唯一的记录。 这是因为Android系统检查NdefMessage的第一条记录，以确定标签的MIME类型或URI，该标签用于创建应用程序进行过滤的意图。 以下代码显示如何创建AAR：



```java
NdefMessage msg = new NdefMessage(
        new NdefRecord[] {
            ...,
            NdefRecord.createApplicationRecord("com.example.android.beam")}
```







五、将NDEF消息发送到其他设备

Android Beam允许在两台Android设备之间进行简单的对等数据交换。 想要将数据发送到另一个设备的应用程序必须在前台，并且不能锁定接收数据的设备。 当发射装置与接收装置接近足够接触时，发射装置显示“触摸到光束”UI。 然后，用户可以选择是否向接收设备发送消息。





注意：前景NDEF推送可用于API级别10，它提供与Android Beam类似的功能。 这些API已被弃用，但可用于支持旧设备。 有关详细信息，请参阅enableForegroundNdefPush（）。





您可以通过调用以下两种方法之一为您的应用程序启用Android Beam：


1、setNdefPushMessage（）：接受一个NdefMessage设置为消息到波束。 当两个设备足够接近时，自动对消息进行射束。




2、setNdefPushMessageCallback（）：接受包含createNdefMessage（）的回调，当设备在范围内时，将调用数据。 回调允许您仅在必要时创建NDEF消息。




活动只能一次推送一个NDEF消息，因此如果同时设置这两个消息，setNdefPushMessageCallback（）优先于setNdefPushMessage（）。 要使用Android Beam，必须满足以下一般准则：


1、正在传播数据的活动必须处于前景。 两台设备必须将其屏幕解锁。




2、您必须将您正在传播的数据封装在NdefMessage对象中。




3、正在接收传输数据的NFC设备必须支持com.android.npp NDEF推送协议或NFC论坛的SNEP（简单NDEF交换协议）。 API级别9（Android 2.3）到API级别13（Android 3.2）的设备需要com.android.npp协议。 com.android.npp和SNEP都需要API级别14（Android 4.0）和更高版本。




注意：如果您的活动使Android Beam处于前台，则禁用标准意图调度系统。 但是，如果您的活动还启用前台调度，那么它仍然可以扫描与前台调度中设置的意图过滤器匹配的标签。





要启用Android Beam：


1、创建一个NdefMessage，其中包含要推送到其他设备的NdefRecords。




2、使用NdefMessage调用setNdefPushMessage（），或者调用setNdefPushMessageCallback传递在活动的onCreate（）方法中的NfcAdapter.CreateNdefMessageCallback对象。 这些方法至少需要一个您要使用Android Beam启用的活动，以及要激活的其他活动的可选列表。




一般来说，通常使用setNdefPushMessage（），如果您的Activity只需要在两个设备进行通信的范围内时始终推送相同的NDEF消息。 当您的应用程序关心应用程序的当前上下文并希望根据用户在应用程序中执行的操作推送NDEF消息时，可以使用setNdefPushMessageCallback。





以下示例显示一个简单活动如何在活动的onCreate（）方法中调用NfcAdapter.CreateNdefMessageCallback（有关完整示例，请参阅AndroidBeamDemo）。 此示例还具有帮助您创建MIME记录的方法：



```java
package com.example.android.beam;

import android.app.Activity;
import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.NfcAdapter.CreateNdefMessageCallback;
import android.nfc.NfcEvent;
import android.os.Bundle;
import android.os.Parcelable;
import android.widget.TextView;
import android.widget.Toast;
import java.nio.charset.Charset;


public class Beam extends Activity implements CreateNdefMessageCallback {
    NfcAdapter mNfcAdapter;
    TextView textView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        TextView textView = (TextView) findViewById(R.id.textView);
        // Check for available NFC Adapter
        mNfcAdapter = NfcAdapter.getDefaultAdapter(this);
        if (mNfcAdapter == null) {
            Toast.makeText(this, "NFC is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        // Register callback
        mNfcAdapter.setNdefPushMessageCallback(this, this);
    }

    @Override
    public NdefMessage createNdefMessage(NfcEvent event) {
        String text = ("Beam me up, Android!\n\n" +
                "Beam Time: " + System.currentTimeMillis());
        NdefMessage msg = new NdefMessage(
                new NdefRecord[] { createMime(
                        "application/vnd.com.example.android.beam", text.getBytes())
         /**
          * The Android Application Record (AAR) is commented out. When a device
          * receives a push with an AAR in it, the application specified in the AAR
          * is guaranteed to run. The AAR overrides the tag dispatch system.
          * You can add it back in to guarantee that this
          * activity starts when receiving a beamed message. For now, this code
          * uses the tag dispatch system.
          */
          //,NdefRecord.createApplicationRecord("com.example.android.beam")
        });
        return msg;
    }

    @Override
    public void onResume() {
        super.onResume();
        // Check to see that the Activity started due to an Android Beam
        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(getIntent().getAction())) {
            processIntent(getIntent());
        }
    }

    @Override
    public void onNewIntent(Intent intent) {
        // onResume gets called after this to handle the intent
        setIntent(intent);
    }

    /**
     * Parses the NDEF Message from the intent and prints to the TextView
     */
    void processIntent(Intent intent) {
        textView = (TextView) findViewById(R.id.textView);
        Parcelable[] rawMsgs = intent.getParcelableArrayExtra(
                NfcAdapter.EXTRA_NDEF_MESSAGES);
        // only one message sent during the beam
        NdefMessage msg = (NdefMessage) rawMsgs[0];
        // record 0 contains the MIME type, record 1 is the AAR, if present
        textView.setText(new String(msg.getRecords()[0].getPayload()));
    }
}
```


请注意，此代码注释出可以删除的AAR。 如果启用AAR，则AAR中指定的应用程序始终会收到Android Beam消息。 如果应用程序不存在，Google Play将开始下载该应用程序。 因此，以下意图过滤器对于Android 4.0设备在技术上不是必需的，或者稍后如果使用AAR：



```
<intent-filter>
  <action android:name="android.nfc.action.NDEF_DISCOVERED"/>
  <category android:name="android.intent.category.DEFAULT"/>
  <data android:mimeType="application/vnd.com.example.android.beam"/>
</intent-filter>
```


使用此意图过滤器，com.example.android.beam应用程序现在可以在扫描NFC标签或接收具有com.example.android.beam类型的AAR的Android Beam时启动，或者当NDEF格式化消息包含 类型application / vnd.com.example.android.beam的MIME记录。





即使AAR保证开始或下载应用程序，建议使用意向过滤器，因为它们允许您在应用程序中启动您选择的活动，而不是始终在AAR指定的包中启动主Activity。 AAR没有活动级别的粒度。 另外，由于某些Android驱动的设备不支持AAR，您还应该将识别信息嵌入到NDEF消息的第一个NDEF记录中，并对其进行过滤，以防万一。 有关如何创建记录的更多信息，请参阅创建NDEF记录的常见类型。






