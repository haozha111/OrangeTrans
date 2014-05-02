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

#include "msd.h";
#include "..\util\BasicMethod.h";
#include <fstream>
#include <iostream>

using namespace OrangeTrans;

namespace OrangeTraining
{
  //code for SentnInfo
  string SentnInfo::CreatePhrase(size_t s, size_t e)
  {
    string phrase = "";
    for (size_t k = s; k <= e; ++k){
      if (k != e){
        phrase += m_words[k] + ' ';
      }
      else{
        phrase += m_words[k];
      }
    }
    return phrase;
  }

  void SentnInfo::InsertPhrase(string srcphra, string tgtphra
    , size_t fs, size_t fe, size_t es, size_t ee)
  {
    MSDPhrasePair phrapair;
    phrapair.m_srcphra = srcphra;
    phrapair.m_tgtphra = tgtphra;
    phrapair.m_fs = fs; phrapair.m_fe = fe;
    phrapair.m_es = es; phrapair.m_ee = ee;
    m_ppstart[es].push_back(phrapair);
    m_ppend[ee].push_back(phrapair);
  }

  //code for MSD
  //! constructor for MSD reordering module
  MSD::MSD(const string &src, const string &tgt
    , const string &aln) : m_src(src)
    , m_tgt(tgt)
    , m_aln(aln)
  {}
  //
  bool MSD::ProcessLine()
  {
    ifstream finsrc(m_src);
    ifstream fintgt(m_tgt);
    ifstream finaln(m_aln);
    string srcline, tgtline, alnline;
    //check validity of input files
    if (!finsrc){
      cerr << "Please check source file: " << m_src << endl;
      return false;
    }
    if (!fintgt){
      cerr << "Please check target file: " << m_tgt << endl;
      return false;
    }
    if (!finaln){
      cerr << "Please check alignment file: " << m_aln << endl;
      return false;
    }

    //start whole process
    while (getline(finsrc, srcline)
      && getline(fintgt, tgtline)
      && getline(finaln, alnline)){
      SentnInfo srcsen, tgtsen;
      srcsen.m_words = BasicMethod::Split(srcline);
      tgtsen.m_words = BasicMethod::Split(tgtline);
      srcsen.m_align.assign(srcsen.m_words.size(), vector<size_t>());
      tgtsen.m_align.assign(tgtsen.m_words.size(), vector<size_t>());
      srcsen.m_alnmax.assign(srcsen.m_words.size(), -1);
      tgtsen.m_alnmax.assign(tgtsen.m_words.size(), -1);
      srcsen.m_alnmin.assign(srcsen.m_words.size(), -1);
      tgtsen.m_alnmin.assign(tgtsen.m_words.size(), -1);


      for (auto& align : BasicMethod::Split(alnline))
      {
        unsigned int srcWordId, tgtWordId;
        //check aligment format
        if (!sscanf_s(align.c_str(), "%d-%d", &srcWordId, &tgtWordId)){
          cerr << "WARNING: " << align << " is a bad aligment point in sentence " << endl
            << "T: " << tgtline << endl << "S: " << srcline << endl;
        }
        //check out of bound case
        if ((size_t)srcWordId >= srcsen.m_words.size() || (size_t)tgtWordId >= tgtsen.m_words.size()){
          cerr << "WARNING: " << align << " out of bounds." << endl
            << "T: " << tgtline << endl << "S: " << srcline << endl;
        }

        srcsen.m_align[srcWordId].push_back(tgtWordId);
        tgtsen.m_align[tgtWordId].push_back(srcWordId);

        if (srcsen.m_alnmax[srcWordId] == -1){
          srcsen.m_alnmax[srcWordId] = tgtWordId;
        }
        else{
          if (tgtWordId > srcsen.m_alnmax[srcWordId]){
            srcsen.m_alnmax[srcWordId] = tgtWordId;
          }
        }
        if (tgtsen.m_alnmax[tgtWordId] == -1){
          tgtsen.m_alnmax[tgtWordId] = srcWordId;
        }
        else{
          if (srcWordId > tgtsen.m_alnmax[tgtWordId]){
            tgtsen.m_alnmax[tgtWordId] = srcWordId;
          }
        }
        if (srcsen.m_alnmin[srcWordId] == -1){
          srcsen.m_alnmin[srcWordId] = tgtWordId;
        }
        else{
          if (tgtWordId < srcsen.m_alnmin[srcWordId]){
            srcsen.m_alnmin[srcWordId] = tgtWordId;
          }
        }
        if (tgtsen.m_alnmin[tgtWordId] == -1){
          tgtsen.m_alnmin[tgtWordId] = srcWordId;
        }
        else{
          if (srcWordId < tgtsen.m_alnmin[tgtWordId]){
            tgtsen.m_alnmin[tgtWordId] = srcWordId;
          }
        }

        srcsen.m_align[srcWordId].push_back(tgtWordId);
        tgtsen.m_align[tgtWordId].push_back(srcWordId);      
      }
      ExtractPhrasePairs(srcsen, tgtsen);
      CalculateMSD(srcsen, tgtsen);
      OutputMSDtemp(tgtsen, )
    }
  }

