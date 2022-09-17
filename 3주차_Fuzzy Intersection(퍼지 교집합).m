clearvars
clc
U = linspace(-10, 10, 1000);
mu_A = membership_A(U);
mu_B = membership_B(U);

AnB_standard = standard_union(mu_A, mu_B, U);
AnB_algebraic = algebraic_union(mu_A, mu_B, U);
AnB_bounded = bounded_union(mu_A, mu_B, U);
AnB_drastic = drastic_union(mu_A, mu_B, U);
AnB_yager = yager_union(mu_A, mu_B, U, 2);
subplot(2,1,1)

hold on
stem(U, mu_A);
stem(U, mu_B);
subplot(2,1,2)
hold on
plot(U, AnB_standard);
plot(U, AnB_yager);
plot(U, AnB_algebraic);
plot(U, AnB_bounded);
plot(U, AnB_drastic);
legend({'mu_A', 'mu_B', 'standard', 'yager', 'algebraic', 'bounded','drastic'});

function mu = membership_A(u)
    mu = 1./ (1+u.^2);
end
function mu = membership_B(u)
    mu = 1./ (1 + (3.*(u-2).^2));
end

function AnB = standard_union(mu_A, mu_B, U)
    N = length(U);
    AnB = zeros(N,1);
    for i = 1 : N
    AnB(i) = min(mu_A(i), mu_B(i));
end
end

function AnB = yager_union(mu_A, mu_B, U, w)
    N = length(U);
    AnB = zeros(N,1);
    for i = 1 : N
    AnB(i) = 1 - (min(1, ((1 - mu_A(i))^w + (1 - mu_B(i)^w)^(1/w))));
    end
end

function AnB = algebraic_union(mu_A, mu_B, U)
    N = length(U);
    AnB = zeros(N,1);
    for i = 1 : N
    AnB(i) = (mu_A(i)*mu_B(i));
    end
end

function AnB = bounded_union(mu_A, mu_B, U)
    N = length(U);
    AnB = zeros(N,1);
    for i = 1 : N
    AnB(i) = max(0, mu_A(i) + mu_B(i) - 1);
    end
end

function AnB = drastic_union(mu_A, mu_B, U)
    N = length(U);
    AnB = zeros(N,1);
    for i = 1 : N
    if mu_B(i) == 1
    AnB(i) = mu_A;
    elseif mu_A(i) == 1
    AnB(i) = mu_B;
    else
    AnB(i) = 0;
    end
    end
end