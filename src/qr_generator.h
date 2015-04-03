// #define NTH_BIT(n, k) ((n & (1 << k)) >> k)

// typedef enum { NUMERIC, ALPHANUMERIC, BYTE, KANJI } encoding_mode_t;
// typedef enum { ERR_CORR_L, ERR_CORR_M, ERR_CORR_Q, ERR_CORR_H } error_correction_t;

// gint qr_convert_char_to_int_alphanumeric(gchar character);
// void qr_initialize_array(gint size);
// void qr_free_array(void);
// gboolean qr_get_pixel(gint x, gint y);
// void qr_set_pixel(gint x, gint y, gboolean value);
// void qr_render_code(gchar *input, error_correction_t error_correction);

void qr_render(gchar *input);
void qr_free(void);
gint qr_get_size(void);
GArray *qr_get_pixels(void);