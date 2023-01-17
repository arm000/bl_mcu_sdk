#include "bflb_mtimer.h"
#include "board.h"

int main(void)
{
    board_init();

    printf("starting timer test...\r\n");

    while (1) {
        uint64_t time = bflb_mtimer_get_time_us();
        uint64_t s, us;
        s = time / (1000 * 1000);
        us = time % (1000 * 1000);
        printf("time is %llu.%06llu\r\n", s, us);
        bflb_mtimer_delay_ms(500);
    }
}
