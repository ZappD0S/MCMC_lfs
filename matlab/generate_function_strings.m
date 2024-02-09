function [func_strings, n_vars_arr] = generate_function_strings(max_order, max_vars)

if nargin < 2 || isempty(max_vars)
    max_vars = max_order + 1;
end

func_strings = strings(0);
n_vars_arr = [];

for order=2:max_order
    n_coeffs = order + 1;
    coeffs = arrayfun(@(i) "a" + num2str(i), order:-1:0);

    if (n_coeffs <= max_vars)
        polyval_coeffs_str = strjoin(coeffs, " ");
        % n_coeffs
        func_string = "polyval([" + polyval_coeffs_str + "], x)";
        func_strings(end + 1) = func_string;
        n_vars_arr(end + 1) = n_coeffs;
    end

    for n_vars=2:min(n_coeffs - 1, max_vars)
        n_zeros = n_coeffs - n_vars;

        zero_pos = nchoosek(2:(n_coeffs - 1), n_zeros)';

        for zp=zero_pos
            vars = coeffs;
            vars(zp) = "0";
            polyval_coeffs_str = strjoin(vars, " ");
            % n_vars
            func_string = "polyval([" + polyval_coeffs_str + "], x)";
            func_strings(end + 1) = func_string;
            n_vars_arr(end + 1) = n_vars;
        end
    end
end

end