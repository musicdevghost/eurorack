/**
  ******************************************************************************
  * @file    i2c.h
  * @author  Baoshi <mail(at)ba0sh1(dot)com>
  * @version 0.1
  * @date    Dec 24, 2014
  * @brief   Bit-bang I2C interface for ESP8266. For pin selection, edit
  *          the configuration block in i2c.c
  *
  ******************************************************************************
  * @copyright
  *
  * Copyright (c) 2015, Baoshi Zhu. All rights reserved.
  * Use of this source code is governed by a BSD-style license that can be
  * found in the LICENSE.txt file.
  *
  * THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED
  * WARRANTY.  IN NO EVENT WILL THE AUTHOR(S) BE HELD LIABLE FOR ANY DAMAGES
  * ARISING FROM THE USE OF THIS SOFTWARE.
  *
  */

#ifndef I2C_H_
#define I2C_H_

#include "stdbool.h"
#include "stdint.h"

/**
 * @brief   Initialize I2C GPIO pins
 */
void i2c_init(uint8_t scl_pin, uint8_t sda_pin);

/**
 * @brief   Send I2C start bit
 * @return  true if start successfully. Otherwise the bus is busy
 */
bool i2c_start(void);

/**
 * @brief   Send I2C stop bit
 */
void i2c_stop(void);

/**
 * @brief    Send data to I2C bus
 * @param    data Data to send
 * @return   true if ACK is received at end of sending. False if not ACK'ed
 */
bool i2c_write(uint8_t data);

/**
 * @brief    Read data from I2C bus
 * @return   Data read
 */
uint8_t i2c_read(void);

/**
 * @brief   Send acknowledge bit (at end of reading)
 * @param   ack ACK (true) or NACK (false)
 */
void i2c_set_ack(bool ack);


#endif
