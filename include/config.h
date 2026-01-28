#pragma once
#include <Arduino.h>

// Number of channels for this build
static constexpr size_t PROJECT_CHANNEL_COUNT = 3;

// Channel pin configuration
constexpr uint8_t CH_INPUT_PINS[PROJECT_CHANNEL_COUNT]  = { D1, D2, D3 };
constexpr uint8_t CH_OUTPUT_PINS[PROJECT_CHANNEL_COUNT] = { D5, D6, D7 };

// Device pin configuration
constexpr uint8_t VACUUM_OUTPUT_PIN = 0xFF;  // No vacuum control by default

constexpr uint8_t LED_PIN = LED_BUILTIN;  // D4

// Sanity checks
static_assert((sizeof(CH_INPUT_PINS)/sizeof(CH_INPUT_PINS[0])) == PROJECT_CHANNEL_COUNT, "CH_INPUT_PINS must match PROJECT_CHANNEL_COUNT");
static_assert((sizeof(CH_OUTPUT_PINS)/sizeof(CH_OUTPUT_PINS[0])) == PROJECT_CHANNEL_COUNT, "CH_OUTPUT_PINS must match PROJECT_CHANNEL_COUNT");
