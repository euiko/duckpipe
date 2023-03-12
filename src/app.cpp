#include "duckdb/main/database.hpp"
#include <duckpipe/app.h>

#include <memory>

#include <boost/program_options.hpp>
#include <drogon/drogon.h>
#include <duckdb.hpp>

namespace po = boost::program_options;

// internal type declaration
namespace duckpipe {
struct HttpServerOptions;
struct DuckServiceOptions;
struct Options;

static void start_http_server(const HttpServerOptions &);

static void start_duckdb_service(const DuckServiceOptions &);
} // namespace duckpipe

// start of the definition/implementation
namespace duckpipe {
struct HttpServerOptions {};
struct DuckServiceOptions {};
struct Options {
    HttpServerOptions http_server;
    DuckServiceOptions duckdb_service;
};

void start_http_server(const HttpServerOptions &) {
    drogon::app()
        .setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 18080)
        .setThreadNum(16)
        .run();
}

void start_duckdb_service(const DuckServiceOptions &) {
    using namespace duckdb;

    DuckDB db(nullptr);
    Connection con(db);
    con.Query("CREATE TABLE integers(i INTEGER, j INTEGER)");

    // insert three rows into the table
    con.Query("INSERT INTO integers VALUES (3, 4), (5, 6), (7, NULL)");

    std::unique_ptr<MaterializedQueryResult> result =
        con.Query("SELECT * FROM integers");
    if (result->HasError()) {
        std::cerr << result->GetError() << std::endl;
    }

    std::cout << "result :" << std::endl << result->ToString() << std::endl;
}

void run(int argc, char **argv) {
    Options opts;

    start_duckdb_service(opts.duckdb_service);
    start_http_server(opts.http_server);
}

} // namespace duckpipe