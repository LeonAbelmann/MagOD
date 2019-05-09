function message_append(handles, text)
curMes = get(handles.messages_text, 'String');
curMes = char(curMes, text);
set(handles.messages_text, 'String', curMes);
pause(0.001);