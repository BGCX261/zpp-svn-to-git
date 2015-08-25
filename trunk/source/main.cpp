/*
This file is part of the ZScript Preprocessor library project.
Copyright(C) 2011 - 2012 Gleeok.
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

Neither the name of the copyright holders nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <Commdlg.h>
#include <iostream>
#include <fstream>

#include "Preprocessor.h"
#include "PostProcessor.h"


void SeperatePathFromName( const std::string& original_filename, std::string& path, std::string& name )
{
	uint pos = original_filename.find_last_of( "\\/");

	if( pos != std::string::npos )
	{
		path = original_filename.substr( 0, pos + 1 );
		name = original_filename.substr( pos + 1, std::string::npos );
	}
}


void SaveDebugFile( const std::string& path, const std::string& filename, const std::string& debug_code )
{
	std::ofstream f;
	std::string debugfilename = path + filename;
	//std::string debugfilename = ((std::string("__") + original_filename) );
	f.open( debugfilename.c_str() );
	if( f.is_open() )
	{
		f << debug_code;
		f.close();
	}
	else printf( "debug error: cannot open %s.", debugfilename.c_str() );
}


bool GetFilePathNative(std::string &fn)
{
	std::string ret;

	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER;

	char file[MAX_PATH] = "";

	ofn.lpstrFilter = "Script Files (z,zh,h,hpp,c,cpp,cs)\0*.z;*.zh;*.h;*.hpp;*.c;*.cpp;*cs\0All files\0*.*\0\0";
	ofn.lpstrFile = file;

	if( ::GetOpenFileName( &ofn ) )
	{
		fn = file;
		return true;
	}

	return false;
}



int main( int argc, char *argv[] )
{
	std::string tmp;
	std::string code;
	std::string filename = argv[ argc - 1 ];

	if( argc < 2 )
	{
		std::string openfilename;
		if( !GetFilePathNative( openfilename ) )
		{
			std::cout << "No file selected. Cannot complete compilation process.\n";
			std::cout << "Press any key to exit...\n";
			std::cin >> tmp;
			return 0;
		}
		else
		{
			filename = openfilename;
		}
	}
	else
	{
		filename = argv[ argc - 1 ];
	}

	std::string path, name;
	SeperatePathFromName( filename, path, name );

	//just an empty list for now.
	std::vector<std::string> empty_arg_list;

	std::cout << "Processing " << filename;
	try
	{
		//If this fails an exception will be thrown.
		int ret = CppPreprocessor::ProcessFile( empty_arg_list, filename, code );
		if( ret != 0 )
		{
			std::cout << "There were errors. Build Failed. \n";
			std::cin >> tmp;
			return ret;
		}
	}
	catch(std::exception e)
	{
		std::cout << "The following exception has been raised:\n";
		std::cout << "An internal pre-processing error occured: (" << e.what() << ")\n";
		std::cin >> tmp;
		return -1;
	}
	catch(...)
	{
		std::cout << "An unknown possibly fatal exception has been caught: terminating...\n";
		std::cin >> tmp;
		return -1;
	}

	if( code.empty() )
	{
		std::cout << "The returned code is empty. There is nothing to save.\n";
		std::cin >> tmp;
		return 0;
	}

#ifdef _DEBUG
	std::string debug_code_copy(code);
#endif

	Postprocessor::_map_type scripts;
	Postprocessor::Process( filename, code, &scripts );

	if( path.empty() )
	{
		std::cout << "internal error: invalid path. Output could not be completed.\n";
		std::cin >> tmp;
		return 1;
	}

#ifdef _DEBUG
	std::string debug_name("__preprocessed_");
	debug_name.append(name);
	std::cout << "*DEBUG BUILD MESSAGE* Saving " << debug_name << "\n";
	SaveDebugFile( path, debug_name, debug_code_copy );
#endif

	std::string _n = name.substr( 0, name.find_first_of('.') );
	std::string foldername = std::string("_") + _n + "_";
	std::string fullpath = path + foldername;

	//win32 bullshit
	//WCHAR buf[ MAX_PATH ];
	char buf[ MAX_PATH ];
	memset( buf, 0, sizeof(char) * MAX_PATH );
	for( uint i(0); i < fullpath.size(); ++i )
		buf[i] = fullpath[i];

	if( ::CreateDirectory( buf, NULL ) == 0 )
	{
		//already exists
	}

	foldername.push_back('/');
	fullpath.push_back('/');

	std::vector<std::string> script_filenames;

	//finally, save the processed files in the folder.
	for( Postprocessor::_map_type_iter it = scripts.begin(); it != scripts.end(); ++ it )
	{
		script_filenames.push_back( (std::string("_") + (*it).first) );

		std::cout << "Saving " << script_filenames.back() << "\n";
		SaveDebugFile( fullpath, script_filenames.back(), (*it).second.code );
	}

	//last, create the include file.
	std::string include_code = 
		"// (ZPP)Zscript Preprocessor ver. 1.0 auto-generated include file. *DO NOT HAND MODIFY*\n";

	std::cout << "Saving auto-generated include file.\n";
	for( std::vector<std::string> ::iterator it = script_filenames.begin(); it != script_filenames.end(); ++ it )
	{
		include_code += "import \"";
		include_code += foldername + *it;
		include_code += "\"\n";
	}

	SaveDebugFile( path, std::string("_") + name, include_code );


	std::cout << "success.\n";

#ifdef _DEBUG
	//let's take a look at any warning or error messages that were output.
	std::cout << "*DEBUG BUILD MESSAGE* Press any key to exit.\n";
	std::cin >> tmp;
#endif

	return 0;
}


