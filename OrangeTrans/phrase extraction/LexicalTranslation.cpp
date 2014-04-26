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

#include "LexicalTranslation.h";
#include "..\util\BasicMethod.h";

namespace OrangeTraining
{
	LexicalTranslation::LexicalTranslation(){};
	
	void LexicalTranslation::BuildLexicon(string &psource, string &ptarget
		, string &palign)
	{
		string srcline, tgtline, alignline;
		ifstream source(psource);
		ifstream target(ptarget);
		ifstream align(palign);

		size_t sentenceID = 1;
		while (getline(source, srcline)
			&& getline(target, tgtline)
			&& getline(align, alignline)){
			
			if (srcline == ""){
				cerr << endl
					<< " WARNING: EMPTY sentence detected: " << sentenceID << " line in source file." << endl;
			}
			if (tgtline == ""){
				cerr << endl
					<< " WARNING: EMPTY sentence detected: " << sentenceID << " line in target file." << endl;
			}
			if (alignline == ""){
				cerr << endl
					<< " WARNING: EMPTY alignment detected: " << sentenceID << " line in alignment file." << endl;
			}

			using namespace OrangeTrans;

			vector<string> tmpVec = BasicMethod::Split(alignline);
			vector<string> srcVec = BasicMethod::Split(srcline);
			vector<string> tgtVec = BasicMethod::Split(tgtline);
			vector<size_t> srcAlignCount(srcVec.size(), 0);
			vector<size_t> tgtAlignCount(tgtVec.size(), 0);

			for (auto& align : tmpVec){
				unsigned int srcWordId, tgtWordId;
				//check aligment format
				if (!sscanf_s(align.c_str(), "%d-%d", &srcWordId, &tgtWordId)){
					cerr << "WARNING: " << align << " is a bad aligment point in sentence " << sentenceID << endl;
					cerr << "T: " << tgtline << endl << "S: " << srcline << endl;
				}
				//check out of bound case
				if ((size_t)srcWordId >= srcVec.size() || (size_t)tgtWordId >= tgtVec.size()){
					cerr << "WARNING: " << align << " out of bounds." << endl
						<< "T: " << tgtline << endl << "S: " << srcline << endl;
				}

				string srcword = srcVec[srcWordId];
				string tgtword = tgtVec[tgtWordId];

				srcAlignCount[srcWordId]++;
				tgtAlignCount[tgtWordId]++;

				//add words to global count dictionary
				if (m_srccount.count(srcword)){
					m_srccount[srcword]++;
				}
				else{
					m_srccount[srcword] = 1;
				}

				if (m_tgtcount.count(tgtword)){
					m_tgtcount[tgtword]++;
				}
				else{
					m_tgtcount[tgtword] = 1;
				}

				//add cooccur
				if (m_globalCooccur.count(srcword)){
					if (m_globalCooccur[srcword].count(tgtword)){
						m_globalCooccur[srcword][tgtword]++;
					}
					else{
						m_globalCooccur[srcword][tgtword] = 1;
					}
				}
				else{
					m_globalCooccur[srcword] = map<string, size_t>();
					m_globalCooccur[srcword][tgtword] = 1;
				}
			}

			//add NULL translation e.g. NULL->X  X->NULL
			for (int i = 0; i < srcAlignCount.size(); ++i){
				if (srcAlignCount[i] == 0){
					//source -> NULL
					if (m_srccount.count(srcVec[i])){
						m_srccount[srcVec[i]]++;
					}
					else{
						m_srccount[srcVec[i]] = 1;
					}
					if (m_tgtcount.count("NULL")){
						m_tgtcount["NULL"]++;
					}
					else{
						m_tgtcount["NULL"] = 1;
					}
					if (m_globalCooccur.count(srcVec[i])){
						if (m_globalCooccur[srcVec[i]].count("NULL")){
							m_globalCooccur[srcVec[i]]["NULL"]++;
						}
						else{
							m_globalCooccur[srcVec[i]]["NULL"] = 1;
						}
					}
					else{
						m_globalCooccur[srcVec[i]] = map<string, size_t>();
						m_globalCooccur[srcVec[i]]["NULL"] = 1;
					}
				}
			}

			for (int i = 0; i < tgtAlignCount.size(); ++i){
				if (tgtAlignCount[i] == 0){
					//NULL -> target
					if (m_tgtcount.count(tgtVec[i])){
						m_tgtcount[tgtVec[i]]++;
					}
					else{
						m_tgtcount[tgtVec[i]] = 1;
					}
					if (m_srccount.count("NULL")){
						m_srccount["NULL"]++;
					}
					else{
						m_srccount["NULL"] = 1;
					}
					if (m_globalCooccur.count("NULL")){
						if (m_globalCooccur["NULL"].count(tgtVec[i])){
							m_globalCooccur["NULL"][tgtVec[i]]++;
						}
						else{
							m_globalCooccur["NULL"][tgtVec[i]] = 1;
						}
					}
					else{
						m_globalCooccur["NULL"] = map<string, size_t>();
						m_globalCooccur["NULL"][tgtVec[i]] = 1;
					}
				}
			}
			if (sentenceID % 10000 == 0){
				cerr << "\r  " << sentenceID << " sentences processed." << endl;
			}
			sentenceID++;
		}
		source.close();
		target.close();
		align.close();
	}

	void LexicalTranslation::Write(string &ps2t, string &pt2s)
	{
		ofstream s2t(ps2t);
		ofstream t2s(pt2s);
		
		for (auto& p1 : m_globalCooccur){
			for (auto& p2 : p1.second){
				//write src -> tgt lexicon  p(t|s)
				s2t << p1.first << " " << p2.first << " "
					<< (double)p2.second / (double)m_srccount[p1.first]
					<< " " << endl;
				//write tgt -> src lexicon  p(s|t)
				t2s << p2.first << " " << p1.first << " "
					<< (double)p2.second / (double)m_tgtcount[p2.first]
					<< " " << endl;
			}
		}

		s2t.close();
		t2s.close();
	}
	
}