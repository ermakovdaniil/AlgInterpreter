/*
  interface.h
  ������� ����������� ����� ������ �����
*/

#pragma once
#include <iosfwd>
#include "lexical_analizer.h"

class Command {
public:
    virtual ~Command() {} // ����������� ����������
    virtual bool Process(Parser&) = 0;
};