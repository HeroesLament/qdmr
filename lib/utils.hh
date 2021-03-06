/** @defgroup util Utility functions and classes.
 * This module collects alls utility functions and classes. That is, functions to encode some
 * data and also classes implementing the DFU file format.
 */

#ifndef UTILS_HH
#define UTILS_HH

#include <QString>
#include <inttypes.h>

#include "signaling.hh"


/** Decodes the unicode string stored in @c data of size @c size. The @c fill code also defines the
 * end-of-string symbol.
 * @returns The decoded string. */
QString decode_unicode(const uint16_t *data, size_t size, uint16_t fill=0x0000);
/** Encodes the string @c text as unicode and stores the result into @c data using up-to @c size
 * 16bit words in data. The @c fill word specifies the fill and end-of-string word. */
void encode_unicode(uint16_t *data, const QString &text, size_t size, uint16_t fill=0x0000);

/** Decodes the ascii string in @c data into a @c QString of up-to size length. The @c fill word
 * specifies the fill and end-of-string word. */
QString decode_ascii(const uint8_t *data, size_t size, uint16_t fill=0x00);
/** Encodes the given QString @c text of up-to size length as ASCII into @c data using the
 * @c fill word as fill and end-of-string word. */
void encode_ascii(uint8_t *data, const QString &text, size_t size, uint16_t fill=0x00);

/** Decodes an 8 digit BCD encoded frequency (in MHz). */
double decode_frequency(uint32_t bcd);
/** Eecodes an 8 digit BCD encoded frequency (in MHz). */
uint32_t encode_frequency(double freq);

/** Decodes binary (24bit) encoded DMR ID. */
uint32_t decode_dmr_id_bin(const uint8_t *id);
/** Encodes binary (24bit) encoded DMR ID. */
void encode_dmr_id_bin(uint8_t *id, uint32_t num);
/** Decodes bcd (32bit) encoded DMR ID. */
uint32_t decode_dmr_id_bcd(const uint8_t *id);
/** Encodes bcd (32bit) encoded DMR ID. */
void encode_dmr_id_bcd(uint8_t *id, uint32_t num);

QString decode_dtmf_bin(const uint8_t *num, int size=16, uint8_t fill=0xff);
bool encode_dtmf_bin(const QString &number, uint8_t *num, int size=16, uint8_t fill=0xff);

/** Decodes the CTCSS tone or DCS code to @c Signaling::Code. */
Signaling::Code decode_ctcss_tone_table(uint16_t data);
/** Encodes the CTCSS tone or DCS code from @c Signaling::Code. */
uint16_t encode_ctcss_tone_table(Signaling::Code code);

/** Validates a DMR ID number. */
bool validDMRNumber(const QString &text);
/** Validates a DTMF number. */
bool validDTMFNumber(const QString &text);

/** Formats a frequency in MHz passed as double. */
QString format_frequency(double MHz);

#endif // UTILS_HH
