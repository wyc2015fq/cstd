# 高仿IoC第三课：对Spring第三课效果的实现 - PeterBishop - CSDN博客





2019年02月10日 20:56:39[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：53
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这课我们来实现Spring第三课所讲的东西



  首先实现构造器注入

XmlParser.java
package com.ioc;



import com.ioc.util.FileUtil;

import org.dom4j.Document;

import org.dom4j.DocumentException;

import org.dom4j.Element;

import org.dom4j.io.SAXReader;



import java.io.File;

import java.lang.reflect.Constructor;

import java.lang.reflect.InvocationTargetException;

import java.util.ArrayList;

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

        Object bean = null;



        //这里开始实现依赖注入的部分

        //先写构造器注入

        bean = constructorInject(beanElement);



        //这里开始实现setter注入的部分

        //留做作业

        setterInject(bean,beanElement);



        //构造BeanDefinition对象

        BeanDefinition beanDefinition = new BeanDefinition(name,beanClass,bean);



        //注册BeanDefinition对象

        registry.regist(name,beanDefinition);

    }



    //setter注入

    private void setterInject(Object bean, Element beanElement)

    {

        //作业部分

    }



    //构造器注入

    private Object constructorInject(Element beanElement) throws ClassNotFoundException, IllegalAccessException, InstantiationException

    {

        Object bean = null;

        Class<?> beanClass = Class.*forName*(beanElement.attributeValue("class"));

        List<Element> constuctorArgElements = beanElement.elements("constructor-arg");

        if (constuctorArgElements != null && !constuctorArgElements.isEmpty())

        {

            //有constructor-arg标签

            //获取构造器参数表

            Object[] constructorArgs = parseConstructorArgs(constuctorArgElements);



            //获取对应的构造器

            Constructor<?>[] constructors = beanClass.getConstructors();

            for (Constructor<?> constructor : constructors)

            {

                if (constructor.getParameterCount() == constructorArgs.length)

                {

                    try {

                        //使用这个构造器来构造对象

                        //这里其实有个问题，就是constructorArgs里的元素全是String类型的，这里需要转换数据类型

                        Class<?>[] parameterTypes = constructor.getParameterTypes();

                        for (int i = 0; i < parameterTypes.length; i++)

                        {

                            //转换到合适的类型

                            constructorArgs[i] = castToSuitType(constructorArgs[i],parameterTypes[i]);

                        }

                        bean = constructor.newInstance(constructorArgs);

                    } catch (InvocationTargetException e) {

                        //这个构造器参数表类型不太对

                        continue;

                    }

                    break;

                }

            }

            if (bean == null)

            {

                throw new RuntimeException("找不到对应的构造器,无法构造对应对象");

            }

        }

        else

        {

            //这是没有constructor-arg标签的情况

            //构造bean对象

            bean = beanClass.newInstance();

        }

        return bean;

    }



    private Object castToSuitType(Object constructorArg, Class<?> parameterType)

    {

        //这个方法很枯燥，我就不带着写了，直接把我以前写过的拿过来算了

        String arg = constructorArg.toString();

        Object value;

        if (parameterType == String.class)

        {

            value = arg;

        }

        else if(parameterType == Integer.class || parameterType == int.class)

        {

            value = Integer.*parseInt*(arg);

        }

        else if (parameterType == Float.class || parameterType == float.class)

        {

            value = Float.*parseFloat*(arg);

        }

        else if (parameterType == Double.class || parameterType == double.class)

        {

            value = Double.*parseDouble*(arg);

        }

        else if (parameterType == Short.class || parameterType == short.class)

        {

            value = Short.*parseShort*(arg);

        }

        else

        {

            //这里之所以写constructorArg而不是arg就是因为怕这个参数类型是非字符串可以转换到的类型

            value = constructorArg;

        }

        return value;

    }



    //解析多个constructor-arg标签

    private Object[] parseConstructorArgs(List<Element> constructorArgElements)

    {

        List<Object> constructorArgs = new ArrayList<>(constructorArgElements.size());

        for (Element constructorArgElement : constructorArgElements)

        {

                constructorArgs.add(parseConstructorArg(constructorArgElement));

        }

        return constructorArgs.toArray();

    }



    //解析单个constructor-arg标签

    private Object parseConstructorArg(Element constructorArgElement)

    {

        /*

            这里其实还有个问题

            构造器里的参数可能不是基本数据类型或包装类型或String，而是其他类比如自定义的class，这里 我们说过用ref

         */

        String value = constructorArgElement.attributeValue("value");

        if (value == null || "".equals(value))

        {

            Object ref = registry.get(constructorArgElement.attributeValue("ref"));

            return ref;

        }

        return value;

    }

}


作业: 实现setter注入



