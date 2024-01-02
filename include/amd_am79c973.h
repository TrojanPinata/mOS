#ifndef __OS__AMD_AM79C973_H
#define __OS__AMD_AM79C973_H

#include <types.h>
#include <driver.h>
#include <pci.h>
#include <port.h>
#include <interrupts.h>

namespace os {
	class amd_am79c973;
	class RawDataHandler {
		public:
			amd_am79c973* backend;	
		public:
			RawDataHandler(amd_am79c973* backend);
			~RawDataHandler();

			virtual bool OnRawDataReceived(uint8_t* buffer, uint32_t size);
			void Send(uint8_t* buffer, uint32_t size);
	};

	class amd_am79c973 : public Driver, public InterruptHandler {
		public:	

		struct InitializationBlock {
			uint16_t mode;
			unsigned reserved1:4;
			unsigned numSendBuffers:4;
			unsigned reserved2:4;
			unsigned numRecvBuffers:4;
			uint64_t physicalAddress:48;
			uint16_t reserved3;
			uint64_t logicalAddress;
			uint32_t recvBufferDescrAddress;
			uint32_t sendBufferDescrAddress;
			} __attribute__((packed));

		struct BufferDescriptor {
			uint32_t address;
			uint32_t flags;
			uint32_t flags2;
			uint32_t avail;
		} __attribute__((packed));

		Port16Bit MACAddress0Port;
		Port16Bit MACAddress2Port;
		Port16Bit MACAddress4Port;
		Port16Bit registerDataPort;
		Port16Bit registerAddressPort;
		Port16Bit resetPort;
		Port16Bit busControlRegisterDataPort;
		InitializationBlock initBlock;

		BufferDescriptor* sendBufferDescr;
		uint8_t sendBufferDescrMemory[2048+15];
		uint8_t sendBuffers[2*1024+15][8];
		uint8_t currentSendBuffer;

		BufferDescriptor* recvBufferDescr;
		uint8_t recvBufferDescrMemory[2048+15];
		uint8_t recvBuffers[2*1024+15][8];
		uint8_t currentRecvBuffer;
			
		RawDataHandler* handler;

		public:
			amd_am79c973(PeripheralComponentInterconnectDeviceDescriptor *dev, InterruptManager* interrupts);	
			~amd_am79c973();

			void Activate();
			int Reset();
			uint32_t HandleInterrupt(uint32_t esp);
		
			void Send(uint8_t* buffer, int size);
			void Receive();
			void SetHandler(RawDataHandler* handler);
			uint64_t GetMACAddress();
				
			void SetIPAddress(uint32_t);
			uint32_t GetIPAddress();
		};
}

#endif
