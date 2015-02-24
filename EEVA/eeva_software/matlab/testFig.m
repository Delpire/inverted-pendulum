function varargout = testFig(varargin)
% TESTFIG MATLAB code for testFig.fig
%      TESTFIG, by itself, creates a new TESTFIG or raises the existing
%      singleton*.
%
%      H = TESTFIG returns the handle to a new TESTFIG or the handle to
%      the existing singleton*.
%
%      TESTFIG('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TESTFIG.M with the given input arguments.
%
%      TESTFIG('Property','Value',...) creates a new TESTFIG or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before testFig_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to testFig_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help testFig

% Last Modified by GUIDE v2.5 17-Sep-2014 12:20:28

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @testFig_OpeningFcn, ...
                   'gui_OutputFcn',  @testFig_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before testFig is made visible.
function testFig_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to testFig (see VARARGIN)

    % Choose default command line output for testFig
    handles.output = hObject;
    
    serialPort = serial('COM38','BaudRate',57600,'DataBits',8,'Parity','None','FlowControl','None','StopBits',1);
    set(serialPort,'InputBufferSize',84);  % three transmissions long
    
    handles.serialPort = serialPort; % Add the serial port to the handles

%     port = portSelectorGUI;
%     if (~isempty(port))
%       handles.serialPort.Port=port;
%       fopen(handles.serialPort);
%     end
     
    fopen(handles.serialPort);

    handles.timer = timer(...
        'ExecutionMode', 'fixedRate', ...       % Run timer repeatedly
        'Period', 0.5, ...                      % set period in seconds
        'TimerFcn', {@timerCallback,hObject});  % Specify callback function

    % Update handles structure
    guidata(hObject, handles);
    
%    start(handles.timer);
    
% UIWAIT makes testFig wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = testFig_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on key press with focus on edit1 and none of its controls.
function edit1_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  structure with the following fields (see UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)

disp(eventdata)


% --- Executes on key press with focus on figure1 and none of its controls.
function figure1_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  structure with the following fields (see FIGURE)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)

    %disp(eventdata)
    if strcmp(eventdata.Key,'leftarrow')
        sendByte(1,handles)
    end
    if strcmp(eventdata.Key,'rightarrow')
        sendByte(2,handles)
    end
    if strcmp(eventdata.Key,'uparrow')
        sendByte(3,handles)
    end
    if strcmp(eventdata.Key,'downarrow')
        sendByte(4,handles)
    end
    if strcmp(eventdata.Key,'space')
        sendByte(5,handles)
    end
    
    updateDisplay(handles);

function sendByte(byte,handles)

 CRLF = uint8([13 10]);
 bytes = ['<{(' uint8(byte) CRLF];

 %flushinput(handles.serialPort);
 fwrite(handles.serialPort,bytes);  %this blocks till sent, timeout doesn't seem to work

function [angle,battery,success] = receiveAllData(sPort)
  n = sPort.BytesAvailable;
  success = false;
  angle = 0;
  battery =0;
  if (n >= 28)                        % make sure you have at least two transmissions
    data = uint8(fread(sPort,n)'); 
    starts = strfind(data,'<{(');
    if (starts)
         start = starts(end-1);       % use the second closest to the end to get full one
         CRLF = uint8([13 10]);
         if(data(start+12:start+13) == CRLF)
           success = true;
           angle = typecast(data(start+4:start+7),'single');
           battery = typecast(data(start+8:start+11),'single');
         end
    end
    flushinput(sPort);
  end
  
% --- Executes during object deletion, before destroying properties.
function figure1_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
fclose(handles.serialPort)
stop(handles.timer);

function updateDisplay(handles)
    
    [angle,battery,success]=receiveAllData(handles.serialPort);
    if success
        th = (angle+90)*pi/180;
        x = [0 cos(th)];
        y = [0 sin(th)];
        cla(handles.axes1);         % clear the plot
        line('Xdata',x,'Ydata',y,'Parent',handles.axes1);
        str = num2str(battery);
        set(handles.battText,'String',str);  
    end
    
function timerCallback(hObject,eventdata,hfigure)
    
    handles = guidata(hfigure);
    %stop(handles.timer);  % use for debugging

    %updateDisplay(handles);
    
