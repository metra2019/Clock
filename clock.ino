#include <GyverEncoder.h>
#include <GyverTM1637.h>
#include <iarduino_RTC.h>
// подключение часов
iarduino_RTC cl(RTC_DS1307);

// подключение дисплея
#define CLK_DP 2
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

int modes[]
{
  CLOCK, TIMER
};

#define COUNT_MODES sizeof(modes) / sizeof(modes[0])
int mode = CLOCK;
int t_min = 0, t_sec = 0;

void setup()
{
  Serial.begin(9600);
  cls();
  disp.brightness(2);
  cl.begin();
  enc.setTickMode(AUTO);
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

  
// индикация времени
void print_time()
{
  static long l_time = 0;
  disp.point(true);
  cl.gettime();
  // индикация каждую секунду
  if (millis() - l_time > 1000)
  {
    disp.displayClock(cl.Hours, cl.minutes);
    l_time = millis();
  }
}

void loop() {
  // опрос эндокера - автоматический
  // обработка
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

  // меню
  switch (mode)
  {
    case CLOCK:
      print_time();
      break;
    case TIMER:
      timer(); // вызов функции таймера
      break;
  }
}
