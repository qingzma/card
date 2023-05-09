#include "SQLParser.h"
#include "util/sqlhelper.h"
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
TEST(ParserTest, BasicParserTest) {
  std::string query = "SELECT COUNT(*) FROM TBL";
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);

  for (auto i = 0u; i < result.size(); ++i) {
    // Print a statement summary.
    // hsql::printStatementInfo(result.getStatement(i));
    const hsql::SQLStatement *statement = result.getStatement(i);

    EXPECT_TRUE(statement->isType(hsql::kStmtSelect));
    if (statement->isType(hsql::kStmtSelect)) {
      const auto *select =
          static_cast<const hsql::SelectStatement *>(statement);
      EXPECT_EQ(std::string(select->fromTable->getName()), "TBL");
    }
  }
};