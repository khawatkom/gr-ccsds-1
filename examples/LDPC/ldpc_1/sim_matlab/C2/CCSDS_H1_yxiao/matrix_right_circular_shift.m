function [c]=matrix_right_circular_shift(a,b)      
if b>=0
    s=size(a);
    k=mod(b,s(2));
    m=[1:s(2) 1:s(2)];
    if k==0
        n=m(1:s(2));
    else
        n=m((1+(s(2)-k)):(s(2)+(s(2)-k)));
    end
    for i=1:s(2)
        c(:,i)=a(:,n(i));
    end
end