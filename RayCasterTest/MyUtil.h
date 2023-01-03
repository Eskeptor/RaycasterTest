#pragma once
#include <iostream>

/**
@brief			Utility Class
@author			Eskeptor
@date			2023-01-02
@version		0.0.1
*/
class CMyUtility
{
public:
	static void ClearCmd()
	{
#if defined(_WIN32) || defined(_WIN64)
		//system("cls");
#elif defined(__linux__) || defined(__linux) || defined(linux)
		std::cout << "\033[2J\033[1;1H";
#else

#endif
	}
};