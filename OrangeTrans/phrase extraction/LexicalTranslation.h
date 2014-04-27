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
#ifndef LEXICAL_TRANSLATION_H_INCLUDED_
#define LEXICAL_TRANSLATION_H_INCLUDED_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace OrangeTraining
{
    class LexicalTranslation
    {
    public:
        LexicalTranslation();
        void BuildLexicon(string &psource, string &ptarget
            , string &palign);
        void Write(string &ps2t, string &pt2s);

    private:
        map<string, map<string, size_t> > m_globalCooccur; //!record the cooccur statistics of source word and target word
        map<string, size_t> m_srccount; //!record the total count of source word
        map<string, size_t> m_tgtcount; //!record the total count of target word
        map<string, map<string, double> > m_src2tgtlex; //!source->target lexicon
        map<string, map<string, double> > m_tgt2srclex; //!target->source lexicon
    };
}

#endif