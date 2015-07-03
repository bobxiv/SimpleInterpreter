// Interpreter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <fstream>

//The debuging flag, in debug it shows:
//1) The lexer status
//#define DEBUG

//BEGIN ----------------------------- Grammar includes ----------------------------------------------

//The Exceptions
#include "GrammarException.h"

//The Lexical Analizer
#include "Lexer.h"

using namespace Lexical_Analysis;

//It have some operators and functions used in main, not important to the translation
#include "Miscelanius.h"

//The SimbolTable
#include "SimbolTable.h"

#include "Parser.h"

#include "ResolveSyntaxTree.h"



//END ------------------------------- Grammar includes ----------------------------------------------

void SerchTheEnd(std::ifstream& In)
	{	
	std::string novo;
	int beg=1;
	int end=0;
	while( In && ((beg-end) == 0) )
		{
		std::getline(In,novo);
		if( novo == "{" )
			++beg;
		if( novo == "}" )
			++end;
		}
	}

void _TokenLisp_Print(Dash_Structures::DBTree<Token>* pDBT, Dash_Structures::DBTree<Token>::iterator it)
	{
	if( it == pDBT->end() )
		{
		std::cout<<". ";
		return;
		}

	std::cout<<"( "<<(*it);

	_TokenLisp_Print(pDBT, it.left_child());

	std::cout<<" ";

	_TokenLisp_Print(pDBT, it.right_child());

	std::cout<<" )";

	return;
	}

void TokenLisp_Print(Dash_Structures::DBTree<Token>* pDBT)
	{
	_TokenLisp_Print(pDBT,pDBT->begin());
	}


