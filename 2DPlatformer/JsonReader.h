#ifndef _JSON_READER_H_
#define _JSON_READER_H_

#include "rapidjson/document.h"
#include <fstream>

using namespace rapidjson;

class JsonReader
{
public:
	JsonReader();
	JsonReader(const JsonReader& other);
	~JsonReader();

private:
	std::string m_fileName;
};
#endif