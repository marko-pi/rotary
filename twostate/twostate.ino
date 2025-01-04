#define PIN_A 2
#define PIN_B 3
#define PIN_P 4  

uint8_t z;
uint8_t neutral;  /* neutral state value, 0 (false) or 1 (true) */ 
bool lasta;       /* pin A jumped last in previous half cycle */
int num = 0;

void rotary_a_down()
{
  /* encoder in 00 state, pin A jumped down last */
  if(digitalRead(PIN_B)==0)
  {
    /* for neutral state 00, this is the end of the cycle, but register the counterclockwise rotation only if pin A jumped last in both half cycles */
    if (!neutral and lasta)
    {
      num = num - 1;
      Serial.println(num);
    }
    lasta = true;
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_up, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_up, RISING);
  }
  /* encoder in 01 state, waiting for B to jump down */
  else
  {
    detachInterrupt(digitalPinToInterrupt(PIN_A));
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_down, FALLING);
  }
}

void rotary_b_down()
{
  /* encoder in 00 state, pin B jumped down last */
  if(digitalRead(PIN_A)==0)
  {
    /* for neutral state 00, this is the end of the cycle, but register the clockwise rotation only if pin B jumped last in both half cycles */
    if (!neutral and !lasta)
    {
      num = num + 1;
      Serial.println(num);
    }
    lasta = false;
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_up, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_up, RISING);
  }
  /* encoder in 10 state, waiting for A to jump down */
  else
  {
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_down, FALLING);
    detachInterrupt(digitalPinToInterrupt(PIN_B));
  }
}

void rotary_a_up()
{
  /* encoder in 11 state, in A jumped up last */
  if(digitalRead(PIN_B)==1)
  {
    /* for neutral state 11, this is the end of the cycle, but register the counterclockwise rotation only if pin A jumped last in both half cycles */
    if (neutral and lasta)
    {
      num = num - 1;
      Serial.println(num);
    }
    lasta = true;
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_down, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_down, FALLING);
  }
  /* encoder in 10 state, waiting for B to jump up */
  else
  {
    detachInterrupt(digitalPinToInterrupt(PIN_A));
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_up, RISING);
  }
}

void rotary_b_up()
{
  /* encoder in 11 state, pin B jumped up last */
  if(digitalRead(PIN_A)==1)
  {
    /* for neutral state 11, this is the end of the cycle, but register the clockwise rotation only if pin B jumped last in both half cycles */
    if (neutral and !lasta)
    {
      num = num + 1;
      Serial.println(num);
    }
    lasta = false;
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_down, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_down, FALLING);
  }
  /* encoder in 01 state, waiting for A to jump up */
  else
  {
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_up, RISING);
    detachInterrupt(digitalPinToInterrupt(PIN_B));
  }
}

void setup()
{
  int8_t l, r;

  pinMode(PIN_A, INPUT);
  pinMode(PIN_B, INPUT);
  pinMode(PIN_P, INPUT);
  
  // uncomment to initiate state True (11)
  //pinMode(PIN_A, INPUT_PULLUP);
  //pinMode(PIN_B, INPUT_PULLUP);
  //pinMode(PIN_P, INPUT_PULLUP);

  // read initial states
  z = digitalRead(PIN_P);
  l = digitalRead(PIN_A);
  r = digitalRead(PIN_B);

  /* neutral state is 11 */
  if((l==1) and (r==1))
  {
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_down, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_down, FALLING);
    neutral = 1; /* true */
  }

  /* neutral state is 00 */
  if((l==0) and (r==0))
  {
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary_a_up, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary_b_up, RISING);
    neutral = 0; /* false */
  }

  Serial.begin(115200);
  Serial.println(num, DEC);
}

void loop()
{
  if (digitalRead(PIN_P) == !z)
  {
    Serial.println(num);
    delay(250);
  }
}