  void MSD::ExtractPhrasePairsGivenSrc(SentnInfo &srcsen, SentnInfo &tgtsen
    , size_t fs, size_t fe, size_t minaln, size_t maxaln)
  {
    string srcphra, tgtphra;
    int i = 0, j = 0;
    /* Check whether all words within e phrase ranging from <minaln> to
    * <maxaln> aligned to f phrase from <fs> to <fe>
    */
    for (i = minaln; i <= maxaln; i++) {
      if (tgtsen.m_alnmin[i] == -1)
        continue;
      if (tgtsen.m_alnmin[i] < fs || tgtsen.m_alnmax[i] > fe)
        return;
    }
    srcphra = srcsen.CreatePhrase(fs, fe);
    tgtphra = tgtsen.CreatePhrase(minaln, maxaln);
    tgtsen.InsertPhrase(srcphra, tgtphra, fs, fe, minaln, maxaln);
    /* Check if we can extend previous e phrase in both directions
    */
    for (i = minaln; i >= 0; --i) {
      if (i < minaln && tgtsen.m_alnmin[i] != -1)
        break;
      for (j = maxaln; j < tgtsen.m_words.size(); ++j) {
        if (i == minaln && j == maxaln)
          continue;

        if (j > maxaln && tgtsen.m_alnmin[j] != -1)
          break;
        if (j - i + 1 > MaxExtractPhraLen)
          break;
        srcphra = srcsen.CreatePhrase(fs, fe);
        tgtphra = tgtsen.CreatePhrase(i, j);
        tgtsen.InsertPhrase(srcphra, tgtphra, fs, fe, i, j);
      }
    }
  }

  void MSD::ExtractPhrasePairs(SentnInfo &srcsen, SentnInfo &tgtsen)
  {
    int i = 0, j = 0, k = 0;
    int minaln = 0, maxaln = 0;
    for (i = 0; i < srcsen.m_words.size(); ++i) {
      minaln = maxaln = -1;

      for (j = i; j < srcsen.m_words.size(); ++j) {
        if (j - i + 1 > MaxExtractPhraLen) /* too long ? */
          break;

        /* Update <minaln> and <maxaln>
        */
        if (minaln == -1) {
          minaln = srcsen.m_alnmin[j];
          maxaln = srcsen.m_alnmax[j];
        }
        else if (srcsen.m_alnmin[j] != -1) {
          if (minaln > srcsen.m_alnmin[j])
            minaln = srcsen.m_alnmin[j];
          if (maxaln < srcsen.m_alnmax[j])
            maxaln = srcsen.m_alnmax[j];
        }

        if (maxaln - minaln + 1 > MaxExtractPhraLen)
          /* means f[i..j] corresponds to a too
          long e phrase */
          break;
        if (minaln == -1) /* f[i..j] doesn't align to any e words,
                           which means this phrase can't exist
                           on its own, it's either a part of some
                           other phrases or not a phrase at all */
                           continue;
        ExtractPhrasePairsGivenSrc(srcsen, tgtsen, i, j, minaln, maxaln);
      }
    }
  }

  void MSD::CalculateMSD(SentnInfo &srcsen, SentnInfo &tgtsen)
  {
    size_t preEend, nextEstart;
    for (int i = 0; i < tgtsen.m_words.size(); ++i){
      /*calculate previous MSD*/
      for (MSDPhrasePair &phrapair : tgtsen.m_ppstart[i]){
        if (i == 0){
          if (phrapair.m_fs != 0){
            phrapair.m_pd = 1;
            m_pDttl++;
          }
          else{
            phrapair.m_pm = 1;
            m_pMttl++;
          }
          continue;
        }
        preEend = i - 1;
        bool flag = false;
        for (size_t aln : tgtsen.m_align[preEend]){
          //M
          if (aln == phrapair.m_fs - 1){
            phrapair.m_pm = 1;
            m_pMttl++;
            flag = true;
            break;
          }
          //S
          if (aln == phrapair.m_fe + 1){
            phrapair.m_ps = 1;
            m_pSttl++;
            flag = true;
            break;
          }
        }
        if (!flag){
          phrapair.m_pd = 1;
          m_pDttl++;
        }
      }

      //calculate next M,S,D
      for (MSDPhrasePair &phrapair : tgtsen.m_ppend[i]){
        if (i == tgtsen.m_words.size() - 1){
          if (phrapair.m_fe == srcsen.m_words.size() - 1){
            phrapair.m_nm = 1;
            m_nMttl++;
          }
          else{
            phrapair.m_nd = 1;
            m_nDttl++;
          }
          continue;
        }
        nextEstart = i + 1;
        bool flag = false;
        for (size_t aln : tgtsen.m_align[nextEstart]){
          //M
          if (aln == phrapair.m_fe + 1){
            phrapair.m_nm = 1;
            m_nMttl++;
            flag = true;
            break;
          }
          //S
          if (aln == phrapair.m_fs - 1){
            phrapair.m_ns = 1;
            m_nSttl++;
            flag = true;
            break;
          }
        }
        if (!flag){
          phrapair.m_nd = 1;
          m_nDttl++;
        }
      }
    }
  }

  void MSD::OutputMSDtemp(SentnInfo &tgtsen, ofstream &fout)
  {
    for (size_t i = 0; i < tgtsen.m_words.size(); ++i){
      for (MSDPhrasePair &phrapair : tgtsen.m_ppstart[i]){
        if (phrapair.m_fe - phrapair.m_fs + 1 > MaxExtractPhraLen
          || phrapair.m_ee - phrapair.m_es + 1 > MaxExtractPhraLen){
          continue;
        }
        tempPhrapairs++;
        fout << phrapair.m_srcphra << " ||| " << phrapair.m_tgtphra
          << " ||| " << phrapair.m_pm << " " << phrapair.m_ps << " "
          << phrapair.m_pd << phrapair.m_nm << " " << phrapair.m_ns
          << " " << phrapair.m_nd << endl;
      }
    }

    fout.close();
  }

}