#include <fuzzy.h>

/*
Степень принадлежности µ𝔸(x)
в какой степени (мере) элемент x принадлежит нечёткому множеству 𝔸.
\param[out] Мера принадлежности
\param[in] х Элемент из Х
\param[in] A Нечеткое множество 𝔸
*/

#define fVLN      -125 //Очень большое отрицательное
#define fLN       -90 //Большое отрицательное
#define fMN       -55 //Среднее отрицательное
#define fSN       -20 //Малое отрицательное
#define fNO       0 //Нулевое
#define fSP       20 //Малое положительное
#define fMP       55 //Среднее положительное
#define fLP       90 //Большое положительное
#define fVLP      125 //Очень сильное положительное

double mu(int16_t x, int16_t A) {
    return exp((-(x - A)*(x - A)) / 1800);
}

/*
Добавить правила к регулятору
\param[out]
\param[in] ife значение лингвистической переменной "отклонение" для ошибки
\param[in] op оператор И / ИЛИ
\param[in] deltae значение лингвистической переменной "отклонение" первой разности ошибки
\param[in] then значение лингвистической переменной "воздействие"
*/
void addrule(int16_t fe, op_type op, int16_t fde, int16_t z) {
    rules[numofrules].fe = fe;
    rules[numofrules].op = op;
    rules[numofrules].fde = fde;
    rules[numofrules].z = z;
    numofrules++;
}

/*
Решение нечеткого регулятора. Нечеткий вывод
\param[out] Управляющее воздействие
\param[in] e текущая ошибка
\param[in] de первая разница ошибок
*/
double getFuzzyConclusion(int16_t e, int16_t de) {
    double summ_alpha_c = 0, summ_alpha = 0;

    // цикл по правилам

    for (int i = 0; i < (int)(numofrules); i++)
    {
        double alpha = 0, mue = 0, mude = 0;
                // степень соответствия ошибки нечеткому множеству fe для i-го
        mue = mu(e, rules[i].fe);
        // степень соответствия первой разности ошибки нечеткому множеству fde для i-го правила
        mude = mu(de, rules[i].fde); // применяем логический оператор И/ИЛИ
        alpha = rules[i].op == 0 ? MIN(mue, mude) : MAX(mue, mude);
        // числитель и знаменатель для дискретного варианта
        // центроидного метода приведения к четкости
        summ_alpha_c += (alpha * rules[i].z);
        summ_alpha += alpha;
    }
    // вычисляем воздействие на объект управления
    return summ_alpha_c / summ_alpha;
}

/*
Инициализация нечеткого алгоритма добавление правил
*/
void Fuzzy_Init()
{
    numofrules = 0;
    addrule(fNO, AND, fNO, fNO);
    addrule(fSN, AND, fSN, fSN);
    addrule(fSP, AND, fSP, fSP);
    addrule(fVLN, OR, fVLN, fVLN);
    addrule(fVLP, OR, fVLP, fVLP);
    addrule(fLN, AND, fSN, fVLN);
    addrule(fLP, AND, fSP, fVLP);
    addrule(fMN, AND, fMP, fSN);
    addrule(fMN, AND, fMN, fLN);
    addrule(fMP, AND, fMN, fSP);
    addrule(fMP, AND, fMP, fLP);
    /*
    addrule(fNO, AND, fNO, fNO);
    addrule(fVLN, OR, fVLN, fVLP);
    addrule(fVLP, OR, fVLP, fVLN);
    addrule(fLN, AND, fSN, fVLP);
    addrule(fLP, AND, fSP, fVLN);
    addrule(fSN, AND, fSN, fSP);
    addrule(fSP, AND, fSP, fSN);
    addrule(fMN, AND, fMP, fSP);
    addrule(fMN, AND, fMN, fLP);
    addrule(fMP, AND, fMN, fSN);
    addrule(fMP, AND, fMP, fLN);*/
}
