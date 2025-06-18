#pragma once

#include "PString.h"

const char * page_person ();
const char * page_blink_control ();
const char * page_main ();

uint32_t get_blinky_yellow_delay();
void set_blinky_yellow_delay(uint32_t msDelay);

uint32_t get_blinky_red_delay();
void set_blinky_red_delay(uint32_t msDelay);

//void send_http_404_html(ptools::PWriter & out, const ptools::PString & sPath);
void send_http_404_html(ptools::PWriter & s, const char * pStrFile);
