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
#ifndef STACK_DECODER_H_INCLUDED_
#define STACK_DECODER_H_INCLUDED_

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "Hypothesis.h"
#include "HypothesisStack.h"
#include "trie.h"
#include "PhraseTable.h"
#include "..\lm\KnerserNeyBackOff.h"

using namespace std;
using namespace OrangeLM;

namespace OrangeDecoder
{
  class DecodeParams
  {
  public:
    DecodeParams();
    double transWgt;
    double lmWgt;
    double reorderWgt;
    double futurecostWgt;
    double transpWgt;
    double transplexWgt;
    double transpinvWgt;
    double transplexinvWgt;
    double tgtlen;
  };

  class StackDecoder
  {
  public:
    StackDecoder(DecodeParams &decodeParams, DecodeOption &decodeOption);
    ~StackDecoder();
    void DecodeSentence(string srcsentn);
    void Decode(string phrasetbl, string msdtable, string lmpath, string testset, string output1best);
    void Clear();
  private:
    void ReadPhraseTable(string path);
    void ReadMSDTable(string path);
    void EstimateFutureCost();
    void Output1Best();

    KnerserNeyBackOff* m_lmProvider;//! language model component
    PhraseTable m_phratbl;
    vector<HypothesisStack> m_hypoStacks;//! an array of hypothesis stacks
    DecodeParams m_decodeParams;//! feature weights
    DecodeOption m_decodeOption;//! decoding options
    vector<string> m_srcsentn;//! the foreign sentence to translate
    size_t m_srclen;
    double** m_futureCost;//! future cost matrix for any input span
    ofstream m_1bestOutput;//! output file stream for 1best translation
  };
}
#endif