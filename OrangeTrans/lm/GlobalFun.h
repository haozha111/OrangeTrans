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

#ifndef GLOBAL_TRANS_PIPELINE_H_INCLUDED_
#define GLOBAL_TRANS_PIPELINE_H_INCLUDED_
#include<map>
#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include <process.h> 
#include <direct.h>
#include <cmath>
#define N_GRAM 3 //定义语言模型采用trigram
#define ENDICON_NUM 13
#define UNIGRAM_FREQ_PATH  "unigram.txt"
#define MIN_PRO  -11111 //定义最小的概率 给未登录的词
#define BIGRAM_FREQ_PATH  "bigram.txt"
#define TRIGRAM_FREQ_PATH "trigram.txt"
#define MAX_READ_LINE_LEN  1024 //最大每次读取行数  

using namespace std;

namespace OrangeLM
{
  typedef struct{
    double pKn; //平滑概率
    double bow; //后退权重
  }GramProbBack;
  typedef map<string, int> GramListMap;
  typedef map<string, double> GramProb;
  typedef map<string, GramProbBack> GramProbBow; //带有后退权重的gram概率map 
  /*
  *判断对于输入的words是否为句子的结束标志
  */
  bool IsEndOfSentence(string words);
  /*
  * 计算num1的index次方
  */
  int PowInt(int num1, int index);
  /*
  *字符串过滤
  */
  void FilterString(string &str);
  /*
  *获取指定路径下的gram词频
  */
  void GetGramFreq(const string &gramSavePath, GramListMap &gramList);
  /*
  *获取指定路径下的gram概率
  */

  void GetGramPro(const string &gramSavePath, GramProbBow &gramList);
  /*
  *从指定的gramSrcMap，其当前为srcLevelGram降低到desLevelGram，并存入gramDesMap中
  */
  void  ReduceGramLevel(GramListMap &gramSrcMap, int srcLevel, GramListMap &gramDesMap, int desLevel);

  //将指定gramList写入指定文件路径gramFreqSavePath中
  void  WriteMapToFile(const string &gramFreqSavePath, GramListMap &gramList);

  //将指定gramList写入指定文件路径gramFreqSavePath中
  void  WriteProbToFile(const string &gramFreqSavePath, GramProb &gramList);

  //将指定的gramListVector写入文件 ，主要用于作为srilm的输入模型文件
  void WriteProbVectorToFile(const string &gramFreqSavePath, vector<GramProb> &gramList);

  //将指定的含有回退权重的n-gram概率map写入文件 ，主要用于作为srilm的输入模型文件
  void WriteProbBowToFile(const string &gramFreqSavePath, vector<GramProbBow> &gramList);

  //将指定gramList概率写入指定文件路径gramFreqSavePath目录底下的三个文件中
  void WriteProbSplitBowToFile(const string &gramFreqSavePath, vector<GramProbBow> &gramList);

  //对于输入的句子，将其分隔成为vector
  void  SentenceToVector(string sentence, vector<string> &wordVector, int flag);

  //对于输入的字符串，将其分隔为单词vector

  void StringToVector(string str, vector<string>&wordVector);
}

#endif