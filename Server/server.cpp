#include "../Libraries/server.h"


int main(int argc, char* argv[1])
{
	int listener;
	struct sockaddr_in addr;

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		perror("socket");
		exit(1);
	}

	int port = atoi(argv[1]);

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		exit(2);
	}

	listen(listener, 1);
	
	while(1)
	{

		sock = accept(listener, NULL, NULL);
		if(sock < 0)
		{
			perror("accept");
			exit(3);
		}

        switch(fork())
        {
        case -1:
            perror("fork");
            break;
            
        case 0:
            close(listener);
            Menu(nullptr);
            close(sock);
            _exit(0);
            
        default:
            close(sock);
        }

	}
	
	return 0;
}
