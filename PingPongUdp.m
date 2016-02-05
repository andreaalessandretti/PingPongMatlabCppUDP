clc
close all
clear all

hudpr = dsp.UDPReceiver;
hudps = dsp.UDPSender;
hudpr.LocalIPPort  = 1501;

hudps.RemoteIPPort = 1500;
hudps.LocalIPPort  = 1501;
hudps.RemoteIPAddress = '127.0.0.1';

while(1)
    
    %% WAIT TO RECIVE
    
    fprintf('Waiting to receive ... ');
    dataReceived = step(hudpr);
    
    while(isempty(dataReceived))
         dataReceived = step(hudpr);
         pause(0.1);
    end
    
    y = typecast(dataReceived, 'double');
    fprintf('received %f\n',y);
    
    
    %% SLEEP RANDOM SEC IN [0,1]
    pause(rand);
    
    
    %% SEND RECIVED MESSAGE + 1
    step(hudps, double(y+1));
    fprintf('Send %f\n',double(y+1)); 
    
end
