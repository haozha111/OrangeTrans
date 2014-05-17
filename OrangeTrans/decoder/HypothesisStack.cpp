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

#include "HypothesisStack.h"
#include <algorithm>

namespace OrangeDecoder
{
  DecodeOption::DecodeOption()
  {
    m_pruneMethod = PruningMethod::Histogram;
    m_beamSize = 30;
  }

  DecodeOption::DecodeOption(PruningMethod pruneMethod)
  {
    m_pruneMethod = pruneMethod;
    if (pruneMethod == PruningMethod::Histogram){
      m_beamSize = 30;
    }
    else if (pruneMethod == PruningMethod::Threshold){
      m_beamSize = 0.001;
    }
  }

  HypothesisStack::HypothesisStack(DecodeOption &decodeOption)
  {
    m_pruneMethod = decodeOption.m_pruneMethod;
    m_beamSize = decodeOption.m_beamSize;
  }

  HypothesisStack::~HypothesisStack()
  {
    for (Hypothesis* hypo : m_hypoStack){
      delete hypo;
    }
    m_hypoStack.clear();
  }

  void HypothesisStack::Add(Hypothesis* hypo)
  {
    //we first try to recombine hypos
    bool combined = false;
    for (int i = 0; i < m_hypoStack.size(); ++i){
      if (m_hypoStack[i]->Recombine(hypo)){
        //if we can recombine the hypothesis, we preserve the hypo
        //with lower cost
        combined = true;

        if (m_hypoStack[i]->m_prob > hypo->m_prob){
          for (Hypothesis* backptr : hypo->m_prevhypo){
            m_hypoStack[i]->m_prevhypo.insert(backptr);
            //preserve the best previous hypo back pointer
            if (backptr->m_prob > m_hypoStack[i]->m_bestprevHypo->m_prob){
              m_hypoStack[i]->m_bestprevHypo = backptr;
            }
          }
          delete hypo;
        }
        else{
          for (Hypothesis* backptr : m_hypoStack[i]->m_prevhypo){
            hypo->m_prevhypo.insert(backptr);
            if (backptr->m_prob > hypo->m_bestprevHypo->m_prob){
              hypo->m_bestprevHypo = backptr;
            }
          }
          delete m_hypoStack[i];
          m_hypoStack[i] = hypo;
        }
        break;
      }
    }

    if (!combined){
      m_hypoStack.push_back(hypo);
    }

    if (m_pruneMethod == PruningMethod::Threshold){
      //we prune hypothesis stack with beam threshold
      sort(m_hypoStack.begin(), m_hypoStack.end(), HypothesisStack::cmp);
      size_t k = m_hypoStack.size();
      while (--k){
        if (m_hypoStack[k]->m_prob < m_beamSize * m_hypoStack[0]->m_prob){
          delete m_hypoStack[k];
          m_hypoStack.pop_back();
        }
        else{
          break;
        }
      }
    }
    else if (m_pruneMethod == PruningMethod::Histogram){
      //we prune hypothesis stack using histogram pruning
      if (m_hypoStack.size() > m_beamSize){
        sort(m_hypoStack.begin(), m_hypoStack.end(), HypothesisStack::cmp);
        size_t k = m_hypoStack.size();
        while (--k && k >= m_beamSize){
          delete m_hypoStack[k];
          m_hypoStack.pop_back();
        }
      }
    }
  }

  bool HypothesisStack::cmp(Hypothesis* a, Hypothesis* b)
  {
    return a->m_prob > b->m_prob;
  }
}