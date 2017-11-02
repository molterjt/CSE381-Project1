#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "moviesdb.h"

#include <cstring>

std::vector<std::string> movies;

mysqlpp::Query connect() {
    // Connect to database with: database, server, userID, password
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    // Create a query 
    mysqlpp::Query query = myDB.query();
    std::cout << "Connected to DB\n" << std::endl;
    return myDB.query();
}

/*
mysqlpp::Query movieList(){
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1");
    mysqlpp::Query query = myDB.query();
    query << "SELECT movies.title from movies";
    query.parse();
    mysqlpp::StoreQueryResult results = query.store();
    return myDB.query();
}
*/

void getMovieListing(std::vector<std::string> &movies) {
    mysqlpp::Connection myDB("cse381", "localhost", "cse381", "m1am1"); 
    mysqlpp::Query query = myDB.query();
    query << "SELECT movies.title FROM movies";
    query.parse();
    mysqlpp::StoreQueryResult results = query.store();
    for(size_t row = 0; (row < results.size()); row++) {
        std::string title = results[row][0].c_str();
        movies.push_back(title);
        std::cout << title << "\n" << std::endl;
    }
    
}

void getMovieInfo(const char * path, std::string &buf) {
	buf += "title = " + std::string(path) + "\n";
}


/*
int main(int argc, char *argv[]) {
   if( connect() ){
      std::cout << "Connected" << "\n";   }else {
      std::cout << "Error connecting \n";
    }
    (void) argc;
    (void) argv;
   // getMovieListing(movies);
    //movieList();
    return 0;
}
*/




