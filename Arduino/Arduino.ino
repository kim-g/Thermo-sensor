#include "dht11.h"				// Добавим библиотеку для работы с сенсором
#include <LiquidCrystal.h>		// Добавим библиотеку для работы с экраном

#define DHT11PIN 2				// Вывод для сенсора
#define TEMP_CORR -1        		// Корректировка датчика температуры
#define HUM_CORR 28        		// Корректировка датчика влажности
#define UPDATE_Temp 5000      		// Количество млисекунд между обновлениями датчиков
#define UPDATE_Time 1000          // Количество млисекунд между обновлениями индикатора времени
#define LOOP_DELAY 100            // Время задержки в цикле
#define SECONDS_IN_DAY 86400	// Количество секунд в сутках
dht11 DHT11;					// Сенсор

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);	// Экран (в скобках пины подключения)

// Зададим символ градуса
// Custom Character Generator for HD44780 LCD Modules
// http://omerk.github.io/lcdchargen/
byte tempChar[8] = {0b00110,0b01001,0b01001,0b00110,0b00000,0b00000,0b00000,0b00000};
byte m_Char[8] = {0b00000,  0b00000,  0b10001,  0b11011,  0b10101,  0b10001,  0b10001,  0b00000};
byte p_Char[8] = {0b00000,  0b00000,  0b11111,  0b10001,  0b10001,  0b10001,  0b10001,  0b00000};
byte l_Char[8] = {0b00000,  0b00000,  0b01111,  0b01001,  0b01001,  0b01001,  0b10001,  0b00000};
byte I_Char[8] = {0b10001,0b10001,0b10001,0b10011,0b10101,0b11001,0b10001,0b00000};
byte Y_Char[8] = {0b10001,0b10001,0b10001,0b01111,0b00001,0b10001,0b01110,0b00000};
byte r_Char[8] = {0b00000,0b00000,0b11110,0b10001,0b10001,0b11110,0b10000,0b10000};
byte cube_Char[8] = {0b00100,0b01010,0b10001,0b11011,0b10101,0b10101,0b01010,0b00100};

int LastUpdateTemp = -10000;
int LastUpdateTime = -10000;
bool Separator = true;
byte Date[6] = {17,04,29,12,30,00};
unsigned long TimeDel = 0;


// Инициализация прибора
void setup() {
  Serial.begin(9600); //Активируем передачу данных по COM порту через USB и указываем скорость передачи в бодах
  lcd.begin(16, 2);	// Инициализация
  lcd.clear();		// Очищаем экран
  lcd.createChar(0, tempChar);		// Добавим знак градуса
  lcd.createChar(1, m_Char);    // Добавим знак 'м'
  lcd.createChar(2, p_Char);    // Добавим знак 'п'
  lcd.createChar(3, l_Char);    // Добавим знак 'л'
  lcd.createChar(4, I_Char);    // Добавим знак 'И'
  lcd.createChar(5, Y_Char);    // Добавим знак 'У'
  lcd.createChar(6, r_Char);    // Добавим знак 'р'
  lcd.createChar(7, cube_Char);    // Добавим знак кубика
  lcd.setCursor(0,0); 				// Установим курсор на начало первой строки
  lcd.print("CEHCOP t");lcd.write((byte)0); lcd.print(" ");lcd.write((byte)4); lcd.print(" B");lcd.write((byte)3); lcd.print(".");	//Выведем приветствие
  lcd.setCursor(0,1);  				// Установим курсор на начало второй строки
  lcd.print(" ");lcd.write((byte)7);lcd.print(" ");lcd.write((byte)4);lcd.print("OC ");lcd.write((byte)5);lcd.write((byte)6);lcd.print("O PAH  ");			//Выведем приветствие
  delay(5000);						// Подождём 2 секунды
  lcd.clear();						// Очищаем экран
  Temp_Hum_Show(0,0);
  LastUpdateTemp = millis();
  TimePrint(0,1);
  LastUpdateTime = millis();
}

