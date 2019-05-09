function valid = check_input(val, for_type, limits)

% Assume negative output
valid = false;

% Check for whole number
if strcmp(for_type, 'int')
    
    % Check if it is a number
    val_num = str2double(val);
    if isnan(val_num)
        return;
    end
    
    % Check if it is a whole integer
    if rem(val_num, 1)
        return;
    end
    
    % Check if it is between limits
    if val_num<limits(1) || val_num>limits(2)
        return;
    end
    
    % Return with positive result
    valid = true;
    return;
    
% Check for decimal unit
elseif strcmp(for_type, 'double')
    
    % Check if it is a number
    val_num = str2double(val);
    if isnan(val_num)
        return;
    end
    
    % Check if it is between limits
    if val_num<limits(1) || val_num>limits(2)
        return;
    end
    
    % Return with positive result
    valid = true;
    return;
    
else
    
    error('Invalid number of for_type specified');
    
end