#pragma once

#define FPS 60

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 1200

#define PLAYER_WIDTH 54
#define PLAYER_HEIGHT 27

#define PLAYER_JUMP_WIDTH 46
#define PLAYER_JUMP_HEIGHT 43

#define PLAYER_DEAD_WIDTH 28
#define PLAYER_DEAD_HEIGHT 32

#define PLAYER_JUMP_V -20.0f
#define PLAYER_JUMP_HIGH_V -30.0f
#define PLAYER_JUMP_SMALL_V -10.0f

#define PLAYER_HORIZONTAL_SPEED 4.0f
#define PLAYER_HORIZONTAL_SPEED_SLOW 2.4f
#define PLAYER_HORIZONTAL_SPEED_MEDIUM 8.0f
#define PLAYER_HORIZONTAL_SPEED_FAST 12.0f

#define PLAYER_LIFT_FROM_BELOW_TRESHOLD 15.0f

#define PLAYER_ON_GROUND_TRESHOLD_ABOVE 1.0

#define FRICTION_ZERO_TRESHOLD 1.0f

#define TRAMPOLINE_WIDTH 20
#define TRAMPOLINE_HEIGHT 10
#define TRAMPOLINE_DETECTION_HEIGHT 10
#define TRAMPOLINE_DETECTION_PADDING 0
#define TRAMPOLINE_DETECTION_HPADDING 4

#define STATIC_OBJECT_ID_TRAMPOLINE 100
#define STATIC_OBJECT_ID_DECORATION_START 200
