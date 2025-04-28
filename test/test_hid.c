#include <hid/hid.h>

// static void hid_test(void);

int main(void) {
    rcc_clock_setup_pll(
        &rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // set system clock to 72MHz using 8MHz HSE
    hid_init();

    for (;;) {
        hid_test();
        delay(25);
    }
    return 0;
}

static void hid_test(void) {
    static float       theta  = 0.0f;
    static const float step   = 0.1f;
    static const float radius = 10.0f;
    uint8_t            buf[4] = {0, 0, 0, 0};

    int8_t dx = (int8_t) (radius * cosf(theta));
    int8_t dy = (int8_t) (radius * sinf(theta));

    buf[1] = (uint8_t) dx;
    buf[2] = (uint8_t) dy;

    theta += step;
    if (theta > 2.0f * M_PI) {
        // theta -= 2.0f * M_PI;  // Açıyı sıfırla
    }

    usbd_ep_write_packet(usbd_dev, 0x81, buf, 4);
}
