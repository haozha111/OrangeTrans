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

#pragma once
#include "GlobalFun.h"
#define  PROB_FILENAME "trigram_pro1.txt"
#define RES_PROB_FILENAME "rest_pro.txt"

namespace OrangeLM
{
  class AbsoluteDiscounting
  {
  private:
    GramProb m_GramPro; //!存入n_gram的概率
    double m_restPro; //!剩余的概率

  public:
    /*
    *采用绝对减值法计算gram的概率
    *gramMapList为输入的n-gram的统计词频
    *gramProbList为输出的n-gram统计概率
    *allocatPro为分配给当前gram的总概率
    *restPro为分配给当前gram后剩余的概率
    */
    void UseAbsoluteDiscounting(GramListMap &gramMapList, int vocabularySize, string savePath);

    //获取n-gram概率的列表

    vector<GramProb> GetGramProVec();

    //对输入句子sentence进行打分
    double ScoreSentence(string sentence);

    //对输入的gram进行打分
    double ScoreGram(vector<string> gramVec);

    /*
    *加载指定路径gramProSavePath下的概率map 到gramList中
    */
    void LoadGramPro(const string gramProSavePath);

    AbsoluteDiscounting(void);
    ~AbsoluteDiscounting(void);
  };
}


