#include <Keyboard.h>
int rowReset = 2;
int selectCol = 12;
int enableKeyboard = 4;
int currCol = 0;
int currRow = 0;
int debounceTime = 50;

struct key_state {
  const unsigned char key_code;
  char old = 0;
  char curr = 0;
  char debounce = 0;

  key_state(const unsigned char kc): key_code(kc) {}
};

key_state keys[]={
  KEY_F8,
  KEY_RETURN,
  '[',
  ']',
  KEY_RIGHT_ALT, //kana
  KEY_RIGHT_SHIFT,
  '`', //¥
  KEY_BACKSPACE, //STOP
  KEY_F7,
  '\\', //@
  '\'', //:
  ';',
  KEY_TAB, //_
  '/',
  '-',
  '=', //^
  KEY_F6,
  'o',
  'l',
  'k',
  '.',
  ',',
  'p',
  '0',
  KEY_F5,
  'i',
  'u',
  'j',
  'm',
  'n',
  '9',
  '8',
  KEY_F4,
  'y',
  'g',
  'h',
  'b',
  'v',
  '7',
  '6',
  KEY_F3,
  't',
  'r',
  'd',
  'f',
  'c',
  '5',
  '4',
  KEY_F2,
  'w',
  's',
  'a',
  'x',
  'z',
  'e',
  '3',
  KEY_F1,
  KEY_ESC,
  'q',
  KEY_LEFT_CTRL,
  KEY_LEFT_SHIFT,
  KEY_LEFT_GUI, // GRPH
  '1',
  '2',
  KEY_HOME, //CLR HOME,
  KEY_UP_ARROW,
  KEY_RIGHT_ARROW,
  KEY_LEFT_ARROW,
  KEY_DOWN_ARROW,
  ' ',
  KEY_DELETE,
  KEY_INSERT
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  

  pinMode(rowReset, OUTPUT);
  pinMode(selectCol, OUTPUT);
  pinMode(enableKeyboard, OUTPUT);

  // start row at 0
  digitalWrite(rowReset, HIGH);
  digitalWrite(rowReset, LOW);  
  digitalWrite(enableKeyboard, HIGH);
  Keyboard.begin();
}

void nextCol() {
  //Serial.println((PORTD & 64) >> 6);
  if (currCol % 2 == 0) {
    //Faster version of: digitalWrite(12, HIGH);
    PORTD = PORTD | B01000000;
  } else {
    //Faster version of: digitalWrite(12, LOW);
    PORTD = PORTD & B10111111;
    currRow = (currRow + 1) % 10;
  }
  currCol = (currCol + 1) % 2;
}

void nextRow() {
  nextCol();
  nextCol();
}

void scanKeyboard() {
  for (int i = 0; i < 18; i++) {
    //read port 8-11
    char pinb = PINB >> 4;
    int keyValIdx = currCol * 4 + currRow * 8;
    keys[keyValIdx].curr = pinb & 1; 
    keys[keyValIdx + 1].curr = (pinb >> 1) & 1;
    keys[keyValIdx + 2].curr = (pinb >> 2) & 1;
    keys[keyValIdx + 3].curr = (pinb >> 3) & 1;
    nextCol();
  }
  //skip over to first row
  nextRow();
  //determine what to release and what to press
  diffModifiers();
  diffKeys();
}

void diffKeys() {
  for (int i = 0; i < 72; i++) {
    key_state* key = keys + i;
    int diff = (key->old << 1) | key->curr;
    if (key->debounce != 0) {
      key->debounce = (key->debounce + 1) % debounceTime;  
    }
    else if (diff == 2) {
      Keyboard.release(key->key_code);
      key->old = key->curr;
      key->debounce++;
    } else if (diff == 1) {
      Keyboard.press(key->key_code);
      key->old = key->curr;
      key->debounce++;
    }
  }
}


void diffAndSyncKey(int i) {
  key_state* key = keys + i;
  int diff = (key->old << 1) | key->curr;
  if (key->debounce != 0) {
    return;
  } else if (diff == 2) {
    Keyboard.release(key->key_code);
    key->debounce++;  
    key->old = key->curr;
  } else if (diff == 1) {
    Keyboard.press(key->key_code);
    key->debounce++;
    key->old = key->curr;  
  }
}


void diffModifiers() {
  //4,5, 59-61
  diffAndSyncKey(4);
  diffAndSyncKey(5);
  diffAndSyncKey(59);
  diffAndSyncKey(60);
  diffAndSyncKey(61);
}

void loop() {
  scanKeyboard();
}
