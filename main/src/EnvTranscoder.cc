#include "eudaq/EnvTranscoder.hh"

EnvTranscoder::EnvTranscoder() {
	_typeMap[ENV_UNKN] = "UNKN";
	_typeMap[ENV_VOLT] = "VOLT";
	_typeMap[ENV_CURR] = "CURR";
	_typeMap[ENV_TEMP] = "TEMP";
	_typeMap[ENV_HUMI] = "HUMI";
	
	_strMap["UNKN"] = ENV_UNKN;
	_strMap["VOLT"] = ENV_VOLT;
	_strMap["CURR"] = ENV_CURR;
	_strMap["TEMP"] = ENV_TEMP;
	_strMap["HUMI"] = ENV_HUMI;
	
	
	
}

std::string EnvTranscoder::encode(const EnvType type, const std::string name, const float val, const std::string unit) {
	std::string out = _typeMap[type];
	out+= " ";
	out+= name;
	out+= " ";
	char flt[16];
	sprintf(flt,"%.4f",val);
	out+= flt;
	out+= " ";
	out+= unit;
	return out;
}

EnvSet EnvTranscoder::decode(std::string in) {
	EnvSet out;
	char flag[8]; // the messages always start with "ENV: "
	char name[16];
	char unit [8];
	char type [16];
	char date[16];
	char time[16];
	float val;
	sscanf(in.c_str(),"%s %s %s %f %s %s %s",flag,type,name,&val,unit, date, time);
	out.name = std::string(name);
	out.unit = std::string(unit);
	out.type = _strMap[type];
	out.value = val;
	out.date = std::string(date);
	out.time = std::string(time);
	return out;
}