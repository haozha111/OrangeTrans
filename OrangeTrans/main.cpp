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

#include <iostream>
#include <vector>
#include <string>

#include "phrase extraction\PhraseExtractionPipeline.h";
#include "phrase extraction\LexicalTransPipeline.h";

using namespace std;

void main(int argc, char* argv[])
{
	using namespace OrangeTraining;
	string src = "C:\\Users\\Administrator\\Desktop\\orange\\training data\\chinese.txt";
	string tgt = "C:\\Users\\Administrator\\Desktop\\orange\\training data\\english.txt";
	string align = "C:\\Users\\Administrator\\Desktop\\orange\\training data\\Alignment.txt";
	string rule = "C:\\Users\\Administrator\\Desktop\\orange\\work\\phrase extraction\\rule.txt";
	string ruleinv = "C:\\Users\\Administrator\\Desktop\\orange\\work\\phrase extraction\\ruleinv.txt";
	/*PhraseExtractionPipeline p = PhraseExtractionPipeline(src, tgt, align, rule, ruleinv);
	p.StartPhraseExtraction();*/


	/*******************************/
	string s2t = "C:\\Users\\Administrator\\Desktop\\orange\\work\\phrase extraction\\s2t.txt";
	string t2s = "C:\\Users\\Administrator\\Desktop\\orange\\work\\phrase extraction\\t2s.txt";

	LexicalTransPipeline lexpipe;
	lexpipe.Start(src, tgt, align, s2t, t2s);
	

}

