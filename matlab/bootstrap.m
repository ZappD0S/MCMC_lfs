function [mu, sigma] = bootstrap(nboot, bootfun, x, y, dy)

samples = zeros(1, nboot);

for i = 1:nboot
    ay = y + dy .* randn(size(dy));
    samples(i) = bootfun(x, ay);
end

mu = mean(samples);
sigma = std(samples);

% TODO: bootstrap plot?

end