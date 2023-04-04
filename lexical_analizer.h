/*
  lexical_analizer.h
  ��������� ������������ �����������
*/

#pragma once
#include <fstream>

// ���� ������, ������������ �������������
enum  LexemType {
    LT_Unknown,
    LT_Number,
    LT_End,
    LT_Delimiter,
    LT_Identifier,
    LT_Label,
    LT_String,
    LT_EOL,
};

// ��������� �������
struct Lexem {
    LexemType   type;         // ���
    float       value;        // ��������    (���� ��� TT_Number)
    char        delimiter;    // ����������� (���� ��� TT_Delimiter)
    std::string name;         // ���         (���� ��� TT_Identifier)
};

// ������ �������� ������, ����������� ��� �� �������
class Parser {
    // ���� ��������� ����������� ����������� ������ �������������
    // ����� ��� ���������� ������������, ���������������� �� ���������
    // ��� ������������ ������������� �������� ������ Parser.
    // �������� �� � �������� ����� ������ Parser � �������� �����������
    // �������������� ��������� ����� �������.
    Parser(const Parser&);
    Parser& operator = (const Parser&);
public:
    class Holder {
        std::istream::pos_type pos; // ������� � ������
        unsigned int           line_number; // ����� ������
        friend class Parser; // ������������� ����� Parser
    };
    std::istream& in; // ����� ��� ������ � ������
    Lexem         last; // ��������� �������
    int           line_number; // ����� ������
public:
    Parser(std::istream& str); // �����������
    Lexem get_lexem(); // ��������� ��������� �������
    Lexem get_last(); // ��������� ���������� �������

    bool  SkipUntilEOL(); // ������� ������

    Holder  Hold() const; // ����������� ������� ��� �������� ���������� � ������� � ������
    void    Fetch(Holder); // ����������� ������� � ������ ����� � ����� ������
    void    Reset(); // ������� ������
};