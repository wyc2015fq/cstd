# Google Gson解析Json数据应用实例 - z69183787的专栏 - CSDN博客
2013年07月10日 19:57:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1111
个人分类：[Json转换-Gson](https://blog.csdn.net/z69183787/article/category/2175429)

 /**
     * 带日期类型Bean、Json相互转换
     */
    @Test
**public void** testDateBeanJson() {
       // 日期Bean数据
       DateBean bean = **new** DateBean("1001", "scott", 20, **new** Date());
       // Bean(带日期属性) -> Json
       gson = gsonBuilder.registerTypeAdapter(java.util.Date.**class**, 
**new** DateSerializerUtils()).setDateFormat(DateFormat.*LONG*).create();
       String json = gson.toJson(bean);
       System.*out*.println(json);
       // Json -> Bean(带日期类型属性)
       gson = gsonBuilder.registerTypeAdapter(java.util.Date.**class**, 
**new** DateDeserializerUtils()).setDateFormat(DateFormat.*LONG*).create();
       java.lang.reflect.Type type = 
**new** com.google.gson.reflect.TypeToken<DateBean>(){}.getType();
       DateBean b = gson.fromJson(json, type);
       System.*out*.println(b);
    }
    /**
     * 泛型日期List、Json相互转换
     */
    @Test
**public void** testDateGenericListJson() {
       // 泛型日期List
       List<DateBean> list = **new** ArrayList<DateBean>();
**for**(**int** i = 0; i < 3; i++) {
           DateBean user = **new** DateBean("100" + i, "name" + i, 20 + i, **new** Date());
           list.add(user);
       }
       System.*out*.println(list);
       // 泛型日期List -> Json
       gson = gsonBuilder.registerTypeAdapter(java.util.Date.**class**, 
**new** DateSerializerUtils()).setDateFormat(DateFormat.*LONG*).create();
       java.lang.reflect.Type type = 
**new** com.google.gson.reflect.TypeToken<List<DateBean>>(){}.getType();
       String json = gson.toJson(list, type);
       System.*out*.println(json);   
       // Json -> 泛型日期List
       gson = gsonBuilder.registerTypeAdapter(java.util.Date.**class**, 
**new** DateDeserializerUtils()).setDateFormat(DateFormat.*LONG*).create();
       List<DateBean> users = gson.fromJson(json.toString(), type);
       System.*out*.println(users);
    }
}
