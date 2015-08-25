/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#pragma once

#include "BasicTimer.h"

#include <string>
#include <map>


typedef unsigned int uint;


/*
	Preprocessor - final pass: 
	Remove the unwanted cpp processor 
	information, incuded files, line information, 
	and other directives so it can be compiled
	sucessfully.
*/

class Postprocessor
{
public:

	class Script
	{
	public:
		std::string code;
		int line;

		Script() : code(), line(1) {}
		Script( const Script& s ) : code(s.code), line(s.line) {}
		~Script()
		{}

		Script &operator = ( const Script& s )
		{
			code	= s.code;
			line	= s.line;
			return *this;
		}
	};

	typedef std::pair<std::string, Script> _pair_type;
	typedef std::map<std::string, Script> _map_type;
	typedef _map_type ::iterator _map_type_iter;

	Postprocessor();
	~Postprocessor();

	static void Process( const std::string& code_filename, std::string& code, _map_type *out_script_sections );
	static void ProcessLineDirective( std::string& code, uint& index, std::string& filename, int& line_number );
	static void RemoveLine( std::string& code, uint index, bool remove_line_ending = false );
	static void RemovePath( std::string& filepath );
	static bool GetNextLine( std::string& code, uint& index, int& line, std::string& _currentLine );

};


