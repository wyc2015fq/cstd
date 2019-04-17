# FFMPEG学习【libavformat】:Public Metadata API - 一世豁然的专栏 - CSDN博客





2017年06月15日 08:59:07[一世豁然](https://me.csdn.net/Explorer_day)阅读数：497标签：[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/Explorer_day/article/category/6289310)









元数据API允许libavformat在分解时将元数据标签导出到客户端应用程序。




相反，它允许客户端应用程序在复用时设置元数据。





元数据将使用AVDictionary API在AVFormatContext，AVStream，AVChapter和AVProgram结构的“元数据”字段中导出或设置为键/值字符串对。 像FFmpeg中的所有字符串一样，元数据被假定为UTF-8编码的Unicode。 请注意，在大多数情况下，由demuxers导出的元数据不会被检查为有效的UTF-8。





要牢记的重要概念：


1、钥匙是独一无二的 永远不会有2个相同键的标签。 这也意味着语义上的意思，即解复用器不应该故意地产生几个不同但在语义上相同的键。 例如key = Author5，key = Author6。 在这个例子中，所有作者必须放在同一标签中。




2、元数据是平的，不是分层的; 没有子标签。 如果要存储例如制作人Alice和演员Bob的子代的电子邮件地址，可以使用key = alice_and_bobs_childs_email_address。




3、几个修饰符可以应用于标签名称。 这是通过以下列列表中的顺序附加破折号字符（' - '）和修饰符名称来完成的：例如 foo-eng-sort，而不是foo-sort-eng。

 1）、语言 - 其特定语言的本地化标签附加ISO 639-2 / B 3字母语言代码。 例如：Author-ger = Michael，Author-eng = Mike原始/默认语言在不合格的“作者”标签中。 如果设置任何已翻译的标签，解组员应设置默认值。

 2）、排序 - 应该用于排序的标签的修改版本将附加'-sort'。 例如。 artist =“The Beatles”，artist-sort =“Beatles，The”。




4、一些协议和解复用器支持元数据更新。 成功调用av_read_packet（）后，AVFormatContext.event_flags或AVStream.event_flags将被更新，以指示元数据是否已更改。 为了检测流中的元数据更改，您需要循环访问AVFormatContext中的所有流，并检查其各自的event_flags。




5、Demuxers尝试以通用格式导出元数据，但是没有通用等价物的标签将保留在容器中。 遵循通用标签名称列表：



```
专辑 - 此作品所属集的名称
album_artist - 集合/专辑的主要创作者，如果与艺术家不同。例如“各种艺术家”编辑专辑。
艺术家 - 作品的主要创作者
评论 - 文件的任何其他说明。
作曲家 - 谁组成的作品，如果不同于艺术家。
版权所有者版权所有者。
creation_time--创建文件的日期，最好在ISO 8601中。
日期 - 创建日期，最好在ISO 8601中。
盘 - 子集的数量，例如。光盘在多盘集合中。
编码器 - 生成文件的软件/硬件的名称/设置。
encoded_by - 创建该文件的个人/组。
filename - 文件的原始名称。
流派 - <不言而喻>。
语言 - 最重要的语言采用ISO 639-2格式。可以指定多种语言用逗号分隔它们。
表演艺术家-如果不同于艺术家，他们完成了这项工作。例如，“也是sprach Zarathustra”，艺术家将是“理查德施特劳斯“和表演者”伦敦爱乐乐团“。
发布商 - 标签/发布商的名称。
service_name - 广播中服务的名称（通道名称）。
service_provider - 广播中服务提供商的名称。
标题 - 作品的名称。
轨道 - 该工作的数量在集合中，可以以当前/总计的形式。
variant_bitrate - 当前流是其一部分的比特率变体的总比特率
```


查看[示例](http://ffmpeg.org/doxygen/trunk/metadata_8c-example.html)部分，了解如何使用元数据API的应用示例。



