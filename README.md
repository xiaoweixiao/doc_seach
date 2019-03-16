# 数据处理模块
将boost库的html文件合并成一个文本文档中，每一个html文件占用新文本一行，每一行有不可见字符将文件的content、url、title分割开。
# 索引模块
使用vector&lt;Doc_Info&gt;构建正排，使用unordered_map&lt;string,vector&lt;Weight&gt;&gt;构建倒排。
# 搜索模块
根据搜索词分词结果查倒排索引，再通过doc_id查正排索引得到与搜索词相关文档。
