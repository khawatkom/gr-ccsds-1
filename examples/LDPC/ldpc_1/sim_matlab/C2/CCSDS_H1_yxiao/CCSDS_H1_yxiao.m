% This program provided the parity check matrix in Fig.6-1 of LDPC in TM SYNCHRONIZATION
% AND CHANNEL CODING, Draft Recommendation for Space Data System Standards
% (CCSDS 131.0-P-1.1).
% Copyright (C 2010-2015) Yang XIAO, Beijing Jiaotong Univ., Jan. 15, 2010
% 
% Contact Add: Prof. Yang XIAO
% Institute of Information Science
% Beijing Jiaotong University
% Beijing 100044, China
%  E-Mail: yxiao@bjtu.edu.cn.
%
% We found that the generator matrix in Fig.6-2 to be wrong, Bi,j's are not
% circulant. You can not derive  circulant Bi,j's from the parity check
% matrix in Fig.6-1.
% There are two vital problems in the LDPC codes of CCSDS 131.0-P-1.1: 
% (1) the parity check matrix in Fig.6-1 is not full rank;
% (2) the generator matrix in Fig.6-2 is wrong, which are not circulant.
% To check our comments for the CCSDS 131.0-P-1.1 codes, you only construct the generator
% matrix in Fig.6-2, then multiply the transpose of the the parity check
% matrix in GF(2) provided by our program. You will find that the product
% in GF(2) is not zero!
 

% The data of matrix A comes from Table 6-1 of  CCSDS 131.0-P-1.1: Specification of Circulants

A=[0, 176 12, 239 0, 352 24, 431 0, 392 151, 409 0, 351 9, 359 0, 307 53, 329 0, 207 18, 281 0, 399 202, 457 0, 247 36, 261;
   99, 471 130, 473 198, 435 260, 478 215, 420 282, 481 48, 396 193, 445 273, 430 302, 451 96, 379 191, 386 244, 467 364, 470 51, 382 192, 414]; 

for i=1:16
    b1(1)=A(1,2*i-1)+1-1;
    b1(2)=A(1,2*i)+1-1-(A(1,2*i-1)+1+1)+1;
    b1(3)=511-(A(1,2*i)+1+1)+1;
    b2(1)=A(2,2*i-1)+1-1;
    b2(2)=A(2,2*i)+1-1-(A(2,2*i-1)+1+1)+1;
    b2(3)=511-(A(2,2*i)+1+1)+1;
    M{1,i}=[circulant([zeros(1,b1(1)) 1 zeros(1,b1(2)) 1 zeros(1,b1(3))]);circulant([zeros(1,b2(1)) 1 zeros(1,b2(2)) 1 zeros(1,b2(3))])];
end                                         
D1=[M{1,15} M{1,16}];                       
D2=[M{1,1} M{1,2} M{1,3} M{1,4} M{1,5} M{1,6} M{1,7} M{1,8} M{1,9} M{1,10} M{1,11} M{1,12} M{1,13} M{1,14}] ;

H1=[D2 D1];
mesh(255*H1)  % Show the scatters "1s" of the parity matrix of the CCSDS LDPC
H = [M{1,1} M{1,2} M{1,3} M{1,4} M{1,5} M{1,6} M{1,7} M{1,8} M{1,9} M{1,10} M{1,11} M{1,12} M{1,13} M{1,14} M{1,15} M{1,16}];