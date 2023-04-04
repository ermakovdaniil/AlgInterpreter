/*
  Executor.cpp
  Модуль, отвечающий за инициализацию и выполнение программы.
*/

#include "stdafx.h"
#include "executor.h"
#include "name_table.h"
#include "label_table.h"
#include "lexical_analizer.h"
#include "commands.h"

void register_commands() { // Запись имён команд в таблицу имён интерпретатора
    NT.RegisterCommand("FOR", new CmdFor);
    NT.RegisterCommand("GOTO", new CmdGoto);
    NT.RegisterCommand("IF", new CmdIf);
    NT.RegisterCommand("INPUT", new CmdInput);
    NT.RegisterCommand("LET", new CmdExpression);
    NT.RegisterCommand("NEXT", new CmdNext);
    NT.RegisterCommand("PRINT", new CmdPrint);
}

// Функция обязана вернуть false при завершении
bool execute_command(Parser& parser) {
    Lexem lex = parser.get_last(); // Получение лексемы

    if (lex.type == LT_End) // Если лексема не является концом файла 
        return false;

    switch (lex.type) { // Ветвление в зависимости от типа лексемы
    case LT_Identifier: // Тип идентификатор
    {
        if (!NT.ProcessCommand(parser))
            throw "Can't process command"; // Вывод сообщенния о том, что невозможно выполнить команду 
    }
    break;
    case LT_EOL: // Тип конец строки
        parser.get_lexem(); // Получение следующей лексемы 
        break;
    case LT_Number: // Тип число
        parser.get_lexem(); // Получение следующей лексемы 
        break;
    default:
        throw "Bad syntax";
    }
    return true;
}

void scan_for_labels(Parser& parser) // Поиск меток для перехода
{
    bool prev_lexem_is_EOL(true); // Переменная, следящая за концом строки
    while (parser.get_lexem().type != LT_End) { // Пока тип лексемы не конец файла
        if (prev_lexem_is_EOL && parser.get_last().type == LT_Number) { // Если прошлая лексема не конец строки и тип текущей лексемы число
            unsigned int label(static_cast<unsigned int>(parser.get_last().value)); // Приведение значения метки к типу uint
            parser.get_lexem(); // Получение следующей лексемы 
            LT.AddLabel(label, parser.Hold()); // Добавление метки перехода в таблицу меток перехода
        }

        prev_lexem_is_EOL = (parser.get_last().type == LT_EOL); // Проверка лексемы на принадлежность к типу "конец строки"
    }
}

void execute_script(std::istream& in) { // Выполнение скрипта
    register_commands(); // Запись имён команд в таблицу имён интерпретатора
    Parser parser(in);

    scan_for_labels(parser); // Поиск меток для перехода
    parser.Reset(); // Очистка буфера 


    parser.get_lexem(); // Получение следующей лексемы 
    while (execute_command(parser)) // Выполнение команды до тех пора, пока функция не вернёт false 
        ;

    std::cout << "OK" << std::endl; // Вывод сообщенния о том, что выполнения завершено  
}