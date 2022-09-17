clearvars
clc
U = linspace(-10, 10, 1000);
mu_A = membership_A(U);
mu_B = membership_B(U);

AuB_standard = standard_union(mu_A, mu_B, U);
AuB_algebraic = algebraic_union(mu_A, mu_B, U);
AuB_bounded = bounded_union(mu_A, mu_B, U);
AuB_drastic = drastic_union(mu_A, mu_B, U);
AuB_yager = yager_union(mu_A, mu_B, U, 2);
subplot(2,1,1)

hold on
stem(U, mu_A);
stem(U, mu_B);
subplot(2,1,2)
hold on
plot(U, AuB_standard);
plot(U, AuB_yager);
plot(U, AuB_algebraic);
plot(U, AuB_bounded);
plot(U, AuB_drastic);
legend({'mu_A', 'mu_B', 'standard', 'yager', 'algebraic', 'bounded','drastic'});

function mu = membership_A(u)
    mu = 1./ (1+u.^2);
end
function mu = membership_B(u)
    mu = 1./ (1 + (3.*(u-2).^2));
end

function AuB = standard_union(mu_A, mu_B, U)
    N = length(U);
    AuB = zeros(N,1);
    for i = 1 : N
    AuB(i) = max(mu_A(i), mu_B(i));
end
end

function AuB = yager_union(mu_A, mu_B, U, w)
    N = length(U);
    AuB = zeros(N,1);
    for i = 1 : N
    AuB(i) = min(1, ((mu_A(i))^w + (mu_B(i)^w)^(1/w)));
    end
end

function AuB = algebraic_union(mu_A, mu_B, U)
    N = length(U);
    AuB = zeros(N,1);
    for i = 1 : N
    AuB(i) = mu_A(i) + mu_B(i) - (mu_A(i)*mu_B(i));
    end
end

function AuB = bounded_union(mu_A, mu_B, U)
    N = length(U);
    AuB = zeros(N,1);
    for i = 1 : N
    AuB(i) = min(mu_A + mu_B);
    end
end

function AuB = drastic_union(mu_A, mu_B, U)
    N = length(U);
    AuB = zeros(N,1);
    for i = 1 : N
    if mu_B(i) == 0
    AuB(i) = mu_A;
    elseif mu_A(i) == 0
    AuB(i) = mu_B;
    else
    AuB(i) = 1;
    end
    end
end