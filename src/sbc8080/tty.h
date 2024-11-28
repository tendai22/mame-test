#ifndef HEADER__TTY
#define HEADER__TTY

#include "emu.h"
#include <time.h>

/* Time between characters sent to output device (seconds) */
// changed as milliseconds about 10000bps serial speed
#define OUTPUT_DEVICE_PERIOD 1

class tty {
public:
    void device_reset(void);
    void device_update(uint8_t state);
    uint8_t input_device_read(void);
    uint8_t input_device_status(void);
    void output_device_write(uint8_t value);
    // set a callback function
    void set_irq_cb(void (*fptr)(offs_t offset, uint8_t value)) { tty_irq_cb = fptr; };

private:
    // input_device
    void input_device_reset(void);
    void input_device_update(void);
    int input_device_ack(void);
    void input_device_write(unsigned int value);
    void update_user_input(void);

    // output_device
    void output_device_reset(void);
    void output_device_update(void);
    int output_device_ack(void);
    unsigned int output_device_read(void);

private:
    // one byte read ahead and ungetc
    // we compose PIR9 (uart creg) with g_input_device_ready 
    // and g_output_device_ready
    int		input_device_value = -1;
    int		input_device_ready = 0;			/* Current status in input device */

    int		output_device_data_ready = 0;	/* 1 if g_output_device_data is valid, to be sent */
    int		output_device_data = 0xe5;		/* output data to be sent, 0xe5 has no means, magic number */
    int		output_device_empty = 1;		/* 1 if output queue is empty, ready to be written to DREG */
    time_t	output_device_last_output;		/* Time of last char output */
    int     quit = 0;
    int     nmi = 0;

    // callback vector
    // offset: 0: input irq assert/clear
    //         1: output irq assert/clear
    // value:  0: clear
    //         1: assert
    void (*tty_irq_cb)(offs_t offset, uint8_t value) = nullptr;
#define IRQ_INPUT_DEVICE 0
#define IRQ_OUTPUT_DEVICE 1

    //
    void reset_asciiart_input(void);
    void input_device_restore(void);

    // linux tty driver
    void changemode(int dir);
    int kbhit(void);
    int tty_get_char(void);
    // get_msec
    long int get_msec(void);
};


#endif /* HEADER__TTY */

