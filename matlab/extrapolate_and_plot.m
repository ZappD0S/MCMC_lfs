function extrapolate_and_plot(sampler, Ts)

base_path = fullfile(pwd, "..", "plots", class(sampler));

E0s = zeros(size(Ts));
dE0s = zeros(size(Ts));

DeltaEs = zeros(size(Ts));
dDeltaEs = zeros(size(Ts));

for i=1:length(Ts)
    T = Ts(i);
    Ns = round(linspace(T * 1.25, T * 5, 30));
    aa = T ./ Ns;

    sub_path = fullfile(base_path, sprintf("T_%f", T));

    [E0s, dE0s, DeltaEs, dDeltaEs] = estimate_energies(sampler, aa, Ns, sub_path);
    [E0, dE0, DeltaE, dDeltaE] = extrapolate_true_energies(E0s, dE0s, DeltaEs, dDeltaEs, T, aa, sub_path);

    E0s(i) = E0;
    dE0s(i) = dE0;

    DeltaEs(i) = DeltaE;
    dDeltaEs(i) = dDeltaE;
end

clf("reset")
errorbar(Ts, E0s, dE0s)
xlabel("T")
ylabel("E_0")
saveas(gcf, fullfile(base_path, sprintf("E0_T_%f-%f.png", Ts(1), Ts(end))))

clf("reset")
errorbar(Ts, DeltaEs, dDeltaEs)
xlabel("T")
ylabel("\Delta E")
saveas(gcf, fullfile(base_path, sprintf("DeltaE_T_%f-%f.png", Ts(1), Ts(end))))

end