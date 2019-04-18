# Spring mvc 使用 jackson2 返回json格式数据时数据的构成 - z69183787的专栏 - CSDN博客
2014年12月04日 14:41:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12632
xml：
```
<bean id="viewResolver"
          class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
        <property name="contentNegotiationManager" ref="cnManager"/>
        <property name="viewResolvers"><!-- 针对freemarker的视图配置 --> 
            <list>
                <bean class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"/>
                   <!-- <property name="viewNames" value=".ftl"/>-->
                    <property name="contentType" value="text/html; charset=utf-8"/>
                    <property name="cache" value="true" />
                    <property name="prefix" value="" />
                    <property name="suffix" value=".ftl" />
                    <property name="order" value="2"/>
                </bean>
                <!--<bean id="liteDeviceDelegatingViewResolver" class="org.springframework.mobile.device.view.LiteDeviceDelegatingViewResolver">-->
                    <!--<constructor-arg>-->
                        <!--<bean-->
                                <!--class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">-->
                            <!--<property name="cache" value="true"/>-->
                            <!--<property name="requestContextAttribute" value="rc"/>-->
                            <!--<property name="prefix" value=""/>-->
                            <!--<property name="suffix" value=".ftl"/>-->
                            <!--<property name="exposeSpringMacroHelpers" value="true"/>-->
                            <!--<property name="exposeRequestAttributes" value="true"/>-->
                            <!--<property name="exposeSessionAttributes" value="true"/>-->
                            <!--<property name="contentType" value="text/html;charset=UTF-8"/>-->
                        <!--</bean>-->
                    <!--</constructor-arg>-->
                    <!--<property name="mobilePrefix" value="mobile/" />-->
                    <!--<property name="tabletPrefix" value="tablet/" />-->
                    <!--<property name="enableFallback" value="true" />-->
                <!--</bean>-->
                <bean
                        class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="prefix" value="/WEB-INF/view/jsp/"></property><!-- 配置页面路径 -->
                    <property name="suffix" value=".jsp"></property><!-- 文件以value值结尾 -->
                </bean>
            </list>
        </property>
        <property name="defaultViews">
            <list>
                <bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView">
                    <!-- <property name="extractValueFromSingleKeyModel" value="true"/> -->
                    <property name="prettyPrint" value="true"/>
                    <property name="contentType" value="text/plain"/>
                </bean>
                <bean class="org.springframework.web.servlet.view.xml.MarshallingView">
                    <constructor-arg ref="jaxb2Marshaller">
                    </constructor-arg>
                </bean>
                <bean class="com.wonders.stpt.bid.controller.JXLExcelView"/>
            </list>
        </property>
    </bean>
    <bean id="jaxb2Marshaller" class="org.springframework.oxm.jaxb.Jaxb2Marshaller">
        <property name="classesToBeBound">
            <list>
                <value>com.wonders.stpt.bid.domain.Route</value>
                <value>com.wonders.stpt.bid.domain.Dictionary</value>
            </list>
        </property>
    </bean>
```
方法1：
```java
@RequestMapping(method = RequestMethod.GET, value = "/dictionaries")
    public void dictionaries( Model model, String id) throws Exception {
        Dictionary dictionary = new Dictionary();
        dictionary.setParentNo(id);
        List<Dictionary> dictionaries = dictionaryService.getDictionaries(dictionary);
        List list = new ArrayList();
        for (Dictionary dict : dictionaries) {
            Map map = new HashMap();
            map.put("id", dict.getDictId());
            map.put("name", dict.getDictName());
            map.put("dictType", dict.getDictType());
            map.put("dictNo", dict.getDictNo());
            map.put("dictOrder", dict.getDictOrder());
            if (dict.getParentNo() == null)
                map.put("isParent", true);
            else
                map.put("isParent", false);
            list.add(map);
        }
        model.addAttribute("root", list);
        model.addAttribute("maxNo", dictionaryService.getMaxDictNo());
    }
```
json1：
```
{
  "root" : [ {
    "id" : "7",
    "dictNo" : "7",
    "dictOrder" : 1,
    "name" : "行业",
    "isParent" : true,
    "dictType" : "trade"
  }, {
    "id" : "5",
    "dictNo" : "5",
    "dictOrder" : 2,
    "name" : "集团",
    "isParent" : true,
    "dictType" : "groups"
  }, {
    "id" : "6",
    "dictNo" : "6",
    "dictOrder" : 3,
    "name" : "线路",
    "isParent" : true,
    "dictType" : "routeType"
  } ],
  "currentUser" : {
    "userId" : "2",
    "loginName" : "G00100000123",
    "password" : null,
    "userName" : "李晴阳",
    "roles" : [ {
      "roleId" : null,
      "roleName" : "ROLE_EDITOR",
      "description" : null,
      "removed" : null,
      "createTime" : null,
      "creator" : null,
      "updateTime" : null,
      "updater" : null
    } ]
  },
  "maxNo" : 20
```
方法2：
```java
@RequestMapping(method = RequestMethod.GET, value = "/dictionaries")
    public void dictionaries(Dictionary dictionary, Model model, String id) throws Exception {
        dictionary.setParentNo(id);
        List<Dictionary> dictionaries = dictionaryService.getDictionaries(dictionary);
        List list = new ArrayList();
        for (Dictionary dict : dictionaries) {
            Map map = new HashMap();
            map.put("id", dict.getDictId());
            map.put("name", dict.getDictName());
            map.put("dictType", dict.getDictType());
            map.put("dictNo", dict.getDictNo());
            map.put("dictOrder", dict.getDictOrder());
            if (dict.getParentNo() == null)
                map.put("isParent", true);
            else
                map.put("isParent", false);
            list.add(map);
        }
        model.addAttribute("root", list);
        model.addAttribute("maxNo", dictionaryService.getMaxDictNo());
    }
```
json2：
```
{
  "dictionary" : {
    "dictId" : null,
    "dictNo" : null,
    "dictName" : null,
    "dictType" : null,
    "dictOrder" : null,
    "parentNo" : null,
    "createTime" : null,
    "updateTime" : null,
    "removed" : null,
    "creator" : null,
    "updater" : null
  },
  "root" : [ {
    "id" : "7",
    "dictNo" : "7",
    "dictOrder" : 1,
    "name" : "行业",
    "isParent" : true,
    "dictType" : "trade"
  }, {
    "id" : "5",
    "dictNo" : "5",
    "dictOrder" : 2,
    "name" : "集团",
    "isParent" : true,
    "dictType" : "groups"
  }, {
    "id" : "6",
    "dictNo" : "6",
    "dictOrder" : 3,
    "name" : "线路",
    "isParent" : true,
    "dictType" : "routeType"
  } ],
  "currentUser" : {
    "userId" : "2",
    "loginName" : "G00100000123",
    "password" : null,
    "userName" : "李晴阳",
    "roles" : [ {
      "roleId" : null,
      "roleName" : "ROLE_EDITOR",
      "description" : null,
      "removed" : null,
      "createTime" : null,
      "creator" : null,
      "updateTime" : null,
      "updater" : null
    } ]
  },
  "maxNo" : 20
}
```
可以看到，除了方法中的Model被转换了，另一个参数 Dictionary也被转换了。
