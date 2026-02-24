#pragma once

/// @brief Нормальное форматирование строки при выводе в консоль escape-последовательностью
#define NORMAL_FORMAT "\x1b[0m"

/// @brief Чёрный цвет шрифта строки при выводе в консоль escape-последовательностью
#define BLACK_FORMAT "\x1b[30m"
/// @brief Форматирует текст в скобках, возвращая настройки к нормальным далее. Чёрный шрифт
#define BLACK_TEXT(...) BLACK_FORMAT __VA_ARGS__ NORMAL_FORMAT

/// @brief Красный цвет шрифта строки при выводе в консоль escape-последовательностью
#define RED_FORMAT "\x1b[31m"
/// @brief Форматирует текст в скобках, возвращая настройки к нормальным далее. Красный шрифт
#define RED_TEXT(...) RED_FORMAT __VA_ARGS__ NORMAL_FORMAT

/// @brief Зелёный цвет шрифта строки при выводе в консоль escape-последовательностью
#define GREEN_FORMAT "\x1b[32m"
/// @brief Форматирует текст в скобках, возвращая настройки к нормальным далее. Зелёный шрифт
#define GREEN_TEXT(...) GREEN_FORMAT __VA_ARGS__ NORMAL_FORMAT

/// @brief Жёлтый цвет шрифта строки при выводе в консоль escape-последовательностью
#define YELLOW_FORMAT "\x1b[33m"
/// @brief Форматирует текст в скобках, возвращая настройки к нормальным далее. Жёлтый шрифт
#define YELLOW_TEXT(...) YELLOW_FORMAT __VA_ARGS__ NORMAL_FORMAT

/// @brief Жирное форматирование строки при выводе в консоль escape-последовательностью
#define BOLD_FORMAT "\x1b[1m"
/// @brief Форматирует текст в скобках, возвращая настройки к нормальным далее. Жирный шрифт
#define BOLD_TEXT(...) BOLD_FORMAT __VA_ARGS__ NORMAL_FORMAT

/// @brief Курсивное форматирование строки при выводе в консоль escape-последовательностью
#define ITALIC_FORMAT "\x1b[3m"
/// @brief Форматирует текст в скобках, возвращая настройки к нормальным далее. Курсивный шрифт
#define ITALIC_TEXT(...) ITALIC_FORMAT __VA_ARGS__ NORMAL_FORMAT
