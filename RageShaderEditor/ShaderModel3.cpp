#include "ShaderModel3.h"
#include "utils.h"

void ShaderModel3::CommentsLayout::Add(unsigned int _size) {
	size.push_back(_size);
}

void ShaderModel3::CommentsLayout::Add(std::string _str) {
	size.push_back(_str.length() + 1);
}

// ������� ���������
void ShaderModel3::CommentsLayout::CreateLayout(unsigned startPos) {
	// ���������� �� �������� ������ �� �������� � ����������.\
	���� ������ � ���� �� ��������� IV � MC:LA, �� ������ ������ �� ����� ������ �����
}
//void ShaderModel3::Shader::BuildComments(std::vector<ShaderVariable> *variables) {
//	
//
//}