// Pin definitions
const int stepPin = 9;
const int dirPin = 8;
const int inputPin = 6;
const int enaPin = 10;  // Enable output

// Step tracking
volatile long stepCount = 0;

// Edge tracking
bool lastState = LOW;
bool firstValidEdgeSeen = false;

// Storage arrays
long falling[4];
long rising[4];

int fallIndex = 0;
int riseIndex = 0;

// Control flag
bool done = false;

void setup() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(inputPin, INPUT);

    digitalWrite(dirPin, HIGH); // Set direction

    lastState = digitalRead(inputPin);
    digitalWrite(enaPin, HIGH); // Set direction
}

void loop() {
    if (!done) {
        stepMotor();
        checkInput();
    } else {
        // Stop motor
        digitalWrite(stepPin, LOW);
    }
}

void stepMotor() {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);

    stepCount++;
}

void checkInput() {
    bool currentState = digitalRead(inputPin);

    if (currentState != lastState) {
        // Edge detected

        // Ignore first rising edge if it's the first event
        if (!firstValidEdgeSeen) {
            if (currentState == HIGH) {
                // Rising edge — ignore
                lastState = currentState;
                return;
            } else {
                // First valid edge must be falling
                firstValidEdgeSeen = true;
            }
        }

        if (currentState == LOW && fallIndex < 4) {
            // Falling edge
            falling[fallIndex++] = stepCount;
        }
        else if (currentState == HIGH && riseIndex < 4) {
            // Rising edge
            rising[riseIndex++] = stepCount;
        }

        // Check if done
        if (fallIndex >= 4 && riseIndex >= 4) {
            done = true;

            // Calculate averages
            float fallingAvg = (
                (falling[1] - falling[0]) +
                (falling[2] - falling[1]) +
                (falling[3] - falling[2])
            ) / 3.0;

            float risingAvg = (
                (rising[0] - falling[0]) +
                (rising[1] - falling[1]) +
                (rising[2] - falling[2])
            ) / 3.0;

            // Store or print results
            Serial.begin(9600);
            Serial.print("Falling Avg: ");
            Serial.println(fallingAvg);

            Serial.print("Rising Avg: ");
            Serial.println(risingAvg);
        }

        lastState = currentState;
    }
}