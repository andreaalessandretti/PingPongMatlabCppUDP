
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>


using namespace std;

int main()
{

	// UDP CONNECTION SETUP

	struct sockaddr_in cpp_addr, matlab_addr,sender_addr;

	//c++ UDP port
	cpp_addr.sin_family = AF_INET;
	cpp_addr.sin_addr.s_addr = inet_addr("127.0.0.1") ;
	cpp_addr.sin_port = htons(1500); // C++ UDP Port Number

	//Matlab UDP port
	matlab_addr.sin_family = AF_INET;
	matlab_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	matlab_addr.sin_port = htons(1501); // Matlab UDP Port Number

	// CREATE AND BIND SOCKET TO LOCAL ADDRESS

	int client = socket(AF_INET, SOCK_DGRAM, 0);

	if (client < 0)
	{
		printf("\nError creating socket.");
		exit(1);
	}

	printf("\nSocket created.");


	if (( bind(client, (struct sockaddr*)&cpp_addr,sizeof(cpp_addr)) ) < 0)
	{
		printf("\nError binding connection.");
		exit(1);
	}



	int recvlen;
	int bufSize = 2048;
	unsigned char buf[bufSize];
	socklen_t addrlen = sizeof(matlab_addr);

	double y[1]={0};

	char const * message;

	for (;;) { // Infinite Loop


		// SEND y

		message = reinterpret_cast<char const *>(y);

		if (sendto(client, message, 1*sizeof(y), 0, (struct sockaddr *)&matlab_addr, sizeof(matlab_addr)) < 0) {
			perror("\Error sending message");
			exit(1);
		}

		printf("\nSent %f",y[0]);


		// SLEEP ON ESEC

		sleep(1);


		// WAIT TO RECIVE

		printf("\nWaiting to receive ... ");

		recvlen = recvfrom(client, buf, bufSize, 0,  (struct sockaddr *)&sender_addr, &addrlen);

		//note: sender_addr is never used. This is because Matlab can send the message form a port
		//      that is automatically assigned by the OS. Although, Matlab keeps listening from the same port
		//      that is specified above, i.e., 1501.

		if (recvlen > 0) {
			buf[recvlen] = 0;
		}else{
			printf(" Noting received. \n");
			exit(1);
		}

		double (* bufDouble) = reinterpret_cast<double *>(buf);

		printf(" received %f", bufDouble[0]);


		// SET Y = RECIVED MESSAGE + 1

		y[0] =  bufDouble[0]+1;

	}

}
