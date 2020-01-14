#include "math.h"

unsigned long timep, time, etime;
//// MOTOR 1 ////
double enA=11;
const int in1=5;
const int in2=4;
const int chA=2;
const int chB=3;

int count=0;//pulse from encoder
int angle=0; 

boolean A,B;
byte state, statep;
String set1;
int setpoint1;
double Kp=0.32; //By Zieger method
double Ki=0.10;
double Kd=0.30;

float last_error=0;
float curr_error=0;
float chg_error=0;
float total_error=0;
float pid_term=0;
float pid_term_scaled=0; //if total gain not in PWM range, scale it down so not > |255|


//// MOTOR 2 ////
double enB=12;
const int in1_2=7;
const int in2_2=6;
const int chA_2=18;
const int chB_2=19;

int count2=0;
int angle2=0; 
  
boolean C,D;
byte state2, statep2;
  
String set2;
int setpoint2;
double Kp2=0.32; //try & error but can be calculated
double Ki2=0.10;
double Kd2=0.30;
  
float last_error2=0;
float curr_error2=0;
float chg_error2=0;
float total_error2=0;
float pid_term2=0;
float pid_term_scaled2=0; //if total gain not in PWM range, scale it down so not > |255|

//// MOTOR 3 ////
double enC=8;
const int in1_3=10;
const int in2_3=9;
const int chA_3=20;
const int chB_3=21;

int count3 = 0;
int angle3 = 0; 

boolean E,F;
byte state3, statep3;

String set3;
int setpoint3;
double Kp3=0.32; //try & error but can be calculated
double Ki3=0.10;
double Kd3=0.30;

float last_error3=0;
float curr_error3=0;
float chg_error3=0;
float total_error3=0;
float pid_term3=0;
float pid_term_scaled3=0; //if total gain not in PWM range, scale it down so not > |255|

int d1;
int d2;
int d3;
int px;
int py;
int pz;

