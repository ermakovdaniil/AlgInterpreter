/*
  name_table.cpp
  Таблица имён интерпретатора.
  Модуль отвечает за хранение имён команд и переменных.
*/

#include "stdafx.h"
#include "name_table.h"

NameTable NT;

NameTable::~NameTable() // Деструктор
{
    // Таблица имён "владеет" объектами команд
    std::map<std::string, Command*>::const_iterator it(commands.begin());
    for (; it != commands.end(); ++it)
        delete (it->second);
}

void NameTable::RegisterCommand(std::string const& name, Command* cmd) { // Регистрация команды языка в таблице имён
    commands[name] = cmd; // Добавление команды в контейнер
}

bool NameTable::ProcessCommand(Parser& parser) // Выполнение команды
{
    std::map<std::string, Command*>::const_iterator it(commands.begin()); // Контейнер map, хранящий команды
    for (; it != commands.end(); ++it) { // Цикл выполнения команд
        if (it->second->Process(parser)) // Если команда выполнена
            return true;
    }

    return false;
}

void  NameTable::SetVariable(std::string const& name, float value) // Занесение в таблицу имён интерпретатора лексему и её значение
{
    variables[name] = value; // Добавление переменной в контейнер
}

float NameTable::GetVariable(std::string const& name) // Получение значения из таблицы имён интерпретатора 
{
    return variables[name]; // Возвращение переменных
}