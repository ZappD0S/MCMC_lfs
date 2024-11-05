function [deltae, ddeltae] = estimate_deltae_creutz(xx, dxx, a) % min_samples)

max_shift = length(xx);


deltaes = zeros(1, max_shift - 1);
ddeltaes = zeros(1, max_shift - 1);

for i=1:(max_shift - 1)
    samples = zeros(1, max_shift - i);
    errs = zeros(1, max_shift - i);

    for j=i+1:max_shift
        dt = (j - i) * a;
        samples(j - i) = -(log(xx(j)) - log(xx(i))) / dt;
        % propagation of error on difference of logarithms
        errs(j - i) = sqrt((dxx(j) / xx(j))^2 + (dxx(i) / xx(i))^2) / dt;
    end

    % assert(all(samples > 0))

    deltaes(i) = mean(samples);
    % ddeltaes(i) = sqrt(errs * errs' / length(errs));
    ddeltaes(i) = sqrt(sum(errs .^ 2)) / length(errs);
end

% assert(all(deltaes > 0));

deltae = mean(deltaes);
ddeltae = sqrt(sum(ddeltaes .^ 2)) / length(ddeltaes);
end