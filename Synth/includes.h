#pragma once

#include <stdio.h>
#include <SFML/Audio.hpp>

// typedefs for easy refactoring
typedef long double hpSignal;
typedef sf::Int64 hpTime;

// useful for sinusoidal stuff
constexpr hpSignal TAO = 6.283185307179586476925286766559L;

// time interval constants
constexpr hpTime TIME_ZERO = 0LL;
constexpr hpTime PICOSECOND = 1LL;
constexpr hpTime NANOSECOND = 1000LL * PICOSECOND;
constexpr hpTime MICROSECOND = 1000LL * NANOSECOND;
constexpr hpTime MILLISECOND = 1000LL * MICROSECOND;
constexpr hpTime SECOND = 1000LL * MILLISECOND;
constexpr hpSignal TICKS_TO_SECONDS = 1.L / SECOND;
