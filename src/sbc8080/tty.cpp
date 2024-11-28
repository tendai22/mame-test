#include <cstdio>
#include <termios.h>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <inttypes.h>
#include "tty.h"

//
// serial device upper layer
//

// upper layer interface

// device_reset
void tty::device_reset(void)
{
	input_device_reset();
	output_device_reset();
}

// update_status
void tty::device_update(uint8_t state)
{
	static int count = 0;

	if (count++ > 1000) {
		count = 0;
		fprintf(stderr, ".");
	}
	update_user_input();
	input_device_update();
	output_device_update();
}


#define ASCIIART

#ifdef ASCIIART
static int file_flag = 1;
static FILE *fp = NULL;
static const char *filename = "ASCIIART.BAS";
#endif

/* Implementation for the input device */
void tty::input_device_reset(void)
{
	changemode(1);
	// I have tried (and failed) to flush pending input by doing
	//  while (kbhit())
	//     osd_get_char();
	// but it did not work. (One character remains in input buffer, 
	// by typing second char, it returns 1st char)
	// I don't know why it did not work, but I recognize omitting this code
	// make it works.
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	input_device_ready = 0;
	tty_irq_cb(IRQ_INPUT_DEVICE, 0);
	tty_irq_cb(IRQ_OUTPUT_DEVICE, 0);

}



void tty::reset_asciiart_input(void)
{
#ifdef ASCIIART
    // startup key-in from ASCIIART.BAS
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "%s cannot open\n", filename);
    }
#endif
}

void tty::input_device_restore(void)
{
	changemode(0);
}

void tty::input_device_update(void)
{
	if (input_device_ready) {
		//int_controller_set(IRQ_INPUT_DEVICE);
		tty_irq_cb(IRQ_INPUT_DEVICE, 1);
	}
}

int tty::input_device_ack(void)
{
	//return M68K_INT_ACK_AUTOVECTOR;
  	return 0;
}

uint8_t tty::input_device_status(void)
{
	uint8_t c = 0;
	if (input_device_ready)
		c |= 1;
	if (output_device_empty)
		c |= 2;
	return c;
}

uint8_t tty::input_device_read(void)
{
	int value;
	//printf("[");
	value = input_device_value;
	// emulate uart_dreg is read.
	//int_controller_clear(IRQ_INPUT_DEVICE);
	tty_irq_cb(IRQ_INPUT_DEVICE, 0);
	input_device_ready = 0;
	//printf("%02X]", value);
	return value;
}

void tty::input_device_write(unsigned int value)
{
	// do nothing
	(void)value;
}

//
// get_msec ... with clock_gettime, a new POSIC standard
//
long int tty::get_msec(void)
{
	struct timespec ts;
	static unsigned long int start = 0, current;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	current = (unsigned long int)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
    current *= 10;
	if (start == 0) {
		start = current;
		fprintf(stderr, "get_msec: start = %ld", start);
	}
	fprintf(stderr, "<%ld>", current - start);
	return current - start;
}

/* Implementation for the output device */
void tty::output_device_reset(void)
{
	output_device_last_output = get_msec();
	output_device_data_ready = 0;
	output_device_empty = 1;
	//int_controller_clear(IRQ_OUTPUT_DEVICE);
	tty_irq_cb(IRQ_OUTPUT_DEVICE, 0);
}

void tty::output_device_update(void)
{
	if(output_device_empty)		// empty check if any data is pending
	{
		if (output_device_data_ready)	// there is a data to be sent in output_device_data
		{
			printf("%c", output_device_data);
			output_device_data_ready = 0;
			output_device_last_output = get_msec();
			output_device_empty = 0;
			//int_controller_clear(IRQ_OUTPUT_DEVICE);
			tty_irq_cb(IRQ_OUTPUT_DEVICE, 0);
		}
	} else {	// not empty, now a data is transmitting
		if((get_msec() - output_device_last_output) >= OUTPUT_DEVICE_PERIOD)
		{
			output_device_empty = 1;
			output_device_data_ready = 0;
			fprintf(stderr, "**");
			//int_controller_set(IRQ_OUTPUT_DEVICE);
			tty_irq_cb(IRQ_OUTPUT_DEVICE, 1);
		}
	}
}

int tty::output_device_ack(void)
{
	//return M68K_INT_ACK_AUTOVECTOR;
  return 0;
}

unsigned int tty::output_device_read(void)
{
	//int_controller_clear(IRQ_OUTPUT_DEVICE);
	tty_irq_cb(IRQ_OUTPUT_DEVICE, 0);
	return 0;
}

void tty::output_device_write(uint8_t value)
{
	output_device_data_ready = 1;
	output_device_data = value & 0xff;
	fprintf(stderr, "[[%02x]]", output_device_data);
	if (output_device_empty)
	{
		// send it out to lower physical layer
		// it should be here also, so that short-time consequent output_device_write calling
		// should not overwritten the first output character.
		printf("{%02x}", output_device_data);
		output_device_data_ready = 0;
		output_device_last_output = get_msec();
		output_device_empty = 0;
		//int_controller_clear(IRQ_OUTPUT_DEVICE);
		tty_irq_cb(IRQ_OUTPUT_DEVICE, 0);
	}
}

//
//
//
/* Parse user input and update any devices that need user input */
void tty::update_user_input(void)
{
	static int last_ch = -1;
	int ch = 0;

	if (input_device_ready || !kbhit())
		return;
#if 0
	while (kbhit()) {
		ch = osd_get_char();
		//printf("=%02X=", ch&0xff);
    }
#endif
    ch = tty_get_char();
	fprintf(stderr, "[%02x]\n", ch);

    switch(ch)
	{
	    case 0x1b:
			quit = 1;
			break;
#ifdef ASCIIART
        case 0x0f:
            reset_asciiart_input();
            break;
#endif
		case 0x0e:
			
		case '~':
			if(last_ch != ch)
				nmi = 1;
			break;
		default:
			input_device_ready = 1;
			input_device_value = ch;
	}
	//printf("(%02X)", ch);
    last_ch = ch;
}


//
// Linux tty driver interface
//

void tty::changemode(int dir)
{
  static struct termios oldt, newt;

  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_iflag &= ~( IGNCR | ICRNL );
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

int tty::kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;
#ifdef ASCIIART
    // redirect input
    if (file_flag && fp) {
        //fprintf(stderr, "(%d)", 1);
        return 1;
    }
#endif
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    //printf("%d", f);fflush(stdout);
    return FD_ISSET(STDIN_FILENO, &rdfs);

}

int tty::tty_get_char() {
    int ch;
    struct timespec ts;

#ifdef ASCIIART
    // redirected input
    if (file_flag && fp) {
        ch = fgetc(fp);
        if (ch != EOF) {
            printf("%c", ch);
            return ch;
        }
        fclose(fp);
        fp = NULL;
        file_flag = 0;
        // falling down
    }
#endif

    ts.tv_sec = 0;
    ts.tv_nsec = 100000;  // 1 millisec
    while (!kbhit()) {
        nanosleep(&ts, &ts);
    }
  	ch = getchar();
    if (ch == 0x7f)
        ch = 0x08;
    //if (ch < 0x20 || ch >= 0x7f)
    //    fprintf(stderr, "{%02x}", ch);
    return ch;
}

