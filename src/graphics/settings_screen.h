#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include <SDL.h>
#include <SDL_video.h>
#include "button.h"
#include "multiple_choice.h"


typedef enum {
	SETTINGS_SCREEN_NO_EVENT = 0,
	SETTINGS_SCREEN_NEW_GAME,
	SETTINGS_SCREEN_EXIT
} SettingsScreenEventType;

typedef struct {
    SettingsScreenEventType type;
    union EventData {
        GameSettings settings;
    } data;
} SettingsScreenEvent;

typedef enum {
	MODE_STAGE,
	DIFFICULTY_STAGE,
	COLOR_STAGE,
} SettingScreenStage;

typedef struct {
	SettingScreenStage stage;
	MultipleChoice *mode;
	MultipleChoice *difficulty;
	MultipleChoice *color;
	Button *next_button;
	Button *back_button;
	Button *start_button;
} SettingsScreen;

EngineMessage create_settings_screen(SettingsScreen **settings_screen, SDL_Renderer *renderer);

EngineMessage draw_settings_screen(SDL_Renderer *renderer, SettingsScreen *settings_screen);

void destroy_settings_screen(SettingsScreen *settings_screen);

EngineMessage settings_screen_event_handler(SDL_Event *event, SettingsScreen *settings_screen, SettingsScreenEvent *Settings_screen_event);

void reset_settings_screen(SettingsScreen *settings_screen);

#endif
