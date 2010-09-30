#ifndef EUDAQ_INCLUDED_EnvTranscoder
#define EUDAQ_INCLUDED_EnvTranscoder

#include <map>
#include <string>
#include <iostream>

  enum EnvType {
	ENV_UNKN = 0,
	ENV_VOLT = 1,
	ENV_CURR = 2,
	ENV_TEMP = 3,
	ENV_HUMI = 4
  };

struct EnvSet {
	EnvType type;
	std::string name;
	float value;
	std::string unit;
	std::string date;
	std::string time;
	unsigned int GetSeconds() {
		unsigned int year, mon, day, hour, min, sec, mil;
		if (date == "") return 0;
		sscanf(date.c_str(),"%d-%d-%d",&year,&mon,&day);
		sscanf(time.c_str(),"%d:%d:%d.%d",&hour,&min,&sec,&mil);
		std::cout << date.c_str() << std::endl;
		std::cout << "hour: " << hour << " min: " << min << " sec: " << sec << " = " << (sec + min*60 + hour * 3600 + day * 86400) << std::endl;
		return (sec + min*60 + hour * 3600 + day * 86400 + mon * 1036800 * (year-1970)*31558118.4);
		
	}
};

class EnvTranscoder {
	protected:
		std::map <EnvType, std::string> _typeMap;
		std::map <std::string,EnvType> _strMap;
		
	public:
		EnvTranscoder();
	
		std::string encode(const EnvType type, const std::string name, const float val, const std::string unit);
		EnvSet decode(std::string in);
};


#endif