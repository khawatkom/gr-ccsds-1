function[b]=circulant(a)         
for i=1:length(a)                
    b(i,:)=matrix_right_circular_shift(a,i-1);
end