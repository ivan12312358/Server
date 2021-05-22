# Server

To compile programm:

  make all

To run akinator:

  Interactive mode:
    
    ./akinator -i <filename>
  
    <filename> - name of your base (this option can be empty)    
    
  Server mode:
  
    ./akinator -s <port> <filename>
  
    <filename> - name of your base (this option can be empty)
    <port>     - server port
    
    To connect to server:
      
      telnet <ip> <port>

      <ip> - ip address of the server

To remove files:

  make clean
