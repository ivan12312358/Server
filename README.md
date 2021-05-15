# Server

To compile programm:

  make all

To run akinator:

  Interactive mode:
    
    ./akinator -i <filename>
    
  Server mode:
  
    ./akinator -s <port> <filename>
  
    <filename> - name of your base (this option can be empty)
    <port>     - server port
    
    To connect to server:
      
      telnet localhost <port>

To remove files:

  make clean
