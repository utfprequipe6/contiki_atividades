/*
 * Copyright (c) 2010, Loughborough University - Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *         Example to demonstrate-test cc2530 sensor functionality
 *
 *         B1 turns LED_GREEN on and off.
 *
 *         The node takes readings from the various sensors every x seconds and
 *         prints out the results.
 *
 *         We use floats here to translate the AD conversion results to
 *         meaningful values. However, our printf does not have %f support so
 *         we use an ugly hack to print out the value by extracting the integral
 *         part and then the fractional part. Don't try this at home.
 *
 *         Temperature:
 *           Math is correct, the sensor needs calibration per device.
 *           I currently use default values for the math which may result in
 *           very incorrect values in degrees C.
 *           See TI Design Note DN102 about the offset calibration.
 *
 *         Supply Voltage (VDD):
 *           For VDD, math is correct, conversion is correct.
 *           See DN101 for details.
 *
 *         Make sure you enable/disable things in contiki-conf.h
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */

#include "contiki.h"
#include "contiki-conf.h"
#include "dev/leds.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include "dev/button-sensor.h"
#include "dev/bmp280-sensor.h"
#include <stdio.h>


#define PRINTF(...) printf(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
PROCESS(sensors_test_process, "Sensor Test Process");

AUTOSTART_PROCESSES(&sensors_test_process);


/*---------------------------------------------------------------------------*/

uint16_t temperatura;
uint32_t pressao;
uint16_t pressaoA;
uint16_t pressaoB;

uip_ip6addr_t endereco;
static struct uip_udp_conn *com;
uip_ip6addr_t endereco2;
static struct uip_udp_conn *com2;

static uint32_t buffer[40];

//char * dados = (( char *) buffer);

PROCESS_THREAD(sensors_test_process, ev, data)
{
  static struct etimer et;

  /* Sensor Values */
 // static int rv;
  static struct sensors_sensor *sensor;


  PROCESS_BEGIN();

  PRINTF("========================\n");
  PRINTF("Iniciando sensor BMP\n");
  PRINTF("========================\n");

  /* Set an etimer. We take sensor readings when it expires and reset it. */
  etimer_set(&et, CLOCK_SECOND * 1);
  //bbbb::4820:3d53:b336:e3ee
  //bbbb::816a:a544:ab53:8642
  //uip_ip6addr(&endereco,0xbbbb,0,0,0,0xb547,0x88b9,0x2bc3,0x3156);
  uip_ip6addr(&endereco,0xbbbb,0,0,0,0x7194,0x785c,0x37ab,0xda6c);
  // bbbb::d8ef:3b6b:3d54:8c76
  uip_ip6addr(&endereco2,0xbbbb,0,0,0,0x64d0,0x8c6a,0xa56b,0x0d8a);

  com = udp_new(&endereco,UIP_HTONS(8802),NULL);
  com2 = udp_new(&endereco2,UIP_HTONS(8802),NULL);
  while(1)
  {

    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    sensor = sensors_find("BMP");


    if(sensor)
    {
      PRINTF("evento de medicao...\n");
      leds_on(LEDS_RED|LEDS_GREEN);

      temperatura = sensor->value(BMP_SENSOR_TYPE_TEMP);
      pressao = sensor ->value(BMP_SENSOR_TYPE_PRESS_hPa);
      pressaoA = sensor ->value(BMP_SENSOR_TYPE_LAST_READ_PRESS_Pa_LSB);
      pressaoB = sensor ->value(BMP_SENSOR_TYPE_LAST_READ_PRESS_Pa_MSB);
      pressao = pressaoA|(pressaoB<<16);
      /*dados[0] = 0x79;
      dados[1] = temperatura;
      dados[2] = pressao;
      uip_udp_packet_send(com,dados,12);*/
      buffer[0] = 0x79;
      buffer[1] = temperatura;
      buffer[2] = pressao;
      uip_udp_packet_send(com,buffer,12);
      uip_udp_packet_send(com2,buffer,12);

      PRINTF("Temperatura = %i, pressao = %ud\n", temperatura, pressao);
    }
    etimer_reset(&et);
  }
  PROCESS_END();
}

