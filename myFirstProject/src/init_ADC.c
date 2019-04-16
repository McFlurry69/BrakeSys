#include <ch.h>
#include <hal.h>
#include <motCtrl.h>
#include <chprintf.h>

#define ADC1_NUM_CHANNELS   2
#define ADC1_BUF_DEPTH      1
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


static adcsample_t samples1[ADC1_NUM_CHANNELS * ADC1_BUF_DEPTH];
/*
static int average = 0;
static double fuzzyCurrent=0;
static int count = 0;
static int result = 0;
static int first = 0;
static int second = 0;*/
static int16_t fuzzyCurrent=0;
static int n = 150;
static int16_t inp[3000];
static int16_t out[3000];
static int window = 40;
static int count=0;
static int count2=0;
static int16_t retSpeed=0;

TFConf_t tfconf = {
                   .k             = 125,
                   .T             = 1.5,
                   .prev_output   = 0.0,
                   .output        = 0.0,
                   .input         = 1,
                   .a             = 0,
                   .b             = 0
};

/* pointer to stucture*/
TFConf_t *p_tfconf = &tfconf;


void transm (void);
void smooth(int16_t *input, int16_t *output, int n, int window);
int16_t Expon = 0;

int getADC(void)
{
  return fuzzyCurrent;
}
int getSpd(void)
{
  return Expon;
}

void smooth(int16_t *input, int16_t *output, int n, int window)
{
   int q,i,j,z,k1,k2,hw;
   double tmp, average;
   if(window % 2==0) window++;
   hw=(window-1)/2;
   output[0]=input[0];

   for (i=1;i<n;i++)
   {
       tmp=0;
       if(i<hw){
           k1=0;
           k2=2*i;
           z=k2+1;
       }
       else if((i+hw)>(n-1)){
           k1=i-n+i+1;
           k2=n-1;
           z=k2-k1+1;
       }
       else{
           k1=i-hw;
           k2=i+hw;
           z=window;
       }

       for (j=k1;j<=k2;j++){
           tmp=tmp+input[j];
       }
       if(i % 30 == 0)
       {
         for(q=i;q<i-30;q++)
         {
           average += output[q];
         }
         average/=30;
       }
       if(tmp/z > average + average*0.015)
       {
         output[i]=average;
       }
       else
       {
         output[i]=tmp/z;
       }
   }
}

void transm (void)
{
  inp[count]=samples1[1];
  count++;
  if (count > n)
  {
    smooth(&inp, &out, n, window);
  }
  if (count2 < n)
  {
        fuzzyCurrent=out[count2];
        retSpeed = transADC();
        sdWrite(&SD7, &retSpeed, 2);
        count2++;
  }
  else
  {
    count2 = 0;
    count = 0;
  }
}
//uint16_t gpt_callback_counter = 0, i = 0;
float out_array[205] = {0.0};
int16_t ccc = 0;
static void gpt5_callback (GPTDriver *gptp)
{
    (void)gptp;
    //tfconf.input = palReadPad(GPIOC, 13);//commonADC1UnitGetValue ( 2 );
    if (palReadPad(GPIOC, 13))
    {
      Expon = 1 - exp((-ccc)/400);
      ccc++;
      if(ccc > 2500)
      {
        ccc = 0;
      }
    }
    //tfOutCalculation ( p_tfconf );

    //Expon = (int16_t)tfconf.output;
    //gpt_callback_counter ++;
    /*if ( gpt_callback_counter == 500 )
    {
        gpt_callback_counter = 0;
        if ( i <= 204 )
        {
            out_array[i] = tfconf.output;
            i++;
        }else{
            i=0;
        }
    }*/

}

static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
  (void)adcp;
  (void)buffer;
  (void)n;
}

/* ADC errors callback, should never happen */
static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;
}

static const ADCConversionGroup adcgrpcfg1 = {
  .circular     = true,                                           // working mode = looped
  .num_channels = ADC1_NUM_CHANNELS,
  .end_cb       = adccallback,
  .error_cb     = adcerrorcallback,
  .cr1          = 0,
  .cr2          = ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(0b1100),  // Commutated from GPT
  .smpr1        = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_144),             // for AN10 - 144 samples
  .smpr2        = ADC_SMPR2_SMP_AN3(ADC_SAMPLE_144),              // for AN3  - 144 samples
  .sqr1         = ADC_SQR1_NUM_CH(ADC1_NUM_CHANNELS),
  .sqr2         = 0,
  .sqr3         = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN3) |               // sequence of channels
                  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN10)
  /* If we can macro ADC_SQR2_SQ... we need to write to .sqr2 */
};

static const GPTConfig gpt4cfg1 = {
  .frequency =  600000,
  .callback  =  transm,
  .cr2       =  TIM_CR2_MMS_1,  /* MMS = 010 = TRGO on Update Event.        */
  .dier      =  0U
  /* .dier field is direct setup of register, we don`t need to set anything here until now */
};

static const GPTConfig gpt5cfg1 = {
  .frequency =  30000,        /*  1MHz*/
  .callback  =  gpt5_callback,
  .cr2       =  TIM_CR2_MMS_1,  /* MMS = 010 = TRGO on Update Event.        */
  .dier      =  0U
  /* .dier field is direct setup of register, we don`t need to set anything here until now */
};

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};
