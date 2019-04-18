# spring mvc做上传图片，文件小于10k就不生成临时文件了 - z69183787的专栏 - CSDN博客
2016年09月14日 12:05:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1382
这是spring-mvc.xml中的
```
<bean id="multipartResolver"
    class="org.springframework.web.multipart.commons.CommonsMultipartRe     solver">
    <property name="defaultEncoding" value="UTF-8" />
    <!-- 指定所上传文件的总大小不能超过2000KB。注意maxUploadSize属性的限制     不是针对单个文件，而是所有文件的容量之和 -->
    <property name="maxUploadSize" value="2048000" />
    <property name="maxInMemorySize" value="10240" />  
</bean>
```
下面是方法：
```
@RequestMapping(value="uploadFile.do",method={RequestMethod.GET,RequestMethod.POST})
    public String ajaxUpload(ModelMap mm,HttpSession ses,HttpServletRequest req
            ) throws Exception {
        try {
        MultipartHttpServletRequest multipartRequest = (MultipartHttpServletRequest) req;     
            // 获得文件：
                MultipartFile upload = multipartRequest.getFile("upload");  
            String newFileName = new Date().getTime() + "_" + SesHelper.getSimUser(ses).getManageUser().getMuId()
                    + getExtention(upload.getOriginalFilename());
            CommonsMultipartFile cf= (CommonsMultipartFile)upload;
                DiskFileItem fi = (DiskFileItem)cf.getFileItem();
                File targetFile = fi.getStoreLocation();
            if(ArrayUtils.indexOf(imageExtentions, getExtention(upload.getOriginalFilename()).substring(1))!=-1){
                **RemoteFileUtil.copyImage(targetFile, newFileName);**
            }else{
                throw new BaseException("请确定是图片文件");
            }
            handleSuc(mm);
            mm.put("msg", imgServerName+newFileName);
        } catch (Exception e) {
            handleErr(mm , e);
        }
        return JSON_COM;
        }
```
上面的调这个方法
```
public static synchronized void copyImage(File src, String filename) {
        try {
            **copy(src, filename);**
        } catch (Exception e) {
            e.printStackTrace();
            throw new RuntimeException("请检查服务器：" + serverIps[0] + ":" + serverPorts[0] + "是否连接通畅");
        }
    }
```
上面的调这个方法
```
private static synchronized void copy(File src, String filename)
            throws UnknownHostException, IOException {
        for (int i = 0; i < serverIps.length; i++) {
            Socket client = new Socket(serverIps[i], Integer
                    .parseInt(serverPorts[i]));
            OutputStream os = client.getOutputStream();
            os.write((filename + ";").getBytes());
            byte[] byteBuffer = new byte[1];
            FileInputStream fos = new FileInputStream(src);
            BufferedInputStream bis = new BufferedInputStream(fos);
            System.out.println(filename);
            **while (bis.read(byteBuffer) != -1) {
                os.write(byteBuffer);
            }**
            os.flush();
            bis.close();
            fos.close();
            os.close();
            client.close();
            System.out.println(client.getLocalAddress());
        }
    }
```
```
下面是页面
```
```
<script type="text/javascript">
    axUpload=true;
    var fileName = "";
    function ajaxuploadsubmit() {
        var index = layer.load();
        $('#uploadForm').ajaxSubmit({
            url:'${siteUrl}/file/uploadFile.do',
            type:'post',
            dataType: 'json',
            success:function (data){
                if(data.code == "S"){
                    layer.close(index);
                    layer.msg("文件上传成功,上传文件的文件名为"+data.msg, {icon: 1});
                    fileName = data.msg;
                    closeAndOpenSuc();
                }else{
                    layer.close(index);
                    layer.msg(data.msg, {icon: 2});
                }
            }
        });
    }
    function closeAndOpenSuc() {
        layer.close(uploadObj.imgLayerId);
        $('#'+uploadObj.imgInputId).val(fileName);
        $('#'+uploadObj.imgShowId).attr("src",fileName);
    }
    function closeAndOpen() {
        layer.close(uploadObj.imgLayerId);
    }
</script>
<div class="modal-content">
    <div class="modal-body">
        <form id="uploadForm" class="form-horizontal" role="form" enctype="multipart/form-data">
        <div class="padd">
        <div class="form quick-post">
            <div class="form-group">
                <label class="control-label col-lg-2" style="width:100px;padding-top:5px;">选择文件</label>
                <div class="col-lg-6">
                    <input id="upload" type="file" size="30" height="20px" name="upload">
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-lg-2" style="width:100px;padding-top:5px;"> </label>
                <div class="col-lg-6">
                    <input type="button" name="button" value="上传文件" class="btn btn-success" onclick="ajaxuploadsubmit()" />
                </div>
            </div>      
        </div>
        </div>
        </form>
    </div>
    <div class="modal-footer">
        <button type="button" class="btn btn-default" onclick="closeAndOpen()">关闭</button>
    </div>
</div>
```
报错信息
while (bis.read(byteBuffer) != -1) {
os.write(byteBuffer);
}
中会报错
java.io.FileNotFoundException: C:\apache-tomcat-7.0.61\work\Catalina\localhost\FishManage\upload_de2e4204_56e7_4b34_adce_163e38e29654_00000003.tmp (系统找不到指定的文件。)
