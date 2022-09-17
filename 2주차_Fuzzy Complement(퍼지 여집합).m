clearvars
clc

U = linspace(-10, 10, 1000);
mu = gaussmf(U, [1 0]);


c_standard = standard_complement(mu);
c_sugeno = sugeno_complement(mu, 1);
c_yager = yager_complement(mu, 2);

hold on
plot(U, mu);
plot(U, c_standard);
plot(U, c_sugeno);
plot(U, c_yager);
grid on
legend({'mu_A', 'standard', 'sugeno', 'yager'});

function mu = membership_A(u)
    mu = 1./ (1+u.^2);
end
function c = standard_complement(mu)
N = length(mu);
c = zeros(N,1);
for i = 1 : N
    c(i) = 1 - mu(i);
    end
end
function c = sugeno_complement(mu, lambda)
N = length(mu);
c = zeros(N,1);
for i = 1 : N
    c(i) = (1-mu(i) ./ (1+lambda*mu(i)));
    end
end

function c = yager_complement(mu, w)
N = length(mu);
c = zeros(N,1);
    for i = 1 : N
        c(i) = (1-mu(i).^w)^(1/w);
    end
end