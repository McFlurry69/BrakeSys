#include <motCtrl.h>



int16_t iprevRollError = 0;
int16_t iDeltaError = 0;
int16_t iRollError = 0;
int16_t controlCurrent=0;
int16_t koeff = 8;
int16_t Speed = DC_MIN_PWM;
double outp = 0;
double dMTh = 0;
int16_t counter = 0;
double currentNow=0;
int16_t error = 0;

//void transADC(void);


void selfStabilizing()
{
    currentNow = getADC();
    outp = getSpd();
    currentNow = constrain((currentNow-2048) * 0.1, -125, 125);
    error = (outp - currentNow);
    controlCurrent = constrain(error , -125, 125);
    iDeltaError = constrain(controlCurrent - iprevRollError, -125, 125);
    //iprevRollError = controlCurrent;
    if(counter > 50)
    {
      iprevRollError = controlCurrent;
      counter = 0;
    }
    counter++;
    // храним текущую ошибку для составления первой разницы
    // узнаем нечеткий вывод и переводим его в ШИМ для двигателя
    dMTh = getFuzzyConclusion(controlCurrent, iDeltaError) * koeff;  // 8 = 1000/125 - максимальное исправление в 1000 при 125
    // теперь нужно подстроить скорости вращения двигателей
    //int16_t iPart = (int16_t)(dMTh * 0.5);
    int16_t iPart = (int16_t)(dMTh);
    //Speed =(int16_t)dMTh;
    Speed += iPart;
    Speed = Speed > DC_MAX_PWM ? DC_MAX_PWM : Speed;
    Speed = Speed < DC_MIN_PWM ? DC_MIN_PWM : Speed;

    pwmEnableChannel( &PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, Speed) );
}

int16_t transADC()
{
  return outp;
}


