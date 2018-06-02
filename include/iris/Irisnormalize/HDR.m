%%计算两个定序特征的汉明距离
function result=HDR(A1_f,B1,A2_f,B2)

[a b]=size(A1_f);

B=B1&B2;

fenzi=0;
fenmu=0;

for i=1:a
    for j=1:b
        if B(i,j)>0
            fenmu=fenmu+1;
            if A1_f(i,j)==A2_f(i,j)
                fenzi=fenzi;
            else
                fenzi=fenzi+1;
            end
        end
    end
end
if fenmu>0
    result=fenzi/fenmu;
else
    result=0;
end

