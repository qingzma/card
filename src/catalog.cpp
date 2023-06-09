#include "../include/catalog.h"
#include <iostream>

namespace card {
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
    return true;
  } else {
    // found
    return false;
  }
}

void serialize_catalog_to_disk(databases_catalog &databases,
                               std::string &db_prefix) {
  cista::byte_buf buf = cista::serialize(databases);
  std::ofstream output;

  std::string catalog_file_path =
      std::filesystem::path(db_prefix) / std::filesystem::path(DB_FILE_NAME);
  output.open(catalog_file_path, std::ios::out | std::ios::binary);
  output << buf;
  output.close();
}

cista::byte_buf read_file(const char *filename) {
  //   std::ifstream file(filename, std::ios::binary);
  std::ifstream instream(filename, std::ios::in | std::ios::binary);
  std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)),
                            std::istreambuf_iterator<char>());
  return cista::byte_buf(data);
}

databases_catalog *deserialize_catalog_from_disk(std::string &db_prefix) {
  std::string catalog_file_path =
      std::filesystem::path(db_prefix) / std::filesystem::path(DB_FILE_NAME);
  std::cout << catalog_file_path << std::endl;
  cista::byte_buf databases_buffer = read_file(catalog_file_path.c_str());
  databases_catalog *catalog =
      cista::deserialize<databases_catalog>(databases_buffer);
  return catalog;
}

bool init_db(std::string db_prefix) {
  struct stat sta;
  if (stat(db_prefix.c_str(), &sta) == 0) {
    // path exist
    if (std::filesystem::is_empty(db_prefix)) {
      // ok, continue to init
      printf("Initializing database...\n");
      databases_catalog databases({});

      serialize_catalog_to_disk(databases, db_prefix);

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