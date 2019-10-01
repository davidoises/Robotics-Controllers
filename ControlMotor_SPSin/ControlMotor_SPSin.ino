#define CANAL_A 2//D5;
#define CANAL_B 3//D6;
#define IN2 5 //D2; 
#define IN1 6 //D3;

//double deg_to_pwm = 1023.0/360.0;
double deg_to_pwm = 255.0/360.0;
//double count_to_deg = 360.0/(20.4 * 48.0);
//double count_to_deg = 360.0/(19 * 24.0);
double count_to_deg = 360.0/(9.7 * 48.0);

long count = 0;
boolean A,B;
byte state, statep;

double setpoint = -180;
unsigned long initial_time = 0;
double x = -10;
bool ascending = 1;

double error = 0;
double prev_integral = 0;
double prev_error = 0;
double pwm_control = 0;
unsigned long prev_time = 0;

//void ICACHE_RAM_ATTR handleInterrupt();
//void ICACHE_RAM_ATTR handleInterrupt_2();
void handleInterrupt();

void setup() { 
  //Serial.swap();
  Serial.begin(115200); 
  pinMode(CANAL_A, INPUT);
  pinMode(CANAL_B, INPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CANAL_A), handleInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CANAL_B), handleInterrupt, CHANGE);
  delay(2000);
  count = 0;

  delay(5000);

  //analogWriteFreq(500);
  prev_time = millis();
  setpoint = map(analogRead(A11), 0, 1023, -360, 360);
} 

void loop()
{

  unsigned long current_time = millis();
  double delta_time = ((double)current_time - (double)prev_time)/1000;
  error = ((double)count)*count_to_deg - setpoint;
  double integral = (double)delta_time *error + prev_integral;
  double diff = (error - prev_error)/((double)delta_time);

  // kp = .1; ki = .25; kd = 0;
  //kp = 1.51; ki = .7; kd = 0.05;
  double kp = 1.51;
  double ki = .7;
  double kd = 0.1;
  
  pwm_control = kp*error + ki*integral + kd*diff;
  prev_time = current_time;
  prev_error = error;
  prev_integral = integral;

  //int pwm = constrain(abs(pwm_control), 28.3, 255);
  int pwm = constrain(abs(pwm_control), 0, 360);
  if(pwm != 0)
  {
    pwm = map(pwm, 0, 360, 35, 255);
  }
  uint8_t IN = 0;
  if(pwm_control < 0)
  {
    IN = IN2;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else
  {
    IN = IN1;
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, HIGH);
  }

  analogWrite(IN, pwm);

  Serial.print(setpoint);
  Serial.print(" ");
  Serial.print(count*count_to_deg);
  Serial.print(" ");
  Serial.print(delta_time, 6);
  Serial.println(" -360 360");
  //Serial.println(count*count_to_deg);
  

  delay(0.5);
  if((millis() - initial_time) > 750)
  {
    initial_time = millis();
    setpoint = 4*180*asin(x/10)/3.1416;
    if(ascending)
    {
      x++;
      if(x>10)
      {
        ascending = 0;
      }
    }
    if(!ascending)
    {
      x--;
      if(x<-10)
      {
        ascending = 1;
      }
    }
  }
}

//This program get executed when interrupt is occures i.e.change of input state
void handleInterrupt() { 
    A = digitalRead(CANAL_A);
    B = digitalRead(CANAL_B);

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
