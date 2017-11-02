#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <string>
#include <sstream>
#include "moviesdb.h"

/*
* Added the use of sstringstream for pk values
* Have connect() function commented out, could not get it to work
*/

//mysqlpp::Query connect() {
    // Connect to database with: database, server, userID, password
//    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    // Create a query
    //mysqlpp::Query query1 = myDB.query();
    //query1 << "SELECT pk FROM test";
    //query1.parse();  // check to ensure it is correct
    //myDB.query() = query1;

//     return myDB.query();
//}


void getMovieListing(std::vector<std::string> &movies) {
        mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");

        // Create a query
        mysqlpp::Query query = myDB.query();
        query << "SELECT pk, title FROM movies";
        query.parse();  // check to ensure it is correct
        myDB.query() = query;

        mysqlpp::StoreQueryResult result = query.store();
        for (size_t row = 0; (row < result.size()); row++) {
                std::string name = result[row][1].c_str();
                int pk       = result[row][0];
                std::ostringstream ss;
                ss << pk;

                std::string sqlResult = ss.str() + "          " +name;
                movies.push_back(sqlResult);
        }
}

void getMovieInfo(const char * path, std::string &buf) {
        buf += "title = " + std::string(path) + "\n";
}
