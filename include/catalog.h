#ifndef CARD_CATALOG_H_
#define CARD_CATALOG_H_
#include "SQLParser.h"
#include "cista.h"
#include "configuration.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <sys/stat.h>
namespace card {

constexpr auto const SERIALIZATION_MODE = // opt. versioning + check sum
    cista::mode::WITH_VERSION | cista::mode::WITH_INTEGRITY;
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
                  database_catalog &database) noexcept {
  if (databases.databases.find(database.database_name) ==
      databases.databases.end()) {
    // not exist, so add it
    databases.databases.emplace(database.database_name, database);
    return true;
  } else {
    // database already exists, stop
    return false;
  }
}

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
                                  table_catalog &table) noexcept {
  cista::raw::string table_name(statment.tableName);
  if (database.database_schema.find(table_name) ==
      database.database_schema.end()) {
    // not found
    table.table_name = table_name;
    table.table_source_file_path = "";
    std::string p1(database.database_path.data());
    std::string p2(table_name.data());
    table.table_storage_path =
        (std::filesystem::path(p1) / std::filesystem::path(p2)).c_str();
    int column_loc = 0;
    for (hsql::ColumnDefinition *column : *statment.columns) {
      cista::raw::string column_name(column->name);
      hsql::DataType column_type(column->type.data_type);
      column_catalog col({column_name, column_type, column_loc++});
      table.table_schema.emplace(column_name, col);
    }
  } else {
    // found
    return false;
  }
}

bool init_db(std::string db_prefix) {
  struct stat sta;
  if (stat(db_prefix.c_str(), &sta) == 0) {
    // path exist
    if (std::filesystem::is_empty(db_prefix)) {
      // ok, continue to init
      printf("Initializing database...\n");
      databases_catalog databases({});
      cista::byte_buf buf = cista::serialize<SERIALIZATION_MODE>(databases);
      std::ofstream output;

      std::string catalog_file_path = std::filesystem::path(db_prefix) /
                                      std::filesystem::path(DB_FILE_NAME);
      output.open(catalog_file_path);
      output << buf;
      output.close();
      printf("Database is initialized.\n");
      return true;
    } else {
      printf("Fail to init_db as directory is not empty.\n");
      return false;
    }
  } else {
    // path does not exist, so create one.
    bool res = std::filesystem::create_directory(db_prefix);
    if (res) {
      printf("Successfully created db folder.\n");
      init_db(db_prefix);
      return true;
    } else {
      printf("Fail to init_db as unable to create the directory.\n");
      return false;
    }
  }
}

} // namespace card

#endif // CARD_CATALOG_H_