# SpringMVC 异常处理 返回异常code及前端AJAX解析 - z69183787的专栏 - CSDN博客
2017年01月20日 11:10:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4744
后端：
统一异常处理器
```java
@ControllerAdvice
public class DefaultExceptionHandler {
    @ExceptionHandler(value = Exception.class)
    @ResponseStatus(HttpStatus.BAD_REQUEST)
    @ResponseBody
    public ErrorResponse errorResponse(Exception exception) {
        return new ErrorResponse(exception.getMessage());
    }
    /**
     * 没有权限 异常
     * <p/>
     * 后续根据不同的需求定制即可
     */
    @ExceptionHandler({NullPointerException.class})
    @ResponseStatus(HttpStatus.UNAUTHORIZED)
    public ModelAndView processUnauthenticatedException(NativeWebRequest request, Exception e) {
        ModelAndView mv = new ModelAndView();
        mv.addObject("exception", e);
        mv.setViewName("error/unauthorized");
        return mv;
    }
    public class ErrorResponse {
        private String message;
        public ErrorResponse(String message) {
            this.message = message;
        }
        public String getMessage() {
            return message;
        }
    }
}
```
Controller：
```java
@ResponseBody
    @RequestMapping(method = RequestMethod.GET, value = "status",produces = {"application/json"})
    public Test statusTest(Test t) throws Exception {
        //System.out.println(t);
        int a= 0/0;
        return new Test();
    }
```
![](https://img-blog.csdn.net/20170120110821978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
前端
![](https://img-blog.csdn.net/20170120110923438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其余code处理方式一致
