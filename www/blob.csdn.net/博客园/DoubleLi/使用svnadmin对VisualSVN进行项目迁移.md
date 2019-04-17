# 使用svnadmin对VisualSVN进行项目迁移 - DoubleLi - 博客园






**使用svnadmin对VisualSVN进行项目迁移导出**
1> 启动命令行cmd
2> 运行
%VISUALSVN_SERVER%\bin\svnadmin dump PATH-TO-REPO > PATH-TO-DUMP
PATH-TO-REPO - SVN 项目仓库目录
PATH-TO-DUMP - 导出SVN项目仓库dump目录文件名


**导入**
1> 启动 VisualSVN Manager 创建一个空的仓库, 
Start -> All Programs > VisualSVN ;
右击"Repositories"节点选择 "select Create New Repository" 创建一个空的仓库;
注意不要选择"Create default structure (trunk, branches, tags)".

2> 导入已保存的dump文件
%VISUALSVN_SERVER%\bin\svnadmin load PATH-TO-NEW-REPO < PATH-TO-DUMP
PATH-TO-NEW-REPO - SVN 项目仓库目录
PATH-TO-DUMP - 已保存的SVN项目仓库dump文件


%VISUALSVN_SERVER%是VisualSVN安装目录环境变量, 默认安装后会创建此环境变量.

如不存在直接进去VisualSVN安装目录下bin目录执行svnadmin即可.

亦可直接复制Repository目录, 直接使用VisualSVN import功能.









