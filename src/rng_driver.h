/*
 * This file is part of the tyche project.
 *
 * Copyright (C) 2014 Mathias Dalheimer <md@gonium.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_RNG_H
#define SRC_RNG_H 1

#include <stdint.h>

void rng_driver_setup(void);
uint32_t rng_driver_get_uint32(void);

#endif /* SRC_RNG_H */

