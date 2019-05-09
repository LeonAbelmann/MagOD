function y = sum_of_cosine(param, x)
%     N = param(1);
    N = length(param);
    y = 0;
    for i=1:N
        y = y + param(i) * cos( 2*(i-1) * x);
    end
    
end