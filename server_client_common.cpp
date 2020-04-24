#include "server_client_common.h"

void checkSocketErrorExit(int checkVal, const int errorVal,
                          std::string err_msg) {
  if (checkVal == errorVal) {
    perror(err_msg.c_str());
    exit(EXIT_FAILURE);
  }
}