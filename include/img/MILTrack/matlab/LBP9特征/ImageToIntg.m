function ii=ImageToIntg(i,W,H);
s=[];
ii=[];
for m=1:H
    for n=1:W
        if n==1
            s(m,1)=i(m,1); 
        else
            s(m,n)=s(m,n-1)+i(m,n);
        end
        if m==1
             ii(m,n)=s(m,n); 
        else
            ii(m,n)=ii(m-1,n)+s(m,n);
        end
    end
end


