classdef QhoSampler
    properties
        N_warmup(1,1) {mustBeInteger, mustBePositive} = 1e3
        N_sample(1,1) {mustBeInteger, mustBePositive} = 1e5

        NHmc(1,1) {mustBeInteger, mustBePositive} = 100
        epsilon(1,1) double {mustBeReal, mustBePositive} = 0.1

        % ??
        seed(1,1) {mustBeInteger} = 1234
    end
    methods
        function obj = QhoSampler(N_warmup, N_sample, NHmc, epsilon, seed)
            obj.N_warmup = N_warmup;
            obj.N_sample = N_sample;
            obj.NHmc = NHmc;
            obj.epsilon = epsilon;
            obj.seed = seed;
        end

        function [x2_samples, xx_samples] = sample(obj, a, N, m, omg)
            m0 = m * a;
            omg0 = omg * a;

            [~, x2_samples, xx_samples] = qho_sample(obj, N, m0, omg0);

            x2_samples = x2_samples .* a ^ 2;
            xx_samples = xx_samples .* a ^ 2;
        end
    end
end