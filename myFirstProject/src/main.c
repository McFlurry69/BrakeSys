#include <ch.h>
#include <hal.h>
#include <tf_calculation.c>
#include <fuzzy.c>
#include <motCtrl.c>
#include <init_ADC.c>
#include <dif_inits.c>



#include <chprintf.h>
static uint8_t  sd_buff[10];

int main(void)
{
    chSysInit();
    halInit();
    initialization();
    Fuzzy_Init();

    while (true)
    {
      selfStabilizing();
      palSetPadMode(GPIOC, 13,PAL_MODE_INPUT);

      sdReadTimeout( &SD7, sd_buff, 1, TIME_IMMEDIATE   );
      if(sd_buff[0]=='0') counter += 20; //500 Hz
      if(sd_buff[0]=='1') counter -= 20; //500 Hz

      if(sd_buff[0]=='2') koeff += 1; //500 Hz
      if(sd_buff[0]=='3') koeff -= 2; //500 Hz

      //palSetLineMode(PAL_LINE(GPIOA,13),PAL_MODE_OUTPUT_PUSHPULL );
      //palSetPadMode(GPIOA,13,PAL_MODE_OUTPUT_PUSHPULL);
      //palSetLineMode(GPIOB_I2S2_SD, PAL_MODE_OUTPUT_PUSHPULL);
      //palSetLineMode(GPIOB_I2S2_CK, PAL_MODE_OUTPUT_PUSHPULL);
      palSetPadMode(GPIOB, 10,PAL_MODE_OUTPUT_PUSHPULL);
      palSetPadMode(GPIOB, 11,PAL_MODE_OUTPUT_PUSHPULL);

      palSetPad(GPIOB, 10);
      palClearPad(GPIOB, 11);
      //outp = samples1[0]*3 > 10000 ? 10000 : samples1[0]*5;

      //pwmEnableChannel( &PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, outp) );
    }
}

