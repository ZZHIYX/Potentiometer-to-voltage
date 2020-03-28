/* This files provides address values that exist in the system */
#define BOARD                 "DE10-Standard"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_PIXEL_BUF_BASE   0xC8000000
#define FPGA_PIXEL_BUF_END    0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LED_BASE	      0xFF200000
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define TIMER_2_BASE          0xFF202020
#define AV_CONFIG_BASE        0xFF203000
#define RGB_RESAMPLER_BASE    0xFF203010
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define EDGE_DETECT_CTRL_BASE 0xFF203070
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO0_BASE        0xFF708000
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_GPIO2_BASE        0xFF70A000
#define I2C0_BASE             0xFFC04000
#define I2C1_BASE             0xFFC05000
#define I2C2_BASE             0xFFC06000
#define I2C3_BASE             0xFFC07000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define HPS_RSTMGR	      0xFFD05000
#define HPS_RSTMGR_PREMODRST  0xFFD05014
#define FPGA_BRIDGE           0xFFD0501C

#define PIN_MUX		      0xFFD08400
#define CLK_MGR		      0xFFD04000

#define SPIM0_BASE	      0xFFF00000
#define SPIM0_SR	      0xFFF00028
#define SPIM0_DR	      0xFFF00060
/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs


int main(void) {
	//Initialization

	volatile int* ADC_Port = (int*)ADC_BASE; //Set the pointer for the input
	volatile int* GPIO_Port = (int*)JP1_BASE;//Set the pointer for the ouput
	volatile int* Switch = (int*)SW_BASE; //Set the pointer for the switch

	*(GPIO_Port + 1) |= 0x03ff;// Set pin0-9 of the GPIO to be the ouputs(...1111111111)
	*(ADC_Port + 1)=0;// Turn off the continuously updating for all channels( turn off R bit)

	int inputval = 0;

	while (1) {
		//Mode 1: Switch->0, read from channel 0 
		if (!(*Switch) & 0x01) {
			*(ADC_Port) |= 0x8000;//update the channel 0
			inputval = ((*ADC_Port) & 0Xfff);//read 12bits
		}
		//	Mode 2: Switch->1, read from channel 1 
		else if ((*Switch) & 0x01) {
			*(ADC_Port) |= 0x8000;//update all channels
			inputval = ((*(ADC_Port + 1)) & 0xfff);
		}

		//Display
		//As when ADC value=2048, 5 pins on; ADC value = 4096, all pins on. It can be concluded that one pin is corresponded for around 409.

		(*GPIO_Port) &= 0xfffffc00;// clear the previous value for GPIO output

		int output = inputval / 409;
	
		switch (output) {
		case 0:
			*GPIO_Port |= 0x0;
			break;
		case 1:
			*GPIO_Port |= 0x1;
			break;
		case 2:
			*GPIO_Port |= 0x3;
			break;
		case 3:
			*GPIO_Port |= 0x7;
			break;
		case 4:
			*GPIO_Port |= 0xf;
			break;
		case 5:
			*GPIO_Port |= 0x1f;
			break;
		case 6:
			*GPIO_Port |= 0x3f;
			break;
		case 7:
			*GPIO_Port |= 0x7f;
			break;
		case 8:
			*GPIO_Port |= 0xff;
			break;
		case 9:
			*GPIO_Port |= 0x1ff;
			break;
		case 10:
			*GPIO_Port |= 0x3ff;
			break;
		default:
			*GPIO_Port |= 0x0;
			break;
		}

	}
}
