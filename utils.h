#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>
#include <QString>

class Utils
{
	public:
		Utils();

		static inline std::string &ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
		}

		// trim from end
		static inline std::string &rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
		}

		// trim from both ends
		static inline std::string &trim(std::string &s) {
			return ltrim(rtrim(s));
		}
		template<class TContainer>
		static bool begins_with(const TContainer& input, const TContainer& match)
		{
			return input.size() >= match.size()
					&& equal(match.begin(), match.end(), input.begin());
		}
		static std::string removeAccented( std::string str ) {
			std::string copy = "";
			const char *p = str.c_str();
			while ( (*p)!=0 ) {
				const char*
				//   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
				tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
				unsigned char ch = (*p);
				if ( ch >=192 ) {
					copy += tr[ ch-192 ];
				}else
					copy += *p;
				++p;
			}
			return copy;
		}

		static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim)) {
				elems.push_back(item);
			}
			return elems;
		}

		static std::vector<std::string> split(const std::string &s, char delim) {
			std::vector<std::string> elems;
			split(s, delim, elems);
			return elems;
		}
		static std::string removeFileExtension(std::string& filename){
			std::string filename_without_ext = filename.substr(0, filename.find_last_of("."));
			return filename_without_ext;
		}

		static std::wstring toWString(std::string s){
			return QString::fromStdString(s).toStdWString();
		}
};

#endif // UTILS_H
