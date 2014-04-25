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
#ifndef WORD_ALIGNMENT_H_INCLUDED_
#define WORD_ALIGNMENT_H_INCLUDED_

#include "..\util\BasicMethod.h";
#include <string>
#include <vector>

namespace OrangeTraining
{
	/**The WordAlignment is used for storing source language sentence, 
	* target language sentence, and the alignment between them.
	**/

	class WordAlignment
	{
	public:
		//!constructor for the word alignment 
		WordAlignment(
			const std::string &sourceLangSentence
			,const std::string &targetLangSentence
			,const std::string &alignment
			,unsigned int sentenceID);
		~WordAlignment();

		//methods

		void Clear();
		const std::vector<size_t> & GetSourceAlignmentAtPos(size_t pos) const;
		const std::vector<size_t> & GetTargetAlignmentAtPos(size_t pos) const;
		size_t GetSourceAlignmentCountAtPos(size_t pos) const;
		size_t GetTargetAlignmentCountAtPos(size_t pos) const;
		std::string GetSourceSubstring(size_t startPos, size_t endPos) const;
		std::string GetTargetSubstring(size_t startPos, size_t endPos) const;
		std::vector<size_t> GetSourceNullAligned() const;
		std::vector<size_t> GetTargetNullAligned() const;

		//attributes
		const std::vector<std::string> & SourceSentenceVector() const;
		const std::vector<std::string> & TargetSentenceVector() const;
		size_t SourceLength() const;
		size_t TargetLength() const;

	private:
		std::vector<std::string> m_sourceLangSentence; //! stores each word of the source language sentence into a vector
		std::vector<std::string> m_targetLangSentence; //! stores each word of the target language sentence into a vector
		std::vector<std::vector<size_t> > m_srcAlignment; //! stores alignment point for each source language word
		std::vector<std::vector<size_t> > m_tgtAlignment; //! stores alignment point for each target language word
		std::vector<size_t> m_srcAlignmentCount; //! count of aligment points for each src word 
		std::vector<size_t> m_tgtAlignmentCount; //! count of aligment points for each tgt word
		size_t m_sourceLength;
		size_t m_targetLength;
		unsigned int m_sentenceID; 
	};
}

#endif