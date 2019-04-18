# MYSQL 5.7 新增 JSON 方面的系统函数支持 - z69183787的专栏 - CSDN博客
2018年04月05日 12:24:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1773
[https://dev.mysql.com/doc/refman/5.7/en/json-function-reference.html](https://dev.mysql.com/doc/refman/5.7/en/json-function-reference.html)
select PageInfo->'$.utmSource', JSON_UNQUOTE(json_extract(PageInfo,'$.utmSource')) from BLP_WinningRecord where id =140455331
![](https://img-blog.csdn.net/20180405210016922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
JSON_UNQUOTE：去除提取后多余的引号
json_extract（jsonStr,'$.key'） 等同于 jsonStr->'$.key'
JSON_UNQUOTE(json_extract（jsonStr,'$.key'）)   等同于 jsonStr->>'$.key' （两个箭头）
### Creating JSON Values
A JSON array contains a list of values separated by commas and enclosed within `[` and `]` characters:
`["abc", 10, null, true, false]`
A JSON object contains a set of key-value pairs separated by commas and enclosed within `{` and `}` characters:
`{"k1": "value", "k2": 10}`
As the examples illustrate, JSON arrays and objects can contain scalar values that are strings or numbers, the JSON null literal, or the JSON boolean true or false literals. Keys in JSON objects must be strings. Temporal (date, time, or datetime) scalar values are also permitted:
`["12:18:29.000000", "2015-07-29", "2015-07-29 12:18:29.000000"]`
Nesting is permitted within JSON array elements and JSON object key values:
```
[99, {"id": "HK500", "cost": 75.99}, ["hot", "cold"]]
{"k1": "value", "k2": [10, 20]}
```
You can also obtain JSON values from a number of functions supplied by MySQL for this purpose (see [Section 12.16.2, “Functions That Create JSON Values”](https://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html)) as well as by casting values of other types to the `JSON` type using [`CAST(`value` AS JSON)`](https://dev.mysql.com/doc/refman/5.7/en/cast-functions.html#function_cast) (see [Converting between JSON and non-JSON values](https://dev.mysql.com/doc/refman/5.7/en/json.html#json-converting-between-types)). The next several paragraphs describe how MySQL handles JSON values provided as input.
In MySQL, JSON values are written as strings. MySQL parses any string used in a context that requires a JSON value, and produces an error if it is not valid as JSON. These contexts include inserting a value into a column that has the `JSON` data type and passing an argument to a function that expects a JSON value (usually shown as`json_doc` or `json_val` in the documentation for MySQL JSON functions), as the following examples demonstrate:
- 
Attempting to insert a value into a `JSON` column succeeds if the value is a valid JSON value, but fails if it is not:
```
```
mysql> CREATE TABLE t1 (jdoc JSON);
Query OK, 0 rows affected (0.20 sec)
mysql> INSERT INTO t1 VALUES('{"key1": "value1", "key2": "value2"}');
Query OK, 1 row affected (0.01 sec)
mysql> INSERT INTO t1 VALUES('[1, 2,');
ERROR 3140 (22032) at line 2: Invalid JSON text: 
"Invalid value." at position 6 in value (or column) '[1, 2,'.
```
```
Positions for “at position `N`” in such error messages are 0-based, but should be considered rough indications of where the problem in a value actually occurs.
- 
The [`JSON_TYPE()`](https://dev.mysql.com/doc/refman/5.7/en/json-attribute-functions.html#function_json-type) function expects a JSON argument and attempts to parse it into a JSON value. It returns the value's JSON type if it is valid and produces an error otherwise:
```
```
mysql> SELECT JSON_TYPE('["a", "b", 1]');
+----------------------------+
| JSON_TYPE('["a", "b", 1]') |
+----------------------------+
| ARRAY                      |
+----------------------------+
mysql> SELECT JSON_TYPE('"hello"');
+----------------------+
| JSON_TYPE('"hello"') |
+----------------------+
| STRING               |
+----------------------+
mysql> SELECT JSON_TYPE('hello');
ERROR 3146 (22032): Invalid data type for JSON data in argument 1
to function json_type; a JSON string or JSON type is required.
```
```
MySQL handles strings used in JSON context using the `utf8mb4` character set and `utf8mb4_bin` collation. Strings in other character sets are converted to `utf8mb4` as necessary. (For strings in the `ascii` or `utf8` character sets, no conversion is needed because `ascii` and `utf8` are subsets of `utf8mb4`.)
As an alternative to writing JSON values using literal strings, functions exist for composing JSON values from component elements. [`JSON_ARRAY()`](https://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html#function_json-array) takes a (possibly empty) list of values and returns a JSON array containing those values:
```
```
mysql> SELECT JSON_ARRAY('a', 1, NOW());
+----------------------------------------+
| JSON_ARRAY('a', 1, NOW())              |
+----------------------------------------+
| ["a", 1, "2015-07-27 09:43:47.000000"] |
+----------------------------------------+
```
```
[`JSON_OBJECT()`](https://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html#function_json-object) takes a (possibly empty) list of key-value pairs and returns a JSON object containing those pairs:
```
```
mysql> SELECT JSON_OBJECT('key1', 1, 'key2', 'abc');
+---------------------------------------+
| JSON_OBJECT('key1', 1, 'key2', 'abc') |
+---------------------------------------+
| {"key1": 1, "key2": "abc"}            |
+---------------------------------------+
```
```
[`JSON_MERGE()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-merge) takes two or more JSON documents and returns the combined result:
```
```
mysql> SELECT JSON_MERGE('["a", 1]', '{"key": "value"}');
+--------------------------------------------+
| JSON_MERGE('["a", 1]', '{"key": "value"}') |
+--------------------------------------------+
| ["a", 1, {"key": "value"}]                 |
+--------------------------------------------+
```
```
For information about the merging rules, see [Normalization, Merging, and Autowrapping of JSON Values](https://dev.mysql.com/doc/refman/5.7/en/json.html#json-normalization).
JSON values can be assigned to user-defined variables:
```
```
mysql> SET @j = JSON_OBJECT('key', 'value');
mysql> SELECT @j;
+------------------+
| @j               |
+------------------+
| {"key": "value"} |
+------------------+
```
```
However, user-defined variables cannot be of `JSON` data type, so although `@j` in the preceding example looks like a JSON value and has the same character set and collation as a JSON value, it does not have the `JSON` data type. Instead, the result from [`JSON_OBJECT()`](https://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html#function_json-object) is converted to a string when assigned to the variable.
Strings produced by converting JSON values have a character set of `utf8mb4` and a collation of `utf8mb4_bin`:
```
```
mysql> SELECT CHARSET(@j), COLLATION(@j);
+-------------+---------------+
| CHARSET(@j) | COLLATION(@j) |
+-------------+---------------+
| utf8mb4     | utf8mb4_bin   |
+-------------+---------------+
```
```
Because `utf8mb4_bin` is a binary collation, comparison of JSON values is case-sensitive.
```
```
mysql> SELECT JSON_ARRAY('x') = JSON_ARRAY('X');
+-----------------------------------+
| JSON_ARRAY('x') = JSON_ARRAY('X') |
+-----------------------------------+
|                                 0 |
+-----------------------------------+
```
```
Case sensitivity also applies to the JSON `null`, `true`, and `false` literals, which always must be written in lowercase:
```
```
mysql> SELECT JSON_VALID('null'), JSON_VALID('Null'), JSON_VALID('NULL');
+--------------------+--------------------+--------------------+
| JSON_VALID('null') | JSON_VALID('Null') | JSON_VALID('NULL') |
+--------------------+--------------------+--------------------+
|                  1 |                  0 |                  0 |
+--------------------+--------------------+--------------------+
mysql> SELECT CAST('null' AS JSON);
+----------------------+
| CAST('null' AS JSON) |
+----------------------+
| null                 |
+----------------------+
1 row in set (0.00 sec)
mysql> SELECT CAST('NULL' AS JSON);
ERROR 3141 (22032): Invalid JSON text in argument 1 to function cast_as_json:
"Invalid value." at position 0 in 'NULL'.
```
```
Case sensitivity of the JSON literals differs from that of the SQL `NULL`, `TRUE`, and `FALSE` literals, which can be written in any lettercase:
```
```
mysql> SELECT ISNULL(null), ISNULL(Null), ISNULL(NULL);
+--------------+--------------+--------------+
| ISNULL(null) | ISNULL(Null) | ISNULL(NULL) |
+--------------+--------------+--------------+
|            1 |            1 |            1 |
+--------------+--------------+--------------+
```
```
Sometimes it may be necessary or desirable to insert quote characters (`"` or `'`) into a JSON document. Assume for this example that you want to insert some JSON objects containing strings representing sentences that state some facts about MySQL, each paired with an appropriate keyword, into a table created using the SQL statement shown here:
``mysql> CREATE TABLE facts (sentence JSON);``
Among these keyword-sentence pairs is this one:
`mascot: The MySQL mascot is a dolphin named "Sakila".`
One way to insert this as a JSON object into the `facts` table is to use the MySQL [`JSON_OBJECT()`](https://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html#function_json-object) function. In this case, you must escape each quote character using a backslash, as shown here:
```
```
mysql> INSERT INTO facts VALUES 
     >   (JSON_OBJECT("mascot", "Our mascot is a dolphin named \"Sakila\"."));
```
```
This does not work in the same way if you insert the value as a JSON object literal, in which case, you must use the double backslash escape sequence, like this:
```
```
mysql> INSERT INTO facts VALUES 
     >   ('{"mascot": "Our mascot is a dolphin named \\"Sakila\\"."}');
```
```
Using the double backslash keeps MySQL from performing escape sequence processing, and instead causes it to pass the string literal to the storage engine for processing. After inserting the JSON object in either of the ways just shown, you can see that the backslashes are present in the JSON column value by doing a simple [`SELECT`](https://dev.mysql.com/doc/refman/5.7/en/select.html), like this:
```
```
mysql> SELECT sentence FROM facts;
+---------------------------------------------------------+
| sentence                                                |
+---------------------------------------------------------+
| {"mascot": "Our mascot is a dolphin named \"Sakila\"."} |
+---------------------------------------------------------+
```
```
To look up this particular sentence employing `mascot` as the key, you can use the column-path operator [`->`](https://dev.mysql.com/doc/refman/5.7/en/json-search-functions.html#operator_json-column-path), as shown here:
```
```
mysql> SELECT col->"$.mascot" FROM qtest;
+---------------------------------------------+
| col->"$.mascot"                             |
+---------------------------------------------+
| "Our mascot is a dolphin named \"Sakila\"." |
+---------------------------------------------+
1 row in set (0.00 sec)
```
```
This leaves the backslashes intact, along with the surrounding quote marks. To display the desired value using `mascot` as the key, but without including the surrounding quote marks or any escapes, use the inline path operator [`->>`](https://dev.mysql.com/doc/refman/5.7/en/json-search-functions.html#operator_json-inline-path), like this:
```
```
mysql> SELECT sentence->>"$.mascot" FROM facts;
+-----------------------------------------+
| sentence->>"$.mascot"                   |
+-----------------------------------------+
| Our mascot is a dolphin named "Sakila". |
+-----------------------------------------+
```
```
Note
The previous example does not work as shown if the [`NO_BACKSLASH_ESCAPES`](https://dev.mysql.com/doc/refman/5.7/en/sql-mode.html#sqlmode_no_backslash_escapes) server SQL mode is enabled. If this mode is set, a single backslash instead of double backslashes can be used to insert the JSON object literal, and the backslashes are preserved. If you use the `JSON_OBJECT()`function when performing the insert and this mode is set, you must alternate single and double quotes, like this:
```
```
mysql> INSERT INTO facts VALUES 
     > (JSON_OBJECT('mascot', 'Our mascot is a dolphin named "Sakila".'));
```
```
See the description of the [`JSON_UNQUOTE()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-unquote) function for more information about the effects of this mode on escaped characters in JSON values.
### Normalization, Merging, and Autowrapping of JSON Values
When a string is parsed and found to be a valid JSON document, it is also normalized: Members with keys that duplicate a key found earlier in the document are discarded (even if the values differ). The object value produced by the following [`JSON_OBJECT()`](https://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html#function_json-object) call does not include the second `key1` element because that key name occurs earlier in the value:
```
```
mysql> SELECT JSON_OBJECT('key1', 1, 'key2', 'abc', 'key1', 'def');
+------------------------------------------------------+
| JSON_OBJECT('key1', 1, 'key2', 'abc', 'key1', 'def') |
+------------------------------------------------------+
| {"key1": 1, "key2": "abc"}                           |
+------------------------------------------------------+
```
```
Note
This “first key wins” handling of duplicate keys is not consistent with [RFC 7159](https://tools.ietf.org/html/rfc7159). This is a known issue in MySQL 5.7, which is fixed in MySQL 8.0. (Bug #86866, Bug #26369555)
MySQL also discards extra whitespace between keys, values, or elements in the original JSON document. To make lookups more efficient, it also sorts the keys of a JSON object. You should be aware that the result of this ordering is subject to change and not guaranteed to be consistent across releases.
MySQL functions that produce JSON values (see [Section 12.16.2, “Functions That Create JSON Values”](https://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html)) always return normalized values.
#### Merging JSON Values
In contexts that combine multiple arrays, the arrays are merged into a single array by concatenating arrays named later to the end of the first array. In the following example, [`JSON_MERGE()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-merge) merges its arguments into a single array:
```
```
mysql> SELECT JSON_MERGE('[1, 2]', '["a", "b"]', '[true, false]');
+-----------------------------------------------------+
| JSON_MERGE('[1, 2]', '["a", "b"]', '[true, false]') |
+-----------------------------------------------------+
| [1, 2, "a", "b", true, false]                       |
+-----------------------------------------------------+
```
```
Normalization is also performed when values are inserted into JSON columns, as shown here:
```
```
mysql> CREATE TABLE t1 (c1 JSON);
mysql> INSERT INTO t1 VALUES 
     >     ('{"x": 17, "x": "red"}'),
     >     ('{"x": 17, "x": "red", "x": [3, 5, 7]}');
mysql> SELECT c1 FROM t1;
+-----------+
| c1        |
+-----------+
| {"x": 17} |
| {"x": 17} |
+-----------+
```
```
Multiple objects when merged produce a single object. If multiple objects have the same key, the value for that key in the resulting merged object is an array containing the key values:
```
```
mysql> SELECT JSON_MERGE('{"a": 1, "b": 2}', '{"c": 3, "a": 4}');
+----------------------------------------------------+
| JSON_MERGE('{"a": 1, "b": 2}', '{"c": 3, "a": 4}') |
+----------------------------------------------------+
| {"a": [1, 4], "b": 2, "c": 3}                      |
+----------------------------------------------------+
```
```
Nonarray values used in a context that requires an array value are autowrapped: The value is surrounded by `[` and `]` characters to convert it to an array. In the following statement, each argument is autowrapped as an array (`[1]`, `[2]`). These are then merged to produce a single result array:
```
```
mysql> SELECT JSON_MERGE('1', '2');
+----------------------+
| JSON_MERGE('1', '2') |
+----------------------+
| [1, 2]               |
+----------------------+
```
```
Array and object values are merged by autowrapping the object as an array and merging the two arrays:
```
```
mysql> SELECT JSON_MERGE('[10, 20]', '{"a": "x", "b": "y"}');
+------------------------------------------------+
| JSON_MERGE('[10, 20]', '{"a": "x", "b": "y"}') |
+------------------------------------------------+
| [10, 20, {"a": "x", "b": "y"}]                 |
+------------------------------------------------+
```
```
### Searching and Modifying JSON Values
A JSON path expression selects a value within a JSON document.
Path expressions are useful with functions that extract parts of or modify a JSON document, to specify where within that document to operate. For example, the following query extracts from a JSON document the value of the member with the `name` key:
```
```
mysql> SELECT JSON_EXTRACT('{"id": 14, "name": "Aztalan"}', '$.name');
+---------------------------------------------------------+
| JSON_EXTRACT('{"id": 14, "name": "Aztalan"}', '$.name') |
+---------------------------------------------------------+
| "Aztalan"                                               |
+---------------------------------------------------------+
```
```
Path syntax uses a leading `$` character to represent the JSON document under consideration, optionally followed by selectors that indicate successively more specific parts of the document:
- 
A period followed by a key name names the member in an object with the given key. The key name must be specified within double quotation marks if the name without quotes is not legal within path expressions (for example, if it contains a space).
- 
`[`N`]` appended to a `path` that selects an array names the value at position `N` within the array. Array positions are integers beginning with zero. If `path` does not select an array value, `path`[0] evaluates to the same value as `path`:
```
```
mysql> SELECT JSON_SET('"x"', '$[0]', 'a');
+------------------------------+
| JSON_SET('"x"', '$[0]', 'a') |
+------------------------------+
| "a"                          |
+------------------------------+
1 row in set (0.00 sec)
```
```
- 
Paths can contain `*` or `**` wildcards:
- 
`.[*]` evaluates to the values of all members in a JSON object.
- 
`[*]` evaluates to the values of all elements in a JSON array.
- 
``prefix`**`suffix`` evaluates to all paths that begin with the named prefix and end with the named suffix.
- 
A path that does not exist in the document (evaluates to nonexistent data) evaluates to `NULL`.
Let `$` refer to this JSON array with three elements:
`[3, {"a": [5, 6], "b": 10}, [99, 100]]`
Then:
- 
`$[0]` evaluates to `3`.
- 
`$[1]` evaluates to `{"a": [5, 6], "b": 10}`.
- 
`$[2]` evaluates to `[99, 100]`.
- 
`$[3]` evaluates to `NULL` (it refers to the fourth array element, which does not exist).
Because `$[1]` and `$[2]` evaluate to nonscalar values, they can be used as the basis for more-specific path expressions that select nested values. Examples:
- 
`$[1].a` evaluates to `[5, 6]`.
- 
`$[1].a[1]` evaluates to `6`.
- 
`$[1].b` evaluates to `10`.
- 
`$[2][0]` evaluates to `99`.
As mentioned previously, path components that name keys must be quoted if the unquoted key name is not legal in path expressions. Let `$` refer to this value:
`{"a fish": "shark", "a bird": "sparrow"}`
The keys both contain a space and must be quoted:
- 
`$."a fish"` evaluates to `shark`.
- 
`$."a bird"` evaluates to `sparrow`.
Paths that use wildcards evaluate to an array that can contain multiple values:
```
```
mysql> SELECT JSON_EXTRACT('{"a": 1, "b": 2, "c": [3, 4, 5]}', '$.*');
+---------------------------------------------------------+
| JSON_EXTRACT('{"a": 1, "b": 2, "c": [3, 4, 5]}', '$.*') |
+---------------------------------------------------------+
| [1, 2, [3, 4, 5]]                                       |
+---------------------------------------------------------+
mysql> SELECT JSON_EXTRACT('{"a": 1, "b": 2, "c": [3, 4, 5]}', '$.c[*]');
+------------------------------------------------------------+
| JSON_EXTRACT('{"a": 1, "b": 2, "c": [3, 4, 5]}', '$.c[*]') |
+------------------------------------------------------------+
| [3, 4, 5]                                                  |
+------------------------------------------------------------+
```
```
In the following example, the path `$**.b` evaluates to multiple paths (`$.a.b` and `$.c.b`) and produces an array of the matching path values:
```
```
mysql> SELECT JSON_EXTRACT('{"a": {"b": 1}, "c": {"b": 2}}', '$**.b');
+---------------------------------------------------------+
| JSON_EXTRACT('{"a": {"b": 1}, "c": {"b": 2}}', '$**.b') |
+---------------------------------------------------------+
| [1, 2]                                                  |
+---------------------------------------------------------+
```
```
In MySQL 5.7.9 and later, you can use [``column`->`path``](https://dev.mysql.com/doc/refman/5.7/en/json-search-functions.html#operator_json-column-path) with a JSON column identifier and JSON path expression as a synonym for [`JSON_EXTRACT(`column`, `path`)`](https://dev.mysql.com/doc/refman/5.7/en/json-search-functions.html#function_json-extract). See[Section 12.16.3, “Functions That Search JSON Values”](https://dev.mysql.com/doc/refman/5.7/en/json-search-functions.html), for more information. See also [Indexing a Generated Column to Provide a JSON Column Index](https://dev.mysql.com/doc/refman/5.7/en/create-table-secondary-indexes.html#json-column-indirect-index).
Some functions take an existing JSON document, modify it in some way, and return the resulting modified document. Path expressions indicate where in the document to make changes. For example, the [`JSON_SET()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-set), [`JSON_INSERT()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-insert), and [`JSON_REPLACE()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-replace) functions each take a JSON document, plus one or more path/value pairs that describe where to modify the document and the values to use. The functions differ in how they handle existing and nonexisting values within the document.
Consider this document:
``mysql> SET @j = '["a", {"b": [true, false]}, [10, 20]]';``
[`JSON_SET()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-set) replaces values for paths that exist and adds values for paths that do not exist:.
```
```
mysql> SELECT JSON_SET(@j, '$[1].b[0]', 1, '$[2][2]', 2);
+--------------------------------------------+
| JSON_SET(@j, '$[1].b[0]', 1, '$[2][2]', 2) |
+--------------------------------------------+
| ["a", {"b": [1, false]}, [10, 20, 2]]      |
+--------------------------------------------+
```
```
In this case, the path `$[1].b[0]` selects an existing value (`true`), which is replaced with the value following the path argument (`1`). The path `$[2][2]` does not exist, so the corresponding value (`2`) is added to the value selected by `$[2]`.
[`JSON_INSERT()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-insert) adds new values but does not replace existing values:
```
```
mysql> SELECT JSON_INSERT(@j, '$[1].b[0]', 1, '$[2][2]', 2);
+-----------------------------------------------+
| JSON_INSERT(@j, '$[1].b[0]', 1, '$[2][2]', 2) |
+-----------------------------------------------+
| ["a", {"b": [true, false]}, [10, 20, 2]]      |
+-----------------------------------------------+
```
```
[`JSON_REPLACE()`](https://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html#function_json-replace) replaces existing values and ignores new values:
```
```
mysql> SELECT JSON_REPLACE(@j, '$[1].b[0]', 1, '$[2][2]', 2);
+------------------------------------------------+
| JSON_REPLACE(@j, '$[1].b[0]', 1, '$[2][2]', 2) |
+------------------------------------------------+
| ["a", {"b": [1, false]}, [10, 20]]             |
+------------------------------------------------+
```
```
The path/value pairs are evaluated left to right. The document produced by evaluating one pair becomes the new value against which the next pair is evaluated.
`JSON_REMOVE()` takes a JSON document and one or more paths that specify values to be removed from the document. The return value is the original document minus the values selected by paths that exist within the document:
`mysql> SELECT JSON_REMOVE(@j, '$[2]', '$[1].b[1]', '$[1].b[1]');+---------------------------------------------------+| JSON_REMOVE(@j, '$[2]', '$[1].b[1]', '$[1].b[1]') |+---------------------------------------------------+| ["a", {"b": [true]}]                              |+---------------------------------------------------+`
从 MySQL 5.7.8 开始，MySQL 支持原生的 JSON 数据类型。
创建 JSON
类似 varchar，设置 JSON 主要将字段的 type 是 json, 不能设置长度，可以是 NULL  但不能有默认值。
mysql> CREATE TABLE lnmp (
    `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
    `category` JSON,
    `tags` JSON,
    PRIMARY KEY (`id`)
);
mysql> DESC lnmp;
+----------+------------------+------+-----+---------+----------------+
| Field    | Type             | Null | Key | Default | Extra          |
+----------+------------------+------+-----+---------+----------------+
| id       | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
| category | json             | YES  |     | NULL    |                |
| tags     | json             | YES  |     | NULL    |                |
+----------+------------------+------+-----+---------+----------------+
3 rows in set (0.00 sec)
这样 JSON 的字段就创建好了。
插入 JSON
就是插入 json 格式的字符串，可以是对象的形式，也可以是数组的形式
mysql> INSERT INTO `lnmp` (category, tags) VALUES ('{"id": 1, "name": "lnmp.cn"}', '[1, 2, 3]');
Query OK, 1 row affected (0.01 sec)
MySQL 也有专门的函数 JSON_OBJECT，JSON_ARRAY 生成 json 格式的数据
mysql> INSERT INTO `lnmp` (category, tags) VALUES (JSON_OBJECT("id", 2, "name", "php.net"), JSON_ARRAY(1, 3, 5));
Query OK, 1 row affected (0.00 sec)
查看插入的数据
mysql> SELECT * FROM lnmp;
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 2, 3] |
|  2 | {"id": 2, "name": "php.net"} | [1, 3, 5] |
+----+------------------------------+-----------+
2 rows in set (0.00 sec)
更多生成 JSON 值的函数请参考： http://dev.mysql.com/doc/refman/5.7/en/json-creation-functions.html
查询 JSON
查询 json 中的数据用 column->path 的形式，其中对象类型 path 这样表示 $.path, 而数组类型则是 $[index]
mysql> SELECT id, category->'$.id', category->'$.name', tags->'$[0]', tags->'$[2]' FROM lnmp;
+----+------------------+--------------------+--------------+--------------+
| id | category->'$.id' | category->'$.name' | tags->'$[0]' | tags->'$[2]' |
+----+------------------+--------------------+--------------+--------------+
|  1 | 1                | "lnmp.cn"          | 1            | 3            |
|  2 | 2                | "php.net"          | 1            | 5            |
+----+------------------+--------------------+--------------+--------------+
2 rows in set (0.00 sec)
可以看到对应字符串类型的 category->'$.name' 中还包含着双引号，这其实并不是想要的结果，可以用 JSON_UNQUOTE 函数将双引号去掉，从 MySQL 5.7.13 起也可以通过这个操作符 ->> 这个和 JSON_UNQUOTE 是等价的
mysql> SELECT id, category->'$.name', JSON_UNQUOTE(category->'$.name'), category->>'$.name' FROM lnmp;
+----+--------------------+----------------------------------+---------------------+
| id | category->'$.name' | JSON_UNQUOTE(category->'$.name') | category->>'$.name' |
+----+--------------------+----------------------------------+---------------------+
|  1 | "lnmp.cn"          | lnmp.cn                          | lnmp.cn             |
|  2 | "php.net"          | php.net                          | php.net             |
+----+--------------------+----------------------------------+---------------------+
2 rows in set (0.00 sec)
下面说下 JSON 作为条件进行搜索。因为 JSON 不同于字符串，所以如果用字符串和 JSON 字段比较，是不会相等的
mysql> SELECT * FROM lnmp WHERE category = '{"id": 1, "name": "lnmp.cn"}';
Empty set (0.00 sec)
这时可以通过 CAST 将字符串转成 JSON 的形式
mysql> SELECT * FROM lnmp WHERE category = CAST('{"id": 1, "name": "lnmp.cn"}' as JSON);
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 2, 3] |
+----+------------------------------+-----------+
1 row in set (0.00 sec)
通过 JSON 中的元素进行查询, 对象型的查询同样可以通过 column->path
mysql> SELECT * FROM lnmp WHERE category->'$.name' = 'lnmp.cn';
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 2, 3] |
+----+------------------------------+-----------+
1 row in set (0.00 sec)
上面有提到 column->path 形式从 select 中查询出来的字符串是包含双引号的，但作为条件这里其实没什么影响，-> 和 ->> 结果是一样的
mysql> SELECT * FROM lnmp WHERE category->>'$.name' = 'lnmp.cn';
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 2, 3] |
+----+------------------------------+-----------+
1 row in set (0.00 sec)
要特别注意的是，JSON 中的元素搜索是严格区分变量类型的，比如说整型和字符串是严格区分的
mysql> SELECT * FROM lnmp WHERE category->'$.id' = '1';
Empty set (0.00 sec)
mysql> SELECT * FROM lnmp WHERE category->'$.id' = 1;
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 2, 3] |
+----+------------------------------+-----------+
1 row in set (0.00 sec)
可以看到搜索字符串 1 和整型 1 的结果是不一样的。
除了用 column->path 的形式搜索，还可以用JSON_CONTAINS 函数，但和 column->path 的形式有点相反的是，JSON_CONTAINS 第二个参数是不接受整数的，无论 json 元素是整型还是字符串，否则会出现这个错误
mysql> SELECT * FROM lnmp WHERE JSON_CONTAINS(category, 1, '$.id');
ERROR 3146 (22032): Invalid data type for JSON data in argument 2 to function json_contains; a JSON string or JSON type is required.
这里必须是要字符串 1
mysql> SELECT * FROM lnmp WHERE JSON_CONTAINS(category, '1', '$.id');
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 2, 3] |
+----+------------------------------+-----------+
1 row in set (0.01 sec)
对于数组类型的 JSON 的查询，比如说 tags 中包含有 2 的数据，同样要用 JSON_CONTAINS 函数，同样第二个参数也需要是字符串
mysql> SELECT * FROM lnmp WHERE JSON_CONTAINS(tags, '2');
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 2, 3] |
+----+------------------------------+-----------+
1 row in set (0.00 sec)
更多搜索 JSON 值的函数请参考：http://dev.mysql.com/doc/refman/5.7/en/json-search-functions.html
更新 JSON
如果是整个 json 更新的话，和插入时类似的。
mysql> UPDATE lnmp SET tags = '[1, 3, 4]' WHERE id = 1;
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> SELECT * FROM lnmp;
+----+------------------------------+-----------+ 
| id | category                     | tags      | 
+----+------------------------------+-----------+ 
| 1  | {"id": 1, "name": "lnmp.cn"} | [1, 3, 4] | 
| 2  | {"id": 2, "name": "php.net"} | [1, 3, 5] |
+----+------------------------------+-----------+
2 rows in set (0.00 sec)
但如果要更新 JSON 下的元素，MySQL 并不支持 column->path 的形式
mysql> UPDATE lnmp SET category->'$.name' = 'lnmp', tags->'$[0]' = 2 WHERE id = 1;
ERROR 1064 (42000): You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '->'$.name' = 'lnmp', tags->'$[0]' = 2 WHERE id = 1' at line 1
则可能要用到以下几个函数
JSON_INSERT() 插入新值，但不会覆盖已经存在的值
mysql> UPDATE lnmp SET category = JSON_INSERT(category, '$.name', 'lnmp', '$.url', 'www.lnmp.cn') WHERE id = 1;
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> SELECT * FROM lnmp;
+----+----------------------------------------------------+-----------+
| id | category                                           | tags      |
+----+----------------------------------------------------+-----------+
|  1 | {"id": 1, "url": "www.lnmp.cn", "name": "lnmp.cn"} | [1, 3, 4] |
|  2 | {"id": 2, "name": "php.net"}                       | [1, 3, 5] |
+----+----------------------------------------------------+-----------+
2 rows in set (0.00 sec)
可以看到 name 没有被修改，但新元素 url 已经添加进去
JSON_SET() 插入新值，并覆盖已经存在的值
mysql> UPDATE lnmp SET category = JSON_SET(category, '$.host', 'www.lnmp.cn', '$.url', 'http://www.lnmp.cn') WHERE id = 1;
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> SELECT * FROM lnmp;
+----+----------------------------------------------------------------------------------+-----------+
| id | category                                                                         | tags      |
+----+----------------------------------------------------------------------------------+-----------+
|  1 | {"id": 1, "url": "http://www.lnmp.cn", "host": "www.lnmp.cn", "name": "lnmp.cn"} | [1, 3, 4] |
|  2 | {"id": 2, "name": "php.net"}                                                     | [1, 3, 5] |
+----+----------------------------------------------------------------------------------+-----------+
2 rows in set (0.00 sec)
可以看到 host 已经插入，url 已经被修改
JSON_REPLACE() 只替换存在的值
mysql> UPDATE lnmp SET category = JSON_REPLACE(category, '$.name', 'php', '$.url', 'http://www.php.net') WHERE id = 2;
Query OK, 1 row affected (0.01 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> SELECT * FROM lnmp;
+----+----------------------------------------------------------------------------------+-----------+
| id | category                                                                         | tags      |
+----+----------------------------------------------------------------------------------+-----------+
|  1 | {"id": 1, "url": "http://www.lnmp.cn", "host": "www.lnmp.cn", "name": "lnmp.cn"} | [1, 3, 4] |
|  2 | {"id": 2, "name": "php"}                                                         | [1, 3, 5] |
+----+----------------------------------------------------------------------------------+-----------+
2 rows in set (0.00 sec)
可以看到 name 已经被替换，url 不存在被忽略。
JSON_REMOVE() 删除 JSON 元素
mysql> UPDATE lnmp SET category = JSON_REMOVE(category, '$.url', '$.host') WHERE id = 1;
Query OK, 1 row affected (0.01 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> SELECT * FROM lnmp;
+----+------------------------------+-----------+
| id | category                     | tags      |
+----+------------------------------+-----------+
|  1 | {"id": 1, "name": "lnmp.cn"} | [1, 3, 4] |
|  2 | {"id": 2, "name": "php"}     | [1, 3, 5] |
+----+------------------------------+-----------+
2 rows in set (0.00 sec)
更多函数请参考：http://dev.mysql.com/doc/refman/5.7/en/json-modification-functions.html
MySQL JSON 在 PHP 中的表现
虽然在 MySQL 是个JSON 类型，但实际在 PHP 应用中返回的是 JSON 格式的字符串
array(2) {
  [0]=>
  array(3) {
    ["id"]=>
    string(1) "1"
    ["category"]=>
    string(28) "{"id": 1, "name": "lnmp.cn"}"
    ["tags"]=>
    string(9) "[1, 3, 4]"
  }
  [1]=>
  array(3) {
    ["id"]=>
    string(1) "2"
    ["category"]=>
    string(24) "{"id": 2, "name": "php"}"
    ["tags"]=>
    string(9) "[1, 3, 5]"
  }
}
