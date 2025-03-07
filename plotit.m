clear
clc

solution = readmatrix("solution.csv");

%Angular Velocity
figure(1)
plot(solution(1:90000, 1)  ./ 60, abs(solution(1:90000, 3) .* (60 / (2 * pi))))
xlabel("Time (min)")
ylabel("Angular Velocity (RPM)")
title('Angular Velocity vs. Time')

figure(2)
plot(solution(1:90000, 1) ./ 60, solution(1:90000, 5))
xlabel("Time (min)")
ylabel("Lift (N)")
title('Lift vs. Time')