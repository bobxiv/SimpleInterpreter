#include "stdafx.h"
#pragma once
#include "SimbolTable.h"

std::vector<SimbolTable> Enviroment;

SimbolIterator WHILEpos;
SimbolIterator INTEGERpos;

SimbolTable::iterator FindEnviromentUp( std::string val )
	{
	std::vector<SimbolTable>::reverse_iterator pos = Enviroment.rbegin();

	while( pos != Enviroment.rend() )
		{
		SimbolTable::iterator res = pos->find( val );
		if( res != pos->end() )
			return res;
		pos++;
		}
	return Enviroment.begin()->end();
	}

void Initializing_SimbolTable()
	{
	SimbolTable MainTable;
	MainTable.insert(std::pair<std::string,double>("pi",3.14));
	MainTable.insert(std::pair<std::string,double>("e",2.73));
	MainTable.insert(std::pair<std::string,double>("true",1));
	MainTable.insert(std::pair<std::string,double>("false",0));

	WHILEpos = MainTable.insert(std::pair<std::string,double>("WHILE",0)).first;
	INTEGERpos = MainTable.insert(std::pair<std::string,double>("INTEGER",0)).first;

	Enviroment.push_back(MainTable);

	//the functions are resolve using the function FuncitonTable
	//and add to the SimbolTable in runtime, but only compute 
	//if they exist in the Functiontable
		
	return;
	}