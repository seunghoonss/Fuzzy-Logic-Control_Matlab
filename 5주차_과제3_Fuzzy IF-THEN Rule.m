clearvars
clc

y = linspace(0, 5, 1e2);  % 전체 y 범위 설정
x1 = 40;                  % x1값 정의
x2 = linspace(0, 10, 11); % x2값 범의 정의

mu_slow = 1./(1+(exp((x1-45) ./ 5)));    % slow fuzzy set 정의
mu_small = 1./(1+(exp((x2-5) ./ 2)));    % small fuzzy set 정의
mu_large = 1./(1+(exp((-y+1.25) .* 2))); % large fuzzy set 정의

% IF-TEHN rules
mu_p = mu_slow .* mu_small; % table p 연산하여 정의
mu_q = mu_large;            % table q 연산하여 정의
mu_Q = mu_q .* mu_p';       % mamdani implication 연산하여 정의


plot(y, mu_Q, 'LineWidth', 2) % 그래프 그려준다, 선두께 2
grid on             % 격자무늬 생성
xlabel('y')         % xlabel 이름
ylabel('\muQ(y)')   % ylabel 이름
