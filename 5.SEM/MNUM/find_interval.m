
function find_interval = find_interval(x, u, n)
    for i = 1:n
        if x >= u(i) && x < u(i+1)
            find_interval = i;
            break;
        end
    end
end