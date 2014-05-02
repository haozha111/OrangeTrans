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
#ifndef MSD_H_INCLUDED_
#define MSD_H_INCLUDED_

#include <vector>
#include <string>

using namespace std;

namespace OrangeTraining
{
  class MSDPhrasePair
  {
  public:
    string m_srcphra;
    string m_tgtphra;
    size_t m_fs, m_fe;
    size_t m_es, m_ee;
    size_t m_pm, m_ps, m_pd;
    size_t m_nm, m_ns, m_nd;
  };

  class SentnInfo
  {
  public:
    string CreatePhrase(size_t s, size_t e);
    void InsertPhrase(string srcphra, string tgtphra
      , size_t fs, size_t fe, size_t es, size_t ee);
    vector<string> m_words;
    vector<vector<size_t> > m_align;
    vector<int> m_alnmin;
    vector<int> m_alnmax;
    vector<vector<MSDPhrasePair> > m_ppstart;
    vector<vector<MSDPhrasePair> > m_ppend;
  };

  class MSD
  {
  public:
    MSD(const string &src, const string &tgt
      , const string &aln);
    void TrainMSD();
  private:
    void ExtractPhrasePairs(SentnInfo &srcsen, SentnInfo &tgtsen);
    void ExtractPhrasePairsGivenSrc(SentnInfo &srcsen, SentnInfo &tgtsen
      , size_t fs, size_t fe, size_t min_aln, size_t max_aln);
    void CalculateMSD(SentnInfo &srcsen, SentnInfo &tgtsen);
    void OutputMSDtemp(SentnInfo &tgtsen, ofstream &fout);
    bool ProcessLine();
    string m_src;
    string m_tgt;
    string m_aln;
    static const int MaxExtractPhraLen = 7;
    size_t m_pDttl = 0;
    size_t m_pMttl = 0;
    size_t m_pSttl = 0;
    size_t m_nDttl = 0;
    size_t m_nMttl = 0;
    size_t m_nSttl = 0;
    size_t tempPhrapairs = 0;
  };
}

#endif