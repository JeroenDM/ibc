// This GUI file is generated by guibuilder version 3.0
//////////
f=figure('figure_position',[400,50],'figure_size',[827,417],'auto_resize','on','background',[33],'figure_name','Graphic window number %d');
//////////
delmenu(f.figure_id,gettext('File'))
delmenu(f.figure_id,gettext('?'))
delmenu(f.figure_id,gettext('Tools'))
toolbar(f.figure_id,'off')
handles.dummy = 0;
handles.start=uicontrol(f,'unit','normalized','BackgroundColor',[-1,-1,-1],'Enable','on','FontAngle','normal','FontName','Tahoma','FontSize',[12],'FontUnits','points','FontWeight','normal','ForegroundColor',[-1,-1,-1],'HorizontalAlignment','center','ListboxTop',[],'Max',[1],'Min',[0],'Position',[0.0192744,0.7615385,0.1893424,0.1923077],'Relief','default','SliderStep',[0.01,0.1],'String','Start Acquisition','Style','pushbutton','Value',[0],'VerticalAlignment','middle','Visible','on','Tag','start','Callback','start_callback(handles)')
handles.stop=uicontrol(f,'unit','normalized','BackgroundColor',[-1,-1,-1],'Enable','on','FontAngle','normal','FontName','Tahoma','FontSize',[12],'FontUnits','points','FontWeight','normal','ForegroundColor',[-1,-1,-1],'HorizontalAlignment','center','ListboxTop',[],'Max',[1],'Min',[0],'Position',[0.0204082,0.5415385,0.1893424,0.1923077],'Relief','default','SliderStep',[0.01,0.1],'String','Stop Acquisition','Style','pushbutton','Value',[0],'VerticalAlignment','middle','Visible','on','Tag','stop','Callback','stop_callback(handles)')
handles.reset=uicontrol(f,'unit','normalized','BackgroundColor',[-1,-1,-1],'Enable','on','FontAngle','normal','FontName','Tahoma','FontSize',[12],'FontUnits','points','FontWeight','normal','ForegroundColor',[-1,-1,-1],'HorizontalAlignment','center','ListboxTop',[],'Max',[1],'Min',[0],'Position',[0.0226757,0.3315385,0.1893424,0.1923077],'Relief','default','SliderStep',[0.01,0.1],'String','Reset Figure','Style','pushbutton','Value',[0],'VerticalAlignment','middle','Visible','on','Tag','reset','Callback','reset_callback(handles)')
handles.figure= newaxes();handles.figure.margins = [ 0 0 0 0];handles.figure.axes_bounds = [0.2358277,0.0615385,0.7324263,0.5884615];


//////////
// Callbacks are defined as below. Please do not delete the comments as it will be used in coming version
//////////

function start_callback(handles)
//Write your callback for  start  here

endfunction


function stop_callback(handles)
//Write your callback for  stop  here

endfunction


function reset_callback(handles)
//Write your callback for  reset  here

endfunction


