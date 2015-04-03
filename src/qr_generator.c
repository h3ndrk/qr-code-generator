/**
 * Copyright (C) 2015 NIPE-SYSTEMS
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <qrencode.h>
#include <errno.h>

#include "qr_generator.h"

static GArray *qr_code_data = NULL;
static gint qr_code_size = 0;
static gint qr_code_version = 0;
static gboolean pseudo_true = TRUE;
static gboolean pseudo_false = FALSE;

gint qr_render(gchar *input)
{
	QRcode *qr = NULL;
	gint i_qr = 0;
	gint j_qr = 0;
	
	qr = QRcode_encodeString(input, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
	
	if(qr == NULL)
	{
		switch(errno)
		{
			case EINVAL:
			{
				return ERR_INVALID_INPUT;
				break;
			}
			case ENOMEM:
			{
				return ERR_NO_MEMORY;
				break;
			}
			case ERANGE:
			{
				return ERR_RANGE;
				break;
			}
		}
	}
	
	if(qr_code_data != NULL)
	{
		g_array_free(qr_code_data, TRUE);
		qr_code_data = NULL;
	}
	
	qr_code_data = g_array_new(FALSE, TRUE, sizeof(gboolean));
	
	qr_code_size = qr->width;
	qr_code_version = qr->version;
	
	// printf("\e[0;107m  \e[0m");
	
	// for(j_qr = 0; j_qr < qr->width; j_qr++)
	// {
	// 	printf("\e[0;107m  \e[0m");
	// }
	
	// printf("\e[0;107m  \e[0m\n");
	
	for(i_qr = 0; i_qr < qr->width; i_qr++)
	{
		// printf("\e[0;107m  \e[0m");
		
		for(j_qr = 0; j_qr < qr->width; j_qr++)
		{
			if(qr->data[(i_qr * qr->width) + j_qr] & 0x1)
			{
				// printf("\e[0;100m  \e[0m");
				g_array_append_val(qr_code_data, pseudo_true);
			}
			else
			{
				// printf("\e[0;107m  \e[0m");
				g_array_append_val(qr_code_data, pseudo_false);
			}
		}
		
		// printf("\e[0;107m  \e[0m\n");
	}
	
	// printf("\e[0;107m  \e[0m");
	
	// for(j_qr = 0; j_qr < qr->width; j_qr++)
	// {
	// 	printf("\e[0;107m  \e[0m");
	// }
	
	// printf("\e[0;107m  \e[0m\n");
	
	QRcode_free(qr);
	
	return ERR_NO_ERROR;
}

void qr_free(void)
{
	if(qr_code_data != NULL)
	{
		g_array_free(qr_code_data, TRUE);
		qr_code_data = NULL;
	}
}

gint qr_get_size(void)
{
	return qr_code_size;
}

GArray *qr_get_pixels(void)
{
	return qr_code_data;
}

// static GArray *qr_code = NULL;
// static gint qr_code_size = 0;
// static const guint qr_character_amount[] =
// {
// 	41, 25, 17, 10,
// 	34, 20, 14, 8,
// 	27, 16, 11, 7,
// 	17, 10, 7, 4,
// 	77, 47, 32, 20,
// 	63, 38, 26, 16,
// 	48, 29, 20, 12,
// 	34, 20, 14, 8,
// 	127, 77, 53, 32,
// 	101, 61, 42, 26,
// 	77, 47, 32, 20,
// 	58, 35, 24, 15,
// 	187, 114, 78, 48,
// 	149, 90, 62, 38,
// 	111, 67, 46, 28,
// 	82, 50, 34, 21,
// 	255, 154, 106, 65,
// 	202, 122, 84, 52,
// 	144, 87, 60, 37,
// 	106, 64, 44, 27,
// 	322, 195, 134, 82,
// 	255, 154, 106, 65,
// 	178, 108, 74, 45,
// 	139, 84, 58, 36,
// 	370, 224, 154, 95,
// 	293, 178, 122, 75,
// 	207, 125, 86, 53,
// 	154, 93, 64, 39,
// 	461, 279, 192, 118,
// 	365, 221, 152, 93,
// 	259, 157, 108, 66,
// 	202, 122, 84, 52,
// 	552, 335, 230, 141,
// 	432, 262, 180, 111,
// 	312, 189, 130, 80,
// 	235, 143, 98, 60,
// 	652, 395, 271, 167,
// 	513, 311, 213, 131,
// 	364, 221, 151, 93,
// 	288, 174, 119, 74,
// 	772, 468, 321, 198,
// 	604, 366, 251, 155,
// 	427, 259, 177, 109,
// 	331, 200, 137, 85,
// 	883, 535, 367, 226,
// 	691, 419, 287, 177,
// 	489, 296, 203, 125,
// 	374, 227, 155, 96,
// 	1022, 619, 425, 262,
// 	796, 483, 331, 204,
// 	580, 352, 241, 149,
// 	427, 259, 177, 109,
// 	1101, 667, 458, 282,
// 	871, 528, 362, 223,
// 	621, 376, 258, 159,
// 	468, 283, 194, 120,
// 	1250, 758, 520, 320,
// 	991, 600, 412, 254,
// 	703, 426, 292, 180,
// 	530, 321, 220, 136,
// 	1408, 854, 586, 361,
// 	1082, 656, 450, 277,
// 	775, 470, 322, 198,
// 	602, 365, 250, 154,
// 	1548, 938, 644, 397,
// 	1212, 734, 504, 310,
// 	876, 531, 364, 224,
// 	674, 408, 280, 173,
// 	1725, 1046, 718, 442,
// 	1346, 816, 560, 345,
// 	948, 574, 394, 243,
// 	746, 452, 310, 191,
// 	1903, 1153, 792, 488,
// 	1500, 909, 624, 384,
// 	1063, 644, 442, 272,
// 	813, 493, 338, 208,
// 	2061, 1249, 858, 528,
// 	1600, 970, 666, 410,
// 	1159, 702, 482, 297,
// 	919, 557, 382, 235,
// 	2232, 1352, 929, 572,
// 	1708, 1035, 711, 438,
// 	1224, 742, 509, 314,
// 	969, 587, 403, 248,
// 	2409, 1460, 1003, 618,
// 	1872, 1134, 779, 480,
// 	1358, 823, 565, 348,
// 	1056, 640, 439, 270,
// 	2620, 1588, 1091, 672,
// 	2059, 1248, 857, 528,
// 	1468, 890, 611, 376,
// 	1108, 672, 461, 284,
// 	2812, 1704, 1171, 721,
// 	2188, 1326, 911, 561,
// 	1588, 963, 661, 407,
// 	1228, 744, 511, 315,
// 	3057, 1853, 1273, 784,
// 	2395, 1451, 997, 614,
// 	1718, 1041, 715, 440,
// 	1286, 779, 535, 330,
// 	3283, 1990, 1367, 842,
// 	2544, 1542, 1059, 652,
// 	1804, 1094, 751, 462,
// 	1425, 864, 593, 365,
// 	3517, 2132, 1465, 902,
// 	2701, 1637, 1125, 692,
// 	1933, 1172, 805, 496,
// 	1501, 910, 625, 385,
// 	3669, 2223, 1528, 940,
// 	2857, 1732, 1190, 732,
// 	2085, 1263, 868, 534,
// 	1581, 958, 658, 405,
// 	3909, 2369, 1628, 1002,
// 	3035, 1839, 1264, 778,
// 	2181, 1322, 908, 559,
// 	1677, 1016, 698, 430,
// 	4158, 2520, 1732, 1066,
// 	3289, 1994, 1370, 843,
// 	2358, 1429, 982, 604,
// 	1782, 1080, 742, 457,
// 	4417, 2677, 1840, 1132,
// 	3486, 2113, 1452, 894,
// 	2473, 1499, 1030, 634,
// 	1897, 1150, 790, 486,
// 	4686, 2840, 1952, 1201,
// 	3693, 2238, 1538, 947,
// 	2670, 1618, 1112, 684,
// 	2022, 1226, 842, 518,
// 	4965, 3009, 2068, 1273,
// 	3909, 2369, 1628, 1002,
// 	2805, 1700, 1168, 719,
// 	2157, 1307, 898, 553,
// 	5253, 3183, 2188, 1347,
// 	4134, 2506, 1722, 1060,
// 	2949, 1787, 1228, 756,
// 	2301, 1394, 958, 590,
// 	5529, 3351, 2303, 1417,
// 	4343, 2632, 1809, 1113,
// 	3081, 1867, 1283, 790,
// 	2361, 1431, 983, 605,
// 	5836, 3537, 2431, 1496,
// 	4588, 2780, 1911, 1176,
// 	3244, 1966, 1351, 832,
// 	2524, 1530, 1051, 647,
// 	6153, 3729, 2563, 1577,
// 	4775, 2894, 1989, 1224,
// 	3417, 2071, 1423, 876,
// 	2625, 1591, 1093, 673,
// 	6479, 3927, 2699, 1661,
// 	5039, 3054, 2099, 1292,
// 	3599, 2181, 1499, 923,
// 	2735, 1658, 1139, 701,
// 	6743, 4087, 2809, 1729,
// 	5313, 3220, 2213, 1362,
// 	3791, 2298, 1579, 972,
// 	2927, 1774, 1219, 750,
// 	7089, 4296, 2953, 1817,
// 	5596, 3391, 2331, 1435,
// 	3993, 2420, 1663, 1024,
// 	3057, 1852, 1273, 784
// };
// static const gchar qr_alphanumeric_converting[] =
// {
// 	'0',
// 	'1',
// 	'2',
// 	'3',
// 	'4',
// 	'5',
// 	'6',
// 	'7',
// 	'8',
// 	'9',
// 	'A',
// 	'B',
// 	'C',
// 	'D',
// 	'E',
// 	'F',
// 	'G',
// 	'H',
// 	'I',
// 	'J',
// 	'K',
// 	'L',
// 	'M',
// 	'N',
// 	'O',
// 	'P',
// 	'Q',
// 	'R',
// 	'S',
// 	'T',
// 	'U',
// 	'V',
// 	'W',
// 	'X',
// 	'Y',
// 	'Z',
// 	' ',
// 	'$',
// 	'%',
// 	'*',
// 	'+',
// 	'-',
// 	'.',
// 	'/',
// 	':'
// };

// gint qr_convert_char_to_int_alphanumeric(gchar character)
// {
// 	gint i = 0;
	
// 	for(i = 0; i < 45; i++)
// 	{
// 		if(qr_alphanumeric_converting[i] == character)
// 		{
// 			return i;
// 		}
// 	}
	
// 	return 42; // return '.'
// }

// void qr_initialize_array(gint size)
// {
// 	gint i = 0;
// 	gboolean temp = FALSE;
	
// 	qr_code = g_array_new(FALSE, TRUE, sizeof(gboolean));
	
// 	for(i = 0; i < size * size; i++)
// 	{
// 		g_array_append_val(qr_code, temp);
// 	}
	
// 	qr_code_size = size;
// }

// void qr_free_array(void)
// {
// 	g_array_free(qr_code, TRUE);
// }

// gboolean qr_get_pixel(gint x, gint y)
// {
// 	if(qr_code == NULL)
// 	{
// 		return FALSE;
// 	}
	
// 	return g_array_index(qr_code, gboolean, y * qr_code_size + x);
// }

// void qr_set_pixel(gint x, gint y, gboolean value)
// {
// 	g_array_index(qr_code, gboolean, y * qr_code_size + x) = value;
// }

// void qr_render_code(gchar *input, error_correction_t error_correction)
// {
// 	guint i = 0;
// 	gint j = 0;
// 	encoding_mode_t encoding_mode = NUMERIC;
// 	gint qr_version = 0;
// 	gint qr_size = 0;
// 	gint error_correction_offset = 0;
// 	gint encoding_mode_offset = 0;
// 	GArray *encoded_data = g_array_new(FALSE, TRUE, sizeof(gboolean));
// 	guint input_size = 0;
// 	gboolean pseudo_true = TRUE;
// 	gboolean pseudo_false = FALSE;
// 	gboolean temp_bit = FALSE;
// 	guint temp_bits = 0;
	
// 	if(input == NULL)
// 	{
// 		return;
// 	}
	
// 	input_size = strlen(input);
	
// 	// test for numeric
// 	for(i = 0; i < input_size; i++)
// 	{
// 		if(!(input[i] == '0' ||
// 			input[i] == '1' ||
// 			input[i] == '2' ||
// 			input[i] == '3' ||
// 			input[i] == '4' ||
// 			input[i] == '5' ||
// 			input[i] == '6' ||
// 			input[i] == '7' ||
// 			input[i] == '8' ||
// 			input[i] == '9'))
// 		{
// 			encoding_mode = ALPHANUMERIC;
// 			break;
// 		}
// 	}
	
// 	if(encoding_mode != NUMERIC)
// 	{
// 		// printf("Numeric does not match\n");
		
// 		// test for alphanumeric
// 		for(i = 0; i < input_size; i++)
// 		{
// 			if(!(input[i] == '0' ||
// 				input[i] == '1' ||
// 				input[i] == '2' ||
// 				input[i] == '3' ||
// 				input[i] == '4' ||
// 				input[i] == '5' ||
// 				input[i] == '6' ||
// 				input[i] == '7' ||
// 				input[i] == '8' ||
// 				input[i] == '9' ||
// 				input[i] == 'A' ||
// 				input[i] == 'B' ||
// 				input[i] == 'C' ||
// 				input[i] == 'D' ||
// 				input[i] == 'E' ||
// 				input[i] == 'F' ||
// 				input[i] == 'G' ||
// 				input[i] == 'H' ||
// 				input[i] == 'I' ||
// 				input[i] == 'J' ||
// 				input[i] == 'K' ||
// 				input[i] == 'L' ||
// 				input[i] == 'M' ||
// 				input[i] == 'N' ||
// 				input[i] == 'O' ||
// 				input[i] == 'P' ||
// 				input[i] == 'Q' ||
// 				input[i] == 'R' ||
// 				input[i] == 'S' ||
// 				input[i] == 'T' ||
// 				input[i] == 'U' ||
// 				input[i] == 'V' ||
// 				input[i] == 'W' ||
// 				input[i] == 'X' ||
// 				input[i] == 'Y' ||
// 				input[i] == 'Z' ||
// 				input[i] == ' ' ||
// 				input[i] == '$' ||
// 				input[i] == '%' ||
// 				input[i] == '*' ||
// 				input[i] == '+' ||
// 				input[i] == '-' ||
// 				input[i] == '.' ||
// 				input[i] == '/' ||
// 				input[i] == ':'))
// 			{
// 				encoding_mode = BYTE;
// 				break;
// 			}
// 		}
		
// 		// if(encoding_mode != ALPHANUMERIC)
// 		// {
// 		// 	printf("Alphanumeric does not match\n");
// 		// }
// 	}
	
// 	switch(encoding_mode)
// 	{
// 		case NUMERIC:
// 		{
// 			encoding_mode_offset = 0;
// 			printf("Numeric encoding\n");
// 			break;
// 		}
// 		case ALPHANUMERIC:
// 		{
// 			encoding_mode_offset = 1;
// 			printf("Alphanumeric encoding\n");
// 			break;
// 		}
// 		case BYTE:
// 		{
// 			encoding_mode_offset = 2;
// 			printf("Byte encoding\n");
// 			break;
// 		}
// 		case KANJI:
// 		{
// 			encoding_mode_offset = 3;
// 			printf("Kanji encoding\n");
// 			break;
// 		}
// 	}
	
// 	switch(error_correction)
// 	{
// 		case ERR_CORR_L:
// 		{
// 			error_correction_offset = 0;
// 			break;
// 		}
// 		case ERR_CORR_M:
// 		{
// 			error_correction_offset = 4;
// 			break;
// 		}
// 		case ERR_CORR_Q:
// 		{
// 			error_correction_offset = 8;
// 			break;
// 		}
// 		case ERR_CORR_H:
// 		{
// 			error_correction_offset = 12;
// 			break;
// 		}
// 	}
	
// 	for(qr_version = 1; qr_version <= 40; qr_version++)
// 	{
// 		if(input_size <= qr_character_amount[(qr_version - 1) * 16 + error_correction_offset + encoding_mode_offset])
// 		{
// 			break;
// 		}
// 	}
	
// 	printf("ver %i, err %i, enc %i, id %i: %i\n", qr_version, error_correction_offset, encoding_mode_offset, (qr_version - 1) * 16 + error_correction_offset + encoding_mode_offset, qr_character_amount[(qr_version - 1) * 16 + error_correction_offset + encoding_mode_offset]);
	
// 	qr_size = 21 + (qr_version - 1) * 4;
	
// 	printf("QR Code size: %i\n", qr_size);
	
// 	switch(encoding_mode)
// 	{
// 		case NUMERIC:
// 		{
// 			printf("Mode indicator: 0001\n");
			
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_true);
			
// 			break;
// 		}
// 		case ALPHANUMERIC:
// 		{
// 			printf("Mode indicator: 0010\n");
			
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_true);
// 			g_array_append_val(encoded_data, pseudo_false);
			
// 			break;
// 		}
// 		case BYTE:
// 		{
// 			printf("Mode indicator: 0100\n");
			
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_true);
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_false);
			
// 			break;
// 		}
// 		case KANJI:
// 		{
// 			printf("Mode indicator: 1000\n");
			
// 			g_array_append_val(encoded_data, pseudo_true);
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_false);
// 			g_array_append_val(encoded_data, pseudo_false);
			
// 			break;
// 		}
// 	}
	
// 	// printf("Number = %i\n", input_size);
	
// 	// for(j = 8; j >= 0; j--)
// 	// {
// 	// 	printf("%i: %i\n", j, NTH_BIT(input_size, j));
// 	// }
	
// 	if(qr_version > 0 && qr_version < 10)
// 	{
// 		switch(encoding_mode)
// 		{
// 			case NUMERIC:
// 			{
// 				printf("Amount of bits: 10\n");
				
// 				for(j = 9; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case ALPHANUMERIC:
// 			{
// 				printf("Amount of bits: 9\n");
				
// 				for(j = 8; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case BYTE:
// 			{
// 				printf("Amount of bits: 8\n");
				
// 				for(j = 7; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case KANJI:
// 			{
// 				printf("Amount of bits: 8\n");
				
// 				for(j = 7; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 		}
// 	}
// 	else if(qr_version > 9 && qr_version < 27)
// 	{
// 		switch(encoding_mode)
// 		{
// 			case NUMERIC:
// 			{
// 				printf("Amount of bits: 12\n");
				
// 				for(j = 11; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case ALPHANUMERIC:
// 			{
// 				printf("Amount of bits: 11\n");
				
// 				for(j = 10; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case BYTE:
// 			{
// 				printf("Amount of bits: 16\n");
				
// 				for(j = 15; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case KANJI:
// 			{
// 				printf("Amount of bits: 10\n");
				
// 				for(j = 9; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 		}
// 	}
// 	else if(qr_version > 26 && qr_version < 41)
// 	{
// 		switch(encoding_mode)
// 		{
// 			case NUMERIC:
// 			{
// 				printf("Amount of bits: 14\n");
				
// 				for(j = 13; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case ALPHANUMERIC:
// 			{
// 				printf("Amount of bits: 13\n");
				
// 				for(j = 12; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case BYTE:
// 			{
// 				printf("Amount of bits: 16\n");
				
// 				for(j = 15; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 			case KANJI:
// 			{
// 				printf("Amount of bits: 12\n");
				
// 				for(j = 11; j >= 0; j--)
// 				{
// 					temp_bit = NTH_BIT(input_size, j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
				
// 				break;
// 			}
// 		}
// 	}
	
// 	switch(encoding_mode)
// 	{
// 		case NUMERIC:
// 		{
// 			for(i = 0; i < input_size; i += 3)
// 			{
// 				if(i + 2 == input_size) // only two numbers
// 				{
// 					printf("2: %c%c\n", input[i], input[i + 1]);
// 				}
// 				else if(i + 1 == input_size) // only one number
// 				{
// 					printf("1: %c\n", input[i]);
// 				}
// 				else // three numbers
// 				{
// 					printf("3: %c%c%c\n", input[i], input[i + 1], input[i + 2]);
					
// 					temp_bits = (input[i] - '0') * 100 + (input[i + 1] - '0') * 10 + (input[i + 2] - '0');
					
// 					printf("-> %i\n", temp_bits);
					
// 					for(j = 9; j >= 0; j--)
// 					{
// 						temp_bit = NTH_BIT(temp_bits, j);
// 						printf("%i\n", temp_bit);// g_array_append_val(encoded_data, temp_bit);
// 					}
// 				}
// 			}
			
// 			break;
// 		}
// 		case ALPHANUMERIC:
// 		{
// 			for(i = 0; i < input_size; i += 2)
// 			{
// 				if(i + 1 == input_size) // odd number of characters
// 				{
// 					temp_bits = qr_convert_char_to_int_alphanumeric(input[i]);
					
// 					for(j = 5; j >= 0; j--)
// 					{
// 						temp_bit = NTH_BIT(temp_bits, j);
// 						g_array_append_val(encoded_data, temp_bit);
// 					}
// 				}
// 				else // even number of characters
// 				{
// 					temp_bits = (qr_convert_char_to_int_alphanumeric(input[i]) * 45) + qr_convert_char_to_int_alphanumeric(input[i + 1]);
					
// 					for(j = 10; j >= 0; j--)
// 					{
// 						temp_bit = NTH_BIT(temp_bits, j);
// 						g_array_append_val(encoded_data, temp_bit);
// 					}
// 				}
// 			}
			
// 			break;
// 		}
// 		case BYTE:
// 		{
// 			for(i = 0; i < input_size; i++)
// 			{
// 				for(j = 0; j < 8; j++)
// 				{
// 					temp_bit = NTH_BIT(input[i], j);
// 					g_array_append_val(encoded_data, temp_bit);
// 				}
// 			}
			
// 			break;
// 		}
// 	}
	
// 	printf("Encoded data:\n");
	
// 	for(i = 0; i < encoded_data->len; i++)
// 	{
// 		printf("%i", g_array_index(encoded_data, gboolean, i));
// 	}
	
// 	printf("\n");
	
// 	g_array_free(encoded_data, TRUE);
// }