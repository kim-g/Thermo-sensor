using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    class Sensor
    {
        bool connected = false; // Подключено
        SerialPort COM; // Порт подключения по COM

        public Sensor(SerialPort Port)
        {
            COM = Port;
        }

        public bool Connected() //Показывает, подключено ли
        {
            return connected;
        }

        public bool Connect()
        {
            try
            {
                COM.Open();
            }
            catch
            {

            }
            finally
            {
                connected = COM.IsOpen;
            }
            return COM.IsOpen;
        }

    }
}
