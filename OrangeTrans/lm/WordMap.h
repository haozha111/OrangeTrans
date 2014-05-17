/**
*Orange Trans: A statistical machine translation decoder: Phrase-based translation
*and Hiero Phrase-based translation.
*
*$Id:
*$Version:
*0.0.1
*$Created by:
*Xiong Lv
*$Last Modified:
**/

/*
*WordMap类主要用于生成n-gram的词项和其词频
*
*/

#pragma once
#include "GlobalFun.h"
#include <fstream>

namespace OrangeLM
{
  class WordMap
  {
  private:
    GramListMap m_uniGramList; //!uniGram 统计词频
    GramListMap m_biGramList;  //!biGram 统计词频
    GramListMap m_triGramList; //!triGram 统计词频
    vector<GramListMap> m_GramVec;//!三个gram的统计词频


    void WordMap::GenerateNGramMap(string &text);
    void WordMap::ExtractNGram(
      vector<string> &wordsVec, //获取的单词向量
      int n_gram, //使用n元统计
      GramListMap &gramList//n元统计词频map
      );
    bool  WriteGramFreqToFile(const string &gramFreqSavePath); //将统计的3个gram词频写入文件

  public:
    //将标点符号也考虑的gram 生成
    void GetNNramMapIgnoreIcon(string &text);
    //从指定文档集中抽取gram词频

    void DealCorpus(const string &corpusPath, const string &gramFreqSavePath);
    GramListMap GetNGramMap(int &n_gram); //参数为返回第n_gram的gramMap
    WordMap(void);
    ~WordMap(void);
  };
}



