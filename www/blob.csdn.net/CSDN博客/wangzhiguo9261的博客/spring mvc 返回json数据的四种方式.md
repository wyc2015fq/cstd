
# spring mvc 返回json数据的四种方式 - wangzhiguo9261的博客 - CSDN博客


2018年07月03日 09:05:43[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：874


一.返回**ModelAndView,其中包含map集**
![复制代码](https://common.cnblogs.com/images/copycode.gif)
/** 返回ModelAndView类型的结果
     * 检查用户名的合法性,如果用户已经存在，返回false，否则返回true(返回json数据，格式为{"valid",true})*/@RequestMapping(value= "/checkNameExistsMethod2", produces = "application/json;charset=UTF-8")//这里的produces值在不设置的情况下将根据返回结果自动决定public@ResponseBody
    ModelAndView checkNameValidMethod2(@RequestParam String name) {booleanresult =true;//...Map<String, Boolean> map =newHashMap<>();
        map.put("valid", result);returnnewModelAndView(newMappingJackson2JsonView(), map);
    }![复制代码](https://common.cnblogs.com/images/copycode.gif)

二.返回String类型的json，这里有两种方式。
方式一：使用jackson-databind-x.x.x.jar包中的ObjectMapper将Map型数据改写为String并返回
![复制代码](https://common.cnblogs.com/images/copycode.gif)
/** 返回String类型的结果
     * 检查用户名的合法性,如果用户已经存在，返回false，否则返回true(返回json数据，格式为{"valid",true})*/@RequestMapping(value= "/checkNameExistsMethod1", produces = "application/json;charset=UTF-8")public@ResponseBody
    String checkNameValidMethod1(@RequestParam String name) {booleanresult =true;//...Map<String, Boolean> map =newHashMap<>();
        map.put("valid", result);
        ObjectMapper mapper=newObjectMapper();
        String resultString= "";try{
            resultString=mapper.writeValueAsString(map);
        }catch(JsonProcessingException e) {
            e.printStackTrace();
        }returnresultString;
    }![复制代码](https://common.cnblogs.com/images/copycode.gif)

方式二：
直接返回字符串,主要key/value值必须使用含有转义字符\的双引号，单引号无效
![复制代码](https://common.cnblogs.com/images/copycode.gif)
/** 返回String类型的结果
     * 检查用户名的合法性,如果用户已经存在，返回false，否则返回true(返回json数据，格式为{"valid",true})*/@RequestMapping(value= "/checkNameExistsMethod1",produces = "application/json;charset=UTF-8")public@ResponseBody
    String checkNameValidMethod1(@RequestParam String name) {booleanresult =true;
     
        String resultString= "{\"result\":true}";//注意一定是双引号 "{\"result\":\"success\"}"returnresultString;
    }![复制代码](https://common.cnblogs.com/images/copycode.gif)

三.返回任何预定义class类型的结果：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
@RequestMapping(value = "/findEmployeebyName")public@ResponseBody
    Employee findEmployeebyName(String name) {
        List<Employee> lstEmployees =employeeService.getAllEmployees();for(Employee employee : lstEmployees) {if(employee.getName().equals(name))returnemployee;
        }returnnull;
    }![复制代码](https://common.cnblogs.com/images/copycode.gif)
这里的Employ必须事先定义好。
四.使用HttpServletResponse对象的response.getWriter().write(xxx)方法
![复制代码](https://common.cnblogs.com/images/copycode.gif)
@RequestMapping(value="/forbiddenUser")publicvoidforbiddenUser(intid,HttpServletRequest request,HttpServletResponse response) {
        String resultString="{\"result\":\"success\"}";//注意一定是双引号 "{\"result\":true}"try{response.setContentType("application/json");response.getWriter().write(resultString);
        }catch(IOException e) {
            e.printStackTrace();
        }        
    }

