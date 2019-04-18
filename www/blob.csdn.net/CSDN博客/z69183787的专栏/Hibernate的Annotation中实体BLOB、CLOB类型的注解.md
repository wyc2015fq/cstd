# Hibernate的Annotation中实体BLOB、CLOB类型的注解 - z69183787的专栏 - CSDN博客
2013年04月03日 10:51:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11528
在Hibernate Annotation中，实体BLOB、CLOB类型的注解与普通的实体属性有些不同，具体操作如下： 
BLOB类型的属性声明为byte[]或者java.sql.Blob： 
@Lob 
@Basic(fetch=FetchType.LAZY) 
@Column(name="IMGS", columnDefinition="BLOB", nullable=true)
private byte[] imgs; 
public byte[] getImgs() { 
   return this.imgs; 
} 
public void setImgs(byte[] imgs) { 
   this.imgs= imgs; 
} 
//下面的例子就是用的java.sql.Blob
@Lob 
@Basic(fetch=FetchType.LAZY) 
@Column(name="IMGS", columnDefinition="BLOB", nullable=true)
private Blob imgs; 
public Blob getImgs() { 
   return this.imgs; 
} 
public void setImgs(Blob imgs) { 
   this.imgs= imgs; 
}
CLOB类型的属性声明为String或java.sql.Clob即可： 
@Lob 
@Basic(fetch = FetchType.EAGER) 
@Column(name="REMARK", columnDefinition="CLOB", nullable=true)
private String remark; 
public String getRemark() { 
   return this.remark; 
} 
public void setRemark(String remark) { 
   this.remark = remark; 
}
@Lob 
@Basic(fetch = FetchType.EAGER) 
@Column(name="REMARK", columnDefinition="CLOB", nullable=true)
private Clob remark; 
public Clob getRemark() { 
   return this.remark; 
} 
public void setRemark(Clob remark) { 
   this.remark = remark; 
}
【注】：在Oracle数据库中byte[]和String存储的是二进制流文件，而Blob和Clob存储的是原文件，如果Clob字
        符串中出现中文乱码情况，请检查数据库的编码是否支持中文。
上面是Oracle和DB2数据库的写法，如果是MySQL数据库写法如下：
@Lob 
@Basic(fetch = FetchType.EAGER) 
@Column(name="REMARK", columnDefinition="TEXT", nullable=true)
private String remark; 
public String getRemark() { 
   return this.remark; 
} 
public void setRemark(String remark) { 
   this.remark = remark; 
}
--------------------------------------------------------------------------------------------------
也可直接把字段属性直接定义成Blob类型。
例如插入操作：
receiptSample_edit.jsp文件的部分代码片段：
<form method="post" name="theForm" id="theForm" action="" enctype="multipart/form-data">
    <td height="20" align="right">请选择图片：</td>
    <td align="left">
<input type="file" name="filename" id="filename"
 class="content"/>
    </td>
