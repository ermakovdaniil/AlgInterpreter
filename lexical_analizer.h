/*
  lexical_analizer.h
  Интерфейс лексического анализатора
*/

#pragma once
#include <fstream>

// Типы лексем, используемых калькулятором
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

// Структура лексемы
struct Lexem {
    LexemType   type;         // тип
    float       value;        // значение    (если тип TT_Number)
    char        delimiter;    // разделитель (если тип TT_Delimiter)
    std::string name;         // имя         (если тип TT_Identifier)
};

// Парсер входного текста, разделяющий его на лексемы
class Parser {
    // Есть опасность копирования содержимого класса конструктором
    // копии или оператором присваивания, сгенерированными по умолчанию
    // при неправильном использовании объектов класса Parser.
    // Объявляя их в закрытой части класса Parser я подавляю возможность
    // автоматической генерации таких методов.
    Parser(const Parser&);
    Parser& operator = (const Parser&);
public:
    class Holder {
        std::istream::pos_type pos; // Позиция в потоке
        unsigned int           line_number; // Номер строки
        friend class Parser; // Дружественный класс Parser
    };
    std::istream& in; // Поток для работы с вводом
    Lexem         last; // Последняя лексема
    int           line_number; // Номер строки
public:
    Parser(std::istream& str); // Конструктор
    Lexem get_lexem(); // Получение следующей лексемы
    Lexem get_last(); // Получение предыдущей лексемы

    bool  SkipUntilEOL(); // Пропуск строки

    Holder  Hold() const; // Конструктор объекта для хранения информации о позиции в потоке
    void    Fetch(Holder); // Определение позиции в буфере ввода и номер строки
    void    Reset(); // Очистка буфера
};