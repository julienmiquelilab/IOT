/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * Motion Sensor example using HC-SR501
 * http://www.mysensors.org/build/motion
 *
 */
//////////////////////// Power saver
//Optimisation de la consommation
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h> 
volatile int f_wdt=1;



//////////////////////// Power saver


// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <MySensors.h>

unsigned long SLEEP_TIME = 120000; // Sleep time between reports (in milliseconds)
#define DIGITAL_INPUT_SENSOR 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define CHILD_ID 1   // Id of the sensor child

// Initialize motion message
MyMessage msg(CHILD_ID, V_TRIPPED);

//////////////////////// Power saver

// Watchdog Interrupt Service est exécité lors d'un timeout du WDT
/*
ISR(WDT_vect) {
if(f_wdt == 0) {
  f_wdt = 1; // flag global }
}

// paramètre : 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9=8 sec
void setup_watchdog(int ii) 
{
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
  // Clear the reset flag
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);
}

void enterSleep(void) 
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode(); //Entre dans le mode veille choisi

  // Le programme va reprendre ici après le timeout du WDT

  sleep_disable(); // La 1ère chose à faire est de désactiver le mode veille
}
//////////////////////// Power saver
*/

void setup()
{
  clock_prescale_set(clock_div_2);
  
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input

  //Optimisation de la consommation
  //power_adc_disable(); // Convertisseur Analog / Digital pour les entrées analogiques

  //power_spi_disable();
  //power_twi_disable(); 
  // Si pas besoin de communiquer par l'usb
  //power_usart0_disable();
  //Extinction des timers, attention timer0 utilisé par millis ou delay
  //power_timer0_disable();
  //power_timer1_disable();
  //power_timer2_disable();


  //setup_watchdog(9);
}

void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Motion Sensor", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MOTION);
}

bool lastTripped = false;
void loop()
{
   // Read digital motion value
  bool tripped = digitalRead(DIGITAL_INPUT_SENSOR) == HIGH;

  Serial.println(tripped);
  if(lastTripped != tripped)
  {
    send(msg.set(tripped?"1":"0"));  // Send tripped value to gw  
  }
  // Sleep until interrupt comes in on motion sensor. Send update every two minute.
  sleep(digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), CHANGE, SLEEP_TIME);
}
