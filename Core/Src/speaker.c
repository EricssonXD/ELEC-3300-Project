#include "speaker.h"
//#include "datas.h"

#define	WM8978_ADDRESS				0x1A
#define	WM8978_WIRTE_ADDRESS		(WM8978_ADDRESS << 1 | 0)
#define	BUFFER_SIZE					2048

HAL_StatusTypeDef WM8978_Register_Wirter(uint8_t reg_addr, uint16_t data)
{
	uint8_t pData[10] = {0};

	pData[0] = (reg_addr << 1) | ((data >> 8) & 0x01);
	pData[1] = data & 0xFF;
	return HAL_I2C_Master_Transmit(&hi2c1, WM8978_WIRTE_ADDRESS, pData, 2, 1000);
}

void WM8978_Init(void)
{
	WM8978_Register_Wirter(0, 0);		// 软复位
	WM8978_Register_Wirter(1, 0x0F);	// 模拟放大器使能， 使能输出输入缓存区
	WM8978_Register_Wirter(3, 0x7F);	// 使能左右声道和L\ROUT2
	WM8978_Register_Wirter(4, 0x10);	// I2S 16bit
	WM8978_Register_Wirter(6,0);		// MCU提供时钟
	WM8978_Register_Wirter(10, 0x08);	// 输出音质最好
	WM8978_Register_Wirter(43, 0x10);	// ROUT2反相
	WM8978_Register_Wirter(54,30);		// 设置LOUT2左声道音量
	WM8978_Register_Wirter(55,30|(1<<8));	// 设置ROUT2右声道音量， 更新左右声道音量
}

void WM8978_Play(uint16_t *data)
{
	uint32_t DataLength = 0;
	uint8_t* DataAddress = NULL;
	uint16_t* TempAddress = NULL;

	DataLength = sizeof(data) - 0x2c;
	DataAddress = (unsigned char *)(data + 0x2c);
	TempAddress = (uint16_t*)DataAddress;

	while(1)
	{
		HAL_I2S_Transmit(&hi2s2, TempAddress, BUFFER_SIZE / 2, 1000);
		DataLength -= BUFFER_SIZE;
		TempAddress += (BUFFER_SIZE / 2);
		if(DataLength < BUFFER_SIZE) break;
	}
}


void WM8978_PlayChar(unsigned char *data)
{
    uint32_t DataLength = 0;
    uint8_t* DataAddress = NULL;
    uint16_t* TempAddress = NULL;

    // Adjusting DataLength calculation for char
    DataLength = sizeof(data) - 0x2c; // Assuming sizeof(char) == 1, adjust if not
    DataAddress = (unsigned char *)(data + 0x2c); // No change needed here
    TempAddress = (uint16_t*)DataAddress; // Cast to uint16_t* as before

    while(1)
    {
        // Transmitting data in chunks of BUFFER_SIZE / 2
        HAL_I2S_Transmit(&hi2s2, TempAddress, BUFFER_SIZE / 2, 1000);
        DataLength -= BUFFER_SIZE;
        TempAddress += (BUFFER_SIZE / 2);
        if(DataLength < BUFFER_SIZE) break;
    }
}

