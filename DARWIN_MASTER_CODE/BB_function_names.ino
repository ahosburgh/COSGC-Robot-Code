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
void setCal();
int GetGoldenDirection();

void StartUpLights();
void LightsOut();

void MoveForward(int dir);
void Drift(int dir);
void TurnLeft(int deg);
void TurnRight(int deg);

void CenterStepper();
float degToSteps(float deg);
void StepperLeft(int deg);
void StepperRight(int deg);

bool Sweep();
void Avoidence();
bool ObjectDetection();
void MeasureObject();
int GetDistance();
int ServoPos(int deg);
void LevelTOF(int);
int ReadServoInDeg();
int ServoToDeg(int servoPos);
int DegToServo(int deg);
