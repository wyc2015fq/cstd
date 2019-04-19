# 在Android上使用XML - xqhrs232的专栏 - CSDN博客
2012年03月23日 11:07:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：505
原文地址::[http://www.ibm.com/developerworks/cn/xml/x-android/](http://www.ibm.com/developerworks/cn/xml/x-android/)
入门
在本文中，您将学习如何构建通过 Internet 使用 XML 的 Android 应用程序。Android 应用程序是使用 Java™ 编程语言编写的，因此具备 Java 技术方面的经验是必需的。要进行 Android 开发，您需要使用 Android SDK。本文中的所有代码适用于任何版本的 Android SDK，但 SDK 1.5_pre 是用于开发代码的。您可以使用 SDK 和一个文本编辑器来开发 Android 应用程序，但使用 Android Developer Tools (ADT)（一款
 Eclipse 插件）会更加简单。在本文中，我们使用 0.9 版本的 ADT 和 Eclipse 3.4.2, Java 版本。有关所有这些工具的链接，请参见 [参考资料](http://www.ibm.com/developerworks/cn/xml/x-android/#resources)。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
Android 上的 XML
Android 平台是一个开源移动开发平台。它允许您访问各种移动设备的所有方面，这些移动设备从低级图形设备到手机摄像头上的硬件不一而足。由于 Android 可以实现这么丰富的功能，因此您可能想知道为何还要为 XML 伤脑筋呢。并不是因为使用 XML 是多么地有趣；而是因为它能提供一些特殊的支持。XML 经常用作 Internet 上的一种数据格式。如果您希望通过 Internet 访问数据，则数据很有可能是 XML 格式。如果您希望发送数据给 Web 服务，那么您可能也需要发送 XML。简而言之，如果您的
 Android 应用程序将利用 Internet，那么您可能需要使用 XML。幸运的是，您可以采用多种方法在 Android 上使用 XML。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
XML 解析器
## 常用缩略语
- API：应用程序编程接口（Application programming interface） 
- RSS：Really Simple Syndication 
- SDK：软件开发包（Software Developers Kit） 
- UI：用户界面（User interface） 
- URL：通用资源定位符（Universal Resource Locator） 
- XML：可扩展标记语言（Extensible Markup Language） 
Android 平台最大的一个优势在于它利用了 Java 编程语言。Android SDK 并未向您的标准 Java Runtime Environment (JRE) 提供一切可用功能，但它支持其中很大一部分功能。Java 平台支持通过许多不同的方式来使用 XML，并且大多数与 XML 相关的 Java API 在 Android 上得到了完全支持。举例来说，Java 的 Simple API for XML (SAX) 和 Document Object Model (DOM) 在 Android
 上都是可用的。这些 API 多年以来一直都是 Java 技术的一部分。较新的 Streaming API for XML (StAX) 在 Android 中并不可用。但是， Android 提供了一个功能相当的库。最后，Java XML Binding API 在 Android 中也不可用。这个 API 已确定可以在 Android 中实现。但是，它更倾向于是一个重量级的 API，需要使用许多不同类的实例来表示 XML 文档。因此，这对于受限的环境，比如说 Android 针对的手持设备，不太理想。在后续小节中，我们将以
 Internet 上的一个简单的 XML 源为例，来看看如何在 Android 应用程序中使用上述各种 API 来解析它。首先，我们来看看这个简单应用程序的主要部分，它将通过 Internet 来使用 XML。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
Android 新闻阅读器
应用程序将从热门 Android 开发人员站点 Androidster 获取一个 RSS 提要，并将它解析为一组简单的 Java 对象，您可以使用这些对象构建一个 Android ListView（参见
[下载](http://www.ibm.com/developerworks/cn/xml/x-android/#download) 部分获取源代码）。这是一种典型的多态行为 — 提供相同行为的不同实现（不同的 XML 解析算法）。[清单
 1](http://www.ibm.com/developerworks/cn/xml/x-android/#list1) 展示了如何在 Java 代码中使用一个接口建立这一模型。
**清单 1. XML 提要解析器接口 **
				
package org.developerworks.android;
import java.util.List;
public interface FeedParser {
    List<Message> parse();
}
在 [清单 2](http://www.ibm.com/developerworks/cn/xml/x-android/#list2) 中，`Message` 类是一个典型的 Plain Old Java Object (POJO)，它表示一种数据结构。
**清单 2. `Message` POJO**
				
public class Message implements Comparable<Message>{
    static SimpleDateFormat FORMATTER = 
        new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss Z");
    private String title;
    private URL link;
    private String description;
    private Date date;
      // getters and setters omitted for brevity
    public void setLink(String link) {
        try {
            this.link = new URL(link);
        } catch (MalformedURLException e) {
            throw new RuntimeException(e);
        }
    }
    public String getDate() {
        return FORMATTER.format(this.date);
    }
    public void setDate(String date) {
        // pad the date if necessary
        while (!date.endsWith("00")){
            date += "0";
        }
        try {
            this.date = FORMATTER.parse(date.trim());
        } catch (ParseException e) {
            throw new RuntimeException(e);
        }
    }
    
    @Override
    public String toString() {
             // omitted for brevity
    }
    @Override
    public int hashCode() {
            // omitted for brevity
    }
    
    @Override
    public boolean equals(Object obj) {
            // omitted for brevity
    }
      // sort by date
    public int compareTo(Message another) {
        if (another == null) return 1;
        // sort descending, most recent first
        return another.date.compareTo(date);
    }
}
[清单 2](http://www.ibm.com/developerworks/cn/xml/x-android/#list2) 中的消息基本上是相当直观的。通过允许日期和链接作为简单的对象被访问，同时将它们表示为较强类型的对象（`java.util.Date` 和
`java.net.URL`），它隐藏了一些内部状态。它是一个典型的 Value Object，因此它基于其内部状态实现了 `equals()` 和
`hashCode()`。它还实现了 `Comparable` 接口，因此您可以使用它进行排序（按日期）。在实践中，提要中的数据始终是有序的，因为没有必要再进行排序。
每个解析器实现都需要提供一个 URL 给 Androidster 提要，并使用它打开一个到 Androidster 站点的 HTTP 连接。这一常见行为自然是在 Java 代码中建模，我们使用了一个抽象基类，如
[清单 3](http://www.ibm.com/developerworks/cn/xml/x-android/#list3) 所示。
**清单 3. 基本提要解析器类**
				
public abstract class BaseFeedParser implements FeedParser {
    // names of the XML tags
    static final String PUB_DATE = "pubDate";
    static final  String DESCRIPTION = "description";
    static final  String LINK = "link";
    static final  String TITLE = "title";
    static final  String ITEM = "item";
    
    final URL feedUrl;
    protected BaseFeedParser(String feedUrl){
        try {
            this.feedUrl = new URL(feedUrl);
        } catch (MalformedURLException e) {
            throw new RuntimeException(e);
        }
    }
    protected InputStream getInputStream() {
        try {
            return feedUrl.openConnection().getInputStream();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
基类存储 `feedUrl` 并使用它打开了一个 `java.io.InputStream`。如果出现任何差错，它会抛出一个
`RuntimeException`，造成应用程序出现故障。基类还为标记的名称定义了一些简单的常量。[清单 4](http://www.ibm.com/developerworks/cn/xml/x-android/#list4) 显示了提要中的一些示例内容，以便于您理解这些标记的重要性。
**清单 4. 示例 XML 提要**
				
<?xml version="1.0" encoding="UTF-8"?>
<!-- generator="FeedCreator 1.7.2" -->
<rss version="2.0">
    
  <channel>
        
    <title>android_news</title>
           
    <description>android_news</description>
        
    <link>http://www.androidster.com/android_news.php</link>
        
    <lastBuildDate>Sun, 19 Apr 2009 19:43:45 +0100</lastBuildDate>
        
    <generator>FeedCreator 1.7.2</generator>
        
    <item>
            
      <title>Samsung S8000 to Run Android, Play DivX, Take Over the 
World</title>
             
      <link>http://www.androidster.com/android_news/samsung-s8000-to-run-android-
play-divx-take-over-the-world</link>
            
      <description>More details have emerged on the first Samsung handset 
to run Android. A yet-to-be announced phone called the S8000 is being 
reported ...</description>
            
      <pubDate>Thu, 16 Apr 2009 07:18:51 +0100</pubDate>
        
    </item>
        
    <item>
            
      <title>Android Cupcake Update on the Horizon</title>
            
      <link>http://www.androidster.com/android_news/android-cupcake-update-
on-the-horizon</link>
            
      <description>After months of discovery and hearsay, the Android 
build that we have all been waiting for is about to finally make it 
out ...</description>
            
      <pubDate>Tue, 14 Apr 2009 04:13:21 +0100</pubDate>
        
    </item>
    
  </channel>
</rss>
如 [清单 4](http://www.ibm.com/developerworks/cn/xml/x-android/#list4) 中的示例所示，一个
`ITEM` 对应于一个 `Message` 实例。项目的子节点（`TITLE`、`LINK` 等）对应于
`Message` 实例的属性。现在，您已经对提要有了一定的认识，并且已经创建了所有常用部分，接下来看看如何使用 Android 上可用的各种技术来解析这个提要。您将从 SAX 开始。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
使用 SAX
在 Java 环境中，当您需要一个速度快的解析器并且希望最大限度减少应用程序的内存占用时，通常可以使用 SAX API。这非常适用于运行 Android 的移动设备。您可以在 Java 环境中照原样使用 SAX API，在 Android 上运行它不需要做任何修改。[清单 5](http://www.ibm.com/developerworks/cn/xml/x-android/#list5) 显示了
`FeedParser` 接口的一个 SAX 实现。
**清单 5. SAX 实现**
				
public class SaxFeedParser extends BaseFeedParser {
    protected SaxFeedParser(String feedUrl){
        super(feedUrl);
    }
    
    public List<Message> parse() {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        try {
            SAXParser parser = factory.newSAXParser();
            RssHandler handler = new RssHandler();
            parser.parse(this.getInputStream(), handler);
            return handler.getMessages();
        } catch (Exception e) {
            throw new RuntimeException(e);
        } 
    }
}
如果您以前使用过 SAX，那么这对您肯定非常熟悉。与任何 SAX 实现相同，大多数细节都在 SAX 处理程序中。在分解 XML 文档时，处理程序从 SAX 解析器接收事件。在本例中，您创建了一个新的名称为
`RssHandler` 的类，并将它注册为解析器的处理程序，如 [清单 6](http://www.ibm.com/developerworks/cn/xml/x-android/#list6) 所示。
**清单 6. SAX 处理程序**
				
import static org.developerworks.android.BaseFeedParser.*;
public class RssHandler extends DefaultHandler{
    private List<Message> messages;
    private Message currentMessage;
    private StringBuilder builder;
    
    public List<Message> getMessages(){
        return this.messages;
    }
    @Override
    public void characters(char[] ch, int start, int length)
            throws SAXException {
        super.characters(ch, start, length);
        builder.append(ch, start, length);
    }
    @Override
    public void endElement(String uri, String localName, String name)
            throws SAXException {
        super.endElement(uri, localName, name);
        if (this.currentMessage != null){
            if (localName.equalsIgnoreCase(TITLE)){
                currentMessage.setTitle(builder.toString());
            } else if (localName.equalsIgnoreCase(LINK)){
                currentMessage.setLink(builder.toString());
            } else if (localName.equalsIgnoreCase(DESCRIPTION)){
                currentMessage.setDescription(builder.toString());
            } else if (localName.equalsIgnoreCase(PUB_DATE)){
                currentMessage.setDate(builder.toString());
            } else if (localName.equalsIgnoreCase(ITEM)){
                messages.add(currentMessage);
            }
            builder.setLength(0);    
        }
    }
    @Override
    public void startDocument() throws SAXException {
        super.startDocument();
        messages = new ArrayList<Message>();
        builder = new StringBuilder();
    }
    @Override
    public void startElement(String uri, String localName, String name,
            Attributes attributes) throws SAXException {
        super.startElement(uri, localName, name, attributes);
        if (localName.equalsIgnoreCase(ITEM)){
            this.currentMessage = new Message();
        }
    }
}
`RssHandler` 类扩展了 `org.xml.sax.helpers.DefaultHandler` 类。该类为 SAX 解析器生成的事件所对应的所有方法都提供了一个默认的非操作实现。这允许子类根据需要仅覆盖一些方法。`RssHandler` 提供了一个额外的 API，即
`getMessages`。它返回处理程序在从 SAX 解析器接收事件时所收集的 `Message` 对象列表。它有另外两个内部变量，`currentMessage` 针对被解析的
`Message` 实例，以及名称为 `builder` 的 `StringBuilder` 变量，用于存储文本节点中的字符数据。解析器将相应事件发送给处理程序时会调用
`startDocument` 方法，这两个变量的初始化操作就是在此时完成。
查看 [清单 6](http://www.ibm.com/developerworks/cn/xml/x-android/#list6) 中的
`startElement` 方法。在 XML 文档中每次遇到开始标记时都会调用它。您只关心该标记何时为 `ITEM` 标记。对于这种情况，您将创建一个新的
`Message`。现在来看 `characters` 方法。遇到文本节点中的字符数据时便会调用此方法。数据只是被添加到
`builder` 变量中。最后，我们来看 `endElement` 方法。遇到结束标记时会调用此方法。对于与某 
```
Message
```
 属性相对应的标记，如 `TITLE` 和 `LINK`，则使用 `builder` 变量中的数据在
`currentMessage` 上设置适当的属性。如果结束标记是一个 `ITEM`，则 `currentMessage` 将被添加到 Messages 列表中。所有这些都是非常典型的 SAX 解析；此处的一切都不是 Android 所特有的。因此，如果您知道如何编写 Java SAX 解析器，则应该知道如何编写 Android SAX 解析器。但是，Android SDK 确实在 SAX 上添加了一些便捷的特性。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
更加简单的 SAX 解析
Android SDK 提供了一个名称为 `android.util.Xml` 的实用类。[清单 7](http://www.ibm.com/developerworks/cn/xml/x-android/#list7) 展示了如何使用这个相同的实用类来设置一个 SAX 解析器。
**清单 7. Android SAX 解析器**
				
public class AndroidSaxFeedParser extends BaseFeedParser {
    public AndroidSaxFeedParser(String feedUrl) {
        super(feedUrl);
    }
    public List<Message> parse() {
        RssHandler handler = new RssHandler();
        try {
            Xml.parse(this.getInputStream(), Xml.Encoding.UTF_8, handler);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        return handler.getMessages();
    }
}
注意，这个类仍然使用了一个标准的 SAX 处理程序，因此您仅仅重用了 [清单 7](http://www.ibm.com/developerworks/cn/xml/x-android/#list7) 中所示的 `RssHandler`。能够重用 SAX 处理程序是非常不错的，但其代码稍微有些复杂。您可以想像，如果需要解析一个更加复杂的 XML 文档，则处理程序可能会带来各种各样的 bug。举例来说，回头看看
[清单 6](http://www.ibm.com/developerworks/cn/xml/x-android/#list6) 中的
`endElement` 方法。注意，在尝试设置属性之前，它检查了 `currentMessage` 是否为 null。现在，再回头看看
[清单 4](http://www.ibm.com/developerworks/cn/xml/x-android/#list4) 中的示例 XML。注意，`ITEM` 标记外部有一些
`TITLE` 和 `LINK` 标记。这就是使用 null 检查的原因。否则，每一个 `TITLE` 标记会导致一个
`NullPointerException`。Android 提供了自己独有的 SAX API（参见 [清单 8](http://www.ibm.com/developerworks/cn/xml/x-android/#list8)），它排除了您编写自己的 SAX 处理程序的需要。
**清单 8. 经过简化的 Android SAX 解析器**
				
public class AndroidSaxFeedParser extends BaseFeedParser {
    public AndroidSaxFeedParser(String feedUrl) {
        super(feedUrl);
    }
    public List<Message> parse() {
        final Message currentMessage = new Message();
        RootElement root = new RootElement("rss");
        final List<Message> messages = new ArrayList<Message>();
        Element channel = root.getChild("channel");
        Element item = channel.getChild(ITEM);
        item.setEndElementListener(new EndElementListener(){
            public void end() {
                messages.add(currentMessage.copy());
            }
        });
        item.getChild(TITLE).setEndTextElementListener(new EndTextElementListener(){
            public void end(String body) {
                currentMessage.setTitle(body);
            }
        });
        item.getChild(LINK).setEndTextElementListener(new EndTextElementListener(){
            public void end(String body) {
                currentMessage.setLink(body);
            }
        });
        item.getChild(DESCRIPTION).setEndTextElementListener(new 
EndTextElementListener(){
            public void end(String body) {
                currentMessage.setDescription(body);
            }
        });
        item.getChild(PUB_DATE).setEndTextElementListener(new EndTextElementListener(){
            public void end(String body) {
                currentMessage.setDate(body);
            }
        });
        try {
            Xml.parse(this.getInputStream(), Xml.Encoding.UTF_8, 
root.getContentHandler());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        return messages;
    }
}
新的 SAX 解析代码并未使用 SAX 处理程序，而是使用了 SDK 中的 android.sax 包中的类。这些类允许您构建 XML 文档的结构，并根据需要添加事件监听程序。在以上代码中，您声明文档将有一个
`rss` 根元素，并且它有一个 `channel` 子元素。然后，您声明 `channel` 将有一个
`ITEM` 子元素，并且开始添加监听程序。对于每个监听程序，您都使用了一个实现了特定接口（`EndElementListner` 或
`EndTextElementListener`）的匿名内部类。注意，您不需要跟踪字符数据。不仅仅因为这样会更加简单，更重要的是更加高效。最后，在调用 Xml.parse 实用方法时，您将传递一个通过根元素生成的处理程序。
[清单 8](http://www.ibm.com/developerworks/cn/xml/x-android/#list8) 中的所有代码都是可选的。如果您习惯 Java 环境中的标准 SAX 解析代码，那么您可以坚持使用它。如果您希望尝试 Android SDK 所提供的便捷的包装器，那么也可以使用它。如果您完全不希望使用 SAX 会怎样呢？可以使用一些备选方案。其中的首选方法就是 DOM。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
使用 DOM
Android 完全支持 DOM 解析，就像在桌面机器或服务器上使用 Java 代码运行它一样。[清单 9](http://www.ibm.com/developerworks/cn/xml/x-android/#list9) 显示了一个基于 DOM 的解析器接口实现。
**清单 9. 基于 DOM 的提要解析器实现**
				
public class DomFeedParser extends BaseFeedParser {
    protected DomFeedParser(String feedUrl) {
        super(feedUrl);
    }
    public List<Message> parse() {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        List<Message> messages = new ArrayList<Message>();
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            Document dom = builder.parse(this.getInputStream());
            Element root = dom.getDocumentElement();
            NodeList items = root.getElementsByTagName(ITEM);
            for (int i=0;i<items.getLength();i++){
                Message message = new Message();
                Node item = items.item(i);
                NodeList properties = item.getChildNodes();
                for (int j=0;j<properties.getLength();j++){
                    Node property = properties.item(j);
                    String name = property.getNodeName();
                    if (name.equalsIgnoreCase(TITLE)){
                        message.setTitle(property.getFirstChild().getNodeValue());
                    } else if (name.equalsIgnoreCase(LINK)){
                        message.setLink(property.getFirstChild().getNodeValue());
                    } else if (name.equalsIgnoreCase(DESCRIPTION)){
                        StringBuilder text = new StringBuilder();
                        NodeList chars = property.getChildNodes();
                        for (int k=0;k<chars.getLength();k++){
                            text.append(chars.item(k).getNodeValue());
                        }
                        message.setDescription(text.toString());
                    } else if (name.equalsIgnoreCase(PUB_DATE)){
                        message.setDate(property.getFirstChild().getNodeValue());
                    }
                }
                messages.add(message);
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        } 
        return messages;
    }
}
与第一个 SAX 示例类似，以上代码完全没有特定于 Android 的地方。DOM 解析器将所有 XML 文档读取到内存中，然后允许您使用 DOM API 遍历 XML 树、检索所需的数据。这是非常直观的代码，并且，在某些方面比基于 SAX 的实现更加简单。但是，DOM 通常更加占用内存，因为一切内容都会先读取到内存中。这对于运行 Android 的移动设备来说是一个问题，但是当 XML 文档始终保持很小的大小时是可行的。这可能意味着，Android 的开发人员会认为 SAX 解析在 Android 应用程序上更加常见，因此为它提供了额外的实用工具。Android
 还提供了另一种类型的 XML 解析器，它就是 pull 解析器。 
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
XML pull 解析器
如前所述，Android 并未提供对 Java StAX API 的支持。但是，Android 确实附带了一个 pull 解析器，其工作方式类似于 StAX。它允许您的应用程序代码从解析器中获取事件，这与 SAX 解析器自动将事件推入处理程序相反。[清单 10](http://www.ibm.com/developerworks/cn/xml/x-android/#list10) 显示了提要解析接口的一个 pull
 解析器实现。
**清单 10. 基于 Pull 解析器的实现**
				
public class XmlPullFeedParser extends BaseFeedParser {
    public XmlPullFeedParser(String feedUrl) {
        super(feedUrl);
    }
    public List<Message> parse() {
        List<Message> messages = null;
        XmlPullParser parser = Xml.newPullParser();
        try {
            // auto-detect the encoding from the stream
            parser.setInput(this.getInputStream(), null);
            int eventType = parser.getEventType();
            Message currentMessage = null;
            boolean done = false;
            while (eventType != XmlPullParser.END_DOCUMENT && !done){
                String name = null;
                switch (eventType){
                    case XmlPullParser.START_DOCUMENT:
                        messages = new ArrayList<Message>();
                        break;
                    case XmlPullParser.START_TAG:
                        name = parser.getName();
                        if (name.equalsIgnoreCase(ITEM)){
                            currentMessage = new Message();
                        } else if (currentMessage != null){
                            if (name.equalsIgnoreCase(LINK)){
                                currentMessage.setLink(parser.nextText());
                            } else if (name.equalsIgnoreCase(DESCRIPTION)){
                                currentMessage.setDescription(parser.nextText());
                            } else if (name.equalsIgnoreCase(PUB_DATE)){
                                currentMessage.setDate(parser.nextText());
                            } else if (name.equalsIgnoreCase(TITLE)){
                                currentMessage.setTitle(parser.nextText());
                            }    
                        }
                        break;
                    case XmlPullParser.END_TAG:
                        name = parser.getName();
                        if (name.equalsIgnoreCase(ITEM) && 
currentMessage != null){
                            messages.add(currentMessage);
                        } else if (name.equalsIgnoreCase(CHANNEL)){
                            done = true;
                        }
                        break;
                }
                eventType = parser.next();
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        return messages;
    }
}
pull 解析器的运行方式与 SAX 解析器相似。它提供了类似的事件（开始元素和结束元素），但您需要使用 `(parser.next()` 提取它们。事件将作为数值代码被发送，因此您可以使用一个简单 case-switch。注意，解析并未像 SAX 解析那样监听元素的结束，而是在开始处完成了大部分处理。在
[清单 10](http://www.ibm.com/developerworks/cn/xml/x-android/#list10) 的代码中，当某个元素开始时，您可以调用
`parser.nextText()` 从 XML 文档中提取所有字符数据。还需注意，您设置了一个标记（布尔变量 `done`）来确定何时到达感兴趣内容的结束部分。这允许您提早停止读取 XML 文档，因为您知道代码将不会关心文档的其余部分。这有时非常实用，特别是当您只需要访问一小部分 XML 文档时。通过尽快停止解析，您可以极大地减少解析时间。这种优化对于连接速度较慢的移动设备尤为重要。pull 解析器可以提供一些性能优势以及易用性。它还可以用于编写 XML。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
创建 XML
目前为止，我一直专注于通过 Internet 解析 XML。但是，有时您的应用程序可能需要将 XML 发送到远程服务器。显然，您可以只使用一个 
```
StringBuilder
```
 来创建 XML 字符串。另一种备选方法来自 [清单 11](http://www.ibm.com/developerworks/cn/xml/x-android/#list11) 中的 Pull 解析器。
**清单 11. 使用 pull 解析器编写 XML**
				
private String writeXml(List<Message> messages){
    XmlSerializer serializer = Xml.newSerializer();
    StringWriter writer = new StringWriter();
    try {
        serializer.setOutput(writer);
        serializer.startDocument("UTF-8", true);
        serializer.startTag("", "messages");
        serializer.attribute("", "number", String.valueOf(messages.size()));
        for (Message msg: messages){
            serializer.startTag("", "message");
            serializer.attribute("", "date", msg.getDate());
            serializer.startTag("", "title");
            serializer.text(msg.getTitle());
            serializer.endTag("", "title");
            serializer.startTag("", "url");
            serializer.text(msg.getLink().toExternalForm());
            serializer.endTag("", "url");
            serializer.startTag("", "body");
            serializer.text(msg.getDescription());
            serializer.endTag("", "body");
            serializer.endTag("", "message");
        }
        serializer.endTag("", "messages");
        serializer.endDocument();
        return writer.toString();
    } catch (Exception e) {
        throw new RuntimeException(e);
    } 
}
`XmlSerializer` 类是 [前一部分](http://www.ibm.com/developerworks/cn/xml/x-android/#xpull) 所使用的 `XmlPullParser` 包的一部分。它没有提取事件，而是将它们推出到数据流或编写程序中。在本例中，它仅仅将事件推送到了一个
`java.io.StringWriter` 实例中。它提供了一个直观的 API，通过各种方法开始和结束文档、处理元素以及添加文本或属性。这是
`StringBuilder` 的一种出色的替换方案，因为它可以更加轻松地确保您的 XML 具有良好结构。
[**回页首**](http://www.ibm.com/developerworks/cn/xml/x-android/#ibm-pcon)
结束语
您希望为 Android 设备构建何种类型的应用程序？无论如何，如果它需要通过 Internet 使用数据，那么都可能需要使用 XML。在本文中，您看到 Android 提供了大量用于处理 XML 的工具。您可以选择其中之一作为自己的工具，或者您可以根据用例来进行选择。大多数时间，使用 SAX 是比较安全的，并且 Android 提供了一种传统的 SAX 使用方法，以及一个便捷的 SAX 包装器。如果您的文档比较小，那么 DOM 可能是一种比较简单的方法。如果您的文档比较大，但您只需要文档的一部分，则
 XML Pull 解析器可能是更为有效的方法。最后，对于编写 XML，Pull 解析器包也提供了一种便捷的方法。因此，无论您的 XML 需求如何，Android 都能在一定程度上满足它们。
