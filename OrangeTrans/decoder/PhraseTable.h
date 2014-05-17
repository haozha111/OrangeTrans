/**
*Orange Trans: A statistical machine translation decoder: Phrase-based translation
*and Hiero Phrase-based translation.
*
*$Id:
*$Version:
*0.0.1
*$Created by:
*Haoliang Zhang(A30041839@gmail.com)
*$Last Modified:
**/

#pragma once
#ifndef PHRASE_TABLE_H_INCLUDED_
#define PHRASE_TABLE_H_INCLUDED_

#include <map>
#include <string>
#include <vector>
#include "trie.h"
#include "..\lm\KnerserNeyBackOff.h"

using namespace std;
using namespace OrangeLM;

namespace OrangeDecoder
{
  class PhraseTable
  {
  public:
    map<string, vector<TranslationOption*> > m_table;
    void Clear();

    static bool cmp(TranslationOption* a, TranslationOption* b);
    void AddTranslationOption(string srcphra, TranslationOption* option);
    TranslationOption* FindTransOption(string srcphra, string tgtphra);
    double GetCheapestTransCost(string srcphra);
    double GetCheapestLMCost(string srcphra, KnerserNeyBackOff* lmProvider);
  };
}

#endif