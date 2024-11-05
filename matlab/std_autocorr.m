function dx = std_autocorr(x)
    [Nsamples, N] = size(x);
    acf = MYautocorr_fft(x, floor(Nsamples / 2));

    [~, max_inds] = max(acf < 0, [], 1);

    cumsums = cumsum(acf(1:max(max_inds), :), 1);
    % tau_ints = cumsums(1:N, max_inds) - 0.5;

    inds = sub2ind(size(cumsums), max_inds - 1, 1:N);
    tau_ints = cumsums(inds) - 0.5;

    dx = std(x, [], 1) ./ sqrt(Nsamples ./ (2 .* tau_ints));
end