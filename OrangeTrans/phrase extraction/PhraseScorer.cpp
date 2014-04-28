/**
*Orange Trans: A statistical machine translation decoder: Phrase-based translation
*and Hiero Phrase-based translation.
*
*$Id:The function of PhraseExtractor is to extract phrases (and assign scores) that are consistent with
the input word alignment.

*$Version:
*0.0.1
*$Created by:
*Haoliang Zhang(A30041839@gmail.com)
*$Last Modified:
**/

#include "PhraseScorer.h";
#include "..\util\BasicMethod.h";
namespace OrangeTraining
{
  //code for LexicalTable
  void LexicalTable::LoadLexicalTable(string &pLexicaltable)
  {
    using namespace OrangeTrans;
    ifstream fin(pLexicaltable);
    if (!fin){
      cerr << "ERROR: Please check file path " << pLexicaltable << endl;
      return;
    }

    //read lexical table
    string line;
    while (!getline(fin, line)){
      vector<string> tmpVec = BasicMethod::Split(line);
      if (tmpVec.size() != 3){
        cerr << "WARNING: lexicon entry: " << line << " is bad data." << endl;
        continue;
      }
      if (m_lexTable.count(tmpVec[0])){
        m_lexTable[tmpVec[0]][tmpVec[1]] = atof(tmpVec[2].c_str());
      }
      else{
        m_lexTable[tmpVec[0]] = map<string, double>();
        m_lexTable[tmpVec[0]][tmpVec[1]] = atof(tmpVec[2].c_str());
      }
    }
    fin.close();
  }

  //code for PhraseTableEntry
  PhraseTableEntry::PhraseTableEntry() :m_phraseTransWgt(0)
    , m_lexicalWgt(1){}
}