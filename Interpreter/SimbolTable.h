#pragma once

#include <hash_map>
#include <string>
#include <vector>

//The All Migthy SimbolTable!!!
typedef stdext::hash_map<std::string,double, stdext::hash_compare<std::string, std::less<std::string> >> SimbolTable;
typedef stdext::hash_map<std::string,double, stdext::hash_compare<std::string, std::less<std::string> >>::iterator
															SimbolIterator;

extern std::vector<SimbolTable> Enviroment;

extern SimbolIterator WHILEpos;
extern SimbolIterator INTEGERpos;

SimbolTable::iterator FindEnviromentUp( std::string );

void Initializing_SimbolTable();
