void DCDriftLeft();
void DCDriftRight();
void DCLeft();
void DCRight();
void DCForward();
void DCBack(int ReverseTime);
void DCStop();

bool Navigation(float dir);
float IMUDirection();
float IMUPitch();
void CalibrateIMU();
float GetGoldenDirection();

void StartUpLights();
void LightsOut();

void MoveForward(int dir);
void Drift(int dir);
void TurnLeft(int deg);
void TurnRight(int deg);

float degToSteps(float deg);
void StepperLeft(float deg);
void StepperRight(float deg);

bool ObjectDetection();
int GetDistance();
float ServoPos(int deg);
void LevelTOF();
float ServoToDeg(int servoPos);
float DegToServo(float deg);
