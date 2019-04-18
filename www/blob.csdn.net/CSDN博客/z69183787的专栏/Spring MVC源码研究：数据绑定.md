# Spring MVC源码研究：数据绑定 - z69183787的专栏 - CSDN博客
2018年07月19日 15:41:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：111
[http://www.cnblogs.com/guangshan/p/4431800.html](http://www.cnblogs.com/guangshan/p/4431800.html)
在做Spring MVC时，我们只需用@Controllor来标记Controllor的bean，再用@RequestMapping("标记")来标记需要接受请求的方法，方法中第一个参数为HttpServletRequest类型，最后一个参数为Model类型，中间可以为任何POJO，只要符合标准，有set和get，Spring即可以根据网页请求中的参数名，自动绑定到POJO对象的属性名，这是相当方便的。其中的原理是什么呢？看下源代码就可以知道了。
首先，要知道Method对象的invoke（调用，借助）方法，看下这一段代码：
```java
Class clazz = Class.forName("TaskProvidePropsList");//这里的类的全名
      Object obj = clazz.newInstance();//获取类的实例
      Field[] fields = clazz.getDeclaredFields();//获取属性列表
      //写数据
      for(Field f : fields) {
       PropertyDescriptor pd = new PropertyDescriptor(f.getName(), clazz);//获取属性对象，要标明是哪个类
       Method wM = pd.getWriteMethod();//获得写方法，即setter
       obj = wM.invoke(obj, 2);//执行obj对象的wm方法，2为参数。因为知道是int类型的属性，所以传个int过去就是了。。实际情况中需要判断下他的参数类型，这里可以有返回值，为obj对象的引用
      }
      //读数据
      for(Field f : fields) {
       PropertyDescriptor pd = new PropertyDescriptor(f.getName(), clazz);//获取属性对象，要标明是哪个类
       Method rM = pd.getReadMethod();//获得读方法，即getter
       Integer num = (Integer) rM.invoke(obj);//执行obj对象的rm方法。因为知道是int类型的属性,所以转换成integer就是了。。也可以不转换直接打印
       System.out.println(num);
}
```
　　用法还是比较简单的。
　　接下来就是Spring中是怎么调用这个方法为POJO赋值的。
　　最先要追溯到 org.springframework.web.servlet.FrameworkServlet类中的doGet(HttpServletRequest request, HttpServletResponse response)等do方法（do方法是servlet中的基本方法，相当于Spring接管了所有do方法，然后去分发方法。若不使用框架，则需要自己去写servlet继承HttpServlet实现所有do方法，也是可以使用的，但是服务器较大时这样就太复杂了，框架就是为了这个而存在的）
　　这个方法中调用processRequest(request, response)，processRequest中调用doService(request, response)，doService是所有do方法的核心，负责把该请求根据情况分发给不同的service（@RequestMapping注解标明的方法）。
　　doService在FrameworkServlet的子类DispatcherServlet（Dispatch：派遣，调度）中实现，doService中又调用doDispatch(request, response)，把当前请求分配给合适的Service的合适方法。
　　doDispatch方法中调用(HandlerAdapter)ha.handle(processedRequest, response, mappedHandler.getHandler())方法去寻找与该请求挂钩的方法（processedRequest是由checkMultipart(request)转换而来的）（注释中写的Actually invoke the handler.）。
　　接着HandlerAdapter中的handle方法调用handleInternal(request, response, (HandlerMethod) handler)返回一个ModelAndView。　　
　　handleInternal方法中调用invokeHandleMethod(request, response, handlerMethod)方法返回一个ModelAndView。
　　然后invokeHandleMethod中调用ServletInvocableHandlerMethod类的invokeAndHandle(webRequest, mavContainer)方法。
　　该方法中第一句为：Object returnValue = invokeForRequest(webRequest, mavContainer, providedArgs);这里终于到核心了，为请求调用绑定类。
　　webRequest为httpRequest包装后的类，添加了uri成员，标记请求uri以便方便分配。mavContainer为ModelAndViewContainer类对象，用于绑定模块（POJO）和视图（view即网页）。
　　在invokeForRequest方法中，第一句为Object[] args = getMethodArgumentValues(request, mavContainer, providedArgs);该方法返回与该uri请求匹配的Mapping方法的参数列表。
　　getMethodArgumentValues中第一句为MethodParameter[] parameters = getMethodParameters();获取方法列表，经过处理后，返回方法中所有参数类型的Bean的一个实例并赋值，为args数组。
　　关键点来的：上一步中有对实例赋值的过程，这个值是从哪里来的呢？没错就是从Request的参数列表中来的，绑定数据就是在getMethodArgumentValues方法中做的。具体是怎么做的呢？
```java
private Object[] getMethodArgumentValues(NativeWebRequest request, ModelAndViewContainer mavContainer,
            Object... providedArgs) throws Exception {
        MethodParameter[] parameters = getMethodParameters();//获取所有参数所属的类
        Object[] args = new Object[parameters.length];
        for (int i = 0; i < parameters.length; i++) {
            MethodParameter parameter = parameters[i];
            parameter.initParameterNameDiscovery(this.parameterNameDiscoverer);
            GenericTypeResolver.resolveParameterType(parameter, getBean().getClass());
            args[i] = resolveProvidedArgument(parameter, providedArgs);
            if (args[i] != null) {
                continue;
            }
            if (this.argumentResolvers.supportsParameter(parameter)) {
                try {
                    args[i] = this.argumentResolvers.resolveArgument(
                            parameter, mavContainer, request, this.dataBinderFactory);
                    continue;
                }
                catch (Exception ex) {
                    if (logger.isTraceEnabled()) {
                        logger.trace(getArgumentResolutionErrorMessage("Error resolving argument", i), ex);
                    }
                    throw ex;
                }
            }
            if (args[i] == null) {
                String msg = getArgumentResolutionErrorMessage("No suitable resolver for argument", i);
                throw new IllegalStateException(msg);
            }
        }
        return args;
    }
    private String getArgumentResolutionErrorMessage(String message, int index) {
        MethodParameter param = getMethodParameters()[index];
        message += " [" + index + "] [type=" + param.getParameterType().getName() + "]";
        return getDetailedErrorMessage(message);
    }
```
　　就是这一句：args[i] = resolveProvidedArgument(parameter, providedArgs);和args[i] = this.argumentResolvers.resolveArgument(*parameter, mavContainer, request, this.dataBinderFactory);（*dataBinderFactory为数据绑定工厂方法*）*
　　主要起作用的是后一句，HandlerMethodArgumentResolverComposite.resolveArgument中调用org.springframework.web.method.support.HandlerMethodArgumentResolver.resolveArgument(parameter, mavContainer, webRequest, binderFactory);
　　在resolveArgument（多态方法，Request和Response和Model和普通POJO都有重写方法）中，以POJO重写方法为例：
```java
public final Object resolveArgument(
            MethodParameter parameter, ModelAndViewContainer mavContainer,
            NativeWebRequest request, WebDataBinderFactory binderFactory)
            throws Exception {
        String name = ModelFactory.getNameForParameter(parameter);
        Object attribute = (mavContainer.containsAttribute(name)) ?
                mavContainer.getModel().get(name) : createAttribute(name, parameter, binderFactory, request);
        WebDataBinder binder = binderFactory.createBinder(request, attribute, name);
        if (binder.getTarget() != null) {
            bindRequestParameters(binder, request);
            validateIfApplicable(binder, parameter);
            if (binder.getBindingResult().hasErrors()) {
                if (isBindExceptionRequired(binder, parameter)) {
                    throw new BindException(binder.getBindingResult());
                }
            }
        }
        // Add resolved attribute and BindingResult at the end of the model
        Map<String, Object> bindingResultModel = binder.getBindingResult().getModel();
        mavContainer.removeAttributes(bindingResultModel);
        mavContainer.addAllAttributes(bindingResultModel);
        return binder.getTarget();
    }
```
　　通过ModelFactiry获取Model的类名，之后获取mavContainer中该属性名称的对象，若没有则调用createAttribute(name, parameter, binderFactory, request)创建，从String value = getRequestValueForAttribute(attributeName, request);Request中获取这个名称的值，如果没有则调用父类的createAttribute，父类的createAttribute返回一个Bean的实体类，所有属性为空。
　　之后binderFactory.createBinder(request, attribute, name);为实体类创建一个绑定器，绑定器包含实体类，实体类名和http请求。然后初始化绑定器，为绑定器赋Request。
　　接着调用bindRequestParameters(binder, request);开始为Bean绑定值。bindRequestParameters中获取servetBinder和servletRequest，之后servletBinder.bind(servletRequest);
　　bind方法中，MutablePropertyValues mpvs = new ServletRequestParameterPropertyValues(request);获取请求中所有参数。addBindValues(mpvs, request);doBind(mpvs);
　　doBind中checkFieldDefaults(mpvs);checkFieldMarkers(mpvs);再调用父类的doBind。checkAllowedFields(mpvs);checkRequiredFields(mpvs);检查参数
　　applyPropertyValues(mpvs);方法把所有属性绑定到bean实体类中。再调用getPropertyAccessor().setPropertyValues(mpvs, isIgnoreUnknownFields(), isIgnoreInvalidFields());设置值。
```java
public void setPropertyValues(PropertyValues pvs, boolean ignoreUnknown, boolean ignoreInvalid)
            throws BeansException {
        List<PropertyAccessException> propertyAccessExceptions = null;
        List<PropertyValue> propertyValues = (pvs instanceof MutablePropertyValues ?
                ((MutablePropertyValues) pvs).getPropertyValueList() : Arrays.asList(pvs.getPropertyValues()));
        for (PropertyValue pv : propertyValues) {
            try {
                // This method may throw any BeansException, which won't be caught
                // here, if there is a critical failure such as no matching field.
                // We can attempt to deal only with less serious exceptions.
                setPropertyValue(pv);
            }
            catch (NotWritablePropertyException ex) {
                if (!ignoreUnknown) {
                    throw ex;
                }
                // Otherwise, just ignore it and continue...
            }
            catch (NullValueInNestedPathException ex) {
                if (!ignoreInvalid) {
                    throw ex;
                }
                // Otherwise, just ignore it and continue...
            }
            catch (PropertyAccessException ex) {
                if (propertyAccessExceptions == null) {
                    propertyAccessExceptions = new LinkedList<PropertyAccessException>();
                }
                propertyAccessExceptions.add(ex);
            }
        }
        // If we encountered individual exceptions, throw the composite exception.
        if (propertyAccessExceptions != null) {
            PropertyAccessException[] paeArray =
                    propertyAccessExceptions.toArray(new PropertyAccessException[propertyAccessExceptions.size()]);
            throw new PropertyBatchUpdateException(paeArray);
        }
    }
```
 　　List<PropertyValue> propertyValues存储所有PropertyValue，每个PropertyValue中包含属性名（键名）与值（从网页Request中传过来的，json方式最好），还有一个属性的map。之后调用setPropertyValue(pv);
　　其中nestedBw = getBeanWrapperForPropertyPath(propertyName);获取上面实体类。tokens = getPropertyNameTokens(getFinalPath(nestedBw, propertyName));使用propertyName存储所有属性（去除方括号和大括号），nestedBw.setPropertyValue(tokens, pv);为实体类设置值。最终有BeanWrapperImpl类的setPropertyValue方法，为Request中传过来的所有参数name值，与RequestMapping中函数传入参数Bean类型的成员变量名比较，如果有相同的，则调用相应的写方法setter给该变量设置值（其实还有更复杂的List，Array，Map类型绑定，都在BeanWrapperImpl类中实现，可以详细看看）。
　　总的来说：先遍历所有Mapping中参数的Bean对象，再遍历Request中所有参数和值对，接着再遍历Bean对象中所有方法，找到Bean中有set方法的和Request中参数名相同的成员变量，调用set方法设置该成员变量值。
