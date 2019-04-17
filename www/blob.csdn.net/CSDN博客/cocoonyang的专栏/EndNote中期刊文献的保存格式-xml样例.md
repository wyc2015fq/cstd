# EndNote中期刊文献的保存格式 - xml样例 - cocoonyang的专栏 - CSDN博客





2014年08月31日 20:11:02[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1053








EndNote是文献编辑利器，它可直接解析pdf格式文献，并从中提取出文献的作者名、期刊名等信息。但是EndNote不支持.caj格式文献，也就是说不能直接通过引入（import）将.caj格式文献添加EndNote中（我使用的是EndNote 16）。

欲向EndNote中import .caj格式文献时，可根据EndNote文献的xml保存格式编辑一个xml文件，然后就可通过EndNote->File->Import->File，其中 Import
 Option选项使用EndNote generated XML，就可顺利将文献引入EndNote中。



如下所示为EndNote文献的xml保存格式文件的一个简单样例。





<xml>




<records>




<record>


<databasename="reference.enl"path="C:\Users\foo\Documents\reference\reference.enl">reference.enl</database>

<source-appname="EndNote"version="16.0">EndNote</source-app>

<rec-number>26</rec-number>



<foreign-keys>


<keyapp="EN"db-id="tzx50pvro9x5etevw0opsftqp9x">26</key>


</foreign-keys>



<ref-typename="Journal Article">17</ref-type>



<contributors>




<authors>




<author>


<styleface="normal"font="default"charset="134"size="100%">作者名</style>


</author>


<author>


<style face="normal" font="default" charset="134" size="100%">作者名</style>


</author>





</authors>




</contributors>





<titles>




<title>


<styleface="normal"font="default"size="100%">文献名</style>


</title>





<secondary-title>


<styleface="normal"font="default"charset="134"size="100%">期刊名</style>


</secondary-title>




</titles>





<periodical>




<full-title>


<styleface="normal"font="default"size="100%">期刊名</style>


</full-title>




</periodical>





<pages>


<styleface="normal"font="default"size="100%">页码（例如：360-364）</style>


</pages>





<volume>


<styleface="normal"font="default"size="100%">卷号（例如：31） </style>


</volume>





<number>


<styleface="normal"font="default"size="100%">期号（例如：2）</style>


</number>





<dates>




<year>


<styleface="normal"font="default"size="100%">年（例如：2010）</style>


</year>




</dates>





<urls>




<pdf-urls>




<url>

EndNote的保存路径（例如：internal-pdf://foo-1046799104/foo.pdf）

</url>




</pdf-urls>




</urls>




</record>




</records>




</xml>














