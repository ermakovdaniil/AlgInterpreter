/*
  name_table.cpp
  ������� ��� ��������������.
  ������ �������� �� �������� ��� ������ � ����������.
*/

#include "stdafx.h"
#include "name_table.h"

NameTable NT;

NameTable::~NameTable() // ����������
{
    // ������� ��� "�������" ��������� ������
    std::map<std::string, Command*>::const_iterator it(commands.begin());
    for (; it != commands.end(); ++it)
        delete (it->second);
}

void NameTable::RegisterCommand(std::string const& name, Command* cmd) { // ����������� ������� ����� � ������� ���
    commands[name] = cmd; // ���������� ������� � ���������
}

bool NameTable::ProcessCommand(Parser& parser) // ���������� �������
{
    std::map<std::string, Command*>::const_iterator it(commands.begin()); // ��������� map, �������� �������
    for (; it != commands.end(); ++it) { // ���� ���������� ������
        if (it->second->Process(parser)) // ���� ������� ���������
            return true;
    }

    return false;
}

void  NameTable::SetVariable(std::string const& name, float value) // ��������� � ������� ��� �������������� ������� � � ��������
{
    variables[name] = value; // ���������� ���������� � ���������
}

float NameTable::GetVariable(std::string const& name) // ��������� �������� �� ������� ��� �������������� 
{
    return variables[name]; // ����������� ����������
}