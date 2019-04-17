# Struts2学习笔记之文件上传 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月23日 22:09:13[boonya](https://me.csdn.net/boonya)阅读数：812
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)









Struts2学习笔记

第九记：文件上传


（1）、单个文件上传

     1-1：实现步骤：

         1)、在WEB-INF下的lib目录中添加commons fileupload 1.2.1.jar和commons Io 1.3.2.jar

             文件下载路径：http://commons.apache.org/

         2)、在文件上传的Action中添加上传文件的字段

             如：private File upfile;

                    private String fileName;.....

         3）、编写上传的方法

           public  String upload() throws Exception{

                String realpath=ServletActionContext.getServletContext().getRealPath("/images");

                File file=new File(realpath);

                if(!file.exists())file.mkdirs();

               FileUtils.copyFile(file, new File("/upload/images"));

               return "SUCCESS";


         }

      1-2：编写上传表单

          <form  enctype="multipart/form-data"  method="post" action="file_upload">

                   <input type="file"  name="upfile"/ ><input type="text"  name="fileName"/ ><input type="submit" value="上传"/>


          </form>

      1-3：编写Action代码

          public class FileUploadAction{

                    private File upfile;

                    private String fileName;

                    public  void  setUpfle (File upfile){

                          this.upfile=upfile;


                    }

                    public File  getUpfile(){

                          return this.upfile;


                   }

                     public  void  setFileName(String fileName){

                          this.fileName=fileName;


                    }

                    public String getFileName(){

                          return this.fileName;


                   }

                   public  String execute() throws Exception{

                            String realpath=ServletActionContext.getServletContext().getRealPath("/images");

                             System.out.println(realpath);//查看文件的真实路径

                             if(upfile!=null){

                                    File  savefile=new File(new File(realpath),fileName);

                                    if(!savefile.getParentFile().exists())savefile.getParentFile().mkdirs();

                                   FileUtils.copyFile(upfile, savefile);


                             }


                          return "SUCCESS";


                  }

           }

      1-4：设置文件上传大小

             在struts.xml 中配置常量参数，避免文件过大上传失败。

            <constant  name="Struts.multipart.maxSize"   value="10701096"/>//设置最大上传为10MB

（2）、多个文件上传

        2-1：编写多个上传文件选择

     <form  enctype="multipart/form-data"  method="post" action="file_upload">

       文件1：  <input type="file"  name="upfile"/ ><input type="text"  name="fileName"/ ><input type="submit" value="上传"/><br/>

       文件2：  <input type="file"  name="upfile"/ ><input type="text"  name="fileName"/ ><input type="submit" value="上传"/><br/>

       文件3：  <input type="file"  name="upfile"/ ><input type="text"  name="fileName"/ ><input type="submit" value="上传"/><br/>


      </form>

          注：字段名称一致，组合为数组类型提交到后台。

        2-2：将属性字段设置为数组类型

        public class FileUploadAction{

                    private File upfile [];

                    private String fileName [];

                    public  void  setUpfle (File [] upfile){

                          this.upfile=upfile;


                    }

                    public File  getUpfile(){

                          return this.upfile;


                   }

                     public  void  setFileName(String [] fileName){

                          this.fileName=fileName;


                    }

                    public String getFileName(){

                          return this.fileName;


                   }

                   public  String execute() throws Exception{

                            String realpath=ServletActionContext.getServletContext().getRealPath("/images");

                             System.out.println(realpath);//查看文件的真实路径

                             if(upfile!=null){

                                   File  savedir=new File(realpath);

                                   if(!savedir.exists())savedir.mkdirs();


                                   for(int i=0;i<upfile.length;i++){//循环保存文件

                                          File  savefile=new File(savedir,fileName[i]);

                                          FileUtils.copyFile(upfile[i], savefile);


                                  }


                             }


                          return "SUCCESS";


                  }

           }





