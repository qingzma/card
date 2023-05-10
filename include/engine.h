// #ifndef CARD_INCLUDE_ENGINE_H_
// #define CARD_INCLUDE_ENGINE_H_
// #include "SQLParser.h"
// #include "catalog.h"
// #include "util/sqlhelper.h"

// namespace card {
// class Engine {
// private:
//   hsql::SQLParserResult parsedResult;
//   databases_catalog *databases_cat;

// public:
//   Engine() = delete;
//   Engine(const Engine &) = delete;
//   Engine &operator=(const Engine &) = delete;
//   Engine(runtime_config &run_config);

//   virtual ~Engine();

//   void AcceptQuery(std::string query);
// };
// } // namespace card

// #endif // CARD_INCLUDE_ENGINE_H_