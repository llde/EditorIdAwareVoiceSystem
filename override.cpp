#include "utils.h"
#include "finder.h"
#include "override.h"

#include <unordered_map>
#include <string>
#include <vector>

static std::unordered_map<std::string, std::vector<std::string>>  raceOverrides;

//For now overrideFile is ignored
void InitializeOverrides(const char* overrideFile){
	if (overrideFile == nullptr) _WARNING("Override file still not implemented!");
	std::vector<std::string> raceOver;
	raceOver.push_back("high elf");
	raceOver.push_back("highelf"); //The race editor id.
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("elfo supremo", raceOver));
	std::vector<std::string> raceOverA;
	raceOverA.push_back("elfo supremo");
	raceOverA.push_back("highelf"); //The race editor id.
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("high elf", raceOverA));
	std::vector<std::string> raceOver1;
	raceOver1.push_back("argonian");
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("argoniano", raceOver1));
	std::vector<std::string> raceOver2;
	raceOver2.push_back("imperial");
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("imperiale", raceOver2));
	std::vector<std::string> raceOver3;
	raceOver3.push_back("redguard");
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("guardiarossa", raceOver3));
	std::vector<std::string> raceOver4;
	raceOver4.push_back("orc");
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("orco", raceOver4));
	std::vector<std::string> raceOver5;
	raceOver5.push_back("golden saint");
	raceOver5.push_back("goldensaint"); //The race editor id.
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("santo d'oro", raceOver5));
	std::vector<std::string> raceOver6;
	raceOver6.push_back("dark seducer");
	raceOver6.push_back("darkseducer"); //The race editor id.
	raceOverrides.insert(std::pair<std::string, std::vector<std::string>>("oscura seduttr", raceOver));
//TODO wood elf and dark elf are not included right now as for vanilla  they use High Elf folder.
//Why they thinked this was a good idea still eludes me.
}

//Will need to test for overhead.
const char* getOverrideFor(char* RaceToOverride){
	if (RaceToOverride == nullptr) return nullptr;
	stringToLower(RaceToOverride);
	auto& vec_ref = raceOverrides.find(RaceToOverride);
	if (vec_ref == raceOverrides.end()) return nullptr;
	auto& val = vec_ref->second;
	for each (std::string over in val){
		if (FileExists(over.c_str())) return over.c_str();
	}
	return nullptr;
}
	