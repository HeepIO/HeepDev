#include "mcc_generated_files/mcc.h"
#include "W5500.h"

// Gateway Address (GAR) - 0x0001->0x0004
#define GAR0 0x0001
// Subnet Mask Address (SUBR) - 0x0005->0x0008
#define SUBR0 0x0005
// Source Hardware Address [MAC Address] (SHAR) - 0x0009->0x000E
#define SHAR0 0x0009
// Source IP Address (SIPR) - 0x000F->0x0012
#define SIPR 0x000F

#define SS_PIN_TRIS TRISCbits.TRISC0
#define SS_PIN LATCbits.LATC0
#define RESET_PIN_TRIS TRISCbits.TRISC2
#define RESET_PIN LATCbits.LATC2
#define INT_PIN_TRIS TRISCbits.TRISC1
#define INT_PIN LATCbits.LATC1

void InitializeW5500()
{
    SS_PIN_TRIS = 0;
    RESET_PIN_TRIS = 0;
    INT_PIN_TRIS = 1;
    
    SS_PIN = 1;
    RESET_PIN = 1;
    
    ResetW5500();
}

void ResetW5500()
{   
    RESET_PIN = 0;
    
    for(unsigned long i = 0; i < 65535; i++)
    {
        Nop();
    }
    
    RESET_PIN = 1;
        
    for(unsigned long i = 0; i < 65535; i++)
    {
        Nop();
    }
}

void SetW5500SS()
{
    SS_PIN = 0;
}

void ResetW5500SS()
{
    SS_PIN = 1;
}


/* 
 * SPI on W5500 works in 3 phases.
 * 1. Address Phase
 * 2. Control Phase
 * 3. Data Phase
 */
void WriteToW5500(uint16_t addr, uint8_t controlBit, uint8_t* buf, uint16_t len)
{
    SetW5500SS();
    
    SPI1_Exchange8bit(addr >> 8); // Get High Byte
    SPI1_Exchange8bit(addr & 0xff); // Get Low Byte
    SPI1_Exchange8bit(controlBit);
    
    uint16_t i = 0;
    for(i = 0; i < len; i++)
    {
        SPI1_Exchange8bit(buf[i]);
    }
    
    ResetW5500SS();
}

void ReadFromW5500(uint16_t addr, uint8_t controlBit, uint8_t* buf, uint16_t len)
{
    SetW5500SS();
    
    SPI1_Exchange8bit(addr >> 8); // Get High Byte
    SPI1_Exchange8bit(addr & 0xff); // Get Low Byte
    SPI1_Exchange8bit(controlBit);
    
    uint16_t i = 0;
    for(i = 0; i < len; i++)
    {
        buf[i] = SPI1_Exchange8bit(0);
    }
    
    ResetW5500SS();
}

void WriteSubnetMask(uint8_t* buf)
{
    WriteToW5500(SUBR0, 0b00000100, buf, 4);
}

void ReadSubnetMask(uint8_t* buf)
{
    ReadFromW5500(SUBR0, 0x00, buf, 4);
}

void WriteMacAddress(uint8_t* buf)
{
    WriteToW5500(SHAR0, 0b00000100, buf, 6);
}

void ReadMacAddress(uint8_t* buf)
{
    ReadFromW5500(SHAR0, 0x00, buf, 6);
}

void WriteSourceIP(uint8_t* buf)
{
    WriteToW5500(SIPR, 0b00000100, buf, 4);
}

void ReadSourceIP(uint8_t* buf)
{
    ReadFromW5500(SIPR, 0x00, buf, 4);
}

void WriteDefaultGateway(uint8_t* buf)
{
    WriteToW5500(GAR0, 0b00000100, buf, 4);
}

void ReadDefaultGateway(uint8_t* buf)
{
    ReadFromW5500(GAR0, 0x00, buf, 4);
}

uint8_t AreBufsEqual(uint8_t* buf1, uint8_t* buf2, uint8_t len)
{
    for(int i = 0; i < len; i++)
    {
        if(buf1[i] != buf2[i])
        {
            return 0;
        }
    }
    
    return 1;
}

uint8_t TestW5500RegisterWriting()
{
    uint8_t writeSubBuff [4] = {255,255,14,0};
    uint8_t readSubBuf [4];
    
    uint8_t writeMACBuf [6] = {123, 231, 222, 102, 111, 4};
    uint8_t readMACBuf[6];
    
    uint8_t writeSourceIPBuf [4] = {52,25,64,11};
    uint8_t readSourceIPBuf [4];
    
    uint8_t writeGatewayBuf [4] = {57,65,61,42};
    uint8_t readGatewayBuf [4];
    
    uint8_t success = 0;
    uint8_t numTests = 0;
    
    WriteSubnetMask(writeSubBuff);
    ReadSubnetMask(readSubBuf);
    
    WriteMacAddress(writeMACBuf);
    ReadMacAddress(readMACBuf);
    
    WriteSourceIP(writeSourceIPBuf);
    ReadSourceIP(readSourceIPBuf);
    
    WriteDefaultGateway(writeGatewayBuf);
    ReadDefaultGateway(readGatewayBuf);
    
    success += AreBufsEqual(writeSubBuff, readSubBuf, 4);
    numTests++;
    success += AreBufsEqual(writeMACBuf, readMACBuf, 6);
    numTests++;
    success += AreBufsEqual(writeSourceIPBuf, readSourceIPBuf, 4);
    numTests++;
    success += AreBufsEqual(writeGatewayBuf, readGatewayBuf, 4);
    numTests++;
    
    if(numTests == success)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}