#include "dht11.h"				// Добавим библиотеку для работы с сенсором
#include <LiquidCrystal.h>		// Добавим библиотеку для работы с экраном

#define DHT11PIN 2				// Вывод для сенсора
#define TEMP_CORR -1        		// Корректировка датчика температуры
#define HUM_CORR 28        		// Корректировка датчика влажности
#define UPDATE_T 5000      		// Количество млисекунд между обновлениями датчиков
#define SECONDS_IN_DAY 86400	// Количество секунд в сутках
dht11 DHT11;					// Сенсор

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);	// Экран (в скобках пины подключения)

// Зададим символ градуса
// Custom Character Generator for HD44780 LCD Modules
// http://omerk.github.io/lcdchargen/
byte tempChar[8] = {0b00110,0b01001,0b01001,0b00110,0b00000,0b00000,0b00000,0b00000};

int LastUpdate = -10000;
bool Separator = true;
// Инициализация прибора
void setup() {
  lcd.begin(16, 2);	// Инициализация
  lcd.clear();		// Очищаем экран
  lcd.createChar(0, tempChar);		// Добавим знак градуса
  lcd.setCursor(0,0); 				// Установим курсор на начало первой строки
  lcd.print("Temperature sensor");	//Выведем приветствие
  lcd.setCursor(0,1);  				// Установим курсор на начало второй строки
  lcd.print("IOS UB RAS");			//Выведем приветствие
  delay(2000);						// Подождём 2 секунды
  lcd.clear();						// Очищаем экран
}

//	Цикл работы
void loop() 
{
    int CurTime = millis();
	if (CurTime - LastUpdate > UPDATE_T)
	{
		LastUpdate = CurTime;
		Temp_Hum_Show(0,0);
	}
	
	//Вывод времени
	TimePrint(0,1);
	
	delay(1000);
}

// Вывод влажности и температуры
void Temp_Hum_Show(int Col, int Row)
{
	// Очистим экран
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

    lcd.clear();	// Очищаем экран
	lcd.setCursor(Col, Row); // Установим курсор на начало первой строки
	lcd.print("Temp "); lcd.print((int)DHT11.temperature+TEMP_CORR); lcd.write((byte)0); lcd.print(" ");	// Напишем температуру

	lcd.setCursor(Col + 9, Row); // Установим курсор на десятый символ первой строки
	lcd.print("H. ");  lcd.print((int)DHT11.humidity+HUM_CORR);  lcd.print("% "); 	// Напишем влажность
}

// Вывод времени относительно времени работы
void TimePrint(int Col, int Row)
{
	int DateTime = millis()/1000; //Получаем время работы прибора
	int Date = DateTime / SECONDS_IN_DAY; // Получим количество дней
	
	lcd.setCursor(Col,Row); // Установим курсор на начало первой строки
	lcd.print ("26.04.17 ");
	
	lcd.setCursor(Col+11,Row); // Установим курсор на начало первой строки
	int time=DateTime%(SECONDS_IN_DAY);	//24*60*60
	if (time/60/60<10) { lcd.print ("0"); }
	lcd.print (time/60/60);
	if (Separator) lcd.print (":"); else lcd.print (" ");
	Separator = !Separator;
	if (time/60%60<10) { lcd.print ("0"); }
	lcd.print ((time/60)%60);
	/*lcd.print (":");
	if (time%60<10) { lcd.print ("0"); }
	lcd.print (time%60);*/
}

