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
#include "phrase extraction\PhraseTransTblBuilder.h";
#include "phrase extraction\PhraseTransTblBuilder.h";
#include "phrase extraction\PhraseTblFilter.h";
using namespace std;

void main(int argc, char* argv[])
{
  using namespace OrangeTraining;

  string src = "C:\\Users\\v-haozha\\Desktop\\orange\\data\\chi.txt";
  string tgt = "C:\\Users\\v-haozha\\Desktop\\orange\\data\\eng.txt";
  string align = "C:\\Users\\v-haozha\\Desktop\\orange\\data\\aln.txt";
  string rule = "C:\\Users\\v-haozha\\Desktop\\orange\\work\\extract\\rule";
  string ruleinv = "C:\\Users\\v-haozha\\Desktop\\orange\\work\\extract\\rule.inv";
  //PhraseExtractionPipeline p = PhraseExtractionPipeline(src, tgt, align, rule, ruleinv);
  //p.StartPhraseExtraction();

  /*******************************/
  //string s2t = "C:\\Users\\v-haozha\\Desktop\\orange\\work\\lex\\s2t.lex";
  //string t2s = "C:\\Users\\v-haozha\\Desktop\\orange\\work\\lex\\t2s.lex";

  //LexicalTransPipeline lexpipe;
  //lexpipe.Start(src, tgt, align, s2t, t2s);

  string r1 = "C:\\Users\\v-haozha\\Desktop\\NiuTrans_1.3.0_CWMT2013\\work\\extract\\extract.sorted";
  string r2 = "C:\\Users\\v-haozha\\Desktop\\NiuTrans_1.3.0_CWMT2013\\work\\extract\\extract.inv.sorted";
  string plexs2t = "C:\\Users\\v-haozha\\Desktop\\orange\\work\\lex\\s2t.lex";
  string plext2s = "C:\\Users\\v-haozha\\Desktop\\orange\\work\\lex\\t2s.lex";
  string output = "C:\\Users\\v-haozha\\Desktop\\orange\\work\\model\\phrasetable";
  //PhraseTransTblBuilder builder = PhraseTransTblBuilder(rule + ".sorted", ruleinv + ".sorted", output, plexs2t, plext2s);
  //builder.BuildPhraseTransTbl();

  PhraseTblFilter filter = PhraseTblFilter(rule + ".sorted", rule + ".sorted.pruned");
  filter.Filter(30);
}

