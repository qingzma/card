#ifndef CARD_CONFIGURATION_H_
#define CARD_CONFIGURATION_H_
#include <string>
namespace card {
static const char *DB_FILE_NAME = "catalog";

struct runtime_config {
  std::string prefix;
};
} // namespace card

#endif // CARD_CONFIGURATION_H_