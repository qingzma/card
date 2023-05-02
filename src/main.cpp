#include <bloom_filter.hpp>
#include <bloom_filter/basic.hpp>
#include <bloom_filter/counting.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>

// include the sql parser
#include "SQLParser.h"

// contains printing utilities
#include "util/sqlhelper.h"

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "Usage: ./card \"SELECT * FROM test;\"\n");
    return -1;
  }

  // bf::basic_bloom_filter b(0.8, 100);
  bf::counting_bloom_filter b(bf::make_hasher(5), 1000000, 16);
  b.clear();
  b.add("foo");
  b.add(32);
  b.add("foo");
  b.add(33);
  b.add("foo");

  std::cout << b.lookup("foo") << std::endl;
  std::cout << b.lookup(32) << std::endl;
  std::cout << b.lookup(33) << std::endl;
  std::cout << b.lookup(40) << std::endl;

  std::string query = argv[1];

  // parse a given query
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);

  // check whether the parsing was successful

  if (result.isValid()) {
    printf("Parsed successfully!\n");
    printf("Number of statements: %lu\n", result.size());

    for (auto i = 0u; i < result.size(); ++i) {
      // Print a statement summary.
      hsql::printStatementInfo(result.getStatement(i));
      const hsql::SQLStatement *statement = result.getStatement(i);

      if (statement->isType(hsql::kStmtSelect)) {
        const auto *select =
            static_cast<const hsql::SelectStatement *>(statement);
        // printf("haha %s", select->fromTable->getName());
      }
    }
    return 0;
  } else {
    fprintf(stderr, "Given string is not a valid SQL query.\n");
    fprintf(stderr, "%s (L%d:%d)\n", result.errorMsg(), result.errorLine(),
            result.errorColumn());
    return -1;
  }
}
