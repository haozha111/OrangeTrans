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
/*
*Knerser-Ney数据平滑方法，采用的是其后备平滑方式
*
*/
//定义maptree 用于高效查找n-1-gram_* 的问题
//统计map 键值string开始的相应位置的词频统计信息

namespace OrangeLM
{
  typedef struct{
    int count;
    int freq_N[3];
  }Stat;
  typedef struct{
    map<string, Stat> bigram;
    Stat stat;
  }UnigramMap;

  typedef map<string, UnigramMap> UnigramMapTree;


  class KnerserNeyBackOff
  {
  private:
    vector<GramListMap> m_GramMapVec; //! the probability of n-gram

    vector<GramProbBow> m_GramProBack;//! the probability of n-gram with backoff weight;

  public:

    //计算加入后退概率的n_gram计算
    //caculate the n_gram with backoff-weight probability
    void CaculateNGramProBow(vector<GramListMap> &gramMapVec, string &proSavePath);
    //计算n_gram概率
    //caculate the probability of n_gram
    void CaculateGramProVec(vector<GramListMap> &gramMapVec, string & proSavePath);

    //采用绝对减值法 对于 计算低阶gram的概率
    //
    void AbsoluteDiscounting(GramProb &gramProbList, GramListMap &gramMapList, double allocatePro, double &restPro);

    //加载gram概率表
    //load the n_gram probability Map
    void LoadGramPro(const string gramProSavePath);

    //释放所有资源
    //release all source
    void ReleaseSource();

    //获取n-gram概率的列表
    //get the n-gram probability
    vector<GramProbBow> GetGramProVec();

    //对输入带有上下文信息的句子sentence进行打分
    //score the sentence with context
    double ScoreSentenceWithContext(vector<string> sentence);


    //对于无上下文信息的sentence打分
    //score the sentence without context
    double ScoreSentenceWithoutContext(vector<string> sentence);

    //对输入的gram进行打分
    //socre the n-gram
    double ScoreGram(vector<string> gramVec);

    //将src中第pos个空格后的字符替换为iterm
    void  ReplaceMapString(string src, string &des, int pos);

    //计算指定gram层次的概率和其低阶的bow
    //caculate the probability of n_gram  and the backoff weights of n-1 gram
    void CaculateBowAndPkn(GramListMap &gramList,
      GramListMap &vocabulary,
      GramProbBow &gramProb,
      GramProbBow &lowGramBow,
      UnigramMapTree &uniGramMapTree,
      int order);



    //对于输入的gram频率 计算其discount
    //caculate the discount of the input gram_frequency

    void  CaculateDiscount(GramListMap &gramList, double *discount);

    KnerserNeyBackOff(void);
    ~KnerserNeyBackOff(void);
  };


}