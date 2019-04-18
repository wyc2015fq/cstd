# 使用ExceptionHandler进行Spring mvc 异常处理 - z69183787的专栏 - CSDN博客
2017年01月20日 17:27:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1568
```java
public abstract class AbstractController {
	
	protected final Logger logger = LoggerFactory.getLogger(getClass());
	
	@ExceptionHandler(Exception.class)
	@ResponseStatus(HttpStatus.INTERNAL_SERVER_ERROR)
	public @ResponseBody ResponseEntity<ErrorMessageDto> handleAllExceptions(Exception ex) {
		ErrorMessageDto result = new ErrorMessageDto();
		if (ex instanceof UnAuthorityException) {
			result.setMessage(ex.getMessage());
			result.setStatus(401);
			return new ResponseEntity<ErrorMessageDto>(result, HttpStatus.FORBIDDEN);
		} else if (ex instanceof RunTimeException) {
			result.setMessage(ex.getMessage());
			result.setStatus(500);
			return new ResponseEntity<ErrorMessageDto>(result, HttpStatus.INTERNAL_SERVER_ERROR);
		} else {
			logger.error(ex.getMessage(), ex);
			Cat.logError(ex);
			result.setMessage(ex.getMessage());
			result.setStatus(500);
			return new ResponseEntity<ErrorMessageDto>(result, HttpStatus.INTERNAL_SERVER_ERROR);
		}
	}
```
```java
public class ErrorMessageDto {
    private int status;
    private String message;
    public ErrorMessageDto() {
    }
    public ErrorMessageDto(int status, String message) {
        this.status = status;
        this.message = message;
    }
    public int getStatus() {
        return status;
    }
    public void setStatus(int status) {
        this.status = status;
    }
    public String getMessage() {
        return message;
    }
    public void setMessage(String message) {
        this.message = message;
    }
    public void setParam(int status, String message) {
        this.status = status;
        this.message = message;
    }
}
```
