
for k=2:2  %¡Ω÷÷∫À
    allimg=[];
    OM=load(strcat(file,'ordifilter',num2str(k+1),sita{1}));
    OM=OM.ordifilterall;
    [m n]=size(OM);
    for i=1:m
        for j=1:n
            mm=OM{i,j};
            mv = mm(:, floor((size(mm, 2) + 1)/2));
        end
    end
end
