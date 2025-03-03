clear
clc

solution = readmatrix("solution.csv");

% Cyclic Loading
figure(1)
plot(solution(length(solution) - 200:length(solution) - 1, 1), solution(length(solution) - 200:length(solution) - 1, 5));
xlabel("Time(s)")
ylabel("Lift(N)")
title('Cyclic Lift forces during Propeller Autorotation')

%Angular Velocity
figure(2)
plot(solution(1:80000, 1), solution(1:80000, 3))
xlabel("Time(s)")
ylabel("Angular Velocity (rads/s)")
title('Angular Velocity Steady State')