#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

class XmlWritter {
	int currentZPos;
	std::stringstream xml;
	char tabSymbol;
	bool lastGroupIsNull;
	std::vector<std::string> groupBuffer;

	void AppendTabs() {
		for (int i = 0; i < currentZPos; i++)
			xml << tabSymbol;
	}
public:
	void WriteValue(std::string child, std::string value) {
		AppendTabs();
		xml << '<' << child << '>' << value << "</" << child << ">\n";
	}
	void WriteValue(std::string child, int value) {
		AppendTabs();
		xml << '<' << child << '>' << value << "</" << child << ">\n";
	}
	void WriteValue(std::string child, float value) {
		AppendTabs();
		xml << '<' << child << '>' << value << "</" << child << ">\n";
	}
	void WriteValue(std::string child, unsigned value) {
		AppendTabs();
		xml << '<' << child << '>' << value << "</" << child << ">\n";
	}
	void WriteGroupStart(std::string child, int count = -1) {
		AppendTabs();
		if (count != 0) {
			xml << '<' << child << ">\n";
			groupBuffer[currentZPos] = child;
			currentZPos++;
		}
		else {
			xml << '<' << child << " />\n";
			lastGroupIsNull = true;
		}
	}
	void WriteGroupEnd() {
		if (lastGroupIsNull) {
			lastGroupIsNull = 0;
			return;
		}
		currentZPos--;
		AppendTabs();
		xml << "</" << groupBuffer[currentZPos] << ">\n";
	}

	void WriteEnd() {
		while (currentZPos > 0) {
			WriteGroupEnd();
		}
	}

	void Save(std::string path) {
		WriteEnd();
		std::ofstream out(path);
		out << xml.str();
		out.close();
	}


	XmlWritter() {
		currentZPos = 0;
		xml.setf(std::ios::fixed);
		xml.precision(8);
		xml << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
		groupBuffer.resize(255);
		lastGroupIsNull = false;
		tabSymbol = '\t';
	}
};