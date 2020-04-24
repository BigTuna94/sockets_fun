#include "arpa/inet.h"

#include "server_client_common.h"

int main(int argc, char const *argv[]) {

  int sock = 0;
  int valRead;
  struct sockaddr_in serverAddress;
  char hello[] = "New Client Connection!";
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));

  serverAddress.sin_family = DOMAIN_FAMILY;
  serverAddress.sin_port = htons(PORT);

  checkSocketErrorExit(
    (sock = socket(DOMAIN_FAMILY, SOCK_STREAM, 0)),
    SOCKET_ERROR,
    "Socket creation error"
  );

  checkSocketErrorExit( // convert address to network format (binary)
     inet_pton(DOMAIN_FAMILY, "127.0.0.1", &serverAddress.sin_addr),
     0,
     "Invalid address/address not supported. Could not convert"
  );

  checkSocketErrorExit(
    connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)),
    SOCKET_ERROR,
    "Connection failed"
  );

  send(sock, hello, strlen(hello), 0);
  std::cout << "Hello message sent from client" << std::endl;

  while (true) {
    std::string new_message_str;
    std::cout << "Enter a message to send to client: ";
    std::getline(std::cin, new_message_str);
    std::cout << "Sending message \"" << new_message_str
              << "\" with length: " << new_message_str.length()
              << " and strlen:" << strlen(new_message_str.c_str())
              << std::endl;
    size_t sendResult = send(sock, new_message_str.c_str(), new_message_str.length(), 0);
    std::cout << "Message Sent. Returned: " << sendResult << std::endl;
  }

  valRead = read(sock, buffer, sizeof(buffer));
  std::cout << "Message received from server: " << buffer << std::endl;

  std::cout << "Client exiting..." << std::endl;
  return 0;
}
