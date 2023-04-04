/*
  Commands.h
  ��������� ������ �����
*/

#pragma once
#include <string>
#include <iostream>
#include <stack>
#include "interface.h"

// �������� �������, ������� ��������� ������� BASIC-��������� �����
class CmdExpression : public Command { // ������������� ���������
public:
    virtual bool Process(Parser&); // �������������� ������� �������� ������ Command
};

class CmdPrint : public Command { // ����� � �������
public:
    virtual bool Process(Parser&);
};

class CmdInput : public Command { // ���� � �������
public:
    virtual bool Process(Parser&);
};

class CmdFor : public Command { // ���� for
public:
    virtual bool Process(Parser&);
};

class CmdNext : public Command { // ������� ��� � �����
public:
    virtual bool Process(Parser&);
};

class CmdGoto : public Command { // �������� goto
public:
    virtual bool Process(Parser&);
};

class CmdIf : public Command { // �������� if
public:
    virtual bool Process(Parser&);
};