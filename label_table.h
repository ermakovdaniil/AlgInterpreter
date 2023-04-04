/*
  label_table.h
  ��������� ������� ����� ���������
*/

#pragma once

#include <map>
#include "lexical_analizer.h"

class LabelTable {
    class LabelTableImpl* pImpl; // ������������ ��������� �� ������� �����
public:
    LabelTable(); // �����������
    ~LabelTable(); // ����������
    void           AddLabel(unsigned int, Parser::Holder); // ���������� ����� �������� � �������
    Parser::Holder GetHolder(unsigned int); // ��������� ����� �������� �� �������
};

extern LabelTable LT;