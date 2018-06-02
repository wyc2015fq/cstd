function p = my_polyfit(x,y,n)

x = x(:);
y = y(:);

% Construct Vandermonde matrix.
V(:,n+1) = ones(length(x),1,class(x));
for j = n:-1:1
   V(:,j) = x.*V(:,j+1);
end

% Solve least squares problem.
p = V\y;
p = p.';  % Polynomial coefficients are row vectors by convention.
