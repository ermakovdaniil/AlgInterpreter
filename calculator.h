/*
  Calculator.h
  Интерфейс вычислителя выражений
*/

#pragma once
#include <cmath>
#include <string>
#include "lexical_analizer.h"

// Предварительное объявление вычислителя выражений
float evaluate_expression(Parser& parser);