// ===========================================
// Reaction Time Experiment (Auto 15 Trials)
// LED = pin 8, Buzzer = pin 9, Button = pin 2
// ===========================================

int ledPin = 8;
int buzzerPin = 9;
int buttonPin = 2;

// trial schedule (15 trials, 5 of each cue)
int trials[15] = {1,1,1,1,1, 2,2,2,2,2, 3,3,3,3,3};

unsigned long startTime;
unsigned long reactionTime;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println("subject,trial,cue,reaction_ms");

  // Randomize the trial list using Fisher-Yates shuffle
  for (int i=14; i>0; i--) {
    int j = random(0, i+1);
    int temp = trials[i];
    trials[i] = trials[j];
    trials[j] = temp;
  }
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's') {
      runAllTrials();
    }
  }
}

void runAllTrials() {
  for (int t=0; t<15; t++) {
    int cueType = trials[t];

    // ensure button is released
    while (digitalRead(buttonPin) == HIGH) {}

    // random delay 3–5 seconds between trials
    delay(random(3000, 5000));

    // Activate cue
    if (cueType == 1) digitalWrite(ledPin, HIGH);
    else if (cueType == 2) digitalWrite(buzzerPin, HIGH);
    else if (cueType == 3) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
    }

    startTime = millis();

    // wait for reaction
    while (digitalRead(buttonPin) == LOW) {}

    delay(20); // debounce

    reactionTime = millis() - startTime;

    // turn off cues
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);

    // print CSV
    Serial.print("X");               // subject ID placeholder
    Serial.print(",");
    Serial.print(t+1);               // trial number 1–15
    Serial.print(",");
    
    if (cueType == 1) Serial.print("LED");
    if (cueType == 2) Serial.print("BUZZER");
    if (cueType == 3) Serial.print("BOTH");

    Serial.print(",");
    Serial.println(reactionTime);

    // wait for release
    while (digitalRead(buttonPin) == HIGH) {}
  }

  Serial.println("=== Experiment complete ===");
}

