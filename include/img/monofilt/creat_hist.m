step_bin_a=total/bin_num_a;
for R_num=1:region_num_w*region_num_h
    Hist_A(:,R_num)=zeros(bin_num_a,1);
    region=img_A(region_h_index(R_num,:),region_w_index(R_num,:));

    for i=1:bin_num_a
        range_l=(i-1)*step_bin_a;range_h=step_bin_a*i;
        tem_index=find(region>=range_l&region<range_h);
        Hist_A(i,R_num)=(length(tem_index));
    end
end