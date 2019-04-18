# SpringMvc 中RedirectAttributes对象重定向传参 - z69183787的专栏 - CSDN博客
2016年09月20日 16:01:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1511
### Struts2式的FlashAttribute
为了防止用户刷新重复提交，save操作之后一般会redirect到另一个页面，同时带点操作成功的提示信息。因为是Redirect，Request里的attribute不会传递过去，如果放在session中，则需要在显示后及时清理，不然下面每一页都带着这个信息也不对。Spring在3.1才提供了这个能力。
publicStringsave(@ModelAttribute("group")Groupgroup,RedirectAttributesredirectAttributes){accountManager.saveGroup(group);redirectAttributes.addFlashAttribute("message","修改权限组成功");return"redirect:/account/group/";}
摘录自：[https://github.com/springside/springside4/wiki/SpringMVC](https://github.com/springside/springside4/wiki/SpringMVC)
