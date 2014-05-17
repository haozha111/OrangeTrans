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

#include "AbsoluteDiscounting.h"

namespace OrangeLM
{
  AbsoluteDiscounting::AbsoluteDiscounting(void)
  {
  }


  AbsoluteDiscounting::~AbsoluteDiscounting(void)
  {
  }
  /*
  *采用绝对减值法计算低阶gram的概率
  *gramMapList为输入的n-gram的统计词频
  *gramProbList为输出的n-gram统计概率
  *allocatPro为分配给当前gram的总概率
  *restPro为分配给当前gram后剩余的概率
  */
  void AbsoluteDiscounting::UseAbsoluteDiscounting(GramListMap &gramMapList, int vocabularySize, string savePath)
  {
    string fileNameTri = savePath + PROB_FILENAME;
    string fileNameRest = savePath + RES_PROB_FILENAME;
    GramProb gramProbList;
    int totalGramNum = 0; //整个语料Gram个数
    double discountNum; //折扣数
    double restProTri = 0.0; //Gram重新计算概率后的剩余概率
    int n1 = 0, n2 = 0;  //统计在Gram中出现次数为1次和两次的事件个数
    int gramListNum = gramMapList.size();
    GramListMap::iterator itr_gram = gramMapList.begin();
    GramListMap::iterator itr_gram_end = gramMapList.end();
    for (; itr_gram != itr_gram_end; itr_gram++)
    {
      totalGramNum += itr_gram->second;
      if (itr_gram->second == 1)
      {
        n1++;
      }
      else if (itr_gram->second == 2)
      {
        n2++;
      }
      else
      {
        continue;
      }

    }
    discountNum = (double)n1 / (n1 + 2 * n2);  //使用上限代替优化的dicountNum

    for (itr_gram = gramMapList.begin(); itr_gram != itr_gram_end; itr_gram++) //重新计算减值后Gram的每个事件的概率
    {

      m_GramPro[itr_gram->first] = (((double)itr_gram->second - discountNum) / totalGramNum);
    }
    m_restPro = (1.0 - (double)(discountNum*gramListNum) / totalGramNum); //计算Gram剩余概率
    m_restPro = m_restPro / PowInt(vocabularySize, N_GRAM);
    cout << "计算完成m_GramPro!" << endl;

    WriteProbToFile(fileNameTri, m_GramPro);

    ofstream fout;
    fout.open(fileNameRest);
    fout << m_restPro << endl;
    fout.close();
    cout << "成功将m_gram存入文件！" << endl;
    //return gramProbList;
  }
  /*
  *加载指定路径gramProSavePath下的概率map 到gramList中
  */
  void AbsoluteDiscounting::LoadGramPro(const string gramProSavePath)
  {
    string prob_file = gramProSavePath + PROB_FILENAME;
    string rest_prob_file = gramProSavePath + RES_PROB_FILENAME;
    //GetGramPro(prob_file,m_GramPro);

    ifstream fin;
    fin.open(rest_prob_file);
    if (!fin)
    {
      cout << "Failed to open rest_prob file : " << rest_prob_file << endl;
      return;
    }
    char buf[1024];
    fin.getline(buf, 1024);
    //string rest_str = buf;
    m_restPro = atof(buf);
    fin.close();
    cout << "成功加载gramPro文件!" << endl;
  }
  /*
  * 对于句子sentence进行打分
  */
  double AbsoluteDiscounting::ScoreSentence(string sentence)
  {
    vector<string> wordVec;
    vector<string> gramVec;
    SentenceToVector(sentence, wordVec, 0);
    double score = 0.0;
    int wordVecLen = wordVec.size();
    int len_sent;
    len_sent = wordVec.size() - N_GRAM;
    string singleGram;
    if (len_sent >= 0) //句子单词量至少大于N_GRAM长度
    {
      gramVec.push_back(wordVec[0]);
      gramVec.push_back(wordVec[1]);
      score = ScoreGram(gramVec);
      int i = 0, j = 0;
      for (i = 1; i <= len_sent; i++)
      {
        gramVec.clear();
        for (j = i; j < N_GRAM + i - 1; j++) //将N_GRAM单词组合成一个string，其中以空格分隔
        {
          gramVec.push_back(wordVec[j]);

        }
        gramVec.push_back(wordVec[j]);
        score *= ScoreGram(gramVec);
      }
    }
    return score;
  }
  double AbsoluteDiscounting::ScoreGram(vector<string> gramVec)//对输入的gram进行打分
  {
    int gramSize = gramVec.size();
    string tempstr;
    int i = 0;
    for (i = 0; i<gramSize - 1; i++)
    {
      tempstr += gramVec[i];
      tempstr += " ";
    }
    tempstr += gramVec[i];
    if (m_GramPro.count(tempstr)>0)
      return m_GramPro[tempstr];
    else
      return m_restPro;
  }
}