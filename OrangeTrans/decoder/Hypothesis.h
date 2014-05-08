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

using namespace std;

namespace OrangeDecoder
{
  class Hypothesis
  {
  public:

  private:
    size_t m_coverCount; //! The number of foreign words that have been translated
    vector<bool> m_coverage; //! coverage vector of the foreign sentence
    vector<string> m_partialtrans; //! partial translation
  };
}

#endif