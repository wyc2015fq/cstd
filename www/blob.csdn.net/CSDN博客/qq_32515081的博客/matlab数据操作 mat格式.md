# matlab数据操作 mat格式 - qq_32515081的博客 - CSDN博客
2017年12月22日 15:43:42[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：681
13.1 数据基本操作
　　本节介绍基本的数据操作，包括工作区的保存、导入和文件打开。
13.1.1 文件的存储
　　MATLAB支持工作区的保存。用户可以将工作区或工作区中的变量以文件的形式保存，以备在需要时再次导入。保存工作区可以通过菜单进行，也可以通过命令窗口进行。
　　1. 保存整个工作区
　　选择File菜单中的Save Workspace As…命令，或者单击工作区浏览器工具栏中的Save，可以将工作区中的变量保存为MAT文件。
　　2. 保存工作区中的变量
　　在工作区浏览器中，右击需要保存的变量名，选择Save As…，将该变量保存为MAT文件。
　　3. 利用save命令保存
　　该命令可以保存工作区，或工作区中任何指定文件。该命令的调用格式如下：
● save：将工作区中的所有变量保存在当前工作区中的文件中，文件名为 matlab.mat，MAT文件可以通过load函数再次导入工作区，MAT函数可以被不同的机器导入，甚至可以通过其他的程序调用。
● save('filename')：将工作区中的所有变量保存为文件，文件名由filename指定。如果filename中包含路径，则将文件保存在相应目录下，否则默认路径为当前路径。
● save('filename', 'var1', 'var2', ...)：保存指定的变量在 filename 指定的文件中。
● save('filename', '-struct', 's')：保存结构体s中全部域作为单独的变量。
● save('filename', '-struct', 's', 'f1', 'f2', ...)：保存结构体s中的指定变量。
● save('-regexp', expr1, expr2, ...)：通过正则表达式指定待保存的变量需满足的条件。
● save('..., 'format')，指定保存文件的格式，格式可以为MAT文件、ASCII文件等。
13.1.2 数据导入
　　MATLAB中导入数据通常由函数load实现，该函数的用法如下：
● load：如果matlab.mat文件存在，导入matlab.mat中的所有变量，如果不存在，则返回error。
● load filename：将filename中的全部变量导入到工作区中。
● load filename X Y Z ...：将filename中的变量X、Y、Z等导入到工作区中，如果是MAT文件，在指定变量时可以使用通配符“*”。
● load filename -regexp expr1 expr2 ...：通过正则表达式指定需要导入的变量。
● load -ascii filename：无论输入文件名是否包含有扩展名，将其以ASCII格式导入；如果指定的文件不是数字文本，则返回error。
● load -mat filename：无论输入文件名是否包含有扩展名，将其以mat格式导入；如果指定的文件不是MAT文件，则返回error。
例13-1 将文件matlab.map中的变量导入到工作区中。
　　首先应用命令whos –file查看该文件中的内容：
　　>> whos -file matlab.mat
　　 Name                  Size              Bytes Class
　　 A                    2x3              48 double array
　　 I_q                 415x552x3            687240 uint8 array
　　 ans                1x3               24 double array
　　 num_of_cluster       1x1               8 double array
　　Grand total is 687250 elements using 687320 bytes
　　将该文件中的变量导入到工作区中：
　　>> load matlab.mat
　　该命令执行后，可以在工作区浏览器中看见这些变量，如图13-1所示。
图13-1 导入变量后的工作区视图
　　接下来用户可以访问这些变量。
　　>> num_of_cluster
　　num_of_cluster =
　　     3
　　MATLAB中，另一个导入数据的常用函数为importdata，该函数的用法如下：
● importdata('filename')，将filename中的数据导入到工作区中；
● A = importdata('filename')，将filename中的数据导入到工作区中，并保存为变量A；
● importdata('filename','delimiter')，将filename中的数据导入到工作区中，以delimiter指定的符号作为分隔符；
例13-2 从文件中导入数据。
　　>> imported_data = importdata('matlab.mat')
　　imported_data = 
　　               ans: [1.1813 1.0928 1.6534]
　　                 A: [2x3 double]
　　               I_q: [415x552x3 uint8]
　　    num_of_cluster: 3
　　与load函数不同，importdata将文件中的数据以结构体的方式导入到工作区中。
13.1.3 文件的打开
　　MATLAB中可以使用open命令打开各种格式的文件，MATLAB自动根据文件的扩展名选择相应的编辑器。
   需要注意的是open('filename.mat')和load('filename.mat')的不同，前者将filename.mat以结构体的方式打开在工作区中，后者将文件中的变量导入到工作区中，如果需要访问其中的内容，需要以不同的格式进行。
例13-3 open与load的比较。
　　>> clear
　　>> A = magic(3);
　　>> B = rand(3);
　　>> save
　　Saving to: matlab.mat
　　>> clear
　　>> load('matlab.mat')
　　>> A
　　A =
　　     8     1     6
　　     3     5     7
　　     4     9     2
　　>> B
　　B =
　　    0.9501    0.4860    0.4565
　　    0.2311    0.8913    0.0185
　　    0.6068    0.7621    0.8214
　　>> clear
　　>> open('matlab.mat')
　　ans = 
　　    A: [3x3 double]
　　    B: [3x3 double]
　　>> struc1=ans;
　　>> struc1.A
　　ans =
　　     8     1     6
　　     3     5     7
　　     4     9     2
　　>> struc1.B
　　ans =
　　    0.9501    0.4860    0.4565
　　    0.2311    0.8913    0.0185
　　    0.6068    0.7621    0.8214
