# android上传图片至服务器 - 零度的博客专栏 - CSDN博客
2016年05月20日 14:50:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：388
本实例实现了android上传手机图片至服务器，服务器进行保存。
```
1、服务器servlet代码[]()
 
```
publicvoid doPost(HttpServletRequest request, HttpServletResponse response)  
           throws ServletException, IOException {  
            
           String temp=request.getSession().getServletContext().getRealPath("/")+"temp";   //临时目录
           System.out.println("temp="+temp);
           String loadpath=request.getSession().getServletContext().getRealPath("/")+"Image"; //上传文件存放目录
           System.out.println("loadpath="+loadpath);
           DiskFileUpload fu =new DiskFileUpload();
           fu.setSizeMax(1*1024*1024);   // 设置允许用户上传文件大小,单位:字节
           fu.setSizeThreshold(4096);   // 设置最多只允许在内存中存储的数据,单位:字节
           fu.setRepositoryPath(temp); // 设置一旦文件大小超过getSizeThreshold()的值时数据存放在硬盘的目录
          
           //开始读取上传信息
int index=0;
           List fileItems =null;
                
                        
                                try {
                                        fileItems = fu.parseRequest(request);
                                         System.out.println("fileItems="+fileItems);
                                } catch (Exception e) {
                                        e.printStackTrace();
                                }
                        
                
           Iterator iter = fileItems.iterator(); // 依次处理每个上传的文件
while (iter.hasNext())
           {
               FileItem item = (FileItem)iter.next();// 忽略其他不是文件域的所有表单信息
if (!item.isFormField())
               {
                   String name = item.getName();//获取上传文件名,包括路径
                   name=name.substring(name.lastIndexOf("\\")+1);//从全路径中提取文件名
long size = item.getSize();
                   if((name==null||name.equals("")) && size==0)
                         continue;
                   int point = name.indexOf(".");
                   name=(new Date()).getTime()+name.substring(point,name.length())+index;
                   index++;
                   File fNew=new File(loadpath, name);
                   try {
                                        item.write(fNew);
                                } catch (Exception e) {
                                        // TODO Auto-generated catch block
                                        e.printStackTrace();
                                }
                  
                  
               }
               else//取出不是文件域的所有表单信息
               {
                   String fieldvalue = item.getString();
            //如果包含中文应写为：(转为UTF-8编码)
                   //String fieldvalue = new String(item.getString().getBytes(),"UTF-8");
               }
           }
           String text1="11";
           response.sendRedirect("result.jsp?text1="+ text1);
    }
```
[]()
```
2、android客户端代码
```
```
publicclass PhotoUpload extends Activity {
    private String newName ="image.jpg";
    private String uploadFile ="/sdcard/image.JPG";
    private String actionUrl ="http://192.168.0.71:8086/HelloWord/myForm";
    private TextView mText1;
    private TextView mText2;
    private Button mButton;
    @Override
      publicvoid onCreate(Bundle savedInstanceState)
      {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.photo_upload);
        mText1 = (TextView) findViewById(R.id.myText2);
        //"文件路径：\n"+
        mText1.setText(uploadFile);
        mText2 = (TextView) findViewById(R.id.myText3);
        //"上传网址：\n"+
        mText2.setText(actionUrl);
        /* 设置mButton的onClick事件处理 */    
        mButton = (Button) findViewById(R.id.myButton);
        mButton.setOnClickListener(new View.OnClickListener()
        {
          publicvoid onClick(View v)
          {
            uploadFile();
          }
        });
      }
      /* 上传文件至Server的方法 */
      privatevoid uploadFile()
      {
        String end ="\r\n";
        String twoHyphens ="--";
        String boundary ="*****";
        try
        {
          URL url =new URL(actionUrl);
          HttpURLConnection con=(HttpURLConnection)url.openConnection();
          /* 允许Input、Output，不使用Cache */
          con.setDoInput(true);
          con.setDoOutput(true);
          con.setUseCaches(false);
          /* 设置传送的method=POST */
          con.setRequestMethod("POST");
          /* setRequestProperty */
          con.setRequestProperty("Connection", "Keep-Alive");
          con.setRequestProperty("Charset", "UTF-8");
          con.setRequestProperty("Content-Type",
                             "multipart/form-data;boundary="+boundary);
          /* 设置DataOutputStream */
          DataOutputStream ds =
            new DataOutputStream(con.getOutputStream());
          ds.writeBytes(twoHyphens + boundary + end);
          ds.writeBytes("Content-Disposition: form-data; "+
                        "name=\"file1\";filename=\""+
                        newName +"\""+ end);
          ds.writeBytes(end);  
          /* 取得文件的FileInputStream */
          FileInputStream fStream =new FileInputStream(uploadFile);
          /* 设置每次写入1024bytes */
          int bufferSize =1024;
          byte[] buffer =newbyte[bufferSize];
          int length =-1;
          /* 从文件读取数据至缓冲区 */
          while((length = fStream.read(buffer)) !=-1)
          {
            /* 将资料写入DataOutputStream中 */
            ds.write(buffer, 0, length);
          }
          ds.writeBytes(end);
          ds.writeBytes(twoHyphens + boundary + twoHyphens + end);
          /* close streams */
          fStream.close();
          ds.flush();
          /* 取得Response内容 */
          InputStream is = con.getInputStream();
          int ch;
          StringBuffer b =new StringBuffer();
          while( ( ch = is.read() ) !=-1 )
          {
            b.append( (char)ch );
          }
          /* 将Response显示于Dialog */
          showDialog("上传成功"+b.toString().trim());
          /* 关闭DataOutputStream */
          ds.close();
        }
        catch(Exception e)
        {
          showDialog("上传失败"+e);
        }
      }
      /* 显示Dialog的method */
      privatevoid showDialog(String mess)
      {
        new AlertDialog.Builder(PhotoUpload.this).setTitle("Message")
         .setMessage(mess)
         .setNegativeButton("确定",new DialogInterface.OnClickListener()
         {
           publicvoid onClick(DialogInterface dialog, int which)
           {          
           }
         })
         .show();
      }
    }
```
[]()
```
