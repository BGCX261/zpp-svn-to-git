/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#pragma once


#include <string>
#include <vector>
#include "PostProcessor.h"
#include "BasicTimer.h"


class CppPreprocessor
{
public:
	CppPreprocessor();
	~CppPreprocessor();

	static int ProcessFile
		(
			int argc,
			char *argv[],
			std::string& out_file,
			BasicTimer* timer = 0
		);

	static int ProcessFile
		(
			const std::vector<std::string>& _Args,
			const std::string& filename,
			std::string& out_file,
			BasicTimer* timer = 0
		);


};

