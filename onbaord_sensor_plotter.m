hgaccel_data_raw = readtable("HGACCEL.CSV", ReadVariableNames=false);

figure;
hold on
plot(hgaccel_data_raw.Var1(1:77276), 9.81 * 0.049 * hgaccel_data_raw.Var2(1:77276));
plot(hgaccel_data_raw.Var1(1:77276), 9.81 * 0.049 * hgaccel_data_raw.Var3(1:77276));
plot(hgaccel_data_raw.Var1(1:77276), 9.81 * 0.049 * hgaccel_data_raw.Var4(1:77276));
title("hg accelerometer data (LAB)")
legend("x-axis", "y-axis", "z-axis");
hold off

gyro_data_raw = readtable("GYRO.CSV", ReadVariableNames=false);

figure;
hold on
plot(gyro_data_raw.Var1(1:77276), 0.070 * gyro_data_raw.Var2(1:77276));
plot(gyro_data_raw.Var1(1:77276), 0.070 * gyro_data_raw.Var3(1:77276));
plot(gyro_data_raw.Var1(1:77276), 0.070 * gyro_data_raw.Var4(1:77276));
title("Gyro data (LAB)")
legend("pitch", "roll", "yaw");
hold off

lgaccel_data_raw = readtable("LGACCEL.CSV", ReadVariableNames=false);

figure;
hold on
plot(lgaccel_data_raw.Var1(1:77276), 9.81 * 0.976 * 10^-3 * lgaccel_data_raw.Var2(1:77276));
plot(lgaccel_data_raw.Var1(1:77276), 9.81 * 0.976 * 10^-3* lgaccel_data_raw.Var3(1:77276));
plot(lgaccel_data_raw.Var1(1:77276), 9.81 * 0.976 * 10^-3 * lgaccel_data_raw.Var4(1:77276));
title("lg accelerometer data (LAB)")
legend("x-axis", "y-axis", "z-axis");
hold off

baro_data_raw = readtable("GYRO.CSV", ReadVariableNames=false);

figure;
hold on
plot(baro_data_raw.Var1(1:77276), 1/(2^6) * baro_data_raw.Var2(1:77276));
plot(baro_data_raw.Var1(1:77276), 1/(2^16) * baro_data_raw.Var3(1:77276));
title("baro data (LAB)")
hold off

Lgaccel_data_raw = readtable("HGACCEL.CSV", ReadVariableNames=false);
