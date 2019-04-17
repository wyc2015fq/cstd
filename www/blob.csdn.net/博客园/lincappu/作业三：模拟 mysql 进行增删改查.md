# 作业三：模拟 mysql 进行增删改查 - lincappu - 博客园







# [作业三：模拟 mysql 进行增删改查](https://www.cnblogs.com/lincappu/p/8143871.html)





```
1 # !/usr/bin/env python3
  2 # _*_coding:utf-8_*_
  3 
  4 
  5 def help_sql(cmd):
  6     if cmd in func_dic.keys():
  7         print('{} usage:\n '.format(cmd))
  8         if cmd == 'select':
  9             print("\tselect * from staff_table;")
 10             print("\tselect name,age from staff_table where age > 22;")
 11             print("\tselect * from staff_table where dept = \"IT\";")
 12             print("\tselect * from staff_table where date like \"2013\";")
 13         elif cmd == 'insert':
 14             print("\tinsert talbe (Alex Li,22,136510546011,IT,2013-04-01);")
 15         elif cmd == "update":
 16             print("\tupdate staff_table set dept = \"Market\" where dept = \"IT\";")
 17         elif cmd == "delete":
 18             print("\tdelete table  5;")
 19     else:
 20         print('your input ERROR!')
 21     return
 22 
 23 
 24 def input_sql():
 25     '''
 26     接受用户输入，做一些判断，转化 list 并一起返回。
 27     :return:sql
 28     '''
 29     exit_flag = False
 30     while exit_flag is not True:
 31         sql = input('sql > ').strip()
 32         sql_list = sql_to_list(sql)
 33         if sql.startswith('help'):
 34             cmd = sql_list[1]
 35             help_sql(cmd)
 36             continue
 37         elif sql == 'q' or sql == 'quit' or sql == 'Q' or sql == 'QUIT' or sql == 'exit' or sql == 'EXIT':
 38             print('已退出程序！')
 39             exit(1)
 40         elif len(sql_list) == 0:
 41             continue
 42         elif len(sql_list) < 2:
 43             continue
 44         elif not (sql.startswith('select') or sql.startswith('insert') or sql.startswith('update') or sql.startswith(
 45                 'delete')):
 46             continue
 47         exit_flag = True
 48     return sql
 49 
 50 
 51 def delete_quotes(str):
 52     '''
 53     删掉用户输入的条件值中带双引号。单引号不用去
 54     :param str:
 55     :return:
 56     '''
 57     if '"' in str:
 58         str = str.strip('"')
 59     return str
 60 
 61 
 62 def sql_to_list(sql, delimiter=' '):
 63     '''
 64     将用户输入的字符串转化为列表的形式，
 65     :param sql:
 66     :param delimiter:
 67     :return:
 68     '''
 69     tmp_list = filter(None, sql.split(delimiter))
 70     sql_list = [item for item in tmp_list]
 71     return sql_list
 72 
 73 
 74 def analyze(sql, table):
 75     '''
 76     分析用户输入sql 语句，作用：
 77     1.拿到 cmd
 78     2.判断表是否存在
 79     3.返回命令和要操作的表
 80     :param sql:
 81     :param sql_list:
 82     :param table:
 83     :return:
 84     '''
 85     sql_list = sql_to_list(sql)
 86     cmd = sql_list[0]
 87     if sql.startswith('select'):
 88         sql_table = sql_list[3]
 89         if sql_table in table:
 90             return cmd, sql_table
 91         else:
 92             return False
 93     if sql.startswith('insert') or sql.startswith('update') or sql.startswith('delete'):
 94         sql_table = sql_list[1]
 95         if sql_table in table:
 96             return cmd, sql_table
 97         else:
 98             return False
 99 
100             # input_list = sql_to_list(sql)
101             # cmd = input_list[0]
102             # input_table = input_list[3]
103             # if input_table in table:
104             #     return cmd, input_table
105             # else:
106             #     return False
107 
108 
109 def file_to_data(sql_table):
110     '''
111     将文件中的数据读到内存中，以遍读取。这个方法是把所有的数据上来就读进内存中。
112     :param sql_table:
113     :return:
114     '''
115     n = 0
116     struct_list = []
117     data_list = []
118     with open(sql_table, 'r', encoding='utf-8') as f:
119         for line in f:
120             if n == 0:
121                 # line = line.strip('\n')
122                 struct_list = line.strip('\n').split(',')
123             else:
124                 # line = line.strip('\n')
125                 line_list = line.strip('\n').split(',')
126                 data_list.append(line_list)
127             n += 1
128     return struct_list, data_list
129 
130 
131 def data_to_file(struct_list, data_list, table):
132     '''
133     将内存中的数据写入到文件里
134     :param struct_list:
135     :param data_list:
136     :param table:
137     :return:
138     '''
139     with open(table, 'w', encoding='utf-8') as f:
140         f.write(','.join(struct_list) + '\n')
141         for sub_list in data_list:
142             f.write(','.join(sub_list) + '\n')
143         print('Write  Done!')
144 
145 
146 def get_cloum_number(colum_name, struct_list):
147     '''
148     获取列所对应的索引，然后用这个索引在数据项里面取值
149     :param colum_name:
150     :param struct_list:
151     :return:
152     '''
153     colum_number = struct_list.index(colum_name)
154     # print(colum_number)
155     return colum_number
156 
157 
158 def auto_increament_id(data_list):
159     '''
160     insert 数据项时，获取新的 id
161     :param data_list:
162     :return:
163     '''
164     current_max_id = int(data_list[-1][0])
165     new_index_id = current_max_id + 1
166     return new_index_id
167 
168 
169 # 增删改查阶段
170 def select(sql, struct_list, data_list, sql_table):
171     sql_list = sql_to_list(sql)
172     select_colum = sql_list[1].split(',')
173     all_cloum = False
174     all_line = False
175     if '*' in select_colum:  # 判断输出那些列
176         all_cloum = True
177     else:
178         cloum_numbers_list = []
179         for cloum_name in select_colum:
180             number = get_cloum_number(cloum_name, struct_list)
181             cloum_numbers_list.append(number)
182 
183     # 关键字过滤
184     select_row = []  # 过滤后的行
185     if 'where' in sql:
186         condition_column_name = sql_list[5]
187         condition_str = sql_list[6]
188         condition_value = sql_list[7]
189         condition_value = delete_quotes(condition_value)
190         condition_column_number = get_cloum_number(condition_column_name, struct_list)
191 
192         if condition_str == '=':
193             for line in data_list:
194                 if line[condition_column_number] == condition_value:
195                     select_row.append(line)
196         if condition_str == '>':
197             for line in data_list:
198                 if line[condition_column_number] > condition_value:
199                     select_row.append(line)
200         if condition_str == '>=':
201             for line in data_list:
202                 if line[condition_column_number] >= condition_value:
203                     select_row.append(line)
204         if condition_str == '<':
205             for line in data_list:
206                 if line[condition_column_number] < condition_value:
207                     select_row.append(line)
208         if condition_str == '<=':
209             for line in data_list:
210                 if line[condition_column_number] <= condition_value:
211                     select_row.append(line)
212         if condition_str == 'like':
213             for line in data_list:
214                 if condition_value in line[condition_column_number]:
215                     select_row.append(line)
216     else:
217         all_line = True
218         select_row = data_list
219 
220     # 输出查询的结果
221     print('select result:')
222     print('#'.center(80, '#'))
223     print('\033[1;31;1m{}\033[0m rows in set'.format(len(select_row)))
224     if all_cloum is True:
225         print('\033[1;34;1m{:<13} {:<13} {:<13} {:<13} {:<13} {:<13}\033[0m'.format(*struct_list))
226         for line in select_row:
227             print('{:<13} {:<13} {:<13} {:<13} {:<13} {:<13}'.format(*line))
228             # print('\t')
229     else:
230         len_num = len(select_colum)
231         format_str1 = '\033[1;34;1m{:13}\033[0m' * len_num
232         format_str2 = '{:13}' * len_num
233         print(format_str1.format(*select_colum))
234         for line in select_row:
235             row_list = []
236             for s in cloum_numbers_list:
237                 row_list.append(line[s])
238             print(format_str2.format(*row_list))
239     print('#'.center(80, '#'))
240     return True
241 
242 
243 def insert(sql, struct_list, data_list, sql_table):
244     '''
245     insert sql : insert   user_info.json  (Liusong Fan,22,13143236545,IT,2015-09-09)
246     insert sql : insert   user_info.json  (Liusong Fan,22,13487654567,IT,2015-09-09)
247     电话号码作为主键。
248     目前只支持插入全部数据项。
249     手机号码必须是11位
250     :param sql:
251     :param struct_list:
252     :param data_list:
253     :return:
254     '''
255     tmp_insert = sql_to_list(sql.strip().strip('insert').strip(), '(')
256     insert_table = tmp_insert[0].strip()
257     insert_info = (tmp_insert[1].strip('(').strip(')'))
258     insert_list = insert_info.split(',')
259 
260     phone_number = insert_list[2]
261     if len(phone_number) != 11:
262         print("Phone number wrong(11 numbers), can't insert")
263         return False
264     phone_exist = False
265     for item in data_list:
266         if phone_number == item[3]:
267             phone_exist = True
268     if phone_exist:
269         print("Phone number is exist, can't insert")
270         return False
271     else:
272         new_index_id = auto_increament_id(data_list)
273         insert_list.insert(0, str(new_index_id))
274         data_list.append(insert_list)
275     data_to_file(struct_list, data_list, insert_table)
276 
277 
278 def delete(sql, struct_list, data_list, sql_table):
279     '''
280     # delete  table 5
281     使用按 id 号删除
282     :param ql:
283     :param struct_list:
284     :param data_list:
285     :param sql_table:
286     :return:
287     '''
288     delete_flag = False
289     delete_list = sql_to_list(sql.strip())
290     delete_index_id = delete_list[2]
291     for item in data_list:
292         if delete_index_id == item[0]:
293             delete_flag = True
294             data_list.remove(item)
295     if not delete_flag:
296         print("ERROE:The delete id is not exist,can't delete")
297         return True
298     else:
299         data_to_file(struct_list, data_list, sql_table)
300 
301 
302 def update(sql, struct_list, data_list, sql_table):
303     '''
304     update user_info.json set dept = "Market" where id = "1"
305     set 的值和 where 的值要加"号
306     id 是自增的不允许修改，phone 是主键，不允许重复
307     set 和 where 只支持 =
308     :param sql:
309     :param struct_list:
310     :param data_list:
311     :param sql_table:
312     :return:
313     '''
314     update_list = sql_to_list(sql)
315     set_flag = update_list[2]
316     modify_column = update_list[3]
317     equal_flag = update_list[4]
318     modify_value = update_list[5]
319     modify_value = delete_quotes(modify_value)
320     where_flag = update_list[6]
321     condition_column = update_list[7]
322     condition_str = update_list[8]
323     condition_value = update_list[9]
324     condition_value = delete_quotes(condition_value)
325     modify_column_number = get_cloum_number(modify_column, struct_list)
326     condition_column_number = get_cloum_number(condition_column, struct_list)
327 
328     modify_flag = False
329     if modify_column == 'id':
330         print("ERROR: you can't modify id column value")
331         return False
332     else:
333         if set_flag == 'set' and equal_flag == '=' and where_flag == 'where' and condition_str == '=':
334             phone_exists = False
335             phone_number = modify_value
336             for item in data_list:
337                 if phone_number == item[3]:
338                     phone_exists = True
339             if phone_exists:
340                 print("ERROR: you input phone number is exists, can't update")
341                 return False
342             for d_list in data_list:
343                 if d_list[condition_column_number] == condition_value:
344                     d_list[modify_column_number] = modify_value
345                     modify_flag = True
346             if modify_flag:
347                 data_to_file(struct_list, data_list, sql_table)
348             else:
349                 print("ERROR: no match records")
350         else:
351             print("ERROR: you upate sql error, please input 'help update' get help")
352 
353 
354 func_dic = {
355     '''
356     要操作的数据类型
357     '''
358     'select': select,
359     'update': update,
360     'insert': insert,
361     'delete': delete,
362 }
363 
364 
365 def main():
366     exit_flag = False
367     table = ['user_info.json']
368     print('-'.center(60, '-'))
369     print('Please input【help [select/update/insert/delete] to get help.')
370     print('-'.center(60, '-'))
371     while exit_flag is not True:
372         sql = input_sql()
373         res = analyze(sql, table)
374         if not res:
375             print('ERROR: your input sql table is not exists!')
376             continue
377         else:
378             cmd = res[0]
379             sql_table = res[1]
380         struct_list, data_list = file_to_data(sql_table)
381         if cmd in func_dic.keys():
382             res = func_dic[cmd](sql, struct_list, data_list, sql_table)
383             if not res:
384                 continue
385         else:
386             print('ERROR:  your command!')
387 
388 
389 if __name__ == '__main__':
390     main()
```