void setup() {
  Serial.begin(115200);

  //// MOTOR 1 ////
  pinMode(chA,INPUT_PULLUP);
  pinMode(chB,INPUT_PULLUP);
  attachInterrupt(0,changeA,CHANGE);//interrupt pin with interrupt vector of change mode
  attachInterrupt(1,changeB,CHANGE);
  
  pinMode(enA,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  digitalWrite(chA,HIGH); //turn on pull up resistor
  digitalWrite(chB,HIGH);

   //// MOTOR 2 ////
  pinMode(chA_2,INPUT_PULLUP);
  pinMode(chB_2,INPUT_PULLUP);
  attachInterrupt(4,changeC,CHANGE);
  attachInterrupt(5,changeD,CHANGE);
  
  pinMode(enB,OUTPUT);
  pinMode(in1_2,OUTPUT);
  pinMode(in2_2,OUTPUT);
  digitalWrite(chA_2,HIGH); //turn on pull up resistor
  digitalWrite(chB_2,HIGH);

  //// MOTOR 3 ////
  pinMode(chA_3,INPUT_PULLUP);
  pinMode(chB_3,INPUT_PULLUP);
  attachInterrupt(2,changeE,CHANGE);
  attachInterrupt(3,changeF,CHANGE);
  
  pinMode(enC,OUTPUT);
  pinMode(in1_3,OUTPUT);
  pinMode(in2_3,OUTPUT);
  digitalWrite(chA_3,HIGH); //turn on pull up resistor
  digitalWrite(chB_3,HIGH);
  
  timep = millis(); 

}


void loop() {
coordinate();
PID_calc();
AngleCorrection();
PID_calc_2();
AngleCorrection_2();
PID_calc_3();
AngleCorrection_3();

printAngle();
analogWrite(enA,150);
analogWrite(enB,150);
analogWrite(enC,150);
}
void coordinate(){
 
  while(Serial.available()>0)
  {
    set1 = Serial.readStringUntil(',');
    set2 = Serial.readStringUntil(',');
    set3 = Serial.readStringUntil('\0');
  }
    setpoint1 = set1.toInt();
    setpoint2 = set2.toInt();
    setpoint3 = set3.toInt();

}

void PID_calc()
{
  angle = (count * 0.21);
  curr_error = setpoint1 - angle;//propotional

  chg_error = curr_error - last_error;   //derivative
  total_error += curr_error;   //integral
  pid_term = (Kp*curr_error)+(Ki*total_error)+(Kd*chg_error);   //total gain
  pid_term = constrain(pid_term, -255, 255);    //constrain to appropriate value
  pid_term_scaled = abs(pid_term);   //ensure is +ve value

  last_error = curr_error;
}

void PID_calc_2()
{
  angle2 = (count2 * 0.21);
  curr_error2 = setpoint2 - angle2;//propotional

  chg_error2 = curr_error2 - last_error2;   //derivative
  total_error2 += curr_error2;   //integral 
  pid_term2 = (Kp2*curr_error2)+(Ki2*total_error2)+(Kd2*chg_error2);   //total gain
  pid_term2 = constrain(pid_term2, -255, 255);    //constrain to appropriate value
  pid_term_scaled2 = abs(pid_term2);   //ensure is +ve value

  last_error2 = curr_error2;
}

void PID_calc_3()
{
  angle3 = (count3 * 0.21);
  curr_error3 = setpoint3 - angle3;//propotional

  chg_error3 = curr_error3 - last_error3;   //derivative
  total_error3 += curr_error3;   //integral 
  pid_term3 = (Kp3*curr_error3)+(Ki3*total_error3)+(Kd3*chg_error3);   //total gain
  pid_term3 = constrain(pid_term3, -255, 255);    //constrain to appropriate value
  pid_term_scaled3 = abs(pid_term3);   //ensure is +ve value
}
void AngleCorrection()
{
  if(angle < setpoint1)
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else if(angle > setpoint1)
  {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else
  {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,HIGH);
  }
}
void AngleCorrection_2()
{
  if(angle2 < setpoint2)
  {
    digitalWrite(in1_2,LOW);
    digitalWrite(in2_2,HIGH);
  }
  else if(angle2 > setpoint2)
  {
    digitalWrite(in1_2,HIGH);
    digitalWrite(in2_2,LOW);
  }
  else
  {
    digitalWrite(in1_2,HIGH);
    digitalWrite(in2_2,HIGH);
  }
}

void AngleCorrection_3()
{
  if(angle3 < setpoint3)
  {
    digitalWrite(in1_3,LOW);
    digitalWrite(in2_3,HIGH);
  }
  else if(angle3 > setpoint3)
  {
    digitalWrite(in1_3,HIGH);
    digitalWrite(in2_3,LOW);
  }
  else
  {
    digitalWrite(in1_3,HIGH);
    digitalWrite(in2_3,HIGH);
  }
}

void printAngle()
{
  time = millis();
  etime = time - timep;//current time-previous time
  
  if (etime > 1000) //if greater than 1000, print data to gui
  {
    Serial.print("C1 C2 C3 = ");
    Serial.print(count);
    Serial.print("  ");
    Serial.print(count2);
    Serial.print("  ");
    Serial.print(count3);
    Serial.print(" \n");
    Serial.print("A1 A2 A3 = ");
    Serial.print(angle);
    Serial.print("  ");
    Serial.print(angle2);
    Serial.print("  ");
    Serial.print(angle3);
    Serial.print(" \n");

    Serial.print("Px Py Pz = ");
    Serial.print(px);
    Serial.print("  ");
    Serial.print(py);
    Serial.print("  ");
    Serial.print(pz);
    Serial.print(" \n");
    
  Serial.print("\n");
  
    timep = time;  
  }
}
void changeA() 
{
  A = digitalRead(2);
  B = digitalRead(3);

  if ((A==HIGH)&&(B==HIGH)) state = 1;
  if ((A==HIGH)&&(B==LOW)) state = 2;
  if ((A==LOW)&&(B==LOW)) state = 3;
  if((A==LOW)&&(B==HIGH)) state = 4;
  
  switch (state)
  {
    case 1:
    {
      if (statep == 2) count++;
      if (statep == 4) count--;
      break;
    }
    case 2:
    {
      if (statep == 1) count--;
      if (statep == 3) count++;
      break;
    }
    case 3:
    {
      if (statep == 2) count --;
      if (statep == 4) count ++;
      break;
    }
    
    default:
    {
      if (statep == 1) count++;
      if (statep == 3) count--;
    }
  }
  statep = state;
}

void changeB()
{
  A = digitalRead(2);
  B = digitalRead(3);

  if ((A==HIGH)&&(B==HIGH)) state = 1;
  if ((A==HIGH)&&(B==LOW)) state = 2;
  if ((A==LOW)&&(B==LOW)) state = 3;
  if((A==LOW)&&(B==HIGH)) state = 4;
  
  switch (state)
  {
    case 1:
    {
      if (statep == 2) count++;
      if (statep == 4) count--;
      break;
    }
    case 2:
    {
      if (statep == 1) count--;
      if (statep == 3) count++;
      break;
    }
    case 3:
    {
      if (statep == 2) count --;
      if (statep == 4) count ++;
      break;
    }
    
    default:
    {
      if (statep == 1) count++;
      if (statep == 3) count--;
    }
  }
  statep = state;
}
void changeC()
{
  C = digitalRead(18);
  D = digitalRead(19);

  if ((C==HIGH)&&(D==HIGH)) state2 = 1;
  if ((C==HIGH)&&(D==LOW)) state2 = 2;
  if ((C==LOW)&&(D==LOW)) state2 = 3;
  if((C==LOW)&&(D==HIGH)) state2 = 4;
  
  switch (state2)
  {
    case 1:
    {
      if (statep2 == 2) count2++;
      if (statep2 == 4) count2--;
      break;
    }
    case 2:
    {
      if (statep2 == 1) count2--;
      if (statep2 == 3) count2++;
      break;
    }
    case 3:
    {
      if (statep2 == 2) count2 --;
      if (statep2 == 4) count2 ++;
      break;
    }
    default:
    {
      if (statep2 == 1) count2++;
      if (statep2 == 3) count2--;
    }
  }
  
  statep2 = state2;

}

void changeD()
{
  C = digitalRead(18);
  D = digitalRead(19);

  if ((C==HIGH)&&(D==HIGH)) state2 = 1;
  if ((C==HIGH)&&(D==LOW)) state2 = 2;
  if ((C==LOW)&&(D==LOW)) state2 = 3;
  if((C==LOW)&&(D==HIGH)) state2 = 4;
  
  switch (state2)
  {
    case 1:
    {
      if (statep2 == 2) count2++;
      if (statep2 == 4) count2--;
      break;
    }
    case 2:
    {
      if (statep2 == 1) count2--;
      if (statep2 == 3) count2++;
      break;
    }
    case 3:
    {
      if (statep2 == 2) count2 --;
      if (statep2 == 4) count2 ++;
      break;
    }
    default:
    {
      if (statep2 == 1) count2++;
      if (statep2 == 3) count2--;
    }
  }
  
  statep2 = state2;
  
}

void changeE()
{
  E = digitalRead(20);
  F = digitalRead(21);

  if ((E==HIGH)&&(F==HIGH)) state3 = 1;
  if ((E==HIGH)&&(F==LOW)) state3 = 2;
  if ((E==LOW)&&(F==LOW)) state3 = 3;
  if((E==LOW)&&(F==HIGH)) state3 = 4;
  
  switch (state3)
  {
    case 1:
    {
      if (statep3 == 2) count3++;
      if (statep3 == 4) count3--;
      break;
    }
    case 2:
    {
      if (statep3 == 1) count3--;
      if (statep3 == 3) count3++;
      break;
    }
    case 3:
    {
      if (statep3 == 2) count3 --;
      if (statep3 == 4) count3 ++;
      break;
    }
    default:
    {
      if (statep3 == 1) count3++;
      if (statep3 == 3) count3--;
    }
  }
  statep3 = state3;

}

void changeF()
{
  E = digitalRead(20);
  F = digitalRead(21);

  if ((E==HIGH)&&(F==HIGH)) state3 = 1;
  if ((E==HIGH)&&(F==LOW)) state3 = 2;
  if ((E==LOW)&&(F==LOW)) state3 = 3;
  if((E==LOW)&&(F==HIGH)) state3 = 4;
  
  switch (state3)
  {
    case 1:
    {
      if (statep3 == 2) count3++;
      if (statep3 == 4) count3--;
      break;
    }
    case 2:
    {
      if (statep3 == 1) count3--;
      if (statep3 == 3) count3++;
      break;
    }
    case 3:
    {
      if (statep3 == 2) count3 --;
      if (statep3 == 4) count3 ++;
      break;
    }
    default:
    {
      if (statep3 == 1) count3++;
      if (statep3 == 3) count3--;
    }
  }
  
  statep3 = state3;
}