</form>
receiptSample_edit.js文件的保存方法部分代码片段：
var imageName = $("#filename").val();
var img = imageName.lastIndexOf(".");
var imgNa = imageName.substring(img);
var imgfrom = imgNa.toLowerCase();
if (imageName != "") {
    if (imgfrom != ".gif" && imgfrom != ".jpg" && imgfrom != ".bmp") {
        alert("不支持的图片类型" + imgNa);
        return;
     }
}
$("#theForm").attr("action", "editReceiptSample.do");
$("#theForm").submit();    //注意是submit提交，不是ajax提交的
ReceiptSampleCtrl.java文件中的方法：
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.multipart.MultipartHttpServletRequest;
@RequestMapping(value = "editReceiptSample.do")
private String editReceiptSample(HttpServletRequest request, ReceiptSampleVO rsVo) throws Exception {
   Map parmMap = getParmMap(request);
MultipartHttpServletRequest multipartRequest = (MultipartHttpServletRequest)
 request;
MultipartFile file = multipartRequest.getFile("filename");
   receiptSampleService.saveEditReceipt(rsVo, getGfmisAllUserId(request), parmMap, file);
   return "receiptsystem/receipttypemanage/receiptSample_list";
}
ReceiptSampleServiceImpl.java文件中的方法：
public void saveEditReceipt(ReceiptSampleVO rsVO, String userId, Map map, MultipartFile file) throws IOException{
   ReceiptSample rs = receiptSampleDao.findUnique("select rs from ReceiptSample rs where rs.receiptSampleId = ?", new Object[]{Long.parseLong(receiptSampleId)});
   if (file.getSize() != 0) {
       rs.setImgContent(Hibernate.createBlob(file.getInputStream()));
       rs.setImgUploadTime(new Date());
       rs.setImgUploadUserId(Long.parseLong(userId));
   }
   receiptSampleDao.save(rs);
}
例如读取操作：
@Entity
@Table(name = "R_RECEIPT_SAMPLE", schema = "GFMIS_ALL")
public class ReceiptSample implements Serializable{
   @Id
    @Column(name = "RECEIPT_SAMPLE_ID")
    @SequenceGenerator(name = "RECEIPT_SAMPLE_ID_GENERATOR", sequenceName = "GFMIS_ALL.SEQ_RECEIPT_SAMPLE_ID", allocationSize = 1)
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "RECEIPT_SAMPLE_ID_GENERATOR")
    private Long receiptSampleId;
    @Column(name = "IMG_CONTENT")
    private Blob imgContent;
    public Blob getImgContent() {
        return imgContent;
    }
    public void setImgContent(Blob imgContent) {
        this.imgContent = imgContent;
    }
}
ReceiptSampleServiceImpl.java文件中的方法：
@Override
public Blob showImg(Long imageId, HttpServletResponse rep) throws Exception{
   Blob imgCount;
   imgCount = receiptSampleDao.get(imageId).getImgContent();
   OutputStream output = null;
   InputStream input = null;
   try {
       final String CONTENT_TYPE = "plain-text/txt;charset=" + "GB2312";
       rep.reset();
       rep.setContentType(CONTENT_TYPE);
       BusinessException.throwWhenNull(imgCount,"该票据类型默认版本没有票样图片信息");
       input = imgCount.getBinaryStream();
//     rep.setHeader("Content-Disposition", "attachment;filename=" + filename + "");
//     bos1.close();
       output = rep.getOutputStream();
       byte data[] = new byte[4096];
       int size = input.read(data);
       while (size != -1) {
            output.write(data, 0, size);
            size = input.read(data);
       }
       output.flush();
    }finally {
        try {
             if (input != null)
                  input.close();
             if (output != null)
                  output.close();
        } catch (IOException e) {
             e.printStackTrace();
        }
    }
    return imgCount;
}
ReceiptSampleCtrl.java文件中的方法：
@RequestMapping(value = "viewImg.do")
public String viewImg(HttpServletRequest request, HttpServletResponse rep) throws Exception {
   Map parmMap = getParmMap(request);
   String imgName = (String) parmMap.get("imageId");
   Blob img = receiptSampleService.showImg(Long.parseLong(imgName), rep);
   request.setAttribute("img", img);
   return "receiptsystem/receipttypemanage/ReceiptSampleImg_show";
}
ReceiptSampleImg_show.jsp文件中的内容：
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page isELIgnored="false" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title></title>
    <%@include file="/receiptcommon/commonHead.jsp" %>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <meta http-equiv="Expires" CONTENT="0"/>
    <meta http-equiv="Pragma" CONTENT="no-cache"/>
</head>
<body>
<table>
    <tr>
        <td>
            <c:choose>
                <c:when test="${img != null}">
                    ${img}
                </c:when>
                <c:otherwise>
                    没有图片
                </c:otherwise>
            </c:choose>
        </td>
    </tr>
</table>
</body>
</html>
