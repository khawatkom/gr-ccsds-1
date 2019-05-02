function [ value ] = funct(x , y)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
value = (sign(x)*sign(y))*fe(fe(x) + fe(y));

end

