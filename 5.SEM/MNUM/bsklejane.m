function bsklejane = bsklejane(a, b, c, N)
    n = 3;
    M = c * (N - 2 * n);
    h_hat = (b - a) / M;
    h = (b - a) / (N - 2 * n);
    
    u = zeros(1, N+1);
    for i = 0:N
        u(i+1) = a + (i - n) * h;
    end

    xs = a + (0:M) * h_hat;

    for j = 0:M
        y(j + 1) = abs(xs(j + 1) - (a + b) / 2);
    end

    A = sparse(M+1, N+1);
    for j = 0:M
        A(j+1, :) = Ni3(xs(j + 1), u, n, N);
    end
    y
    d = A \ y';
    
    s_values = zeros(1, M+1);
    
    for x_i = 1:M+1
        x = xs(x_i);
        k = find_interval(x, u, N);
        d2 = d;
        d_pop = d2;
        for j = 1:n
            for i = k-n+j:k
                alpha = (x - u(i)) / (u(i+n+1-j) - u(i)); 
                d2(i) = (1 - alpha) * d_pop(i-1) + alpha * d_pop(i);
            end
            d_pop = d2;
        end
        s_values(x_i) = d2(k);
    end
    
    plot(xs, s_values, 'b-', xs, y, 'ro');
    xlabel('x');
    ylabel('s(x)');
    title('B-sklejane Funkcje');
end
