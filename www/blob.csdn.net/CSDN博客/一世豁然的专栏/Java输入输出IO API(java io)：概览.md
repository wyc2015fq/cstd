# Java输入输出IO API(java.io)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月14日 08:06:54[一世豁然](https://me.csdn.net/Explorer_day)阅读数：383标签：[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[C/C++/Java](https://blog.csdn.net/Explorer_day/article/category/2276819)









通过数据流，序列化和文件系统提供系统输入和输出。 除非另有说明，否则将null参数传递到此程序包中任何类或接口中的构造函数或方法将导致抛出NullPointerException。




一、包规格


- [Java Object Serialization Specification](https://developer.android.google.cn/openjdk-redirect.html?v=8&path=/platform/serialization/spec/serialTOC.html)




二、相关文档

有关概述，教程，示例，指南和工具文档，请参阅：


- [Serialization Enhancements](https://developer.android.google.cn/openjdk-redirect.html?v=8&path=/technotes/guides/serialization)






**接口**

[Closeable](https://developer.android.google.cn/reference/java/io/Closeable.html)


可关闭是可以关闭的数据的源或目的地。




[DataInput](https://developer.android.google.cn/reference/java/io/DataInput.html)



DataInput接口提供从二进制流读取字节，并从其中重建任何Java基元类型的数据。





[DataOutput](https://developer.android.google.cn/reference/java/io/DataOutput.html)


DataOutput接口提供将数据从任何Java基本类型转换为一系列字节，并将这些字节写入二进制流。





[Externalizable](https://developer.android.google.cn/reference/java/io/Externalizable.html)


只有Externalizable实例的类的身份才能写入序列化流，并且该类负责保存和恢复其实例的内容。





[FileFilter](https://developer.android.google.cn/reference/java/io/FileFilter.html)


抽象路径名的过滤器。





[FilenameFilter](https://developer.android.google.cn/reference/java/io/FilenameFilter.html)


用于实现此接口的类的实例用于过滤文件名。





[Flushable](https://developer.android.google.cn/reference/java/io/Flushable.html)


Flushable是可以刷新的数据的目的地。





[ObjectInput](https://developer.android.google.cn/reference/java/io/ObjectInput.html)


ObjectInput扩展了DataInput接口以包含对象的读取。





[ObjectInputValidation](https://developer.android.google.cn/reference/java/io/ObjectInputValidation.html)


回调接口允许验证图中的对象。





[ObjectOutput](https://developer.android.google.cn/reference/java/io/ObjectOutput.html)


ObjectOutput扩展了DataOutput接口，包括写入对象。




[ObjectStreamConstants](https://developer.android.google.cn/reference/java/io/ObjectStreamConstants.html)


写入对象序列化流的常量。





[Serializable](https://developer.android.google.cn/reference/java/io/Serializable.html)


类的可串行性由实现java.io.Serializable接口的类启用。








**类**

[BufferedInputStream](https://developer.android.google.cn/reference/java/io/BufferedInputStream.html)


BufferedInputStream将功能添加到另一个输入流，即缓冲输入并支持标记和重置方法的能力。





[BufferedOutputStream](https://developer.android.google.cn/reference/java/io/BufferedOutputStream.html)


该类实现缓冲输出流。





[BufferedReader](https://developer.android.google.cn/reference/java/io/BufferedReader.html)


从字符输入流读取文本，缓冲字符，以提供字符，数组和行的有效读取。





[BufferedWriter](https://developer.android.google.cn/reference/java/io/BufferedWriter.html)


将文本写入字符输出流，缓冲字符以提供单个字符，数组和字符串的有效写入。





[ByteArrayInputStream](https://developer.android.google.cn/reference/java/io/ByteArrayInputStream.html)


ByteArrayInputStream包含一个内部缓冲区，其中包含可以从流中读取的字节。





[ByteArrayOutputStream](https://developer.android.google.cn/reference/java/io/ByteArrayOutputStream.html)


该类实现了将数据写入字节数组的输出流。





[CharArrayReader](https://developer.android.google.cn/reference/java/io/CharArrayReader.html)


该类实现了一个字符缓冲区，可以用作字符输入流。





[CharArrayWriter](https://developer.android.google.cn/reference/java/io/CharArrayWriter.html)


该类实现了可以用作写入器的字符缓冲区。





[Console](https://developer.android.google.cn/reference/java/io/Console.html)


访问与当前Java虚拟机关联的基于字符的控制台设备（如果有的话）的方法。





[DataInputStream](https://developer.android.google.cn/reference/java/io/DataInputStream.html)


数据输入流允许应用程序以独立于机器的方式从基础输入流读取原始Java数据类型。





[DataOutputStream](https://developer.android.google.cn/reference/java/io/DataOutputStream.html)


数据输出流使应用程序以便携式方式将原始Java数据类型写入输出流。





[File](https://developer.android.google.cn/reference/java/io/File.html)


文件和目录路径名的抽象表示。





[FileDescriptor](https://developer.android.google.cn/reference/java/io/FileDescriptor.html)


文件描述符类的实例用作表示开放文件，开放套接字或其他字节源或底层的特定于机器的结构的不透明句柄。





[FileInputStream](https://developer.android.google.cn/reference/java/io/FileInputStream.html)


FileInputStream从文件系统中的文件获取输入字节。





[FileOutputStream](https://developer.android.google.cn/reference/java/io/FileOutputStream.html)


文件输出流是用于将数据写入文件或FileDescriptor的输出流。





[FilePermission](https://developer.android.google.cn/reference/java/io/FilePermission.html)


旧版安全码; 不使用。





[FileReader](https://developer.android.google.cn/reference/java/io/FileReader.html)


阅读字符文件的方便类。





[FileWriter](https://developer.android.google.cn/reference/java/io/FileWriter.html)


用于编写字符文件的方便类。





[FilterInputStream](https://developer.android.google.cn/reference/java/io/FilterInputStream.html)


FilterInputStream包含一些其他输入流，它用作其基本的数据源，可能会沿着数据转换或提供附加功能。





[FilterOutputStream](https://developer.android.google.cn/reference/java/io/FilterOutputStream.html)


该类是过滤输出流的所有类的超类。





[FilterReader](https://developer.android.google.cn/reference/java/io/FilterReader.html)


用于读取过滤后的字符流的抽象类。





[FilterWriter](https://developer.android.google.cn/reference/java/io/FilterWriter.html)


用于编写过滤后的字符流的抽象类。





[InputStream](https://developer.android.google.cn/reference/java/io/InputStream.html)


这个抽象类是表示输入字节流的所有类的超类。





[InputStreamReader](https://developer.android.google.cn/reference/java/io/InputStreamReader.html)


InputStreamReader是从字节流到字符流的桥梁：它读取字节，并使用指定的字符集将其解码为字符。





[LineNumberInputStream](https://developer.android.google.cn/reference/java/io/LineNumberInputStream.html)


该类在API级别1中已被弃用。此类错误地假定字节充分表示字符。 从JDK 1.1开始，对字符流进行操作的首选方法是通过新的字符流类，其中包括一个用于计算行号的类。





[LineNumberReader](https://developer.android.google.cn/reference/java/io/LineNumberReader.html)


缓冲字符输入流，跟踪行号。





[ObjectInputStream](https://developer.android.google.cn/reference/java/io/ObjectInputStream.html)


ObjectInputStream反序列化先前使用ObjectOutputStream编写的原始数据和对象。





[ObjectInputStream.GetField](https://developer.android.google.cn/reference/java/io/ObjectInputStream.GetField.html)


提供对从输入流读取的持久性字段的访问。





[ObjectOutputStream](https://developer.android.google.cn/reference/java/io/ObjectOutputStream.html)


ObjectOutputStream将Java对象的原始数据类型和图形写入OutputStream。





[ObjectOutputStream.PutField](https://developer.android.google.cn/reference/java/io/ObjectOutputStream.PutField.html)


提供对要写入ObjectOutput的持久字段的编程访问。





[ObjectStreamClass](https://developer.android.google.cn/reference/java/io/ObjectStreamClass.html)


序列化的类描述符。





[ObjectStreamField](https://developer.android.google.cn/reference/java/io/ObjectStreamField.html)


Serializable类的Serializable字段的描述。





[OutputStream](https://developer.android.google.cn/reference/java/io/OutputStream.html)


这个抽象类是表示输出字节流的所有类的超类。





[OutputStreamWriter](https://developer.android.google.cn/reference/java/io/OutputStreamWriter.html)


OutputStreamWriter是从字符流到字节流的桥梁：使用指定的字符集将写入的字符编码为字节。





[PipedInputStream](https://developer.android.google.cn/reference/java/io/PipedInputStream.html)


]管道输入流应连接到管道输出流; 管道输入流然后提供写入管道输出流的任何数据字节。




[PipedOutputStream](https://developer.android.google.cn/reference/java/io/PipedOutputStream.html)


管道输出流可以连接到管道输入流以创建通信管道。





[PipedReader](https://developer.android.google.cn/reference/java/io/PipedReader.html)


流水字符输入流。





[PipedWriter](https://developer.android.google.cn/reference/java/io/PipedWriter.html)


流水字符输出流。





[PrintStream](https://developer.android.google.cn/reference/java/io/PrintStream.html)


PrintStream将功能添加到另一个输出流，即可以方便地打印各种数据值的表示。'





[PrintWriter](https://developer.android.google.cn/reference/java/io/PrintWriter.html)


将对象的格式表示打印到文本输出流。





[PushbackInputStream](https://developer.android.google.cn/reference/java/io/PushbackInputStream.html)


PushbackInputStream将功能添加到另一个输入流，即可以“推回”或“未读”一个字节。





[PushbackReader](https://developer.android.google.cn/reference/java/io/PushbackReader.html)


一个字符流读取器，允许将字符推回到流中。





[RandomAccessFile](https://developer.android.google.cn/reference/java/io/RandomAccessFile.html)


该类的实例支持读取和写入随机访问文件。





[Reader](https://developer.android.google.cn/reference/java/io/Reader.html)


用于读取字符流的抽象类。





[SequenceInputStream](https://developer.android.google.cn/reference/java/io/SequenceInputStream.html)


SequenceInputStream表示其他输入流的逻辑级联。





[SerializablePermission](https://developer.android.google.cn/reference/java/io/SerializablePermission.html)


旧版安全码; 不使用。





[StreamTokenizer](https://developer.android.google.cn/reference/java/io/StreamTokenizer.html)


StreamTokenizer类接收输入流并将其解析为“令牌”，允许一次读取一个令牌。





[StringBufferInputStream](https://developer.android.google.cn/reference/java/io/StringBufferInputStream.html)


此类在API级别1中已被弃用。此类不会将字符正确转换为字节。 从JDK 1.1开始，从字符串创建流的首选方法是通过StringReader类。





[StringReader](https://developer.android.google.cn/reference/java/io/StringReader.html)


一个字符流，其源是一个字符串。





[StringWriter](https://developer.android.google.cn/reference/java/io/StringWriter.html)


在字符串缓冲区中收集其输出的字符流，然后可以用于构造字符串。





[Writer](https://developer.android.google.cn/reference/java/io/Writer.html)


用于写入字符流的抽象类。








**异常**

[CharConversionException](https://developer.android.google.cn/reference/java/io/CharConversionException.html)


字符转换异常的基类。





[EOFException](https://developer.android.google.cn/reference/java/io/EOFException.html)


表示在输入过程中意外地达到文件结束或流结束。





[FileNotFoundException](https://developer.android.google.cn/reference/java/io/FileNotFoundException.html)


表示尝试打开由指定路径名表示的文件失败。





[InterruptedIOException](https://developer.android.google.cn/reference/java/io/InterruptedIOException.html)


表示I / O操作已中断。





[InvalidClassException](https://developer.android.google.cn/reference/java/io/InvalidClassException.html)


当序列化运行时检测到Class的以下问题之一时抛出。





[InvalidObjectException](https://developer.android.google.cn/reference/java/io/InvalidObjectException.html)


表示一个或多个反序列化对象失败的验证测试。





[IOException](https://developer.android.google.cn/reference/java/io/IOException.html)


表示发生了某种类型的I / O异常。





[NotActiveException](https://developer.android.google.cn/reference/java/io/NotActiveException.html)


序列化或反序列化不活跃时抛出。





[NotSerializableException](https://developer.android.google.cn/reference/java/io/NotSerializableException.html)


抛出一个实例需要一个Serializable接口。





[ObjectStreamException](https://developer.android.google.cn/reference/java/io/ObjectStreamException.html)


Object Stream类的特殊超类。





[OptionalDataException](https://developer.android.google.cn/reference/java/io/OptionalDataException.html)


表示由于未读原始数据导致的对象读取操作失败的异常，或属于流中序列化对象的数据的结束。





[StreamCorruptedException](https://developer.android.google.cn/reference/java/io/StreamCorruptedException.html)


当从对象流读取的控制信息违反内部一致性检查时抛出。





[SyncFailedException](https://developer.android.google.cn/reference/java/io/SyncFailedException.html)


发出同步操作失败的信号。





[UncheckedIOException](https://developer.android.google.cn/reference/java/io/UncheckedIOException.html)


用未经检查的异常包装IOException。





[UnsupportedEncodingException](https://developer.android.google.cn/reference/java/io/UnsupportedEncodingException.html)


字符编码不受支持。





[UTFDataFormatException](https://developer.android.google.cn/reference/java/io/UTFDataFormatException.html)


表明已经在数据输入流或实现数据输入接口的任何类中读取了修改后的UTF-8格式的格式不正确的字符串。





[WriteAbortedException](https://developer.android.google.cn/reference/java/io/WriteAbortedException.html)


指示在写入操作期间抛出了一个ObjectStreamExceptions。








**错误**

**[IOError](https://developer.android.google.cn/reference/java/io/IOError.html)**

发生严重I / O错误时抛出。




