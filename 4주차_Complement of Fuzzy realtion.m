
clearvars
clc

mf_R = [0.1, 0.0, 0.0; 1.0, 1.0, 0.5; 0.8, 0.2, 0.6]; % 퍼지 R
mf_S = [0.3, 0.7, 0.1; 0.0, 1.0, 0.9; 0.2, 0.2, 0.2]; % 퍼지 S

mf_Rbar = fuzzy_relation_complement(mf_R); % 여관계
mf_u = fuzzy_relation_union(mf_R, mf_S);   % 합관계
mf_i = fuzzy_relation_intersection(mf_R, mf_S); % 교관계

function mf_bar = fuzzy_relation_complement(mf) 
     mf_bar = 1 - mf; % 여관계 함수식
end

function mf_C = fuzzy_relation_union(mf_A, mf_B)
    mf_C = max(mf_A, mf_B); % 합관계 함수식
end

function mf_C = fuzzy_relation_intersection(mf_A, mf_B)
    mf_C = min(mf_A, mf_B); % 교관계 함수식
end