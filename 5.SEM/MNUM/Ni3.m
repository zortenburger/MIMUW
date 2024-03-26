function Ni3 = Ni3(x, u, n, N)
    k = find_interval(x, u, N) ;
    b = zeros(1, N+1);
    b(k) = 1;
    for j = 1:n
        beta = (u(k+1) - x) / (u(k+1) - u(k-j+1));
        b(k-j) = beta * b(k-j+1);
        for i = k-j+1:k-1
            alpha = 1 - beta;
            beta = (u(i+j+1) - x) / (u(i+j+1) - u(i+1));
            b(i) = alpha * b(i) + beta * b(i+1);
        end
        b(k) = b(k) * (1 - beta);
    end
    Ni3 = b;
end