function normd = columnnorm(A)
%function normd = columnnorm(A)
%
%function normalizes each column of the matrix A such that
%the sqrt of the column squares equals 1.
%
norm = sqrt(sum(A.^2));
[m,n]=size(A);
denom = ones(m,1)*norm.^(-1);
normd = A.*denom;

