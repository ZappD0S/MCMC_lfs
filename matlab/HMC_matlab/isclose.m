function [res] = isclose(a, b, rtol, atol)

    if nargin < 4
        atol = 1e-8;
    end

    if nargin < 3
        rtol = 1e-5;
        % atol = 1e-8;
    end

    res = abs(a - b) <= (atol + rtol * abs(b));

end