//Linux programming:(serial port)

// C library headers

#include <stdio.h>

#include <string.h>
#include<stdlib.h>

// Linux headers

#include <fcntl.h> // Contains file controls like O_RDWR

#include <errno.h> // Error integer and strerror() function

#include <termios.h> // Contains POSIX terminal control definitions

#include <unistd.h> // write(), read(), close()

#include<time.h> //to take time from linux
#include<signal.h>
//void file_fun(char *);
int chang=0;
void my_isr(int n)
{
	chang=1;
	exit(0);
}
int main() 
{
	signal(SIGINT,my_isr);  //Handeling SIGINT Signal for normal termination
	FILE *f1,*f2;
	f1=fopen("data1","r");
	int c1=0,c2=0,c3=0,i,j;
	char ch;
	while((ch=fgetc(f1))!=-1)
	{
		c1++;
		if(ch=='\n')
		{
			if(c1>c2)
				c2=c1;
			c1=0;
			c3++;       //finding size of the file
		}
	}
	char **p=malloc(sizeof(char*)*c3);
	c2=c2+2;
	for(i=0;i<c3;i++)
		p[i]=malloc(sizeof(char)*c2);
	fseek(f1,0,SEEK_SET);
	i=0;
	while(fgets(p[i],c2,f1))
		i++;
	fseek(f1,0,SEEK_SET);
	char id[20],rfid_num[15],*ptr1,*ptr2;
	char log[c3];                 //take it as global than only values fill with zero's
	bzero(log,sizeof(log)); 


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
	while(1)
	{
		f2=fopen("data2","a");         //opening the file in the append mode
		int serial_port = open("/dev/ttyUSB0", O_RDWR);

		if(serial_port==-1){
			perror("open /dev/ttyUSB0:");
			return -1;
		}

		// Create new termios struct, we call it 'tty' for convention

		struct termios tty;

		// Read in existing settings, and handle any error

		if(tcgetattr(serial_port, &tty) != 0) {

			printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));

			return 1;

		}

		tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)

		tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)

		tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size

		tty.c_cflag |= CS8; // 8 bits per byte (most common)

		tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)

		tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

		tty.c_lflag &= ~ICANON;

		tty.c_lflag &= ~ECHO; // Disable echo

		tty.c_lflag &= ~ECHOE; // Disable erasure

		tty.c_lflag &= ~ECHONL; // Disable new-line echo

		tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

		tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

		tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

		tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)

		tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

		// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)

		// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

		tty.c_cc[VTIME] = 5; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.

		tty.c_cc[VMIN] = 50;//read max 50bytes from microcontroller

		// Set in/out baud rate to be 9600

		cfsetispeed(&tty, B9600);

		cfsetospeed(&tty, B9600);

		// Save tty settings, also checking for error

		if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {

			printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));

			return 1;

		}

		/*Send a string from linux to microcontroller*/	
		// Write to serial port

		//unsigned char msg[50] ={'v','e','c','t','o','r','\r','\n'};

		//write(serial_port, msg, sizeof(msg));

		usleep(500);

		// Allocate memory for read buffer, set size according to your needs

		char read_buf [256]={0};

		// Normally you wouldn't do this memset() call, but since we will just receive

		// ASCII data for this example, we'll set everything to 0 so we can

		// call printf() easily.

		memset(&read_buf, '\0', sizeof(read_buf));

		// Read bytes. The behaviour of read() (e.g. does it block?,

		// how long does it block for?) depends on the configuration

		// settings above, specifically VMIN and VTIME

		int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

		// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.

		if (num_bytes < 0) {

			printf("Error reading: %s", strerror(errno));

			return 1;

		}





		printf("%s\n",read_buf);   //printing the rfid number on screen
		read_buf[12]='\0';
		char buff[20];
		strcpy(buff,read_buf);

		for(i=0,j=0;i<c3;i++)   //until EOF
		{
			ptr1=strrchr(p[i],','); 
			ptr2=strchr(p[i],',');
			ptr1++;
			strcpy(rfid_num,ptr1);
			rfid_num[12]='\0';
			if(strcmp(rfid_num,buff)==0)
			{
				ptr1=ptr1-2;
				ptr2++;
				while(ptr2<=ptr1)
				{
					id[j]=*ptr2;
					j++;
					ptr2++;
				}
				id[j]='\0';
				if(log[i]==0)
				{
					strcat(id," IN-TIME  ");
					log[i]=1;
				}
				else
				{
					strcat(id," OUT-TIME ");
					log[i]=0;
				}

				time_t t1;
				time(&t1);
				strcat(id,ctime(&t1));
				printf("%s",id);
				fprintf(f2,"%s",id);
			}

		}
		fclose(f2);
		if(chang)
		{
			fclose(f1);
			break;
		}
		close(serial_port);
	}
}
