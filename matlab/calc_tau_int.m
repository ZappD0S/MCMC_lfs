function [tau_int] = calc_tau_int(d, tt0)

tt_start = 0;
tt_end = min(length(d), tt0);

while true
    acf((tt_start:tt_end) + 1) = my_autocorr(d, [tt_start tt_end]);
    
    [tau_int, idx] = max(0.5 + cumsum(acf));
    
    if idx < 0.5 * tt_end
        break
    end
    
    if tt_end == length(d)
        break
    end
    
    tt_start = tt_end + 1;
    tt_end = min(length(d), tt_end * 10);
end

end