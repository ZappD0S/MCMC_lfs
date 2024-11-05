function plot_creutz(DeltaEs, dDeltaEs, T, aa,base_path, target)

clf("reset");

hold on
errorbar(aa, DeltaEs, dDeltaEs, DisplayName="dati")

if nargin == 6 && ~isempty(target)
    plot([aa(1) aa(end)], [target target], "r--", DisplayName="valore teorico")
end

hold off

xlabel("$a$", Interpreter="latex", fontsize=16)
ylabel("$\Delta E$", Interpreter="latex", fontsize=16)

title(sprintf("Stima $\\Delta E$ con metodo Creutz ($T = %.2f$)", T), Interpreter="latex")
legend()

axis tight
axis padded

saveas(gcf, fullfile(base_path, sprintf("de_creutz_a%.2f-%.2f_T%d.pdf", aa(1), aa(end), T)))

end
