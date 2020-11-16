#include <rtls/rtls/rtls.h>

#include <rtls_tdma/rtls_tdma.h>

#if __GNUC__ >= 9
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#endif
#include <message/mavlink/protocol/mavlink.h>

#include <uwb/uwb.h>
#include <uwb_rng/uwb_rng.h>

float g_location[3] = {0.2, 123.456, 567};
uint16_t g_address = 0x1234;
uint16_t g_ntype = ANCHOR;

struct uwb_dev *udev;
void rtls_get_location(float *x, float *y, float *z){
    *x = g_location[0];
    *y = g_location[1];
    *z = g_location[2];
}

void rtls_set_location(float x, float y, float z){
    g_location[0] = x;
    g_location[1] = y;
    g_location[2] = z;
}

void rtls_get_address(uint16_t *address){
    *address = udev->my_short_address;
}

void rtls_get_ntype(uint8_t *ntype){
    *ntype = g_ntype;
}

void rtls_tdma_cb(rtls_tdma_instance_t *rtls_tdma_instance, tdma_slot_t *slot){
    
    // static uint16_t timeout = 0;
    // struct uwb_rng_instance *rng = rtls_tdma_instance->uri;
    // struct uwb_dev *inst = rtls_tdma_instance->dev_inst;
    // uint16_t idx = slot->idx;

    // if (!timeout) {
    //     timeout = uwb_usecs_to_dwt_usecs(uwb_phy_frame_duration(inst, sizeof(ieee_rng_request_frame_t)))
    //         + rng->config.rx_timeout_delay;
    //     printf("# timeout set to: %d %d = %d\n",
    //            uwb_phy_frame_duration(inst, sizeof(ieee_rng_request_frame_t)),
    //            rng->config.rx_timeout_delay, timeout);
    // }
    // tdma_instance_t *tdma = slot->parent;

    // uwb_rng_listen_delay_start(rng, tdma_rx_slot_start(tdma, idx), timeout, UWB_BLOCKING);
}

rtls_tdma_instance_t g_rtls_tdma_instance = {
    .rtls_tdma_cb = rtls_tdma_cb
};
struct uwb_mac_interface g_cbs;

static bool
complete_cb(struct uwb_dev * inst, struct uwb_mac_interface * cbs)
{
    if (inst->fctrl != FCNTL_IEEE_RANGE_16 &&
        inst->fctrl != (FCNTL_IEEE_RANGE_16|UWB_FCTRL_ACK_REQUESTED)) {
        return false;
    }
    return true;
}

void rtls_init(){
    udev = uwb_dev_idx_lookup(0);
    uwb_set_dblrxbuff(udev, false);

    g_rtls_tdma_instance.uri  = (struct uwb_rng_instance *)uwb_mac_find_cb_inst_ptr(udev, UWBEXT_RNG);

    g_cbs = (struct uwb_mac_interface){
        .id =  UWBEXT_APP0,
        .inst_ptr = g_rtls_tdma_instance.uri,
        .complete_cb = complete_cb,
    };
    uwb_mac_append_interface(udev, &g_cbs);

    rtls_tdma_start(&g_rtls_tdma_instance, udev);
}