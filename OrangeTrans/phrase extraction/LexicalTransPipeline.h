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
#ifndef LEXICAL_TRANS_PIPELINE_H_INCLUDED_
#define LEXICAL_TRANS_PIPELINE_H_INCLUDED_

#include <string>

using namespace std;

namespace OrangeTraining
{
  class LexicalTransPipeline
  {
  public:
    LexicalTransPipeline();
    void Start(string &psrc, string &ptgt, string &palign
      , string &ps2t, string &pt2s);
  };
}

#endif