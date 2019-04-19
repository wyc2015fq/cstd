# Dev TreeList 总结 - 毛小亮 - 博客园
# [Dev TreeList 总结](https://www.cnblogs.com/xianerwonder/p/4441722.html)
1.表格的要求：如果要求有父子节点关系，则必须有ID和ParentID字段，并且父节点ParentID字段必须指向ID字段。
2.Access表格在穿入DATATABLE的时候，要想表现出父子节点关系，父子字段必须是相同的数据类型。
3.Dev严格的区分大小写
4.数据在展示前，做好ID（唯一值，并且基本上是混合了父子ID的混合字段，所以表结构不清晰），再添加一个BSM辅助字段用来展现父子节点，这个字段的值可以重复，不受主键条件限制。
4.RUN Designer中可以编辑展示列顺序（Visible Index），将标示列展现在第一个索引。
很重要的两句话:
tlCategory.KeyFieldName = "CategoryId";//设置主键 
tlCategory.ParentFieldName = "ParentId";//设置父级主键 

