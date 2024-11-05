function [best_fitobj, best_start, chi2] = find_best_fit_func(x, y, dy, guess)
funcs = generate_function_strings(6);

weights = 1 ./ (dy .^ 2);
starts = 1:5;

chi2_arr = zeros(length(starts), length(funcs));
errorbar(x, y, dy)

for j=1:length(funcs)
    f = funcs(j);
    f = fittype(f);
    n_vars = numcoeffs(f);

    options = fitoptions(f);
    % we are imposing that all coefficients (except the constant one) are negative
    % this way the func is guaranteed to be decreasing
    options.Lower = zeros(1, n_vars) - inf;
    options.Upper = [inf zeros(1, n_vars - 1)];
    options.StartPoint = [guess zeros(1, n_vars - 1)];

    for i=1:length(starts)
        start = starts(i);
        options.Weights = weights(start:end);

        fitobj = fit(x(start:end)', y(start:end)', f, options);

        sse = weights(start:end) * ((y(start:end)' - feval(fitobj, x(start:end))) .^ 2);
        assert(isscalar(sse));
        df = length(y(start:end)) - n_vars;
        chi2_arr(i, j) = sse / df;
    end
end

dist = abs(chi2_arr - 1);

[~, idx_best] = min(dist, [], "all");
chi2 = chi2_arr(idx_best);
[i0, j0] = ind2sub(size(dist), idx_best);
best_start = starts(i0);

best_f = funcs(j0);
best_f = fittype(best_f);
n_vars = numcoeffs(best_f);

options = fitoptions(best_f);
options.Lower = zeros(1, n_vars) - inf;
options.Upper = [inf zeros(1, n_vars - 1)];
options.StartPoint = [guess zeros(1, n_vars - 1)];

best_fitobj = fit(x(best_start:end)', y(best_start:end)', best_f, options);

end
