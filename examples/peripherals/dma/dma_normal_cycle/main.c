#include "bflb_mtimer.h"
#include "bflb_dma.h"

#define DMA_BUFFER_LENGTH 4100

extern void board_init(void);

static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t src1_buffer[DMA_BUFFER_LENGTH];
static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t src2_buffer[DMA_BUFFER_LENGTH];
static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t src3_buffer[DMA_BUFFER_LENGTH];

static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t dst1_buffer[DMA_BUFFER_LENGTH];
static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t dst2_buffer[DMA_BUFFER_LENGTH];
static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t dst3_buffer[DMA_BUFFER_LENGTH];
static uint8_t dma_tc_flag0 = 0;

struct bflb_device_s *dma0_ch0;

void dma0_ch0_isr(void *arg)
{
    dma_tc_flag0++;
    printf("tc done\r\n");
    if (dma_tc_flag0 == 100) {
        bflb_dma_channel_stop(dma0_ch0);
    }
}

void sram_init()
{
    uint32_t i;

    for (i = 0; i < DMA_BUFFER_LENGTH; i++) {
        src1_buffer[i] = i & 0xff;
        src2_buffer[i] = (i * 0x07) & 0xff;
        src3_buffer[i] = (i * 0x0b) & 0xff;
    }

    memset(dst1_buffer, 0, DMA_BUFFER_LENGTH);
    memset(dst2_buffer, 0, DMA_BUFFER_LENGTH);
    memset(dst3_buffer, 0, DMA_BUFFER_LENGTH);
}

int main(void)
{
    uint32_t i;

    board_init();

    sram_init();

    printf("dma memory with cycle mode case\r\n");
    dma0_ch0 = bflb_device_get_by_name("dma0_ch0");

    struct bflb_dma_channel_config_s config;

    config.direction = DMA_MEMORY_TO_MEMORY;
    config.src_req = 0;
    config.dst_req = 0;
    config.src_addr_inc = DMA_ADDR_INCREMENT_ENABLE;
    config.dst_addr_inc = DMA_ADDR_INCREMENT_ENABLE;
    config.src_burst_count = DMA_BURST_INCR1;
    config.dst_burst_count = DMA_BURST_INCR1;
    config.src_width = DMA_DATA_WIDTH_32BIT;
    config.dst_width = DMA_DATA_WIDTH_32BIT;
    bflb_dma_channel_init(dma0_ch0, &config);

    bflb_dma_channel_irq_attach(dma0_ch0, dma0_ch0_isr, NULL);

    struct bflb_dma_channel_lli_pool_s lli[20]; /* max trasnfer size 4064 * 20 */
    struct bflb_dma_channel_lli_transfer_s transfers[3];

    transfers[0].src_addr = (uint32_t)src1_buffer;
    transfers[0].dst_addr = (uint32_t)dst1_buffer;
    transfers[0].nbytes = DMA_BUFFER_LENGTH;

    transfers[1].src_addr = (uint32_t)src2_buffer;
    transfers[1].dst_addr = (uint32_t)dst2_buffer;
    transfers[1].nbytes = DMA_BUFFER_LENGTH;

    transfers[2].src_addr = (uint32_t)src3_buffer;
    transfers[2].dst_addr = (uint32_t)dst3_buffer;
    transfers[2].nbytes = DMA_BUFFER_LENGTH;

    int used_count = bflb_dma_channel_lli_reload(dma0_ch0, lli, 20, transfers, 3);
    bflb_dma_channel_lli_link_head(dma0_ch0, lli, used_count);
    bflb_dma_channel_start(dma0_ch0);
    while (dma_tc_flag0 < 100) {
        bflb_mtimer_delay_ms(1);
    }

    /* Check data */
    for (i = 0; i < DMA_BUFFER_LENGTH; i++) {
        if (src1_buffer[i] != dst1_buffer[i]) {
            printf("Error! index: %ld, src1: 0x%02x, dst1: 0x%02x\r\n", i, src1_buffer[i], dst1_buffer[i]);
        }
        if (src2_buffer[i] != dst2_buffer[i]) {
            printf("Error! index: %ld, src2: 0x%02x, dst2: 0x%02x\r\n", i, src2_buffer[i], dst2_buffer[i]);
        }
        if (src3_buffer[i] != dst3_buffer[i]) {
            printf("Error! index: %ld, src3: 0x%02x, dst3: 0x%02x\r\n", i, src3_buffer[i], dst3_buffer[i]);
        }
    }

    printf("end\r\n");
    while (1) {
    }
}
