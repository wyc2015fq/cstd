# 学习笔记——DISTINCT - 左直拳的马桶_日用桶 - CSDN博客
2014年03月13日 17:08:56[左直拳](https://me.csdn.net/leftfist)阅读数：1859
DISTINCT印象中向来被人诟病，说它效率低下。但网上那些SQL 面试题答案，却时有用之。其中 COUNT(DISTINCT 句式，我以前很少用，这里做个笔记。
为管理岗位业务培训信息，建立3个表:
S (S#,SN,SD,SA) S#,SN,SD,SA 分别代表学号、学员姓名、所属单位、学员年龄
C (C#,CN ) C#,CN 分别代表课程编号、课程名称
SC ( S#,C#,G ) S#,C#,G 分别代表学号、所选修的课程编号、学习成绩
1. 使用标准SQL嵌套语句查询选修课程名称为’税收基础’的学员学号和姓名
--实现代码:
Select SN,SD FROM S Where [S#] IN( Select [S#] FROM C,SC Where C.[C#]=SC.[C#] AND CN=N'税收基础')
2. 使用标准SQL嵌套语句查询选修课程编号为’C2’的学员姓名和所属单位
--实现代码:
Select S.SN,S.SD FROM S,SC Where S.[S#]=SC.[S#] AND SC.[C#]='C2'
3. 使用标准SQL嵌套语句查询不选修课程编号为’C5’的学员姓名和所属单位
--实现代码:
Select SN,SD FROM S Where [S#] NOT IN( Select [S#] FROM SC Where [C#]='C5')
4. 使用标准SQL嵌套语句查询选修全部课程的学员姓名和所属单位
--实现代码:
Select SN,SD FROM S Where [S#] IN( Select [S#] FROM SC RIGHT JOIN C ON SC.[C#]=C.[C#] GROUP BY [S#] HAVING COUNT(*)=COUNT([S#]))
5. 查询选修了课程的学员人数
--实现代码:
Select 学员人数=COUNT(DISTINCT [S#]) FROM SC
6. 查询选修课程超过5门的学员学号和所属单位
--实现代码:
Select SN,SD FROM S Where [S#] IN( Select [S#] FROM SC GROUP BY [S#] HAVING COUNT(DISTINCT [C#])>5)
