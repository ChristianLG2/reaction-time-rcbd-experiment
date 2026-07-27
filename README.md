# reaction-time-rcbd-experiment

A randomized complete block design (RCB[1]) experiment analyzing the effect of sensory cue type (visual, auditory, combined) on human reaction time, using a linear mixed-effects model in R.

## Overview

This project investigates whether the type of sensory cue presented, visual (LED), auditory (buzzer), or combined (both), produces meaningful differences in human reaction time. Reaction time was measured with an Arduino Uno-based timing rig, and the data were analyzed using a Randomized Complete Block Design with Person as the blocking factor, extended to a linear mixed-effects model to account for replication within blocks.

**Response variable:** Reaction time (milliseconds), recorded automatically by the Arduino.

**Treatment factor, Cue Type (3 levels):**
- **LED** – visual cue
- **BUZZER** – auditory cue
- **BOTH** – simultaneous audio-visual cue

**Blocking factor:** Participant (5 blocks total), used to control for natural individual differences in baseline reaction speed.

## Design

A conventional RCB[1] design assigns each treatment to each block exactly once. In this experiment, each participant instead completed multiple trials per cue type (5 LED, 5 BUZZER, 5 BOTH, 15 trials per person, 75 observations total), introducing replication within blocks. Because Cue Type is a fixed effect and Participant is a random blocking factor, the resulting participant-by-cue interaction is mixed (fixed × random), which requires a **linear mixed-effects model** rather than the classical unreplicated RCB[1] ANOVA.

**Model:**

```
y_ijk = μ + β_j + b_i + (bβ)_ij + ε_ijk
```

- `β_j` — fixed effect of Cue Type
- `b_i` — random effect of Participant
- `(bβ)_ij` — random participant-by-cue interaction
- `ε_ijk` — residual trial-to-trial error

**Hypotheses:**
- H₀: β₁ = β₂ = β₃ = 0
- Hₐ: β_j ≠ 0 for at least one cue type

Significance level: α = 0.05

To reduce confounding from reflex training, participants with competitive or high-frequency video game experience were excluded from the study.

## Data collection

Reaction time was recorded using an Arduino Uno wired to an LED (visual cue, pin 8), a buzzer (auditory cue, pin 9), and a momentary push button (participant response, pin 2). The Arduino:

- randomized the order of the 15 trials per participant (Fisher–Yates shuffle),
- introduced a random 3–5 second delay before each cue,
- activated the assigned cue and measured elapsed time to button press with millisecond accuracy,
- logged results as CSV over serial (`subject, trial, cue, reaction_ms`).

Randomization ensured no cue type was systematically favored by trial order, and all trials were conducted indoors under standardized conditions with a brief practice trial to reduce learning effects.

## Results

Descriptive statistics showed the **BOTH** cue produced the fastest mean reaction time (221.4 ms), followed by **BUZZER** (252.2 ms), with **LED** slowest (275.2 ms). Participant-level summaries confirmed substantial baseline differences between individuals, supporting the use of a blocking design.

The mixed-effects model found a statistically significant effect of Cue Type on reaction time: **F(2, 8) = 5.05, p = 0.038**. Post hoc pairwise comparisons (Tukey-adjusted) showed:

| Comparison | Estimate | p-value |
|---|---|---|
| BOTH vs LED | −53.7 ms | **0.032** (significant) |
| BOTH vs BUZZER | — | 0.225 |
| BUZZER vs LED | — | 0.409 |

Only the BOTH vs. LED comparison reached significance, indicating that combined audio-visual cues produce meaningfully faster reactions than visual cues alone, consistent with theories of multisensory integration and redundant signal facilitation.

Residual diagnostics (Q-Q plot, residuals vs. fitted, residuals vs. order) indicated the normality and constant-variance assumptions were reasonably satisfied, so no response transformation was needed.

## Limitations

- Small sample size (5 participants) limits generalizability.
- Data were collected in a single controlled indoor setting; results may not generalize to real-world environments with more distraction, fatigue, or variable lighting.
- Future work could expand the participant pool, add cue types (e.g., vibration), and examine interactions with factors like age or gaming experience.

## Repository contents

| File | Description |
|---|---|
| `CB1_Data_Collection_Arduino.ino` | Arduino sketch controlling cue presentation, timing, and randomization |
| `CB1 Collected Dataset.xlsx` | Raw collected reaction time data (75 observations) |
| `CB[1].Rmd` | R Markdown source: data import, EDA, mixed-effects model, diagnostics, post hoc analysis |
| `CB[1].html` | Rendered knit output of the analysis (auto-generated) |

## Reproducing the analysis

The analysis was conducted in R using the following packages: `lmerTest`, `emmeans`, `car`, `dplyr`, `mosaic`, `kableExtra`, `ggplot2`, `knitr`.

1. Clone the repo.
2. Open `CB[1].Rmd` in RStudio.
3. Update the hardcoded file path to point to your local copy of `CB1 Collected Dataset.xlsx`.
4. Knit the document to reproduce the full analysis and report.

## Author

Christian Lira Gonzalez
