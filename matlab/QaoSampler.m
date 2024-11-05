classdef QaoSampler < QhoSampler

    properties
        lambda (1,1) {mustBeReal, mustBePositive} = 1e-2
        x0 (1,1) {mustBeReal} = 0.0 % 3.0
    end

    methods
        function obj = QaoSampler()
            obj.sys_type = "qao";
        end

        function [e0_samples, xx_samples, x_samples] = sample(obj, a, N)
            m0 = obj.m / a;
            omg0 = obj.omg * a;
            lambda0 = obj.lambda * a;

            [~, e0_samples, xx_samples, x_samples] = qho_sample(obj, N, m0, omg0, lambda0, obj.x0);

            e0_samples = e0_samples ./ a;

            % x2_samples = x_samples .^ 2;
            % e0_samples = mean(obj.m * obj.omg ^ 2 * x2_samples + obj.lambda * x2_samples .* (3 * x2_samples - 2 * obj.x0 ^ 2));
            % e0_samples = mean(obj.m * obj.omg ^ 2 * x_samples .^ 2 + ...
            %     obj.lambda * x_samples .^ 4 + ...
            %     2 * obj.lambda * x_samples .^ 2 .* (x_samples .^ 2 - obj.x0 ^ 2));
        end
    end

end