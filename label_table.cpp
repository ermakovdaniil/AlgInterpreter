/*
  LabelTable.cpp
  Модуль, отвечающий за хранение меток перехода
*/

#include "stdafx.h"
#include "label_table.h"

class LabelTableImpl {
    std::map<unsigned int, Parser::Holder> labels; // Представление таблицы в виде map
public:
    void           AddLabel(unsigned int, Parser::Holder); // Добавление метки перехода в таблицу
    Parser::Holder GetHolder(unsigned int); // Получение меток перехода из таблицы
};

LabelTable::LabelTable() : pImpl(new LabelTableImpl) { } // Конструктор

LabelTable::~LabelTable() { delete pImpl; pImpl = 0; } // Деструктор

void LabelTable::AddLabel(unsigned int param1, Parser::Holder param2) { // Добавление метки перехода через непрозрачный указатель
    pImpl->AddLabel(param1, param2); // Добавление метки перехода
}

Parser::Holder LabelTable::GetHolder(unsigned int param) { // Получение меток перехода через непрозрачный указатель 
    return pImpl->GetHolder(param); // Получение меток перехода
}

void LabelTableImpl::AddLabel(unsigned int label, Parser::Holder holder) { // Добавление метки перехода 
    labels[label] = holder; // Добавление метки в map контейнер
}

Parser::Holder LabelTableImpl::GetHolder(unsigned int label) { //  Получение меток перехода
    if (0 == labels.count(label)) // Если количество меток равно 0
        throw "Label is absent"; // Вывод сообщенния о том, что мет 
    return labels[label]; // Возвращение map контейнера с метками переходов
}

LabelTable LT; // Таблица меток перехода