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
// LanguageModel.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <fstream>
#include <string>
//#include "ChineseLanguageModel.h"
#include "WordMap.h"
#include "KnerserNeyBackOff.h"
#include "AbsoluteDiscounting.h"
using namespace std;

int main(int argc, char *argv[])
{
  //ChineseLanguageModel *cModel=new ChineseLanguageModel();
  WordMap *wordMap = new WordMap();
  KnerserNeyBackOff *knerserNerBackOff = new KnerserNeyBackOff();
  AbsoluteDiscounting *absoluteDiscounting = new AbsoluteDiscounting();
  string corpusPath = "C:\\xiaoxiong\\studyFile\\TestProject\\NLP\\assistantFile\\TM-training-set\\TM-training-set\\english_train_large.txt";
  string splitWordsSavePath = "C:\\xiaoxiong\\studyFile\\TestProject\\NLP\\assistantFile\\TM-training-set\\TM-training-set\\GramFrequent\\";
  string savePath = "C:\\xiaoxiong\\studyFile\\TestProject\\NLP\\assistantFile\\TM-training-set\\TM-training-set\\AbsoluteDiscounting\\";
  string kn_prob_file_path = "C:\\xiaoxiong\\studyFile\\TestProject\\NLP\\assistantFile\\TM-training-set\\TM-training-set\\KnerserNeyBackOff\\";
  vector<string> gramTextNames;

  //ifstream fin;
  //ofstream fout1,fout2;
  //fin.open("C:\\xiaoxiong\\studyFile\\TestProject\\NLP\\assistantFile\\TM-training-set\\TM-training-set\\english.txt");
  //fout1.open("C:\\xiaoxiong\\studyFile\\TestProject\\NLP\\assistantFile\\TM-training-set\\TM-training-set\\english_train.txt");
  //fout2.open("C:\\xiaoxiong\\studyFile\\TestProject\\NLP\\assistantFile\\TM-training-set\\TM-training-set\\english_test.txt");

  //char buffer[2048];
  //int count=0;
  //while(!fin.eof())
  //{
  //	fin.getline(buffer,2048);
  //	count++;
  //	if(count<=80000)
  //		fout1<<buffer<<endl;
  //	else
  //		fout2<<buffer<<endl;
  //}
  //fin.close();
  //fout1.close();
  //fout2.close();

  //wordMap->DealCorpus(corpusPath,splitWordsSavePath);
  //GramListMap gramListTri;
  //GramListMap gramListUni;
  //gramListTri = GetGramFreq(splitWordsSavePath+"trigram_Path.txt");
  //gramListUni = GetGramFreq(splitWordsSavePath+"unigram_Path.txt");
  vector<GramListMap> gramListVec;
  knerserNerBackOff->LoadGramPro();
  knerserNerBackOff->ScoreSentenceWithContext();

  vector<GramProb> gramProbVec;

  GramListMap gramList;
  GramProb gramProList;
  //for(int i =0 ;i<N_GRAM ;i++)
  //{
  //	gramListVec.push_back(gramList);
  //	gramProbVec.push_back(gramProList);
  //	string path = splitWordsSavePath+gramTextNames[i];
  //	GetGramFreq(path,gramListVec[i]);
  //}
  //knerserNerBackOff->CaculateNGramProBow(gramListVec,kn_prob_file_path);
  knerserNerBackOff->LoadGramPro(kn_prob_file_path);
  char c;
  cin >> c;
  return 0;
}