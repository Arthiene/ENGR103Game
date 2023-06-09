#include <Adafruit_CircuitPlayground.h>

bool gameStarted;
volatile bool leftButtonPressed = false;
volatile bool rightButtonPressed = false;
int check[3] = {0};
int gamecheck[3] = {0};
int count = 0;
int level = 1;
int soundThreshold = 80;

//Turn all led red, yellow, then green, then start the game
void startGame() {
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0); //red
  }
  delay(1000);
  CircuitPlayground.clearPixels();


  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 255, 255, 0); //yellow
  }
  delay(1000);
  CircuitPlayground.clearPixels();


  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 0, 255, 0); //green
  }
  delay(1000);
  CircuitPlayground.clearPixels();
  delay(1000);

}

void setup() {
  CircuitPlayground.begin();
  attachInterrupt(4, leftButton, FALLING);//left button press
  attachInterrupt(5, rightButton, FALLING);//right button press
  gameStarted = true;
  startGame();
}

void startStage(){
  count = 0;//Resetting values that track button input
  int correct = 0;
for (int i = 0; i < 3; i++) {//Resetting values that track button input
    check[i] = 0;
    gamecheck[i] = 0;
}
  //select either the right or left LED to light up
  //How many times it lights up depends on level
  for(int i = 0; i < level; i++){
    int side = random(1, 3);
    gamecheck[i] = side;
    //Left LED
    if(side == 1){
      for (int i = 0; i < 5; ++i) {
      CircuitPlayground.setPixelColor(i, 150, 0, 150);
      }
      delay(500);
      CircuitPlayground.clearPixels();
      delay(1000);
    }
    //Right LED
    if(side == 2){
      for (int i = 5; i < 10; ++i) {
      CircuitPlayground.setPixelColor(i, 0, 0, 255);
      }
      delay(500);
      CircuitPlayground.clearPixels();
      delay(1000);
    }
  }    
//Wait 5 second for user input
delay(5000);

// Serial.println("finish");
  //check if the button input match the radomizer output
  for(int i = 0; i < level; i++){
    if(check[i] != gamecheck[i])
      correct++;
  }
  //If the inputs are correct
  if(correct == 0)
    success();
  //If the inputs are incorrect
  if(correct != 0)
    blinkRed();
}

//Function that activate when user fails the game
//Turn all LED red and play a sound
void blinkRed(){
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }
  CircuitPlayground.playTone(50, 500);
  CircuitPlayground.clearPixels(); 
  delay(500);
  gameStarted = false;//Stop the game from repeating
  level = 1; //Go back to first level
}
//Function that activate when user win the game or pass a level
//Turn all LED red and play a sound
void success(){
  //If user beat all stage play light pattern and sound
  if(level == 3){
    CircuitPlayground.playTone(400, 200);
  for (int cycle = 0; cycle < 3; ++cycle) {
  for (int i = 0; i < 10; ++i) {
    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);
    CircuitPlayground.setPixelColor(i, r, g, b);
    delay(100);
    level = 1;
  }
  CircuitPlayground.clearPixels();
  gameStarted = false;
}
  }
  //Turn all LED light blue and blink twice then move to next stage
  else{
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 0, 255, 255);
  }
  delay(500);
  CircuitPlayground.clearPixels();
  delay(500);
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, 0, 255, 255);
  }
  delay(500);
  CircuitPlayground.clearPixels();
  delay(500);
  level++; //Increase the difficulty then go to next stage
  }
}


void loop() {
  //Only continue playing the game when gameStarted is true
  while(gameStarted == true){
    startStage();
  }
  delay(100);
  //To start a new game once you win or lose the game, you have to make a sound that ...
  //... above a threshold to play again
  float sound = CircuitPlayground.mic.soundPressureLevel(50);
  Serial.println(sound);
  if(sound > soundThreshold){
    startGame();
    gameStarted = true;
  }
}
//1 in the array means a left button was pressed
void leftButton() {
  check[count] = 1;
  count++;
  Serial.println("L");
}
//2 in the array means a right button was pressed
void rightButton() {
  check[count] = 2;
  count++;
  Serial.println("R");
}
