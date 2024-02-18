classdef QaoSampler < QhoSampler
    methods
        function obj = QaoSampler()
            obj.sys_type = "qao";
        end

        function [e0_samples, xx_samples] = sample(obj, a, N, m, omg, lambda)
            m0 = m / a;
            omg0 = omg * a;
            lambda0 = lambda * a;

            [~, e0_samples, xx_samples] = qho_sample(obj, N, m0, omg0, lambda0);

            e0_samples = e0_samples ./ a;
        end
    end

end