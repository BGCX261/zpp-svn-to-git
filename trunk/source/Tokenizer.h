/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#pragma once


#include <string>
#include <vector>



class Tokenizer 
{
public:
	static const std::string default_delimiters;
	typedef unsigned int size_type;

	Tokenizer();
	Tokenizer( const std::string& str );
	Tokenizer( const std::string& str, const std::string& delimiters );
	Tokenizer( const Tokenizer& t );
	~Tokenizer()
	{}

	size_type GetPosition() const;
	bool GetNextToken( std::string& token );
	void SetString( const char* cstr );
	void SetString( const std::string& str );
	void SetDelimiters( const char* delimiters );
	void SetDelimiters( const std::string& delimiters );
	void SetPosition( size_type pos );

	static bool GetToken( const std::string& str, const std::string& delimiters, size_type& index, std::string& token );
	static void GetTokens( const std::string& str, const std::string& delimiters, std::vector<std::string> &tokens );


	template<class T> static void ToString( std::string& outstr, T value );
	template<class T> static std::string ToString( T value );

	static int ToInt( const char* str );
	static int ToInt( const std::string& str );
	static int ToInt( const std::string& str, size_type offset );
	static float ToFloat( const char* str );
	static float ToFloat( const std::string& str );
	static float ToFloat( const std::string& str, size_type offset );

protected:
	size_type _index;
	std::string _string;
	std::string _delimiters;
};



