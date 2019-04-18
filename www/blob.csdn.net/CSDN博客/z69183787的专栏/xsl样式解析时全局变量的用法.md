# xsl样式解析时全局变量的用法 - z69183787的专栏 - CSDN博客
2014年02月20日 16:32:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：964
1.首先记住 variable这个变量在xsl中只得赋值一次,以后不得再改变了..呵呵.这样怎么称得变量了,不知道draft的专家们是吃什么饭的.
2.所谓全局,因为xsl:variable元素是xsl:stylesheet的子无素,所以在stylesheet 中定义的变量可以在整个实例空间中引用.
<xsl:stylesheet version="1.0"  xmlns:xsl="[http://www.w3.org/1999/XSL/Transform](http://www.w3.org/1999/XSL/Transform)" >
<xsl:param name="viewtype" select="'view'"/>
 <xsl:variable name="URLVariable">
 <xsl:choose>
  <xsl:when test="contains($viewtype,'edit')">  
    <xsl:text>/flowEngine/page1.wml</xsl:text>
  </xsl:when>
   <xsl:when test="contains($viewtype,'view')">  
   <xsl:text>/flowEngine/page2.wml</xsl:text>
  </xsl:when>
   <xsl:otherwise>
   <xsl:text>/flowEngine/error.wml</xsl:text>   
   </xsl:otherwise>
 </xsl:choose>
</xsl:variable>
<xsl:template match="/data">
 <xsl:value-of select ="$URLVariable"/>
 <a href="{$URLVariable}"  > myURL</a>
 ...........在实例空间中都有效
</xsl:template>
</xsl:stylesheet >
3.引用方法
 1.$vairableName的方法
  对于<xsl:value-of select ="$vairableName"/>之类以node对象为值的元素,请使用之.
  但<xsl:text>$hyperURLVariable</xsl:text>是无效的.
 2.{$vairableName}的方法
  对于一些以非node对象为值的引用,请使用{$vairableName},两边的大括号不能少.
  如 <a href="{$hyperURLVariable}"  > myURL</a>
  但<xsl:text>{$hyperURLVariable}</xsl:text>也是无效的.
4.全局变量的错误使用方法
 定义的全局变量
 <xsl:stylesheet version="1.0"  xmlns:xsl="[http://www.w3.org/1999/XSL/Transform](http://www.w3.org/1999/XSL/Transform)" >
 <xsl:param name="viewtype" select="'view'"/>
  <xsl:variable name="URLVariable"/> <!---在这里定义一个空值--> 
  <xsl:template match="/data">
  <xsl:choose>
   <xsl:variable  name="URLVariable">
    <xsl:when test="contains($viewtype,'edit')">  
     <xsl:text>/flowEngine/page1.wml</xsl:text>
    </xsl:when>
   </xsl:variable>
   <xsl:variable  name="URLVariable">
    <xsl:when test="contains($viewtype,'view')">  
     <xsl:text>/flowEngine/page2.wml</xsl:text>
    </xsl:when>
   </xsl:variable>
   <xsl:variable  name="URLVariable">
     <xsl:otherwise>
     <xsl:text>/flowEngine/error.wml</xsl:text>   
     </xsl:otherwise>
    </xsl:variable>
    <!---一般人会认为这是全局变量赋值,错了,其实是在一个更小的代码空间了重新定义一个局部变量)
  </xsl:choose>  
  <xsl:value-of select ="$URLVariable"/>   <
  <a href="{$URLVariable}"  > myURL</a>
  <!----上面的全局变量的值仍是空的,你根本不然引用到xsl:choose中的URLVariable的值,这个错误
  ,浪费我三个小时去获得它---->
  ...........全局变量在整个实例空间中都有效,只是我们没有正确赋值.
 </xsl:template>
 </xsl:stylesheet >
 希望看了这些,对你有些帮助!
