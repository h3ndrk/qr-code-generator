typedef enum { NUMERIC, ALPHANUMERIC, BYTE, KANJI } encoding_mode_t;

void qr_initialize_array(gint size);
void qr_free_array(void);
gboolean qr_get_pixel(gint x, gint y);
void qr_set_pixel(gint x, gint y, gboolean value);
void qr_render_code(gchar *input);