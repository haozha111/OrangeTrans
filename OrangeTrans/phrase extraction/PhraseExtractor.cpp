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
#include "PhraseExtractor.h";

using namespace std;

namespace OrangeTraining
{
  PhraseExtractor::PhraseExtractor(WordAlignment &wordAlignment, PhraseCollection &phraseCollection)
    : m_wordAlignment(wordAlignment), m_phraseCollection(phraseCollection) {}


  //! implementation of phrase pair extraction algorithm
  bool PhraseExtractor::ExtractPhrasePair(RuleOptions &options) const
  {
    //traverse all possible spans of target phrase
    size_t tgtlen = m_wordAlignment.TargetLength();
    size_t srclen = m_wordAlignment.SourceLength();
    for (size_t tgtstart = 0; tgtstart < tgtlen; ++tgtstart){
      for (size_t tgtend = tgtstart; tgtend < tgtlen; ++tgtend){
        //check if exceeds maxlen 
        if ((tgtend - tgtstart + 1) > options.tgtMaxLen){
          break;
        }

        //check both sides of target length to see whether it breaks the rule of tgtMaxNullExp
        if (tgtstart + options.tgtMaxNullExp <= tgtend){
          bool nullFlag = true;
          for (size_t k = tgtstart; k <= tgtstart + options.tgtMaxNullExp; ++k){
            if (m_wordAlignment.GetTargetAlignCount(k) > 0){
              nullFlag = false;
              break;
            }
          }
          if (nullFlag){
            continue;
          }
        }

        if (tgtend - options.tgtMaxNullExp >= tgtstart){
          bool nullFlag = true;
          for (size_t k = tgtend; k >= tgtend - options.tgtMaxNullExp; --k){
            if (m_wordAlignment.GetTargetAlignCount(k) > 0){
              nullFlag = false;
              break;
            }
          }
          if (nullFlag){
            continue;
          }
        }
        //find the minimally matching foreign phrase
        size_t srcStartPos = srclen;
        size_t srcEndPos = 0;
        for (size_t k = tgtstart; k <= tgtend; ++k){
          for (auto& align : m_wordAlignment.GetTargetAlign(k)){
            srcStartPos = (align < srcStartPos) ? align : srcStartPos;
            srcEndPos = (align > srcStartPos) ? align : srcStartPos;
          }
        }

        ValidateRuleConsistency(PhrasePair(srcStartPos, srcEndPos, tgtstart, tgtend, m_wordAlignment),
          m_wordAlignment, options);
      }
    }

    //add X ||| NULL or NULL ||| X
    if (options.includeNullAlign){
      for each (size_t pos in m_wordAlignment.GetSourceNullAligned()){
        AddRule(PhrasePair(m_wordAlignment.GetSourcePhrase(pos, pos), "NULL"), true);
      }

      for each(size_t pos in m_wordAlignment.GetTargetNullAligned()){
        AddRule(PhrasePair("NULL", m_wordAlignment.GetTargetPhrase(pos, pos)), true);
      }
    }
    return true;
  }

  //! check whether the extracted phrase pair is consistent with the word alignment
  void PhraseExtractor::ValidateRuleConsistency(PhrasePair &phrasePair, WordAlignment &wordAlignment,
    RuleOptions &options) const
  {
    //check if at least one alignment point
    if (phrasePair.SourceStart() > phrasePair.SourceEnd()){
      return;
    }
    //first check if the source phrase length exceed maximum length
    if ((phrasePair.SourceEnd() - phrasePair.SourceStart() + 1) > options.srcMaxLen){
      return;
    }
    //check if the rule breaks consistency
    for (size_t k = phrasePair.SourceStart(); k <= phrasePair.SourceEnd(); ++k){
      for (auto& align : wordAlignment.GetSourceAlign(k)){
        if (align < phrasePair.TargetStart() || align > phrasePair.TargetEnd()){
          return; //break the consistency rule
        }
      }
    }

    //expand the phrase pairs to include additional unaligned source
    for (int sourceStart = phrasePair.SourceStart();
      sourceStart >= 0
      && (sourceStart == phrasePair.SourceStart() || wordAlignment.GetSourceAlignCount(sourceStart) == 0)
      && ((phrasePair.SourceStart() - sourceStart) <= options.srcMaxNullExp); --sourceStart){
      for (size_t sourceEnd = phrasePair.SourceEnd();
        sourceEnd < wordAlignment.SourceLength()
        && (sourceEnd == phrasePair.SourceEnd() || wordAlignment.GetSourceAlignCount(sourceEnd) == 0)
        && (sourceEnd - sourceStart + 1) <= options.srcMaxLen
        && (sourceEnd - phrasePair.SourceEnd()) <= options.srcMaxNullExp; ++sourceEnd){
        //build a new phrase instance
        PhrasePair newPhrasePair = PhrasePair(sourceStart, sourceEnd, phrasePair.TargetStart(), phrasePair.TargetEnd(), wordAlignment);
        AddRule(newPhrasePair, false);
      }
    }
  }


  //!Add a new phrase pair to the collection
  void PhraseExtractor::AddRule(PhrasePair &phrasePair, bool hasEmptyRule) const
  {
    m_phraseCollection.AddPhrasePair(phrasePair, hasEmptyRule);
  }


  //code for RuleOptions 

  RuleOptions::RuleOptions()
    :srcMaxLen(3)
    , tgtMaxLen(5)
    , srcMaxNullExp(3)
    , tgtMaxNullExp(5)
    , includeNullAlign(true)
  {}
}
