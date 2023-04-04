/*
     Commands.cpp
   Модуль, отвечающий за вычисление выражений.
   Парсер выражения подаётся в функцию evaluate_expression.
   Все промежуточные вычисления и результаты в формате float.

   Особые ситуации:
       В случае возникновения ошибок выбрасываются исключения типа
   const char *
 */

#include "stdafx.h"
#include "commands.h"
#include "calculator.h"
#include "name_table.h"
#include "label_table.h"

 // LET
bool CmdExpression::Process(Parser& parser) { // Инициализация переменной
    if (parser.get_last().name != "LET") // Если имя последней лексемы не является "LET"
        return false;

    std::string left(parser.get_lexem().name); // Получение имени лексемы
    if (parser.get_lexem().type != LT_Delimiter || // Если лексема не является разделителем
        parser.get_last().delimiter != '=') // Или последняя лексема не является разделителем "="
        throw "Bad assigment"; // Вывод сообщения о том, что неправильно написана строка

    float value = evaluate_expression(parser); // Вычисление выражения

    NT.SetVariable(left, value); // Занесение лексемы и её значения в таблицу имён интерпретатора

    return true;
}

// PRINT
bool CmdPrint::Process(Parser& parser) { // Вывод в консоль
    if (parser.get_last().name != "PRINT") // Если имя последней лексемы не является "PRINT"
        return false;

    for (;;) { // Бесконечный цикл
        switch (parser.get_lexem().type) { // Ветвление в зависимости от типа лексемы
        case LT_String: // Тип строка
            std::cout << parser.get_last().name; // Вывод лексемы в консоль
            break;
        case LT_Identifier: // Тип идентификатор
            std::cout << NT.GetVariable(parser.get_last().name); // Вывод лексеммы из таблицы имён интерпретатора в консоль
            break;
        default:
            std::cout << std::endl; // Вывод пустой строки
            return true;
        }
    }

    return true;
}

// INPUT
bool CmdInput::Process(Parser& parser) { // Ввод в консоль
    if (parser.get_last().name != "INPUT") // Если имя последней лексемы не является "INPUT"
        return false;

    std::cout << "?";
    float value;
    std::cin >> value; // Считывание ввода значения
    NT.SetVariable(parser.get_lexem().name, value); // Занесение лексемы и её значения в таблицу имён интерпретатора

    parser.get_lexem(); // Получение следующей лексемы

    return true;
}

// Структура цикла for
struct ForInfo {
    std::string     control_variable; // Счётчик
    Parser::Holder  loop_begin; // Начало цикла
    float           upper_limit; // Конец цикла
    float           step; // Шаг

    ForInfo() : step(1.0f) {} // Установка шага
};

std::stack<ForInfo> CmdForStack;

// FOR I=0 TO 10
bool CmdFor::Process(Parser& parser) { // Цикл for
    if (parser.get_last().name != "FOR") // Если имя последней лексемы не является "FOR"
        return false;

    ForInfo for_info; // Объявление структуры цикла

    if (parser.get_lexem().type != LT_Identifier) // Если тип лексемы не идентификатор
        throw "Identifier expected"; // Вывод сообщения о том, что ожидался идентификатор

    for_info.control_variable = parser.get_last().name; // Запись имени лексемы в структуру

    if (parser.get_lexem().type != LT_Delimiter || // Если лексема не является разделителем
        parser.get_last().delimiter != '=') // Или последняя лексема не является разделителем "="
        throw "Assigment expected"; // Вывод сообщения о том, что ожидалось определение 

    NT.SetVariable(for_info.control_variable, evaluate_expression(parser)); // Занесение лексемы и её значения в таблицу имён интерпретатора

    if (parser.get_last().type != LT_Identifier || // Если лексема не является идентификатором 
        parser.get_last().name != "TO") // Или имя лексемы не является "TO"
        throw "TO expected in FOR statement"; // Вывод сообщения о том, что ожидалось "TO" в цикле for

    for_info.upper_limit = evaluate_expression(parser); // Запись конца цикла в структуру

    if (parser.get_last().type != LT_EOL) // Если лексема не является символом конца строки
        throw "STEP not supported now in FOR statement"; // Вывод сообщения о том, что "STEP" не поддерживается в цикле for

    for_info.loop_begin = parser.Hold(); // Запись начала цикла в структуру 

    CmdForStack.push(for_info); // Запись информации о цикле в стек

    return true;
}

// NEXT
bool CmdNext::Process(Parser& parser) { // Сделать шаг в цикле
    if (parser.get_last().name != "NEXT") // Если имя последней лексемы не является "NEXT"
        return false;

    ForInfo for_info = CmdForStack.top(); // Помещение структуры цикла for в начало стека

    float value(NT.GetVariable(for_info.control_variable)); // Получение значения счётчика цикла из таблицы имён интерпретатора 
    value += for_info.step; // Увеличение счётчика
    NT.SetVariable(for_info.control_variable, value); // Занесение лексемы и её значения в таблицу имён интерпретатора

    if (value <= for_info.upper_limit) { // Если счётчик меньше или равен концу цикла
        parser.Fetch(for_info.loop_begin); // Получение начала цикла и номера строки из буфера
    }
    else
        CmdForStack.pop(); // Удаление из стека последнего элемента

    parser.get_lexem(); // Получение следующей лексемы
    return true;
}

// GOTO
bool CmdGoto::Process(Parser& parser) { // Оператор goto
    if (parser.get_last().name != "GOTO") // Если имя последней лексемы не является "GOTO"
        return false;

    if (parser.get_lexem().type != LT_Number) // Если тип лексемы не число
        throw "Line number expected"; // Вывод сообщения о том, что ожидалось число

    parser.Fetch(LT.GetHolder(static_cast<unsigned int>(parser.get_last().value))); // Получение из таблицы меток перехода значение, находящееся на позиции (значение лексемы)
                                                                                    // Определение позиции в буфере и номера строки
    parser.get_lexem(); // Получение следующей лексемы
    return true;
}

// IF 
bool CmdIf::Process(Parser& parser) { // Оператор if
    if (parser.get_last().name != "IF") // Если имя последней лексемы не является "IF"
        return false;

    bool cond = (evaluate_expression(parser) != 0.0f); // Если расчёт не равен 0 

    if (parser.get_last().type != LT_Identifier || // Если тип лексемы не идентификатор 
        parser.get_last().name != "THEN") // Или имя лексемы не является "THEN" 
        throw "THEN expected in IF statement"; // Вывод сообщения о том, что ожидалось "THEN" в if

    if (!cond) // Если расчёт равен 0
        parser.SkipUntilEOL(); // Пропуск строки
    else
        parser.get_lexem(); // Получение следующей лексемы

    return true;
}