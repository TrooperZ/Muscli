#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	// https://m.media-amazon.com/images/I/81yLya2IJtL._UF1000,1000_QL80_.jpg
	// https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
		
	// Because downloading audio from youtube is a beast, ill use ytdl to download audio but i will use sockets to download the album art.
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4 (2), SOCK_STREAM = TCP (1)
	
	
	return 0;
}