%%计算两个一维定序特征的汉明距离
function result=Hamm(A1,M1,A2,M2)

a=size(A1);

M=M1&M2;

fenzi=0;
fenmu=0;

for i=1:a
    if M(i)>0
        fenmu=fenmu+1;
         if A1(i)==A2(i)
             fenzi=fenzi;
         else
             fenzi=fenzi+1;
         end
    end   
end

if fenmu>0
    result=fenzi/fenmu;
else
    result=0;
end

