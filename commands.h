/*
  Commands.h
  Интерфейс команд языка
*/

#pragma once
#include <string>
#include <iostream>
#include <stack>
#include "interface.h"

// Создание классов, которые описывают команды BASIC-образного языка
class CmdExpression : public Command { // Инициализация перемнной
public:
    virtual bool Process(Parser&); // Переопредление функции базового класса Command
};

class CmdPrint : public Command { // Вывод в консоль
public:
    virtual bool Process(Parser&);
};

class CmdInput : public Command { // Ввод в консоль
public:
    virtual bool Process(Parser&);
};

class CmdFor : public Command { // Цикл for
public:
    virtual bool Process(Parser&);
};

class CmdNext : public Command { // Сделать шаг в цикле
public:
    virtual bool Process(Parser&);
};

class CmdGoto : public Command { // Оператор goto
public:
    virtual bool Process(Parser&);
};

class CmdIf : public Command { // Оператор if
public:
    virtual bool Process(Parser&);
};