/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PiFaceWrapper.cpp
 * Author: julian
 * 
 * Created on 9. November 2018, 14:25
 */
#include "pifacedigital.h"
#include "PiFaceWrapper.h"
#include <mcp23s17.h>

int PiFaceWrapper::mcp23s17_fd; 
int PiFaceWrapper::pfd_count;
     
// Standard constructor, using which interrupts are disabled, hw adress 0 will be used.
PiFaceWrapper::PiFaceWrapper() {
    PiFaceWrapper(0, 0);
}

PiFaceWrapper::PiFaceWrapper(int _hw_addr, int _interrupts_enabled) {
    hw_addr     =   _hw_addr;
    file_handle =   pifacedigital_open(hw_addr);
    
    // Opening connection
    open();
    
}

PiFaceWrapper::PiFaceWrapper(const PiFaceWrapper& orig) {
}

PiFaceWrapper::~PiFaceWrapper() {
    close(hw_addr);
}

int PiFaceWrapper::open_noinit(uint8_t hw_addr)
{
    // if initialising the first PiFace Digital, open the fd
    if (pfd_count <= 0) {
        if ((mcp23s17_fd = mcp23s17_open(bus, chip_select)) < 0) {
            fprintf(stderr,
                    "pifacedigital_open_noinit: ERROR Could not open MCP23S17 "
                    "device.");
            return -1;
        }
    }
    pfd_count++;
    return  mcp23s17_fd; // returns the fd, for advanced users.
}

int PiFaceWrapper::open(){
      if ((mcp23s17_fd = pifacedigital_open_noinit(hw_addr)) < 0) {
        fprintf(stderr,
                "pifacedigital_open: ERROR Could not open MCP23S17 device.");
        return -1;
      }
      
       // set up config register
    const uint8_t ioconfig = BANK_OFF | \
                             INT_MIRROR_OFF | \
                             SEQOP_OFF | \
                             DISSLW_OFF | \
                             HAEN_ON | \
                             ODR_OFF | \
                             INTPOL_LOW;
    
    mcp23s17_write_reg(ioconfig, IOCON, hw_addr, mcp23s17_fd);

    // I/O direction
    mcp23s17_write_reg(0x00, IODIRA, hw_addr, mcp23s17_fd);
    mcp23s17_write_reg(0xff, IODIRB, hw_addr, mcp23s17_fd);

    // GPIOB pull ups
    mcp23s17_write_reg(0xff, GPPUB, hw_addr, mcp23s17_fd);

    // enable interrupts
    mcp23s17_write_reg(0xff, GPINTENB, hw_addr, mcp23s17_fd);

    return mcp23s17_fd; // returns the fd, for advanced users.
}

void PiFaceWrapper::close(uint8_t hw_addr)
{
    if (pfd_count <= 0)
        return;

    pfd_count--;

    // disable interrupts if enabled
    const uint8_t intenb = mcp23s17_read_reg(GPINTENB, hw_addr, mcp23s17_fd);
    if (intenb) {
        mcp23s17_write_reg(0, GPINTENB, hw_addr, mcp23s17_fd);
        // now do some other interrupt stuff...
        // TODO
    }

    // if no more PiFace Digital's, close the fd
    if (pfd_count <= 0) {
        pfd_count = 0;
        close(mcp23s17_fd);
    }
}


uint8_t PiFaceWrapper::read_reg(uint8_t reg, uint8_t hw_addr)
{
    return mcp23s17_read_reg(reg, hw_addr, mcp23s17_fd);
}

void PiFaceWrapper::write_reg(uint8_t data, uint8_t reg, uint8_t hw_addr)
{
    mcp23s17_write_reg(data, reg, hw_addr, mcp23s17_fd);
}

uint8_t PiFaceWrapper::read_bit(uint8_t bit_num, uint8_t reg, uint8_t hw_addr)
{
    return mcp23s17_read_bit(bit_num, reg, hw_addr, mcp23s17_fd);
}

void PiFaceWrapper::write_bit(uint8_t data,
                             uint8_t bit_num,
                             uint8_t reg,
                             uint8_t hw_addr)
{
    mcp23s17_write_bit(data, bit_num, reg, hw_addr, mcp23s17_fd);
}


uint8_t PiFaceWrapper::digital_read(uint8_t pin_num)
{
    return read_bit(pin_num, INPUT, hw_addr);
}

void PiFaceWrapper::digital_write(uint8_t pin_num, uint8_t value)
{
    write_bit(value, pin_num, OUTPUT, hw_addr);
}

int PiFaceWrapper::enable_interrupts()
{
   return mcp23s17_enable_interrupts();
}

int PiFaceWrapper::disable_interrupts()
{
   return mcp23s17_disable_interrupts();
}

int PiFaceWrapper::wait_for_input(uint8_t *data,
                                 int timeout,
                                 uint8_t hw_addr)
{
    // Flush any pending interrupts prior to wait
    read_reg(0x11, hw_addr);

    // Wait for input state change
    int ret = mcp23s17_wait_for_interrupt(timeout);

    // Read & return input register, thus clearing interrupt
    *data = pifacedigital_read_reg(0x11, hw_addr);
    return ret;
}

