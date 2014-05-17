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
#ifndef HYPOTHESIS_STACK_H_INCLUDED_
#define HYPOTHESIS_STACK_H_INCLUDED_

#include <vector>
#include "Hypothesis.h"

using namespace std;

namespace OrangeDecoder
{
  enum class PruningMethod { Histogram, Threshold };

  class DecodeOption
  {
  public:
    DecodeOption();
    DecodeOption(PruningMethod pruneMethod);
    PruningMethod m_pruneMethod;
    double m_beamSize;
  };

  class HypothesisStack
  {
  public:
    HypothesisStack(DecodeOption &decodeOption);
    ~HypothesisStack();
    //! add a new hypo to this stack.
    void Add(Hypothesis* hypo);
    vector<Hypothesis*> m_hypoStack;//! the storing stack for hypothesis
  private:
    bool static cmp(Hypothesis* a, Hypothesis* b);
    PruningMethod m_pruneMethod;
    double m_beamSize;
  };
}
#endif