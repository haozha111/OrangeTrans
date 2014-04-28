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
#ifndef PHRASE_EXTRACTION_PIPELINE_H_INCLUDED_
#define PHRASE_EXTRACTION_PIPELINE_H_INCLUDED_

#include <fstream>
#include <string>

namespace OrangeTraining
{
  class PhraseExtractionPipeline
  {
  public:
    PhraseExtractionPipeline(std::string srcTrainset
      , std::string tgtTrainset
      , std::string align
      , std::string ruleOuput
      , std::string ruleInvOutput);

    bool StartPhraseExtraction();
  private:
    std::string m_srcTrainset;
    std::string m_tgtTrainset;
    std::string m_align;
    std::string m_ruleOuput;
    std::string m_ruleInvOutput;
  };
}

#endif