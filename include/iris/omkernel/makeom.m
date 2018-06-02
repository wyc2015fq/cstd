function result=makeom(im,ordifilter)
result=imfilter(double(im),ordifilter,'replicate','same');
[M,N]=size(result);
for i=1:M
    for j=1:N
        if result(i,j)>0
            result(i,j)=1;
        else
            result(i,j)=0;
        end
    end
end

