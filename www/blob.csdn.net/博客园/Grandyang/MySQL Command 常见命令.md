# MySQL Command 常见命令 - Grandyang - 博客园







# [MySQL Command 常见命令](https://www.cnblogs.com/grandyang/p/5343562.html)









```
/* Load data from txt file */
LOAD DATA LOCAL INFILE "D:/data.txt" INTO TABLE tname;


/* Load .sql command file */
SOURCE D:/mysql.sql;


/* Change passowrd */
UPDATE mysql.user SET password = PASSWORD('123456') WHERE User = 'root';


/* Show current database */
SELECT DATABASE();


/* Show current user */
SELECT USER();


/* Create the database */
CREATE DATABASE dbname;


/* Choose the database */
USE dbname;


/* Delete the database */
DROP dbname;


/* Show the tables */
SHOW TABLES;


/* Describe the table */
DESCRIBE tname;


/* Create the table */
CREATE TABLE tname (id INT, name VARCHAR(30));


/* Delete the table */
DROP TABLE tname;


/* Delete the date in table */
TRUNCATE TABLE tname;


/* Insert the table */
INSERT INTO tname VALUES (1, 'hello');


/* Search the table */
SELECT id, name FROM tname WHERE id = 1 ORDER BY DESC;


/* Update the table */
UPDATE tname SET name = 'world' WHERE id = 1;


/* Display the table */
SELECT * FROM tname;
```



More Information Check:

[http://blog.csdn.net/ithomer/article/details/5131863](http://blog.csdn.net/ithomer/article/details/5131863)














