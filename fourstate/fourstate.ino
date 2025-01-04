#define PIN_A 2
#define PIN_B 3
#define PIN_P 4  

uint8_t lrmem;
uint8_t z;
int num = 0;

void rotary()
{
  static int lrsum = 0;
  static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};
  int8_t l, r;

  l = digitalRead(PIN_A);
  r = digitalRead(PIN_B);

  lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;
  lrsum = lrsum + TRANS[lrmem];
  /* encoder not in the neutral (detent) state */
  if (lrsum % 4 != 0) return;
  /* encoder in the neutral state - clockwise rotation */
  if (lrsum == 4)
  {
    lrsum = 0;
    num = num + 1;
    Serial.println(num);
    return;
  }
  /* encoder in the neutral state - anti-clockwise rotation */
  if (lrsum == -4)
  {
    lrsum = 0;
    num = num - 1;
    Serial.println(num);
    return;
  }
  /* lrsum > 0 if the impossible transition */
  lrsum = 0;
  return;
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
  lrmem = 2*l + r;

  Serial.begin(115200);
  Serial.println(lrmem, DEC);
  Serial.println(num, DEC);

  attachInterrupt(digitalPinToInterrupt(PIN_A), rotary, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_B), rotary, CHANGE);
}

void loop()
{
  if (digitalRead(PIN_P) == !z)
  {
    Serial.println(num);
    delay(250);
  }
}