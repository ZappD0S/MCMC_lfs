classdef QhoSampler
    properties
        sys_type = "qho"
        N_warmup(1,1) {mustBeInteger, mustBePositive} = 1e5
        N_sample(1,1) {mustBeInteger, mustBePositive} = 1e6

        NHmc(1,1) {mustBeInteger, mustBePositive} = 100
        epsilon(1,1) double {mustBeReal, mustBePositive} = 1e-2

        max_shift {mustBeInteger, mustBePositive} = 7

        rev_check (1,1) logical = 1
        seed {mustBeInteger} = []
    end

    methods
        function [e0_samples, xx_samples] = sample(obj, a, N, m, omg)
            m0 = m / a;
            omg0 = omg * a;

            [~, e0_samples, xx_samples] = qho_sample(obj, N, m0, omg0);

            e0_samples = e0_samples ./ a;
        end
    end
end