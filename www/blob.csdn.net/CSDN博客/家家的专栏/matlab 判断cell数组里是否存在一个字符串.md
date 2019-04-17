# matlab 判断cell数组里是否存在一个字符串 - 家家的专栏 - CSDN博客





2014年07月01日 15:40:13[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：11118








                
personnamelist=[];i=0;

homeimages_array=dir(fullfile('./test/','*.jpg'));

for ihome=1:size(homeimages_array,1)

    temp=homeimages_array(ihome).name(1:end-4);
**ind = find(ismember(personnamelist,char(temp(1:5))))**

    if  **~numel(ind)**

        i=i+1;

        personnamelist{i}=char(temp(1:5));

        personsum(i)=1;

    else

        personsum(ind)=personsum(ind)+1;

    end

end
            


