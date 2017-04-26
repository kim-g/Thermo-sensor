#include "dht11.h"				// Добавим библиотеку для работы с сенсором
#include <LiquidCrystal.h>		// Добавим библиотеку для работы с экраном

#define DHT11PIN 2				// Вывод для сенсора
dht11 DHT11;					// Сенсор

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);	// Экран (в скобках пины подключения)

// Зададим символ градуса
// Custom Character Generator for HD44780 LCD Modules
// http://omerk.github.io/lcdchargen/
byte tempChar[8] = {0b00110,0b01001,0b01001,0b00110,0b00000,0b00000,0b00000,0b00000};


// Количество итераций (не надо, убрать)
int x = 0;

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
}

//	Цикл работы
void loop() 
{
    // Очистим экран
	lcd.clear();
    x++;	// Номер измерения
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
	lcd.setCursor(0,0); // Установим курсор на начало первой строки
	lcd.print("Temp "); lcd.print((int)DHT11.temperature); lcd.write((byte)0);	// Напишем температуру

	lcd.setCursor(0,1); // Установим курсор на начало второй строки
	lcd.print("Humidity ");  lcd.print((int)DHT11.humidity);  lcd.print("%   "); 	// Напишем влажность
	
	delay(5000);
}

