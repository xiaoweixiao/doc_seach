//构建索引模块和搜索模块
#include"../common/util.hpp"

namespace search{

  struct Doc_Info{
    uint64_t doc_id;
    std::string title;
    std::string content;
    std::string url;
  };

  struct Weight{
    uint64_t doc_id;
    int weight;//权重
  };


  typedef std::vector<Weight> InvertedList;

  class Index{
private:
  std::vector<Doc_Info> forward_index_;//正排索引
  std::unordered_map<std::string,InvertedList> inverted_index_;//倒排索引
  cppjieba::Jieba jieba_;
public:
  Index();
  //读取raw_input文件，构建索引
  bool Build(const std::string& input_path);
  const Doc_Info* GetDocInfo(uint64_t doc_id) const;//正排索引
  const InvertedList* GetInvertedList(const std::string& key) const;//倒排索引，得到倒排拉链

  void CutWord(const std::string&input,std::vector<std::string>* output);
private:
  const Doc_Info* BuildForward(const std::string& line);
  void BuildInverted(const Doc_Info& doc_info);
  };

  //搜索模块
  class Search{
  };
}//end search
