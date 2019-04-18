# java.sql.Clob 转化为String的方法 - z69183787的专栏 - CSDN博客
2014年01月15日 16:22:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6160
 public  String clobToString(Clob clob)   
   {   
       if(clob == null) {   
           return null;   
       }   
       try  
       {   
           Reader inStreamDoc = clob.getCharacterStream();   
           char[] tempDoc = new char[(int) clob.length()];   
           inStreamDoc.read(tempDoc);   
           inStreamDoc.close();   
           return new String(tempDoc);   
       }   
       catch (IOException e)   
       {   
           e.printStackTrace();   
       }   
       catch (SQLException es)   
       {   
           es.printStackTrace();   
         }   
         return null;   
     }
