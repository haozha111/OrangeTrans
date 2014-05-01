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
#ifndef PHRASETBL_FILTER_H_INCLUDED_
#define PHRASETBL_FILTER_H_INCLUDED_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace OrangeTraining
{
  class PhraseTblFilter
  {
  public:
    PhraseTblFilter(const string &pruletable, const string &output);
    //!filter the phrase table by only preserving top k(default 30) translation options
    //for each source phrase
    void Filter(size_t topK) const;

  private:
    string m_pruletable;
    string m_output;
    bool cmp(const pair<string, size_t> &a, const pair<string, size_t> &b);
  };
}

#endif