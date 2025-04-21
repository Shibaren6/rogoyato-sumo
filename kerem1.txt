byte direction = 2; // 0 -> Left, 1 -> Right, 2 -> Forward
byte line = 0; // 0-> I'm Not On The Line, 1 -> I'm On The Line Or Too Near
byte founded = 0; // 0 -> Was Not Found, 1 -> Was Found
byte enemy_direction = 0; // 0 -> Left, 1 -> Right
byte state = 0; // 0 -> BASLA, 1 -> ARA, 2 -> YANA GEC, 3 -> SALDIR, 4 -> GERI CEKIL
final byte max_movement_duration = 3; // The Movement Limit For SALDIR

// States
final byte BASLA = 0;
final byte ARA = 1;
final byte YANA_GEC = 2;
final byte SALDIR = 3;
final byte GERI_CEKIL = 4;

// Signals
byte leftSignal = 0; // 0 -> Dont See, 1 -> See
byte rightSignal = 0; // 0 -> Dont See, 1 -> See
byte distance = 0; // Distance

void setup() {
  // put your setup code here, to run once:
  setAllThings();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(state){
    case BASLA:
      waitUntilStart();
      break;
    case ARA:
      signalReadings();
      settleDirection();
      break;
    case YANA_GEC:
      movingNearOfEnemy(2,3);
      signalReadings();
      if(leftSignal == 1 && rightSignal == 1){ // IN FORWARD BUT TOO NEAR, DISTANCE < 5
        founded = 1;
        state = SALDIR;
      }
      else if(leftSignal == 0 && rightSignal == 0 && distance < 40){ // IN FORWARD, DISTANCE < 40
        founded = 1;
        move(2);
        state = SALDIR;
      }
      else{
        founded = 0;
        state = ARA;
      }
      break;
    case SALDIR:
      signalReadings();
      if(leftSignal == 1 && rightSignal == 1){ // IN FORWARD BUT TOO NEAR, DISTANCE < 5
        founded = 1;
        dash(3);
        state = GERI_CEKIL;
      }
      else if(leftSignal == 0 && rightSignal == 0 && distance < 40 && distance != 0){ // IN FORWARD, DISTANCE < 40
        founded = 1;
        move(3);
      }
      else{
        founded = 0;
        state = ARA;
      }
      break;
    case GERI_CEKIL:
      moveBack(1);
      state = YANA_GEC;
      break;
  }
}

void setAllThings(){
  // Default Settings
}

void waitUntilStart(){
  // Start Waiting
}

byte readLeftSignal(){
  // If Left Signal See Return 1 Else Return 0
}

byte readRightSignal(){
  // If Left Signal See Return 1 Else Return 0
}

byte calculateDistance(){
  // Return Distance
}

void signalReadings(){
  leftSignal = readLeftSignal();
  rightSignal = readRightSignal();
  distance = calculateDistance();
}

void settleDirection(){
  if(leftSignal == 0 && rightSignal == 0 && distance == 0){ // WAS NOT FOUND
    founded = 0;
    turn(5);
    move(1);
    state = ARA;
  }
  else if(leftSignal == 1 && rightSignal == 0 && distance == 0){ // IN LEFT
    founded = 1;
    enemy_direction = 0;
    turnLeft(5);
    state = ARA;
  }
  else if(leftSignal == 0 && rightSignal == 1 && distance == 0){ // IN RIGHT
    founded = 1;
    enemy_direction = 1;
    turnRight(5);
    state = ARA;
  }
  else if(leftSignal == 0 && rightSignal == 0 && distance < 40){ // IN FORWARD, DISTANCE < 40
    founded = 1;
    state = YANA_GEC;
  }
  else if(leftSignal == 0 && rightSignal == 0 && distance != 0){ // IN FORWARD, DISTANCE > 40
    founded = 1;
    move(5);
    state = ARA;
  }
  else if(leftSignal == 1 && rightSignal == 0 && distance != 0){ // IN A LITTLE BIT LEFT
    founded = 1;
    enemy_direction = 0;
    turnLeft(2);
    state = ARA;
  }
  else if(leftSignal == 0 && rightSignal == 1 && distance != 0){ // IN A LITTLE BIT RIGHT
    founded = 1;
    enemy_direction = 1;
    turnRight(2);
    state = ARA;
  }
  else if(leftSignal == 1 && rightSignal == 1 && distance == 0){ // IN FORWARD BUT TOO NEAR, DISTANCE < 4
    founded = 1;
    state = SALDIR;
  }
  else{ // IN FORWARD, DISTANCE >= 4 && DISTANCE <= 5
    founded = 1;
    state = SALDIR;
  }
}

void turn(byte angle){ 
  // Turn To Enemy Direction
  if(enemy_direction == 0){ // Left
    turnLeft(angle);
  }
  else{
    turnRight(angle);
  } 
}

void turnLeft(byte angle){
  // Turn Left
}

void turnRight(byte angle){
  // Turn Right
}

void controlTheLine(){
  // If I'm On The Line Or It's Too Near Do line = 1 Else Do line = 0
}

void move(byte time){
  controlTheLine();
  if(line){
    turn(180); // Turn The Back
  }
  else{
    moveForward(time);
  }
  stopMove();
}

void moveForward(byte time){
  // Go Forward In Time Limitation
}

void movingNearOfEnemy(byte time, byte angle){
  byte oppositeDirection = enemy_direction == 0 ? 1 : 0;

  if(oppositeDirection == 0){
    // To Left
    turnLeft(angle);
    move(time);
    turnRight(angle);
  }
  else{
    // To Right
    turnRight(angle);
    move(time);
    turnLeft(angle);
  }
}

void stopMove(){
  // Stop Move
}

void dash(byte time){
  // Dash, For Attacking
  controlTheLine();
  if(line){
    stopMove(); // Turn The Back
    turn(180);
    state = ARA;
  }
  else if(time < max_movement_duration){
    dashMove(time);
  }
  else{
    dashMove(max_movement_duration);
  }
}

void dashMove(byte time){
  // Dash Move
}

void moveBack(byte time){
  controlTheLine();
  if(line){
    stopMove(); // Turn The Back
  }
  state = YANA_GEC;
}