#include <GyverEncoder.h>
#include <GyverTM1637.h>
#include <iarduino_RTC.h>
// подключение часов
iarduino_RTC cl(RTC_DS1307);

// подключение дисплея
#define CLK_DP 5
#define DIO_DP 4
GyverTM1637 disp(CLK_DP, DIO_DP);

// подключение эндокера
#define CLK_ENC 6 // s1
#define DT 7 // s2
#define SW 8 // key
Encoder enc(CLK_ENC, DT, SW, TYPE2); // для работы c энкодером

// режимы
#define CLOCK 0
#define TIMER 1
#define SEC 2
#define CHARGE 3
#define SETTING

int modes[]
{
  CLOCK, TIMER, SEC, CHARGE
};

#define COUNT_MODES sizeof(modes) / sizeof(modes[0])
int mode = CLOCK;
int t_min = 0, t_sec = 0;

//зуммер
#define buz 3
#define FREEQ_TICK 1000
#define FREEQ_TIME 100

//АКБ
#define bat_pin A1
#define K 5./1024
double lavel_charge = 0;

#define but 2
int i = 0;

void setup()
{
  Serial.begin(9600);
  cls();
  disp.brightness(2);
  cl.begin();
  enc.setTickMode(AUTO);

  mode = 0;
  disp.point(false);
  
  lavel_charge = analogRead(bat_pin)*K / 4.6 * 100;
  
  pinMode(but, INPUT_PULLUP);
  attachInterrupt(0, but_tick, FALLING);
}

void but_tick(){
  i++;  
}

// функция очистки
void cls()
{
  disp.point(false);
  disp.clear();
}

// функция таймера
void timer(int m = 0, int s = 0)
{
  disp.point(true);
  disp.displayClock(m, s);
}

void secmer() {
  static int l_time = 0;
  disp.point(false);

  if (millis() - l_time > 1000) {
    l_time = millis();
    disp.displayInt(i);
  }
  if (i > 100) i = 0;
}

void get_charge() {
  static long l_time = 0;
  disp.point(false);
  
  if (millis() - l_time > 60*000)
    disp.displayInt(analogRead(bat_pin)*K / 4.6 * 100);
}
// индикация времени
void print_time()
{
  static long l_time = 0;
  disp.point(true);
  cl.gettime();
  // индикация каждую секунду
  if (millis() - l_time > 10000)
  {
    disp.displayClock(cl.Hours, cl.minutes);
    l_time = millis();
  }
}

void loop() {
  if (enc.isRight()) {
    if (mode == COUNT_MODES - 1)
      mode = 0;
    else
      mode++;
  }
  if (enc.isLeft()) {
    if (mode == 0)
      mode = COUNT_MODES - 1;
    else
      mode--;
  }

  if (enc.isPress()) {
    tone(buz, FREEQ_TICK, FREEQ_TIME);
  }
  // меню
  switch (mode)
  {
    case CLOCK:
      print_time();
      break;
    case TIMER:
      timer(); // вызов функции таймера
      break;
    case SEC:
      secmer();
      break;
    case CHARGE:
      get_charge();
      break;
  }
}
