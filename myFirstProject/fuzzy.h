#include <math.h>
#include <stdint.h>

/*
*   Значения лингвистических переменных "отклонение", "воздействие"
*/

#define fVLN -125  //Очень большое отрицательное
#define fLN -90    //Большое отрицательное
#define fMN -55    //Среднее отрицательное
#define fSN -20    //Малое отрицательное
#define fNO 0      //Нулевое
#define fSP 20     //Малое положительное
#define fMP 55     //Среднее положительное
#define fLP 90     //Большое положительное
#define fVLP 125   //Очень сильное положительное

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef enum {
    AND,
    OR
} op_type;

uint8_t numofrules;

struct rule {
    int16_t fe;
    op_type op;
    int16_t fde;
    int16_t z;
};
struct rule rules[12];
/*
@brief Степень принадлежности µ𝔸(x) в какой степени (мере) элемент x принадлежит нечёткому множеству 𝔸.

\param[in] х Элемент из Х
\param[in] A Нечеткое множество 𝔸
\return Мера принадлежности
*/
double mu(int16_t x, int16_t A);

/*
@brief Добавить правила к регулятору

\param[in] ife значение лингвистической переменной "отклонение" для ошибки
\param[in] op оператор И / ИЛИ
\param[in] deltae значение лингвистической переменной "отклонение" первой разности ошибки
\param[in] then значение лингвистической переменной "воздействие"
*/
void addrule(int16_t fe, op_type op, int16_t fde, int16_t z);

/*
@brief Реализация нечеткого регулятора согласно правил

\param[in] e текущая ошибка
\param[in] de первая разница ошибок
\return Управляющее воздействие
*/
double getFuzzyConclusion(int16_t e, int16_t de);
/**
 * @brief Инициализация нечеткого алгоритма добавление правил
 *
 */
void Fuzzy_Init();
