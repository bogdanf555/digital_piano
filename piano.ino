// include libraries for I2C Liquid Crystal and Tone
#include "LiquidCrystal_I2C.h"
#include "Tone.h"

// include custom headers
#include "Vector.h"
#include "const.h"
#include "key.h"

// lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

// buzzers
Tone buzzer1;
Tone buzzer2;

// tones on board
int octave_index = 3;
int *notes_on_board = octaves[octave_index];

// piano state
int piano_state = PLAY;

// recording vectors
Vector recorded_keys(RECORD_CAP);
Vector recorded_time_stamps(RECORD_CAP);

// recording variables
bool first_recorded = false;
unsigned long first_stamp;

// singing variables
int singed_index = -1;
unsigned start_singing_stamp;

// keys creation
Key keyboard[8] = {{FIRST}, {SECOND}, {THIRD}, {FOURTH}, {FIFTH}, {SIXTH}, {SEVENTH}, {EIGHTH}};
Key record_button(RECORD_BUTTON);
Key change_octave_button(CHANGE_OCTAVE_BUTTON);

// playing vector in play state
Vector playing_notes(PLAYING_NOTES_CAP);

void lcd_update()
{
    lcd.clear();
    lcd.setCursor(0, 0);

    if (piano_state == PLAY)
    {
        lcd.print("PLAYING...");
        lcd.setCursor(0, 1);
        lcd.print("Octave: ");
        lcd.setCursor(8, 1);
        lcd.print(String(octave_index + 1));
    }
    else if (piano_state == REC)
    {
        lcd.print("RECORDING...");
        lcd.setCursor(0, 1);
        lcd.print("NO NOTES: ");
        lcd.setCursor(10, 1);
        lcd.print(String(recorded_keys.length()));
    }
    else
    {
        lcd.print("SINGING...");
    }
}

void check_vector_allocation()
{
    if (playing_notes.allocation_failed() || recorded_keys.allocation_failed() || recorded_time_stamps.allocation_failed())
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
        digitalWrite(LED_BUILTIN, HIGH);
    }
}

void play_note(int note)
{
    playing_notes.push_back(note);
    buzzer1.play(note);
}

void stop_note(int note)
{
    playing_notes.remove(note);
    buzzer1.stop();
    for (int i = 0; i < playing_notes.length(); i++)
    {
        buzzer1.play(playing_notes.at_index(i));
    }
}

void record_note(int note)
{
    recorded_keys.push_back(note);
    recorded_time_stamps.push_back(millis());
}

void setup()
{
    // init led built in
    pinMode(LED_BUILTIN, OUTPUT);

    // init piezo
    pinMode(PIEZO_OUTPUT_1, OUTPUT);
    pinMode(PIEZO_OUTPUT_2, OUTPUT);

    // init utils
    record_button.begin();
    change_octave_button.begin();

    // init piano buttons
    for (int i = 0; i < KEYBOARD_LENGTH; i++)
    {
        keyboard[i].begin();
    }

    // init buzzers
    buzzer1.begin(PIEZO_OUTPUT_1);
    buzzer2.begin(PIEZO_OUTPUT_2);

    // init lcd
    lcd.init();
    lcd.backlight();

    // check if the vectors were allocated
    check_vector_allocation();
}

int key_state;
void loop()
{
    lcd_update();

    // check record button
    key_state = record_button.isToggled();
    if (key_state == JUST_PRESSED)
    {
        Serial.println(piano_state);
        if (piano_state == PLAY)
        {
            piano_state = REC;
            first_recorded = false;
            first_stamp = 0;
            recorded_keys.reset();
            recorded_time_stamps.reset();
        }
        else if (piano_state == REC)
        {
            piano_state = SING;
            recorded_time_stamps.subtract_by(recorded_time_stamps.first());
            singed_index = -1;
            buzzer1.stop();
            playing_notes.reset();
        }
        else if (piano_state == SING)
        {
            buzzer1.stop();
            playing_notes.reset();
            piano_state = PLAY;
        }
    }

    if (piano_state == SING)
    {
        if (singed_index == -1)
            start_singing_stamp = millis();

        if (singed_index < recorded_keys.length())
        {
            int note = recorded_keys.at_index(singed_index);
            unsigned int timestamp = recorded_time_stamps.at_index(singed_index);

            if (timestamp + start_singing_stamp >= millis())
            {
                if (note >= 0)
                {
                    int to_play = octaves[(note / 10)][(note % 10)];
                    play_note(to_play);
                }
                else
                {
                    int to_stop = octaves[(-note / 10)][(-note % 10)];
                    stop_note(to_stop);
                }
                singed_index++;
            }
        }
        else
        {
            singed_index = -1;
        }
    }
    else
    {
        // check change octave button
        key_state = change_octave_button.isToggled();
        if (key_state == JUST_PRESSED && playing_notes.is_empty())
        {
            octave_index = (octave_index + 1) % 7;
            notes_on_board = octaves[octave_index];
        }

        // check piano keys
        for (int i = 0; i < KEYBOARD_LENGTH; i++)
        {
            key_state = keyboard[i].isToggled();

            if (key_state == JUST_PRESSED)
            {
                if (piano_state == REC)
                {

                    record_note(octave_index * 10 + i);

                    if (!first_recorded)
                    {
                        first_recorded = true;
                        first_stamp = recorded_time_stamps.last();
                    }
                }

                play_note(notes_on_board[i]);
            }
            else if (key_state == JUST_RELEASED)
            {

                if (piano_state == REC)
                {
                    record_note(-(octave_index * 10 + i));
                }

                stop_note(notes_on_board[i]);
            }
        }
    }
}