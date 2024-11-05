function plot_distr(x_samples, nbins, base_path)

% [pdf, x_pdf, bw] = kde(x_samples);
% disp(bw);
[pdf, x_pdf] = kde(x_samples);

clf("reset");

hold on
histogram(x_samples, nbins, Normalization="pdf", DisplayName="istogramma")
plot(x_pdf, pdf, DisplayName="densità stimata (KDE)")
hold off

xlabel("$x$", Interpreter="latex")
ylabel("$f(x)$", Interpreter="latex")
legend(Location="best")

% axis tight
axis padded

saveas(gcf, fullfile(base_path, "distr_plot.pdf"))

end