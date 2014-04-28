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

#include "BasicMethod.h";
#include <sstream>

using namespace std;

namespace OrangeTrans
{
  /**Split input string into a list of substrings with delimiter.
  * \param stringtoSplit  The string that is to be splitted
  * \param delimiter  The user specified delimiter
  * \return the list of strings separated by the given delimiter
  */
  vector<string> BasicMethod::Split(const string &stringtoSplit, const string &delimiter)
  {
    string::size_type start = 0, index = 0;
    string::size_type delimiter_len = delimiter.size();
    vector<string> splitted;

    while (index != string::npos && start < stringtoSplit.size()){
      index = stringtoSplit.find(delimiter, start);
      if (index == start){
        start += delimiter_len;
        continue;
      }
      if (index == string::npos){
        splitted.push_back(stringtoSplit.substr(start));
        break;
      }
      splitted.push_back(stringtoSplit.substr(start, index - start));
      start = index + delimiter_len;
    }
    return splitted;
  }

  /**split input string by space*/
  vector<string> BasicMethod::Split(const string &stringtoSplit)
  {
    vector<string> res;
    stringstream sin(stringtoSplit);
    string word;
    while (sin >> word) {
      res.push_back(word);
    }
    return res;
  }

  /**sort input file using cygwin sort.exe*/
  void BasicMethod::Sort(string &inputFile)
  {
    string cmd = "c:\\cygwin64\\bin\\sort.exe " + inputFile + " > " + inputFile + ".sorted"; 
    system(cmd.c_str());
  }

  /**Delete the input file using system command*/
  void BasicMethod::Delete(string &inputFile)
  {
    string cmd = "del " + inputFile;
    system(cmd.c_str());
  }
}

