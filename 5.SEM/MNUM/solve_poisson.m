function solve_poisson = solve_poisson(N)
    h = 1/N;
    x = (1:N-1) * h;
    y = (1:N-1) * h;

    [X, Y] = meshgrid(x, y);

    % Warunki brzegowe (0 na brzegu kwadratu)
    u = zeros(N-1, N-1);

    % Definicja funkcji f(x, y) (tu przykład: f(x, y) = x * y)
    f = ones(N-1, N-1) .* x;
 
    max_iter = 1000;  % Maksymalna liczba iteracji
    tol = 1e-6;       % Tolerancja

    % Budowanie macierzy A
    main_diag = 4 * ones(N - 1, 1);
    off_diag = -1 * ones(N - 1, 1);

    T = spdiags([off_diag, main_diag, off_diag], [-1, 0, 1], N-1, N-1);
    A = kron(speye(N - 1), T);
    I1 = -1 * ones((N-1)^2, 1);
    I2 = spdiags([I1, I1], [-N+1, N-1], (N-1)^2, (N-1)^2);
    A = (A + I2)/(h*h);

    % Inicjalizacja wektora rozwiązania
    u_vector = u(:);

    for iter = 1:max_iter
        % Oblicz residuum
        res = f(:) - A * u_vector;
        n = norm(res)
        
        % Sprawdź warunek stopu
        if n < tol
            break;
        end

        % Oblicz kierunek
        if iter == 1
            p = res;
        else
            beta = (res' * res) / (res_old' * res_old);
            p = res + beta * p;
        end

        % Oblicz krok
        Ap = A * p;
        alpha = (res' * res) / (p' * Ap);

        % Aktualizuj rozwiązanie
        u_vector = u_vector + alpha * p;

        res_old = res;
    end

    % Przywróć macierz rozwiązania
    u = reshape(u_vector, [N-1, N-1]);

    % Wykres rozwiązania
    figure;
    mesh(X, Y, u)
    xlabel('x');
    ylabel('y');
    zlabel('u(x, y)');
    title('Rozwiązanie równania Poissona');
end

