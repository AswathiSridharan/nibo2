/*  BSD-License:

Copyright (c) 2007 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef NIBOCC_ADC_HPP_
#define NIBOCC_ADC_HPP_
#if defined(NIBOCC_USE_ADC) || defined (DOXYGEN)

/*! @file    adc.hpp
 *  @brief   Routinen zum Einlesen der Analogeingaenge
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#include <inttypes.h>
#include "nibo/adc.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zum Einlesen der Analogeingaenge
 */
class Adc
{
  public:
    /*!
     * Liest einen analogen Kanal aus
     * @param channel Kanalnummer - Nummer des Port A Pins.
     *                (0 fuer PA0, 1 fuer PA1, 2 fuer PA2 ...)
     */
    static uint16_t read(uint8_t channel) {return ::adc_read(channel);}

    /*!
     * Initialisert den AD-Kanal auf Input ohne Pullup.
     * @param channel Kanalnummer - Nummer des Port A Pins.
     *                (0 fuer PA0, 1 fuer PA1, 2 fuer PA2 ...)
     */
    static void init(uint8_t channel) {::adc_init(channel);}

    /*!
     * Liefert einen Zufallswert zurück. Der Wert errechnet sich aus der Summe
     * aller gemessenen Analog-Spannungen. Der Wert selbst sollte nur als Seed
     * (Enropiequelle) verwendet werden, da er monoton wachsend ist. Der Wert
     * wird um so zufälliger, je mehr analoge Messungen vorher durchgeführt
     * wurden.
     * @return Random Seed
     */
    static uint16_t get_random_seed() {return ::adc_get_random_seed();}

};

} // namespace nibocc

#endif // NIBOCC_USE_ADC
#endif // NIBOCC_ADC_HPP_
