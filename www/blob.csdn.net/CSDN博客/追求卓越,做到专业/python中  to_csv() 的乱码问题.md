
# python中 .to_csv() 的乱码问题 - 追求卓越,做到专业 - CSDN博客


2019年02月15日 14:45:26[Waldenz](https://me.csdn.net/enter89)阅读数：37


**解决方法**：添加参数 encoding='utf_8_sig'
df.to_csv('users.csv', encoding='utf_8_sig')

