# 结合springmvc，使用ajax上传base64图片数据 - z69183787的专栏 - CSDN博客
2017年08月02日 16:02:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1288
**注（其他应用）： 使用canvas画布 进行html 中所需的dom元素，进行绘图，使用 toDataUrl转换成base64 编码 上传图片**
一、前端：
`<input type="file" id="myImage" name="myImage"/>  `- 1
- 1
```xml
<script type="text/javascript">  
    $("#myImage").bind("change",function(){
        uploadFile($(this));
    });
    //通过onChange直接获取base64数据
q   function uploadFile(file){
        var f = file.files[0];
        var reader = new FileReader();
        reader.onload = function(){
            var data = e.target.result;
            if (data.lastIndexOf('data:base64') != -1) {
                  data = data.replace('data:base64', 'data:image/jpeg;base64');
            } else if (data.lastIndexOf('data:,') != -1) {
                  data = data.replace('data:,', 'data:image/jpeg;base64,');
            }
            if(isCanvasSupported()){
            }else{
                alert("您的浏览器不支持");
            }
        };
        reader.onerror = function(){
            console.log("上传失败了 ");
        }
        reader.readAsDataURL(f);
    }
    //ajax异步上传
    function ajaxUploadBase64File(base64Data){
        var url = window.location.protocol + '//' + window.location.host + "/register/uploadBase64";
        $.ajax({
            url:url,
            type:"post",
            data:{base64Data:base64Data},
            dataType:"json",
            success:function(data){
                if(data.success == true){
                    console.log("上传成功");
                }else{
                    console.log("上传失败");
                }
            },
            error:function(){
                console.log("上传失败");
            }
        });
    }; 
    //是否支持canvas
    function isCanvasSupported(){
        var elem = document.createElement('canvas');
        return !!(elem.getContext && elem.getContext('2d'));
    };
</script>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
二、后台：
```java
@RequestMapping(value="/uploadBase64",method=RequestMethod.POST)
    @ResponseBody
    public ActionResult<Map<String,String>> base64UpLoad(@RequestParam String base64Data, HttpServletRequest request, HttpServletResponse response){  
        ActionResult<Map<String,String>> result = new ActionResult<Map<String,String>>();
        try{  
            logger.debug("上传文件的数据："+base64Data);
            String dataPrix = "";
            String data = "";
            logger.debug("对数据进行判断");
            if(base64Data == null || "".equals(base64Data)){
                throw new Exception("上传失败，上传图片数据为空");
            }else{
                String [] d = base64Data.split("base64,");
                if(d != null && d.length == 2){
                    dataPrix = d[0];
                    data = d[1];
                }else{
                    throw new Exception("上传失败，数据不合法");
                }
            }
            logger.debug("对数据进行解析，获取文件名和流数据");
            String suffix = "";
            if("data:image/jpeg;".equalsIgnoreCase(dataPrix)){//data:image/jpeg;base64,base64编码的jpeg图片数据
                suffix = ".jpg";
            } else if("data:image/x-icon;".equalsIgnoreCase(dataPrix)){//data:image/x-icon;base64,base64编码的icon图片数据
                suffix = ".ico";
            } else if("data:image/gif;".equalsIgnoreCase(dataPrix)){//data:image/gif;base64,base64编码的gif图片数据
                suffix = ".gif";
            } else if("data:image/png;".equalsIgnoreCase(dataPrix)){//data:image/png;base64,base64编码的png图片数据
                suffix = ".png";
            }else{
                throw new Exception("上传图片格式不合法");
            }
            String tempFileName = getRandomFileName() + suffix;
            logger.debug("生成文件名为："+tempFileName);
            //因为BASE64Decoder的jar问题，此处使用spring框架提供的工具包
            byte[] bs = Base64Utils.decodeFromString(data);
            try{
                //使用apache提供的工具类操作流
                FileUtils.writeByteArrayToFile(new File(Global.getConfig(UPLOAD_FILE_PAHT), tempFileName), bs);  
            }catch(Exception ee){
                throw new Exception("上传失败，写入文件失败，"+ee.getMessage());
            }
            Map<String,String> map =new HashMap<String,String>();   
            map.put("tempFileName", tempFileName);
            result.setResultMessage("上传成功");
            result.setData(map);
            logger.debug("上传成功");
        }catch (Exception e) {  
            logger.debug("上传失败,"+e.getMessage());
            result.setSuccess(false);
            result.setResultMessage("上传失败,"+e.getMessage());  
        }  
        return result;
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
参考： 
[Base64编码与图片互转](http://blog.csdn.net/kouwoo/article/details/44405621)、 
[JAVA 把base64图片数据转为本地图片](http://www.devba.com/index.php/archives/3401.html)、 
[如何把图片转换成base64在后台转换成图片放在本地](http://www.imooc.com/article/14623)
