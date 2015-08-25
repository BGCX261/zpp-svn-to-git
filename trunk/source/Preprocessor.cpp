/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#include "Preprocessor.h"

#include <mcpp.h>
#include <direct.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <map>
#include <fstream>

#include "GarbageCollect.h"



#ifdef _MSC_VER
	#pragma warning ( disable : 4996 )
#endif


CppPreprocessor::CppPreprocessor()
	{
	}


CppPreprocessor::~CppPreprocessor()
	{
	}



int CppPreprocessor::ProcessFile
		(
		int argc,
		char *argv[],
		std::string& out_file,
		BasicTimer* timer
		)
	{
		std::string filename;
		std::vector<std::string> args;

		for( int i(0); i < argc; ++i )
		{
			if( i == argc - 1 )
			{
				filename = argv[ argc - 1 ];
				break;;
			}
			args.push_back( argv[ i ] );
		}

		return ProcessFile( args, filename, out_file, timer );
	}


int CppPreprocessor::ProcessFile
		(
		const std::vector<std::string>& _Args,
		const std::string& filename,
		std::string& out_file,
		BasicTimer* timer
		)
	{

		GarbageCollector gc;

		std::vector<std::string> args( _Args );
		if( args.empty() )
		{
			// all that is required is a valid path/file, so just set the default flags
			char buf[ 260 ];
			args.push_back( _getcwd( buf, 260 ) );
			args.push_back( "-W17" );
			args.push_back( "-+" );
			args.push_back( "-k" );
			args.push_back( "-D__DEFINED__" );
		}

		int		num_args		= args.size() + 1;
		int		return_value	= 0;
		char*	error_string	= 0;
		char**	arg				= gc[ new char*[ num_args ] ];

		out_file.clear();


		//allocate memory fot null-terminated strings
		for( uint i(0); i < args.size(); ++i )
		{
			arg[ i ] = gc[ new char[ args[ i ].size() + 1 ] ];
			strcpy( arg[ i ], args[ i ].c_str() );
		}
		arg[ args.size() ] = gc[ new char[ filename.size() + 1 ] ];
		strcpy( arg[ args.size() ], filename.c_str() );


		BasicTimer* time = timer;
		if( !timer )
			time = gc( new BasicTimer() );


		time->StartTimer();

		// lets do it
		mcpp::use_mem_buffers( 1 );

		return_value	= mcpp::lib_main( num_args, &arg[0] );
		out_file		= mcpp::get_mem_buffer( OUT );
		error_string	= mcpp::get_mem_buffer( ERR );

		time->StopTimer();


		if( out_file.empty() )
		{
			printf( "fatal error : preprocessor failure,\n" );
		}

		if( error_string )
		{
			std::ofstream f( "error.log" );
			if( f.is_open() )
			{
				f << error_string;
				f.close();
			}
			printf( "%s", error_string );
		}

		printf( "Preprocessor finished with status: %d. Elapsed time: %d minutes, %.2f seconds.\n",
			return_value, time->GetMinutes(), time->GetSeconds() );


		return return_value;
	}





#ifdef _MSC_VER
	#pragma warning ( default : 4996 )
#endif


