#pragma once
#include <vector>;
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include "fxc_structures.h"
#include "utils.h"
#include "xml_writter.h"
#include "d3dx9.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "d3dx9x.h"

namespace IV {
	void ToXML(std::vector<unsigned char>* fxc_data, std::string fileName, std::string folder);
	void ToFXC(std::string filePath);
}