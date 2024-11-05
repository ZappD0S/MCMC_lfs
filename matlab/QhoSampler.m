classdef QhoSampler
    properties
        sys_type = "qho"
        N_warmup(1,1) {mustBeInteger, mustBePositive} = 1e5
        N_sample(1,1) {mustBeInteger, mustBePositive} = 1e6

        NHmc(1,1) {mustBeInteger, mustBePositive} = 100
        epsilon(1,1) double {mustBeReal, mustBePositive} = 1e-2

        m (1,1) {mustBeReal, mustBePositive} = 1
        omg (1,1) {mustBeReal, mustBePositive} = 1

        max_shift {mustBeInteger, mustBePositive} = 7

        rev_check (1,1) logical = 0
        seed {mustBeInteger} = []
    end

    methods
        function [e0_samples, xx_samples, x_samples] = sample(obj, a, N)
            m0 = obj.m / a;
            omg0 = obj.omg * a;

            [~, e0_samples, xx_samples, x_samples] = qho_sample(obj, N, m0, omg0);

            e0_samples = e0_samples ./ a;

            % e0_samples = obj.m * obj.omg^2 .* mean(x_samples .^ 2);


            % [~, Phis] = qho_sample(obj, N, m0, omg0);

            % x2_samples = mean(Phis .^ 2);

            % xx_samples = zeros(obj.N_sample, obj.max_shift);
            % Phis_shift = Phis;

            % for i=1:obj.max_shift
            %     Phis_shift = circshift(Phis_shift, 1, 1);
            %     xx_samples(:, i) = mean(Phis .* Phis_shift);
            % end
        end
    end
end