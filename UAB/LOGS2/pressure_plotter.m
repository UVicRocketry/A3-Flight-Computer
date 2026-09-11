pressure_data_raw = readtable("LOGS2/LOGS1/PPORTS.CSV", ReadVariableNames=false);

rf = rowfilter(pressure_data_raw);
fig_title = "";

for x = 10:12
    fig = figure();
    hold on
    plot(pressure_data_raw(rf.Var2 == x, :).Var1, pressure_data_raw(rf.Var2 == x, :).Var3);
    fig_title = sprintf('LOGS1 - DAQ ID 0x2%d', x);
    title(fig_title);
    hold off
end