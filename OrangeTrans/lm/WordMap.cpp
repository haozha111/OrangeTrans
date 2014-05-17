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
#include "WordMap.h"

namespace OrangeLM
{
  void WordMap::DealCorpus(const string &corpusPath, const string &gramFreqSavePath)
  {
    //string endIcon = ":.,!?;";
    GramListMap gramList;
    for (int i = 0; i < N_GRAM; i++)
    {
      m_GramVec.push_back(gramList);
    }
    ifstream fin;
    fin.open(corpusPath);

    if (!fin)
    {
      cout << " file read error!" << endl;
      return;
    }
    string text, tempstr;
    char buf[2048];
    int count = 0;
    while (getline(fin, text))
    {
      count++;
      if (count % 10000 == 0)
        cout << "Extract line count : " << count << endl;
      GetNNramMapIgnoreIcon(text);  //对每一行进行提取n-gram
    }
    fin.close();

    WriteGramFreqToFile(gramFreqSavePath);
    cout << "finished to write the grams frequency in to file path:" << gramFreqSavePath << endl;

  }
  bool  WordMap::WriteGramFreqToFile(const string &gramFreqSavePath){

    string savePathList[N_GRAM] = { UNIGRAM_FREQ_PATH, BIGRAM_FREQ_PATH, TRIGRAM_FREQ_PATH };
    size_t len_gramList[N_GRAM] = { m_uniGramList.size(), m_biGramList.size(), m_triGramList.size() };
    for (size_t i = 0; i < N_GRAM; i++)
    {
      string savePath = gramFreqSavePath + savePathList[i];
      WriteMapToFile(savePath, m_GramVec[i]);
    }
    return true;

  }
  void WordMap::GetNNramMapIgnoreIcon(string &text)
  {
    int last_pos = 0;
    int pos = 0;
    string sep = " ";
    vector<string> wordsVector;
    string tempstr;
    int i = 0;
    wordsVector.clear();
    wordsVector.push_back("<s>");
    while (true)
    {
      pos = text.find(sep, last_pos);
      if (pos == string::npos)
      {
        break;
      }
      else
      {
        tempstr = text.substr(last_pos, pos - last_pos);//获取空格中间的单词
        if (tempstr.empty())
        {
          last_pos = pos + 1;
          continue;
        }
        if (tempstr.compare(" ") == 0)
        {
          last_pos = pos + 1;
          continue;
        }

        wordsVector.push_back(tempstr);
      }
      last_pos = pos + 1;
    }
    wordsVector.push_back("</s>");
    for (int i = 0; i < N_GRAM; i++)
    {
      ExtractNGram(wordsVector, i + 1, m_GramVec[i]);
    }
  }

  /*
  *生成n_gram统计词频map
  */
  void WordMap::GenerateNGramMap(string &text){
    int last_pos = 0;
    int pos = 0;
    string sep = " \r\n";//以空格 和 回车 作为分隔符
    string tempstr;
    vector<string> wordsVector; //存储读入的单词
    int nGram[3] = { 1, 2, 3 }; //三种gram 分别为 1gram bigram trigram
    wordsVector.push_back("<BOS>");//加入句首标识符
    int count = 1;

    m_GramVec.push_back(m_uniGramList);
    m_GramVec.push_back(m_biGramList);
    m_GramVec.push_back(m_triGramList);
    while (true)
    {
      pos = text.find_first_of(sep, last_pos);
      if (pos == string::npos)
      {
        break;
      }
      else
      {
        tempstr = text.substr(last_pos, pos - last_pos);//获取空格中间的单词
        //tempstr.find_first_of(sep);
        //FilterString(tempstr);

        if (tempstr.empty())
        {
          last_pos = pos + 1;
          continue;
        }
        else if (IsEndOfSentence(tempstr) || tempstr.size() > 300)//找到一句话的截止符
        {
          wordsVector.push_back("<EOS>");//加入句尾标识符
          for (int i = 0; i < m_GramVec.size(); i++)
          {
            ExtractNGram(wordsVector, nGram[i], m_GramVec[i]);
          }
          count++;
          if (count % 1000 == 0)
            cout << "当前读取句数为:" << count << endl;
          wordsVector.clear();
          wordsVector.push_back("<BOS>");//加入句首标识符
        }
        else
        {
          wordsVector.push_back(tempstr);//将单词加入到容器中

        }
        last_pos = pos + 1;//调整到下一个单词位置

      }

    }

    cout << "generate 1.unigram,2.bigram,3.trigram" << endl;
  }
  /*
  **按照从单词向量wordsVec中统计n_gram的词频
  */
  void WordMap::ExtractNGram(
    vector<string> &wordsVec, //获取的单词向量
    int n_gram, //使用n元统计
    GramListMap &gramList//n元统计词频map
    )
  {
    int len_sent;
    len_sent = wordsVec.size() - n_gram;
    string singleGram;
    if (len_sent >= 0) //句子单词量至少大于N_GRAM长度
    {
      /*singleGram+=wordsVec[0];
      singleGram+=" ";
      singleGram+=wordsVec[1];
      gramList[singleGram]++;*/
      int i = 0, j = 0;
      for (i = 0; i <= len_sent; i++)
      {
        singleGram.clear();
        j = 0;
        for (j = i; j < n_gram + i - 1; j++) //将N_GRAM单词组合成一个string，其中以空格分隔
        {
          singleGram += wordsVec[j];
          singleGram += " ";
        }
        singleGram += wordsVec[j];
        if (singleGram.empty() == 0)
          gramList[singleGram]++;
      }
    }

  }
  /*
  *获取第n_gram种类型的gramMap
  */
  GramListMap WordMap::GetNGramMap(int &n_gram)
  {
    //if(n_gram>0&&n_gram<=N_GRAM)
    return m_GramVec[n_gram];
  }
  WordMap::WordMap(void)
  {
  }

  WordMap::~WordMap(void)
  {
  }

}
