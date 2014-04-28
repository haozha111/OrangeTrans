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

#include "PhraseTransTblBuilder.h";
#include "..\util\BasicMethod.h";
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
    while (!getline(fin, line)){
      vector<string> tmpVec = BasicMethod::Split(line);
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

  double LexicalTable::GetLexicalWeight(const PhraseTblEntry &entry)
  {
    vector<vector<size_t> > alignedToTgt = entry.AlignedToTarget();
    vector<string> srcphrase = entry.SourcePhraseVec();
    vector<string> tgtphrase = entry.TargetPhraseVec();
    double lexicalWgt = 1.0;
    for (size_t t = 0; t < alignedToTgt.size(); ++t){
      double score = 0;
      string tgtword = tgtphrase[t];
      for (size_t s : alignedToTgt[t]){
        string srcword = srcphrase[s];
        double lexScore = m_lexTable[srcword][tgtword];
        score += lexScore;
      }
      score /= alignedToTgt[t].size();
      lexicalWgt *= score;
    }
    return lexicalWgt;
  }


  //code for PhraseTableEntry
  /**Create a phrase table entry. Initialize phrases, scores and alignment*/
  bool PhraseTblEntry::CreatePhraseTblEntry(const string &srcPhrase
    , const string &tgtPhrase
    , const string &align)
  {
    m_phraseTransWgt = 0;
    m_freq = 0;
    m_srcphraseVec = BasicMethod::Split(srcPhrase);
    m_tgtphraseVec = BasicMethod::Split(tgtPhrase);

    m_srclen = BasicMethod::Split(srcPhrase).size();
    m_tgtlen = BasicMethod::Split(tgtPhrase).size();
    m_alignedToSrc.assign(m_srclen, vector<size_t>());
    m_alignedToTgt.assign(m_tgtlen, vector<size_t>());
    //init alignment of phrase
    vector<string> tmpVec = BasicMethod::Split(align);
    size_t srcWordId, tgtWordId;
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
      //compute lexical weight
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

  string PhraseTblEntry::SourcePhrase() const
  {
    return m_srcphrase;
  }

  string PhraseTblEntry::TargetPhrase() const
  {
    return m_tgtphrase;
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

  void PhraseTblEntry::SetFrequency(size_t freq)
  {
    m_freq = freq;
  }

  //code for PhraseScorer
  PhraseTransTblBuilder::PhraseTransTblBuilder(string pRule, string pInvRule)
    : m_rule(pRule), m_invrule(pInvRule)
  {}

  void PhraseTransTblBuilder::BuildPhraseTransTbl()
  {
    //read extracted rule table.
    ifstream fin1(m_rule);
    ifstream fin2(m_invrule);
    string line1, line2;

    if (!fin1){
      cerr << "Cannot find rule table: " << m_rule
        << ". Please check input path." << endl;
    }

    if (!fin2){
      cerr << "Cannot find rule table: " << m_invrule
        << ". Please check input path." << endl;
    }

    while (getline(fin1, line1)){
      vector<string> tmpVec = BasicMethod::Split(line1, " ||| ");
      if (tmpVec.size() != 3){
        cerr << "Bad rule form detected:" << line1 << endl;
        continue;
      }
      PhraseTblEntry entry;
      entry.CreatePhraseTblEntry(tmpVec[0], tmpVec[1]
        , tmpVec[2]);
    }
  }

  //code for phrase table
  PhraseTable::PhraseTable() : m_size(0) {}

  /**Insert a rule into the phrase table
  *The insert may cause the computation of phrase scores*/
  void PhraseTable::Insert(PhraseTblEntry &entry)
  {

  }
}