/*
  label_table.h
  Интерфейс таблицы меток переходов
*/

#pragma once

#include <map>
#include "lexical_analizer.h"

class LabelTable {
    class LabelTableImpl* pImpl; // Непрозрачный указатель на таблицу меток
public:
    LabelTable(); // Конструктор
    ~LabelTable(); // Деструктор
    void           AddLabel(unsigned int, Parser::Holder); // Добавление метки перехода в тублицу
    Parser::Holder GetHolder(unsigned int); // Получение меток перехода из таблицы
};

extern LabelTable LT;