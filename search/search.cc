#include"search.h"

namespace search{
  const char* const DICT_PATH = "../jieba_dict/jieba.dict.utf8";
  const char* const HMM_PATH = "../jieba_dict/hmm_model.utf8";
  const char* const USER_DICT_PATH = "../jieba_dict/user.dict.utf8";
  const char* const IDF_PATH = "../jieba_dict/idf.utf8";
  const char* const STOP_WORD_PATH = "../jieba_dict/stop_words.utf8";

  Index::Index():jieba_(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH){}
  bool Index::Build(const std::string& input_path)
  {
    std::cout << "Index Build Start " << std::endl;
    //1.按行读取文件内容
    std::ifstream file(input_path.c_str());
    if(!file.is_open()){
      std::cout << "input_path open failed input_path= " << input_path << std::endl;
      return false;
    }
    std::string line;
    while(std::getline(file,line)){
    //2.对文件内容解析，构造DocInfo对象，插入forward_index数组，构建正排索引
    const Doc_Info* doc_info = BuildForward(line);
    //3.更新构建倒排索引
    BuildInverted(*doc_info);
    }
    return true;
  }

  const Doc_Info* Index::GetDocInfo(uint64_t doc_id) const//正排索引
  {
    if(doc_id >= forward_index_.size()){
      return NULL;
    }
    return &forward_index_[doc_id];
  }

  const InvertedList* Index::GetInvertedList(const std::string& key) const//倒排索引，得到倒排拉链
  {
    auto pos = inverted_index_.find(key);
    if(pos == inverted_index_.end()){
      return NULL;
    }
    return &pos->second;
  }

  const Doc_Info* Index::BuildForward(const std::string& line){
    
    std::vector<std::string> tokens;//存放切分结果

    StringUtil::Split(line,&tokens,"\3");
    if(tokens.size()!=3){
      std::cout << "tokens not ok" << std::endl;
      return NULL;
    }
    //构造Doc_Info对象
    Doc_Info doc_info;
    doc_info.doc_id = forward_index_.size();
    doc_info.title = tokens[0];
    doc_info.url = tokens[1];
    doc_info.content = tokens[2];

    //将Doc_Info对象插入到forward_index_数组中
    forward_index_.push_back(doc_info);
    return &forward_index_.back();
  }

  //构建倒排拉链
  void Index::BuildInverted(const Doc_Info& doc_info)
  {
    //1.分词
    std::vector<std::string> title_tokens;
    CutWord(doc_info.title,&title_tokens);
    std::vector<std::string> content_tokens;
    CutWord(doc_info.content,&content_tokens);
    //2.词频统计
    struct WordCnt{
      int title_cnt;
      int content_cnt;
    };
    //用一个hash表完成词频统计
    std::unordered_map<std::string,WordCnt> word_cnt;
    for(std::string word:title_tokens){
      boost::to_lower(word);//统计词频应该忽略大小写，全转换成小写
      ++word_cnt[word].title_cnt;
    }
    for(std::string word:content_tokens){
      boost::to_lower(word);
      ++word_cnt[word].content_cnt;
    }
    //3.遍历分词结果，在倒排索引中查找
    for(const auto& word_pair:word_cnt){
      Weight weight;
      weight.doc_id = doc_info.doc_id;
      weight.weight = 10*word_pair.second.title_cnt + word_pair.second.content_cnt;
      
      //4.在倒排索引中插入Weight对象
      InvertedList& inverted_list = inverted_index_[word_pair.first];
      inverted_list.push_back(weight); 
    }
  }//end BuildInverted 

  void Index::CutWord(const std::string&input,std::vector<std::string>* output)
  {
    jieba_.CutForSearch(input,*output);
  }
};//end search