//	Цикл работы
void loop() 
{
  int CurTime = millis();
	if (CurTime - LastUpdateTemp > UPDATE_Temp)
	{
		LastUpdateTemp = CurTime;
		Temp_Hum_Show(0,0);
	}
	
	//Вывод времени
 if (CurTime - LastUpdateTime > UPDATE_Time)
 {
    LastUpdateTime = CurTime;
    TimePrint(0,1);
 }

 // Приём данных по COM
 if (Serial.available() > 0)
 {
   char FL = Serial.read();
   // Установка даты/времени
   if (FL == 'T')  // Пришедший символ
   {
     byte Pos = 0;
     while (Serial.available() > 0) //Считаем то, что нам надо
     {
       Date[Pos] = (byte)Serial.read();
       Pos++;
       if (Pos>6) break;
     }
     TimeDel = millis();
   }

   // Установка даты/времени
   if (FL == 'G')  // Пришедший символ
   {
     Serial.println(DHT11.temperature+TEMP_CORR);
     Serial.println(';');
     Serial.println(DHT11.humidity+HUM_CORR);
   }
 }
		
	delay(LOOP_DELAY);
}

// Вывод влажности и температуры
void Temp_Hum_Show(int Col, int Row)
{
    int chk = DHT11.read(DHT11PIN);	// Считаем значение
 
    lcd.setCursor(0,0); // Установим курсор на начало первой строки
	
	// Получим коды ошибок DHT11
    switch (chk)
    {
        case DHTLIB_OK: 	// Если всё хорошо – продолжим, в противном случае напишем об ошибке
            break;
        case DHTLIB_ERROR_CHECKSUM: 
            lcd.print("Checksum error"); 
            delay(60000);
            return; // начать новый loop()
            break;
        case DHTLIB_ERROR_TIMEOUT: 
            lcd.print("Time out error"); 
            delay(60000);
            return; // начать новый loop()
            break;
        default: 
            lcd.print("Unknown error"); 
            delay(60000);
            return; // начать новый loop()
            break;
    }

	lcd.setCursor(Col, Row); // Установим курсор на начало первой строки
	lcd.print("Te");lcd.write((byte)1);lcd.write((byte)2);lcd.print(" "); lcd.print((int)DHT11.temperature+TEMP_CORR); lcd.write((byte)0); lcd.print(" ");	// Напишем температуру

	lcd.setCursor(Col + 9, Row); // Установим курсор на десятый символ первой строки
	lcd.print("B");lcd.write((byte)3);lcd.print(".");  lcd.print((int)DHT11.humidity+HUM_CORR);  lcd.print("% "); 	// Напишем влажность
}

// Вывод времени относительно времени работы
void TimePrint(int Col, int Row)
{
	lcd.setCursor(Col,Row); // Установим курсор на начало первой строки
	if (Date[2]<10) { lcd.print ("0"); } lcd.print (Date[2]); lcd.print ("."); // Число
  if (Date[1]<10) { lcd.print ("0"); } lcd.print (Date[1]); lcd.print ("."); // Месяц
  if (Date[0]<10) { lcd.print ("0"); } lcd.print (Date[0]);                  // Год


  // Вычисляем время
  int time=((millis()-TimeDel)/1000)%(SECONDS_IN_DAY);  //24*60*60
  byte Hour = Date[3] + time/60/60;
  byte Minutes = Date[4] + time/60%60;
  byte Seconds = Date[5] + time%60;
  Minutes += Seconds/60;
  Seconds = Seconds%60;
  Hour += Minutes/60;
  Minutes = Minutes % 60;

  // И нарисуем его
	lcd.setCursor(Col+11,Row); // Установим курсор на начало первой строки
	if (Hour<10) { lcd.print ("0"); }
	lcd.print (Hour);
	if (Separator) lcd.print (":"); else lcd.print (" ");
	Separator = !Separator;
	if (Minutes<10) { lcd.print ("0"); }
	lcd.print (Minutes);
}



