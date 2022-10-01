clearvars
clc

U = linspace(0, 10, 1e3); % 전체 x 범위 설정

mu_F = 1./(1+(U-10).^2); % fast fuzzy set 정의 
mu_M = 1./(1+(U-5).^2);  % medium fuzzy set 정의
mu_S = 1./(1+(U).^2);    % slow fuzzy set 정의

plot(U, mu_F, U, mu_M, U, mu_S, 'LineWidth', 2) % 그래프 그려준다, 선두께 2
grid on                 % 격자무늬 생성
xlabel('x')             % xlabel 이름
ylabel('\mu(x)')        % ylabel 이름        
legend('F', 'M', 'S')   % 범례 설정