// -No Copyright- 2010 Yulian "d3g0n" Shvab
// This project donated to the public domain

#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <chrono>
#include <thread>
#include <vector>
#include "iv_fxc.h"
#include "utils.h"

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)

void main(int argc, char** argv)
{
	
	//std::cout << std::stof("0.01") << std::endl;
	//setlocale(LC_ALL, "Ru_UA");
	//float g = std::stof("0.01");
	int arr[]{ 'H','e','l','l','o',' ','W','o','r','l','d','!','\n' };
	for (char a : arr) {
		std::cout << a;
	}
	//auto startTime = std::chrono::milliseconds::count;
	//std::cout << startTime<<"\n";

	//time_t t = time(NULL);
	//struct tm tm = *localtime(&t);
	//tm.
	auto millisecInStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//auto sec_since_epoch = duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	//std::cout << "milliseconds since epoch: " << millisec_since_epoch << std::endl;

	if (argc < 2) {
		return;
	}

	std::string ext = GetExtension(argv[1]);
	if (ext == "fxc") {
		FILE* fxc;
		fxc = fopen(argv[1], "rb");
		if (!fxc) {
			printf("file on path %s not found\n", argv[1]);
			system("pause");
			return;
		}

		std::vector<unsigned char> fxc_data;

		fseek(fxc, 0, SEEK_END);
		size_t fileLength = ftell(fxc);
		fseek(fxc, 0, SEEK_SET);

		unsigned magic;
		fread(&magic, 0x4, 1, fxc);
		if (magic != 1635280754)
			return;

		fseek(fxc, 0, SEEK_SET);
		fxc_data.resize(fileLength);
		fread(&fxc_data[0], 0x1, fileLength, fxc);
		IV::ToXML(&fxc_data, GetFileNameWithoutExtension(argv[1]), GetFolderName(argv[1]));
	}
	else if (ext == "xml") {
		IV::ToFXC(argv[1]);
	}
		auto millisecInEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned long long timeUsed = millisecInEnd - millisecInStart;
		printf("elapsed %llu.%llu second\n", timeUsed / 1000, timeUsed % 1000);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
