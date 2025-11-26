#include <_ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>


// g++ main.cpp \
//   -g                
//   -I/opt/homebrew/opt/openssl@3/include \
//   -L/opt/homebrew/opt/openssl@3/lib \
//   -lssl -lcrypto
struct URL {
    std::string protocol;
    std::string host;
    std::string path;
    std::string port;
    // https://example.com:port/dir/ect/ory

    URL() : protocol("http"), host(""), path("/"), port("80") {}
    /**
     * @brief Construct a new URL object with a URL string
     *
     * This breaks the string into the protocol, host, path, and port
     * components. If no protocol or port is specified, http 80 is used by
     * default.
     *
     * @param url the target URL
     */
    URL(std::string url) : protocol("http"), port("80"), path("/") {
        size_t p = 0;

        // Find the protocol
        size_t protocolEnd = url.find("://");
        if (protocolEnd != std::string::npos) {
            protocol = url.substr(0, protocolEnd);
            p = protocolEnd + 3;
        }

        // Find the host
        size_t hostEnd = url.find_first_of(":/", p);
        host = url.substr(p, hostEnd - p);
        p = hostEnd;

        // Find the port if there is one
        if (p != std::string::npos && url[p] == ':') {
            size_t portStart = p + 1;
            size_t portEnd = url.find('/', portStart);
            std::string portStr = url.substr(portStart, portEnd - portStart);

            port = portStr;

            p = portEnd;
        }

        // Find path
        if (p != std::string::npos && url[p] == '/') {
            path = url.substr(p);
        }
    };
};

int main() {
    // https://m.media-amazon.com/images/I/81yLya2IJtL._UF1000,1000_QL80_.jpg
    // https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/

    // http://httpforever.com/

    // clang-format off
    URL a(
    "https://m.media-amazon.com/images/I/81yLya2IJtL._UF1000,1000_QL80_.jpg");
    // clang-format on

    // Address info struct to put data into
    struct addrinfo *res = nullptr;

    // getaddrinfo is set of wrapped system calls that returns a linked list of
    // resolved IPs which can be used to open sockets to
    int address = getaddrinfo(a.host.data(), a.port.data(), 0, &res);

    // Go through each resolved address linked list style.
    while (res) {
        // buf stores the IP address
        char buf[res->ai_addrlen];

        // inet_top takes the binary representation of the IP address and
        // formats it to a human readable view. AF_INET specifies IPv4, we must
        // also pass a void pointer to the data, and we pass the buffer and the
        // size of the buffer.
        inet_ntop(AF_INET, &(((sockaddr_in *)res->ai_addr)->sin_addr), buf,
                  sizeof(buf));

        std::cout << "Found address " << buf << '\n';

        std::cout << "Connecting to address...\nCreating socket...\n";
        // We then create a socket to connect to that IP, matching the
        // response's family, socktype, and protocl
        int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        std::cout << "Socket status " << sock << "\nConnecting to socket...\n";

        // Connect to the socket
        int conn = connect(sock, res->ai_addr, res->ai_addrlen);

        int savedConnStatus = conn;
        std::cout << "Connected to socket, status " << conn << '\n';

        // Build the HTTP request
        std::string requestData =
            "GET " + a.path +
            " HTTP/1.1\r\n"
            "Host: " +
            a.host +
            "\r\n"
            "Connection: close\r\n"  // ensures server closes after sending
            "\r\n";

        std::cout << "Sending http request with the data: " << requestData
                  << '\n';

        // Send the HTTP request to the address
        int sending = send(sock, requestData.data(), requestData.size(), 0);

        std::cout << "Sent bytes: " << sending << '\n';

        // 4 KB buffer at a time
        char buffer[4096];
        int n;

        std::ofstream ofs;
        ofs.open("output.jpeg", std::ios::binary);
        if (!ofs) {
            std::cout << "Error opening file. \n";
            return -1;
        }

        bool headersCleared = false;
        while ((n = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
            // Process headers so we don't write them to the image
            if (!headersCleared) {
                std::string chunk(buffer, n);
                std::cout << "Headers: \n\n " << chunk;
                size_t endHeadersIndex = chunk.find("\r\n\r\n");
                
                if (endHeadersIndex != std::string::npos) {
                    std::string body =
                        chunk.substr(endHeadersIndex + 4);  // body begins here
                    ofs.write(body.data(), body.size());

                    headersCleared = !headersCleared;
                    continue;
                } else {
                    std::cout << "Could not find headers... \n";
                    return -1;
                }
            }
            std::cout.write(buffer, n);
            ofs.write(buffer, n);  // write exactly n bytes
        }

        std::cout << "\nConnection closed by server, total bytes read: " << n
                  << "\n";

        close(sock);
        if (savedConnStatus != 0) {
            res = res->ai_next;
        } else {
            res = nullptr;
        }
    }

    return 0;
}

int testURL() {
    const std::string testUrls[] = {
        "http://www.example.com",
        "https://www.example.com",
        "http://www.example.com:8080",
        "https://www.example.com/path/to/resource",
        "http://127.0.0.1:3000/test",
        "www.example.com",   // No protocol, should default to http
        "example.com:1234",  // No protocol, custom port
        "http://sub.domain.example.com/path",
        "192.168.1.1",                  // IP only
        "http://192.168.1.1:8080/path"  // IP with port and path
    };

    for (const auto &urlStr : testUrls) {
        try {
            URL url(urlStr);
            std::cout << "Original URL: " << urlStr << "\n";
            std::cout << "Protocol: " << url.protocol << "\n";
            std::cout << "Host: " << url.host << "\n";
            std::cout << "Port: " << url.port << "\n";
            std::cout << "Path: " << url.path << "\n";
            std::cout << "---------------------------\n";
        } catch (const std::invalid_argument &e) {
            std::cout << "Failed to parse URL: " << urlStr << "\n";
            std::cout << "Reason: " << e.what() << "\n";
            std::cout << "---------------------------\n";
        }
    }

    return 0;
}