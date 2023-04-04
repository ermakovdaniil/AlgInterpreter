/*
  name_table.h
  ��������� ������� ��� ��������������
*/

#pragma once

#include <string>
#include <map>
#include "interface.h"

class NameTable {
    std::map<std::string, Command*> commands; // �������
    std::map<std::string, float>    variables; // ����������
public:
    ~NameTable(); // ����������
    // ����������� ������� ����� � ������� ����
    void RegisterCommand(std::string const& name, Command*);
    // ��������� �������, ���� ��� ��������
    bool ProcessCommand(Parser&);

    void  SetVariable(std::string const& name, float value); // ��������� ������� � � �������� � ������� ��� �������������� 
    float GetVariable(std::string const& name); // ��������� �������� �� ������� ��� �������������� 
};

extern NameTable NT;