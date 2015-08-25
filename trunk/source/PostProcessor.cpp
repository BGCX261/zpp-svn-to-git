/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#include "PostProcessor.h"
#include "Tokenizer.h"



Postprocessor::Postprocessor()
	{
	}


Postprocessor::~Postprocessor()
	{
	}


bool Postprocessor::GetNextLine( std::string& code, uint& index, int& line, std::string& _currentLine )
	{
		_currentLine.clear();
		for( const uint size = code.size(); index < size; )
		{
			const char c = code[ index++ ];

			_currentLine.push_back( c );
			if( c == '\n' )
			{
				++line;
				return true;
			}
		}

		return false; //eof
	}


void Postprocessor::RemovePath( std::string& filepath )
	{
		for( int i(int(filepath.size() - 1)); i >= 0 ; --i )
		{
			const char c = filepath[ i ];

			if( c == '"' )
			{
				filepath.erase( i, 1 );
			}
			else if( c == '/' )
			{
				filepath.erase( 0, i + 1 );
				break;
			}
			else if( c == '\\' )
			{
				filepath.erase( 0, i + 1 );
				break;
			}
		}
	}


void Postprocessor::RemoveLine( std::string& code, uint index, bool remove_line_ending )
	{
		uint starting_index = index;
		uint amount_to_remove = 0;

		while( index < code.size() && code[ index ] != '\n')
		{
			++amount_to_remove;
			++index;
		}

		if( remove_line_ending && index < code.size() && code[ index ] == '\n' )
			++amount_to_remove;

		code.erase( starting_index, amount_to_remove );
	}


void Postprocessor::ProcessLineDirective( std::string& code, uint& index, std::string& filename, int& line_number )
	{
		filename.clear();
		std::string token;

		// get the line number
		//GetNextToken( code, index, token );
		Tokenizer::GetToken( code, Tokenizer::default_delimiters, index, token );

		line_number = (uint) atoi( token.c_str() );

		// get the filename
		//GetNextToken( code, index, filename );
		Tokenizer::GetToken( code, Tokenizer::default_delimiters, index, filename );
		RemovePath( filename );
	}


void Postprocessor::Process( const std::string& code_filename, std::string& code, _map_type *out_script_sections )
	{
		_map_type &scripts = *out_script_sections;

		std::string _fname( code_filename );
		RemovePath( _fname );

		scripts.clear();
		scripts.insert( _pair_type( _fname, Script() ) );

		Script *current_script			= &scripts[ _fname ];
		bool eof						= false;
		int line						= 1;
		uint c							= 0;
		uint cur_section_start_index	= 0;
		int cur_section_line_start		= 0;

		std::string token;
		std::string filename;
		std::string currentline;


		// Pass 3 - Clean up the preprocessed file and re-format our line-endings
		while( !eof )
		{
			// process the directive, if any
			if( c < code.size() && code[ c ] == '#' )
			{
				uint temp_index = c + 1;
				//GetNextToken( code, temp_index, token );
				Tokenizer::GetToken( code, Tokenizer::default_delimiters, temp_index, token );

				// get info and append to scripts
				if( token == "line" )
				{
					int line_number = 0;

					//Tokenizer::GetToken( code, Tokenizer::default_delimiters, temp_index, filename );
					ProcessLineDirective( code, temp_index, filename, line_number );
					RemoveLine( code, c, (line_number == 1 ? true : false) );

					current_script = &scripts[ filename ];

					int line_diff = line_number - 1 - current_script->line;
					if( line_diff > 0 )
					{
						current_script->code.append( line_diff, '\n' );
						current_script->line += line_diff;
					}
				}
				else if( token == "include" )
				{
					//delete the current line
					RemoveLine( code, c );
				}
				else
				{
					printf( "unknown token!" );
				}

			}

			// we can be assured any '#' info has already been resolved
			else if( GetNextLine( code, c, line, currentline ) )
			{
				current_script->code += currentline;
				current_script->line++;
			}
			else eof = true;

		}

		// and last but not least, make sure files end with a new line.
		for( _map_type_iter it = scripts.begin(); it != scripts.end(); ++ it )
			(*it).second.code.push_back('\n');

	}
