# mysql触发器用法详解 - fanyun的博客 - CSDN博客
2016年04月12日 13:34:15[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3642
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
MySQL触发器语法详解:
**触发器 trigger是一种特殊的存储过程**，他在插入（inset）、删除（delete）或修改（update）特定表中的数据时触发执行，它比数据本身标准的功能更精细和更复杂的数据控制能力。触发器不是由程序调用，而是由某个事件来触发的。在有数据修改时自动强制执行其业务规则，经常用于加强数据的完整性约束和业务规则等。触发器可以查询其他表，而且包含复制的sql语句。触发器也可用于强制引用完整性。触发器可以强制比用check约束定义的约束更为复杂的约束。
**(一).CREATE TRIGGER语法**
        CREATE TRIGGER trigger_nametrigger_time trigger_event ON tbl_name FOR EACH ROW trigger_stmt;
        触发程序是与表有关的命名数据库对象，当表上出现特定事件时，将激活该对象。
        触发程序与命名为tbl_name的表相关。tbl_name必须引用永久性表。不能将触发程序与TEMPORARY表或视图关联起来。
        trigger_time是触发程序的动作时间。它可以是BEFORE或AFTER，以指明触发程序是在激活它的语句之前或之后触发。
        trigger_event指明了激活触发程序的语句的类型。trigger_event可以是下述值之一：
        (1).INSERT：将新行插入表时激活触发程序，例如，通过INSERT、LOAD DATA和REPLACE
语句。
       (2).UPDATE：更改某一行时激活触发程序，例如，通过UPDATE语句。
       (3).DELETE：从表中删除某一行时激活触发程序，例如，通过DELETE和REPLACE语句。
请注意，trigger_event与以表操作方式激活触发程序的SQL语句并不很类似，这点很重要。例如，关于INSERT的BEFORE触发程序不仅能被INSERT语句激活，也能被LOAD DATA语句激活。可能会造成混淆的例子之一是INSERT INTO .. ON DUPLICATE UPDATE ...语法：BEFORE INSERT触发程序对于每一行将激活，后跟AFTER INSERT触发程序，或BEFORE UPDATE和AFTER  UPDATE触发程序，具体情况取决于行上是否有重复键。
      对于具有相同触发程序动作时间和事件的给定表，不能有两个触发程序。例如，对于某一表，不能有两个BEFORE UPDATE触发程序。但可以有1个BEFORE UPDATE触发程序和1个BEFORE  INSERT触发程序，或1个BEFOREUPDATE触发程序和1个AFTER UPDATE触发程序。trigger_stmt是当触发程序激活时执行的语句。如果你打算执行多个语句，可使用BEGIN ... END复合语句结构。这样，就能使用存储子程序中允许的相同语句
**(二).DROP TRIGGER语法**
     DROP TRIGGER[schema_name.]trigger_name舍弃触发程序。方案名称（schema_name）是可选的。如果省略了schema（方案），将从当前方案中舍弃触发程序。
     注释：从MySQL 5.0.10之前的MySQL版本升级到5.0.10或更高版本时（包括所有的MySQL5.1版本），必须在升级之前舍弃所有的触发程序，并在随后重新创建它们，否则，在升级之后DROP TRIGGER不工作。DROP TRIGGER语句需要SUPER权限。
**(三).使用触发程序**
      在本节中，介绍了在MySQL 5.1中使用触发程序的方法，并介绍了在使用触发程序方面的限制。
      触发程序是与表有关的命名数据库对象，当表上出现特定事件时，将激活该对象。在某些触发程序的用法中，可用于检查插入到表中的值，或对更新涉及的值进行计算。 
      触发程序与表相关，当对表执行INSERT、DELETE或UPDATE语句时，将激活触发程序。可以将触发程序设置为在执行语句之前或之后激活。例如，可以在从表中删除每一行之前，或在更新了,每一行后激活触发程序。要想创建触发程序或舍弃触发程序，可使用CREATE TRIGGER或DROP TRIGGER语句.触发程序不能调用将数据返回客户端的存储程序，也不能使用采用CALL语句的动态SQL（允许存储程序通过参数将数据返回触发程序）。
      触发程序不能使用以显式或隐式方式开始或结束事务的语句，如START TRANSACTION、
COMMIT或ROLLBACK。
     使用OLD和NEW关键字，能够访问受触发程序影响的行中的列（OLD和NEW不区分大小写）。
     在INSERT触发程序中，仅能使用NEW.col_name，没有旧行。在DELETE触发程序中，仅能使用OLD.col_name，没有新行。在UPDATE触发程序中，可以使用OLD.col_name来引用更新前的某一行的列，也能使用NEW.col_name来引用更新后的行中的列。
用OLD命名的列是只读的。你可以引用它，但不能更改它。对于用NEW命名的列，如果具有SELECT权限，可引用它。在BEFORE触发程序中，如果你具有UPDATE权限，可使用“SET NEW.col_name = value”更改它的值。这意味着，你可以使用触发程序来更改将要插入到新行中的值，或用于更新行的值。在BEFORE触发程序中，AUTO_INCREMENT列的NEW值为0，不是实际插入新记录时将自动生成的序列号。
     通过使用BEGIN ...END结构，能够定义执行多条语句的触发程序。在BEGIN块中，还能使用存储子程序中允许的其他语法，如条件和循环等。但是，正如存储子程序那样，定义执行多条语句的触发程序时，如果使用mysql程序来输入触发程序，需要重新定义语句分隔符，以便能够在触发程序定义中使用字符“;”。在下面的示例中，演示了这些要点。在该示例中，定义了1个UPDATE触发程序，用于检查更新每一行时将使用的新值，并更改值，使之位于0～100的范围内。它必须是BEFORE触发程序，这是因为，需要在将值用于更新行之前对其进行检查：
mysql> delimiter //
mysql> CREATE TRIGGER upd_check BEFORE UPDATE ON account
      -> FOR EACH ROW
      -> BEGIN
      -> IF NEW.amount < 0 THEN
      -> SET NEW.amount = 0;
      -> ELSEIF NEW.amount > 100 THEN
      ->  SET NEW.amount = 100;
      ->  END IF;
      -> END;//
mysql> delimiter ;
      较为简单的方法是，单独定义存储程序，然后使用简单的CALL语句从触发程序调用存储程序。如果你打算从数个触发程序内部调用相同的子程序，该方法也很有帮助。在触发程序的执行过程中，MySQL处理错误的方式如下：
     (1)如果BEFORE触发程序失败，不执行相应行上的操作。
     (2)仅当BEFORE触发程序（如果有的话）和行操作均已成功执行，才执行AFTER触发程序。
     (3) 如果在BEFORE或AFTER触发程序的执行过程中出现错误，将导致调用触发程序的整个语句的失败。
     (4)对于事务性表，如果触发程序失败（以及由此导致的整个语句的失败），该语句所执行的所有更改将回滚。对于非事务性表，不能执行这类回滚，因而，即使语句失败，失败之前所作的任何更改依然有效。
例一：
mysql> CREATE TABLE account (acct_num INT, amount DECIMAL(10,2));
mysql> CREATE TRIGGER ins_sum BEFORE INSERT ON account  FOR EACH ROW SET @sum = @sum + NEW.amount;
