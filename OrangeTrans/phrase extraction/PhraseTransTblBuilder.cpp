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

#include "PhraseTransTblBuilder.h"
#include "..\util\Utility.h"
#include <math.h>

namespace OrangeTraining
{
  using namespace OrangeTrans;

  //code for LexicalTable
  void LexicalTable::LoadLexicalTable(string &pLexicaltable)
  {
    using namespace OrangeTrans;
    ifstream fin(pLexicaltable);
    if (!fin){
      cerr << "ERROR: Please check file path " << pLexicaltable << endl;
      return;
    }

    //read lexical table
    string line;
    while (getline(fin, line)){
      vector<string> tmpVec = Utility::Split(line);
      if (tmpVec.size() != 3){
        cerr << "WARNING: lexicon entry: " << line << " is bad data." << endl;
        continue;
      }
      if (m_lexTable.count(tmpVec[0])){
        m_lexTable[tmpVec[0]][tmpVec[1]] = atof(tmpVec[2].c_str());
      }
      else{
        m_lexTable[tmpVec[0]] = map<string, double>();
        m_lexTable[tmpVec[0]][tmpVec[1]] = atof(tmpVec[2].c_str());
      }
    }
    fin.close();
  }

  double LexicalTable::GetLexicalWeight(PhraseTblEntry &entry)
  {
    vector<vector<size_t> > alignedToTgt = entry.AlignedToTarget();
    vector<string> srcphrase = entry.SourcePhraseVec();
    vector<string> tgtphrase = entry.TargetPhraseVec();
    double lexicalWgt = 1.0;
    for (size_t t = 0; t < alignedToTgt.size(); ++t){
      double score = 0;
      string tgtword = tgtphrase[t];
      if (alignedToTgt[t].size() > 0)
      {
        for (size_t s : alignedToTgt[t]){
          string srcword = srcphrase[s];
          double lexScore = m_lexTable[srcword][tgtword];
          score += lexScore;
        }
        score /= (double)alignedToTgt[t].size();
      }
      if (score > 0)
      {
        lexicalWgt *= score;
      }
    }
    return log(lexicalWgt); //return the natural logarithm of score
  }

  //code for PhraseTableEntry
  /**Create a phrase table entry. Initialize phrases, scores and alignment*/
  bool PhraseTblEntry::CreatePhraseTblEntry(string &srcPhrase
    , string &tgtPhrase, string &align)
  {
    m_srcphrase = srcPhrase;
    m_tgtphrase = tgtPhrase;
    m_align = align;
    if (align == "####"){
      return true;
    }
    m_phraseTransWgt = 0;
    m_freq = 1;
    m_srcphraseVec = Utility::Split(srcPhrase);
    m_tgtphraseVec = Utility::Split(tgtPhrase);
    m_srclen = m_srcphraseVec.size();
    m_tgtlen = m_tgtphraseVec.size();
    m_alignedToSrc.assign(m_srclen, vector<size_t>());
    m_alignedToTgt.assign(m_tgtlen, vector<size_t>());
    //init alignment of phrase
    vector<string> tmpVec = Utility::Split(align);
    unsigned int srcWordId, tgtWordId;
    for (auto &aln : tmpVec){
      if (!sscanf_s(aln.c_str(), "%d-%d", &srcWordId, &tgtWordId)){
        cerr << "bad phrase alignment: << aln << T :"
          << tgtPhrase << " S: " << srcPhrase << endl;
        return false;
      }
      //check if exceed length bound
      if (srcWordId >= m_srclen || tgtWordId >= m_tgtlen){
        cerr << "Phrase alignment out of bound! T: " << tgtPhrase
          << " S: " << srcPhrase << endl;
        return false;
      }
      m_alignedToSrc[srcWordId].push_back(tgtWordId);
      m_alignedToTgt[tgtWordId].push_back(srcWordId);
    }
    return true;
  }

  const vector<string>& PhraseTblEntry::SourcePhraseVec() const
  {
    return m_srcphraseVec;
  }

  const vector<string>& PhraseTblEntry::TargetPhraseVec() const
  {
    return m_tgtphraseVec;
  }

