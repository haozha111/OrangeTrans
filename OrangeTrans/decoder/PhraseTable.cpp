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
#include "PhraseTable.h"
#include "..\util\Utility.h"
#include <algorithm>

using namespace OrangeTrans;

namespace OrangeDecoder
{
  bool PhraseTable::cmp(TranslationOption* a, TranslationOption* b)
  {
    return
      (a->p + a->plex + a->pinv + a->plexinv) >
      (b->p + b->plex + b->pinv + b->plexinv);
  }

  void PhraseTable::Clear()
  {
    m_table.clear();
  }

  void PhraseTable::AddTranslationOption(string srcphra, TranslationOption* option)
  {
    if (m_table.count(srcphra)){
      m_table[srcphra].push_back(option);
      sort(m_table[srcphra].begin(), m_table[srcphra].end(), PhraseTable::cmp);
    }
    else{
      m_table[srcphra] = vector<TranslationOption*>();
      m_table[srcphra].push_back(option);
    }

  }

  TranslationOption* PhraseTable::FindTransOption(string srcphra, string tgtphra)
  {
    if (!m_table.count(srcphra)){
      return NULL;
    }
    else{
      for (TranslationOption* option : m_table[srcphra]){
        if (option->translation == tgtphra){
          return option;
        }
      }
      return NULL;
    }
  }

  double PhraseTable::GetCheapestTransCost(string srcphra)
  {
    if (m_table.count(srcphra)){
      return
        (m_table[srcphra][0]->p + m_table[srcphra][0]->plex
        + m_table[srcphra][0]->pinv + m_table[srcphra][0]->plexinv) / 4.0;
    }
  }

  double PhraseTable::GetCheapestLMCost(string srcphra, KnerserNeyBackOff* lmProvider)
  {
    double maxlm = -1000000;
    if (m_table.count(srcphra)){
      for (TranslationOption* option : m_table[srcphra]){
        double score = lmProvider->ScoreSentenceWithoutContext(Utility::Split(option->translation));
        maxlm = maxlm < score ? score : maxlm;
      }
    }
    return maxlm;
  }
}