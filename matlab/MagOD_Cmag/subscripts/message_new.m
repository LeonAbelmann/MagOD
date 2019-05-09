function message_new(handles, text)
set(handles.messages_text, 'String', sprintf(text));
pause(0.001);