  const vector<vector<size_t> >& PhraseTblEntry::AlignedToSource() const
  {
    return m_alignedToSrc;
  }

  const vector<vector<size_t> >& PhraseTblEntry::AlignedToTarget() const
  {
    return m_alignedToTgt;
  }

  size_t PhraseTblEntry::SourceLength() const
  {
    return m_srclen;
  }

  size_t PhraseTblEntry::TargetLength() const
  {
    return m_tgtlen;
  }

  bool PhraseTblEntry::Equals(const PhraseTblEntry &entry) const
  {
    return (m_srcphrase == entry.m_srcphrase)
      && (m_tgtphrase == entry.m_tgtphrase)
      && (m_align == entry.m_align);
  }

  //code for PhraseTransTblBuilder
  PhraseTransTblBuilder::PhraseTransTblBuilder(string pRule
    , string pInvRule
    , string pOutput
    , string pLexs2t
    , string pLext2s)
    : m_rule(pRule)
    , m_invrule(pInvRule)
    , m_output(pOutput)
    , m_plexs2t(pLexs2t)
    , m_plext2s(pLext2s)
  {}

  bool PhraseTransTblBuilder::BuildPhraseTransTbl()
  {
    //read extracted rule table.
    ifstream fin1(m_rule);
    ifstream fin2(m_invrule);
    ofstream fout(m_output);
    ofstream fout1(m_output + ".s2t");
    ofstream fout2(m_output + ".t2s");

    if (!fin1){
      cerr << "Cannot find rule table: " << m_rule
        << ". Please check input path." << endl;
      return false;
    }

    if (!fin2){
      cerr << "Cannot find rule table: " << m_invrule
        << ". Please check input path." << endl;
      return false;
    }

    if (!fout){
      cerr << "Cannot create output file: " << m_output
        << ". Please check output path." << endl;
      return false;
    }

    cerr << "Start building phrase translation table..." << endl;
    //load lexical translation table
    m_lexs2t.LoadLexicalTable(m_plexs2t);
    m_lext2s.LoadLexicalTable(m_plext2s);
    BuildOneDirection(fin1, fout1, m_lexs2t);
    cerr << "Finished: Source to target translation table" << endl;
    BuildOneDirection(fin2, fout2, m_lext2s);
    cerr << "Finished: Taget to source translation table" << endl;
    
    fin1.close();
    fin2.close();
    fout1.close();
    fout2.close();

    fin1.open(m_output + ".s2t");
    fin2.open(m_output + ".t2s");

    MergeTable(fin1, fin2, fout);
    cerr << "Finished: Merged translation table" << endl;

    return true;
  }

  void PhraseTransTblBuilder::BuildOneDirection(ifstream &fin, ofstream &fout, LexicalTable &lex)
  {

    PhraseTable phraseTable = PhraseTable(lex, fout);
    string line;
    while (getline(fin, line)){
      vector<string> tmpVec = Utility::Split(line, " ||| ");
      if (tmpVec.size() != 3){
        cerr << "Bad rule form detected:" << line << endl;
        continue;
      }
      PhraseTblEntry entry;
      entry.CreatePhraseTblEntry(tmpVec[0], tmpVec[1], tmpVec[2]);
      phraseTable.Insert(entry);
    }
    //note that there are left unprocessed phrase pairs in phrase table
    //we can insert a dummy phrasetblentry
    PhraseTblEntry dummy;
    string stop = "####";
    dummy.CreatePhraseTblEntry(stop, stop, stop);
    phraseTable.Insert(dummy);
  }

