%%计算两个定序特征的汉明距离
function result=HDR2(A1_f,B1,A2_f,B2)

[a b]=size(A1_f);

%B=B1&B2;

fenzi=0;
fenmu=a*b;

for i=1:a
    for j=1:b
       
            
            if A1_f(i,j)==A2_f(i,j)
                fenzi=fenzi;
            else
                fenzi=fenzi+1;
            end
      
    end
end


result=fenzi/fenmu;


