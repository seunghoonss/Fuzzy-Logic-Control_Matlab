
clearvars
clc

mf_R = [0.4, 0.1; 0.3, 0.2; 0.0, 0.7];  % 퍼지 R
mf_S = [0.3, 0.6; 0.9, 0.0];            % 퍼지 S

mf_C = fuzzy_composition(mf_R, mf_S, "min"); % R*s 합성 max-min
mf_Cp = fuzzy_composition(mf_R, mf_S, "product"); % R%s 합성 max-product


function mf_C = fuzzy_composition(mf_R, mf_S, type) % 함수 곱
    if size(mf_R, 2) ~= size(mf_S, 1) % martix 함수 곱 성립 조건
        error('wrong matrix shape.') % 아닐 경우 error 메시지 출력
    end

    n = size(mf_R, 1); % mf_R 행 차원 길이 3
    m = size(mf_S, 2); % mf_S 열 차원 길이 2
    l = size(mf_R, 2);
    mf_C = zeros(n,m); % (3, 2) 크기를 가지는 0의 원소로 구성된 mf_C 행렬

    if lower(type) == "min" % type이 min 일때 
        for i = 1: n % i가 1에서 3까지의 반복문
            for j = 1: m % j가 1에서 2까지의 반복문
                mf_C(i,j) = max(min(mf_R(i, :), mf_S(:, j)')); % max-min 식
            end
        end
    elseif lower(type) == "product" % type이 prodcut 일때
        for i = 1: n                % i가 1에서 3까지의 반복문
            for j = 1 : m           % j가 1에서 2까지의 반복문
                mf_C(i,j) = max((mf_R(i, :)) .*(mf_S(:, j)')); % max-product 식
            end
        end
    else
        error("type should be 'min' or 'product'."); % 아닐 경우 erro 메시지 출력
    end
end