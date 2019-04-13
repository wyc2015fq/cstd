
# rtt 打印 float - 嵌入式Linux - CSDN博客

2017年03月03日 18:19:35[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：754



```python
/*rtt print float*/
		float float_value;
    float_value = 3.1415;
    char float_str[80];
    sprintf(float_str, "Float value is %f\n", float_value); 
    NRF_LOG_PRINTF(float_str);
```


