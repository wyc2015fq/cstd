# SpringMVC 重定向参数 RedirectAttributes - z69183787的专栏 - CSDN博客
2016年09月20日 16:03:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6552
SpringMVC 中常用到 redirect 来实现重定向。但使用场景各有需求，如果只是简单的页面跳转显然无法满足所有要求，比如重定向时需要在 url 中拼接参数，或者返回的页面需要传递 Model。SpringMVC 用 RedirectAttributes 解决了这两个需要。
首先，在 Controller 中做 redirect 中可用如下方式实现：
return new ModelAndView(“redirect:/index”);
或
return “redirect:/index”;
此时，如果只是重定向至某一 URL 或者比较简单地址，也可以不用 RedirectAttributes，直接拼接，如：return “redirect:/index?param1=value1″;
但是这样似乎有点过于简单粗暴，而且参数多了很容易使代码可读性变差。使用 RedirectAttributes 来设置重定向页面的参数，SpringMVC 会自动拼接 url。接下来主要介绍该对象的两个方法：
1. addAttribute
```
@RequestMapping("/save")
public String save(User user, RedirectAttributes redirectAttributes) {
    redirectAttributes.addAttribute("param", "value1");
    return "redirect:/index";
}
```
请求 /save 后，跳转至/index，并且会在url拼接 ?param=value1。
2. addFlashAttribute
```
@RequestMapping("/save")
public String save(User user, RedirectAttributes redirectAttributes) {
    redirectAttributes.addFlashAttribute("param", "value1");
    return "redirect:/index";
}
```
请求 /save 后，跳转至 /index，并且可以在 index 对应的模版中通过表达式，比如 jsp 中 jstl 用 ${param}，获取返回值。该值其实是保存在 session 中的，并且会在下次重定向请求时删除。
RedirectAttributes 中两个方法的简单介绍就是这样。
