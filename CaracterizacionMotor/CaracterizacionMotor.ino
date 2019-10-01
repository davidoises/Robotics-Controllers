uint8_t CANAL_A = 2;//D5;
uint8_t CANAL_B = 3;//D6;
uint8_t IN2 = 5; //D2; 
uint8_t IN1 = 6; //D3;

//double deg_to_pwm = 1023.0/360.0;
double deg_to_pwm = 255.0/360.0;
double count_to_deg = 360.0/(20.4 * 48.0);
unsigned long prev_time = 0;

long count = 0;
boolean A,B;
byte state, statep;

double setpoint = 90;

//void ICACHE_RAM_ATTR handleInterrupt();
//void ICACHE_RAM_ATTR handleInterrupt_2();
void handleInterrupt();
void handleInterrupt_2();
void test_motor(int pwm, int lapse);

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
} 

void loop()
{

  //digitalWrite(IN2, HIGH);
  //digitalWrite(IN1, LOW);
    
  // CHARACTERIZATION LOOP
  // 1. 150 degs
  int deg = 150;
  //int deg = -150;
  double pulse_length = 0.5;
  double res = .05;
  test_motor(deg, pulse_length, res);

  deg = 0;
  pulse_length = 0.75;
  test_motor(deg, pulse_length, res);
  
  deg = 250;
  //deg = -250;
  pulse_length = 0.25;
  test_motor(deg, pulse_length, res);

  deg = -350;
  //deg = 350;
  pulse_length = 0.4;
  test_motor(deg, pulse_length, res);

  deg = -250;
  //deg = 250;
  pulse_length = 0.15;
  test_motor(deg, pulse_length, res);

  deg = 50;
  //deg = -50;
  pulse_length = 1.1;
  test_motor(deg, pulse_length, res);

  deg = -70;
  pulse_length = 0.5;
  test_motor(deg, pulse_length, res);

  deg = 120;
  //deg = -120;
  pulse_length = 0.75;
  test_motor(deg, pulse_length, res);

  deg = -360;
  pulse_length = 1.25;
  test_motor(deg, pulse_length, res);

  deg = 160;
  //deg = -60;
  pulse_length = 1;
  test_motor(deg, pulse_length, res);

  deg = -40;
  //deg = -60;
  pulse_length = 0.85;
  test_motor(deg, pulse_length, res);

  deg = 55;
  //deg = -60;
  pulse_length = 0.3;
  test_motor(deg, pulse_length, res);

  deg = 210;
  //deg = -60;
  pulse_length = 0.9;
  test_motor(deg, pulse_length, res);

  deg = -80;
  //deg = -60;
  pulse_length = 1.1;
  test_motor(deg, pulse_length, res);

  deg = -190;
  //deg = -60;
  pulse_length = 0.65;
  test_motor(deg, pulse_length, res);

  deg = -250;
  //deg = -60;
  pulse_length = 0.35;
  test_motor(deg, pulse_length, res);

  deg = 155;
  //deg = -60;
  pulse_length = 1;
  test_motor(deg, pulse_length, res);  

  deg = 220;
  //deg = -60;
  pulse_length = 0.49;
  test_motor(deg, pulse_length, res);  

  deg = 330;
  //deg = -60;
  pulse_length = 0.41;
  test_motor(deg, pulse_length, res);

  deg = -145;
  //deg = -60;
  pulse_length = 0.2;
  test_motor(deg, pulse_length, res);  

  deg = -60;
  //deg = -60;
  pulse_length = 0.7;
  test_motor(deg, pulse_length, res);  
  
  deg = 90;
  //deg = -60;
  pulse_length = 0.3;
  test_motor(deg, pulse_length, res);

  deg = -90;
  //deg = -60;
  pulse_length = 0.15;
  test_motor(deg, pulse_length, res);   

  deg = 280;
  //deg = -60;
  pulse_length = 0.4;
  test_motor(deg, pulse_length, res);   

  deg = -50;
  //deg = -60;
  pulse_length = 0.8;
  test_motor(deg, pulse_length, res);
  
  deg = -240;
  //deg = -60;
  pulse_length = 0.25 ;
  test_motor(deg, pulse_length, res);

  deg = 80;
  //deg = -60;
  pulse_length = 1.2;
  test_motor(deg, pulse_length, res);

  deg = 350;
  //deg = -60;
  pulse_length = 0.75;
  test_motor(deg, pulse_length, res);
}


// pwm in percentage, lapse in seconds, res in seconds
void test_motor(int pwm, double lapse, double res)
{
  uint8_t IN = 0;
  if(pwm < 0)
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
  

  //count = 0;
  
  for(double i = 0; i < lapse; i+=res)
  {
    //double t = pwm*deg_to_pwm;
    double pwm2 = map(abs(pwm), 0.0, 360, 28.3, 255);
    analogWrite(IN, pwm2);
    delay(res*1000);
    Serial.print(pwm2);
    Serial.print(" ");
    Serial.print(count*count_to_deg);
    Serial.print(" ");
    unsigned long current_time = millis(); 
    Serial.println(current_time - prev_time);
    prev_time = current_time;
  }
}

void handleInterrupt_2() {
  count++;
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
