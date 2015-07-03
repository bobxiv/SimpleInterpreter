#include "stdafx.h"

#pragma once

#include "Lexer.h"

namespace Lexical_Analysis
	{

	TokenStream* Lexer::Analize() throw( UnExpextedSimbol )
	{
	TokenStream* pTS = new TokenStream();

	while( peek != InputStream.end() )
		{

		#pragma region INTEGER
			{
		std::string::iterator beg = peek;
		if( *peek == 'I' )
			{
			++peek;
			if( *peek == 'N' )
				{
				++peek;
				if( *peek == 'T' )
					{
					++peek;
					if( *peek == 'E' )
						{
						++peek;
						if( *peek == 'G' )
							{
							++peek;
							if( *peek == 'E' )
								{
								++peek;
								if( *peek == 'R' )
									{
									pTS->PutToken(Token(TOKENTAG::INTEGER,INTEGERpos));						
									++peek;
									continue;
									}else
										peek=beg;
								}else
									peek=beg;
							}else
								peek=beg;
						}else
							peek=beg;
					}else 
						peek=beg;
				}else
					peek=beg;
			}
			}
		#pragma endregion

		#pragma region WHILE
			{
		std::string::iterator beg = peek;
		if( *peek == 'W' )
			{
			++peek;
			if( *peek == 'H' )
				{
				++peek;
				if( *peek == 'I' )
					{
					++peek;
					if( *peek == 'L' )
						{
						++peek;
						if( *peek == 'E' )
							{
							pTS->PutToken(Token(TOKENTAG::WHILE,WHILEpos));						
							++peek;
							continue;
							}else
								peek=beg;
						}else
							peek=beg;
					}else
						peek=beg;
				}else
					peek=beg;
			}else 
				peek=beg;
			}
		#pragma endregion

		#pragma region NUM
			//the numbers
		if( isdigit(*peek) )
			{
			int tmp_val =0;
			do
				{
					tmp_val = tmp_val * 10 + (*peek - '0');
				}while( ++peek != InputStream.end() && isdigit(*peek) );
				
				char lexeme[255]; itoa(tmp_val,lexeme,10);//temporary buffer
				std::pair<stdext::hash_map<std::string,double>::iterator,bool> simbolpos = 
					Enviroment.back().insert(std::pair<std::string,double>(lexeme,tmp_val));
				pTS->PutToken( Token(TOKENTAG::NUM,simbolpos.first) );
			}

		if( peek == InputStream.end() ) break;
		#pragma endregion This part computes the numbers and get the NUM token 

		#pragma region ID and FUN 
		std::string tmp_id_or_fun;
			//the fun
		if( isalpha(*peek) )
			{
				do
				{	
					tmp_id_or_fun.insert(tmp_id_or_fun.end(),*peek);

				}while( ++peek != InputStream.end() && isalpha(*peek) );
				
				if( peek != InputStream.end() && *peek == '(' )//it is really a fun?
					{
					stdext::hash_map<std::string,double>::iterator simbolpos = FindEnviromentUp(tmp_id_or_fun);
					if( simbolpos == Enviroment.back().end() )
						{
						std::pair<stdext::hash_map<std::string,double>::iterator,bool> insertpos;
						insertpos = Enviroment.back().insert(std::pair<std::string,double>(tmp_id_or_fun,0));
						simbolpos = insertpos.first;
						}

					//if it was already there then we don't touch the simboltable
					pTS->PutToken( Token(TOKENTAG::FUN,simbolpos) );
					//the '(' is nesesary for FUN, but not part of the expression
					continue;
					}

				//the ids
			if( peek != InputStream.end() && isdigit(*peek) )
				{
					do
					{	
						tmp_id_or_fun.insert(tmp_id_or_fun.end(),*peek);

					}while( ++peek != InputStream.end() && isdigit(*peek) );
				}

				stdext::hash_map<std::string,double>::iterator simbolpos = FindEnviromentUp(tmp_id_or_fun);
				if( simbolpos == Enviroment.back().end() )//then it was't in the simboltable let's add it
					{
					std::pair<stdext::hash_map<std::string,double>::iterator,bool> insertpos;
					insertpos = Enviroment.back().insert(std::pair<std::string,double>(tmp_id_or_fun,0));
					simbolpos = insertpos.first;
					}
				
				//if it was already there then we don't touch the simboltable
				pTS->PutToken( Token(TOKENTAG::ID,simbolpos) );
					
			}
					
			//It could be that the ids and numbers get analysed and then it ends, so i need to 
			//take care of this posibility
		if( peek == InputStream.end() ) break;
		#pragma endregion This part computes the ids and funs and get the ID or FUN token

		#pragma region OPERATORS & OTHERS
			//the operators
		if(*peek == '+') {pTS->PutToken( Token(TOKENTAG::PLUS, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == '-') {pTS->PutToken( Token(TOKENTAG::MINUS, Enviroment.begin()->end()) ); ++peek; continue; }		
		if(*peek == '*') {pTS->PutToken( Token(TOKENTAG::PROD, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == '/') {pTS->PutToken( Token(TOKENTAG::DIV, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == '^') {pTS->PutToken( Token(TOKENTAG::POT, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == '(') {pTS->PutToken( Token(TOKENTAG::LPAR, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == ')') {pTS->PutToken( Token(TOKENTAG::RPAR, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == '=') {pTS->PutToken( Token(TOKENTAG::EQUAL, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == ';') {pTS->PutToken( Token(TOKENTAG::ENDCMD, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == '{') {pTS->PutToken( Token(TOKENTAG::BLOCKBEG, Enviroment.begin()->end()) ); ++peek; continue; }
		if(*peek == '}') {pTS->PutToken( Token(TOKENTAG::BLOCKEND, Enviroment.begin()->end()) ); ++peek; continue; }
		#pragma endregion This part computes the arithmetics operators and get the corresponding token

		#pragma region WITHESPACE AND LF
			//the spaces and end of line
		if(*peek == ' ') {++peek; continue; }
		if(*peek == '\n') {++peek; continue; }
		#pragma endregion This part filter the unimportant characters

			//If it does't recognize a token then the InputStream had an error
		throw UnExpextedSimbol();

		}

	return pTS;
	}

	}