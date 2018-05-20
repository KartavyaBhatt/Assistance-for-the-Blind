unsigned char mode;                                //For selecting mode number
unsigned char temp1;                        //For selecting direction
int time;
int N;
int r;


void usart_data_transmit(unsigned char a)
{
     while ( ! (UCSRA & (1 << UDRE)));
            UDR = a;
}

void usart_string_transmit(char *string)
{
        //take string as pointer and transmit each byte
        while(*string)
        {
                usart_data_transmit(*string++);
        }
        
        while ( ! (UCSRA & (1 << UDRE)));
}

unsigned char usart_data_receive()
{
      unsigned char c;

     while(!(UCSRA & (1 << RXC)));
     c = UDR;

     return c;
}

void distance_delay(int d)
{

/*
d is distance in meters
r is radius in cm
N is number of rotations
time is the time delay to generate to cover given distance considering 30RPM DC Motor
*/
     
     //r = 7;
     
     //N = (100 * d) / (6 * r);
     
     //time = 2000 * N;
     
      //delay_ms(time);
      
      delay_ms(12000);
}

 void main()
 {
      unsigned char c;
      DDRA = 0xFF;
      UCSRB = (1 << RXEN)| (1 << TXEN) ;
      UCSRC = (1 << UCSZ1 ) | (1 << UCSZ0 ) | (1 << URSEL);
      UBRRL = 0x33;
     // GICR |= (1<<INT0) | (1<<INT1) | (1<<INT2);
     // MCUCR = 0x0F;
      DDRB = 0x00;

      usart_string_transmit("Group 20");
      usart_data_transmit(0x0d);
      usart_data_transmit(0x0a);

      usart_string_transmit("1. Robotic Toy");

      //Transmits Carriage return to Bluetooth Module
      usart_data_transmit(0x0d);

      //Transmits New Line to Bluetooth Module for new line
      usart_data_transmit(0x0a);

      usart_string_transmit("2. White Cane");

      //Transmits Carriage return to Bluetooth Module
      usart_data_transmit(0x0d);

      //Transmits New Line to Bluetooth Module for new line
      usart_data_transmit(0x0a);


      //Get mode for robot to use
      mode = usart_data_receive();
        
        if(mode == '1')
        {
                usart_string_transmit("Toy Mode Active");

                //Transmits Carriage return to Bluetooth Module
                usart_data_transmit(0x0d);

                //Transmits New Line to Bluetooth Module for new line
                usart_data_transmit(0x0a);

                usart_string_transmit("                       Controls");

                //Transmits Carriage return to Bluetooth Module
                usart_data_transmit(0x0d);

                //Transmits New Line to Bluetooth Module for new line
                usart_data_transmit(0x0a);

                usart_string_transmit("F : Forward");

                //Transmits Carriage return to Bluetooth Module
                usart_data_transmit(0x0d);

                //Transmits New Line to Bluetooth Module for new line
                usart_data_transmit(0x0a);

                usart_string_transmit("B - Backward");

                //Transmits Carriage return to Bluetooth Module
                usart_data_transmit(0x0d);

                //Transmits New Line to Bluetooth Module for new line
                usart_data_transmit(0x0a);

                usart_string_transmit("L - Left Turn");

                //Transmits Carriage return to Bluetooth Module
                usart_data_transmit(0x0d);

                //Transmits New Line to Bluetooth Module for new line
                usart_data_transmit(0x0a);

                usart_string_transmit("R - Right Turn");

                //Transmits Carriage return to Bluetooth Module
                usart_data_transmit(0x0d);

                //Transmits New Line to Bluetooth Module for new line
                usart_data_transmit(0x0a);

        }
        
//------------------------------------------Executing Commands given by the user---------------------------------------------------------------

       while(1)
        {
                //Toy Mode

                if (mode == '1')
                {
                        temp1 = usart_data_receive();

                        usart_string_transmit("Your control is ");

                        usart_data_transmit(temp1);

                        //Transmits Carriage return to Bluetooth Module
                        usart_data_transmit(0x0d);

                        //Transmits New Line to Bluetooth Module for new line
                        usart_data_transmit(0x0a);

                        if (temp1 == 'F')
                        {
                                PORTA = 0x05;
                                usart_string_transmit("Forward command received");
                        }

                        else if(temp1 == 'B')
                        {

                                   PORTA = 0X0A;
                                   usart_string_transmit("Backword command received");
                        }

                        else if(temp1 == 'L')
                        {
                                usart_string_transmit("Left command received");
                                PORTA = 0X06;
                                delay_ms(1000);
                                PORTA = 0X05;
                        }

                        else if(temp1 == 'R')
                        {
                                usart_string_transmit("Right command received");
                                PORTA = 0X09;
                                delay_ms(1000);
                                PORTA = 0X05;
                        }
                        
                        else
                        {
                            break;
                        }
                }

                //Toy Mode Ends


//-----------------------------------------------------------------------------------------------------------------------------------------------------------


                //White Cane Mode
                else
                {
                   usart_string_transmit("Blind mode");
                   
                   PORTA = 0x05;
                   
                   while(! (UCSRA & (1 << UDRE)) )
                   {
                           if(PINB.B4)            //For sensor facing down at front side of the robot
                           {
                                      usart_string_transmit("Stairs Detected");
                                      PORTA = 0x00;                 //Stop the robot and notify the user
                           }
                           
                           if( !(PINB.B0) )            //For sensor facing legs of the human
                           {
                               usart_string_transmit("You are unsafe");
                               PORTA = 0x00;              //Stop the robot and notify user
                           }
                           
                           if(PINB.B1)                    //For sensor in front side of robot
                           {
                                      usart_string_transmit("Obstacle in front");
                                      PORTA = 0x00;                   //Stop the car and notify user
                           }
                           
                           if(PINB.B2)        //For sensor in left side of robot
                           {
                                              usart_string_transmit("Obstacle on left");
                                              PORTA = 0x00;                   //Stop the car and notify user
                           }
                           
                           if(PINB.B3)        //For sensor in right side of robot
                           {
                                              usart_string_transmit("Obstacle on right");
                                              PORTA = 0x00;                   //Stop the car and notify user
                           }
                   }
                }
        }
}