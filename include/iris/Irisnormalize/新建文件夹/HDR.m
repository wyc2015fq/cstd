%%计算两个定序特征的汉明距离
function result=HDR(A1,B1,A2,B2)

[a b]=size(A1);

B=B1&B2;

fenzi=0;
fenmu=0;

for i=1:a
    for j=1:b
        if B(i,j)>0
            fenmu=fenmu+1;
            if A1(i,j)==A2(i,j)
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

