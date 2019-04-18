# Traversing an Array in Node.js - cocoonyang的专栏 - CSDN博客





2017年02月21日 14:33:56[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：281








## Traversing an Array in JavaScript



```java
var array_1 = [1,2,3,4,5,6];

var array_2 = [];
array_2['first'] = 1;
array_2['second'] = 2;
array_2['third'] = 3;
array_2['forth'] = 4;
array_2['fifth'] = 5;
array_2['sixth'] = 6;

console.log( 'array 1 ');
for( var i in array_1 )
{
    console.log( 'index = ', i );
    console.log( 'value = ', array_1[i] );
}

console.log(  );
console.log( 'array 2 ');
for( var i in array_2 )
{
    console.log( 'index = ', i );
    console.log( 'value = ', array_2[i] );
}
```


Results



```
array 1
index =  0
value =  1
index =  1
value =  2
index =  2
value =  3
index =  3
value =  4
index =  4
value =  5
index =  5
value =  6

array 2
index =  first
value =  1
index =  second
value =  2
index =  third
value =  3
index =  forth
value =  4
index =  fifth
value =  5
index =  sixth
value =  6
```


## Insert an element in an ordered array



```java
function insertArray( dataArray, theValue )
{
      var newDataArray = [];
      if( dataArray.length < 1 )
      {
           newDataArray.push( theValue );
           return newDataArray;
      }


      var theIndex = 0;
      var len = dataArray.length;

      for( var index = 0; index < len; index++ )
      {
          var value = dataArray[index]
          if( parseFloat( value ) >  parseFloat( theValue ) )
          {
               newDataArray.push( theValue );
               //newDataArray.push( value );
               theIndex = index;
               break;
          } 
          if( parseFloat( value ) ==  parseFloat( theValue ) )
          {
               theIndex = index;
               break;
           }
          newDataArray.push( value );
      }

      for( var index = theIndex; index < len; index++ )
      {
            var value = dataArray[index]
            newDataArray.push( value );
       }
      return newDataArray;
}
```








## 使用sqlite3_prepare_v2

sqlite3_prepare_v2执行sql select语句，然后sqlite3_step遍历select执行的返回结果




```cpp
#include "sqlite3.h"


void foo()
{
 	int ret = 0;

	sqlite3_stmt *statement;

	ret = sqlite3_prepare(_modelDB, "SELECT * FROM CQUAD4", -1, &statement, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_finalize(statement);
		printf("prepare error ret : %d\n", ret);
		return;
	} 

	while (sqlite3_step(statement) == SQLITE_ROW) {
 
		const char * field = (char *) sqlite3_column_text(statement, 0);
 
		field = (char *) sqlite3_column_text(statement, 1);

		flag = sqlite3_column_int (statement, 2);
		if( true == flag)
		{
			// ...
		}
		
	}

	sqlite3_finalize(statement);
	return;
}
```




Reference 

 [] http://www.zentut.com/c-tutorial/c-array/ 



