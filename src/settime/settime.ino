#include <iarduino_RTC.h>
iarduino_RTC watch(RTC_DS1307);                         // Объявляем объект watch для работы с RTC модулем на базе чипа DS1307, используется шина I2C.

//перед прошивкой
#define HOUR 10
#define MIN 40
#define SEC 0
#define DAY 27
#define MONTH 3
#define YEAR 22
#define DAY_MONTH 7


void setup(){                                               //                                         // Ждем готовности модуля отвечать на запросы.
    Serial.begin(9600);                                     // Инициируем передачу данных в монитор последовательного порта на скорости 9600 бод.
    watch.begin();                                          // Инициируем работу с модулем.
    watch.settime(SEC,MIN,HOUR,DAY,MONTH,YEAR,DAY_MONTH);                      // Записываем время в модуль: 0 сек, 51 мин, 21 час, 27, октября, 2015 года, вторник.
}                                                           //
void loop(){                                                //
    if(millis()%1000==0){                                   // Если прошла 1 секунда.
      Serial.println(watch.gettime("d-m-Y, H:i:s, D"));     // Выводим время.
      delay(1);                                             // Приостанавливаем скетч на 1 мс, чтоб не выводить время несколько раз за 1мс.
    }                                                       //
}     
