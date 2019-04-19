# 基于SpringMVC的RESTful HTTP API实践 - 零度的博客专栏 - CSDN博客
2017年10月17日 11:05:22[零度anngle](https://me.csdn.net/zmx729618)阅读数：396标签：[spring mvc																[restful](https://so.csdn.net/so/search/s.do?q=restful&t=blog)](https://so.csdn.net/so/search/s.do?q=spring mvc&t=blog)
个人分类：[Restful																[SpringMVC](https://blog.csdn.net/zmx729618/article/category/6248665)](https://blog.csdn.net/zmx729618/article/category/6215365)
[http://alexpdh.com/2017/06/26/springMVC-restful-api/](http://alexpdh.com/2017/06/26/springMVC-restful-api/)
#### 理解 REST
**REST（Representational State Transfer）**,中文翻译叫“表述性状态转移”。是 [Roy
 Thomas Fielding](https://en.wikipedia.org/wiki/Roy_Fielding) 在他2000年的[博士论文](http://www.ics.uci.edu/~fielding/pubs/dissertation/top.htm)中提出的。它与传统的
 SOAP Web 服务区别在于，REST关注的是要处理的数据，而 SOAP 主要关注行为和处理。要理解好 REST，根据其首字母拆分出的英文更容易理解。
**表述性（Representational）:**对于 REST 来说，我们网络上的一个个URI资源可以用各种形式来表述，例如：XML、JSON或者HTML等。
**状态（State）：**REST 更关注资源的状态而不是对资源采取的行为。
**转移（Transfer）：**在网络传输过程中，REST 使资源以某种表述性形式从一个应用转移到另一个应用（如从服务端转移到客户端）。
> 
具体来说，REST 中存在行为，它的行为是通过 HTTP 表示操作的方法来定义的即：GET、POST、PUT、DELETE、PATCH；GET用来获取资源，POST用来新建资源（也可以用于更新资源），PUT用来更新资源，DELETE用来删除资源，PATCH用来更新资源。 基于 REST 这样的观点，我们需要避免使用 REST服务、REST Web服务 这样的称呼，这些称呼多少都带有一些强调行为的味道。
#### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8-RESTful-%E6%9E%B6%E6%9E%84%E8%AE%BE%E8%AE%A1%E4%BD%BF%E7%94%A8%E8%AF%AF%E5%8C%BA)使用 RESTful 架构设计使用误区
**RESTful 架构：**是基于 REST 思想的时下比较流行的一种互联网软件架构。它结构清晰、符合标准、易于理解、扩展方便，所以正得到越来越多网站的采用。
- 在没有足够了解 REST 的时候，我们很容易错误的将其视为 “基于 URL 的 Web 服务”，即将 REST 和 SOAP 一样，是一种远程过程调用（remote procedure call，RPC）的机制。但是 REST 和 RPC 几乎没有任何关系，RPC 是面向服务的，而 REST 是面向资源的，强调描述应用程序的事物和名词。这样很容易导致的一个结果是我们在设计 RESTful API 时，在 URI 中使用动词。例如：GET /user/getUser/123。正确写法应该是
 GET /user/123。
#### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8-springMVC-%E6%94%AF%E6%8C%81-RESTful)使用 springMVC 支持 RESTful
在 spring 3.0 以后，spring 这对 springMVC 的一些增强功能对 RESTful 提供了良好的支持。在4.0后的版本中，spring 支持一下方式创建 REST 资源：
- 控制器可以处理所有的 HTTP 方法，包含几个主要的 REST 方法：GET、POST、PUT、DELETE、PATCH；
- 借助 @PathVariable 注解，控制器能够处理参数化的 URL（将变量输入作为 URL 的一部分）；
- 借助 spring 的视图解析器，资源能够以多种方式进行表述，包括将模型数据渲染为 XML、JSON、Atom、已经 RSS 的 View 实现；
- 可以使用 ContentNegotiatingViewResolver 来选择最适合客户端的表述；
- 借助 @ResponseBody 注解和各种 HttpMessageConverter实现，能够替换基于视图的渲染方式；
- 类似地，@RequestBody 注解以及 HttpMessageConverter实现可以将传入的 HTTP 数据转化为传入控制器处理方法的 Java 对象；
- 借助 RestTemplate ，spring 应用能够方便地使用 REST 资源。
#### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E5%88%9B%E5%BB%BA-RESTful-%E6%8E%A7%E5%88%B6%E5%99%A8)创建 RESTful 控制器
代码清单
```
package com.pengdh.controller;
import com.pengdh.entity.EmployeeEntity;
import com.pengdh.service.EmployeeService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
/**
 * @author pengdh
 * @date: 2017-06-27 0:08
 */
@Controller
@RequestMapping("/employs")
public class EmployeeController {
  @Autowired
  private EmployeeService empService;
  @GetMapping(value = "/list", produces = { "application/json;charset=UTF-8" })
  public List<EmployeeEntity> employs(Integer offset,Integer limit) {
    offset = offset == null ? 0 : offset;
    limit = limit == null ? 20 : limit;
    return empService.queryEmployList(offset,limit);
  }
}
```
代码的大致过程是当客户端发起对 “/employs” 的 GET 请求时，将调用服务端的 employs 方法，服务端通过注入的 EmployeeService 获取到一个 EmployeeEntity 列表，并将列表以 JSON 的表述形式返回给客户端。
- 需要注意的是这里控制器本身并不关心资源如何表述。控制器以 Java 对象的方式来处理资源。控制器完成了它的工作以后，资源才会被转化成为适合客户端的形式。spring 提供了两种方法将资源的 java 表述形式转化为发送给客户端的表述形式：
- 内容协商（Content negotiation）：选择一个视图，它能够将模型渲染为呈现给客户端的表述形式；
- 消息转化器（Message conversion）：通过一个消息转换器将控制器所返回的对象转换为呈现给客户端的表述形式。
> 
对于上述两种方式，第一种方式是通过 ContentNegotiatingViewResolver 作为 ViewResolver 的实现，主要是用于将资源渲染人类用户接口所需要的视图模型，如：HTML、JSP等也可以渲染。也可以针对不是人类客户端产生 JSON 或 XML，但是效果不是很理想，往往会产生一些不是客户端所需要的预期结果。如：客户端希望得到的响应可能是：{“name”:”zhangs”,”age”:”20”}。而模型是
 key-value 组成的 map ，可能最终的响应是这样的：{“user”:{“name”:”zhangs”,”age”:”20”}}。基于内容协商的这些限制，这里我们主要讨论第二种方式：使用 Spring 的消息转换功能来生成资源表述。
#### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8-HTTP-%E6%B6%88%E6%81%AF%E8%BD%AC%E6%8D%A2%E5%99%A8)使用 HTTP 消息转换器
这是一种更为直接的方式，消息转换器能够将控制器产生的数据转换为服务于客户端的表述形式。常用的一些消息转换器如：Jackson 的 MappingJacksonHttpMessageConverter 实现 JSON 消息和 Java 对象的互相转换； JAXB 库的 Jaxb2RootElementHttpMessageConverter 实现 XML 和 Java 对象的相互转换等。
##### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E9%80%9A%E8%BF%87-ResponseBody-%E6%B3%A8%E8%A7%A3%E5%AE%9E%E7%8E%B0%E5%93%8D%E5%BA%94%E4%BD%93%E4%B8%AD%E8%BF%94%E5%9B%9E%E8%B5%84%E6%BA%90%E7%8A%B6%E6%80%81%E3%80%82)通过 @ResponseBody 注解实现响应体中返回资源状态。
正常情况下，当处理方法返回 Java 对象时，这个对象会放在模型中并在视图中渲染使用。但是，如果使用了消息转换功能的话，我们需要告诉 Spring 跳过正常的模型/视图流程，并使用消息转换器。实现这种方式最简单的方式是在控制器的方法上添加 @ResponseBody 注解。如：
```
@GetMapping(value = "/list", produces = { "application/json;charset=UTF-8" })
@ResponseBody
public List<EmployeeEntity> employs(Integer offset,Integer limit) {
  offset = offset == null ? 0 : offset;
  limit = limit == null ? 20 : limit;
  return empService.queryEmployList(offset,limit);
}
```
这里 @ResponseBody 注解会告知 Spring 将 List 转换成 JSON 这样的表述形式作为资源发送给客户端。
##### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8-RequestBody-%E6%B3%A8%E8%A7%A3%E5%AE%9E%E7%8E%B0%E5%9C%A8%E8%AF%B7%E6%B1%82%E4%BD%93%E4%B8%AD%E6%8E%A5%E6%94%B6%E8%B5%84%E6%BA%90%E7%8A%B6%E6%80%81)使用 @RequestBody 注解实现在请求体中接收资源状态
使用 @RequestBody 注解可以告知 Spring 查找一个消息转换器，将来自客户端的资源表述转换为对象。如：
```
@PostMapping(value = "/employ", produces = { "application/json;charset=UTF-8" })
public int saveEmploy(@RequestBody EmployeeEntity employeeEntity) {
  return empService.save(employeeEntity);
}
```
##### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8-RestController-%E6%B3%A8%E8%A7%A3%E4%B8%BA%E6%8E%A7%E5%88%B6%E5%99%A8%E9%BB%98%E8%AE%A4%E8%AE%BE%E7%BD%AE%E6%B6%88%E6%81%AF%E8%BD%AC%E6%8D%A2)使用 @RestController 注解为控制器默认设置消息转换
Spring 4.0 引入了 @RestController 注解，在控制器是用 @RestController 代替 @Controller 的话，Spring 将会为该控制器的所有处理方法应用消息转换功能。我们不必在每个方法都添加 @ResponseBody 注解了。如：
```
package com.pengdh.controller;
import com.pengdh.entity.EmployeeEntity;
import com.pengdh.service.EmployeeService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
/**
 * @author pengdh
 * @date: 2017-06-27 0:08
 */
@RestController
@RequestMapping("/employs")
public class EmployeeController {
  @Autowired
  private EmployeeService empService;
  @GetMapping(value = "/list", produces = { "application/json;charset=UTF-8" })
  public List<EmployeeEntity> employs(Integer offset,Integer limit) {
    offset = offset == null ? 0 : offset;
    limit = limit == null ? 20 : limit;
    return empService.queryEmployList(offset,limit);
  }
  @PostMapping(value = "/employ", produces = { "application/json;charset=UTF-8" })
  public int saveEmploy(@RequestBody EmployeeEntity employeeEntity) {
    return empService.save(employeeEntity);
  }
}
```
#### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%B8%BA%E5%AE%A2%E6%88%B7%E7%AB%AF%E6%8F%90%E4%BE%9B%E5%85%B6%E4%BB%96%E5%85%83%E6%95%B0%E6%8D%AE)为客户端提供其他元数据
##### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8-ResponseEntity-%E6%8F%90%E4%BE%9B%E6%9B%B4%E5%A4%9A%E5%93%8D%E5%BA%94%E7%9B%B8%E5%85%B3%E7%9A%84%E5%85%83%E6%95%B0%E6%8D%AE)使用 ResponseEntity 提供更多响应相关的元数据
**可以利用 ResponseEntity 给客户端返回状态码、设置响应头信息等，如给客户端提供返回码:**
```
@GetMapping(value = "/{id}", produces = { "application/json;charset=UTF-8" })
public ResponseEntity<EmployeeEntity> employById(@PathVariable long id) {
  HttpStatus status = null;
  EmployeeEntity employeeEntity = empService.selectById(id);
  if (employeeEntity != null) {
    status = HttpStatus.OK;
  } else {
    status = HttpStatus.NOT_FOUND;
  }
  return new ResponseEntity<EmployeeEntity>(employeeEntity, status);
}
```
> 
如果没有 if 判断，当根据 id 找不到对应的信息的时候，返回给客户端的状态码是默认的 HttpStatus.OK；当加上了判断条件后如果没有相应的信息返回则设置返回状态码为 HttpStatus.NOT_FOUND，最后通过 new 一个 ResponseEntity 会将查询信息和状态码一起返回到客户端。
- 另外，ResponseEntity 还包含有 @ResponseBody 的语义，上面示例中并没有使用 @ResponseBody 注解，但是 ResponseEntity 的负载部分同样可以渲染到响应体中。
##### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8%E6%8E%A7%E5%88%B6%E5%99%A8%E5%BC%82%E5%B8%B8%E5%A4%84%E7%90%86%E5%99%A8-ExceptionHandler-%E5%A4%84%E7%90%86%E5%BC%82%E5%B8%B8%E4%BF%A1%E6%81%AF)使用控制器异常处理器 @ExceptionHandler 处理异常信息
@ExceptionHandler 可以用到控制器的方法中，处理特定的异常：
**创建响应包装类 ResponseResult**
```
package com.pengdh.dto;
import java.io.Serializable;
/**
 * 响应结果封装类
 *
 * @author pengdh
 * @date: 2017-06-29 0:34
 */
public class ResponseResult<T> implements Serializable {
  private static final long serialVersionUID = -3371934618173052904L;
  private String code;
  private String desc;
  private T data;
  public ResponseResult() {
  }
  public ResponseResult(String code, String desc) {
    this.code = code;
    this.desc = desc;
  }
  public ResponseResult(String code, T data) {
    this.code = code;
    this.data = data;
  }
  public String getCode() {
    return code;
  }
  public void setCode(String code) {
    this.code = code;
  }
  public String getDesc() {
    return desc;
  }
  public void setDesc(String desc) {
    this.desc = desc;
  }
  public T getData() {
    return data;
  }
  public void setData(T data) {
    this.data = data;
  }
  @Override
  public String toString() {
    return "ResponseResult{" +
        "code='" + code + '\'' +
        ", desc='" + desc + '\'' +
        ", data=" + data +
        '}';
  }
}
```
**创建一个异常类 ResourceNotFoundException**
```
package com.pengdh.exception;
/**
 * 资源未找到异常
 *
 * @author pengdh
 * @date: 2017-06-29 0:55
 */
public class ResourceNotFoundException extends RuntimeException {
  private static final long serialVersionUID = 4880328265878141724L;
  public ResourceNotFoundException() {
    super();
  }
  public ResourceNotFoundException(String message) {
    super(message);
  }
  public ResourceNotFoundException(String message, Throwable cause) {
    super(message, cause);
  }
}
```
**控制器 EmployeeController**
```
package com.pengdh.controller;
import com.pengdh.dto.ResponseResult;
import com.pengdh.entity.EmployeeEntity;
import com.pengdh.exception.ResourceNotFound;
import com.pengdh.service.EmployeeService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
/**
 * @author pengdh
 * @date: 2017-06-27 0:08
 */
@RestController
@RequestMapping("/employs")
public class EmployeeController {
  @Autowired
  private EmployeeService empService;
  @GetMapping(value = "/{id}", produces = { "application/json;charset=UTF-8" })
  public ResponseResult<EmployeeEntity> employById(@PathVariable long id) {
    ResponseResult<EmployeeEntity> result = new ResponseResult<EmployeeEntity>();
    HttpStatus status = null;
    EmployeeEntity employeeEntity = empService.selectById(id);
    if (employeeEntity == null) {
      throw new ResourceNotFoundException(String.valueOf(id));
    }
    result.setCode(String.valueOf(HttpStatus.OK));
    result.setData(employeeEntity);
    return result;
  }
  @ExceptionHandler(ResourceNotFoundException.class)
  public ResponseResult<Object> handlerException(ResourceNotFoundException e) {
    ResponseResult<Object> result = new ResponseResult<Object>();
    result.setCode(String.valueOf(HttpStatus.NOT_FOUND));
    result.setDesc(e.getMessage());
    return result;
  }
}
```
> 
从控制器代码可以看出，我们通过 @ExceptionHandler 能将控制器的方法的异常场景分出来单独处理。
##### [](http://alexpdh.com/2017/06/26/springMVC-restful-api/#%E4%BD%BF%E7%94%A8-RestControllerAdvice-%E6%8D%95%E8%8E%B7%E6%89%80%E6%9C%89-controller-%E6%8A%9B%E5%87%BA%E7%9A%84%E5%BC%82%E5%B8%B8)使用 @RestControllerAdvice 捕获所有 controller 抛出的异常
@ControllerAdvice是一个@Component，用于定义@ExceptionHandler的@InitBinder和@ModelAttribute方法，适用于所有使用@ RequestMapping的方法。
**新建一个异常处理类 GlobalExceptionHandler**
```
package com.pengdh.exception;
import com.pengdh.dto.ResponseResult;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseStatus;
import org.springframework.web.bind.annotation.RestControllerAdvice;
/**
 * 全局异常处理器
 *
 * @author pengdh
 * @date: 2017-07-11 0:00
 */
@RestControllerAdvice
public class GlobalExceptionHandler {
  @ExceptionHandler(ResourceNotFoundException.class)
  @ResponseStatus(HttpStatus.INTERNAL_SERVER_ERROR)
  public ResponseResult<Object> handlerException(ResourceNotFoundException e) {
    return new ResponseResult<Object>(String.valueOf(HttpStatus.NOT_FOUND), e.getMessage());
  }
}
```
**控制器 EmployeeController**
```
package com.pengdh.controller;
import com.pengdh.dto.ResponseResult;
import com.pengdh.entity.EmployeeEntity;
import com.pengdh.exception.ResourceNotFoundException;
import com.pengdh.service.EmployeeService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
/**
 * @author pengdh
 * @date: 2017-06-27 0:08
 */
@RestController
@RequestMapping("/employs")
public class EmployeeController {
  @Autowired
  private EmployeeService empService;
  @GetMapping(value = "/list", produces = { "application/json;charset=UTF-8" })
  public List<EmployeeEntity> employs(Integer offset,Integer limit) {
    offset = offset == null ? 0 : offset;
    limit = limit == null ? 20 : limit;
    return empService.queryEmployList(offset,limit);
  }
  @PostMapping(value = "/employ", produces = { "application/json;charset=UTF-8" })
  public int saveEmploy(@RequestBody EmployeeEntity employeeEntity) {
    return empService.save(employeeEntity);
  }
  @GetMapping(value = "/{id}", produces = { "application/json;charset=UTF-8" })
  public ResponseResult<EmployeeEntity> employById(@PathVariable long id) {
    ResponseResult<EmployeeEntity> result = new ResponseResult<EmployeeEntity>();
    HttpStatus status = null;
    EmployeeEntity employeeEntity = empService.selectById(id);
    if (employeeEntity == null) {
      throw new ResourceNotFoundException(String.valueOf(id));
    }
    result.setCode(String.valueOf(HttpStatus.OK));
    result.setData(employeeEntity);
    return result;
  }
}
```
> 
从代码可以看出，我们将 controller 中的异常方法移到了一个新增的异常处理类。这样，如果其他 controller 类也抛出 ResourceNotFoundException 异常的时候，就会被 GlobalExceptionHandler 捕获。
