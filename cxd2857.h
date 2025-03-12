#ifndef _CXD2878_H_
#define _CXD2878_H_

enum sony_demod_xtal_t {
	SONY_DEMOD_XTAL_16000KHz = 0, /**< 16 MHz */
	SONY_DEMOD_XTAL_24000KHz = 1, /**< 24 MHz */
	SONY_DEMOD_XTAL_32000KHz = 2 /**< 32 MHz */
};
enum sony_ascot3_xtal_t {
	SONY_ASCOT3_XTAL_16000KHz, /**< 16 MHz */
	SONY_ASCOT3_XTAL_20500KHz, /**< 20.5 MHz */
	SONY_ASCOT3_XTAL_24000KHz, /**< 24 MHz */
	SONY_ASCOT3_XTAL_41000KHz /**< 41 MHz */
};

#define SONY_DEMOD_MAKE_IFFREQ_CONFIG(iffreq) \
	((u32)(((iffreq) / 48.0) * 16777216.0 + 0.5))
#define SONY_DEMOD_ATSC_MAKE_IFFREQ_CONFIG(iffreq) \
	((u32)(((iffreq) / 24.0) * 4294967296.0 + 0.5))

#define MASKUPPER(n) (((n) == 0) ? 0 : (0xFFFFFFFFU << (32 - (n))))
#define MASKLOWER(n) (((n) == 0) ? 0 : (0xFFFFFFFFU >> (32 - (n))))

struct cxd2857_config {
	u8 addr_slvt;
	enum sony_demod_xtal_t xtal;
	u8 tuner_addr;
	enum sony_ascot3_xtal_t tuner_xtal;
	u8 tuner_index;
	bool tlv_mode;
	//for ecp3 update
	void (*write_properties)(struct i2c_adapter *i2c, u8 reg, u32 buf);
	void (*read_properties)(struct i2c_adapter *i2c, u8 reg, u32 *buf);
	// EEPROM access
	void (*write_eeprom)(struct i2c_adapter *i2c, u8 reg, u8 buf);
	void (*read_eeprom)(struct i2c_adapter *i2c, u8 reg, u8 *buf);

	//for 6590SE mode change(T or s);
	void (*RF_switch)(struct i2c_adapter *i2c, u8 rf_in, u8 flag);
	u8 rf_port; //for change command
	void (*TS_switch)(struct i2c_adapter *i2c, u8 flag); //5590
	void (*LED_switch)(struct i2c_adapter *i2c, u8 flag); //5590
};

struct stream_info {
    u16 stream_id;
    u32 stream_type;
};

struct tmcc_stream_infos {
    struct stream_info stream_info[16];
};

#define CONFIG_DVB_CXD2878_MODULE
#define CONFIG_DVB_CXD2878
#if 1
extern struct dvb_frontend *cxd2857_attach(const struct cxd2857_config *config,
					   struct i2c_adapter *i2c);
#else
static inline struct dvb_frontend *
cxd2857_attach(const struct cxd2857_config *config, struct i2c_adapter *i2c)
{
	dev_warn(&i2c->dev, "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}

#endif

#endif