int _tmain(int argc, _TCHAR* argv[])
{
	//Initialize_ProductionTable();

	//initializing the predefine constants
	Initializing_SimbolTable();

	std::ifstream In( "input.txt" );

	if( !In.is_open() )
		std::cerr<<"No se puede abrir el archivo!!!\n";

	TokenStream* ptokstrm = new TokenStream();

	while( In )
		{
		//input will be my InputStream
		std::string input;
		std::getline( In, input);
		
		//lets pass the lexer over the input
		Lexer* plex = new Lexer(input);
				
		try
			{
		ptokstrm = plex->Analize();
			}
		catch( UnExpextedSimbol e )
			{
			std::cout<<"\n"<< e.what();
			getch();
			exit(1);
			}

		#pragma region Lexer Status, in Case of debugging
		#ifdef DEBUG
			std::cout<<"\nLEXER STATUS\n~~~~~~~~~~~~";
			std::cout<<"\n-----------------------------------------------------";
			std::cout<<"\n| The TokenStream\n|\n";
			
			Token tmp_token;
			ptokstrm->GoFirst();
			std::cout<<"| ";
			while( !ptokstrm->EndToken() )
				{
				std::cout<<ptokstrm->GetCurrent();
				ptokstrm->NextToken();
				}

			std::cout<<"\n|\n-----------------------------------------------------";
			std::cout<<"\n| The Token Atributes\n|\n";
			ptokstrm->GoFirst();

			std::cout<<"|    Tag\tPosition in Simbol Table\n";
			std::cout<<"|    ~~~\t~~~~~~~~~~~~~~~~~~~~~~~~\n";
			while( !ptokstrm->EndToken() )
				{	
				//I want the index position of GetCurrent().data but it does't let
				//me do iterator arithmetics, so I do this stinky thing
				if( ptokstrm->GetCurrent().data != SimbolTable.end() )
					{
				int tmp_cont=0;
				stdext::hash_map<std::string,double>::iterator tmp_it = SimbolTable.begin();
				while( tmp_it != ptokstrm->GetCurrent().data )
					{
					++tmp_cont;
					++tmp_it;
					}
				std::cout<<"|    "<<ptokstrm->GetCurrent()/*Hete it is the tagm but it is the same to show the token because of the operator <<*/<<
					"\t\t"<< tmp_cont<<'\n';
				ptokstrm->NextToken();
					}else{//If the position of the table is endm so I say NULL
				std::cout<<"|    "<<ptokstrm->GetCurrent()/*Hete it is the tagm but it is the same to show the token because of the operator <<*/<<
					"\t\t"<< "NULL"<<'\n';
				ptokstrm->NextToken();
					}
				}


			std::cout<<"|\n-----------------------------------------------------";
			std::cout<<"\n| The Simbol Table\n|\n";
			//std::copy(SimbolTable.begin(),SimbolTable.end(),std::ostream_iterator<std::pair<std::string,double>>(std::cout));
			std::cout<<"|      Lexeme\t      Value\n";
			std::cout<<"|      ~~~~~~\t      ~~~~~\n";
			stdext::hash_map<std::string,double>::iterator it = SimbolTable.begin();
			int index=0;
			while( it != SimbolTable.end() )
				{
				std::cout<<"| "<<index<<")     "<<*it;
				++index;
				++it;
				}
			std::cout<<"| "<<"NULL)\n";

			std::cout<<"|\n-----------------------------------------------------";
			//let's put it in the begining again
			ptokstrm->GoFirst();
		#endif
		#pragma endregion

		ptokstrm->GoFirst();
		Parser* ppar = new Parser( *ptokstrm );
		Dash_Structures::DBTree<Token>* pSyntaxTree = new Dash_Structures::DBTree<Token>();
		try
			{
			if( input != "{" || input != "}" )
				pSyntaxTree = ppar->Analize();

			#pragma region WHILE tree
			if( pSyntaxTree->begin()->tag == TOKENTAG::WHILE )
				{
				int h = ResolveSyntaxTree(pSyntaxTree,pSyntaxTree->begin().right_child());
				if( h == 0 )
					{
					delete plex;
					std::string nada;
					std::getline( In,nada);
					if( nada == "{" )
						{
						SerchTheEnd(In);
						if( !In )
							return 1;
						else
							{
							std::getline( In,nada);
							continue;
							}
						}
						else
							continue;
						}
					else
						continue;
				}
			#pragma endregion

			#pragma region Definition
			if( pSyntaxTree->begin()->tag == TOKENTAG::INTEGER )
				{
				pSyntaxTree->begin().left_child()->data->second = ResolveSyntaxTree(pSyntaxTree,pSyntaxTree->begin().right_child());
				}
			#pragma endregion

			#pragma region Assignation
			if( pSyntaxTree->begin()->tag == TOKENTAG::EQUAL )
				{
				pSyntaxTree->begin().left_child()->data->second = ResolveSyntaxTree(pSyntaxTree,pSyntaxTree->begin().right_child());				
				}
			#pragma endregion

			}
		catch( BadSintax e )
			{
			std::cout<<'\n'<<input;
			std::cout<<'\n'<< e.what();
			getch();
			exit(2);
			}
			
		#pragma region Parser Status, in Case of debugging
		#ifdef DEBUG
			std::cout<<"\n\nParser STATUS\n~~~~~~~~~~~~~";
			std::cout<<"\n-----------------------------------------------------";
			std::cout<<"\n| The Production Table\n|\n";
			stdext::hash_map<int,std::pair<NonTerminal,Production>>::iterator prodit= ProductionTable.begin();
			while( prodit != ProductionTable.end() )
				{
				std::cout<<"| "<<(*prodit).second.first <<" -> "<<(*prodit).second.second<<'\n';
				++prodit;
				}
			std::cout<<"|\n|\t\t\tThe Terminals have Capital,\n|\t\t\t the NonTerminals not\n";
			std::cout<<"|\n-----------------------------------------------------";
			std::cout<<"\n| The SyntaxTree, in Lisp notation\n|\n";
			std::cout<<"| ";
			TokenLisp_Print(pSyntaxTree);
			std::cout<<"\n|\n-----------------------------------------------------";
		#endif
		#pragma endregion

		delete plex;
		delete ppar;
		delete pSyntaxTree;
		}

	delete ptokstrm;

	In.close();
	

	std::cout<<std::endl;
	system("pause");

	return 0;
}

