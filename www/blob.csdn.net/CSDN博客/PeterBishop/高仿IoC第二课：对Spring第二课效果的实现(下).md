# 高仿IoC第二课：对Spring第二课效果的实现(下) - PeterBishop - CSDN博客





2019年02月06日 12:03:24[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：33
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们接着上节课的讲。



  这节课也是最后一部分，就是对ClassPathXmlApplicationContext类的构造器的实现



  XmlParser.java
package com.ioc;



import com.ioc.util.FileUtil;

import org.dom4j.Document;

import org.dom4j.DocumentException;

import org.dom4j.Element;

import org.dom4j.io.SAXReader;



import java.io.File;

import java.util.List;



//这个类负责解析xml

public class XmlParser {



    BeanDefinitionRegistry registry;

    Document document;



    public XmlParser(BeanDefinitionRegistry registry,String xmlPath)

    {

        this.registry = registry;

        xmlPath = FileUtil.*resolveClassPath*(xmlPath);

        File file = new File(xmlPath);

        if (file.exists())

        {

            SAXReader reader = new SAXReader();

            try {

                document = reader.read(file);

            } catch (DocumentException e) {

                e.printStackTrace();

            }

        }

    }



    */**     * **解析xml，将解析到的bean通过BeanDefinitionRegistry注册到IoC容器中     */    *public void parse() throws ClassNotFoundException, IllegalAccessException, InstantiationException {

        //我们以xml标签为单位逐步解析

        Element rootElement = document.getRootElement();

        if (rootElement.getName().equals("beans"))

        {

            parseBeans(rootElement);

        }



    }



    //解析beans标签

    private void parseBeans(Element beansElement) throws ClassNotFoundException, InstantiationException, IllegalAccessException {

        List<Element> bean = beansElement.elements("bean");

        for (Element beanElement : bean)

        {

            parseBean(beanElement);

        }

    }



    //解析bean标签

    private void parseBean(Element beanElement) throws ClassNotFoundException, IllegalAccessException, InstantiationException {

        //<bean id="student" class="resources.bean.Student"></bean>

        //我们要得到id和class

        String name = beanElement.attributeValue("id");

        String className = beanElement.attributeValue("class");

        Class<?> beanClass = Class.*forName*(className);



        //构造bean对象

        Object bean = beanClass.newInstance();



        //构造BeanDefinition对象

        BeanDefinition beanDefinition = new BeanDefinition(name,beanClass,bean);



        //注册BeanDefinition对象

        registry.regist(name,beanDefinition);

    }

}


ClassPathXmlApplicationContext的构造器
public ClassPathXmlApplicationContext(String filepath)

{

    */**     * **这个构造器要做的事就是构造一个IoC容器     * 底层容器是一个Map对象，已经被封装在BeanDefinitionRegistry里了     * 下面我们要做的就是解析xml文件，把里面配置的bean都加入到Map对象中     */    *XmlParser parser = new XmlParser(registry,filepath);

    try {

        parser.parse();

    } catch (ClassNotFoundException e) {

        e.printStackTrace();

    } catch (IllegalAccessException e) {

        e.printStackTrace();

    } catch (InstantiationException e) {

        e.printStackTrace();

    }

}


FileUtil.java
package com.ioc.util;



import java.io.File;

import java.net.URL;



public class FileUtil {



    */**     *     * **@param ****relativePath **相对类路径的相对路径     * **@return **绝对路径     */    *public static String resolveClassPath(String relativePath)

    {

        URL resource = FileUtil.class.getClassLoader().getResource("");

        String classPath = resource.toString();

        classPath = classPath.substring(("file:" + File.*separator*).length());



        //这样做可以对任意操作系统都起作用，因为File.separator会根据操作系统不同而不同

        //正则表达式以 '/' 开头, 以'/' 结尾

        relativePath = relativePath.replaceAll("/\\//",File.*separator*);

        return classPath + relativePath;

    }



}


测试代码:
package test;



import com.ioc.ApplicationContext;

import com.ioc.ClassPathXmlApplicationContext;

import resources.bean.Student;



public class MyTest {



    public static void main(String[] args)

    {

        //来试试直接粘贴Spring第二课的测试代码

        //然后删掉多余注释



        //别忘了改路径

        ApplicationContext context = new ClassPathXmlApplicationContext("resources/bean.xml");



        //根据name

        Student student = (Student) context.getBean("student");

        System.*out*.println(student.toString());



        //根据class

        student = context.getBean(Student.class);

        System.*out*.println(student.toString());



        //根据name和class

        student = context.getBean("student",Student.class);

        System.*out*.println(student);



        //完美

        //至此，Spring第二课所教内容基本实现

    }





}




