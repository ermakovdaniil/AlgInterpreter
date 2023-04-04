/*
     Commands.cpp
   ������, ���������� �� ���������� ���������.
   ������ ��������� ������� � ������� evaluate_expression.
   ��� ������������� ���������� � ���������� � ������� float.

   ������ ��������:
       � ������ ������������� ������ ������������� ���������� ����
   const char *
 */

#include "stdafx.h"
#include "commands.h"
#include "calculator.h"
#include "name_table.h"
#include "label_table.h"

 // LET
bool CmdExpression::Process(Parser& parser) { // ������������� ����������
    if (parser.get_last().name != "LET") // ���� ��� ��������� ������� �� �������� "LET"
        return false;

    std::string left(parser.get_lexem().name); // ��������� ����� �������
    if (parser.get_lexem().type != LT_Delimiter || // ���� ������� �� �������� ������������
        parser.get_last().delimiter != '=') // ��� ��������� ������� �� �������� ������������ "="
        throw "Bad assigment"; // ����� ��������� � ���, ��� ����������� �������� ������

    float value = evaluate_expression(parser); // ���������� ���������

    NT.SetVariable(left, value); // ��������� ������� � � �������� � ������� ��� ��������������

    return true;
}

// PRINT
bool CmdPrint::Process(Parser& parser) { // ����� � �������
    if (parser.get_last().name != "PRINT") // ���� ��� ��������� ������� �� �������� "PRINT"
        return false;

    for (;;) { // ����������� ����
        switch (parser.get_lexem().type) { // ��������� � ����������� �� ���� �������
        case LT_String: // ��� ������
            std::cout << parser.get_last().name; // ����� ������� � �������
            break;
        case LT_Identifier: // ��� �������������
            std::cout << NT.GetVariable(parser.get_last().name); // ����� �������� �� ������� ��� �������������� � �������
            break;
        default:
            std::cout << std::endl; // ����� ������ ������
            return true;
        }
    }

    return true;
}

// INPUT
bool CmdInput::Process(Parser& parser) { // ���� � �������
    if (parser.get_last().name != "INPUT") // ���� ��� ��������� ������� �� �������� "INPUT"
        return false;

    std::cout << "?";
    float value;
    std::cin >> value; // ���������� ����� ��������
    NT.SetVariable(parser.get_lexem().name, value); // ��������� ������� � � �������� � ������� ��� ��������������

    parser.get_lexem(); // ��������� ��������� �������

    return true;
}

// ��������� ����� for
struct ForInfo {
    std::string     control_variable; // �������
    Parser::Holder  loop_begin; // ������ �����
    float           upper_limit; // ����� �����
    float           step; // ���

    ForInfo() : step(1.0f) {} // ��������� ����
};

std::stack<ForInfo> CmdForStack;

// FOR I=0 TO 10
bool CmdFor::Process(Parser& parser) { // ���� for
    if (parser.get_last().name != "FOR") // ���� ��� ��������� ������� �� �������� "FOR"
        return false;

    ForInfo for_info; // ���������� ��������� �����

    if (parser.get_lexem().type != LT_Identifier) // ���� ��� ������� �� �������������
        throw "Identifier expected"; // ����� ��������� � ���, ��� �������� �������������

    for_info.control_variable = parser.get_last().name; // ������ ����� ������� � ���������

    if (parser.get_lexem().type != LT_Delimiter || // ���� ������� �� �������� ������������
        parser.get_last().delimiter != '=') // ��� ��������� ������� �� �������� ������������ "="
        throw "Assigment expected"; // ����� ��������� � ���, ��� ��������� ����������� 

    NT.SetVariable(for_info.control_variable, evaluate_expression(parser)); // ��������� ������� � � �������� � ������� ��� ��������������

    if (parser.get_last().type != LT_Identifier || // ���� ������� �� �������� ��������������� 
        parser.get_last().name != "TO") // ��� ��� ������� �� �������� "TO"
        throw "TO expected in FOR statement"; // ����� ��������� � ���, ��� ��������� "TO" � ����� for

    for_info.upper_limit = evaluate_expression(parser); // ������ ����� ����� � ���������

    if (parser.get_last().type != LT_EOL) // ���� ������� �� �������� �������� ����� ������
        throw "STEP not supported now in FOR statement"; // ����� ��������� � ���, ��� "STEP" �� �������������� � ����� for

    for_info.loop_begin = parser.Hold(); // ������ ������ ����� � ��������� 

    CmdForStack.push(for_info); // ������ ���������� � ����� � ����

    return true;
}

// NEXT
bool CmdNext::Process(Parser& parser) { // ������� ��� � �����
    if (parser.get_last().name != "NEXT") // ���� ��� ��������� ������� �� �������� "NEXT"
        return false;

    ForInfo for_info = CmdForStack.top(); // ��������� ��������� ����� for � ������ �����

    float value(NT.GetVariable(for_info.control_variable)); // ��������� �������� �������� ����� �� ������� ��� �������������� 
    value += for_info.step; // ���������� ��������
    NT.SetVariable(for_info.control_variable, value); // ��������� ������� � � �������� � ������� ��� ��������������

    if (value <= for_info.upper_limit) { // ���� ������� ������ ��� ����� ����� �����
        parser.Fetch(for_info.loop_begin); // ��������� ������ ����� � ������ ������ �� ������
    }
    else
        CmdForStack.pop(); // �������� �� ����� ���������� ��������

    parser.get_lexem(); // ��������� ��������� �������
    return true;
}

// GOTO
bool CmdGoto::Process(Parser& parser) { // �������� goto
    if (parser.get_last().name != "GOTO") // ���� ��� ��������� ������� �� �������� "GOTO"
        return false;

    if (parser.get_lexem().type != LT_Number) // ���� ��� ������� �� �����
        throw "Line number expected"; // ����� ��������� � ���, ��� ��������� �����

    parser.Fetch(LT.GetHolder(static_cast<unsigned int>(parser.get_last().value))); // ��������� �� ������� ����� �������� ��������, ����������� �� ������� (�������� �������)
                                                                                    // ����������� ������� � ������ � ������ ������
    parser.get_lexem(); // ��������� ��������� �������
    return true;
}

// IF 
bool CmdIf::Process(Parser& parser) { // �������� if
    if (parser.get_last().name != "IF") // ���� ��� ��������� ������� �� �������� "IF"
        return false;

    bool cond = (evaluate_expression(parser) != 0.0f); // ���� ������ �� ����� 0 

    if (parser.get_last().type != LT_Identifier || // ���� ��� ������� �� ������������� 
        parser.get_last().name != "THEN") // ��� ��� ������� �� �������� "THEN" 
        throw "THEN expected in IF statement"; // ����� ��������� � ���, ��� ��������� "THEN" � if

    if (!cond) // ���� ������ ����� 0
        parser.SkipUntilEOL(); // ������� ������
    else
        parser.get_lexem(); // ��������� ��������� �������

    return true;
}