fid = fopen('H2.txt','wt');
for ii = 1:size(H,1)
fprintf(fid,'%d ',H(ii,:));
fprintf(fid,'\n');
end