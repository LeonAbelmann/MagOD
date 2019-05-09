function ODplot(data,settings)
% Decide what to plot on the x-axis
if strcmp(settings.time_scale, 'days')
    x_data = data.all.time_day;
    xlab = 'Time [days]';
elseif strcmp(settings.time_scale, 'hours')
    x_data = data.all.time_hr;
    xlab = 'Time [hrs]';
elseif strcmp(settings.time_scale, 'minutes')
    x_data = data.all.time_min;
    xlab = 'Time [min]';
elseif strcmp(settings.time_scale, 'seconds')
    x_data = data.all.time_sec;
    xlab = 'Time [sec]';
else
    error('Wrong time...');
end

y_data = data.all.OD;

if settings.LEDcolor
    % Plot graph with linecolor identical to LED color. Uses scatter, which is
    % slow. TODO, make a toggle in the program
    y_color = zeros(length(y_data),3);
    for i=1:length(y_data)
        switch data.all.LED(i)
            case 1 % red
                y_color(i,:) = settings.colors.red;
            case 2 % green 
                y_color(i,:) = settings.colors.green;
            case 3 % blue
                y_color(i,:) = settings.colors.blue;
        end
    end

    % Scatter plot is slow, so limit the amount points plotted
    % For now, just limit to 1000
    skip = floor(length(x_data)/1000);
    scatter(x_data(1:10:end), y_data(1:10:end),1,y_color(1:10:end,:));
else
    %Plot data without colors
    plot(x_data, y_data)
end

box on
xlabel(xlab);
ylabel('OD');
xlim([min(x_data), max(x_data)]);
