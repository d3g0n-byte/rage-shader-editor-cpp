#include "ShaderModel3.h"
#include "utils.h"

void ShaderModel3::CommentsLayout::Add(unsigned int _size) {
	size.push_back(_size);
}

void ShaderModel3::CommentsLayout::Add(std::string _str) {
	size.push_back(_str.length() + 1);
}

// создаем раскладку
void ShaderModel3::CommentsLayout::CreateLayout(unsigned startPos) {
	// создаеться по принципу записи от большого к маленькому.\
	Этот способ я взял из раскладки IV и MC:LA, но каждый объект не будет нового рядка
}
//void ShaderModel3::Shader::BuildComments(std::vector<ShaderVariable> *variables) {
//	
//
//}