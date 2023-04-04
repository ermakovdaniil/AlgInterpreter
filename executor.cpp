/*
  Executor.cpp
  ������, ���������� �� ������������� � ���������� ���������.
*/

#include "stdafx.h"
#include "executor.h"
#include "name_table.h"
#include "label_table.h"
#include "lexical_analizer.h"
#include "commands.h"

void register_commands() { // ������ ��� ������ � ������� ��� ��������������
    NT.RegisterCommand("FOR", new CmdFor);
    NT.RegisterCommand("GOTO", new CmdGoto);
    NT.RegisterCommand("IF", new CmdIf);
    NT.RegisterCommand("INPUT", new CmdInput);
    NT.RegisterCommand("LET", new CmdExpression);
    NT.RegisterCommand("NEXT", new CmdNext);
    NT.RegisterCommand("PRINT", new CmdPrint);
}

// ������� ������� ������� false ��� ����������
bool execute_command(Parser& parser) {
    Lexem lex = parser.get_last(); // ��������� �������

    if (lex.type == LT_End) // ���� ������� �� �������� ������ ����� 
        return false;

    switch (lex.type) { // ��������� � ����������� �� ���� �������
    case LT_Identifier: // ��� �������������
    {
        if (!NT.ProcessCommand(parser))
            throw "Can't process command"; // ����� ���������� � ���, ��� ���������� ��������� ������� 
    }
    break;
    case LT_EOL: // ��� ����� ������
        parser.get_lexem(); // ��������� ��������� ������� 
        break;
    case LT_Number: // ��� �����
        parser.get_lexem(); // ��������� ��������� ������� 
        break;
    default:
        throw "Bad syntax";
    }
    return true;
}

void scan_for_labels(Parser& parser) // ����� ����� ��� ��������
{
    bool prev_lexem_is_EOL(true); // ����������, �������� �� ������ ������
    while (parser.get_lexem().type != LT_End) { // ���� ��� ������� �� ����� �����
        if (prev_lexem_is_EOL && parser.get_last().type == LT_Number) { // ���� ������� ������� �� ����� ������ � ��� ������� ������� �����
            unsigned int label(static_cast<unsigned int>(parser.get_last().value)); // ���������� �������� ����� � ���� uint
            parser.get_lexem(); // ��������� ��������� ������� 
            LT.AddLabel(label, parser.Hold()); // ���������� ����� �������� � ������� ����� ��������
        }

        prev_lexem_is_EOL = (parser.get_last().type == LT_EOL); // �������� ������� �� �������������� � ���� "����� ������"
    }
}

void execute_script(std::istream& in) { // ���������� �������
    register_commands(); // ������ ��� ������ � ������� ��� ��������������
    Parser parser(in);

    scan_for_labels(parser); // ����� ����� ��� ��������
    parser.Reset(); // ������� ������ 


    parser.get_lexem(); // ��������� ��������� ������� 
    while (execute_command(parser)) // ���������� ������� �� ��� ����, ���� ������� �� ����� false 
        ;

    std::cout << "OK" << std::endl; // ����� ���������� � ���, ��� ���������� ���������  
}