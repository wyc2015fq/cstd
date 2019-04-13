
# selenium 获取input输入框中的值的方法 - anlcy - 博客园






# [selenium 获取input输入框中的值的方法](https://www.cnblogs.com/camilla/p/7988241.html)
方法一：获取input的文本值
<input class="form-text-normal" id="txtName" name="Name" type="text" maxlength="45" >星河学院</input>
chrome.findElement( By.id("txtName")  ).getText();

方法二：获取input中value的值
<input class="form-text-normal" id="txtName" name="Name" type="text" maxlength="45" value="星河学院">
chrome.findElement(By.id("txtName")  ).getAttribute("value") ;





