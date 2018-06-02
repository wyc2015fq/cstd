function Dist1=mixHD1(Code1,newMask1,Code2,newMask2)

for m=1:3600
    t1=1+(m-1)*64;
    t2=m*64;
    t3=floor((m-1)/12)+1;
    t4=1+(t3-1)*64;
    t5=t3*64;    
    Dist1(m)=HDR(Code1(:,(t1:t2)),newMask1(:,(t4:t5)),Code2(:,(t1:t2)),newMask2(:,(t4:t5)));
end