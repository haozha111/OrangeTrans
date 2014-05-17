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
#ifndef HYPOTHESIS_H_INCLUDED_
#define HYPOTHESIS_H_INCLUDED_

#include <vector>
#include <set>
#include <stdio.h>
#include "trie.h"

using namespace std;

namespace OrangeDecoder
{
  class Hypothesis
  {
  public:
    //method
    ~Hypothesis();
    void CreateEmptyHypo(size_t srclen);//! create an empty hypo
    void CreateHypo(size_t srclen);//! create a hypo
    bool Recombine(Hypothesis* hypo);//! recombine hypo if possible
    //members
    set<Hypothesis*> m_prevhypo;//! back pointer to the previous expanded hypo(s)
    Hypothesis* m_bestprevHypo;//! back pointer to the best previous expanded hypo
    vector<unsigned int> m_coverage; //! coverage vector of the foreign sentence
    size_t m_covercount; //! the count of foreign words already covered
    pair<string, string> m_lastTwo; //! the last two target words generated
    string m_endofLastSrcphra; //! the end of the last foreign phrase covered
    int m_startposofLastSrcPhra;//! the start position of the last translated foreign phrase
    int m_endposofLastSrcPhra;//! the end position of the last translated foreign phrase
    string m_addedTgtphra;//! the last added target phrase
    TranslationOption* m_addedOption;//! the last added translation option
    double m_prob;//! probability so far
    double m_futurecost; //! an estimate of the future cost
  };
}

#endif