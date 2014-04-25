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

#include "WordAlignment.h";
#include "..\util\BasicMethod.h";

#include <iostream>;

using namespace std;

namespace OrangeTraining
{
	/**Create a word alignment.
	*\param sourceLangSentence  the source language sentence
	*\param targetLangSentence  the target language sentence
	*\param alignment  the aligment between sentence pair
	*/
	WordAlignment::WordAlignment(
		const string &sourceLangSentence
		, const string &targetLangSentence
		, const string &alignment
		, unsigned int sentenceID)
	{
		if (sourceLangSentence == ""){
			cerr << endl
				<< " WARNING: EMPTY sentence detected: " << sentenceID << " line in source file." << endl;
		}
		if (targetLangSentence == ""){
			cerr << endl
				<< " WARNING: EMPTY sentence detected: " << sentenceID << " line in target file." << endl;
		}
		if (alignment == ""){
			cerr << endl
				<< " WARNING: EMPTY alignment detected: " << sentenceID << " line in alignment file." << endl;
		}

		this->m_sourceLangSentence = OrangeTrans::BasicMethod::ConvertStringtoVec(sourceLangSentence);
		this->m_targetLangSentence = OrangeTrans::BasicMethod::ConvertStringtoVec(targetLangSentence);
		this->m_sentenceID = sentenceID;
		this->m_sourceLength = m_sourceLangSentence.size();
		this->m_targetLength = m_targetLangSentence.size();

		vector<string> tmpVec = OrangeTrans::BasicMethod::ConvertStringtoVec(alignment);
		size_t srclen = m_sourceLangSentence.size();
		size_t tgtlen = m_targetLangSentence.size();

		//intialize data structures
		this->m_srcAlignment.assign(srclen, vector<size_t>());
		this->m_tgtAlignment.assign(tgtlen, vector<size_t>());
		this->m_srcAlignmentCount.assign(srclen, 0);
		this->m_tgtAlignmentCount.assign(tgtlen, 0);

		for each (string align in tmpVec)
		{
			unsigned int srcWordId, tgtWordId;
			//check aligment format
			if (!sscanf_s(align.c_str(), "%d-%d", &srcWordId, &tgtWordId)){
				cerr << "WARNING: " << align << " is a bad aligment point in sentence " << sentenceID << endl;
				cerr << "T: " << targetLangSentence << endl << "S: " << sourceLangSentence << endl;
			}
			//check out of bound case
			if ((size_t)srcWordId >= this->m_sourceLangSentence.size() || (size_t)tgtWordId >= this->m_targetLangSentence.size()){
				cerr << "WARNING: " << align << " out of bounds." << endl
					<< "T: " << targetLangSentence << endl << "S: " << sourceLangSentence << endl;
			}

			this->m_srcAlignment[srcWordId].push_back(tgtWordId);
			this->m_tgtAlignment[tgtWordId].push_back(srcWordId);
			this->m_srcAlignmentCount[srcWordId]++;
			this->m_tgtAlignmentCount[tgtWordId]++;
		}

	}

	WordAlignment::~WordAlignment()
	{
		//we clean up now
		Clear();
	}

	/**do some clean up work*/
	void WordAlignment::Clear()
	{
		this->m_sourceLangSentence.clear();
		this->m_targetLangSentence.clear();
		this->m_srcAlignment.clear();
		this->m_tgtAlignment.clear();
	}

	//!get vector representation of souce sentence
	const vector<string> & WordAlignment::SourceSentenceVector() const
	{
		return this->m_sourceLangSentence;
	}

	//! get vector representation of target sentence
	const vector<string> & WordAlignment::TargetSentenceVector() const
	{
		return this->m_targetLangSentence;
	}

	//! return source sentence length
	size_t WordAlignment::SourceLength() const
	{
		return this->m_sourceLength;
	}

	//! return target sentence length
	size_t WordAlignment::TargetLength() const
	{
		return this->m_targetLength;
	}

	//! Get alignment points of source word at position pos
	const vector<size_t> & WordAlignment::GetSourceAlignmentAtPos(size_t pos) const
	{
		return this->m_srcAlignment[pos];
	}

	//! Get alignment points of target word at position pos
	const vector<size_t> & WordAlignment::GetTargetAlignmentAtPos(size_t pos) const
	{
		return this->m_tgtAlignment[pos];
	}

	//! Get count of alignment points of source word at position pos
	size_t WordAlignment::GetSourceAlignmentCountAtPos(size_t pos) const
	{
		if (pos >= 0 && pos < m_srcAlignmentCount.size()){
			return this->m_srcAlignmentCount[pos];
		}
		else{
			throw exception("WordAlignment.GetSourceAlignmentCountAtPos(size_t pos): index out of bounds!");
		}
	}

	//! Get count of alignment points of target word at position pos
	size_t WordAlignment::GetTargetAlignmentCountAtPos(size_t pos) const
	{
		if (pos >= 0 && pos < m_tgtAlignmentCount.size()){
			return this->m_tgtAlignmentCount[pos];
		}
		else{
			throw exception("WordAlignment.GetTargetAlignmentCountAtPos(size_t pos): index out of bounds!");
		}
	}

	//! Get substring(phrase) of source sentence
	string WordAlignment::GetSourceSubstring(size_t startPos, size_t endPos) const
	{
		if (startPos > endPos){
			return "NULL";
		}
		if ((startPos < 0 || startPos >= m_sourceLangSentence.size())
			|| (endPos < 0 || endPos >= m_sourceLangSentence.size())){
			throw exception("WordAlignment.GetSourceSubstring(size_t startPos, size_t endPos): index out of bounds!");
		}


		string src = "";
		for (size_t k = startPos; k <= endPos; ++k){
			src += this->m_sourceLangSentence[k];
			if (k != endPos){
				src += " ";
			}
		}
		return src;
	}

	//! Get substring(phrase) of target sentence
	string WordAlignment::GetTargetSubstring(size_t startPos, size_t endPos) const
	{
		if (startPos > endPos){
			return "NULL";
		}

		if ((startPos < 0 || startPos >= m_targetLangSentence.size())
			|| (endPos < 0 || endPos >= m_targetLangSentence.size())){
			throw exception("WordAlignment.GetTargetSubstring(size_t startPos, size_t endPos): index out of bounds!");
		}

		string tgt = "";
		for (size_t k = startPos; k <= endPos; ++k){
			tgt += this->m_targetLangSentence[k];
			if (k != endPos){
				tgt += " ";
			}
		}
		return tgt;
	}

	//! Get the null-aligned source positions
	vector<size_t> WordAlignment::GetSourceNullAligned() const
	{
		vector<size_t> result;
		for (size_t i = 0; i < m_sourceLength; ++i){
			if (m_srcAlignment[i].size() == 0){
				result.push_back(i);
			}
		}
		return result;
	}

	//! Get the null-aligned target positions
	vector<size_t> WordAlignment::GetTargetNullAligned() const
	{
		vector<size_t> result;
		for (size_t i = 0; i < m_targetLength; ++i){
			if (m_tgtAlignment[i].size() == 0){
				result.push_back(i);
			}
		}
		return result;
	}
}