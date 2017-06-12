#ifndef W5500_H
#define	W5500_H

void InitializeW5500();
void ResetW5500();
void SetW5500SS();
void ResetW5500SS();

void WriteToW5500(uint16_t addr, uint8_t controlBit, uint8_t* buf, uint16_t len);
void ReadFromW5500(uint16_t addr, uint8_t controlBit, uint8_t* buf, uint16_t len);

void WriteSubnetMask(uint8_t* buf);
void ReadSubnetMask(uint8_t* buf);

void WriteMacAddress(uint8_t* buf);
void ReadMacAddress(uint8_t* buf);

void WriteSourceIP(uint8_t* buf);
void ReadSourceIP(uint8_t* buf);

void WriteDefaultGateway(uint8_t* buf);
void ReadDefaultGateway(uint8_t* buf);

void SetSocketRXSize(uint8_t socket, uint8_t size);
void SetSocketTXSize(uint8_t socket, uint8_t size);

void SetSingleByteW5500(uint16_t addr, uint8_t value);
uint8_t ReadSingleByteW5500(uint16_t addr);

void SetMR(uint8_t newVal);
uint8_t ReadMR();

uint8_t ReadPHYCFGR();




void WriteDestinationIP(uint8_t socket, uint8_t* buf);
void WriteDestinationPort(uint8_t socket, uint8_t* buf);
void WriteSocketCommand(uint8_t socket, uint8_t value);
uint8_t ReadSocketStatus(uint8_t socket);

void ConnectToIP(uint8_t* IP, uint8_t* port);

void W5500SoftwareReset();

uint8_t TestW5500RegisterWriting();

#endif	/* W5500_H */
