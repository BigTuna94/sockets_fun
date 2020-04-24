#include "server_client_common.h"

struct free_delete {
  void operator()(void *x) { free(x); }
};

int main(int argc, char const *argv[]) {
  int serverFd, newSocket, valueRead;
  struct sockaddr_in address;
  socklen_t addrLen = sizeof(address);
  int reuse_opt = 1;

  std::unique_ptr<char, free_delete> up_buffer((char *)malloc(BUFFER_SIZE));
  std::string msg = "Hello from the server socket!";

  address.sin_family = DOMAIN_FAMILY;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT); // convert to network byte-order

  // Create the Socket FD
  serverFd = socket(DOMAIN_FAMILY, SOCK_STREAM, 0);
  checkSocketErrorExit(serverFd, SOCKET_ERROR,
                       "Socket creation failed [socket() returned" +
                           std::to_string(serverFd));

  // Enable address and port re-use
  checkSocketErrorExit(
      setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR /*| SO_REUSEPORT*/,
                 &reuse_opt, sizeof(reuse_opt)),
      SOCKET_ERROR, "Setting socket reuse addr and port option failed");

  // attach socket to port
  checkSocketErrorExit(
      bind(serverFd, (const sockaddr *)&address, sizeof(address)), SOCKET_ERROR,
      "Binding socket failed");

  checkSocketErrorExit(listen(serverFd, 3), SOCKET_ERROR,
                       "Listen on socket failed");

  newSocket = accept(serverFd, (struct sockaddr *)&address, &addrLen);
  checkSocketErrorExit(newSocket, SOCKET_ERROR, "Socket accept failed");

  while(true) {
    memset(up_buffer.get(), 0, BUFFER_SIZE);
    valueRead = read(newSocket, up_buffer.get(), BUFFER_SIZE);
    if (valueRead != 0) {
      std::cout << "Message Received! read() returned: " << valueRead 
                << "Buffer strlen: " << strlen((char *)up_buffer.get()) 
                << std::endl;
      std::cout << "Message: " << std::endl << "\t" << up_buffer.get() << std::endl;
    } else {
      // If read() returns 0, we need to try and reconnect (wait for client reconnect)
      std::cout << "read() returned 0! trying to accept new socket connection...";
      newSocket = accept(serverFd, (struct sockaddr *)&address, &addrLen);
      checkSocketErrorExit(newSocket, SOCKET_ERROR, "Socket accept failed");
      std::cout << "done." << std::endl;
    }
  }

  send(newSocket, msg.c_str(), msg.length(), 0);
  std::cout << "Message from sever sent to client" << std::endl;

  std::cout << "Exiting server..." << std::endl;
  return EXIT_SUCCESS;
}
