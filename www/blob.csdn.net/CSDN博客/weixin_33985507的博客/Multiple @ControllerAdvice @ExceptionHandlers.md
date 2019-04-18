# Multiple @ControllerAdvice @ExceptionHandlers - weixin_33985507的博客 - CSDN博客
2019年01月10日 19:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
[https://stackoverflow.com/questions/19498378/setting-precedence-of-multiple-controlleradvice-exceptionhandlers](https://stackoverflow.com/questions/19498378/setting-precedence-of-multiple-controlleradvice-exceptionhandlers)
### 等级最低: generic java.lang.Exception
```
@ControllerAdvice
@Order(Ordered.LOWEST_PRECEDENCE)
public class GlobalEscapeExceptionHandler {
    private static final Logger logger = LoggerFactory.getLogger(GlobalEscapeExceptionHandler.class);
    @ExceptionHandler(Exception.class)
    @ResponseBody
    public Object exceptionHandler(Exception e) {
        logger.error("接口请求Handler映射失败或方法内执行错误: ", e);
        return ResponseUtil.serious();
    }
}
```
### 等级最高: specific exception
```
@RestController
@ControllerAdvice
@Order(Ordered.HIGHEST_PRECEDENCE)
public class SExceptionHandler {
    private static final Logger LOGGER = LoggerFactory.getLogger(SExceptionHandler.class);
    public static final String illegalArgument = "非法参数异常，参数值类型非法，或者参数值为空，或者缺少必传参数";
    @ExceptionHandler(value = {SignFailedException.class})
    public Map<Object,Object> signFailded(SignFailedException exception) {
        HashMap<Object, Object> map = new HashMap<>();
        map.put("exception",exception.getMessage());
        return map;
    }
//    @ExceptionHandler(value = {IllegalArgumentException.class})
//    public CommonResult illegalArgument(IllegalArgumentException exception) {
//        return ResultFactory.clientError(illegalArgument);
//    }
}
```
