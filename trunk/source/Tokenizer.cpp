/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#include "Tokenizer.h"
#include <sstream>


const std::string Tokenizer::default_delimiters(" \t\n\r");


Tokenizer::Tokenizer()
	:
	_index(0),
	_string(),
	_delimiters(default_delimiters)
{}


Tokenizer::Tokenizer( const std::string& str )
	:
	_index(0),
	_string(str),
	_delimiters(default_delimiters)
{}


Tokenizer::Tokenizer( const std::string& str, const std::string& delimiters )
	:
	_index(0),
	_string(str),
	_delimiters(delimiters)
{}


Tokenizer::Tokenizer( const Tokenizer& t )
	:
	_index(t._index),
	_string(t._string),
	_delimiters(t._delimiters)
{}


Tokenizer::size_type Tokenizer::GetPosition() const
	{
		return _index;
	}


void Tokenizer::SetString( const std::string& str )
	{
		_index = 0;
		_string = str;
	}


void Tokenizer::SetString( const char* cstr )
	{
		_index = 0;
		_string = cstr;
	}


void Tokenizer::SetDelimiters( const std::string& delimiters )
	{
		_delimiters = delimiters;
	}


void Tokenizer::SetDelimiters( const char* delimiters )
	{
		_delimiters = delimiters;
	}


void Tokenizer::SetPosition( size_type pos )
	{
		if( pos < _string.size() )
			_index = pos;
	}


bool Tokenizer::GetNextToken( std::string& token ) 
	{
		return GetToken( _string, _delimiters, _index, token );
	}


bool Tokenizer::GetToken( const std::string& str, const std::string& delimiters, size_type& index, std::string& token ) 
	{
		size_type count;
		size_type begin_index;
		size_type end_index;

		begin_index = str.find_first_not_of( delimiters, index );

		if( begin_index == std::string::npos ) 
			return false;

		if( str[ begin_index ] == '"' ) //string can contain delimiters
		{
			end_index = str.find_first_of( '"', begin_index + 1 );
		}
		else
		{
			end_index = str.find_first_of( delimiters, begin_index + 1 );
		}

		if( end_index == std::string::npos )
		{
			index = str.size();
			count = index - begin_index;
		}
		else
		{
			index = end_index;
			count = end_index - begin_index;
		}

		token.assign( &str[ begin_index ], count );

		return true;
	}


void Tokenizer::GetTokens( const std::string& str, const std::string& delimiters, std::vector<std::string> &tokens )
{
	std::string token;
	const size_type size = str.size();

	for( size_type index(0); index < size; )
	{
		if( !GetToken( str, delimiters, index, token ) )
			break;

		tokens.push_back( token );
	}
}


int Tokenizer::ToInt( const char* str )
	{
		return atoi( str );
	}


int Tokenizer::ToInt( const std::string& str )
	{
		return ToInt( str.c_str() );
	}

int Tokenizer::ToInt( const std::string& str, size_type offset )
	{
		return atoi( &str[offset] );
	}


float Tokenizer::ToFloat( const char* str )
	{
		return (float)atof( str );
	}


float Tokenizer::ToFloat( const std::string& str )
	{
		return ToFloat( str.c_str() );
	}


float Tokenizer::ToFloat( const std::string& str, size_type offset )
	{
		return (float)atof( &str[offset] );
	}


template <class T>
std::string Tokenizer::ToString( T value )
	{
		std::ostringstream stringstream;
		stringstream << value;

		return std::string( stringstream.str() );
	}


template<class T> 
void Tokenizer::ToString( std::string& outstr, T value )
	{
		std::ostringstream stringstream;
		stringstream << value;

		outstr.assign( stringstream.str() );
	}






