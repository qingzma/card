#ifndef CARD_CATALOG_H___
#define CARD_CATALOG_H___
#include "cista.h"
#include "configuration.h"
#include <SQLParser.h>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <sys/stat.h>
namespace card {

// constexpr auto const SERIALIZATION_MODE = // opt. versioning + check sum
//     cista::mode::WITH_VERSION | cista::mode::WITH_INTEGRITY;
struct column_catalog {
  cista::raw::string column_name;
  hsql::DataType column_type;
  int32_t column_index; // start with 0
};

struct table_catalog {
  cista::raw::string table_name;
  cista::raw::hash_map<cista::raw::string, column_catalog>
      table_schema; // column_name, column_details
  cista::raw::string table_source_file_path;
  cista::raw::string table_storage_path;
};

struct database_catalog {
  cista::raw::string database_name;
  cista::raw::hash_map<cista::raw::string, table_catalog>
      database_schema; // table_name, table_details
  cista::raw::string database_path;
};

struct databases_catalog {
  cista::raw::hash_map<cista::raw::string, database_catalog> databases;
};

bool add_database(databases_catalog &databases,
                  database_catalog &database) noexcept;

/**
 * @brief Create a table catalog and insert it to the schema
 *
 * @param statment the parsed query statement
 * @param table the table_catalog to be inserted
 * @return true if the table is createdly succesfully
 * @return false if the table already exist, stop then
 */
bool create_and_add_table_catalog(database_catalog &database,
                                  const hsql::CreateStatement &statment,
                                  table_catalog &table) noexcept;

void serialize_catalog_to_disk(databases_catalog &databases,
                               std::string &db_prefix);

databases_catalog *deserialize_catalog_from_disk(std::string &db_prefix);

cista::byte_buf read_file(const char *filename);

bool init_db(std::string db_prefix);

} // namespace card

#endif // CARD_CATALOG_H___