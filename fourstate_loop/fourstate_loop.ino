#define PIN_A 2
#define PIN_B 3
#define PIN_P 4

uint8_t lrmem;
uint8_t z;
int num = 0;

int8_t rotary()
{
  static int lrsum = 0;
  static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};
  int8_t l, r;

  l = digitalRead(PIN_A);
  r = digitalRead(PIN_B);

  lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;
  lrsum = lrsum + TRANS[lrmem];
  /* encoder not in the neutral state */
  if(lrsum % 4 != 0) return(0);
  /* encoder in the neutral state */
  if (lrsum == 4)
  {
    lrsum=0;
    return(1);
  }
  if (lrsum == -4)
  {
    lrsum=0;
    return(-1);
  }
  /* lrsum > 0 if the impossible transition */
  lrsum=0;
  return(0);
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
}

void loop()
{
  int8_t res;
   
  res = rotary();
  if (res!=0)
  {
    num = num + res;
    Serial.println(num);
  }
  if (digitalRead(PIN_P) == !z)
  {
    Serial.println(num);
    delay(250);
  }
}