# 安卓Java读取SD卡文本文件 - wishchinYang的专栏 - CSDN博客
2015年09月17日 21:55:19[wishchin](https://me.csdn.net/wishchin)阅读数：697
       在进行序列识别时，需要对多个模式串进行识别，需要对多行浮点数进行读取，并进行解析。
       所以使用的方法为：
             使用文本多行读取的方式；对每行文本进行正则表达式匹配；再进行字符转换。
代码如下：
```java
@SuppressLint("SdCardPath")
        public static void readTxt2SeqMulti(String txtPath,  Vector<Vector<Float > >  Seq ){  
            try {  
                String encoding="GBK";  
                File file=new File(txtPath);  
                if(file.isFile() && file.exists()){ //判断文件是否存在  
                    InputStreamReader read = new InputStreamReader(  
                            new FileInputStream(file),encoding);//考虑到编码格式  
                    BufferedReader bufferedReader = new BufferedReader(read);  
                    String lineTxt = null;  
                    Seq.clear();
                    
                    while((lineTxt = bufferedReader.readLine()) != null ){  
                        //输入每一行到向量
                        //System.out.println(lineTxt);  
                        String[] sourceStrArray = lineTxt.split(" ");
                        
                        Vector<Float >   SeqS= new Vector<Float >();
                        for (int i = 0; i < sourceStrArray.length; i++) {
                            //System.out.println(sourceStrArray[i]);
                            String SValue = sourceStrArray[i];
                            float Value =Float.parseFloat(SValue );
                            SeqS.add(Value);
                        }
                        Seq.add(SeqS);
                    }  
                    
                    read.close();  
                }else{  
                    System.out.println("找不到指定的文件");  
                }  
            } catch (Exception e) {  
                System.out.println("读取文件内容出错");  
                e.printStackTrace();  
            }  
        }
```
