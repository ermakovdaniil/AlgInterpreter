/*
  name_table.h
  Интерфейс таблицы имён интерпретатора
*/

#pragma once

#include <string>
#include <map>
#include "interface.h"

class NameTable {
    std::map<std::string, Command*> commands; // Команды
    std::map<std::string, float>    variables; // Переменные
public:
    ~NameTable(); // Деструктор
    // Регистрация команды языка в таблице имен
    void RegisterCommand(std::string const& name, Command*);
    // Выполнить команду, если это возможно
    bool ProcessCommand(Parser&);

    void  SetVariable(std::string const& name, float value); // Занесение лексемы и её значения в таблицу имён интерпретатора 
    float GetVariable(std::string const& name); // Получение значения из таблицы имён интерпретатора 
};

extern NameTable NT;