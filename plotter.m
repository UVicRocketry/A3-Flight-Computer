strain_data_raw = readtable("STRAIN.CSV", ReadVariableNames=false);

rf = rowfilter(strain_data_raw);
fig_title = "";

for x = 0:8
    fig = figure();
    hold on
    plot(strain_data_raw(rf.Var2 == x, :).Var1, strain_data_raw(rf.Var2 == x, :).Var3);
    plot(strain_data_raw(rf.Var2 == x, :).Var1, strain_data_raw(rf.Var2 == x, :).Var4);
    plot(strain_data_raw(rf.Var2 == x, :).Var1, strain_data_raw(rf.Var2 == x, :).Var5);
    fig_title = sprintf('DAQ ID 0x10%d', x);
    title(fig_title);
    hold off
end