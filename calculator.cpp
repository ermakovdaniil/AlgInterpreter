/*
     Calculator.cpp
   Модуль, отвечающий за вычисление выражений.
   Парсер выражения подаётся в функцию evaluate_expression.
   Все промежуточные вычисления и результаты в формате float.

   Особые ситуации:
      В случае возникновения ошибок выбрасываются исключения типа
    const char *
*/

#include "stdafx.h"
#include <math.h>
#include "calculator.h"
#include "name_table.h"

// Чтение первичного - т.е. минимально неделимого элемента выражения
float prim(Parser& parser) {
    parser.get_lexem(); // Получение лексемы

    switch (parser.get_last().type) { // Ветвление в зависимости от типа лексемы
    case LT_Number: // Тип число
    {
        float v = parser.get_last().value; // Сохранение значения лексемы в переменную
        parser.get_lexem(); // Получение следующей лексемы
        return v;  // Возвращение значения лексемы
    }
    case LT_Identifier: // Тип идентификатор
    {
        std::string name(parser.get_last().name); // Сохранение имени лексемы в переменную
        parser.get_lexem(); // Получение следующей лексемы
        return NT.GetVariable(name); // Возвращение ссылки на переменную с именем лексемы в таблице имён интерпретатора
    }
    break;
    case LT_Delimiter: // Тип разделитель
        switch (parser.get_last().delimiter) { // Ветвление в зависимости от разделителя
        case '-':
            return -prim(parser); // Возвращение "-"
        case '(':
        {
            float e = evaluate_expression(parser); // Получение "("
            if (parser.get_last().type == LT_Delimiter && parser.get_last().delimiter == ')') { // Если следующая лексема ")"
                parser.get_lexem(); // Получение следующей лексемы
                return e;  // Возвращение "("
            }
            else
                throw "\')\' expected"; // Вывод сообщенния о том, что ожидалось ")"
        }
        default:
            throw "primary expected"; // Вывод сообщенния о том, что ожидался первичный элемент
        }
    default:
        throw "primary expected"; // Вывод сообщенния о том, что ожидался первичный элемент
    }
}

// Обрабатывает возведение в степень
float exp(Parser& parser) {
    float left = prim(parser); // Получение левого аргумента
    for (;;) { // Бесконечный цикл
        if (parser.get_last().type == LT_Delimiter && parser.get_last().delimiter == '^') { // Если лексема является разделителем
            float right = prim(parser);	// Получение правого аргумента
            left = pow(left, right); // Возведение в степень
            break;
        }
        else
            return left; // Возвращение левого аргумента
    }
    return left;
}

// Обрабатывает умножение и деление
float term(Parser& parser) {
    float left = exp(parser); // Получение левого аргумента

    for (;;) { // Бесконечный цикл
        if (parser.get_last().type == LT_Delimiter) { // Если лексема является разделителем
            switch (parser.get_last().delimiter) // Ветвление в зависимости от разделителя
            {
            case '*':
                left *= exp(parser); // Умножение аргумента на следующий аргумент выражения
                break;
            case '/':
            { const float precision = 1.0e-5f; // Точность деления
            float d = exp(parser); // Получение следующего аргумента
            // TODO: ИСПРАВЛЕНА ОШИБКА "<" ИЗМЕНЕНО НА ">"
            if (fabs(d) > precision) { // Если модуль аргумента больше точности
                left /= d; // Деление
            }
            else
                throw "Devide by zero"; // Вывод сообщения о том, что произошло деление на 0
            }
            break;
            default:
                return left; // Возвращение результата умножения / деления
            }
        }
        else
            return left; // Возвращение левого аргумента
    }
    return left;
}

// Обрабатывает сложение и вычитание
float plus_minus(Parser& parser) {
    float left = term(parser); // Получение левого аргумента

    for (;;) { // Бесконечный цикл
        if (parser.get_last().type == LT_Delimiter) { // Если лексема является разделителем
            switch (parser.get_last().delimiter) // Ветвление в зависимости от разделителя
            {
            case '+':
                left += term(parser); // Сложение аргумента со следующим аргументом выражения
                break;
            case '-':
                left -= term(parser); // Вычитание из аргумента следующего аргумента выражения
                break;
            default:
                return left; // Возвращение результата сложения / вычитания
            }
        }
        else
            return left; // Возвращение левого аргумента
    }
    return left;
}

// Вычисляет выражение
float evaluate_expression(Parser& parser) {
    float left = plus_minus(parser); // Получение левого аргумента

    for (;;) { // Бесконечный цикл
        if (parser.get_last().type == LT_Delimiter) { // Если лексема является разделителем
            switch (parser.get_last().delimiter) // Ветвление в зависимости от разделителя
            {
            case '<':
                left = left < plus_minus(parser); // Сравнение левого аргумента со следующим аргументом. left = 1, если было меньше
                break;
            case '>':
                left = left > plus_minus(parser); // Сравнение левого аргумента со следующим аргументом. left = 1, если было больше
                break;
            case '=':
                left = left == plus_minus(parser); // Сравнение левого аргумента со следующим аргументом. left = 1, если были равны
                break;
            default:
                return left; // Возвращение результата сравнения
            }
        }
        else
            return left; // Возвращение левого аргумента
    }
    return left;
}