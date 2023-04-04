/*
  interface.h
  Базовый абстрактный класс команд языка
*/

#pragma once
#include <iosfwd>
#include "lexical_analizer.h"

class Command {
public:
    virtual ~Command() {} // Виртуальный деструктор
    virtual bool Process(Parser&) = 0;
};