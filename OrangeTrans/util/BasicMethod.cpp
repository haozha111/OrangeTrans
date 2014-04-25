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

	vector<string> BasicMethod::ConvertStringtoVec(const string &sourceString)
	{
		return BasicMethod::Split(sourceString, " ");
	}


}

