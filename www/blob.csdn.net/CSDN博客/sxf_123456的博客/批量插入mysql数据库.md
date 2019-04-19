# 批量插入mysql数据库 - sxf_123456的博客 - CSDN博客
2018年08月28日 17:14:32[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：42标签：[mysql																[pyhton](https://so.csdn.net/so/search/s.do?q=pyhton&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[mysql](https://blog.csdn.net/sxf_123456/article/category/7122238)
def _write(self):
    rows = []
    while True:
        row = self._queue.get()
        if row is None:
            break
        rows.append(row)
        if len(rows) == self._batch:
            self._execute_many(rows)
            rows = []
    if rows:
        self._execute_many(rows)
    self._conn.commit()
def _execute_many(self, rows):
    with self._conn.cursor() as cursor:
        fields = self._fields
        table = self.table
        sql = """INSERT INTO `{table}` ({field}) VALUES ({mark})""".format(
            field='`' + '`, `'.join(fields) + '`',
            mark=', '.join(['%s'] * len(fields)),
            table=table
        )
        cursor.executemany(sql, rows)
