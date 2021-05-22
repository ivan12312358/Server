# Server

Documentation in the file:
  index.html

To compile programm:

  make all

To run akinator:

  Interactive mode:
    
    ./akinator.out -i <filename>
  
    <filename> - name of your base (this option can be empty)    
    
  Server mode:
  
    ./akinator.out -s <port> <filename>
  
    <filename> - name of your base (this option can be empty)
    <port>     - server port
    
    To connect to server:
      
      telnet <ip> <port>

      <ip> - ip address of the server

To remove files:

  make clean
