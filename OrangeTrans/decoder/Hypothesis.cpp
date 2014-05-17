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

#include "Hypothesis.h"

namespace OrangeDecoder
{
  Hypothesis::~Hypothesis()
  {
  }

  void Hypothesis::CreateEmptyHypo(size_t srclen)
  {
    m_addedTgtphra = "";
    m_addedOption = NULL;
    m_prob = 1;
    m_coverage.assign(srclen, 0);
    m_covercount = 0;
    m_endofLastSrcphra = "";
    m_startposofLastSrcPhra = -1;
    m_endposofLastSrcPhra = -1;
    m_futurecost = 0;
    m_prevhypo.insert(NULL);
    m_bestprevHypo = NULL;
  }

  void Hypothesis::CreateHypo(size_t srclen)
  {
    m_addedTgtphra = "";
    m_addedOption = NULL;
    m_prob = 0;
    m_coverage.assign(srclen, 0);
    m_covercount = 0;
    m_endofLastSrcphra = "";
    m_futurecost = 0;
  }

  bool Hypothesis::Recombine(Hypothesis* hypo)
  {
    if (m_coverage == hypo->m_coverage
      && m_lastTwo == hypo->m_lastTwo
      && m_endofLastSrcphra == hypo->m_endofLastSrcphra){
      return true;
    }
    else{
      return false;
    }
  }
}