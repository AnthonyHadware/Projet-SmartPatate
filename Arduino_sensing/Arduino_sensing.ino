//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;
int amp;
int nbdoigts;
const int ledverte = 7;
const int ledorange = 4;
const int ledrouge = 2;
const int ledactivite = 8;
int analogPin =  0;
   

void setup()
{
  
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(115200);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+
    
  pinMode(ledverte, OUTPUT);
  pinMode(ledorange, OUTPUT);
  pinMode(ledrouge, OUTPUT);
  pinMode(ledactivite, OUTPUT);
}

void loop()
{
  unsigned int d;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
   
    freq[d] = d;

 //   plot(v,0);              //-Display
 //   plot(results[d],1);
  // delayMicroseconds(1);
  }

PlottArray(1,freq,results); 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
  
  amp = analogRead(analogPin);  //lecture de la valeur max de l'amplitude
  

   if (amp< 500 && amp< 700)  //définition de l'intervalle
    {nbdoigts=0;}
   if (amp> 215 && amp< 240)
    {nbdoigts=1;}
   if (amp> 241 && amp< 260)
    {nbdoigts=2;}
   if (amp> 261&& amp< 500)
    {nbdoigts=3;}

 switch (nbdoigts)
 {
  case 1:
    {
      digitalWrite (ledverte,HIGH);
      digitalWrite (ledorange,LOW);
      digitalWrite (ledrouge,LOW);
      break;
    }
  case 2:
    {
      digitalWrite (ledverte,HIGH);
      digitalWrite (ledorange,HIGH);
      digitalWrite (ledrouge,LOW);      
      break;
    }
  case 3:
    {
      digitalWrite (ledverte,HIGH);
      digitalWrite (ledorange,HIGH);
      digitalWrite (ledrouge,HIGH);      
      break;
    }
  default:
    {
      digitalWrite (ledverte,LOW);
      digitalWrite (ledorange,LOW);
      digitalWrite (ledrouge,LOW);     
      break;
    }
}   
}
   
