#include "Sensor.h"
CircularBuffer<double, 10> PM2_5;
CircularBuffer<double, 10> PM10;
SHT1x _sht(SHT_dataPin, SHT_clockPin);

SHT::SHT() {
	this->_sensor.sensor_value = new float[3];
}

SHT::~SHT() {
	delete[] this->_sensor.sensor_value;
}

void SHT::setup(){
	oTemp = this->read_value(120);
	oHum = this->read_value(125);
  if(isnan(oTemp)) {
    oTemp = 0.0;
    write_value(oTemp, 120);
  }
  if(isnan(oHum)) {
    oHum = 0.0;
    write_value(oHum, 125);
  }
}

void SHT::calibrate(){}

void SHT::read_calib(){
  oTemp = this->read_value(120);
  oHum = this->read_value(125);
}

int SHT::read() {
	this->_sensor.sensor_value[0] = _sht.readHumidity()+ oHum;
	this->_sensor.sensor_value[1] = _sht.readTemperatureC()+ oTemp;
	this->_sensor.sensor_value[2] = _sht.readTemperatureF()+ oTemp*3.2;
	if (((this->_sensor.sensor_value[0] < 0 || this->_sensor.sensor_value[0] > 100) || (this->_sensor.sensor_value[1] < -10 || this->_sensor.sensor_value[1] > 65))) {
		this->_sensor.sensor_status = 0;
		return 0;
	}
	this->_sensor.sensor_status = 1;
	return 1;
}

Dust::Dust() {
	this->_sensor.sensor_value = new float[2];
}

Dust::~Dust() {
	delete[] this->_sensor.sensor_value;
}

void Dust::setup() {
	Dust_Serial.begin(Dust_baudrate);
	aPm25 = this->read_value(100);
	bPm25 = this->read_value(105);
	aPm10 = this->read_value(110);
	bPm10 = this->read_value(115);
  if(isnan(aPm25)) {
    aPm25 = 1.0;
    write_value(aPm25, 100);
  }
  if(isnan(bPm25)) {
    bPm25 = 0.0;
    write_value(bPm25, 105);
  }
  if(isnan(aPm10)) {
    aPm10 = 1.0;
    write_value(aPm10, 110);
  }
  if(isnan(bPm10)) {
    bPm10 = 0.0;
    write_value(bPm10, 115);
  }
}

void Dust::read_calib(){
  aPm25 = this->read_value(100);
  bPm25 = this->read_value(105);
  aPm10 = this->read_value(110);
  bPm10 = this->read_value(115);
}

int Dust::read() {
	int error = 1;
	byte buffer;
	int Rxvalue;
	int len = 0;
	int pm25_serial = 0;
	int pm10_serial = 0;
	double p25;
	double p10;
	int checksum_is;
	bool checksum_ok = false;
	while((Dust_Serial.available() > 0) && (Dust_Serial.available() >= (10 - len)))
	{
		buffer = Dust_Serial.read();
		Rxvalue = int(buffer);
		switch (len)
		{
		case (0):	if (Rxvalue != 0xAA)
		{
			len = -1;
		};
					break;
		case (1):	if (Rxvalue != 0xC0)
		{
			len = -1;
		};
					break;
		case (2):	pm25_serial = Rxvalue; checksum_is = Rxvalue; break;
		case (3):	pm25_serial += (Rxvalue << 8); checksum_is += Rxvalue; break;
		case (4):	pm10_serial = Rxvalue; checksum_is += Rxvalue; break;
		case (5):	pm10_serial += (Rxvalue << 8); checksum_is += Rxvalue; break;
		case (6):	checksum_is += Rxvalue; break;
		case (7):	checksum_is += Rxvalue; break;
		case (8):	if (Rxvalue == (checksum_is % 256))
		        {
			        checksum_ok = true;
		        }
					  else
				    {
						    len = -1;
					  };
					break;
		case (9):	if (Rxvalue != 0xAB)
		{
			len = -1;
		};
					break;
		}

		len++;
		if (len == 10 && checksum_ok == true)
		{
			p25 = (double)pm25_serial / 10.0;
			p10 = (double)pm10_serial / 10.0;
			len = 0; checksum_ok = false; pm25_serial = 0.0; pm10_serial = 0.0; checksum_is = 0;
			error = 0;
			if ((aPm25 != 0) || (aPm10 != 0)) {
        if ((aPm25 == 0) && (bPm25 == 0)) {
           aPm25 = 1;
           bPm25 = 0;
        }
				if ((p25*aPm25 + bPm25) < 0) {
					PM2_5.push(0);
				}
        else if((p25*aPm25 + bPm25) >= 999){
          PM2_5.push(999);
        }
				else {
					PM2_5.push(p25*aPm25 + bPm25);
				}
        if ((aPm10 == 0) && (bPm10 == 0)) {
           aPm10 = 1;
           bPm10 = 0;
        }
				if ((p10*aPm10 + bPm10) < 0) {
					PM10.push(0);
				}
        else if((p10*aPm10 + bPm10) >= 999){
          PM10.push(999);
        }
				else {
					PM10.push(p10*aPm10 + bPm10);
				}
			}
			else {
				PM2_5.push(p25);
				PM10.push(p10);
			}
			Dust_Serial.flush();
			delay(10);
		}
	}
	if (PM2_5.size() > 0) {
		float value;
		for (unsigned int i = 0; i < PM2_5.size(); i++)
		{
			value += PM2_5[i];
		}
		value = (value / PM2_5.size());
		this->_sensor.sensor_value[0] = value;
		for (unsigned int i = 0; i < PM10.size(); i++)
		{
			value += PM10[i];
		}
		value = (value / PM10.size());
		this->_sensor.sensor_value[1] = value;
		this->_sensor.sensor_status = 1;
	}
	else {
		this->_sensor.sensor_status = 0;
	}
	return 1;
}

void Dust::calibrate() {

}

float Sensor::read_value(int _add) {
	float _value = 0.00;
  EEPROM.get(_add, _value);
  delay(50);
  if(abs(_value) < 1e-4) _value = 0.00;
  return _value;
}
int Sensor::write_value(float _value, int _add){
  EEPROM.put(_add, _value);
  delay(50);
  EEPROM.commit();
  return 1;
}
