classdef QhoSampler
    properties
        N_warmup(1,1) {mustBeInteger, mustBePositive} = 1e5
        N_sample(1,1) {mustBeInteger, mustBePositive} = 1e6

        NHmc(1,1) {mustBeInteger, mustBePositive} = 100
        epsilon(1,1) double {mustBeReal, mustBePositive} = 0.01

        max_shift {mustBeInteger, mustBePositive} = 7

        rev_check (1,1) logical = 1
        seed {mustBeInteger} = []
    end
    methods
        function [x2_samples, xx_samples] = sample(obj, a, N, m, omg)
            m0 = m * a;
            omg0 = omg * a;

            [~, x2_samples, xx_samples] = qho_sample(obj, N, m0, omg0);

            x2_samples = x2_samples .* a ^ 2;
            xx_samples = xx_samples .* a ^ 2;
        end
    end
end