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
#ifndef WORD_ALIGNMENT_H_INCLUDED_
#define WORD_ALIGNMENT_H_INCLUDED_

#include "..\util\Utility.h"
#include <string>
#include <vector>

namespace OrangeTraining
{
  /**The WordAlignment is used for storing source language sentence,
  * target language sentence, and the alignment between them.
  **/
  using namespace std;

  class WordAlignment
  {
  public:
    void CreateAlignment(
      const string &srcSentence
      , const string &tgtSentence
      , const string &alignment
      , unsigned int sentenceID);

    //methods

    const vector<size_t> & GetSourceAlign(size_t pos) const;
    const vector<size_t> & GetTargetAlign(size_t pos) const;
    size_t GetSourceAlignCount(size_t pos) const;
    size_t GetTargetAlignCount(size_t pos) const;
    string GetSourcePhrase(size_t start, size_t end) const;
    string GetTargetPhrase(size_t start, size_t end) const;
    vector<size_t> GetSourceNullAligned() const;
    vector<size_t> GetTargetNullAligned() const;

    //attributes
    const vector<string> & SourceSentence() const;
    const vector<string> & TargetSentence() const;
    size_t SourceLength() const;
    size_t TargetLength() const;

  private:
    vector<string> m_srcSentence; //! stores each word of the source language sentence into a vector
    vector<string> m_tgtSentence; //! stores each word of the target language sentence into a vector
    vector<vector<size_t> > m_srcAlign; //! stores alignment point for each source language word
    vector<vector<size_t> > m_tgtAlign; //! stores alignment point for each target language word
    vector<size_t> m_srcAlignCount; //! count of aligment points for each src word 
    vector<size_t> m_tgtAlignCount; //! count of aligment points for each tgt word
    size_t m_srcLength;
    size_t m_tgtLength;
    unsigned int m_sentenceID;
  };
}

#endif