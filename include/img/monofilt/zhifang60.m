function histLBP=zhifang60(A)
histLBP=zeros(1,60);
[m,n]=size(A);
%for t=0:59
    for i=1:m
        for j=1:n
            %if t*4.3<A(i,j)<=(t+1)*4.3
                A(i,j)=round(A(i,j)/4.26);
            end
        end
       
%end
for i=1:m
    for j=1:n
        histLBP(A(i,j)+1)=histLBP(A(i,j)+1)+1;
    end 
end  
