#include "httplib.h"
#include"../search/search.h"
#include<iostream>

int main()
{
  //1.创建search对象
  search::Search s;
  bool ret = s.Init("../data/tmp/raw_input");
  if(!ret){
    std::cout << "Search Init failed" << std::endl;
    return 1;
  }
  //2.创建http服务器
  using namespace httplib;
  Server server;
  //search?query=filesystem
  server.Get("/search",[&s](const Request& req,Response& res){
      std::string query = req.get_param_value("query");
      std::string result;
      s.Searchs(query,&result);
      res.set_content(result,"text/plain");
      });
  server.listen("0.0.0.0",8080);
  return 0;
}
