#include <stdio.h>		// printf
#include <termios.h>
#include <unistd.h>

#include "com.h"

COM::COM()
{

}

int COM::COMOpen(char *port, int baud)
{
    if(PortOpen(port) > 0){
        PortSet(baud, 8, 0, 0);
    }else{
        return -1;
    }
    return 0;
}

COM::~COM()
{

}

int COM::convbaud(unsigned long int baudrate)
{
	switch (baudrate)
	{
		case 2400:
			return B2400;
		case 4800:
			return B4800;
		case 9600:
			return B9600;
		case 19200:
			return B19200;
		case 38400:
			return B38400;
		case 57600:
			return B57600;
		case 115200:
			return B115200;
		default:
			return B9600;
	}
}

/*******************************************
 *	Setup comm attr
 *	fdcom:
 *
 ********************************************/
int COM::PortSet(int baud, int bits, int par, int stop_bit)
{
	struct termios termios_old, termios_new;
	int baudrate, tmp;
	char databit, stopbit, parity;

	bzero(&termios_old, sizeof(termios_old));
	bzero(&termios_new, sizeof(termios_new));
	cfmakeraw(&termios_new);
	tcgetattr(fdcom, &termios_old);	// get the serial port
	// attributions
	// baudrates
	baudrate = convbaud(baud);
	cfsetispeed(&termios_new, baudrate);
	cfsetospeed(&termios_new, baudrate);
	termios_new.c_cflag |= (CLOCAL | CREAD);
	termios_new.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/*
	 * fctl = pportinfo-> fctl; switch(fctl){ case '0':{
	 * termios_new.c_cflag &= ~CRTSCTS; //no flow control }break; case 
	 * '1':{ termios_new.c_cflag |= CRTSCTS; //hardware flow control
	 * }break; case '2':{ termios_new.c_iflag |= IXON | IXOFF |IXANY;
	 * //software flow control }break; } 
	 */
	termios_new.c_cflag &= ~CSIZE;
	databit = bits;
	switch (databit)
	{
		case '5':
			termios_new.c_cflag |= CS5;
		case '6':
			termios_new.c_cflag |= CS6;
		case '7':
			termios_new.c_cflag |= CS7;
		default:
			termios_new.c_cflag |= CS8;
	}

	parity = 0;
	switch (parity)
	{
		case '0':
			{
				termios_new.c_cflag &= ~PARENB;	// no parity check
			}
			break;
		case '1':
			{
				termios_new.c_cflag |= PARENB;	// odd check
				termios_new.c_cflag |= PARODD;
			}
			break;
		case '2':
			{
				termios_new.c_cflag |= PARENB;	// even check
				termios_new.c_cflag &= PARODD;
			}
			break;
		case '3':
			{
				termios_new.c_cflag = ~PARENB;	// space check
			}
	}

	stopbit = '1';
	if (stopbit == '2')
	{
		termios_new.c_cflag |= CSTOPB;	// 2 stop bits
	} else
	{
		termios_new.c_cflag &= ~CSTOPB;	// 1 stop bits
	}

	// other attributions default

	termios_new.c_oflag &= ~OPOST;
	termios_new.c_oflag &= ~(ONLCR | ICRNL);
	termios_new.c_iflag &= ~(ICRNL | INLCR);
	termios_new.c_iflag &= ~(IXON | IXOFF | IXANY);

	termios_new.c_cc[VMIN] = 0;
	termios_new.c_cc[VTIME] = 0;

	tcflush(fdcom, TCIFLUSH);
	tmp = tcsetattr(fdcom, TCSANOW, &termios_new);
	return (tmp);
}

/*******************************************
 *	Open serial port
 *	tty: 
 ********************************************/
int COM::PortOpen(char *port)
{
	fdcom = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
	return (fdcom);
}

/*******************************************
 *	Close serial port
 ********************************************/
void COM::PortClose()
{
	close(fdcom);
}

int COM::COMRead(char *pBuf, int nLength) 
{
    timeval m_tv;
    int i = 0;
    int retLength = 0;

    FD_ZERO(&m_rdset);
    FD_SET(fdcom, &m_rdset);

    m_tv.tv_sec = 1;
    m_tv.tv_usec = 0;

    if (select(1+fdcom, &m_rdset, NULL, NULL, &m_tv) > 0) {
            if (FD_ISSET(fdcom, &m_rdset)) {
                    while(nLength - retLength){
                          i = read(fdcom, pBuf+retLength, nLength-retLength);
                          if(i == 0)continue;
                          if(i < 0)break;
                          retLength += i;
                          if(retLength >= nLength) break;
                    }
            }
    }
    return retLength;
}

int COM::COMWrite(char *pBuf, int nLength) 
{
    int len = write(fdcom, pBuf, nLength);
    return len;
}

