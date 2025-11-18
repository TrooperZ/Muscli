#include <_ctype.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <string>

struct URL {
    std::string protocol;
    std::string host;
    std::string path;
    uint16_t port;

    URL() : protocol("http"), host(""), path("/"), port(80) {}
    /**
     * @brief Construct a new URL object with a URL string
     *
     * This breaks the string into the protocol, host, path, and port
     * components. If no protocol or port is specified, http 80 is used by
     * default.
     *
     * @param url the target URL
     */
    URL(std::string url) {
        size_t urlSize = url.size();
		
		// 1. Find
        // Find dot from the end
        size_t hostDotIndex = std::string::npos;
        size_t hostLeftIndex = std::string::npos;   // Inclusive
        size_t hostRightIndex = std::string::npos;  // Inclusive
        for (size_t i = 0; i < urlSize; ++i) {
            if (url[urlSize - 1 - i] == '.') {
                hostDotIndex = urlSize - 1 - i;
                break;
            }
        }
        // No dot found or the dot is just hanging at the front or back
        // means an invalid URL
        if (hostDotIndex == std::string::npos || hostDotIndex == urlSize - 1 ||
            hostDotIndex == 0) {
            throw std::invalid_argument(
                "The given url does not appear to be a proper URL. (Could not "
                "parse "
                "host.)");
        }

        // Read right of dot
        for (size_t i = hostDotIndex + 1; i < urlSize; ++i) {
            if (i == urlSize - 1) {
                hostRightIndex = i;
                break;
            }
            if (url[i] == '/' || url[i] == ':') {
                hostRightIndex = i - 1;
                break;
            }
        }

        // Read left of dot
        for (size_t i = hostDotIndex - 1; i >= 0; --i) {
            if (i == 0) {
                hostLeftIndex = i;
                break;
            }
            if (url[i] == '/') {
                hostLeftIndex = i + 1;
                break;
            }
        }

        // If left == right, something likely went wrong and this is not a
        // correct URL
        if (hostLeftIndex == hostRightIndex) {
            throw std::invalid_argument(
                "The given url does not appear to be a proper URL. (Could not "
                "parse "
                "host.)");
        }

        // Extract string
        host = url.substr(hostLeftIndex, hostRightIndex - hostLeftIndex + 1);

        // Next, we extract the protocol. We read from index 0 till a ':'. If
        // hostLeftIndex == 0, we assume HTTP (we should probably assume HTTPS,
        // but we can always upgrade HTTP later once we attempt the connection).
        if (hostLeftIndex == 0) {
            protocol = "http";
        } else {
            for (size_t i = 0; i < urlSize; ++i) {
                if (url[i] == ':') {
                    protocol = url.substr(0, i);
                    break;
                }
            }
        }
        // We couldn't figure out the protocol so we just throw an error.
        if (protocol.empty()) {
            throw std::invalid_argument(
                "The given url does not appear to be a proper URL. (Could "
                "not parse "
                "host.)");
        }

        // Next we extract the port, usually since we assume HTTP we will set
        // 80, but if the user specifies a port (www.url.com:1234), we must use
        // that. We just check if url[hostRightIndex] == ':' and that means we
        // have a port specified. We then read until the end or a '/'
        std::string portString;
        size_t portEndIndex = hostRightIndex;
        portString.reserve(5);  // Biggest port number possible is 65535
        if (url[hostRightIndex] == ':') {
            for (size_t i = hostRightIndex + 1; i < urlSize; ++i) {
                if (isnumber(url[i])) {
                    portString.push_back(url[i]);
                } else {
                    portEndIndex += i - 1;
                    break;
                }
            }
        }
        if (!portString.empty()) {
            port = std::stoi(portString);
        } else {
            port = 80;
        }

        // Finally, extract the path. We start at the end of the port (usually
        // will be equal to hostRightIndex if port not specificed) and go to the
        // end of the string.
        path = url.substr(portEndIndex + 1, urlSize - portEndIndex + 1);
    };
};

// int main() {
//     // https://m.media-amazon.com/images/I/81yLya2IJtL._UF1000,1000_QL80_.jpg
//     // https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/

//     return 0;
// }

int main() {
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