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
#include "..\util\Utility.h";
#include <fstream>
#include <iostream>
#include <math.h>

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
    MSDPhrasePair* phrapair = new MSDPhrasePair();
    phrapair->m_srcphra = srcphra;
    phrapair->m_tgtphra = tgtphra;
    phrapair->m_fs = fs; phrapair->m_fe = fe;
    phrapair->m_es = es; phrapair->m_ee = ee;
    phrapair->m_pm = 0; phrapair->m_ps = 0; phrapair->m_pd = 0;
    phrapair->m_nm = 0; phrapair->m_ns = 0; phrapair->m_nd = 0;
    m_ppstart[es].push_back(phrapair);
    m_ppend[ee].push_back(phrapair);
  }

  //code for MSD
  //! constructor for MSD reordering module
  MSD::MSD(const string &src, const string &tgt
    , const string &aln, const string &outputPath) 
    : m_src(src)
    , m_tgt(tgt)
    , m_aln(aln)
    , m_outputPath(outputPath)
    , m_tmpModel(outputPath + "msd.reordering.model.1")
  {}
  //! MSD model training pipeline
  void MSD::TrainMSD()
  {
    ProcessLine();
    //sort the output file
    Utility::Sort(m_tmpModel);
    Utility::Delete(m_tmpModel);
    //combine results(aggregate M S D scores)
    m_tmpModel += ".sorted";
    Aggregate();
    Utility::Delete(m_tmpModel);
    m_tmpModel += ".aggregated";
    CalculateMSDProb();
    Utility::Delete(m_tmpModel);
    m_tmpModel += ".final";
    string tblname = "msd.reorder.table";
    Utility::Rename(m_tmpModel, tblname);
    cerr << "MSD training over." << endl;
  }
  bool MSD::ProcessLine()
  {
    cerr << "MSD reordering Step1: generating msd.reordering.model.1" << endl;
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
    ofstream fout(m_tmpModel);
    if (!fout){
      cerr << "Couldn't create output file msd.reordering.model.1"
        << endl;
    }
    //start whole process
    int lineNo = 1;
    while (getline(finsrc, srcline)
      && getline(fintgt, tgtline)
      && getline(finaln, alnline)){
      if (lineNo % 10000 == 0){
        cerr << "Processed " << lineNo << " lines." << endl;
      }
      SentnInfo srcsen, tgtsen;
      srcsen.m_words = Utility::Split(srcline);
      tgtsen.m_words = Utility::Split(tgtline);
      srcsen.m_align.assign(srcsen.m_words.size(), vector<size_t>());
      tgtsen.m_align.assign(tgtsen.m_words.size(), vector<size_t>());
      srcsen.m_alnmax.assign(srcsen.m_words.size(), -1);
      tgtsen.m_alnmax.assign(tgtsen.m_words.size(), -1);
      srcsen.m_alnmin.assign(srcsen.m_words.size(), -1);
      tgtsen.m_alnmin.assign(tgtsen.m_words.size(), -1);
      tgtsen.m_ppstart.assign(tgtsen.m_words.size(), vector<MSDPhrasePair*>());
      tgtsen.m_ppend.assign(tgtsen.m_words.size(), vector<MSDPhrasePair*>());

      for (auto& align : Utility::Split(alnline))
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
      OutputMSDtemp(tgtsen, fout);
      lineNo++;
    }
    fout.close();
    finsrc.close();
    fintgt.close();
    finaln.close();
  }

  bool MSD::Aggregate() const
  {
    ifstream fin(m_tmpModel);
    if (!fin){
      cerr << "Cannot find file msd.reordering.model.1.sorted" << endl;
      return false;
    }

    ofstream fout(m_tmpModel + ".aggregated");
    if (!fout){
      cerr << "Cannot create aggregation file: " <<
        m_tmpModel + ".arggregated" << endl;
      return false;
    }

    cerr << "MSD reordering Step2: Aggregating MSD scores..." << endl;
    string line, lastsrc = "", lasttgt = "";
    size_t score[6] = {0};
    vector<string> msdscore;
    while (getline(fin, line)){
      vector<string> tmp = Utility::Split(line, " ||| ");
      if (tmp.size() != 3){
        cerr << "msd reordering table bad data: "
          << line << endl;
        continue;
      }
      if (lastsrc == "" && lasttgt == ""){
        lastsrc = tmp[0]; lasttgt = tmp[1];
        memset(score, 0, sizeof(score));
        msdscore = Utility::Split(tmp[2]);
        for (int i = 0; i < 6; ++i){
          score[i] = atoi(msdscore[i].c_str());
        }
      }
      else{
        if (tmp[0] == lastsrc && tmp[1] == lasttgt){
          msdscore = Utility::Split(tmp[2]);
          for (int i = 0; i < 6; ++i){
            score[i] += atoi(msdscore[i].c_str());
          }
        }
        else{
          //we now output the aggregation of previous pharse pair
          fout << lastsrc << " ||| " << lasttgt << " ||| ";
          for (int i = 0; i < 6; ++i){
            if (i != 5){
              fout << score[i] << " ";
            }
            else{
              fout << score[i] << endl;
            }
          }
          lastsrc = tmp[0]; lasttgt = tmp[1];
          memset(score, 0, sizeof(score));
          msdscore = Utility::Split(tmp[2]);
          for (int i = 0; i < 6; ++i){
            score[i] = atoi(msdscore[i].c_str());
          }
        }
      }
    }
    //output remaining items
    fout << lastsrc << " ||| " << lasttgt << " ||| ";
    for (int i = 0; i < 6; ++i){
      if (i != 5){
        fout << score[i] << " ";
      }
      else{
        fout << score[i] << endl;
      }
    }
    fout.close();
    fin.close();
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
      for (MSDPhrasePair* phrapair : tgtsen.m_ppstart[i]){
        if (i == 0){
          if (phrapair->m_fs != 0){
            phrapair->m_pd = 1;
            m_pDttl++;
          }
          else{
            phrapair->m_pm = 1;
            m_pMttl++;
          }
          continue;
        }
        preEend = i - 1;
        bool flag = false;
        for (size_t aln : tgtsen.m_align[preEend]){
          //M
          if (aln == phrapair->m_fs - 1){
            phrapair->m_pm = 1;
            m_pMttl++;
            flag = true;
            break;
          }
          //S
          if (aln == phrapair->m_fe + 1){
            phrapair->m_ps = 1;
            m_pSttl++;
            flag = true;
            break;
          }
        }
        //D
        if (!flag){
          phrapair->m_pd = 1;
          m_pDttl++;
        }
      }

      //calculate next M,S,D
      for (MSDPhrasePair* phrapair : tgtsen.m_ppend[i]){
        if (i == tgtsen.m_words.size() - 1){
          if (phrapair->m_fe == srcsen.m_words.size() - 1){
            phrapair->m_nm = 1;
            m_nMttl++;
          }
          else{
            phrapair->m_nd = 1;
            m_nDttl++;
          }
          continue;
        }
        nextEstart = i + 1;
        bool flag = false;
        for (size_t aln : tgtsen.m_align[nextEstart]){
          //M
          if (aln == phrapair->m_fe + 1){
            phrapair->m_nm = 1;
            m_nMttl++;
            flag = true;
            break;
          }
          //S
          if (aln == phrapair->m_fs - 1){
            phrapair->m_ns = 1;
            m_nSttl++;
            flag = true;
            break;
          }
        }
        //D
        if (!flag){
          phrapair->m_nd = 1;
          m_nDttl++;
        }
      }
    }
  }

  bool MSD::CalculateMSDProb() const
  {
    ifstream fin(m_tmpModel);
    if (!fin){
      cerr << "Cannot find file msd.reordering.model.1.sorted.aggregated" << endl;
      return false;
    }

    ofstream fout(m_tmpModel + ".final");
    if (!fout){
      cerr << "Cannot create file: " <<
        m_tmpModel + ".final" << endl;
      return false;
    }

    cerr << "MSD reordering Step3: Calculating MSD probabilities..." << endl;
    
    //smoothing factor
    double pmfactor = 0.5;
    double psfactor = 0.5;
    double pdfactor = 0.5;
    double nmfactor = 0.5;
    double nsfactor = 0.5;
    double ndfactor = 0.5;

    string line;
    double pm, ps, pd, nm, ns, nd;
    while (getline(fin, line)){
      vector<string> tmp = Utility::Split(line, " ||| ");
      if (!sscanf_s(tmp[2].c_str(), "%lf %lf %lf %lf %lf %lf"
        , &pm, &ps, &pd, &nm, &ns, &nd)){
        cerr << "Bad data in MSD table, line: "
          << line << endl;
      }
      double pttl = pm + pmfactor + ps + psfactor + pd + pdfactor;
      double nttl = nm + nmfactor + ns + nsfactor + nd + ndfactor;
      fout << tmp[0] << " ||| " << tmp[1] << " ||| "
        << log((pm + pmfactor) / pttl) << " " << log((ps + psfactor) / pttl)
        << " " << log((pd + pdfactor) / pttl) << " " << log((nm + nmfactor) / nttl)
        << " " << log((ns + nsfactor) / nttl) << " " << log((nd + ndfactor) / nttl)
        << endl;
    }
    fout.close();
    fin.close();

  }

  void MSD::OutputMSDtemp(SentnInfo &tgtsen, ofstream &fout)
  {
    for (size_t i = 0; i < tgtsen.m_words.size(); ++i){
      for (MSDPhrasePair* phrapair : tgtsen.m_ppstart[i]){
        if (phrapair->m_fe - phrapair->m_fs + 1 > MaxExtractPhraLen
          || phrapair->m_ee - phrapair->m_es + 1 > MaxExtractPhraLen){
          continue;
        }
        m_tempPhrapairs++;
        fout << phrapair->m_srcphra << " ||| " << phrapair->m_tgtphra
          << " ||| " << phrapair->m_pm << " " << phrapair->m_ps << " "
          << phrapair->m_pd << " " << phrapair->m_nm << " " << phrapair->m_ns
          << " " << phrapair->m_nd << endl;
      }
    }
  }

}