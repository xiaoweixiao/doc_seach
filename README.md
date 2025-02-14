# 数据预处理模块
将boost库的html文件合并成一个文本文档中，每一个html文件占用新文本一行，每一行有不可见字符将文件的content、url、title分割开。

1.下载boost库html文件，存储在input文件夹下，使用input路径构建boost库filesystem的path对象。将path对象下不是html的文件剔除掉，剩余五千多个html文档，将这个五千多个文档路径push进vector<string>。

2.将html文件去标签，存储到Doc_Info结构体中，Doc_Info包含title、url、content成员。
  
3.将html文档整合到一个文本文档中，一个html占一行，title、url、content以不可见字符分割开。
# 索引模块
使用vector&lt;Doc_Info&gt;构建正排，使用unordered_map&lt;string,vector&lt;Weight&gt;&gt;构建倒排。

1. 将预处理过的文本文档按行读取，分词，构建正排，正排结构vector&lt;Doc_Info&gt;这里添加vector下标作为Doc_Info的新成员doc_id。

2. 使用结巴分词CutForSearch对Doc_Info的title和content进行分词，使用unordered_map&lt;string,vector&lt;WordCnt&gt;&gt;进行词频统计。

3.根据词频统计结果计算文档权重，push进倒排拉链中。
# 搜索模块
根据搜索词分词结果查倒排索引，再通过doc_id查正排索引得到与搜索词相关文档。

1. 搜索词进行分词，根据分词结果查倒排搜索引拉链。将所有分词倒排拉链结果push到vector&lt;Weight&gt;中，去除重复项，合并权重。
2.根据权重排序，根据Weight结构体的doc_id从正排中得到Doc_Info。Json::FastWriter实现快速序列化。

查询filesystem的结果页面
<img src="https://github.com/xiaoweixiao/doc_search/blob/master/common/%E5%86%92%E7%83%9F%E6%B5%8B%E8%AF%95.png">
