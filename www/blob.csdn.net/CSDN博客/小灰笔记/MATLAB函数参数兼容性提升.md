# MATLAB函数参数兼容性提升 - 小灰笔记 - CSDN博客





2017年03月30日 22:48:21[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：389








       使用MATLAB的时候有时候会遇到把文件名称作为参数的时候，不过，通常情况下MATLAB的命令或者函数访问文件的时候需要给出一个带有扩展名的文件名称或者带有完整路径与扩展名的文件名称作为参数。

       通常，这种情况自己是十分清楚的，因此使用的时候一般不会有什么问题。不过，如果把程序提供给其他人用的时候出问题的可能性就比较大了。

       而一个比较简单的方式就是对文件的扩展名进行一点简单的判断处理，这样就能够在整个文件被处理前做出一点预判或者补救。

       例如，如下的函数：

function DataProcess(mat_file)

% useDataProcess('demo.mat') to process a model's mat file

%

if exist(mat_file) || exist(strcat(mat_file,'.mat'))

if length(mat_file) <= 4

            mat_file = strcat(mat_file,'.mat');

else

if ~strcmp(mat_file(end -3:end),'.mat')

                mat_file = strcat(mat_file,'.mat');

end

end

        load(mat_file);

        signal_info = whos;

        header_file_name = strrep(mat_file,'.mat','_MData.h');

        defination_file_name = strrep(mat_file,'.mat','_MData.c');

for i = 1:length(signal_info)

            var_info = signal_info(i);

            var_class = var_info.class;

if strcmp(var_class,'mpt.Signal')

                var_name = var_info.name;

                var_object = eval(var_name);

ifstrcmp(var_object.CoderInfo.CustomStorageClass,'Global')

                   var_object.CoderInfo.CustomAttributes.HeaderFile = header_file_name;

                    var_object.CoderInfo.CustomAttributes.DefinitionFile= defination_file_name;

end

end

end

        header_file_name = strrep(mat_file,'.mat','_CData.h');

        defination_file_name = strrep(mat_file,'.mat','_CData.c');

for i = 1:length(signal_info)

            var_info = signal_info(i);

            var_class = var_info.class;

if strcmp(var_class,'mpt.Parameter')

                var_name = var_info.name;

                var_object = eval(var_name);

if strcmp(var_object.CoderInfo.CustomStorageClass,'Global')

                   var_object.CoderInfo.CustomAttributes.HeaderFile = header_file_name;

                   var_object.CoderInfo.CustomAttributes.DefinitionFile =defination_file_name;

end

end

end

else

        disp('Can''t find mat file! Please Check!')

end

    save(mat_file);

end



       在处理mat文件前使用exist函数判断一下文件的存在性是不错的，根据返回值还能够判断出文件的类型。不过，如果即使是没有这一个功能，也可以使用额外的方式判断。代码中我通过字符串的切片获取了后面的几个字符，以此来查看输入的参数是否带有正确的扩展名。

       虽说是判断的策略很简单，不过倒是也可以避免很多执行的问题，增加一下程序的健壮性了。



