# 链接：NFC：高级NFC - 一世豁然的专栏 - CSDN博客





2017年05月17日 14:02:02[一世豁然](https://me.csdn.net/Explorer_day)阅读数：384








本文档介绍了高级NFC主题，例如使用各种标签技术，编写NFC标签和前台调度，这使得前台的应用程序即使在其他应用程序过滤相同的应用程序时也可以处理意图。








一、使用支持的标签技术

使用NFC标签和Android设备时，用于读取和写入标签数据的主要格式是NDEF。 当设备用NDEF数据扫描标签时，Android可以解析消息并在可能的情况下将其传送到NdefMessage中。 但是，当您扫描不包含NDEF数据的标签或NDEF数据无法映射到MIME类型或URI时，有些情况。 在这种情况下，您需要直接打开与标签的通信，并用自己的协议（以原始字节）读写。 Android使用android.nfc.tech包提供了对这些用例的通用支持，如表1所示。您可以使用getTechList（）方法来确定标签支持的技术，并使用其中一个由android.nfc.tech提供类创建相应的TagTechnology对象。





表1.支持的标签技术



|类|描述|
|----|----|
|`[TagTechnology](https://developer.android.google.cn/reference/android/nfc/tech/TagTechnology.html)`|所有标签技术类必须实现的接口。|
|`[NfcA](https://developer.android.google.cn/reference/android/nfc/tech/NfcA.html)`|提供对NFC-A（ISO 14443-3A）属性和I / O操作的访问。|
|`[NfcB](https://developer.android.google.cn/reference/android/nfc/tech/NfcB.html)`|提供对NFC-B（ISO 14443-3B）属性和I / O操作的访问。|
|`[NfcF](https://developer.android.google.cn/reference/android/nfc/tech/NfcF.html)`|提供对NFC-F（JIS 6319-4）属性和I / O操作的访问。|
|`[NfcV](https://developer.android.google.cn/reference/android/nfc/tech/NfcV.html)`|提供对NFC-V（ISO 15693）属性和I / O操作的访问。|
|`[IsoDep](https://developer.android.google.cn/reference/android/nfc/tech/IsoDep.html)`|提供访问ISO-DEP（ISO 14443-4）属性和I / O操作。|
|`[Ndef](https://developer.android.google.cn/reference/android/nfc/tech/Ndef.html)`|提供对NDEF数据的访问，以及格式为NDEF的NFC标签的操作。|
|`[NdefFormatable](https://developer.android.google.cn/reference/android/nfc/tech/NdefFormatable.html)`|为可能为NDEF格式化的标签提供格式化操作。|


Android驱动的设备不需要支持以下标签技术。





表2.可选支持的标签技术


|类|描述|
|----|----|
|`[MifareClassic](https://developer.android.google.cn/reference/android/nfc/tech/MifareClassic.html)`|如果此Android设备支持MIFARE，则可以访问MIFARE Classic属性和I / O操作。|
|`[MifareUltralight](https://developer.android.google.cn/reference/android/nfc/tech/MifareUltralight.html)`|如果此Android设备支持MIFARE，则可以访问MIFARE Ultralight属性和I / O操作。|


一）、使用标签技术和ACTION_TECH_DISCOVERED意图

当设备扫描其上具有NDEF数据但不能映射到MIME或URI的标签时，标签分派系统将尝试以ACTION_TECH_DISCOVERED意图开始活动。 当扫描具有非NDEF数据的标签时，还会使用ACTION_TECH_DISCOVERED。 如果标签调度系统无法为您解析，则使用此回退可以直接处理标签上的数据。 使用标签技术时的基本步骤如下：


1、过滤ACTION_TECH_DISCOVERED意图，指定要处理的标签技术。 有关详细信息，请参阅过滤NFC意图。 通常，当NDEF消息无法映射到MIME类型或URI或者扫描的标签不包含NDEF数据时，标签分发系统尝试启动ACTION_TECH_DISCOVERED意图。 有关如何确定的更多信息，请参阅标签调度系统。




2、当您的应用程序收到意图时，从意图获取Tag对象：



```java
Tag tagFromIntent = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
```


3、通过调用android.nfc.tech包中的类之一的工厂方法获取TagTechnology的一个实例。 您可以在调用get factory方法之前调用getTechList（）来枚举支持的标签技术。 例如，要从标签获取MifareUltralight的实例，请执行以下操作：



```java
MifareUltralight.get(intent.getParcelableExtra(NfcAdapter.EXTRA_TAG));
```



二）、读写标签

读取和写入NFC标签涉及从意图获取标签并开始与标签的通信。 您必须定义自己的协议栈来读写数据到标签。 但请记住，在直接使用标签时，您仍然可以读取和写入NDEF数据。 这取决于你想要结构的东西。 以下示例显示如何使用MIFARE Ultralight标签。




```java
package com.example.android.nfc;

import android.nfc.Tag;
import android.nfc.tech.MifareUltralight;
import android.util.Log;
import java.io.IOException;
import java.nio.charset.Charset;

public class MifareUltralightTagTester {

    private static final String TAG = MifareUltralightTagTester.class.getSimpleName();

    public void writeTag(Tag tag, String tagText) {
        MifareUltralight ultralight = MifareUltralight.get(tag);
        try {
            ultralight.connect();
            ultralight.writePage(4, "abcd".getBytes(Charset.forName("US-ASCII")));
            ultralight.writePage(5, "efgh".getBytes(Charset.forName("US-ASCII")));
            ultralight.writePage(6, "ijkl".getBytes(Charset.forName("US-ASCII")));
            ultralight.writePage(7, "mnop".getBytes(Charset.forName("US-ASCII")));
        } catch (IOException e) {
            Log.e(TAG, "IOException while closing MifareUltralight...", e);
        } finally {
            try {
                ultralight.close();
            } catch (IOException e) {
                Log.e(TAG, "IOException while closing MifareUltralight...", e);
            }
        }
    }

    public String readTag(Tag tag) {
        MifareUltralight mifare = MifareUltralight.get(tag);
        try {
            mifare.connect();
            byte[] payload = mifare.readPages(4);
            return new String(payload, Charset.forName("US-ASCII"));
        } catch (IOException e) {
            Log.e(TAG, "IOException while writing MifareUltralight
            message...", e);
        } finally {
            if (mifare != null) {
               try {
                   mifare.close();
               }
               catch (IOException e) {
                   Log.e(TAG, "Error closing tag...", e);
               }
            }
        }
        return null;
    }
}
```






二、使用前台调度系统

前台调度系统允许活动拦截意图，并要求处理相同意图的其他活动的优先级。 使用此系统涉及为Android系统构建一些数据结构，以便能够将适当的意图发送到您的应用程序。 启用前台调度系统：


1、在您的活动的onCreate（）方法中添加以下代码：

 1）、创建一个PendingIntent对象，以便Android系统可以在扫描时使用标签的详细信息进行填充。



```java
PendingIntent pendingIntent = PendingIntent.getActivity(
    this, 0, new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
```
 2）、声明意图过滤器来处理要拦截的意图。 前台调度系统按照设备扫描标签时收到的意图来检查指定的意图过滤器。 如果它匹配，那么你的应用程序处理意图。 如果不匹配，则前台调度系统将返回到意向调度系统。 指定一个null数组的意图过滤器和技术过滤器，指定您要过滤掉所有标记为TAG_DISCOVERED意图。 下面的代码片段处理NDEF_DISCOVERED的所有MIME类型。 你应该只处理你需要的。



```java
IntentFilter ndef = new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED);
    try {
        ndef.addDataType("*/*");    /* Handles all MIME based dispatches.
                                       You should specify only the ones that you need. */
    }
    catch (MalformedMimeTypeException e) {
        throw new RuntimeException("fail", e);
    }
   intentFiltersArray = new IntentFilter[] {ndef, };
```
 3）、设置应用程序想要处理的一系列标签技术。 调用Object.class.getName（）方法来获取要支持的技术类。



```java
techListsArray = new String[][] { new String[] { NfcF.class.getName() } };
```


2、覆盖以下活动生命周期回调，并添加逻辑以在活动丢失（onPause（））并重新获取（onResume（））时重新启用和禁用前台调度。 enableForegroundDispatch（）必须从主线程调用，只有当活动在前台时（调用onResume（）才能保证）。 您还需要实施onNewIntent回调来处理扫描的NFC标签中的数据。



```java
public void onPause() {
    super.onPause();
    mAdapter.disableForegroundDispatch(this);
}

public void onResume() {
    super.onResume();
    mAdapter.enableForegroundDispatch(this, pendingIntent, intentFiltersArray, techListsArray);
}

public void onNewIntent(Intent intent) {
    Tag tagFromIntent = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
    //do something with tagFromIntent
}
```



有关完整的示例，请参阅API Demos中的ForegroundDispatch示例。



