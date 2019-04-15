    #include <ch.h>
    #include <hal.h>

    #include <chprintf.h>


    PWMConfig pwm1conf = {
        .frequency = 100000,
        .period    = 20,
        .callback  = NULL,
        .channels  = {
                      {.mode = PWM_OUTPUT_DISABLED, .callback = NULL}, // Channel 1 is working CH1 = PE9
                      {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                      {.mode = PWM_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                      {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL}
                      },
        .cr2        = 0,
        .dier       = 0
    };


    void initialization(void)
    {
    gptStart(&GPTD5, &gpt5cfg1);
    gptStartContinuous(&GPTD5, gpt5cfg1.frequency / 1000); //Timer period = 1ms
    gptStart(&GPTD4, &gpt4cfg1);
    // ADC driver
    adcStart(&ADCD1, NULL);
    palSetLineMode( LINE_ADC123_IN10, PAL_MODE_INPUT_ANALOG );  // PC0
    palSetLineMode( LINE_ADC123_IN3, PAL_MODE_INPUT_ANALOG );   // PA3

    adcStartConversion(&ADCD1, &adcgrpcfg1, samples1, ADC1_BUF_DEPTH);
    gptStartContinuous(&GPTD4, gpt4cfg1.frequency/1000);          // how often we need ADC value
    /* Just set the limit (interval) of timer counter, you can use this function
          not only for ADC triggering, but start infinite counting of timer for callback processing */

    palSetLineMode( PAL_LINE( GPIOB, 0 ),  PAL_MODE_ALTERNATE(2) );
    pwmStart( &PWMD3, &pwm1conf );


    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );    // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );    // RX
    }
