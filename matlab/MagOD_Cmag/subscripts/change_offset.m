function OD = change_offset(handles,OD)
offsetred=handles.settings.offsetred;
offsetgreen=handles.settings.offsetgreen;
offsetblue=handles.settings.offsetblue;
Vav = handles.data.all.Vav;
LED = handles.data.all.LED;

for i=1:length(Vav)
    switch LED(i)
        case 1
            OD(i) = -log10(Vav(i)/offsetred);%you cannot divide by a vector, so -log10 instead            
        case 2
            OD(i) = -log10(Vav(i)/offsetgreen);          
        case 3
            OD(i) = -log10(Vav(i)/offsetblue);            
    end
end

message_new(handles, sprintf('Change offset red to %f.',offsetred));
message_new(handles, sprintf('Change offset green to %f.', offsetgreen)); 
message_new(handles, sprintf('Change offset blue to %f.', offsetblue));
