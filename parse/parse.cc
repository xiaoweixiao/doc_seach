//数据处理模块，把html进行处理，去标签
//把所有html文件合并成一个文件，这个新文件的一行是一个原来的html
//解析html文件，解析出标题、正文、url
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include"../common/util.hpp"
#include<boost/filesystem/path.hpp>
#include<boost/filesystem/operations.hpp>

const std::string g_input_path = "../data/input";
const std::string g_output_path = "../data/tmp/raw_input";

//html文档目录
struct DocInfo{
  std::string title;
  std::string content;
  std::string url;
};

//input_path是html文档，
bool EnumFile(const std::string& input_path,std::vector<std::string>* file_list)
{
  namespace fs = boost::filesystem;
  fs::path root_path(input_path);//用input_path字符串构建path对象
  if(!fs::exists(root_path)){
    std::cout << "input_path not exist,input_path=" << input_path << std::endl;
    return false;
  }
  //boost遍历目录，借助迭代器
  fs::recursive_directory_iterator end_iter;//未初始化的迭代器作为遍历结束标记
  for(fs::recursive_directory_iterator iter(root_path);iter != end_iter;++iter){
    //1.剔除目录
    if(!fs::is_regular_file(*iter)){
      continue;
    } 
    //2.剔除其它类型文件，只保留html
    if(iter->path().extension()!=".html"){
      continue;
    }
    file_list->push_back(iter->path().string());
  }
  return true;
}

bool ParseTitle(const std::string& html,std::string* title)
{
  //<title>标题</title>
  size_t begin = html.find("<title>");
  if(begin == std::string::npos){
    std::cout<<"<title> not fouund " << std::endl;
    return false;
  }
  size_t end = html.find("</title>");
  if(end == std::string::npos){
    std::cout << "</title> not found" << std::endl;
    return false;
  }

  begin += std::string("<title>").size();
  if(begin >= end){
    std::cout << "begin end error" << std::endl;
    return false;
  }
  *title = html.substr(begin,end-begin);
  return true;
}

bool ParseFile(const std::string& file_path,DocInfo*doc_info)
{
  //1.打开文件，读取文件内容
  std::string html;
  bool ret = FileUtil::Read(file_path,&html);
  if(!ret){
    std::cout << "Read file failed,file_path=" << file_path << std::endl; 
    return false;
  }
  //2.解析标题
  ret = ParseTitle(html,&doc_info->title);
  if(!ret){
    std::cout << "ParseTitle failed,file_path=" << file_path << std::endl;
    return false;
  }
  //3.解析正文，并去除标签
  ret = ParseContent(html,&doc_info->content);
  if(!ret){
    std::cout << "ParseContent failed,file_path=" << file_path << std::endl;
    return false;
  }
  //4.解析url
  ret = ParseUrl(html,&doc_info->url);
  if(!ret){
    std::cout << "ParseUrl failed,file_path=" << file_path << std::endl;
    return false;
  }
}

int main()
{
  //1、枚举出所有input目录的文档路径
  std::vector<std::string> file_list;
  bool ret = EnumFile(g_input_path,&file_list);
  if(!ret){
    std::cout << "EnumFile failed" << std::endl;
    return 1;
  }
  for(const auto& file_path:file_list)
    std::cout << file_path << std::endl;

  std::ofstream output_file(g_output_path.c_str());
  if(!output_file.is_open()){
    std::cout << "open output_file failed! g_output_path=" << g_output_path <<std::endl;
    return 1;
  }
  //2、依次解析每个枚举文件，去标签
  for(const std::string& file_path : file_list){
    //将string类型的html解析到DocInfo结构体
    DocInfo info;
    ret = ParseFile(file_path,&info);
    if(!ret){
      std::cout << "ParseFile failed! file_path=" << file_path << std::endl;
      continue;
    }

    //3、把分析结果合并到raw_input文件中
    WriteOutput(info,output_file);
  }
  output_file.close();
  return 0;
}