  //!Merge the two directions of table and generate one united table
  void PhraseTransTblBuilder::MergeTable(ifstream &fin1
    , ifstream &fin2
    , ofstream &fout)
  {
    string line;
    struct phraseInfo{
      string s2tscore;
      string t2sscore;
      string count;
      string align;
    };

    map<string, map<string, phraseInfo> > table;

    while (getline(fin1, line)){
      vector<string> tmp = Utility::Split(line, " ||| ");
      phraseInfo info;
      string src = tmp[0];
      string tgt = tmp[1];
      info.s2tscore = tmp[2];
      info.count = tmp[3];
      info.align = tmp[4];
      if (table.count(src)){
        table[src][tgt] = info;
      }
      else{
        table[src] = map<string, phraseInfo>();
        table[src][tgt] = info;
      }
    }
    fin1.close();

    while (getline(fin2, line)){
      vector<string> tmp = Utility::Split(line, " ||| ");
      string src = tmp[1];
      string tgt = tmp[0];
      table[src][tgt].t2sscore = tmp[2];
    }
    fin2.close();

    for (auto& p1 : table){
      for (auto& p2 : p1.second){
        //PhraseTable form: 
        //src|||tgt|||pr(t|s), prlex(t|s), pr(s|t), prlex(s|t), bonus, reserved ||| frequency ||| align
        fout << p1.first << " ||| " << p2.first << " ||| "
          << p2.second.s2tscore << "  " << p2.second.t2sscore
          << " " << "1 0" << " ||| " << p2.second.count
          << " ||| " << p2.second.align << endl;
      }      
    }
    fout.close();
    //delete tmp phrase tables
    Utility::Delete(m_output + ".s2t");
    Utility::Delete(m_output + ".t2s");
  }

  //code for phrase table
  PhraseTable::PhraseTable(LexicalTable &lexicalTbl
    , ofstream &output)
    : m_count(0)
    , m_lexicalTbl(lexicalTbl)
    , m_output(output)
  {}

  /**Insert a phrase pair into the phrase table. The phrase 
  *table only holds phrase pairs that shares the same source. 
  *If the insert entry has the same source with the phrase table, 
  *the entry may either be inserted into the table or the scores 
  *will be updated. If the insert phrase's source differs from the 
  *table's source, we will calculate all the phrase translation
  *probabilities in the table, and output the table to a tmp rule 
  *file, clear the table, and insert the new rule.
  *\param entry The PhraseTblEntry to be inserted.
  */
  void PhraseTable::Insert(PhraseTblEntry &entry)
  {
    if (m_count == 0){
      entry.m_lexicalWgt = m_lexicalTbl.GetLexicalWeight(entry);
      m_phraseTable.push_back(entry);
      m_count++;
      return;
    }
    //if current entry is identical to last entry in phrase table, 
    //we just update the count.
    if (entry.Equals(m_phraseTable[m_phraseTable.size() - 1])){
      m_count++;
      m_phraseTable[m_phraseTable.size() - 1].m_freq++;
      return;
    }
    //only alignment differs
    if (entry.m_srcphrase == m_phraseTable[m_phraseTable.size() - 1].m_srcphrase
      && entry.m_tgtphrase == m_phraseTable[m_phraseTable.size() - 1].m_tgtphrase){
      //here we just ignore.
      //TODO: only preserve highest frequent alignment
      m_count++;
      m_phraseTable[m_phraseTable.size() - 1].m_freq++;
      return;
    }
    if (entry.m_srcphrase == m_phraseTable[m_phraseTable.size() - 1].m_srcphrase){
      entry.m_lexicalWgt = m_lexicalTbl.GetLexicalWeight(entry);
      m_phraseTable.push_back(entry);
      m_count++;
      return;
    }
    //if source phrase differs from entries' source in phrase table
    //we compute all scores and output the phrase table.
    for (auto& entry : m_phraseTable){
      //compute phrase translation weight and output the table to tmp file
      //format: source phrase ||| target phrase ||| Pr(t|s) Prlex(t|s) ||| frquency ||| alignment
      entry.m_phraseTransWgt = log((double)entry.m_freq / (double)m_count);
      m_output << entry.m_srcphrase << " ||| " << entry.m_tgtphrase
        << " ||| " << entry.m_phraseTransWgt << " " << entry.m_lexicalWgt
        << " ||| " << entry.m_freq << " ||| " << entry.m_align 
        << endl;
    }
    //clear phrase table
    m_phraseTable.clear();
    // if we reach the end of phrase pairs
    if (entry.m_align == "####"){
      return;
    }
    //insert the new entry
    m_count = 1;
    entry.m_lexicalWgt = m_lexicalTbl.GetLexicalWeight(entry);
    m_phraseTable.push_back(entry);
  }


}