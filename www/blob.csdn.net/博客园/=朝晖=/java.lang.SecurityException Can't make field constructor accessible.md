# java.lang.SecurityException: Can't make field constructor accessible - =朝晖= - 博客园
# [java.lang.SecurityException: Can't make field constructor accessible](https://www.cnblogs.com/dhcn/p/7125101.html)
      原本使用GsonConvertor，在[Android](http://lib.csdn.net/base/android)版本5.1.1上没有任何问题，结果切换到版本6.0.1上，出现以下所示问题：
- java.lang.IllegalArgumentException: Unable to create converter for java.util.List<model>  
，StackOverFlow有个帖子谈这个问题http://stackoverflow.com/questions/34315499/unable-to-create-converter-for-[Java](http://lib.csdn.net/base/java)-util-list-retrofit-2-0-0-beta2 。但是我本身的代码就如答案，所以那个答案没用，看了看Retrofit文档，把Convertor的库由Gson改为Jackson,解决了这个问题,但是JackSon有缺憾，它对JSON对象的接口字段定义要求比较严格，如果Json里面有对象没定义的字段就会Error，这非常限制接口的版本升级，没有办法倒回来继续解决Gson的问题。Gson的问题往下看，根本错误原因是：
- java.lang.SecurityException: Can't make field constructor accessible  
     原来这是一个Android M Preview版的问题，Gson官方也已经承认，加上我还用了ActiveAndroid，解决办法：
- GsonBuilder builder = new GsonBuilder();  
- builder.excludeFieldsWithModifiers(Modifier.FINAL, Modifier.TRANSIENT, Modifier.STATIC);  
- Gson gson = builder.create();  
- 
- 
- retrofit = new Retrofit.Builder()  
-         .baseUrl(baseURL)  
-         .addConverterFactory(GsonConverterFactory.create(gson))  
-         .client(client)  
-         .build();  
