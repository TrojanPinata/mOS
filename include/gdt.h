#ifndef __OS__GDT_H
#define __OS__GDT_H

#include <types.h>

namespace os {
	class GlobalDescriptorTable {

		public:
			class SegmentDescriptor {
			
				private:
					os::uint16_t limit_lo; 
					os::uint16_t base_lo;
					os::uint8_t base_hi; 
					os::uint8_t type; 
					os::uint8_t flags_limit_hi; 
					os::uint8_t base_vhi;

				public:
					SegmentDescriptor(os::uint32_t base, os::uint32_t limit, os::uint8_t type);
					
					os::uint32_t Base();
					os::uint32_t Limit();
			} __attribute__((packed));
	
		SegmentDescriptor nullSegmentSelector;
		SegmentDescriptor unusedSegmentSelector;
		SegmentDescriptor codeSegmentSelector;
		SegmentDescriptor dataSegmentSelector;
	
	public:
		GlobalDescriptorTable();
		~GlobalDescriptorTable();
	
		os::uint16_t CodeSegmentSelector();
		os::uint16_t DataSegmentSelector();
	};
}

#endif
