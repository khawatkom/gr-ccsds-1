function [ value ] = fe( x )
x = abs(x);
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
value = log((exp(x) + 1)/(exp(x) - 1));

end

